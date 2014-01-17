/*
 * RMT (Relaying and Multiplexing Task)
 *
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

#include <linux/export.h>
#include <linux/types.h>
#include <linux/hashtable.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>

#define RINA_PREFIX "rmt"

#include "logs.h"
#include "utils.h"
#include "debug.h"
#include "du.h"
#include "rmt.h"
#include "pft.h"
#include "efcp-utils.h"

#define rmap_hash(T, K) hash_min(K, HASH_BITS(T))

struct rmt_queue {
        struct rfifo *    queue;
        port_id_t         port_id;
        struct hlist_node hlist;
};

static struct rmt_queue * queue_create(port_id_t id)
{
        struct rmt_queue * tmp;

        ASSERT(is_port_id_ok(id));

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        tmp->queue = rfifo_create();
        if (!tmp->queue) {
                rkfree(tmp);
                return NULL;
        }

        INIT_HLIST_NODE(&tmp->hlist);

        tmp->port_id = id;

        LOG_DBG("Queue %pK created successfully (port-id = %d)", tmp, id);

        return tmp;
}

static int queue_destroy(struct rmt_queue * q)
{
        ASSERT(q);
        ASSERT(q->queue);

        LOG_DBG("Destroying queue %pK (port-id = %d)", q, q->port_id);

        hash_del(&q->hlist);

        if (q->queue) rfifo_destroy(q->queue, (void (*)(void *)) pdu_destroy);
        rkfree(q);

        return 0;
}

struct rmt_qmap {
        DECLARE_HASHTABLE(queues, 7);
        spinlock_t    lock;   /* FIXME: Has to be moved in the pipelines */
        int           in_use; /* FIXME: Use rwqo and remove in_use */
};

static struct rmt_qmap * qmap_create(void)
{
        struct rmt_qmap * tmp;

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        hash_init(tmp->queues);
        spin_lock_init(&tmp->lock);
        tmp->in_use = 0;

        return tmp;
}

static int qmap_destroy(struct rmt_qmap * m)
{
        struct rmt_queue *  entry;
        struct hlist_node * tmp;
        int                 bucket;

        ASSERT(m);

        hash_for_each_safe(m->queues, bucket, tmp, entry, hlist) {
                ASSERT(entry);

                if (queue_destroy(entry)) {
                        LOG_ERR("Could not destroy entry %pK", entry);
                        return -1;
                }
        }

        rkfree(m);

        return 0;
}

static struct rmt_queue * qmap_find(struct rmt_qmap * m,
                                    port_id_t         id)
{
        struct rmt_queue *        entry;
        const struct hlist_head * head;

        ASSERT(m);

        if (!is_port_id_ok(id))
                return NULL;

        head = &m->queues[rmap_hash(m->queues, id)];
        hlist_for_each_entry(entry, head, hlist) {
                if (entry->port_id == id)
                        return entry;
        }

        return NULL;
}

struct pft_cache {
        port_id_t * pids;  /* Array of port_id_t */
        size_t      count; /* Entries in the pids array */
};

static int pft_cache_init(struct pft_cache * c)
{
        ASSERT(c);

        c->pids  = NULL;
        c->count = 0;

        LOG_DBG("PFT cache %pK initialized", c);

        return 0;
}

static int pft_cache_fini(struct pft_cache * c)
{
        ASSERT(c);

        if (c->count) {
                ASSERT(c->pids);
                rkfree(c->pids);
        } else {
                ASSERT(!c->pids);
        }

        LOG_DBG("PFT cache %pK destroyed", c);

        return 0;
}

struct rmt {
        address_t               address;
        struct ipcp_instance *  parent;
        struct pft *            pft;
        struct kfa *            kfa;
        struct efcp_container * efcpc;

        struct {
                struct workqueue_struct * wq;
                struct rmt_qmap *         queues;
                struct pft_cache          cache;
        } ingress;

        struct {
                struct workqueue_struct * wq;
                struct rmt_qmap *         queues;
                struct pft_cache          cache;
        } egress;
};

#define MAX_NAME_SIZE 128

static const char * create_name(const char *       prefix,
                                const struct rmt * instance)
{
        static char name[MAX_NAME_SIZE];

        ASSERT(prefix);
        ASSERT(instance);

        if (snprintf(name, sizeof(name),
                     RINA_PREFIX "-%s-%pK", prefix, instance) >=
            sizeof(name))
                return NULL;

        return name;
}

