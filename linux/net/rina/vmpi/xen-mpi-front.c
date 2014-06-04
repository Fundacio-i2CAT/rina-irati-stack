/* A vmpi-impl hypervisor implementation for Xen (guest-side)
 *
 * Copyright 2014 Vincenzo Maffione <v.maffione@nextworks.it> Nextworks
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/slab.h>

#include <asm/xen/page.h>
#include <xen/xen.h>
#include <xen/xenbus.h>
#include <xen/events.h>
#include <xen/page.h>
#include <xen/platform_pci.h>
#include <xen/grant_table.h>

#include "xen-mpi-front.h"
#include <xen/interface/memory.h>
#include <xen/interface/grant_table.h>

#include "vmpi-structs.h"
#include "vmpi-guest-impl.h"


//#define VERBOSE
#ifdef VERBOSE
#define IFV(x) x
#else   /* !VERBOSE */
#define IFV(x)
#endif  /* !VERBOSE */

#define VMPI_BUFFER_SIZE_XEN    2000

#define GRANT_INVALID_REF	0

#define NET_TX_RING_SIZE __CONST_RING_SIZE(xen_mpi_tx, PAGE_SIZE)
#define NET_RX_RING_SIZE __CONST_RING_SIZE(xen_mpi_rx, PAGE_SIZE)
#define TX_MAX_TARGET min_t(int, NET_TX_RING_SIZE, 256)

struct vmpi_impl_info {
	struct list_head list;

	/* Split event channels support, tx_* == rx_* when using
	 * single event channel.
	 */
	unsigned int tx_evtchn, rx_evtchn;
	unsigned int tx_irq, rx_irq;
	/* Only used when split event channels support is enabled */
	char tx_irq_name[IFNAMSIZ+4]; /* DEVNAME-tx */
	char rx_irq_name[IFNAMSIZ+4]; /* DEVNAME-rx */

	struct xenbus_device *xbdev;

	spinlock_t   tx_lock;
	struct xen_mpi_tx_front_ring tx;
	int tx_ring_ref;

	/*
	 * {tx,rx}_skbs store outstanding skbuffs. Free tx_skb entries
	 * are linked from tx_skb_freelist through skb_entry.link.
	 *
	 *  NB. Freelist index entries are always going to be less than
	 *  PAGE_OFFSET, whereas pointers to skbs will always be equal or
	 *  greater than PAGE_OFFSET: we use this property to distinguish
	 *  them.
	 */
	union skb_entry {
                struct vmpi_buffer *buf;
		unsigned long link;
	} tx_skbs[NET_TX_RING_SIZE];
	grant_ref_t gref_tx_head;
	grant_ref_t grant_tx_ref[NET_TX_RING_SIZE];
	struct page *grant_tx_page[NET_TX_RING_SIZE];
	unsigned tx_skb_freelist;

	spinlock_t   rx_lock ____cacheline_aligned_in_smp;
	struct xen_mpi_rx_front_ring rx;
	int rx_ring_ref;
        struct work_struct recv_worker;

	/* Receive-ring batched refills. */
#define RX_MIN_TARGET 8
#define RX_DFL_MIN_TARGET 64
#define RX_MAX_TARGET min_t(int, NET_RX_RING_SIZE, 256)
	unsigned rx_min_target, rx_max_target, rx_target;
	struct vmpi_queue rx_batch;

	struct timer_list rx_refill_timer;

	struct vmpi_buffer *rx_skbs[NET_RX_RING_SIZE];
	grant_ref_t gref_rx_head;
	grant_ref_t grant_rx_ref[NET_RX_RING_SIZE];

        void *private;
        vmpi_impl_callback_t xmit_cb;
        vmpi_impl_callback_t recv_cb;
};

static struct vmpi_impl_info *instance = NULL;

static void skb_entry_set_link(union skb_entry *list, unsigned short id)
{
	list->link = id;
}

static int skb_entry_is_link(const union skb_entry *list)
{
	BUILD_BUG_ON(sizeof(list->buf) != sizeof(list->link));
	return (unsigned long)list->buf < PAGE_OFFSET;
}

/*
 * Access macros for acquiring freeing slots in tx_skbs[].
 */

static void add_id_to_freelist(unsigned *head, union skb_entry *list,
			       unsigned short id)
{
	skb_entry_set_link(&list[id], *head);
	*head = id;
}

static unsigned short get_id_from_freelist(unsigned *head,
					   union skb_entry *list)
{
	unsigned int id = *head;
	*head = list[id].link;
	return id;
}

static int xenmpi_rxidx(RING_IDX idx)
{
	return idx & (NET_RX_RING_SIZE - 1);
}

static struct vmpi_buffer *xenmpi_get_rx_skb(struct vmpi_impl_info *np,
					 RING_IDX ri)
{
	int i = xenmpi_rxidx(ri);
	struct vmpi_buffer *buf = np->rx_skbs[i];
	np->rx_skbs[i] = NULL;
	return buf;
}

static grant_ref_t xenmpi_get_rx_ref(struct vmpi_impl_info *np,
					    RING_IDX ri)
{
	int i = xenmpi_rxidx(ri);
	grant_ref_t ref = np->grant_rx_ref[i];
	np->grant_rx_ref[i] = GRANT_INVALID_REF;
	return ref;
}

