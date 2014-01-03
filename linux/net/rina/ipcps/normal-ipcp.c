/*
 *  Normal IPC Process
 *
 *    Leonardo Bergesio     <leonardo.bergesio@i2cat.net>
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *    Miquel Tarzan         <miquel.tarzan@i2cat.net>
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

#define IPCP_NAME   "normal-ipc"

#define RINA_PREFIX IPCP_NAME

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
#include "efcp.h"
#include "rmt.h"
#include "efcp-utils.h"

/*  FIXME: To be removed ABSOLUTELY */
extern struct kipcm * default_kipcm;

struct normal_info {
        struct name * name;
        struct name * dif_name;
};

struct ipcp_instance_data {
        /* FIXME add missing needed attributes */
        ipc_process_id_t        id;
        u32                     nl_port;
        struct list_head        flows;
        struct list_head        list;
        struct normal_info *    info;
        /*  FIXME: Remove it as soon as the kipcm_kfa gets removed*/
        struct kfa *            kfa;
        struct efcp_container * efcpc;
        struct rmt *            rmt;
        address_t               address;
        struct normal_mgmt *    mgmt;
};

enum normal_flow_state {
        PORT_STATE_NULL = 1,
        PORT_STATE_RECIPIENT_ALLOCATE_PENDING,
        PORT_STATE_INITIATOR_ALLOCATE_PENDING,
        PORT_STATE_ALLOCATED
};

struct cep_ids_entry {
        struct list_head list;
        cep_id_t         cep_id;
};

struct normal_flow {
        port_id_t        port_id;
        cep_id_t         active;
        struct list_head cep_ids_list;
        struct list_head list;
};

static struct normal_flow * find_flow(struct ipcp_instance_data * data,
                                      port_id_t                   port_id)
{
        struct normal_flow * flow;

        list_for_each_entry(flow, &(data->flows), list) {
                if (flow->port_id == port_id)
                        return flow;
        }

        return NULL;
}

struct ipcp_factory_data {
        u32    nl_port;
        struct list_head instances;
};

static struct ipcp_factory_data normal_data;

static int normal_init(struct ipcp_factory_data * data)
{
        ASSERT(data);

        bzero(&normal_data, sizeof(normal_data));
        INIT_LIST_HEAD(&data->instances);

        return 0;
}

static int normal_fini(struct ipcp_factory_data * data)
{
        ASSERT(data);
        ASSERT(list_empty(&data->instances));
        return 0;
}

static int normal_sdu_write(struct ipcp_instance_data * data,
                            port_id_t                   id,
                            struct sdu *                sdu)
{
        struct normal_flow * flow;

        flow = find_flow(data, id);
        if (!flow) {
                LOG_ERR("There is no flow bound to this port_id: %d", id);
                return -1;
        }
        efcp_container_write(data->efcpc, flow->active, sdu);
        return 0;
}

static struct ipcp_factory * normal = NULL;

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

static cep_id_t connection_create_request(struct ipcp_instance_data * data,
                                          port_id_t                   port_id,
                                          address_t                   source,
                                          address_t                   dest,
                                          qos_id_t                    qos_id,
                                          int                         policies)
{
        cep_id_t               cep_id;
        struct connection *    conn;
        struct normal_flow *   flow;
        struct cep_ids_entry * cep_entry;

        conn = rkzalloc(sizeof(*conn), GFP_KERNEL);
        if (!conn)
                return -1;

        conn->destination_address = dest;
        conn->source_address      = source;
        conn->port_id             = port_id;
        conn->qos_id              = qos_id;

        cep_id = efcp_connection_create(data->efcpc, conn);
        if (!is_cep_id_ok(cep_id)) {
                LOG_ERR("Failed EFCP connection creation");
                rkfree(conn);
                return cep_id_bad();
        }

        cep_entry = rkzalloc(sizeof(*cep_entry), GFP_KERNEL);
        if (!cep_entry) {
                LOG_ERR("Could not create a cep_id entry, bailing out");
                efcp_connection_destroy(data->efcpc, cep_id);
                return cep_id_bad();
        }
        INIT_LIST_HEAD(&cep_entry->list);
        cep_entry->cep_id = cep_id;

        flow = find_flow(data, port_id);
        if (!flow) {
                flow = rkzalloc(sizeof(*flow), GFP_KERNEL);
                if (!flow) {
                        LOG_ERR("Could not create a flow in normal-ipcp");
                        efcp_connection_destroy(data->efcpc, cep_id);
                        return cep_id_bad();
                }
                flow->port_id = port_id;
                INIT_LIST_HEAD(&flow->list);
                INIT_LIST_HEAD(&flow->cep_ids_list);
                list_add(&flow->list, &data->flows);
        }

        list_add(&cep_entry->list, &flow->cep_ids_list);
        flow->active = cep_id;

        return cep_id;
}