struct rmt * rmt_create(struct ipcp_instance *  parent,
                        struct kfa *            kfa,
                        struct efcp_container * efcpc)
{
        struct rmt * tmp;
        const char * name;

        if (!parent || !kfa || !efcpc) {
                LOG_ERR("Bogus input parameters");
                return NULL;
        }

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        tmp->address = address_bad();
        tmp->parent  = parent;
        tmp->kfa     = kfa;
        tmp->efcpc   = efcpc;
        tmp->pft     = pft_create();
        if (!tmp->pft) {
                rmt_destroy(tmp);
                return NULL;
        }

        /* Egress */
        name = create_name("egress-wq", tmp);
        if (!name) {
                rmt_destroy(tmp);
                return NULL;
        }
        tmp->egress.wq = rwq_create(name);
        if (!tmp->egress.wq) {
                rmt_destroy(tmp);
                return NULL;
        }
        tmp->egress.queues = qmap_create();
        if (!tmp->egress.queues) {
                rmt_destroy(tmp);
                return NULL;
        }
        if (pft_cache_init(&tmp->egress.cache)) {
                rmt_destroy(tmp);
                return NULL;
        }

        /* Ingress */
        name = create_name("ingress-wq", tmp);
        if (!name) {
                rmt_destroy(tmp);
                return NULL;
        }
        tmp->ingress.wq = rwq_create(name);
        if (!tmp->ingress.wq) {
                rmt_destroy(tmp);
                return NULL;
        }
        tmp->ingress.queues = qmap_create();
        if (!tmp->ingress.queues) {
                rmt_destroy(tmp);
                return NULL;
        }
        if (pft_cache_init(&tmp->ingress.cache)) {
                rmt_destroy(tmp);
                return NULL;
        }

        LOG_DBG("Instance %pK initialized successfully", tmp);

        return tmp;
}
EXPORT_SYMBOL(rmt_create);

int rmt_destroy(struct rmt * instance)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed, bailing out");
                return -1;
        }

        if (instance->ingress.wq)     rwq_destroy(instance->ingress.wq);
        if (instance->ingress.queues) qmap_destroy(instance->ingress.queues);
        pft_cache_fini(&instance->ingress.cache);

        if (instance->egress.wq)      rwq_destroy(instance->egress.wq);
        if (instance->egress.queues)  qmap_destroy(instance->egress.queues);
        pft_cache_fini(&instance->egress.cache);

        if (instance->pft)            pft_destroy(instance->pft);

        rkfree(instance);

        LOG_DBG("Instance %pK finalized successfully", instance);

        return 0;
}
EXPORT_SYMBOL(rmt_destroy);

int rmt_address_set(struct rmt * instance,
                    address_t    address)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (is_address_ok(instance->address)) {
                LOG_ERR("The RMT is already configured");
                return -1;
        }

        instance->address = address;

        return 0;
}
EXPORT_SYMBOL(rmt_address_set);

static int send_worker(void * o)
{
        struct rmt *        tmp;
        struct rmt_queue *  entry;
        bool                out;
        struct hlist_node * ntmp;
        int                 bucket;

        LOG_DBG("Send worker called");

        tmp = (struct rmt *) o;
        if (!tmp) {
                LOG_ERR("No instance passed to send worker !!!");
                return -1;
        }

        out = false;
        while (!out) {
                out = true;
                hash_for_each_safe(tmp->egress.queues->queues,
                                   bucket,
                                   ntmp,
                                   entry,
                                   hlist) {
                        struct sdu * sdu;
                        struct pdu * pdu;
                        port_id_t    port_id;

                        spin_lock(&tmp->egress.queues->lock);
                        pdu     = (struct pdu *) rfifo_pop(entry->queue);
                        port_id = entry->port_id;
                        spin_unlock(&tmp->egress.queues->lock);

                        if (!pdu)
                                break;

                        out = false;
                        sdu = sdu_create_pdu_with(pdu);
                        if (!sdu)
                                break;

                        LOG_DBG("Gonna SEND sdu to port_id %d", port_id);
                        if (kfa_flow_sdu_write(tmp->kfa, port_id, sdu)) {
                                LOG_ERR("Couldn't write SDU to KFA");
                        }
                }
        }

        spin_lock(&tmp->egress.queues->lock);
        tmp->egress.queues->in_use = 0;
        spin_unlock(&tmp->egress.queues->lock);

        return 0;
}

