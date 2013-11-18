/*
 *  Dummy Shim IPC Process
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *    Miquel Tarzan         <miquel.tarzan@i2cat.net>
 *    Sander Vrijders       <sander.vrijders@intec.ugent.be>
 *    Leonardo Bergesio     <leonardo.bergesio@i2cat.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/list.h>

#define SHIM_NAME   "shim-dummy"

#define RINA_PREFIX SHIM_NAME

#include "logs.h"
#include "common.h"
#include "debug.h"
#include "utils.h"
#include "kipcm.h"
#include "ipcp-utils.h"
#include "ipcp-factories.h"
#include "du.h"
#include "kfa.h"
#include "rnl-utils.h"

/* FIXME: To be removed ABSOLUTELY */
extern struct kipcm * default_kipcm;

/* Holds all configuration related to a shim instance */
struct dummy_info {
        /* IPC Instance name */
        struct name * name;
        /* DIF name */
        struct name * dif_name;
};

struct ipcp_instance_data {
        ipc_process_id_t    id;

        /* FIXME: Stores the state of flows indexed by port_id */
        struct list_head    flows;

        /* Used to keep a list of all the dummy shims */
        struct list_head    list;

        struct dummy_info * info;

        struct list_head    apps_registered;

        /* FIXME: Remove it as soon as the kipcm_kfa gets removed*/
        struct kfa *        kfa;
};

enum dummy_flow_state {
        PORT_STATE_NULL = 1,
        PORT_STATE_RECIPIENT_ALLOCATE_PENDING,
        PORT_STATE_INITIATOR_ALLOCATE_PENDING,
        PORT_STATE_ALLOCATED
};

struct dummy_flow {
        port_id_t             port_id;
        port_id_t             dst_port_id;
        struct name *         source;
        struct name *         dest;
        struct list_head      list;
        enum dummy_flow_state state;
        ipc_process_id_t      dst_id;
        struct flow_spec *    fspec;
};

struct app_register {
        struct name *    app_name;
        struct list_head list;
};

static bool is_app_registered(struct ipcp_instance_data * data,
                              const struct name *         name)
{
        struct app_register * app;

        list_for_each_entry(app, &data->apps_registered, list) {
                if (name_is_equal(app->app_name, name)) {
                        return true;
                }
        }

        return false;
}

static struct app_register * find_app(struct ipcp_instance_data * data,
                                      const struct name *         name)
{
        struct app_register * app;

        list_for_each_entry(app, &data->apps_registered, list) {
                if (name_is_equal(app->app_name, name)) {
                        return app;
                }
        }

        return NULL;
}

static struct dummy_flow * find_flow(struct ipcp_instance_data * data,
                                     port_id_t                   id)
{
        struct dummy_flow * flow;

        list_for_each_entry(flow, &data->flows, list) {
                if (flow->port_id == id || flow->dst_port_id == id) {
                        return flow;
                }
        }

        return NULL;
}

static int dummy_flow_allocate_request(struct ipcp_instance_data * data,
                                       const struct name *         source,
                                       const struct name *         dest,
                                       const struct flow_spec *    fspec,
                                       port_id_t                   id)
{
        struct dummy_flow * flow;

        ASSERT(data);
        ASSERT(source);
        ASSERT(dest);

        if (!data->info) {
                LOG_ERR("There is not info in this IPC Process");
                return -1;
        }

        if (!data->info->dif_name) {
                LOG_ERR("This IPC Process doesn't belong to a DIF");
                return -1;
        }

        if (!is_app_registered(data, dest)) {
                char * tmp = name_tostring(dest);

                LOG_ERR("Application %s not registered to IPC process %d",
                        tmp, data->id);

                if (tmp) rkfree(tmp);

                return -1;
        }

        if (find_flow(data, id)) {
                LOG_ERR("A flow already exists on port %d", id);
                return -1;
        }

        flow = rkzalloc(sizeof(*flow), GFP_KERNEL);
        if (!flow)
                return -1;

        flow->dest    = name_dup(dest);
        if (!flow->dest) {
                rkfree(flow);
                return -1;
        }
        flow->source = name_dup(source);
        if (!flow->source) {
                name_destroy(flow->dest);
                rkfree(flow);
                return -1;
        }

        /* Only for readability reasons */
        ASSERT(flow->dest);
        ASSERT(flow->source);

        flow->state   = PORT_STATE_INITIATOR_ALLOCATE_PENDING;
        flow->port_id = id;
        flow->fspec   = flow_spec_dup(fspec);
        flow->dst_port_id = kfa_flow_create(data->kfa, data->id, false);
        ASSERT(is_port_id_ok(flow->dst_port_id));

        INIT_LIST_HEAD(&flow->list);
        list_add(&flow->list, &data->flows);

        if (kipcm_flow_arrived(default_kipcm,
                               data->id,
                               flow->dst_port_id,
                               data->info->dif_name,
                               flow->source,
                               flow->dest,
                               flow->fspec)) {
                return -1;
        }

        return 0;
}