static void rx_refill_timeout(unsigned long data)
{
	/*struct vmpi_impl_info *np = (struct vmpi_impl_info *)data;

        schedule_work(&np->recv_worker); *//* HZ/20 */
}

static int mpifront_tx_slot_available(struct vmpi_impl_info *np)
{
	return (np->tx.req_prod_pvt - np->tx.rsp_cons) <
		(TX_MAX_TARGET - MAX_SKB_FRAGS - 2);
}

static void xenmpi_maybe_wake_tx(struct vmpi_impl_info *np)
{
	if (mpifront_tx_slot_available(np)) {
        }
}

static void xenmpi_alloc_rx_buffers(struct vmpi_impl_info *np)
{
	unsigned short id;
        struct vmpi_buffer *buf;
	int i, batch_target, notify;
	RING_IDX req_prod = np->rx.req_prod_pvt;
	grant_ref_t ref;
	unsigned long mfn;
	struct xen_mpi_rx_request *req;

        printk("%s: I should not be called\n", __func__);

	/*
	 * Allocate skbuffs greedily, even though we batch updates to the
	 * receive ring. This creates a less bursty demand on the memory
	 * allocator, so should reduce the chance of failed allocation requests
	 * both for ourself and for other kernel subsystems.
	 */
	batch_target = np->rx_target - (req_prod - np->rx.rsp_cons);
	for (i = vmpi_queue_len(&np->rx_batch); i < batch_target; i++) {
                /* TODO allocate data as a page */
                buf = vmpi_buffer_create(2000);
		if (unlikely(!buf)) {
			/* Could not allocate any skbuffs. Try again later. */
			mod_timer(&np->rx_refill_timer,
				  jiffies + (HZ/10));

			/* Any skbuffs queued for refill? Force them out. */
			if (i != 0)
				goto refill;
			break;
                }

		vmpi_queue_push(&np->rx_batch, buf);
	}

	/* Is the batch large enough to be worthwhile? */
	if (i < (np->rx_target/2)) {
		if (req_prod > np->rx.sring->req_prod)
			goto push;
		return;
	}

	/* Adjust our fill target if we risked running out of buffers. */
	if (((req_prod - np->rx.sring->rsp_prod) < (np->rx_target / 4)) &&
	    ((np->rx_target *= 2) > np->rx_max_target))
		np->rx_target = np->rx_max_target;

 refill:
	for (;;) {
                buf = vmpi_queue_pop(&np->rx_batch);
		if (buf == NULL)
			break;

		id = xenmpi_rxidx(req_prod);
		BUG_ON(np->rx_skbs[id]);
		np->rx_skbs[id] = buf;

		ref = gnttab_claim_grant_reference(&np->gref_rx_head);
		BUG_ON((signed short)ref < 0);
                mfn = virt_to_mfn(buf->p);
		gnttab_grant_foreign_access_ref(ref,
						np->xbdev->otherend_id,
						mfn,
						0);

		req = RING_GET_REQUEST(&np->rx, req_prod);
		req->id = id;
		req->gref = np->grant_rx_ref[id] = ref;

                req_prod++;
	}
        pr_info("xen-mpi alloced %d rx bufs\n", req_prod - np->rx.req_prod_pvt);

	wmb();		/* barrier so backend seens requests */

	/* Above is a suitable barrier to ensure backend will see requests. */
	np->rx.req_prod_pvt = req_prod;
 push:
	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->rx, notify);
	if (notify)
		notify_remote_via_irq(np->rx_irq);
}

struct vmpi_buffer *vmpi_impl_get_written_buffer(vmpi_impl_info_t *np)
{
	RING_IDX cons, prod;
	unsigned short id;
        struct vmpi_buffer *buf = NULL;

	prod = np->tx.sring->rsp_prod;
	rmb(); /* Ensure we see responses up to 'prod'. */
        cons = np->tx.rsp_cons;

        if (cons != prod) {
                struct xen_mpi_tx_response *txrsp;

                txrsp = RING_GET_RESPONSE(&np->tx, cons);
                /*if (txrsp->status == XEN_NETIF_RSP_NULL)
                        continue; */

                id  = txrsp->id;
                buf = np->tx_skbs[id].buf;
                if (unlikely(gnttab_query_foreign_access(
                                                np->grant_tx_ref[id]) != 0)) {
                        pr_alert("%s: warning -- grant still in use by backend domain\n",
                                        __func__);
                        BUG();
                }
                gnttab_end_foreign_access_ref(
                                np->grant_tx_ref[id], GNTMAP_readonly);
                gnttab_release_grant_reference(
                                &np->gref_tx_head, np->grant_tx_ref[id]);
                np->grant_tx_ref[id] = GRANT_INVALID_REF;
                np->grant_tx_page[id] = NULL;
                add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, id);

                IFV(printk("%s: buf %p, freed id %d, cons %d\n", __func__, buf, id, cons));

                np->tx.rsp_cons = ++cons;
        }

        return buf;
}