int rmt_send_port_id(struct rmt * instance,
                     port_id_t    id,
                     struct pdu * pdu)
{
        struct rmt_queue *     squeue;
        struct rwq_work_item * item;

        if (!pdu_is_ok(pdu)) {
                LOG_ERR("Bogus PDU passed");
                return -1;
        }
        if (!instance) {
                LOG_ERR("Bogus RMT passed");
                pdu_destroy(pdu);
                return -1;
        }
        if (!instance->egress.queues) {
                LOG_ERR("No queues to push into");
                pdu_destroy(pdu);
                return -1;
        }

        spin_lock(&instance->egress.queues->lock);
        squeue = qmap_find(instance->egress.queues, id);
        if (!squeue) {
                spin_unlock(&instance->egress.queues->lock);
                return -1;
        }

        if (rfifo_push_ni(squeue->queue, pdu)) {
                spin_unlock(&instance->egress.queues->lock);
                return -1;
        }
        if (instance->egress.queues->in_use) {
                spin_unlock(&instance->egress.queues->lock);
                LOG_DBG("Work already posted, nothing more to do");
                return 0;
        }
        instance->egress.queues->in_use = 1;
        spin_unlock(&instance->egress.queues->lock);

        /* Is this _ni() call really necessary ??? */
        item = rwq_work_create_ni(send_worker, instance);
        if (!item) {
                LOG_ERR("Couldn't create work");
                return -1;
        }

        ASSERT(instance->egress.wq);

        if (rwq_work_post(instance->egress.wq, item)) {
                LOG_ERR("Couldn't put work in the ingress workqueue");
                return -1;
        }

        return 0;
}
EXPORT_SYMBOL(rmt_send_port_id);

int rmt_send(struct rmt * instance,
             address_t    address,
             qos_id_t     qos_id,
             struct pdu * pdu)
{
        int i;

        if (!instance) {
                LOG_ERR("Bogus RMT passed");
                return -1;
        }
        if (!pdu) {
                LOG_ERR("Bogus PDU passed");
                return -1;
        }

        if (pft_nhop(instance->pft,
                     address,
                     qos_id,
                     &(instance->egress.cache.pids),
                     &(instance->egress.cache.count))) {
                pdu_destroy(pdu);
                return -1;
        }

        /*
         * FIXME:
         *   pdu -> pci-> qos-id | cep_id_t -> connection -> qos-id (former)
         *   address + qos-id (pdu-fwd-t) -> port-id
         */

        for (i = 0; i < instance->egress.cache.count; i++) {
                LOG_DBG("Gonna send PDU to port_id: %d",
                        instance->egress.cache.pids[i]);
                if (rmt_send_port_id(instance,
                                     instance->egress.cache.pids[i],
                                     pdu))
                        LOG_ERR("Failed to send a PDU");
        }

        return 0;
}
EXPORT_SYMBOL(rmt_send);

static int __queue_send_add(struct rmt * instance,
                            port_id_t    id)
{
        struct rmt_queue * tmp;

        tmp = queue_create(id);
        if (!tmp)
                return -1;

        hash_add(instance->egress.queues->queues, &tmp->hlist, id);

        LOG_DBG("Added send queue to rmt %pK for port id %d", instance, id);

        return 0;
}

int rmt_queue_send_add(struct rmt * instance,
                       port_id_t    id)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (!is_port_id_ok(id)) {
                LOG_ERR("Wrong port id");
                return -1;
        }

        if (!instance->egress.queues) {
                LOG_ERR("Invalid RMT");
                return -1;
        }

        if (qmap_find(instance->egress.queues, id)) {
                LOG_ERR("Queue already exists");
                return -1;
        }

        return __queue_send_add(instance, id);
}
EXPORT_SYMBOL(rmt_queue_send_add);

int rmt_queue_send_delete(struct rmt * instance,
                          port_id_t    id)
{
        struct rmt_queue * q;

        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (!is_port_id_ok(id)) {
                LOG_ERR("Wrong port id");
                return -1;
        }

        q = qmap_find(instance->egress.queues, id);
        if (!q) {
                LOG_ERR("Queue does not exist");
                return -1;
        }

        return queue_destroy(q);
}
EXPORT_SYMBOL(rmt_queue_send_delete);

static int __queue_recv_add(struct rmt * instance,
                            port_id_t    id)
{
        struct rmt_queue * tmp;

        tmp = queue_create(id);
        if (!tmp)
                return -1;

        hash_add(instance->ingress.queues->queues, &tmp->hlist, id);

        LOG_DBG("Added receive queue to rmt %pK for port id %d", instance, id);

        return 0;
}

