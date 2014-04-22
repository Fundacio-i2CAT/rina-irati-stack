/*
 * Shim IPC process for hypervisors
 *
 *      Author: Vincenzo Maffione <v.maffione@nextworks.it>
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
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/list.h>

#define SHIM_HV_NAME    "shim-hv-virtio"
#define RINA_PREFIX SHIM_HV_NAME

#include "logs.h"
#include "common.h"
#include "kipcm.h"
#include "debug.h"
#include "utils.h"
#include "du.h"
#include "ipcp-utils.h"
#include "ipcp-factories.h"
#include "vmpi.h"


/* FIXME: To be removed ABSOLUTELY */
extern struct kipcm *default_kipcm;

/* Private data associated to shim IPC process factory. */
static struct ipcp_factory_data {
        struct list_head        instances;
} shim_hv_factory_data;

/* For now there can be only a VMPI device and then only a shim IPC
 * process. Therefore it is enough to hold the associated VMPI handler
 * in a global variable.
 */
static vmpi_info_t *gmpi = NULL;

enum channel_state {
        CHANNEL_STATE_NULL = 0,
        CHANNEL_STATE_PENDING,
        CHANNEL_STATE_ALLOCATED,
};

struct shim_hv_channel {
        enum channel_state              state;
        port_id_t                       port_id;
};

enum shim_hv_command {
        CMD_ALLOCATE_REQ = 0,
        CMD_ALLOCATE_RESP,
        CMD_DEALLOCATE,
};

enum shim_hv_response {
        RESP_OK = 0,
        RESP_KO = 1
};

/* Per-IPC-process data structure that contains all
 * the VMPI-related information (among the other).
 */
struct shim_hv_vmpi {
        vmpi_info_t                     *mpi;
        struct shim_hv_channel          channels[VMPI_MAX_CHANNELS];
};

/* Private data associated to a shim IPC process. */
struct ipcp_instance_data {
        struct list_head        list;
        ipc_process_id_t        id;
        struct name             name;
        int                     assigned;
        struct name             dif_name;
        struct flow_spec        fspec;
        struct kfa              *kfa;
        struct list_head        registered_applications;
        struct shim_hv_vmpi     vmpi;
};

struct name_list_element {
        struct list_head list;
        struct name application_name;
};

static unsigned int port_id_to_channel(struct ipcp_instance_data *priv,
                                       port_id_t port_id)
{
        int i;

        for (i = 0; i < VMPI_MAX_CHANNELS; i++)
                if (priv->vmpi.channels[i].port_id == port_id)
                        return i;

        /* The channel 0 is the control channel, and cannot be bound to a
         * port id. For this reason we use 0 to report failure.
         */
        return 0;
}

/* TODO reuse this function across all the ipcps. */
static struct ipcp_instance_data *
find_instance(struct ipcp_factory_data *factory_data, ipc_process_id_t id)
{

        struct ipcp_instance_data *cur;

        list_for_each_entry(cur, &(factory_data->instances), list) {
                if (cur->id == id) {
                        return cur;
                }
        }

        return NULL;

}

static void ser_uint8(uint8_t **to, uint8_t x)
{
        *(*to) = x;
        *to += 1;
}

static void ser_uint32(uint8_t **to, uint32_t x)
{
        uint32_t *buf = (uint32_t*)(*to);

        *buf = x;
        *to += sizeof(uint32_t);
}

static void ser_string(uint8_t **to, string_t *str, uint32_t len)
{
        memcpy(*to, str, len);
        *to += len;
        *(*to) = '\0';
        *to += 1;
}

static int des_uint8(const uint8_t **from, uint8_t *x, int *len)
{
        if (*len == 0)
                return -1;

        *x = *(*from);
        *from += sizeof(uint8_t);
        *len -= sizeof(uint8_t);

        return 0;
}

static int des_uint32(const uint8_t **from, uint32_t *x, int *len)
{
        if (*len < sizeof(uint32_t))
                return -1;

        *x = *((uint32_t *)(*from));
        *from += sizeof(uint32_t);
        *len -= sizeof(uint32_t);

        return 0;
}