static void xenmpi_tx_buf_gc(struct vmpi_impl_info *np)
{
	RING_IDX cons, prod;
	unsigned short id;
        struct vmpi_buffer *buf;

        printk("%s: I should not be called\n", __func__);

	do {
		prod = np->tx.sring->rsp_prod;
		rmb(); /* Ensure we see responses up to 'rp'. */

		for (cons = np->tx.rsp_cons; cons != prod; cons++) {
			struct xen_mpi_tx_response *txrsp;

			txrsp = RING_GET_RESPONSE(&np->tx, cons);
			if (txrsp->status == XEN_NETIF_RSP_NULL)
				continue;

			id  = txrsp->id;
			buf = np->tx_skbs[id].buf;
			if (unlikely(gnttab_query_foreign_access(
				np->grant_tx_ref[id]) != 0)) {
				pr_alert("%s: warning -- grant still in use by backend domain\n",
					 __func__);
				BUG();
			}
			gnttab_end_foreign_access_ref(
				np->grant_tx_ref[id], GNTMAP_readonly);
			gnttab_release_grant_reference(
				&np->gref_tx_head, np->grant_tx_ref[id]);
			np->grant_tx_ref[id] = GRANT_INVALID_REF;
			np->grant_tx_page[id] = NULL;
			add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, id);
		}

		np->tx.rsp_cons = prod;

		/*
		 * Set a new event, then check for race with update of tx_cons.
		 * Note that it is essential to schedule a callback, no matter
		 * how few buffers are pending. Even if there is space in the
		 * transmit ring, higher layers may be blocked because too much
		 * data is outstanding: in such cases notification from Xen is
		 * likely to be the only kick that we'll get.
		 */
		np->tx.sring->rsp_event =
			prod + ((np->tx.sring->req_prod - prod) >> 1) + 1;
		mb();		/* update shared area */
	} while ((cons == prod) && (prod != np->tx.sring->rsp_prod));

	xenmpi_maybe_wake_tx(np);
}

int
vmpi_impl_write_buf(struct vmpi_impl_info *np, struct vmpi_buffer *buf)
{
	unsigned short id;
	struct xen_mpi_tx_request *tx = NULL;
	char *data = buf->p;
	unsigned int offset = offset_in_page(data);
	unsigned int len = buf->len;
	RING_IDX i;
	grant_ref_t ref;
	unsigned long mfn;
	int notify;
	unsigned long flags;

        if (unlikely(!len)) {
            net_alert_ratelimited("xenmpi: zero length buffer\n");
            goto drop;
        }

	/* If skb->len is too big for wire format, drop skb and alert
	 * user about misconfiguration.
	 */
	if (unlikely(len > XEN_NETIF_MAX_TX_SIZE)) {
		net_alert_ratelimited(
			"xenmpi: skb->len = %u, too big for wire format\n",
			len);
		goto drop;
	}

        /* XXX For now just truncate. */
        if (offset + len > PAGE_SIZE) {
                printk("%s: truncating off %d, len %d\n", __func__, offset, len);
                len = PAGE_SIZE - offset;
        }

	spin_lock_irqsave(&np->tx_lock, flags);

	if (unlikely(!instance)) {
		spin_unlock_irqrestore(&np->tx_lock, flags);
		goto drop;
	}

	i = np->tx.req_prod_pvt;

        while (len) {
                size_t slice = len;

                if (slice > PAGE_SIZE - offset) {
                        slice = PAGE_SIZE - offset;
                }

	        id = get_id_from_freelist(&np->tx_skb_freelist, np->tx_skbs);
	        np->tx_skbs[id].buf = buf;

	        ref = gnttab_claim_grant_reference(&np->gref_tx_head);
	        BUG_ON((signed short)ref < 0);
	        mfn = virt_to_mfn(data);
	        gnttab_grant_foreign_access_ref(
		        ref, np->xbdev->otherend_id, mfn, GNTMAP_readonly);

	        tx = RING_GET_REQUEST(&np->tx, i);
	        tx->id   = id;
	        tx->gref = np->grant_tx_ref[id] = ref;
	        tx->offset = offset;
	        tx->size = slice;
                tx->flags = 1;  /* set the MORE_DATA flag */
	        np->grant_tx_page[id] = virt_to_page(data);

                IFV(printk("%s: buf %p id %d, gref %d, off %d, sz %d\n", __func__,
                        buf, tx->id, tx->gref, tx->offset, tx->size));

                data += slice;
                len -= slice;
                offset = 0;
                i++;
        }
	tx->flags = 0;  /* clear the MORE_DATA flag */

	np->tx.req_prod_pvt = i;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->tx, notify);
	if (notify)
		notify_remote_via_irq(np->tx_irq);
        IFV(printk("%s: pushed [%d], notify [%d]\n", __func__, i, notify));

	spin_unlock_irqrestore(&np->tx_lock, flags);

        return 0;

 drop:
        return -1;
}

static void xenmpi_refill_one(struct vmpi_impl_info *np)
{
	unsigned short id;
        struct vmpi_buffer *buf;
	int notify;
	RING_IDX req_prod = np->rx.req_prod_pvt;
	grant_ref_t ref;
	unsigned long mfn;
	struct xen_mpi_rx_request *req;

        /* TODO allocate data as a page. */
        buf = vmpi_buffer_create(VMPI_BUFFER_SIZE_XEN);
        if (unlikely(!buf)) {
                printk("%s: failed to refill\n", __func__);
                return;
        }

        id = xenmpi_rxidx(req_prod);
        BUG_ON(np->rx_skbs[id]);
        np->rx_skbs[id] = buf;

        ref = gnttab_claim_grant_reference(&np->gref_rx_head);
        BUG_ON((signed short)ref < 0);
        mfn = virt_to_mfn(buf->p);
        gnttab_grant_foreign_access_ref(ref,
                        np->xbdev->otherend_id,
                        mfn,
                        0);

        req = RING_GET_REQUEST(&np->rx, req_prod);
        req->id = id;
        req->gref = np->grant_rx_ref[id] = ref;
        req->offset = offset_in_page(buf->p);
        req->len = VMPI_BUFFER_SIZE_XEN;

        req_prod++;

	wmb();		/* barrier so backend seens requests */

	/* Above is a suitable barrier to ensure backend will see requests. */
	np->rx.req_prod_pvt = req_prod;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->rx, notify);
	if (notify)
		notify_remote_via_irq(np->rx_irq);

        IFV(printk("%s refilled [prod=%u] [id=%d] [gref=%d] [off=%d] [len=%d]\n",
                __func__, req_prod, req->id, req->gref, req->offset, req->len));
}