int rmt_queue_recv_add(struct rmt * instance,
                       port_id_t    id)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (!is_port_id_ok(id)) {
                LOG_ERR("Wrong port id");
                return -1;
        }

        if (!instance->ingress.queues) {
                LOG_ERR("Invalid RMT");
                return -1;
        }

        if (qmap_find(instance->ingress.queues, id)) {
                LOG_ERR("Queue already exists");
                return -1;
        }

        return __queue_recv_add(instance, id);
}
EXPORT_SYMBOL(rmt_queue_recv_add);

int rmt_queue_recv_delete(struct rmt * instance,
                          port_id_t    id)
{
        struct rmt_queue * q;

        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (!is_port_id_ok(id)) {
                LOG_ERR("Wrong port id");
                return -1;
        }

        q = qmap_find(instance->ingress.queues, id);
        if (!q) {
                LOG_ERR("Queue does not exist");
                return -1;
        }

        return queue_destroy(q);
}
EXPORT_SYMBOL(rmt_queue_recv_delete);

static struct pci * sdu_pci_copy(const struct sdu * sdu)
{
        if (!sdu_is_ok(sdu))
                return NULL;

        return pci_create_from(buffer_data_ro(sdu_buffer_ro(sdu)));
}

static int process_mgmt_sdu(struct rmt * rmt,
                            port_id_t    port_id,
                            struct sdu * sdu)
{
        struct buffer * buffer;
        struct pdu *    pdu;

        ASSERT(rmt);
        ASSERT(is_port_id_ok(port_id));
        ASSERT(sdu);

        pdu = pdu_create_with(sdu);
        if (!pdu) {
                LOG_ERR("Cannot get PDU from SDU");
                sdu_destroy(sdu);
                return -1;
        }

        buffer = pdu_buffer_get_rw(pdu);
        if (!buffer_is_ok(buffer)) {
                LOG_ERR("PDU has no buffer ???");
                return -1;
        }

        sdu = sdu_create_buffer_with(buffer);
        if (!sdu_is_ok(sdu)) {
                LOG_ERR("Cannot create SDU");
                pdu_destroy(pdu);
                return -1;
        }

        if (pdu_buffer_disown(pdu)) {
                pdu_destroy(pdu);
                /* FIXME: buffer is owned by PDU and SDU, we're leaking sdu */
                return -1;
        }

        pdu_destroy(pdu);

        ASSERT(rmt->parent);
        ASSERT(rmt->parent->ops);
        ASSERT(rmt->parent->ops->mgmt_sdu_post);

        return (rmt->parent->ops->mgmt_sdu_post(rmt->parent->data,
                                                port_id,
                                                sdu) ? -1 : 0);
}

static int process_dt_sdu(struct rmt *       rmt,
                          port_id_t          port_id,
                          struct sdu *       sdu,
                          struct rmt_queue * entry)
{
        struct pdu * pdu;
        cep_id_t     c;
        address_t    dest_add;

        /* (FUTURE) Address and qos-id are the same, do a single match only */
        pdu = pdu_create_with(sdu);
        if (!pdu) {
                LOG_ERR("Cannot get PDU from SDU");
                sdu_destroy(sdu);
                return -1;
        }
        dest_add = pci_destination(pdu_pci_get_ro(pdu));
        if (!is_address_ok(dest_add)) {
                LOG_ERR("Wrong destination address");
                return -1;
        }

        if (rmt->address != dest_add) {
                qos_id_t qos_id;
                int      i;

                qos_id = pci_qos_id(pdu_pci_get_ro(pdu));
                if (pft_nhop(rmt->pft,
                             dest_add,
                             qos_id,
                             &(rmt->ingress.cache.pids),
                             &(rmt->ingress.cache.count))) {
                        pdu_destroy(pdu);
                        return -1;
                }

                for (i = 0; i < rmt->ingress.cache.count; i++) {
                        if (kfa_flow_sdu_write(rmt->kfa,
                                               rmt->ingress.cache.pids[i],
                                               sdu))
                                LOG_ERR("Cannot write SDU to KFA port-id %d",
                                        rmt->ingress.cache.pids[i]);
                }

                return 0;
        }
        c = pci_cep_destination(pdu_pci_get_ro(pdu));
        if (!is_cep_id_ok(c)) {
                LOG_ERR("Wrong CEP-id in PDU");
                pdu_destroy(pdu);
                return -1;
        }

