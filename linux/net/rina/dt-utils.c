/*
 * DT (Data Transfer)
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *    Sander Vrijders       <sander.vrijders@intec.ugent.be>
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

#define RINA_PREFIX "dt-utils"

#include "logs.h"
#include "utils.h"
#include "debug.h"
#include "dt-utils.h"
#include "dt.h"

struct cwq {
        struct rqueue * q;
        spinlock_t      lock;
};

struct cwq * cwq_create(void)
{
        struct cwq * tmp;

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        tmp->q = rqueue_create();
        if (!tmp->q) {
                LOG_ERR("Failed to create closed window queue");
                return NULL;
        }

        spin_lock_init(&tmp->lock);

        return tmp;
}


int cwq_destroy(struct cwq * queue)
{
        if (!queue)
                return -1;

        ASSERT(queue->q);

        if (rqueue_destroy(queue->q,
                           (void (*)(void *)) pdu_destroy)) {
                LOG_ERR("Failed to destroy closed window queue");
                return -1;
        }

        rkfree(queue);

        return 0;
}

int cwq_push(struct cwq * queue,
             struct pdu * pdu)
{
        if (!queue)
                return -1;

        if (!pdu_is_ok(pdu)) {
                LOG_ERR("Bogus PDU passed");
                return -1;
        }

        spin_lock(&queue->lock);
        if (rqueue_tail_push(queue->q, pdu)) {
                LOG_ERR("Failed to add PDU");
                pdu_destroy(pdu);
                spin_unlock(&queue->lock);
                return -1;
        }
        spin_unlock(&queue->lock);

        return 0;
}


struct pdu * cwq_pop(struct cwq * queue)
{
        struct pdu * tmp;

        if (!queue)
                return NULL;

        spin_lock(&queue->lock);
        tmp = (struct pdu *) rqueue_head_pop(queue->q);
        spin_unlock(&queue->lock);

        if (!tmp) {
                LOG_ERR("Failed to retrieve PDU");
                return NULL;
        }

        return tmp;
}

bool cwq_is_empty(struct cwq * queue)
{
        bool ret;

        if (!queue)
                return false;

        spin_lock(&queue->lock);
        ret = rqueue_is_empty(queue->q);
        spin_unlock(&queue->lock);

        return ret;
}

size_t cwq_size(struct cwq * queue)
{
        LOG_MISSING;
        return 0;
}

struct rtxq_entry {
        unsigned long    time_stamp;
        struct pdu *     pdu;
        int              retries;
        struct list_head next;
};

static struct rtxq_entry * rtxq_entry_create_gfp(struct pdu * pdu, gfp_t flag)
{
        struct rtxq_entry * tmp;

        ASSERT(pdu_is_ok(pdu));

        tmp = rkzalloc(sizeof(*tmp), flag);
        if (!tmp)
                return NULL;

        tmp->pdu        = pdu;
        tmp->time_stamp = jiffies;
        tmp->retries    = 1;

        INIT_LIST_HEAD(&tmp->next);

        return tmp;
}

static struct rtxq_entry * rtxq_entry_create_ni(struct pdu * pdu)
{return rtxq_entry_create_gfp(pdu, GFP_ATOMIC);}

struct rtxqueue {
        struct list_head head;
};

static struct rtxqueue * rtxqueue_create(void)
{
        struct rtxqueue * tmp;

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        return tmp;
}

static int rtxq_entry_destroy(struct rtxq_entry * entry)
{
        if (!entry)
                return -1;

        pdu_destroy(entry->pdu);
        rkfree(entry);

        return 0;
}

static int entries_ack(struct rtxqueue * q,
                       seq_num_t         seq_num)
{
        struct rtxq_entry * cur, * n;

        ASSERT(q);

        list_for_each_entry_safe(cur, n, &q->head, next) {
                if (pci_sequence_number_get(pdu_pci_get_rw((cur->pdu)))
                    <= seq_num) {
                        list_del(&cur->next);
                        rtxq_entry_destroy(cur);
                }
        }

        return 0;
}

static int entries_nack(struct rtxqueue * q,
                        seq_num_t         seq_num)
{
        struct rtxq_entry * cur, * n;
        /* struct pdu * tmp; */

        ASSERT(q);

        list_for_each_entry_safe(cur, n, &q->head, next) {
                if (pci_sequence_number_get(pdu_pci_get_rw((cur->pdu)))
                    >= seq_num) {
                        /*
                         * FIXME: We need a way to use the RMT
                         * tmp = pdu_dup_ni(pdu);
                         * rmt_send(pdu);
                         */
                }
        }

        return 0;
}