struct vmpi_buffer *vmpi_impl_read_buffer(vmpi_impl_info_t *np)
{
        RING_IDX cons, prod;
        struct vmpi_buffer *buf = NULL;

	spin_lock(&np->rx_lock);

	prod = np->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'prod'. */
	cons = np->rx.rsp_cons;

        if (cons != prod) {
	        struct xen_mpi_rx_response *rx = NULL;
                unsigned long ret;
                grant_ref_t ref;

                rx = RING_GET_RESPONSE(&np->rx, cons);
                buf = xenmpi_get_rx_skb(np, cons);
                ref = xenmpi_get_rx_ref(np, cons);

                IFV(printk("%s: rx rsp [cons=%d] [id=%d] [size=%d]\n", __func__,
                                cons, rx->id, rx->status));

                buf->len = rx->status;

                /*
                 * This definitely indicates a bug, either in this driver or in
                 * the backend driver. In future this should flag the bad
                 * situation to the system controller to reboot the backend.
                 */
                if (ref == GRANT_INVALID_REF) {
                        buf = NULL;
                        if (net_ratelimit())
                                pr_warn("Bad rx response id %d.\n",
                                                rx->id);
                        goto out;
                }

                ret = gnttab_end_foreign_access_ref(ref, 0);
                BUG_ON(!ret);
                gnttab_release_grant_reference(&np->gref_rx_head, ref);

                IFV(printk("%s: buffer received [cons=%d, len=%d]\n",
                        __func__, cons, (int)buf->len));
out:
                np->rx.rsp_cons = ++cons;

                xenmpi_refill_one(np);
        }
	spin_unlock(&np->rx_lock);

        return buf;
}

static void recv_worker_function(struct work_struct *work)
{
        struct vmpi_impl_info *np =
            container_of(work, struct vmpi_impl_info, recv_worker);
	struct xen_mpi_rx_response *rx = NULL;
	RING_IDX i, rp;
	int work_done;
	unsigned long flags;
        int budget = 64;
        grant_ref_t ref;

        printk("%s: I should NOT be called\n", __func__);

	spin_lock(&np->rx_lock);

	rp = np->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'rp'. */

	i = np->rx.rsp_cons;
	work_done = 0;
        while ((i != rp) && (work_done < budget)) {
            unsigned long ret;
            struct vmpi_buffer *buf;

            rx = RING_GET_RESPONSE(&np->rx, i);
            buf = xenmpi_get_rx_skb(np, i);
            ref = xenmpi_get_rx_ref(np, i);
            /*
             * This definitely indicates a bug, either in this driver or in
             * the backend driver. In future this should flag the bad
             * situation to the system controller to reboot the backend.
             */
            if (ref == GRANT_INVALID_REF) {
                if (net_ratelimit())
                    pr_warn("Bad rx response id %d.\n",
                            rx->id);
                goto next;
            }

            ret = gnttab_end_foreign_access_ref(ref, 0);
            BUG_ON(!ret);
            gnttab_release_grant_reference(&np->gref_rx_head, ref);

            IFV(printk("%s: buffer received\n", __func__));
            /* TODO pass the buffer up */
            vmpi_buffer_destroy(buf);

            work_done++;
next:
            i++;
        }
        np->rx.rsp_cons = i;

	/* If we get a callback with very few responses, reduce fill target. */
	/* NB. Note exponential increase, linear decrease. */
	if (((np->rx.req_prod_pvt - np->rx.sring->rsp_prod) >
	     ((3*np->rx_target) / 4)) &&
	    (--np->rx_target < np->rx_min_target))
		np->rx_target = np->rx_min_target;

	xenmpi_alloc_rx_buffers(np);

        if (work_done == budget) {
                schedule_work(&np->recv_worker); /* HZ/20 */
        } else {
		int more_to_do = 0;

		local_irq_save(flags);

		RING_FINAL_CHECK_FOR_RESPONSES(&np->rx, more_to_do);
		if (more_to_do) {
                        schedule_work(&np->recv_worker);
                }

		local_irq_restore(flags);
	}

	spin_unlock(&np->rx_lock);
}

static void xenmpi_release_tx_bufs(struct vmpi_impl_info *np)
{
	struct vmpi_buffer *buf;
	int i;

	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		/* Skip over entries which are actually freelist references */
		if (skb_entry_is_link(&np->tx_skbs[i]))
			continue;

		get_page(np->grant_tx_page[i]);
		gnttab_end_foreign_access(np->grant_tx_ref[i],
					  GNTMAP_readonly,
					  (unsigned long)page_address(np->grant_tx_page[i]));
		np->grant_tx_page[i] = NULL;
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
		add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, i);
		buf = np->tx_skbs[i].buf;
	}
}