        if (efcp_container_receive(rmt->efcpc, c, pdu)) {
                LOG_ERR("EFCP container problems");
                return -1;
        }

        return 0;
}

static int receive_worker(void * o)
{
        struct rmt * tmp;
        bool         nothing_to_do;

        LOG_DBG("Receive worker called");

        tmp = (struct rmt *) o;
        if (!tmp) {
                LOG_ERR("No instance passed to receive worker !!!");
                return -1;
        }

        nothing_to_do = false;
        while (!nothing_to_do) {
                struct rmt_queue *  entry;
                int                 bucket;
                struct hlist_node * ntmp;

                nothing_to_do = true;
                hash_for_each_safe(tmp->ingress.queues->queues,
                                   bucket,
                                   ntmp,
                                   entry,
                                   hlist) {
                        struct sdu * sdu;
                        port_id_t    port_id;
                        pdu_type_t   pdu_type;
                        struct pci * pci;

                        ASSERT(entry);

                        spin_lock(&tmp->ingress.queues->lock);
                        sdu     = (struct sdu *) rfifo_pop(entry->queue);
                        port_id = entry->port_id;
                        spin_unlock(&tmp->ingress.queues->lock);

                        if (!sdu) {
                                LOG_ERR("No SDU to work with");
                                break;
                        }

                        nothing_to_do = false;
                        pci = sdu_pci_copy(sdu);
                        if (!pci) {
                                LOG_ERR("No PCI to work with");
                                break;
                        }

                        pdu_type = pci_type(pci);
                        if (!pdu_type_is_ok(pdu_type)) {
                                LOG_ERR("Wrong PDU type");
                                pci_destroy(pci);
                                sdu_destroy(sdu);
                                break;
                        }
                        LOG_DBG("PDU type: %d", pdu_type);

                        /* (FUTURE) PDU ownership is going to be passed on */

                        switch (pdu_type) {
                        case PDU_TYPE_MGMT:
                                process_mgmt_sdu(tmp, port_id, sdu);
                                break;
                        case PDU_TYPE_DT:
                                /*
                                 * (FUTURE)
                                 *
                                 * enqueue PDU in pdus_dt[dest-addr, qos-id]
                                 * don't process it now ...
                                 */
                                process_dt_sdu(tmp, port_id, sdu, entry);
                                break;
                        default:
                                LOG_ERR("Unknown PDU type %d", pdu_type);
                                sdu_destroy(sdu);
                                break;
                        }
                        pci_destroy(pci);

                        /* (FUTURE) foreach_end() */
                }
        }

        /* (FUTURE) for-each list in pdus_dt call process_dt_pdus(pdus_dt) */

        spin_lock(&tmp->ingress.queues->lock);
        tmp->ingress.queues->in_use = 0;
        spin_unlock(&tmp->ingress.queues->lock);

        return 0;
}

int rmt_receive(struct rmt * instance,
                struct sdu * sdu,
                port_id_t    from)
{
        struct rwq_work_item * item;
        struct rmt_queue *     rcv_queue;

        if (!sdu_is_ok(sdu)) {
                LOG_ERR("Bogus SDU passed");
                return -1;
        }
        if (!instance) {
                LOG_ERR("No RMT passed");
                sdu_destroy(sdu);
                return -1;
        }
        if (!is_port_id_ok(from)) {
                LOG_ERR("Wrong port id");
                sdu_destroy(sdu);
                return -1;
        }
        if (!instance->ingress.queues) {
                LOG_ERR("No ingress queues in RMT: %pK", instance);
                sdu_destroy(sdu);
                return -1;
        }

        spin_lock(&instance->ingress.queues->lock);
        rcv_queue = qmap_find(instance->ingress.queues, from);
        if (!rcv_queue) {
                spin_unlock(&instance->ingress.queues->lock);
                return -1;
        }

        if (rfifo_push_ni(rcv_queue->queue, sdu)) {
                spin_unlock(&instance->ingress.queues->lock);
                return -1;
        }
        if (instance->ingress.queues->in_use) {
                spin_unlock(&instance->ingress.queues->lock);
                LOG_DBG("Work already posted, nothing more to do");
                return 0;
        }
        instance->ingress.queues->in_use = 1;
        spin_unlock(&instance->ingress.queues->lock);

        /* Is this _ni() call really necessary ??? */
        item = rwq_work_create_ni(receive_worker, instance);
        if (!item) {
                LOG_ERR("Couldn't create rwq work");
                return -1;
        }

        ASSERT(instance->ingress.wq);

        if (rwq_work_post(instance->ingress.wq, item)) {
                LOG_ERR("Couldn't put work in the ingress workqueue");
                return -1;
        }

        return 0;
}

