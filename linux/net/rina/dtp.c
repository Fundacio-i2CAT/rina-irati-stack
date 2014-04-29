/*
 * DTP (Data Transfer Protocol)
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *    Miquel Tarzan         <miquel.tarzan@i2cat.net>
 *    Leonardo Bergesio     <leonardo.bergesio@i2cat.net>
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

#define RINA_PREFIX "dtp"

#include "logs.h"
#include "utils.h"
#include "debug.h"
#include "dtp.h"
#include "dt.h"
#include "dt-utils.h"
#include "dtcp.h"

/* This is the DT-SV part maintained by DTP */
struct dtp_sv {
        spinlock_t lock;
        /* Configuration values */
        struct connection * connection; /* FIXME: Are we really sure ??? */

        uint_t              seq_number_rollover_threshold;
        uint_t              dropped_pdus;
        seq_num_t           max_seq_nr_rcv;
        seq_num_t           nxt_seq;
        uint_t              max_cwq_len;
        bool                drf_flag;
        timeout_t           a;

        bool                window_based;
        bool                rexmsn_ctrl;
};

/* FIXME: Has to be rearranged */
struct dtp_policies {
        int (* transmission_control)(struct dtp * instance,
                                     struct pdu * pdu);
        int (* closed_window)(struct dtp * instance,
                                      struct pdu * pdu);
        int (* flow_control_overrun)(struct dtp * instance);
        int (* initial_sequence_number)(struct dtp * instance);
        int (* receiver_inactivity_timer)(struct dtp * instance);
        int (* sender_inactivity_timer)(struct dtp * instance);
};

struct dtp {
        struct dt *           parent;
        /*
         * NOTE: The DTP State Vector is discarded only after and explicit
         *       release by the AP or by the system (if the AP crashes).
         */
        struct dtp_sv *       sv; /* The state-vector */

        struct dtp_policies * policies;
        struct rmt *          rmt;
        struct kfa *          kfa;

        struct {
                struct rtimer * sender_inactivity;
                struct rtimer * receiver_inactivity;
                struct rtimer * a;
        } timers;
};

static struct dtp_sv default_sv = {
        .connection                    = NULL,
        .seq_number_rollover_threshold = 0,
        .dropped_pdus                  = 0,
        .max_seq_nr_rcv                = 0,
        .max_cwq_len                   = 0,
        .drf_flag                      = false,
        .a                             = 0,
        .window_based                  = true,
        .rexmsn_ctrl                   = true,
};

static uint_t max_cwq_len_get(struct dtp_sv * sv)
{
        uint_t tmp;

        ASSERT(sv);

        spin_lock(&sv->lock);
        tmp = sv->max_cwq_len;
        spin_unlock(&sv->lock);

        return tmp;
}

static int default_closed_window(struct dtp * dtp, struct pdu * pdu)
{
        struct cwq * cwq;
        struct dt *  dt;

        ASSERT(dtp);
        ASSERT(pdu_is_ok(pdu));

        dt = dtp->parent;
        ASSERT(dt);

        cwq = dt_cwq(dt);
        if (!cwq) {
                LOG_ERR("Failed to get cwq");
                pdu_destroy(pdu);
                return -1;
        }

        if (cwq_size(cwq) < max_cwq_len_get(dtp->sv)-1) {
                if (cwq_push(cwq, pdu)) {
                        LOG_ERR("Failed to push to cwq");
                        pdu_destroy(pdu);
                        return -1;
                }
                return 0;
        }

        return -1;
}

static int default_transmission(struct dtp * dtp, struct pdu * pdu)
{

        ASSERT(dtp);
        ASSERT(pdu_is_ok(pdu));

        /* Post SDU to RMT */
        return rmt_send(dtp->rmt,
                        pci_destination(pdu_pci_get_ro(pdu)),
                        pci_qos_id(pdu_pci_get_ro(pdu)),
                        pdu);
}