static int rtxqueue_destroy(struct rtxqueue * q)
{
        struct rtxq_entry * cur;
        struct rtxq_entry * n;

        if (!q)
                return -1;

        list_for_each_entry_safe(cur, n, &q->head, next) {
                rtxq_entry_destroy(cur);
        }

        rkfree(q);

        return 0;

}

static int rtxqueue_push(struct rtxqueue * q, struct pdu * pdu)
{
        struct rtxq_entry * tmp;

        if (!q)
                return -1;

        if (!pdu_is_ok(pdu))
                return -1;

        tmp = rtxq_entry_create_ni(pdu);
        if (!tmp)
                return -1;

        list_add(&tmp->next, &q->head);

        return 0;
}

static int rtxqueue_rtx(struct rtxqueue * q, unsigned int tr)
{
        return 0;
}

struct rtxq {
        spinlock_t        lock;
        struct rtimer *   r_timer;
        struct dt *       parent;
        struct rtxqueue * queue;
};

static void Rtimer_handler(void * data)
{
        struct rtxq * q;

        q = (struct rtxq *) data;
        if (!q) {
                LOG_ERR("No RTXQ to work with");
                return;
        }

        rtxqueue_rtx(q->queue, dt_sv_tr(q->parent));
        rtimer_restart(q->r_timer, dt_sv_tr(q->parent));
}

int rtxq_destroy(struct rtxq * q)
{
        if (!q)
                return -1;

        if (rtimer_destroy(q->r_timer))
                LOG_ERR("Problems destroying timer for RTXQ %pK", q);

        if (rtxqueue_destroy(q->queue))
                LOG_ERR("Problems destroying queue for RTXQ %pK", q->queue);

        rkfree(q);

        return 0;
}

struct rtxq * rtxq_create(struct dt * dt)
{
        struct rtxq * tmp;

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp)
                return NULL;

        tmp->r_timer = rtimer_create(Rtimer_handler, tmp);
        if (!tmp->r_timer) {
                LOG_ERR("Failed to create retransmission queue");
                rtxq_destroy(tmp);
                return NULL;
        }

        tmp->queue = rtxqueue_create();
        if (!tmp->queue) {
                LOG_ERR("Failed to create retransmission queue");
                rtxq_destroy(tmp);
                return NULL;
        }

        tmp->parent = dt;

        spin_lock_init(&tmp->lock);

        return tmp;
}

int rtxq_push(struct rtxq * q,
              struct pdu *  pdu)
{

        if (!q || !pdu_is_ok(pdu))
                return -1;

        spin_lock(&q->lock);
        rtxqueue_push(q->queue, pdu);
        spin_unlock(&q->lock);

        return 0;
}

int rtxq_ack(struct rtxq * q,
             seq_num_t     seq_num,
             unsigned int  tr)
{
        if (!q)
                return -1;

        spin_lock(&q->lock);
        entries_ack(q->queue, seq_num);
        if (rtimer_restart(q->r_timer, tr)) {
                spin_unlock(&q->lock);
                return -1;
        }
        spin_unlock(&q->lock);

        return 0;
}

int rtxq_nack(struct rtxq * q,
              seq_num_t     seq_num,
              unsigned int  tr)
{
        if (!q)
                return -1;

        spin_lock(&q->lock);
        entries_nack(q->queue, seq_num);
        if (rtimer_restart(q->r_timer, tr)) {
                spin_unlock(&q->lock);
                return -1;
        }
        spin_unlock(&q->lock);

        return 0;
}

int rtxq_set_pop(struct rtxq *      q,
                 seq_num_t          from,
                 seq_num_t          to,
                 struct list_head * p)
{
        if (!q)
                return -1;

        LOG_MISSING;

        return -1;
}