/* FIXME: To be rearranged */
static bool is_rmt_pft_ok(struct rmt * instance)
{ return (instance && instance->pft) ? true : false; }

int rmt_pft_add(struct rmt *       instance,
                address_t          destination,
                qos_id_t           qos_id,
                const port_id_t  * ports,
                size_t             count)
{
        return is_rmt_pft_ok(instance) ? pft_add(instance->pft,
                                                 destination,
                                                 qos_id,
                                                 ports,
                                                 count) : -1;
}
EXPORT_SYMBOL(rmt_pft_add);

int rmt_pft_remove(struct rmt *       instance,
                   address_t          destination,
                   qos_id_t           qos_id,
                   const port_id_t  * ports,
                   const size_t       count)
{
        return is_rmt_pft_ok(instance) ? pft_remove(instance->pft,
                                                    destination,
                                                    qos_id,
                                                    ports,
                                                    count) : -1;
}
EXPORT_SYMBOL(rmt_pft_remove);

int rmt_pft_dump(struct rmt *       instance,
                 struct list_head * entries)
{
        return is_rmt_pft_ok(instance) ? pft_dump(instance->pft,
                                                  entries) : -1;
}
EXPORT_SYMBOL(rmt_pft_dump);

#ifdef CONFIG_RINA_RMT_REGRESSION_TESTS
/* FIXME: Remove extern as soon as possible */
struct buffer * buffer_create_from_gfp(gfp_t        flags,
                                       const void * data,
                                       size_t       size);
static struct pdu * regression_tests_pdu_create(address_t address)
{
        struct buffer * buffer;
        struct pdu *    pdu;
        struct pci *    pci;
        char *          data = "Hello, world";

        buffer =  buffer_create_from_gfp(GFP_KERNEL, data, 13);
        if (!buffer) {
                LOG_DBG("Failed to create buffer");
                return NULL;
        }
        pci = pci_create();
        if (!pci) {
                buffer_destroy(buffer);
                return NULL;
        }

        if (pci_format(pci,
                       0,
                       0,
                       address,
                       0,
                       0,
                       0,
                       PDU_TYPE_MGMT)) {
                buffer_destroy(buffer);
                pci_destroy(pci);
                return NULL;
        }

        pdu = pdu_create();
        if (!pdu) {
                buffer_destroy(buffer);
                pci_destroy(pci);
                return NULL;
        }

        if (pdu_buffer_set(pdu, buffer)) {
                pdu_destroy(pdu);
                buffer_destroy(buffer);
                pci_destroy(pci);
                return NULL;
        }

        if (pdu_pci_set(pdu, pci)) {
                pdu_destroy(pdu);
                pci_destroy(pci);
                return NULL;
        }