static int des_string(const uint8_t **from, const string_t **str, int *len)
{
        int l = *len;
        const uint8_t *f = *from;

        while (l && *f != '\0') {
                f++;
                l--;
        }

        if (*f != '\0')
                return -1;
        /* Skip the NULL character also. */
        f++;
        l--;

        *str = *from;
        *from = f;
        *len = l;

        return 0;
}

/* Build and send an ALLOCATE_RESP message on the control channel. */
static int
shim_hv_send_allocate_resp(struct ipcp_instance_data *priv,
                           unsigned int ch, int response)
{
        uint8_t msg[sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint8_t)];
        uint8_t *msg_ptr;
        struct iovec iov;
        int ret;

        /* Build the message by serialization. */
        msg_ptr = msg;
        ser_uint8(&msg_ptr, CMD_ALLOCATE_RESP);
        ser_uint32(&msg_ptr, ch);
        ser_uint8(&msg_ptr, response);

        iov.iov_base = msg;
        iov.iov_len = sizeof(msg);
        ret = vmpi_write(priv->vmpi.mpi, 0, &iov, 1);

        return !(ret == sizeof(msg));
}

/* Invoked from the RINA stack when an application issues a
 * FlowAllocateRequest to this shim IPC process.
 */
static int
shim_hv_flow_allocate_request(struct ipcp_instance_data *priv,
                              const struct name *src_application,
                              const struct name *dst_application,
                              const struct flow_spec *fspec,
                              port_id_t port_id)
{
        uint32_t ch;
        string_t *src_name;
        string_t *dst_name;
        size_t msg_len;
        ssize_t n;
        uint8_t *msg_buf;
        uint8_t *msg_ptr;
        uint32_t slen, dlen;
        int err = -ENOMEM;
        struct iovec iov;

        /* TODO move this checs to the caller. */
        ASSERT(priv);
        ASSERT(src_application);
        ASSERT(dst_application);

        /* Select an unused channel. */
        for (ch = 1; ch < VMPI_MAX_CHANNELS; ch++)
                if (priv->vmpi.channels[ch].state == CHANNEL_STATE_NULL)
                        break;

        if (ch == VMPI_MAX_CHANNELS) {
                LOG_INFO("%s: no free channel available, try later", __func__);
                err = -EBUSY;
                goto src_name_alloc;
        }

        src_name = name_tostring(src_application);
        if (!src_name) {
                LOG_ERR("%s: allocating source naming info", __func__);
                goto src_name_alloc;
        }

        dst_name = name_tostring(dst_application);
        if (!dst_name) {
                LOG_ERR("%s: allocating destination naming info", __func__);
                goto dst_name_alloc;
        }

        /* Compute the length of the ALLOCATE-REQ message and allocate
         * a buffer for the message itself.
         */
        slen = strlen(src_name);
        dlen = strlen(dst_name);
        msg_len = 1 + sizeof(ch) + slen + 1 + dlen + 1;
        if (msg_len >= 2000) { /* XXX Temporary limitation */
                LOG_ERR("%s: message too long %d", __func__, (int)msg_len);
                goto msg_alloc;
        }
        msg_buf = rkmalloc(msg_len, GFP_KERNEL);
        if (msg_buf == NULL) {
                LOG_ERR("%s: message buffer allocation failed", __func__);
                goto msg_alloc;
        }

        /* Build the message by serialization. */
        msg_ptr = msg_buf;
        ser_uint8(&msg_ptr, CMD_ALLOCATE_REQ);
        ser_uint32(&msg_ptr, ch);
        ser_string(&msg_ptr, src_name, slen);
        ser_string(&msg_ptr, dst_name, dlen);

        /* Send the message on the control channel. */
        iov.iov_base = msg_buf;
        iov.iov_len = msg_len;
        n = vmpi_write(priv->vmpi.mpi, 0, &iov, 1);
        LOG_INFO("%s: vmpi_write(%d) --> %d", __func__, (int)msg_len, (int)n);

        err = 0;
        priv->vmpi.channels[ch].state = CHANNEL_STATE_PENDING;
        priv->vmpi.channels[ch].port_id = port_id;

        LOG_INFO("%s: channel %d --> PENDING", __func__, ch);

        rkfree(msg_buf);
msg_alloc:
        rkfree(dst_name);
dst_name_alloc:
        rkfree(src_name);
src_name_alloc:
        if (err) {
                /* The flow was allocated outside this module. Are we
                 * in charge of deallocate it this method fails?
                 */
                if (kfa_flow_deallocate(priv->kfa, port_id))
                        LOG_ERR("%s: kfa_flow_deallocate() failed", __func__);
        }

        return err;
}