static int dummy_flow_allocate_response(struct ipcp_instance_data * data,
                                        port_id_t                   port_id,
                                        int                         result)
{
        struct dummy_flow * flow;

        ASSERT(data);

        if (!data->info) {
                LOG_ERR("There is not info in this IPC Process");
                return -1;
        }

        if (!data->info->dif_name) {
                LOG_ERR("This IPC Process doesn't belong to a DIF");
                return -1;
        }

        flow = find_flow(data, port_id);
        if (!flow) {
                LOG_ERR("Flow does not exist, cannot allocate");
                return -1;
        }

        if (flow->state != PORT_STATE_INITIATOR_ALLOCATE_PENDING) {
                LOG_ERR("Wrong flow state");
                return -1;
        }

        /* On positive response, flow should transition to allocated state */
        if (result == 0) {
                flow->dst_port_id = port_id;
                flow->state = PORT_STATE_ALLOCATED;
                if (kipcm_flow_commit(default_kipcm,
                                      data->id, flow->port_id)) {
                        list_del(&flow->list);
                        name_destroy(flow->source);
                        name_destroy(flow->dest);
                        rkfree(flow);
                        return -1;
                }
                if (kipcm_flow_commit(default_kipcm,
                                      data->id, port_id)) {
                        kfa_flow_deallocate(data->kfa, port_id);
                        list_del(&flow->list);
                        name_destroy(flow->source);
                        name_destroy(flow->dest);
                        rkfree(flow);
                        return -1;
                }

                if (kipcm_notify_flow_alloc_req_result(default_kipcm,
                                                       data->id,
                                                       flow->port_id,
                                                       0)) {
                        kfa_flow_deallocate(data->kfa, flow->port_id);
                        kfa_flow_deallocate(data->kfa, port_id);
                        list_del(&flow->list);
                        name_destroy(flow->source);
                        name_destroy(flow->dest);
                        rkfree(flow);
                        return -1;
                }
        } else {
                list_del(&flow->list);
                name_destroy(flow->source);
                name_destroy(flow->dest);
                rkfree(flow);
                return -1;
        }

        /*
         * NOTE:
         *   Other shims may implement other behavior here,
         *   such as contacting the apposite shim IPC process
         */

        return 0;
}

static int dummy_flow_deallocate(struct ipcp_instance_data * data,
                                 port_id_t                   id)
{
        struct dummy_flow * flow;
        port_id_t dest_port_id;

        ASSERT(data);
        flow = find_flow(data, id);
        if (!flow) {
                LOG_ERR("Flow does not exist, cannot remove");
                return -1;
        }

        if (id == flow->port_id)
                dest_port_id = flow->dst_port_id;
        else
                dest_port_id = flow->port_id;

        /*
         *FIXME: dummy_flow is not updated after unbinding cause it is going
         *       to be deleted. Is it really needed to unbind+destroy?
         */

        if (kfa_flow_deallocate(data->kfa, id) ||
            kfa_flow_deallocate(data->kfa, dest_port_id)) {
                return -1;
        }

        /* Notify the destination application */
        kipcm_notify_flow_dealloc(data->id, 0, dest_port_id, 1);

        list_del(&flow->list);
        name_destroy(flow->dest);
        name_destroy(flow->source);
        rkfree(flow->fspec);
        rkfree(flow);

        return 0;
}