static void xenmpi_release_rx_bufs(struct vmpi_impl_info *np)
{
	int id, ref;

	spin_lock_bh(&np->rx_lock);

	for (id = 0; id < NET_RX_RING_SIZE; id++) {
		struct page *page;
                struct vmpi_buffer *buf;

		buf = np->rx_skbs[id];
		if (!buf)
			continue;

		ref = np->grant_rx_ref[id];
		if (ref == GRANT_INVALID_REF)
			continue;

		page = virt_to_page(buf->p);

		/* gnttab_end_foreign_access() needs a page ref until
		 * foreign access is ended (which may be deferred).
		 */
		get_page(page);
		gnttab_end_foreign_access(ref, 0,
					  (unsigned long)page_address(page));
		np->grant_rx_ref[id] = GRANT_INVALID_REF;

                vmpi_buffer_destroy(buf);
	}

	spin_unlock_bh(&np->rx_lock);
}

static void xenmpi_uninit(struct vmpi_impl_info *np)
{
	xenmpi_release_tx_bufs(np);
	xenmpi_release_rx_bufs(np);
	gnttab_free_grant_references(np->gref_tx_head);
	gnttab_free_grant_references(np->gref_rx_head);
}

vmpi_info_t *
vmpi_info_from_vmpi_impl_info(vmpi_impl_info_t *np)
{
        return np->private;
}

void vmpi_impl_callbacks_register(vmpi_impl_info_t *np,
                                  vmpi_impl_callback_t xmit,
                                  vmpi_impl_callback_t recv)
{
        np->xmit_cb = xmit;
        np->recv_cb = recv;
}

void vmpi_impl_callbacks_unregister(vmpi_impl_info_t *np)
{
        np->xmit_cb = NULL;
        np->recv_cb = NULL;
}

void vmpi_impl_txkick(vmpi_impl_info_t *np)
{
}

bool vmpi_impl_send_cb(vmpi_impl_info_t *np, int enable)
{
        int more_to_do = 0;
        unsigned long flags;

        if (!enable)
                return true;

        local_irq_save(flags);
        RING_FINAL_CHECK_FOR_RESPONSES(&np->tx, more_to_do);
        local_irq_restore(flags);

        return !more_to_do;
}
bool vmpi_impl_receive_cb(vmpi_impl_info_t *np, int enable)
{
        int more_to_do = 0;
        unsigned long flags;

        if (!enable)
                return true;

        local_irq_save(flags);
        RING_FINAL_CHECK_FOR_RESPONSES(&np->rx, more_to_do);
        local_irq_restore(flags);

        return !more_to_do;
}

static irqreturn_t xenmpi_tx_interrupt(int irq, void *dev_id)
{
	struct vmpi_impl_info *np = dev_id;

        IFV(printk("%s\n", __func__));
        if (likely(np->xmit_cb))
                np->xmit_cb(np);

	return IRQ_HANDLED;
}

static irqreturn_t xenmpi_rx_interrupt(int irq, void *dev_id)
{
	struct vmpi_impl_info *np = dev_id;

        IFV(printk("%s\n", __func__));
        if (likely(np->recv_cb))
                np->recv_cb(np);

	return IRQ_HANDLED;
}

static irqreturn_t xenmpi_interrupt(int irq, void *dev_id)
{
	xenmpi_tx_interrupt(irq, dev_id);
	xenmpi_rx_interrupt(irq, dev_id);
	return IRQ_HANDLED;
}

static struct vmpi_impl_info *xenmpi_create_dev(struct xenbus_device *dev)
{
	int i, err = 0;
	struct vmpi_impl_info *np;

        np = kmalloc(sizeof(struct vmpi_impl_info), GFP_KERNEL);
        if (!np) {
            return ERR_PTR(-ENOMEM);
        }

	np->xbdev            = dev;

	spin_lock_init(&np->tx_lock);
	spin_lock_init(&np->rx_lock);

        INIT_WORK(&np->recv_worker, recv_worker_function);
        vmpi_queue_init(&np->rx_batch, 0, VMPI_BUF_SIZE);
	np->rx_target     = RX_DFL_MIN_TARGET;
	np->rx_min_target = RX_DFL_MIN_TARGET;
	np->rx_max_target = RX_MAX_TARGET;

	init_timer(&np->rx_refill_timer);
	np->rx_refill_timer.data = (unsigned long)np;
	np->rx_refill_timer.function = rx_refill_timeout;

	/* Initialise tx_skbs as a free chain containing every entry. */
	np->tx_skb_freelist = 0;
	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		skb_entry_set_link(&np->tx_skbs[i], i+1);
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
	}

	/* Clear out rx_skbs */
	for (i = 0; i < NET_RX_RING_SIZE; i++) {
		np->rx_skbs[i] = NULL;
		np->grant_rx_ref[i] = GRANT_INVALID_REF;
		np->grant_tx_page[i] = NULL;
	}

	/* A grant for every tx ring slot */
	if (gnttab_alloc_grant_references(TX_MAX_TARGET,
					  &np->gref_tx_head) < 0) {
		pr_alert("can't alloc tx grant refs\n");
		err = -ENOMEM;
		goto exit;
	}
	/* A grant for every rx ring slot */
	if (gnttab_alloc_grant_references(RX_MAX_TARGET,
					  &np->gref_rx_head) < 0) {
		pr_alert("can't alloc rx grant refs\n");
		err = -ENOMEM;
		goto exit_free_tx;
	}

	return np;

 exit_free_tx:
	gnttab_free_grant_references(np->gref_tx_head);
 exit:
        kfree(np);
	return ERR_PTR(err);
}