/* Invoked from the RINA stack when an application issues a
 * FlowAllocateResponse to this shim IPC process.
 */
static int
shim_hv_flow_allocate_response(struct ipcp_instance_data *priv,
                               port_id_t port_id, int result)
{
        unsigned int ch = port_id_to_channel(priv, port_id);
        int ret;
        int response = RESP_KO;

        if (!ch) {
                LOG_ERR("%s: unknown port-id %d", __func__, port_id);
                return -1;
        }

        if (priv->vmpi.channels[ch].state != CHANNEL_STATE_PENDING) {
                LOG_ERR("%s: channel %d in invalid state %d", __func__, ch,
                                priv->vmpi.channels[ch].state);
                goto resp;
        }

        if (result == 0) {
                /* Positive response */
                ret = kipcm_flow_commit(default_kipcm, priv->id, port_id);
                if (ret) {
                        LOG_ERR("%s: kipcm_flow_commit() failed", __func__);
                        goto resp;
                }

                /* Let's do the transition to the ALLOCATED state. */
                priv->vmpi.channels[ch].state = CHANNEL_STATE_ALLOCATED;
                response = RESP_OK;
                LOG_INFO("%s: channel %d --> ALLOCATED", __func__, ch);
        } else {
                /* Negative response */
                kfa_flow_deallocate(priv->kfa, port_id);
                /* XXX The following call should be useless because of the last
                 * call.
                 */
                kfa_port_id_release(priv->kfa, port_id);
                priv->vmpi.channels[ch].state = CHANNEL_STATE_NULL;
                priv->vmpi.channels[ch].port_id = port_id_bad();
                LOG_INFO("%s: channel %d --> NULL", __func__, ch);
        }
resp:
        shim_hv_send_allocate_resp(priv, ch, response);

        return (response == RESP_OK) ? 0 : -1;
}

/* Handler invoked when receiving an ALLOCATE_REQ message from the control
 * channel.
 */