static int dummy_application_register(struct ipcp_instance_data * data,
                                      const struct name *         source)
{
        struct app_register * app_reg;
        char * tmp;

        ASSERT(source);

        if (!data->info) {
                LOG_ERR("There is not info in this IPC Process");
                return -1;
        }
        if (!data->info->dif_name) {
                LOG_ERR("IPC Process doesn't belong to any DIF");
                return -1;
        }

        if (is_app_registered(data, source)) {
                char * tmp = name_tostring(source);
                LOG_ERR("Application %s has been already registered", tmp);
                if (tmp) rkfree(tmp);
                return -1;
        }

        app_reg = rkzalloc(sizeof(*app_reg), GFP_KERNEL);
        if (!app_reg)
                return -1;

        app_reg->app_name = rkzalloc(sizeof(struct name), GFP_KERNEL);
        if (!app_reg->app_name) {
                rkfree(app_reg);

                return -1;
        }
        if (name_cpy(source, app_reg->app_name)) {
                char * tmp = name_tostring(source);

                LOG_ERR("Application %s registration has failed", tmp);
                if (tmp) rkfree(tmp);

                name_destroy(app_reg->app_name);
                rkfree(app_reg);

                return -1;
        }

        INIT_LIST_HEAD(&app_reg->list);
        list_add(&app_reg->list, &data->apps_registered);

        tmp = name_tostring(source);
        LOG_DBG("Application %s registered successfully", tmp);
        if (tmp) rkfree(tmp);

        return 0;
}

static int dummy_application_unregister(struct ipcp_instance_data * data,
                                        const struct name *         source)
{
        struct app_register * app;

        ASSERT(source);

        if (!is_app_registered(data, source)) {
                char * tmp = name_tostring(source);
                LOG_ERR("Application %s is not registered", tmp);
                if (tmp) rkfree(tmp);
                return -1;
        }
        app = find_app(data, source);
        list_del(&app->list);
        name_destroy(app->app_name);
        rkfree(app);

        return 0;
}

static int dummy_unregister_all(struct ipcp_instance_data * data)
{
        struct app_register * cur, *next;

        list_for_each_entry_safe(cur, next, &data->apps_registered, list) {
                list_del(&cur->list);
                name_destroy(cur->app_name);
                rkfree(cur);
        }

        return 0;
}

static struct workqueue_struct * dummy_wq = NULL;

struct write_data {
        struct kfa * kfa;
        port_id_t    port_id;
        struct sdu * sdu;
};

bool is_write_data_complete(const struct write_data * data)
{
        bool ret;

        ret = ((!data || !data->kfa || !data->sdu) ? false : true);

        LOG_DBG("Write data complete? %d", ret);

        return ret;
}

static void write_data_destroy(struct write_data * data)
{
        ASSERT(data);

        rkfree(data);
}

static struct write_data * write_data_create(struct kfa * kfa,
                                             struct sdu * sdu,
                                             port_id_t    port_id)
{
        struct write_data * tmp;

        tmp = rkmalloc(sizeof(*tmp), GFP_ATOMIC);
        if (!tmp)
                return NULL;

        tmp->kfa     = kfa;
        tmp->sdu     = sdu;
        tmp->port_id = port_id;

        return tmp;
}

static int dummy_write(void * o)
{
        struct write_data * tmp;

        tmp = (struct write_data *) o;
        if (!tmp) {
                LOG_ERR("No write data passed");
                return -1;
        }

        if (!is_write_data_complete(tmp)) {
                LOG_ERR("Wrong data passed to dummy_write");
                write_data_destroy(tmp);
                return -1;
        }

        if (kfa_sdu_post(tmp->kfa,
                         tmp->port_id,
                         tmp->sdu))
                return -1;

        write_data_destroy(tmp);

        return 0;
}

static int dummy_sdu_write(struct ipcp_instance_data * data,
                           port_id_t                   id,
                           struct sdu *                sdu)
{
        struct dummy_flow *    flow;
        struct write_data *    tmp;
        struct rwq_work_item * item;
        struct sdu *           copy_sdu;

        LOG_DBG("Dummy SDU write invoked.");

        if (!sdu)
                return -1;

        /*
         * We are going to dup the SDU since the shim has now the ownership
         * and it is always its burden to free it whenever the processing of
         * the SDU is finished (e.g. the SDU has been sent through a wire).
         * For the shim-dummy the processing consists of sending the new SDU
         * to the sdu_ready kfifo, which will take the ownership of this copy.
         */
        copy_sdu = sdu_dup_gfp(GFP_ATOMIC, sdu);
        if (!copy_sdu)
                return -1;

        sdu_destroy(sdu);