static int connection_update_request(struct ipcp_instance_data * data,
                                     port_id_t                   port_id,
                                     cep_id_t                    src_cep_id,
                                     cep_id_t                    dst_cep_id)
{
        if (efcp_connection_update(data->efcpc, src_cep_id, dst_cep_id))
                return -1;

        if (kipcm_flow_commit(default_kipcm, data->id, port_id)) {
                efcp_connection_destroy(data->efcpc, src_cep_id);
                return -1;
        }

        return 0;
}

static struct normal_flow * find_flow_cepid(struct ipcp_instance_data * data,
                                            cep_id_t                    id)
{
        struct normal_flow * pos;

        list_for_each_entry(pos, &(data->flows), list) {
                if (pos->active == id) {
                        return pos;
                }
        }
        return NULL;
}

static int remove_cep_id_from_flow(struct normal_flow * flow,
                                   cep_id_t             id)
{
        struct cep_ids_entry *pos, *next;

        list_for_each_entry_safe(pos, next, &(flow->cep_ids_list), list) {
                if (pos->cep_id == id) {
                        list_del(&pos->list);
                        rkfree(pos);
                        return 0;
                }
        }
        return -1;
}

static int ipcp_flow_notification(struct ipcp_instance_data * data,
                                  port_id_t                   pid)
{
        LOG_MISSING;

        if (kfa_flow_bind_rmt(data->kfa, pid, data->rmt))
                return -1;

        if (rmt_send_queue_add(data->rmt, pid))
                return -1;

        if (rmt_rcve_queue_add(data->rmt, pid))
                return -1;

        return 0;
}

static int connection_destroy_request(struct ipcp_instance_data * data,
                                      cep_id_t                    src_cep_id)
{
        struct normal_flow * flow;

        if (efcp_connection_destroy(data->efcpc, src_cep_id))
                return -1;

        if (!(&data->flows))
                return -1;

        flow = find_flow_cepid(data, src_cep_id);
        if (!flow) {
                LOG_ERR("Could not retrieve flow by cep_id :%d", src_cep_id);
        }
        if (remove_cep_id_from_flow(flow, src_cep_id)) {
                LOG_ERR("Could not remove cep_id: %d", src_cep_id);
        }
        if (list_empty(&flow->cep_ids_list))
                rkfree(flow);

        return 0;
}

static cep_id_t
connection_create_arrived(struct ipcp_instance_data * data,
                          port_id_t                   port_id,
                          address_t                   source,
                          address_t                   dest,
                          qos_id_t                    qos_id,
                          cep_id_t                    dst_cep_id,
                          int                         policies)
{
        struct connection *    conn;
        cep_id_t               cep_id;
        struct normal_flow *   flow;
        struct cep_ids_entry * cep_entry;

        conn = rkzalloc(sizeof(*conn), GFP_KERNEL);
        if (!conn) {
                LOG_ERR("Failed connection creation");
                return -1;
        }
        conn->destination_address = dest;
        conn->source_address      = source;
        conn->port_id             = port_id;
        conn->qos_id              = qos_id;
        conn->destination_cep_id  = dst_cep_id;

        cep_id = efcp_connection_create(data->efcpc, conn);
        if (!is_cep_id_ok(cep_id)) {
                LOG_ERR("Failed EFCP connection creation");
                rkfree(conn);
                return cep_id_bad();
        }
        LOG_DBG("Cep_id allocated for the arrived connection request: %d",
                cep_id);

        cep_entry = rkzalloc(sizeof(*cep_entry), GFP_KERNEL);
        if (!cep_entry) {
                LOG_ERR("Could not create a cep_id entry, bailing out");
                efcp_connection_destroy(data->efcpc, cep_id);
                return cep_id_bad();
        }
        INIT_LIST_HEAD(&cep_entry->list);
        cep_entry->cep_id = cep_id;

        flow = find_flow(data, port_id);
        if (!flow) {
                flow = rkzalloc(sizeof(*flow), GFP_KERNEL);
                if (!flow) {
                        LOG_ERR("Could not create a flow in normal-ipcp");
                        efcp_connection_destroy(data->efcpc, cep_id);
                        return cep_id_bad();
                }
                flow->port_id = port_id;
                INIT_LIST_HEAD(&flow->list);
                INIT_LIST_HEAD(&flow->cep_ids_list);
                list_add(&flow->list, &data->flows);
        }

        list_add(&cep_entry->list, &flow->cep_ids_list);
        flow->active = cep_id;

        return cep_id;
}