static void shim_hv_handle_allocate_req(struct ipcp_instance_data *priv,
                                        const uint8_t *msg, int len)
{
        struct name *src_application = NULL;
        struct name *dst_application = NULL;
        const string_t *src_name;
        const string_t *dst_name;
        port_id_t port_id;
        uint32_t ch;
        int err = -1;

        if (des_uint32(&msg, &ch, &len)) {
                LOG_ERR("%s: truncated msg: while reading channel", __func__);
                goto out;
        }

        if (des_string(&msg, &src_name, &len)) {
                LOG_ERR("%s: truncated msg: while reading source application "
                        "name", __func__);
                goto out;
        }

        if (des_string(&msg, &dst_name, &len)) {
                LOG_ERR("%s: truncated msg: while reading destination "
                        "application name", __func__);
                goto out;
        }

        LOG_INFO("%s: received ALLOCATE_REQ(ch = %u, src = %s, dst = %s)",
                        __func__, ch, src_name, dst_name);

        if (ch >= VMPI_MAX_CHANNELS) {
                LOG_ERR("%s: bogus channel %u", __func__, ch);
                goto out;
        }

        if (priv->vmpi.channels[ch].state != CHANNEL_STATE_NULL) {
                LOG_ERR("%s: channel %d in invalid state %d", __func__, ch,
                                priv->vmpi.channels[ch].state);
                goto reject;
        }

        src_application = string_toname(src_name);
        if (src_application == NULL) {
                LOG_ERR("%s: invalid src name %s", __func__, src_name);
                goto reject;
        }

        dst_application = string_toname(dst_name);
        if (dst_application == NULL) {
                LOG_ERR("%s: invalid dst name %s", __func__, dst_name);
                goto dst_app;
        }

        port_id = kfa_port_id_reserve(priv->kfa, priv->id);
        if (!is_port_id_ok(port_id)) {
                LOG_ERR("%s: kfa_port_id_reserve() failed", __func__);
                goto port_alloc;
        }

        err = kfa_flow_create(priv->kfa, priv->id, port_id);
        if (err) {
                LOG_ERR("%s: kfa_flow_create() failed", __func__);
                goto flow_alloc;
        }

        err = kipcm_flow_arrived(default_kipcm, priv->id, port_id,
                                 &priv->dif_name, src_application,
                                 dst_application, &priv->fspec);
        if (err) {
                LOG_ERR("%s: kipcm_flow_arrived() failed", __func__);
                goto flow_arrived;
        }

        /* Everything is ok: Let's do the transition to the PENDING
         * state.
         */
        err = 0;
        priv->vmpi.channels[ch].state = CHANNEL_STATE_PENDING;
        priv->vmpi.channels[ch].port_id = port_id;
        LOG_INFO("%s: channel %d --> PENDING", __func__, ch);

flow_arrived:
        if (err)
                kfa_flow_deallocate(priv->kfa, port_id);
flow_alloc:
        if (err)
                kfa_port_id_release(priv->kfa, port_id);
port_alloc:
        if (dst_application)
                rkfree(dst_application);
dst_app:
        if (src_application)
                rkfree(src_application);
reject:
        if (err)
                shim_hv_send_allocate_resp(priv, ch, RESP_KO);
out:
        return;
}

/* Handler invoked when receiving an ALLOCATE_RESP message from the control
 * channel.
 */
static void shim_hv_handle_allocate_resp(struct ipcp_instance_data *priv,
                                         const uint8_t *msg, int len)
{
        uint32_t ch;
        uint8_t response;
        port_id_t port_id;
        int ret = -1;

        if (des_uint32(&msg, &ch, &len)) {
                LOG_ERR("%s: truncated msg: while reading channel", __func__);
                goto out;
        }

        if (des_uint8(&msg, &response, &len)) {
                LOG_ERR("%s: truncated msg: while reading response", __func__);
                goto out;
        }

        LOG_INFO("%s: received ALLOCATE_RESP(ch = %d, resp = %u)",
                        __func__, ch, response);

        if (ch >= VMPI_MAX_CHANNELS) {
                LOG_ERR("%s: bogus channel %u", __func__, ch);
                goto out;
        }

        if (priv->vmpi.channels[ch].state != CHANNEL_STATE_PENDING) {
                LOG_ERR("%s: channel %d in invalid state %d", __func__, ch,
                                priv->vmpi.channels[ch].state);
                goto out;
        }
        port_id = priv->vmpi.channels[ch].port_id;

        ret = kipcm_flow_commit(default_kipcm, priv->id, port_id);
        if (ret) {
                LOG_ERR("%s: kipcm_flow_commit() failed", __func__);
                goto flow_commit;
        }

        ret = kipcm_notify_flow_alloc_req_result(default_kipcm, priv->id,
                                        port_id, (response == RESP_OK) ? 0 : 1);
        if (ret) {
                LOG_ERR("%s: kipcm_notify_flow_alloc_req_result() failed",
                        __func__);
                goto flow_commit;
        }

        if (response == RESP_OK) {
                /* Everything is ok: Let's do the transition to the ALLOCATED
                 * state.
                 */
                ret = 0;
                priv->vmpi.channels[ch].state = CHANNEL_STATE_ALLOCATED;
                LOG_INFO("%s: channel %d --> ALLOCATED", __func__, ch);
        } else {
                priv->vmpi.channels[ch].state = CHANNEL_STATE_NULL;
                priv->vmpi.channels[ch].port_id = port_id_bad();
                LOG_INFO("%s: channel %d --> NULL", __func__, ch);
        }

flow_commit:
        if (ret) {
                if (kfa_flow_deallocate(priv->kfa, port_id))
                        LOG_ERR("%s: kfa_flow_deallocate() failed", __func__);
        }
out:
        return;
}