        list_for_each_entry(flow, &data->flows, list) {
                if (flow->port_id == id) {
                        tmp = write_data_create(data->kfa,
                                                copy_sdu,
                                                flow->dst_port_id);
                        if (!is_write_data_complete(tmp))
                                return -1;

                        item = rwq_work_create(GFP_ATOMIC, dummy_write, tmp);
                        if (!item) {
                                write_data_destroy(tmp);
                                return -1;
                        }

                        ASSERT(dummy_wq);

                        if (rwq_work_post(dummy_wq, item)) {
                                write_data_destroy(tmp);
                                sdu_destroy(copy_sdu);
                                return -1;
                        }

                        return 0;
                }
                if (flow->dst_port_id == id) {
                        tmp = write_data_create(data->kfa,
                                                copy_sdu,
                                                flow->port_id);
                        if (!is_write_data_complete(tmp))
                                return -1;

                        item = rwq_work_create(GFP_ATOMIC, dummy_write, tmp);
                        if (!item) {
                                write_data_destroy(tmp);
                                return -1;
                        }

                        ASSERT(dummy_wq);

                        if (rwq_work_post(dummy_wq, item)) {
                                write_data_destroy(tmp);
                                sdu_destroy(copy_sdu);
                                return -1;
                        }

                        return 0;

                }
        }
        LOG_ERR("There is no flow allocated for port-id %d", id);

        return -1;
}

static int dummy_deallocate_all(struct ipcp_instance_data * data)
{
        struct dummy_flow * pos, * next;

        list_for_each_entry_safe(pos, next, &data->flows, list) {
                list_del(&pos->list);
                name_destroy(pos->dest);
                name_destroy(pos->source);
                rkfree(pos);
        }
        return 0;
}

struct ipcp_factory_data {
        struct list_head instances;
};

static struct ipcp_factory_data dummy_data;

static int dummy_init(struct ipcp_factory_data * data)
{
        ASSERT(data);

        bzero(&dummy_data, sizeof(dummy_data));
        INIT_LIST_HEAD(&data->instances);

        dummy_wq = rwq_create("dummy-wq");
        if (!dummy_wq) {
                LOG_CRIT("Cannot create dummy workqueue");
                return -1;
        }

        return 0;
}

static int dummy_fini(struct ipcp_factory_data * data)
{
        int ret;

        ASSERT(data);

        ret = rwq_destroy(dummy_wq);

        return ret;
}

static int dummy_assign_to_dif(struct ipcp_instance_data * data,
                               const struct dif_info *     dif_information)
{
        struct ipcp_config * pos;

        ASSERT(data);
        ASSERT(dif_information);

        if (!data->info) {
                LOG_ERR("There is no info for this IPC process");
                return -1;
        }

        data->info->dif_name = name_dup(dif_information->dif_name);
        if (!data->info->dif_name) {
                char * tmp = name_tostring(dif_information->dif_name);
                LOG_ERR("Assingment of IPC Process to DIF %s failed", tmp);
                if (tmp) rkfree(tmp);

                rkfree(data->info);

                return -1;
        }

        if (dif_information->configuration)
                list_for_each_entry(pos,
                                    &(dif_information->configuration->
                                      ipcp_config_entries),
                                    next)
                        LOG_DBG("Configuration entry name: %s; value: %s",
                                pos->entry->name,
                                pos->entry->value);

        LOG_DBG("Assigned IPC Process to DIF %s",
                data->info->dif_name->process_name);

        return 0;
}

static int dummy_update_dif_config(struct ipcp_instance_data * data,
                                   const struct dif_config *   new_config)
{
        /* Nothing to be reconfigured */

        return -1;
}

static struct ipcp_instance_ops dummy_instance_ops = {
        .flow_allocate_request  = dummy_flow_allocate_request,
        .flow_allocate_response = dummy_flow_allocate_response,
        .flow_deallocate        = dummy_flow_deallocate,
        .application_register   = dummy_application_register,
        .application_unregister = dummy_application_unregister,
        .sdu_write              = dummy_sdu_write,
        .assign_to_dif          = dummy_assign_to_dif,
        .update_dif_config      = dummy_update_dif_config,
};

static struct ipcp_instance_data *
find_instance(struct ipcp_factory_data * data,
              ipc_process_id_t           id)
{

        struct ipcp_instance_data * pos;

        list_for_each_entry(pos, &(data->instances), list) {
                if (pos->id == id) {
                        return pos;
                }
        }

        return NULL;
}