static int normal_check_dt_cons(struct dt_cons * dt_cons)
{
        /* FIXME: What should we check here? */
        return 0;
}

static int normal_assign_to_dif(struct ipcp_instance_data * data,
                                const struct dif_info *     dif_information)
{
        struct dt_cons * dt_cons;

        data->info->dif_name = name_dup(dif_information->dif_name);
        data->address = dif_information->configuration->address;
        if (rmt_address_set(data->rmt, data->address))
                return -1;

        dt_cons = dif_information->configuration->dt_cons;

        if (normal_check_dt_cons(dt_cons)) {
                LOG_ERR("Configuration constants for the DIF are bogus...");
                return -1;
        }

        efcp_container_set_dt_cons(dt_cons, data->efcpc);

        return 0;
}

static int normal_management_sdu_read(struct ipcp_instance_data * data,
                                      struct sdu_wpi **           sdu_wpi)
{
        LOG_DBG("Trying to read mgmt SDU from IPC Process %d", data->id);

        return rmt_management_sdu_read(data->rmt, sdu_wpi);
}

static int normal_management_sdu_write(struct ipcp_instance_data * data,
                                       port_id_t                   port_id,
                                       struct sdu *                sdu)
{
        struct pci *  pci;
        struct pdu *  pdu;
        address_t     dst_address;

        LOG_DBG("Passing SDU to be written to N-1 port %d from IPC Process %d"
                , port_id, data->id);

        if (!sdu) {
                LOG_ERR("No data passed, bailing out");
                return -1;
        }

        /*FIXME: fake PFT */
        if (port_id ==1) 
                dst_address = 17;
        else
                dst_address = 16;

        pci = pci_create();
        if (!pci)
                return -1;

        if (pci_format(pci,
                       0,
                       0,
                       data->address,
                       dst_address,
                       0,
                       0,
                       PDU_TYPE_MGMT)) {
                pci_destroy(pci);
                return -1;
        }

        pdu = pdu_create();
        if (!pdu) {
                pci_destroy(pci);
                return -1;
        }

        if (pdu_buffer_set(pdu, sdu_buffer_rw(sdu))) {
                pci_destroy(pci);
                return -1;
        }

        if (pdu_pci_set(pdu, pci)) {
                pci_destroy(pci);
                return -1;
        }

        LOG_DBG("Going to send to the RMT:");
        LOG_DBG("src_address %d", pci_source(pci));
        LOG_DBG("dst_address: %d", pci_destination(pci));
        LOG_DBG("port: %d", port_id);

        /* Give the data to RMT now ! */
        if (rmt_send(data->rmt,
                     pci_destination(pci),
                     pci_cep_destination(pci),
                     pdu)) {
                LOG_ERR("Could not send to RMT");
                return -1;
        }       
        return 0;

}

/*  FIXME: register ops */
static struct ipcp_instance_ops normal_instance_ops = {
        .flow_allocate_request     = NULL,
        .flow_allocate_response    = NULL,
        .flow_deallocate           = NULL,
        .application_register      = NULL,
        .application_unregister    = NULL,
        .sdu_write                 = normal_sdu_write,
        .assign_to_dif             = normal_assign_to_dif,
        .update_dif_config         = NULL,
        .connection_create         = connection_create_request,
        .connection_update         = connection_update_request,
        .connection_destroy        = connection_destroy_request,
        .connection_create_arrived = connection_create_arrived,
        .flow_binding_ipcp         = ipcp_flow_notification,
        .management_sdu_read       = normal_management_sdu_read,
        .management_sdu_write      = normal_management_sdu_write
};