static void shim_hv_handle_deallocate(struct ipcp_instance_data *priv,
                                      const uint8_t *msg, int len)
{
        uint32_t ch;

        if (des_uint32(&msg, &ch, &len)) {
                LOG_ERR("%s: truncated msg: while reading channel", __func__);
                return;
        }

        LOG_INFO("%s: received DEALLOCATE(ch = %d)", __func__, ch);
}

static void shim_hv_handle_control_msg(struct ipcp_instance_data *priv,
                                       const uint8_t *msg, int len)
{
        uint8_t cmd;

        if (des_uint8(&msg, &cmd, &len)) {
                LOG_ERR("%s: truncated msg: while reading command", __func__);
                return;
        }

        switch (cmd) {
                case CMD_ALLOCATE_REQ:
                        shim_hv_handle_allocate_req(priv, msg, len);
                        break;
                case CMD_ALLOCATE_RESP:
                        shim_hv_handle_allocate_resp(priv, msg, len);
                        break;
                case CMD_DEALLOCATE:
                        shim_hv_handle_deallocate(priv, msg, len);
                        break;
                default:
                        LOG_ERR("%s: unknown cmd %u\n", __func__, cmd);
                        break;
        }
}

static void shim_hv_recv_callback(void *opaque, unsigned int channel, const char *buffer,
                                  int len)
{
        struct ipcp_instance_data *priv = opaque;

        if (unlikely(channel == 0)) {
                /* Control channel. */
                shim_hv_handle_control_msg(priv, buffer, len);
                return;
        }

        /* Regular traffic channel. */
}

/* Register an application to this IPC process. */
static int
shim_hv_application_register(struct ipcp_instance_data *priv,
                             const struct name *application_name)
{
        struct name_list_element *cur;
        char *tmpstr = name_tostring(application_name);
        int ret = -1;

        /* Is this application already registered? */
        list_for_each_entry(cur, &priv->registered_applications, list) {
                if (name_is_equal(application_name, &cur->application_name)) {
                        LOG_ERR("%s: Application %s already registered",
                                __func__, tmpstr);
                        goto out;
                }
        }

        /* Add the application to the list of registered applications. */
        cur = rkzalloc(sizeof(*cur), GFP_KERNEL);
        if (!cur) {
                LOG_ERR("%s: Allocating list element\n", __func__);
                goto out;
        }

        if (name_cpy(application_name, &cur->application_name)) {
                LOG_ERR("%s: name_cpy() failed\n", __func__);
                goto name_alloc;
        }

        list_add(&priv->registered_applications, &cur->list);

        ret = 0;
        LOG_INFO("%s: Application %s registered", __func__, tmpstr);

name_alloc:
        if (ret) {
                rkfree(cur);
        }
out:
        if (tmpstr)
                rkfree(tmpstr);

        return ret;
}

/* Unregister an application from this IPC process. */
static int
shim_hv_application_unregister(struct ipcp_instance_data *priv,
                               const struct name *application_name)
{
        struct name_list_element *cur;
        struct name_list_element *found = NULL;
        char *tmpstr = name_tostring(application_name);

        /* Is this application registered? */
        list_for_each_entry(cur, &priv->registered_applications, list) {
                if (name_is_equal(application_name, &cur->application_name)) {
                        found = cur;
                        break;
                }
        }

        if (!found) {
                LOG_ERR("%s: Application %s not registered", __func__, tmpstr);
                goto out;
        }

        /* Remove the application from the list of registered applications. */
        name_fini(&found->application_name);
        list_del(&found->list);
        rkfree(found);

        LOG_INFO("%s: Application %s unregistered", __func__, tmpstr);

out:
        if (tmpstr)
                rkfree(tmpstr);

        return (found != NULL);
}