static struct ipcp_instance * dummy_create(struct ipcp_factory_data * data,
                                           const struct name *        name,
                                           ipc_process_id_t           id)
{
        struct ipcp_instance * inst;

        ASSERT(data);

        /* Check if there already is an instance with that id */
        if (find_instance(data,id)) {
                LOG_ERR("There's a shim instance with id %d already", id);
                return NULL;
        }

        /* Create an instance */
        LOG_DBG("Create an instance");
        inst = rkzalloc(sizeof(*inst), GFP_KERNEL);
        if (!inst)
                return NULL;

        /* fill it properly */
        LOG_DBG("Fill it properly");
        inst->ops  = &dummy_instance_ops;
        inst->data = rkzalloc(sizeof(struct ipcp_instance_data), GFP_KERNEL);
        if (!inst->data) {
                LOG_ERR("Fill it properly failed");
                rkfree(inst);
                return NULL;
        }

        inst->data->id = id;
        INIT_LIST_HEAD(&inst->data->flows);
        inst->data->info = rkzalloc(sizeof(*inst->data->info), GFP_KERNEL);
        if (!inst->data->info) {
                LOG_ERR("Failed creation of inst->data->info");
                rkfree(inst->data);
                rkfree(inst);
                return NULL;
        }

        inst->data->info->name = name_dup(name);
        if (!inst->data->info->name) {
                LOG_ERR("Failed creation of ipc name");
                rkfree(inst->data->info);
                rkfree(inst->data);
                rkfree(inst);
                return NULL;
        }

        /* FIXME: Remove as soon as the kipcm_kfa gets removed*/
        inst->data->kfa = kipcm_kfa(default_kipcm);
        LOG_DBG("KFA instance %pK bound to the shim dummy", inst->data->kfa);

        /*
         * Bind the shim-instance to the shims set, to keep all our data
         * structures linked (somewhat) together
         */

        INIT_LIST_HEAD(&inst->data->apps_registered);

        LOG_DBG("Adding dummy instance to the list of shim dummy instances");

        INIT_LIST_HEAD(&(inst->data->list));
        LOG_DBG("Initialization of instance list: %pK", &inst->data->list);
        list_add(&(inst->data->list), &(data->instances));
        LOG_DBG("Inst %pK added to the dummy instances", inst);

        return inst;
}

static int dummy_destroy(struct ipcp_factory_data * data,
                         struct ipcp_instance *     instance)
{
        struct ipcp_instance_data * pos, * next;

        ASSERT(data);
        ASSERT(instance);

        /* Retrieve the instance */
        list_for_each_entry_safe(pos, next, &data->instances, list) {
                if (pos->id == instance->data->id) {
                        /* Unbind from the instances set */
                        list_del(&pos->list);
                        dummy_deallocate_all(pos);
                        dummy_unregister_all(pos);

                        /* Destroy it */
                        if (pos->info->dif_name)
                                name_destroy(pos->info->dif_name);

                        if (pos->info->name)
                                name_destroy(pos->info->name);

                        rkfree(pos->info);
                        rkfree(pos);
                        rkfree(instance);

                        return 0;
                }
        }

        return -1;
}

static struct ipcp_factory_ops dummy_ops = {
        .init      = dummy_init,
        .fini      = dummy_fini,
        .create    = dummy_create,
        .destroy   = dummy_destroy,
};

struct ipcp_factory * shim = NULL;

static int __init mod_init(void)
{
        shim = kipcm_ipcp_factory_register(default_kipcm,
                                           SHIM_NAME,
                                           &dummy_data,
                                           &dummy_ops);
        if (!shim) {
                LOG_CRIT("Cannot register %s factory", SHIM_NAME);
                return -1;
        }

        return 0;
}

static void __exit mod_exit(void)
{
        ASSERT(shim);

        if (kipcm_ipcp_factory_unregister(default_kipcm, shim)) {
                LOG_CRIT("Cannot unregister %s factory", SHIM_NAME);
                return;
        }
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("RINA Dummy Shim IPC");

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Francesco Salvestrini <f.salvestrini@nextworks.it>");
MODULE_AUTHOR("Miquel Tarzan <miquel.tarzan@i2cat.net>");
MODULE_AUTHOR("Sander Vrijders <sander.vrijders@intec.ugent.be>");
MODULE_AUTHOR("Leonardo Bergesio <leonardo.bergesio@i2cat.net>");