static struct ipcp_instance * normal_create(struct ipcp_factory_data * data,
                                            const struct name *        name,
                                            ipc_process_id_t           id)
{
        struct ipcp_instance * instance;

        ASSERT(data);

        if (find_instance(data, id)) {
                LOG_ERR("There is already a normal ipcp instance with id %d",
                        id);
                return NULL;
        }

        LOG_DBG("Creating normal IPC process...");
        instance = rkzalloc(sizeof(*instance), GFP_KERNEL);
        if (!instance) {
                LOG_ERR("Could not allocate memory for normal ipc process "
                        "with id %d", id);
                return NULL;
        }

        instance->ops  = &normal_instance_ops;
        instance->data = rkzalloc(sizeof(struct ipcp_instance_data),
                                  GFP_KERNEL);
        if (!instance->data) {
                LOG_ERR("Could not allocate memory for normal ipcp "
                        "internal data");
                rkfree(instance);
                return NULL;
        }

        instance->data->id      = id;
        instance->data->nl_port = data->nl_port;
        instance->data->info    = rkzalloc(sizeof(struct normal_info *),
                                           GFP_KERNEL);
        if (!instance->data->info) {
                LOG_ERR("Could not allocate memory for normal ipcp info");
                rkfree(instance->data->mgmt); 
                rkfree(instance->data);
                rkfree(instance);
                return NULL;
        }

        instance->data->info->name = name_dup(name);
        if (!instance->data->info->name) {
                LOG_ERR("Failed creation of ipc name");
                rkfree(instance->data->info);
                rkfree(instance->data->mgmt); 
                rkfree(instance->data);
                rkfree(instance);
                return NULL;
        }

        /*  FIXME: Remove as soon as the kipcm_kfa gets removed */
        instance->data->kfa = kipcm_kfa(default_kipcm);

        instance->data->efcpc = efcp_container_create(instance->data->kfa);
        if (!instance->data->efcpc) {
                name_destroy(instance->data->info->name);
                rkfree(instance->data->info);
                rkfree(instance->data->mgmt); 
                rkfree(instance->data);
                rkfree(instance);
                return NULL;
        }
        instance->data->rmt = rmt_create(instance->data->kfa,
                                         instance->data->efcpc);
        if (!instance->data->rmt) {
                LOG_ERR("Failed creation of RMT instance");
                efcp_container_destroy(instance->data->efcpc);
                name_destroy(instance->data->info->name);
                rkfree(instance->data->info);
                rkfree(instance->data->mgmt); 
                rkfree(instance->data);
                rkfree(instance);
                return NULL;
        }

        if (efcp_bind_rmt(instance->data->efcpc, instance->data->rmt)) {
                LOG_ERR("Failed binding of RMT and EFCPC");
                rmt_destroy(instance->data->rmt);
                efcp_container_destroy(instance->data->efcpc);
                name_destroy(instance->data->info->name);
                rkfree(instance->data->info);
                rkfree(instance->data->mgmt); 
                rkfree(instance->data);
                rkfree(instance);
                return NULL;
        }


        /* FIXME: Probably missing normal flow structures creation */
        INIT_LIST_HEAD(&instance->data->flows);
        INIT_LIST_HEAD(&instance->data->list);
        list_add(&(instance->data->list), &(data->instances));
        LOG_DBG("Normal IPC process instance created and added to the list");

        return instance;
}

static int normal_deallocate_all(struct ipcp_instance_data * data)
{
        LOG_MISSING;
        return 0;
}

static int normal_destroy(struct ipcp_factory_data * data,
                          struct ipcp_instance *     instance)
{

        struct ipcp_instance_data * tmp;

        ASSERT(data);
        ASSERT(instance);

        tmp = find_instance(data, instance->data->id);
        if (!tmp) {
                LOG_ERR("Could not find normal ipcp instance to destroy");
                return -1;
        }

        list_del(&tmp->list);

        /* FIXME: flow deallocation not implemented */
        if (normal_deallocate_all(tmp)) {
                LOG_ERR("Could not deallocate normal ipcp flows");
                return -1;
        }

        if (tmp->info->dif_name)
                name_destroy(tmp->info->dif_name);

        if (tmp->info->name)
                name_destroy(tmp->info->name);

        efcp_container_destroy(tmp->efcpc);
        rmt_destroy(tmp->rmt);
        rkfree(tmp->info);
        rkfree(tmp);
        rkfree(instance);

        return 0;
}

static struct ipcp_factory_ops normal_ops = {
        .init    = normal_init,
        .fini    = normal_fini,
        .create  = normal_create,
        .destroy = normal_destroy,
};

static int __init mod_init(void)
{
        normal = kipcm_ipcp_factory_register(default_kipcm,
                                             IPCP_NAME,
                                             &normal_data,
                                             &normal_ops);
        if (!normal)
                return -1;

        return 0;
}

static void __exit mod_exit(void)
{
        ASSERT(normal);

        kipcm_ipcp_factory_unregister(default_kipcm, normal);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("RINA normal IPC Process");

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Francesco Salvestrini <f.salvestrini@nextworks.it>");
MODULE_AUTHOR("Miquel Tarzan <miquel.tarzan@i2cat.net>");
MODULE_AUTHOR("Sander Vrijders <sander.vrijders@intec.ugent.be>");
MODULE_AUTHOR("Leonardo Bergesio <leonardo.bergesio@i2cat.net>");