/* Callback invoked when a shim IPC process is assigned to a DIF. */
static int
shim_hv_assign_to_dif(struct ipcp_instance_data *priv,
                      const struct dif_info *dif_information)
{
        ASSERT(priv);
        ASSERT(dif_information);

        if (priv->assigned) {
                ASSERT(priv->dif_name.process_name);
                LOG_ERR("%s: IPC process already assigned to the DIF %s",
                        __func__,  priv->dif_name.process_name);
                return -1;
        }

        priv->assigned = 1;
        if (name_cpy(dif_information->dif_name, &priv->dif_name)) {
                LOG_ERR("%s: name_cpy() failed\n", __func__);
                return -1;
        }

        return 0;
}

static const struct name *
shim_hv_ipcp_name(struct ipcp_instance_data *priv)
{
        ASSERT(priv);
        ASSERT(name_is_ok(&priv->name));

        return &priv->name;
}

/* Interface exported by a shim IPC process. */
static struct ipcp_instance_ops shim_hv_ipcp_ops = {
        .flow_allocate_request     = shim_hv_flow_allocate_request,
        .flow_allocate_response    = shim_hv_flow_allocate_response,
        .flow_deallocate           = NULL,
        .flow_binding_ipcp         = NULL,
        .flow_destroy              = NULL,

        .application_register      = shim_hv_application_register,
        .application_unregister    = shim_hv_application_unregister,

        .assign_to_dif             = shim_hv_assign_to_dif,
        .update_dif_config         = NULL,

        .connection_create         = NULL,
        .connection_update         = NULL,
        .connection_destroy        = NULL,
        .connection_create_arrived = NULL,

        .sdu_enqueue               = NULL,
        .sdu_write                 = NULL,

        .mgmt_sdu_read             = NULL,
        .mgmt_sdu_write            = NULL,
        .mgmt_sdu_post             = NULL,

        .pft_add                   = NULL,
        .pft_remove                = NULL,
        .pft_dump                  = NULL,
        .pft_flush                 = NULL,

        .ipcp_name                 = shim_hv_ipcp_name,
};

/* Initialize the IPC process factory. */
static int
shim_hv_factory_init(struct ipcp_factory_data *factory_data)
{
        ASSERT(factory_data);

        INIT_LIST_HEAD(&factory_data->instances);

        LOG_INFO("%s initialized", SHIM_HV_NAME);

        return 0;
}

/* Uninitialize the IPC process factory. */
static int
shim_hv_factory_fini(struct ipcp_factory_data *data)
{
        ASSERT(data);

        ASSERT(list_empty(&data->instances));

        LOG_INFO("%s uninitialized", SHIM_HV_NAME);

        return 0;
}