        return pdu;
}
static bool regression_tests_egress_queue(void)
{
        struct rmt *        rmt;
        struct rmt_queue *  tmp;
        port_id_t           id;
        struct pdu *        pdu;
        address_t           address;
        const char *        name;
        struct rmt_queue *  entry;
        bool                out;
        struct hlist_node * ntmp;
        int                 bucket;

        address = 11;

        rmt = rkzalloc(sizeof(*rmt), GFP_KERNEL);
        if (!rmt) {
                LOG_DBG("Could not malloc memory for RMT");
                return false;
        }

        LOG_DBG("Creating a new qmap instance");
        rmt->egress.queues = qmap_create();
        if (!rmt->egress.queues) {
                LOG_DBG("Failed to create qmap");
                rmt_destroy(rmt);
                return false;
        }

        LOG_DBG("Creating rmt-egress-wq");
        name = create_name("egress-wq", rmt);
        if (!name) {
                rmt_destroy(rmt);
                return false;
        }
        rmt->egress.wq = rwq_create(name);
        if (!rmt->egress.wq) {
                rmt_destroy(rmt);
                return false;
        }

        id = 1;
        if (rmt_queue_send_add(rmt, id)) {
                LOG_DBG("Failed to add queue");
                rmt_destroy(rmt);
                return false;
        }
        LOG_DBG ("Added to qmap");

        tmp = qmap_find(rmt->egress.queues, id);
        if (!tmp) {
                LOG_DBG("Failed to retrieve queue");
                rmt_destroy(rmt);
                return false;
        }

        tmp = NULL;

        pdu = regression_tests_pdu_create(address);
        if (!pdu) {
                LOG_DBG("Failed to create pdu");
                rmt_destroy(rmt);
                return false;
        }

        LOG_DBG("Data: %s", (char *) buffer_data_ro(pdu_buffer_get_ro(pdu)));
        LOG_DBG("Length: %d", buffer_length(pdu_buffer_get_ro(pdu)));
        LOG_DBG("PDU Type: %X", pci_type(pdu_pci_get_ro(pdu)));
        LOG_DBG("PCI Length: %d", pci_length(pdu_pci_get_ro(pdu)));

        LOG_DBG("Pushing PDU into queues");
        spin_lock(&rmt->egress.queues->lock);
        tmp = qmap_find(rmt->egress.queues, id);
        if (!tmp) {
                spin_unlock(&rmt->egress.queues->lock);
                pdu_destroy(pdu);
                rmt_destroy(rmt);
                return false;
        }

        if (rfifo_push_ni(tmp->queue, pdu)) {
                spin_unlock(&rmt->egress.queues->lock);
                pdu_destroy(pdu);
                rmt_destroy(rmt);
                return false;
        }
        spin_unlock(&rmt->egress.queues->lock);

        out = false;
        while (!out) {
                out = true;
                hash_for_each_safe(rmt->egress.queues->queues,
                                   bucket,
                                   ntmp,
                                   entry,
                                   hlist) {
                        struct sdu * sdu;
                        port_id_t    port_id;

                        spin_lock(&rmt->egress.queues->lock);
                        pdu     = (struct pdu *) rfifo_pop(entry->queue);
                        port_id = entry->port_id;
                        spin_unlock(&rmt->egress.queues->lock);

                        if (!pdu) {
                                LOG_DBG("Where is our PDU???");
                                break;
                        }
                        out = false;
                        sdu = sdu_create_pdu_with(pdu);
                        if (!sdu) {
                                LOG_DBG("Where is our SDU???");
                                break;
                        }

                        if (sdu_destroy(sdu)) {
                                LOG_DBG("Failed destruction of SDU");
                                LOG_DBG("SDU was not ok");
                        }
                }
        }

        if (queue_destroy(tmp)) {
                LOG_DBG("Failed to destroy queue");
                rmt_destroy(rmt);
                return false;
        }

        rmt_destroy(rmt);

        return true;
}

static bool regression_tests_process_mgmt_sdu(struct rmt * rmt,
                                              port_id_t    port_id,
                                              struct sdu * sdu)
{
        struct buffer * buffer;
        struct pdu *    pdu;

        pdu = pdu_create_with(sdu);
        if (!pdu) {
                LOG_DBG("Cannot get PDU from SDU");
                sdu_destroy(sdu);
                return false;
        }

        buffer = pdu_buffer_get_rw(pdu);
        if (!buffer_is_ok(buffer)) {
                LOG_DBG("PDU has no buffer ???");
                return false;
        }

        sdu = sdu_create_buffer_with(buffer);
        if (!sdu_is_ok(sdu)) {
                LOG_DBG("Cannot create SDU");
                pdu_destroy(pdu);
                return false;
        }

        if (pdu_buffer_disown(pdu)) {
                pdu_destroy(pdu);
                /* FIXME: buffer is owned by PDU and SDU, we're leaking sdu */
                return false;
        }

        pdu_destroy(pdu);

        if (sdu_destroy(sdu)) {
                LOG_DBG("Cannot destroy SDU something bad happened");
                return false;
        }

        return true;
}