/**
 * Entry point to this code when a new device is created.  Allocate the basic
 * structures and the ring buffers for communication with the backend, and
 * inform the backend of the appropriate details for those.
 */
static int mpifront_probe(struct xenbus_device *dev,
			  const struct xenbus_device_id *id)
{
	int err;
	struct vmpi_impl_info *np;

	np = xenmpi_create_dev(dev);
	if (IS_ERR(np)) {
		err = PTR_ERR(np);
		xenbus_dev_fatal(dev, err, "creating mpi dev");
		return err;
	}

	dev_set_drvdata(&dev->dev, np);

        instance = NULL;

        np->private = vmpi_init(np, &err, false);
        if (np->private == NULL) {
		xenbus_dev_fatal(dev, err, "vmpi_init() failed");
                goto vmpi_ini;
        }

        printk("%s: Xen-mpi probe completed\n", __func__);

	return 0;

vmpi_ini:
        /* TODO undo xenmpi_create_dev(dev) */

        return err;
}

static void xenmpi_end_access(int ref, void *page)
{
	/* This frees the page as a side-effect */
	if (ref != GRANT_INVALID_REF)
		gnttab_end_foreign_access(ref, 0, (unsigned long)page);
}

static void xenmpi_disconnect_backend(struct vmpi_impl_info *info)
{
	/* Stop old i/f to prevent errors whilst we rebuild the state. */
	spin_lock_bh(&info->rx_lock);
	spin_lock_irq(&info->tx_lock);
	// XXX mpi_carrier_off(info->netdev);
        /* Uninstall the instance */
        instance = NULL;
	spin_unlock_irq(&info->tx_lock);
	spin_unlock_bh(&info->rx_lock);

	if (info->tx_irq && (info->tx_irq == info->rx_irq))
		unbind_from_irqhandler(info->tx_irq, info);
	if (info->tx_irq && (info->tx_irq != info->rx_irq)) {
		unbind_from_irqhandler(info->tx_irq, info);
		unbind_from_irqhandler(info->rx_irq, info);
	}
	info->tx_evtchn = info->rx_evtchn = 0;
	info->tx_irq = info->rx_irq = 0;

	/* End access and free the pages */
	xenmpi_end_access(info->tx_ring_ref, info->tx.sring);
	xenmpi_end_access(info->rx_ring_ref, info->rx.sring);

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->tx.sring = NULL;
	info->rx.sring = NULL;

        printk("%s: xen-mpi disconnection complete\n", __func__);
}

/**
 * We are reconnecting to the backend, due to a suspend/resume, or a backend
 * driver restart.  We tear down our mpi structure and recreate it, but
 * leave the device-layer structures intact so that this is transparent to the
 * rest of the kernel.
 */
static int mpifront_resume(struct xenbus_device *dev)
{
	struct vmpi_impl_info *info = dev_get_drvdata(&dev->dev);

	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xenmpi_disconnect_backend(info);
	return 0;
}

static int setup_mpifront_single(struct vmpi_impl_info *info)
{
	int err;

	err = xenbus_alloc_evtchn(info->xbdev, &info->tx_evtchn);
	if (err < 0)
		goto fail;

	err = bind_evtchn_to_irqhandler(info->tx_evtchn,
					xenmpi_interrupt,
					0, "xen-mpi", info);
	if (err < 0)
		goto bind_fail;
	info->rx_evtchn = info->tx_evtchn;
	info->rx_irq = info->tx_irq = err;

	return 0;

bind_fail:
	xenbus_free_evtchn(info->xbdev, info->tx_evtchn);
	info->tx_evtchn = 0;
fail:
	return err;
}

static int setup_mpifront_split(struct vmpi_impl_info *info)
{
	int err;

	err = xenbus_alloc_evtchn(info->xbdev, &info->tx_evtchn);
	if (err < 0)
		goto fail;
	err = xenbus_alloc_evtchn(info->xbdev, &info->rx_evtchn);
	if (err < 0)
		goto alloc_rx_evtchn_fail;

	snprintf(info->tx_irq_name, sizeof(info->tx_irq_name),
		 "%s-tx", "xen-mpi");
	err = bind_evtchn_to_irqhandler(info->tx_evtchn,
					xenmpi_tx_interrupt,
					0, info->tx_irq_name, info);
	if (err < 0)
		goto bind_tx_fail;
	info->tx_irq = err;

	snprintf(info->rx_irq_name, sizeof(info->rx_irq_name),
		 "%s-rx", "xen-mpi");
	err = bind_evtchn_to_irqhandler(info->rx_evtchn,
					xenmpi_rx_interrupt,
					0, info->rx_irq_name, info);
	if (err < 0)
		goto bind_rx_fail;
	info->rx_irq = err;

	return 0;

bind_rx_fail:
	unbind_from_irqhandler(info->tx_irq, info);
	info->tx_irq = 0;
bind_tx_fail:
	xenbus_free_evtchn(info->xbdev, info->rx_evtchn);
	info->rx_evtchn = 0;
alloc_rx_evtchn_fail:
	xenbus_free_evtchn(info->xbdev, info->tx_evtchn);
	info->tx_evtchn = 0;
fail:
	return err;
}