/* Create a new shim IPC process. */
static struct ipcp_instance *
shim_hv_factory_ipcp_create(struct ipcp_factory_data *factory_data,
                            const struct name *name, ipc_process_id_t id)
{
        struct ipcp_instance *ipcp;
        struct ipcp_instance_data *priv;
        int i;
        int err;

        /* Check if there already is an instance with the specified id. */
        if (find_instance(factory_data, id)) {
                LOG_ERR("%s: id %d already created", __func__, id);
                goto alloc_ipcp;
        }

        /* For now we only accept a single IPC process for DIF.
         * This restriction will be removed when we are able to
         * manage multiple VMPI devices per Virtual Machine, e.g.
         * when we have a naming scheme for that.
         */
        if (!list_empty(&factory_data->instances)) {
                LOG_ERR("%s: multiple IPC processes are not allowed",
                                __func__);
                goto alloc_ipcp;
        }

        /* Allocate a new ipcp instance. */
        ipcp = rkzalloc(sizeof(*ipcp), GFP_KERNEL);
        if (!ipcp)
                goto alloc_ipcp;

        ipcp->ops = &shim_hv_ipcp_ops;

        /* Allocate private data for the new shim IPC process. */
        ipcp->data = priv = rkzalloc(sizeof(struct ipcp_instance_data), GFP_KERNEL);
        if (!priv)
                goto alloc_data;

        priv->id = id;
        priv->assigned = 0;
        bzero(&priv->fspec, sizeof(priv->fspec));
        priv->fspec.max_sdu_size = 2000;   /* XXX Temporary limitation. */
        priv->fspec.max_allowable_gap = -1;

        if (name_cpy(name, &priv->name)) {
                LOG_ERR("%s: name_cpy() failed\n", __func__);
                goto alloc_name;
        }

        INIT_LIST_HEAD(&priv->registered_applications);

        priv->kfa = kipcm_kfa(default_kipcm);
        ASSERT(priv->kfa);

        /* Initialize the VMPI-related data structure. */
        bzero(&priv->vmpi, sizeof(priv->vmpi));
        priv->vmpi.mpi = gmpi;
        ASSERT(priv->vmpi.mpi);
        for (i = 0; i < VMPI_MAX_CHANNELS; i++) {
                priv->vmpi.channels[i].state = CHANNEL_STATE_NULL;
                priv->vmpi.channels[i].state = port_id_bad();
        }
        err = vmpi_register_read_callback(priv->vmpi.mpi, shim_hv_recv_callback, priv);
        if (err) {
                LOG_ERR("%s: vmpi_register_read_callback() failed", __func__);
                goto read_callback;
        }

        /* Add this IPC process to the factory global list. */
        list_add(&priv->list, &factory_data->instances);

        return ipcp;

read_callback:
        name_fini(&priv->name);
alloc_name:
        rkfree(priv);
alloc_data:
        rkfree(ipcp);
alloc_ipcp:
        LOG_ERR("%s: failed\n", __func__);

        return NULL;
}

/* Destroy a shim IPC process. */
static int
shim_hv_factory_ipcp_destroy(struct ipcp_factory_data *factory_data,
                             struct ipcp_instance *ipcp)
{
        struct ipcp_instance_data *cur, *next, *found;

        ASSERT(factory_data);
        ASSERT(ipcp);

        found = NULL;
        list_for_each_entry_safe(cur, next, &factory_data->instances, list) {
                if (cur == ipcp->data) {
                        list_del(&cur->list);
                        found = cur;
                        break;
                }
        }

        if (!found) {
                LOG_ERR("%s: entry not found", __func__);
        }

        name_fini(&ipcp->data->name);
        name_fini(&ipcp->data->dif_name);
        rkfree(ipcp->data);
        rkfree(ipcp);

        return 0;
}

static struct ipcp_factory_ops shim_hv_factory_ops = {
        .init      = shim_hv_factory_init,
        .fini      = shim_hv_factory_fini,
        .create    = shim_hv_factory_ipcp_create,
        .destroy   = shim_hv_factory_ipcp_destroy,
};

/* A factory for shim IPC processes for hypervisors. */
static struct ipcp_factory *shim_hv_ipcp_factory = NULL;

/* As a first solution, this shim initialization and uninitialization
 * functions are called by the vmpi code. This is why these functions
 * are not static.
 */
int
shim_hv_init(vmpi_info_t *mpi)
{
        ASSERT(mpi);
        gmpi = mpi;

        shim_hv_ipcp_factory = kipcm_ipcp_factory_register(
                        default_kipcm, SHIM_HV_NAME,
                        &shim_hv_factory_data,
                        &shim_hv_factory_ops);

        if (shim_hv_ipcp_factory == NULL) {
                LOG_ERR("%s: factory registration failed\n", __func__);
                return -1;
        }

        LOG_INFO("%s: success", __func__);

        return 0;
}

void
shim_hv_fini(void)
{
        ASSERT(shim_hv_ipcp_factory);

        kipcm_ipcp_factory_unregister(default_kipcm, shim_hv_ipcp_factory);

        LOG_INFO("%s", __func__);
}