static struct dtp_policies default_policies = {
        .transmission_control      = default_transmission,
        .closed_window             = default_closed_window,
        .flow_control_overrun      = NULL,
        .initial_sequence_number   = NULL,
        .receiver_inactivity_timer = NULL,
        .sender_inactivity_timer   = NULL,
};

bool dtp_drf_flag(struct dtp * instance)
{
        bool flag;

        if (!instance || !instance->sv)
                return false;

        spin_lock(&instance->sv->lock);
        flag = instance->sv->drf_flag;
        spin_unlock(&instance->sv->lock);

        return flag;
}

#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
static void drf_flag_set(struct dtp_sv * sv, bool value)
{
        ASSERT(sv);

        spin_lock(&sv->lock);
        sv->drf_flag = value;
        spin_unlock(&sv->lock);
}
#endif

static seq_num_t nxt_seq_get(struct dtp_sv * sv)
{
        seq_num_t tmp;

        ASSERT(sv);

        spin_lock(&sv->lock);
        tmp = sv->nxt_seq++;
        spin_unlock(&sv->lock);

        return tmp;
}

#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
#if 0
static uint_t dropped_pdus(struct dtp_sv * sv)
{
        uint_t tmp;

        ASSERT(sv);

        spin_lock(&sv->lock);
        tmp = sv->dropped_pdus;
        spin_unlock(&sv->lock);

        return tmp;
}
#endif

static void dropped_pdus_inc(struct dtp_sv * sv)
{
        ASSERT(sv);

        spin_lock(&sv->lock);
        sv->dropped_pdus++;
        spin_unlock(&sv->lock);
}
#endif

#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
static seq_num_t max_seq_nr_rcv(struct dtp_sv * sv)
{
        seq_num_t tmp;

        ASSERT(sv);

        spin_lock(&sv->lock);
        tmp = sv->max_seq_nr_rcv;
        spin_unlock(&sv->lock);

        return tmp;
}

static void max_seq_nr_rcv_set(struct dtp_sv * sv, seq_num_t nr)
{
        ASSERT(sv);

        spin_lock(&sv->lock);
        sv->max_seq_nr_rcv = nr;
        spin_unlock(&sv->lock);
}
#endif

static void tf_sender_inactivity(void * data)
{ /* Runs the SenderInactivityTimerPolicy */ }

static void tf_receiver_inactivity(void * data)
{ /* Runs the ReceiverInactivityTimerPolicy */ }

static void tf_a(void * data)
{ }

struct dtp * dtp_create(struct dt *         dt,
                        struct rmt *        rmt,
                        struct kfa *        kfa,
                        struct connection * connection)
{
        struct dtp * tmp;

        if (!dt) {
                LOG_ERR("No DT passed, bailing out");
                return NULL;
        }

        if (!rmt) {
                LOG_ERR("No RMT passed, bailing out");
                return NULL;
        }

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp) {
                LOG_ERR("Cannot create DTP instance");
                return NULL;
        }

        tmp->parent = dt;

        tmp->sv = rkmalloc(sizeof(*tmp->sv), GFP_KERNEL);
        if (!tmp->sv) {
                LOG_ERR("Cannot create DTP state-vector");

                dtp_destroy(tmp);
                return NULL;
        }
        *tmp->sv            = default_sv;
        /* FIXME: fixups to the state-vector should be placed here */

        spin_lock_init(&tmp->sv->lock);

        tmp->sv->connection = connection;

        tmp->policies       = &default_policies;
        /* FIXME: fixups to the policies should be placed here */

        tmp->rmt            = rmt;
        tmp->kfa            = kfa;

        tmp->timers.sender_inactivity   = rtimer_create(tf_sender_inactivity,
                                                        tmp);
        tmp->timers.receiver_inactivity = rtimer_create(tf_receiver_inactivity,
                                                        tmp);
        tmp->timers.a                   = rtimer_create(tf_a, tmp);
        if (!tmp->timers.sender_inactivity   ||
            !tmp->timers.receiver_inactivity ||
            !tmp->timers.a) {
                dtp_destroy(tmp);
                return NULL;
        }

        LOG_DBG("Instance %pK created successfully", tmp);

        return tmp;
}