static bool regression_tests_ingress_queue(void)
{
        struct rmt *           rmt;
        struct rmt_queue *     tmp;
        port_id_t              id;
        struct sdu *           sdu;
        struct pdu *           pdu;
        address_t              address;
        const char *           name;
        bool                   nothing_to_do;

        address = 17;

        rmt = rkzalloc(sizeof(*rmt), GFP_KERNEL);
        if (!rmt) {
                LOG_DBG("Could not malloc memory for RMT");
                return false;
        }

        LOG_DBG("Creating a qmap instance for ingress");
        rmt->ingress.queues = qmap_create();
        if (!rmt->ingress.queues) {
                LOG_DBG("Failed to create qmap");
                rmt_destroy(rmt);
                return false;
        }

        LOG_DBG("Creating rmt-ingress-wq");
        name = create_name("ingress-wq", rmt);
        if (!name) {
                rmt_destroy(rmt);
                return false;
        }
        rmt->ingress.wq = rwq_create(name);
        if (!rmt->ingress.wq) {
                rmt_destroy(rmt);
                return false;
        }

        id = 1;
        if (rmt_queue_recv_add(rmt, id)) {
                LOG_DBG("Failed to add queue");
                rmt_destroy(rmt);
                return false;
        }
        LOG_DBG ("Added to qmap");

        tmp = qmap_find(rmt->ingress.queues, id);
        if (!tmp) {
                LOG_DBG("Failed to retrieve queue");
                rmt_destroy(rmt);
                return false;
        }
        tmp = NULL;

        pdu = regression_tests_pdu_create(address);
        if (!pdu) {
                LOG_DBG("Failed to create pdu");
                rmt_destroy(rmt);
                return false;
        }

        sdu = sdu_create_pdu_with(pdu);
        if (!sdu) {
                LOG_DBG("Failed to create SDU");
                pdu_destroy(pdu);
                rmt_destroy(rmt);
                return false;
        }
        spin_lock(&rmt->ingress.queues->lock);
        tmp = qmap_find(rmt->ingress.queues, id);
        if (!tmp) {
                spin_unlock(&rmt->ingress.queues->lock);
                sdu_destroy(sdu);
                rmt_destroy(rmt);
                return false;
        }

        if (rfifo_push_ni(tmp->queue, sdu)) {
                spin_unlock(&rmt->ingress.queues->lock);
                sdu_destroy(sdu);
                rmt_destroy(rmt);
                return false;
        }
        spin_unlock(&rmt->ingress.queues->lock);

        nothing_to_do = false;
        while (!nothing_to_do) {
                struct rmt_queue *  entry;
                int                 bucket;
                struct hlist_node * ntmp;

                nothing_to_do = true;
                hash_for_each_safe(rmt->ingress.queues->queues,
                                   bucket,
                                   ntmp,
                                   entry,
                                   hlist) {
                        struct sdu * sdu;
                        port_id_t    pid;
                        pdu_type_t   pdu_type;
                        struct pci * pci;

                        ASSERT(entry);

                        spin_lock(&rmt->ingress.queues->lock);
                        sdu     = (struct sdu *) rfifo_pop(entry->queue);
                        pid = entry->port_id;
                        spin_unlock(&rmt->ingress.queues->lock);

                        if (!sdu) {
                                LOG_DBG("No SDU to work with");
                                break;
                        }

                        nothing_to_do = false;
                        pci = sdu_pci_copy(sdu);
                        if (!pci) {
                                LOG_DBG("No PCI to work with");
                                break;
                        }

                        pdu_type = pci_type(pci);
                        if (!pdu_type_is_ok(pdu_type)) {
                                LOG_ERR("Wrong PDU type");
                                pci_destroy(pci);
                                sdu_destroy(sdu);
                                break;
                        }
                        LOG_DBG("PDU type: %X", pdu_type);
                        switch (pdu_type) {
                        case PDU_TYPE_MGMT:
                                regression_tests_process_mgmt_sdu(rmt,
                                                                  pid,
                                                                  sdu);
                                break;
                        case PDU_TYPE_DT:
                                /*
                                 * (FUTURE)
                                 *
                                 * enqueue PDU in pdus_dt[dest-addr, qos-id]
                                 * don't process it now ...
                                 *
                                 * process_dt_sdu(rmt, port_id, sdu, entry);
                                 */
                                break;
                        default:
                                LOG_ERR("Unknown PDU type %d", pdu_type);
                                sdu_destroy(sdu);
                                break;
                        }
                        pci_destroy(pci);

                        /* (FUTURE) foreach_end() */
                }
        }

        if (queue_destroy(tmp)) {
                LOG_DBG("Failed to destroy queue");
                rmt_destroy(rmt);
                return false;
        }

        rmt_destroy(rmt);

        return true;
}

bool regression_tests_rmt(void)
{
        if (!regression_tests_egress_queue()) {
                LOG_ERR("Failed regression test on egress queues");
                return false;
        }

        if (!regression_tests_ingress_queue()) {
                LOG_ERR("Failed regression test on ingress queues");
                return false;
        }

        return true;
}
#endif