static int setup_mpifront(struct xenbus_device *dev, struct vmpi_impl_info *info)
{
	struct xen_mpi_tx_sring *txs;
	struct xen_mpi_rx_sring *rxs;
	int err;
	unsigned int feature_split_evtchn;

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->rx.sring = NULL;
	info->tx.sring = NULL;

	err = xenbus_scanf(XBT_NIL, info->xbdev->otherend,
			   "feature-split-event-channels", "%u",
			   &feature_split_evtchn);
	if (err < 0)
		feature_split_evtchn = 0;

	txs = (struct xen_mpi_tx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!txs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating tx ring page");
		goto fail;
	}
	SHARED_RING_INIT(txs);
	FRONT_RING_INIT(&info->tx, txs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(txs));
	if (err < 0)
		goto grant_tx_ring_fail;

	info->tx_ring_ref = err;
	rxs = (struct xen_mpi_rx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!rxs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating rx ring page");
		goto alloc_rx_ring_fail;
	}
	SHARED_RING_INIT(rxs);
	FRONT_RING_INIT(&info->rx, rxs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(rxs));
	if (err < 0)
		goto grant_rx_ring_fail;
	info->rx_ring_ref = err;

	if (feature_split_evtchn)
		err = setup_mpifront_split(info);
	/* setup single event channel if
	 *  a) feature-split-event-channels == 0
	 *  b) feature-split-event-channels == 1 but failed to setup
	 */
	if (!feature_split_evtchn || (feature_split_evtchn && err))
		err = setup_mpifront_single(info);

	if (err)
		goto alloc_evtchn_fail;

	return 0;

	/* If we fail to setup mpifront, it is safe to just revoke access to
	 * granted pages because backend is not accessing it at this point.
	 */
alloc_evtchn_fail:
	gnttab_end_foreign_access_ref(info->rx_ring_ref, 0);
grant_rx_ring_fail:
	free_page((unsigned long)rxs);
alloc_rx_ring_fail:
	gnttab_end_foreign_access_ref(info->tx_ring_ref, 0);
grant_tx_ring_fail:
	free_page((unsigned long)txs);
fail:
	return err;
}

/* Common code used when first setting up, and when resuming. */
static int talk_to_mpiback(struct xenbus_device *dev,
			   struct vmpi_impl_info *info)
{
	const char *message;
	struct xenbus_transaction xbt;
	int err;

	/* Create shared ring, alloc event channel. */
	err = setup_mpifront(dev, info);
	if (err)
		goto out;

again:
	err = xenbus_transaction_start(&xbt);
	if (err) {
		xenbus_dev_fatal(dev, err, "starting transaction");
		goto destroy_ring;
	}

	err = xenbus_printf(xbt, dev->nodename, "tx-ring-ref", "%u",
			    info->tx_ring_ref);
	if (err) {
		message = "writing tx ring-ref";
		goto abort_transaction;
	}
	err = xenbus_printf(xbt, dev->nodename, "rx-ring-ref", "%u",
			    info->rx_ring_ref);
	if (err) {
		message = "writing rx ring-ref";
		goto abort_transaction;
	}

	if (info->tx_evtchn == info->rx_evtchn) {
		err = xenbus_printf(xbt, dev->nodename,
				    "event-channel", "%u", info->tx_evtchn);
		if (err) {
			message = "writing event-channel";
			goto abort_transaction;
		}
	} else {
		err = xenbus_printf(xbt, dev->nodename,
				    "event-channel-tx", "%u", info->tx_evtchn);
		if (err) {
			message = "writing event-channel-tx";
			goto abort_transaction;
		}
		err = xenbus_printf(xbt, dev->nodename,
				    "event-channel-rx", "%u", info->rx_evtchn);
		if (err) {
			message = "writing event-channel-rx";
			goto abort_transaction;
		}
	}