int dtp_destroy(struct dtp * instance)
{
        if (!instance) {
                LOG_ERR("Bad instance passed, bailing out");
                return -1;
        }

        if (instance->timers.sender_inactivity)
                rtimer_destroy(instance->timers.sender_inactivity);
        if (instance->timers.receiver_inactivity)
                rtimer_destroy(instance->timers.receiver_inactivity);
        if (instance->timers.a)
                rtimer_destroy(instance->timers.a);

        if (instance->sv)
                rkfree(instance->sv);
        rkfree(instance);

        LOG_DBG("Instance %pK destroyed successfully", instance);

        return 0;
}

int dtp_write(struct dtp * instance,
              struct sdu * sdu)
{
        struct pdu *          pdu;
        struct pci *          pci;
        struct dtp_sv *       sv;
        struct dt *           dt;
        struct dtcp *         dtcp;
        struct rtxq *         rtxq;
        int                   ret = 0;
        struct pdu *          cpdu;
        struct dtp_policies * policies;

        if (!sdu_is_ok(sdu))
                return -1;

        if (!instance) {
                LOG_ERR("Bogus instance passed, bailing out");
                sdu_destroy(sdu);
                return -1;
        }
#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
        /* Stop SenderInactivityTimer */
        if (rtimer_stop(instance->timers.sender_inactivity)) {
                LOG_ERR("Failed to stop timer");
                sdu_destroy(sdu);
                return -1;
        }
#endif
        sv = instance->sv;
        ASSERT(sv); /* State Vector must not be NULL */

        dt = instance->parent;
        ASSERT(dt);

        dtcp = dt_dtcp(dt);

        policies = instance->policies;
        ASSERT(policies);

        pci = pci_create();
        if (!pci) {
                sdu_destroy(sdu);
                return -1;
        }

        /* Step 1: Sequencing */
        /*
         * Incrementing here means the PDU cannot
         * be just thrown away from this point onwards
         */
        /* Probably needs to be revised */

        if (pci_format(pci,
                       sv->connection->source_cep_id,
                       sv->connection->destination_cep_id,
                       sv->connection->source_address,
                       sv->connection->destination_address,
                       nxt_seq_get(sv),
                       sv->connection->qos_id,
                       PDU_TYPE_DT)) {
                pci_destroy(pci);
                sdu_destroy(sdu);
                return -1;
        }

        /* FIXME: Check if we need to set DRF */

        pdu = pdu_create();
        if (!pdu) {
                pci_destroy(pci);
                sdu_destroy(sdu);
                return -1;
        }

        if (pdu_buffer_set(pdu, sdu_buffer_rw(sdu))) {
                pci_destroy(pci);
                sdu_destroy(sdu);
                return -1;
        }

        if (pdu_pci_set(pdu, pci)) {
                sdu_buffer_disown(sdu);
                pdu_destroy(pdu);
                sdu_destroy(sdu);
                pci_destroy(pci);
                return -1;
        }

        sdu_buffer_disown(sdu);
        sdu_destroy(sdu);

        /* Step 2: Protection */
        /* Step 2: Delimiting (fragmentation/reassembly) */

        /*
         * FIXME: The two ways of carrying out flow control
         * could exist at once, thus reconciliation should be
         * the first and default case if both are present.
         */

        if (dtcp) {
                if (sv->rexmsn_ctrl) {
                        /* FIXME: Add timer for PDU */
                        rtxq = dt_rtxq(dt);
                        if (!rtxq) {
                                LOG_ERR("Failed to get rtxq");
                                pdu_destroy(pdu);
                                return -1;
                        }

                        cpdu = pdu_dup(pdu);
                        if (!cpdu) {
                                LOG_ERR("Failed to copy PDU");
                                LOG_ERR("PDU ok? %d", pdu_pci_present(pdu));
                                LOG_ERR("PDU type: %d",
                                        pci_type(pdu_pci_get_ro(pdu)));
                                pdu_destroy(pdu);
                                return -1;
                        }

                        if (rtxq_push(rtxq, cpdu)) {
                                LOG_ERR("Couldn't push to rtxq");
                                pdu_destroy(pdu);
                                return -1;
                        }
                }

                if (sv->window_based) {
                        LOG_DBG("WindowBased");
                        if (!dt_sv_window_closed(dt) &&
                            pci_sequence_number_get(pci) <
                            dtcp_snd_rt_win(dtcp)) {
                                /*
                                 * Might close window
                                 */
                                if (policies->transmission_control(instance,
                                                                   pdu)) {
                                        LOG_ERR("Problems with transmission "
                                                "control");
                                        return -1;
                                }
                        } else {
                                dt_sv_window_closed_set(dt, true);
                                if (policies->closed_window(instance, pdu)) {
                                        LOG_ERR("Problems with the "
                                                "closed window policy");
                                        return -1;
                                }
                        }
                }
                return 0;
        }

        /* Post SDU to RMT */
        ret = rmt_send(instance->rmt,
                       pci_destination(pci),
                       pci_qos_id(pci),
                       pdu);
#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
        if (rtimer_start(instance->timers.sender_inactivity,
                         2 * (dt_sv_mpl(dt) + dt_sv_r(dt) + dt_sv_a(dt)))) {
                LOG_ERR("Failed to start timer");
                return -1;
        }
#endif
        return ret;
}

int dtp_mgmt_write(struct rmt * rmt,
                   address_t    src_address,
                   port_id_t    port_id,
                   struct sdu * sdu)
{
        struct pci * pci;
        struct pdu * pdu;
        address_t    dst_address;

        /*
         * NOTE:
         *   DTP should build the PCI header src and dst cep_ids = 0
         *   ask FT for the dst address the N-1 port is connected to
         *   pass to the rmt
         */

        if (!sdu) {
                LOG_ERR("No data passed, bailing out");
                return -1;
        }

        dst_address = 0; /* FIXME: get from PFT */

        /*
         * FIXME:
         *   We should avoid to create a PCI only to have its fields to use
         */
        pci = pci_create();
        if (!pci)
                return -1;

        if (pci_format(pci,
                       0,
                       0,
                       src_address,
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
                pdu_destroy(pdu);
                return -1;
        }

        if (pdu_pci_set(pdu, pci)) {
                pci_destroy(pci);
                return -1;
        }

        /* Give the data to RMT now ! */

        /* FIXME: What about sequencing (and all the other procedures) ? */
        return rmt_send(rmt,
                        pci_destination(pci),
                        pci_cep_destination(pci),
                        pdu);

}

int dtp_receive(struct dtp * instance,
                struct pdu * pdu)
{
        struct sdu *          sdu;
        struct buffer *       buffer;
        struct dtp_policies * policies;
        struct pci *          pci;
        struct dtp_sv *       sv;
        struct dtcp *         dtcp;
        struct dt *           dt;
#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
        seq_num_t             seq_num;
#endif

        if (!pdu_is_ok(pdu)) {
                LOG_ERR("Bogus data, bailing out");
                return -1;
        }

        if (!instance                  ||
            !instance->kfa             ||
            !instance->sv              ||
            !instance->sv->connection) {
                LOG_ERR("Bogus instance passed, bailing out");
                pdu_destroy(pdu);
                return -1;
        }

        policies = instance->policies;
        ASSERT(policies);

        sv = instance->sv;
        ASSERT(sv); /* State Vector must not be NULL */

        dt = instance->parent;
        ASSERT(dt);

        dtcp = dt_dtcp(dt);