	err = xenbus_printf(xbt, dev->nodename, "request-rx-copy", "%u",
			    1);
	if (err) {
		message = "writing request-rx-copy";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-rx-notify", "%d", 1);
	if (err) {
		message = "writing feature-rx-notify";
		goto abort_transaction;
	}

	err = xenbus_transaction_end(xbt, 0);
	if (err) {
		if (err == -EAGAIN)
			goto again;
		xenbus_dev_fatal(dev, err, "completing transaction");
		goto destroy_ring;
	}

	return 0;

 abort_transaction:
	xenbus_transaction_end(xbt, 1);
	xenbus_dev_fatal(dev, err, "%s", message);
 destroy_ring:
	xenmpi_disconnect_backend(info);
 out:
	return err;
}

static int xenmpi_connect(struct vmpi_impl_info *np)
{
	int i, requeue_idx, err;
	struct sk_buff *skb;
	grant_ref_t ref;
	struct xen_mpi_rx_request *req;
	unsigned int feature_rx_copy;

	err = xenbus_scanf(XBT_NIL, np->xbdev->otherend,
			   "feature-rx-copy", "%u", &feature_rx_copy);
	if (err != 1)
		feature_rx_copy = 0;

	if (!feature_rx_copy) {
		dev_info(&np->xbdev->dev,
			 "backend does not support copying receive path\n");
		return -ENODEV;
	}

	err = talk_to_mpiback(np->xbdev, np);
	if (err)
		return err;

	spin_lock_bh(&np->rx_lock);
	spin_lock_irq(&np->tx_lock);

	/* Step 1: Discard all pending TX packet fragments. */
	xenmpi_release_tx_bufs(np);

	/* Step 2: Rebuild the RX buffer freelist and the RX ring itself. */
	for (requeue_idx = 0, i = 0; i < NET_RX_RING_SIZE; i++) {
		skb_frag_t *frag;
		const struct page *page;
                struct vmpi_buffer *buf;

		if (!np->rx_skbs[i])
			continue;

		buf = np->rx_skbs[requeue_idx] = xenmpi_get_rx_skb(np, i);
		ref = np->grant_rx_ref[requeue_idx] = xenmpi_get_rx_ref(np, i);
		req = RING_GET_REQUEST(&np->rx, requeue_idx);

		frag = &skb_shinfo(skb)->frags[0];
		page = virt_to_page(buf->p);
		gnttab_grant_foreign_access_ref(
			ref, np->xbdev->otherend_id,
			pfn_to_mfn(page_to_pfn(page)),
			0);
		req->gref = ref;
		req->id   = requeue_idx;
                req->offset = offset_in_page(buf->p);
                req->len = VMPI_BUFFER_SIZE_XEN;

		requeue_idx++;
	}

	np->rx.req_prod_pvt = requeue_idx;

	/*
	 * Step 3: All public and private state should now be sane.  Get
	 * ready to start sending and receiving packets and give the driver
	 * domain a kick because we've probably just requeued some
	 * packets.
	 */
        /* Install the instance. */
	// XXX mpi_carrier_on(np->netdev);
        instance = np;
	notify_remote_via_irq(np->tx_irq);
	if (np->tx_irq != np->rx_irq)
		notify_remote_via_irq(np->rx_irq);
	xenmpi_tx_buf_gc(np);
        for (i = 0; i < 64; i++) {
                xenmpi_refill_one(np);
        }
	// XXX xenmpi_alloc_rx_buffers(np);

        np->rx.sring->rsp_event = np->rx.rsp_cons + 1;
        if (RING_HAS_UNCONSUMED_RESPONSES(&np->rx)) {
            //schedule_work(&np->recv_worker); /* HZ/20 */
        }

	spin_unlock_irq(&np->tx_lock);
	spin_unlock_bh(&np->rx_lock);

        printk("%s: xen-mpi connection completed\n", __func__);

	return 0;
}

/**
 * Callback received when the backend's state changes.
 */
static void mpiback_changed(struct xenbus_device *dev,
			    enum xenbus_state backend_state)
{
	struct vmpi_impl_info *np = dev_get_drvdata(&dev->dev);

	dev_dbg(&dev->dev, "%s\n", xenbus_strstate(backend_state));
        printk("%s: xen-mpi backend state --> %s\n", __func__,
                   xenbus_strstate(backend_state));

	switch (backend_state) {
	case XenbusStateInitialising:
	case XenbusStateInitialised:
	case XenbusStateReconfiguring:
	case XenbusStateReconfigured:
	case XenbusStateUnknown:
		break;

	case XenbusStateInitWait:
		if (dev->state != XenbusStateInitialising)
			break;
		if (xenmpi_connect(np) != 0)
			break;
		xenbus_switch_state(dev, XenbusStateConnected);
		break;

	case XenbusStateConnected:
		// XXX netdev_notify_peers(netdev);
		break;

	case XenbusStateClosed:
		if (dev->state == XenbusStateClosed)
			break;
		/* Missed the backend's CLOSING state -- fallthrough */
	case XenbusStateClosing:
		xenbus_frontend_closed(dev);
		break;
	}
}

static const struct xenbus_device_id mpifront_ids[] = {
	{ "mpi" },
	{ "" }
};


static int xenmpi_remove(struct xenbus_device *dev)
{
	struct vmpi_impl_info *info = dev_get_drvdata(&dev->dev);

	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xenmpi_disconnect_backend(info);

        xenmpi_uninit(info);

	del_timer_sync(&info->rx_refill_timer);

        vmpi_fini(false);

        vmpi_queue_fini(&info->rx_batch);

        kfree(info);

        printk("%s: Xen-mpi removed\n", __func__);

	return 0;
}

static DEFINE_XENBUS_DRIVER(mpifront, ,
	.probe = mpifront_probe,
	.remove = xenmpi_remove,
	.resume = mpifront_resume,
	.otherend_changed = mpiback_changed,
);

static int __init mpi_init(void)
{
        int err;

	if (!xen_domain())
		return -ENODEV;

	err = xenbus_register_frontend(&mpifront_driver);
        if (err) {
            goto xen_register;
        }

        printk("Xen Virtual MPI driver loaded\n");

        return 0;

xen_register:

        return err;
}
module_init(mpi_init);


static void __exit mpi_exit(void)
{
	xenbus_unregister_driver(&mpifront_driver);
        printk("Xen Virtual MPI driver unloaded\n");
}
module_exit(mpi_exit);

MODULE_DESCRIPTION("Xen virtual MPI frontend");
MODULE_LICENSE("GPL");
MODULE_ALIAS("xen:mpi");
MODULE_ALIAS("xenmpi");