        if (!pdu_pci_present(pdu)) {
                LOG_DBG("Couldn't find PCI in PDU");
                pdu_destroy(pdu);
                return -1;
        }
        pci = pdu_pci_get_rw(pdu);

#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
        /* Stop ReceiverInactivityTimer */
        if (rtimer_stop(instance->timers.receiver_inactivity)) {
                LOG_ERR("Failed to stop timer");
                pdu_destroy(pdu);
                return -1;
        }

        seq_num = pci_sequence_number_get(pci);

        if (!(pci_flags_get(pci) ^ PDU_FLAGS_DATA_RUN)) {
                max_seq_nr_rcv_set(sv, seq_num);
                drf_flag_set(sv, true);
                policies->initial_sequence_number(instance);
                if (dtcp) {
                        if (dtcp_sv_update(dtcp, seq_num)) {
                                LOG_ERR("Failed to update dtcp sv");
                                pdu_destroy(pdu);
                                return -1;
                        }
                }
        } else if (seq_num < dt_sv_rcv_lft_win(dt)) {
                pdu_destroy(pdu);
                dropped_pdus_inc(sv);
                LOG_DBG("Dropped a PDU, total: %d", sv->dropped_pdus);
                /* Send an ACK/Flow Control PDU with current window values */
                if (dtcp_ack_flow_control_pdu_send(dtcp)) {
                        LOG_ERR("Failed to send ack / flow control pdu");
                        return -1;
                }
                return 0;
        } else if (dt_sv_rcv_lft_win(dt) < seq_num &&
                   seq_num < max_seq_nr_rcv(sv)) {
                /* Check if it is a duplicate in the gaps */
                /* if / else for this */
                LOG_MISSING;

                if (dtcp) {
                        if (dtcp_sv_update(dtcp, seq_num)) {
                                LOG_ERR("Failed to update dtcp sv");
                                pdu_destroy(pdu);
                                return -1;
                        }
                } else {
                        if (dt_sv_rcv_lft_win_set(dt, max_seq_nr_rcv(sv))) {
                                LOG_ERR("Failed to set new "
                                        "left window edge");
                                pdu_destroy(pdu);
                                return -1;
                        }
                }
        } else if (seq_num == (max_seq_nr_rcv(sv) + 1)) {
                max_seq_nr_rcv_set(sv, seq_num);
                if (dtcp) {
                        if (dtcp_sv_update(dtcp, seq_num)) {
                                LOG_ERR("Failed to update dtcp sv");
                                pdu_destroy(pdu);
                                return -1;
                        }
                } else {
                        if (dt_sv_rcv_lft_win_set(dt, max_seq_nr_rcv(sv))) {
                                LOG_ERR("Failed to set new "
                                        "left window edge");
                                pdu_destroy(pdu);
                                return -1;
                        }
                }
        } else if (seq_num > (max_seq_nr_rcv(sv) + 1)) {
                LOG_MISSING;

        } else {
                /* Something went wrong! */
                pdu_destroy(pdu);
                LOG_ERR("Something is horribly wrong on receiving");
                return -1;
        }
#endif
        buffer = pdu_buffer_get_rw(pdu);
        sdu    = sdu_create_buffer_with(buffer);
        if (!sdu) {
                pdu_destroy(pdu);
                return -1;
        }

        ASSERT(instance->sv);

        if (kfa_sdu_post(instance->kfa,
                         instance->sv->connection->port_id,
                         sdu)) {
                LOG_ERR("Could not post SDU to KFA");
                pdu_destroy(pdu);
                return -1;
        }

        pdu_buffer_disown(pdu);
        pdu_destroy(pdu);
#ifdef CONFIG_RINA_RELIABLE_FLOW_SUPPORT
        /* Start ReceiverInactivityTimer */
        if (rtimer_start(instance->timers.receiver_inactivity,
                         3 * (dt_sv_mpl(dt) + dt_sv_r(dt) + dt_sv_a(dt)))) {
                LOG_ERR("Failed to start timer");
                return -1;
        }
#endif
        return 0;
}
