/*
 * DTCP (Data Transfer Control Protocol)
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
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

#define RINA_PREFIX "dtcp"

#include "logs.h"
#include "utils.h"
#include "debug.h"
#include "dtcp.h"
#include "rmt.h"
#include "connection.h"

/* This is the DT-SV part maintained by DTCP */
struct dtcp_sv {
        /* SV lock */
        spinlock_t   lock;

        /* TimeOuts */

        /*
         * Time interval sender waits for a positive ack before
         * retransmitting
         */
        timeout_t    trd;

        /*
         * When flow control is rate based this timeout may be
         * used to pace number of PDUs sent in TimeUnit
         */
        uint_t       pdus_per_time_unit;

        /* Sequencing */

        /*
         * Outbound: NextSndCtlSeq contains the Sequence Number to
         * be assigned to a control PDU
         */
        seq_num_t    next_snd_ctl_seq;

        /*
         * Inbound: LastRcvCtlSeq - Sequence number of the next
         * expected // Transfer(? seems an error in the spec’s
         * doc should be Control) PDU received on this connection
         */
        seq_num_t    last_rcv_ctl_seq;

        /*
         * Retransmission: There’s no retransmission queue,
         * when a lost PDU is detected a new one is generated
         */

        /* Outbound */
        seq_num_t    last_snd_data_ack;

        /*
         * Seq number of the lowest seq number expected to be
         * Acked. Seq number of the first PDU on the
         * RetransmissionQ.
         */
        seq_num_t    send_left_wind_edge;

        /*
         * Maximum number of retransmissions of PDUs without a
         * positive ack before declaring an error
         */
        uint_t       data_retransmit_max;

        /* Inbound */
        seq_num_t    last_rcv_data_ack;

        /* Time (ms) over which the rate is computed */
        uint_t       time_unit;

        /* Flow Control State */

        /* Outbound */
        uint_t       sndr_credit;

        /* snd_rt_wind_edge = LastSendDataAck + PDU(credit) */
        seq_num_t    snd_rt_wind_edge;

        /* PDUs per TimeUnit */
        uint_t       sndr_rate;

        /* PDUs already sent in this time unit */
        uint_t       pdus_sent_in_time_unit;

        /* Inbound */

        /*
         * PDUs receiver believes sender may send before extending
         * credit or stopping the flow on the connection
         */
        uint_t       rcvr_credit;

        /* Value of credit in this flow */
        seq_num_t    rcvr_rt_wind_edge;

        /*
         * Current rate receiver has told sender it may send PDUs
         * at.
         */
        uint_t       rcvr_rate;

        /*
         * PDUs received in this time unit. When it equals
         * rcvr_rate, receiver is allowed to discard any PDUs
         * received until a new time unit begins
         */
        uint_t       pdus_rcvd_in_time_unit;

        /*
         * Control of duplicated control PDUs
         * */
        uint_t       dup_acks;
        uint_t       dup_flow_ctl;
};

struct dtcp_policies {
        int (* flow_init)(struct dtcp * instance);
        int (* sv_update)(struct dtcp * instance, seq_num_t seq);
        int (* lost_control_pdu)(struct dtcp * instance);
        int (* rtt_estimator)(struct dtcp * instance);
        int (* retransmission_timer_expiry)(struct dtcp * instance);
        int (* received_retransmission)(struct dtcp * instance);
        int (* rcvr_ack)(struct dtcp * instance, seq_num_t seq);
        int (* sending_ack)(struct dtcp * instance);
        int (* sending_ack_list)(struct dtcp * instance);
        int (* initial_credit)(struct dtcp * instance);
        int (* initial_rate)(struct dtcp * instance);
        int (* receiving_flow_control)(struct dtcp * instance);
        int (* update_credit)(struct dtcp * instance);
        int (* flow_control_overrun)(struct dtcp * instance);
        int (* reconcile_flow_conflict)(struct dtcp * instance);
        int (* rcvr_flow_control)(struct dtcp * instance, seq_num_t seq);
        int (* rate_reduction)(struct dtcp * instance);
};

struct dtcp {
        struct dt *            parent;

        /*
         * NOTE: The DTCP State Vector can be discarded during long periods of
         *       no traffic
         */
        struct dtcp_sv *       sv; /* The state-vector */
        struct dtcp_policies * policies;
        struct connection *    conn;
        struct rmt *           rmt;

        /* FIXME: Add QUEUE(flow_control_queue, pdu) */
        /* FIXME: Add QUEUE(closed_window_queue, pdu) */
        /* FIXME: Add QUEUE(rx_control_queue, ...) */
};

static int last_rcv_ctrl_seq_set(struct dtcp * dtcp,
                                 seq_num_t last_rcv_ctrl_seq)
{
        if (!dtcp || !dtcp->sv)
                return -1;

        spin_lock(&dtcp->sv->lock);
        dtcp->sv->last_rcv_ctl_seq = last_rcv_ctrl_seq;
        spin_lock(&dtcp->sv->lock);

        return 0;
}

static int dup_flow_ctrl_inc(struct dtcp * dtcp)
{
        if (!dtcp || !dtcp->sv)
                return -1;

        dtcp->sv->dup_flow_ctl++;

        return 0;
}

static struct pdu * pdu_ctrl_ack_create(struct dtcp * dtcp,
                                        seq_num_t     last_ctrl_seq_rcvd,
                                        seq_num_t     snd_left_wind_edge,
                                        seq_num_t     snd_rt_wind_edge,
                                        seq_num_t     left_wind_edge,
                                        seq_num_t     rt_wind_edge)
{
        LOG_MISSING;

        return NULL;
}

static struct pdu * pdu_ctrl_ack_flow(struct dtcp * dtcp,
                                      seq_num_t     last_ctrl_seq_rcvd,
                                      seq_num_t     ack_nack_seq,
                                      seq_num_t     new_rt_wind_edge,
                                      seq_num_t     left_wind_edge,
                                      seq_num_t     rt_wind_edge)
{
        struct pdu * pdu;
        struct pci * pci;
        seq_num_t    seq;

        LOG_MISSING;

        pdu = pdu_create();
        if (!pdu)
                return NULL;

        pci = pci_create();
        if (!pci) {
                pdu_destroy(pdu);
                return NULL;
        }
        spin_lock(&dtcp->sv->lock);
        seq = dtcp->sv->next_snd_ctl_seq;
        dtcp->sv->next_snd_ctl_seq++;
        spin_unlock(&dtcp->sv->lock);
        if (pci_format(pci,
                       dtcp->conn->source_cep_id,
                       dtcp->conn->destination_cep_id,
                       dtcp->conn->source_address,
                       dtcp->conn->destination_address,
                       seq,
                       0,
                       PDU_TYPE_ACK_AND_FC)) {
                pdu_destroy(pdu);
                pci_destroy(pci);
                return NULL;
        }

        if (pci_control_ack_seq_num_set(pci, ack_nack_seq)) {
                pdu_destroy(pdu);
                pci_destroy(pci);
                return NULL;
        }

        if (pci_control_last_seq_num_rcvd_set(pci, last_ctrl_seq_rcvd)) {
                pdu_destroy(pdu);
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

static int rcv_ack_ctl(struct dtcp * dtcp, struct pdu * pdu)
{
        LOG_MISSING;
        return 0;
}

/*
  static int rcv_nack_ctl(struct dtcp * dtcp, struct pdu * pdu)
  {
  LOG_MISSING;
  return 0;
  }
*/

static int rcv_flow_ctl(struct dtcp * dtcp,
                        struct pci *  pci,
                        seq_num_t     seq_num,
                        struct pdu *  pdu)
{
        dtcp->sv->snd_rt_wind_edge = pci_control_new_rt_wind_edge(pci);

        LOG_MISSING;

        return 0;
}

static int rcv_ack_and_flow_ctl(struct dtcp * dtcp,
                                struct pci *  pci,
                                seq_num_t     seq_num,
                                struct pdu *  pdu)
{
        LOG_MISSING;
        return 0;
}

int dtcp_common_rcv_control(struct dtcp * dtcp, struct pdu * pdu)
{
        struct pci * pci;
        pdu_type_t   type;
        seq_num_t    seq_num;

        if (!pdu_is_ok(pdu)) {
                LOG_ERR("PDU is not ok");
                pdu_destroy(pdu);
                return -1;
        }

        if (!dtcp) {
                LOG_ERR("DTCP instance bogus");
                pdu_destroy(pdu);
                return -1;
        }

        pci = pdu_pci_get_rw(pdu);
        if (!pci_is_ok(pci)) {
                LOG_ERR("PCI couldn't be retrieved");
                pdu_destroy(pdu);
                return -1;
        }

        type = pci_type(pci);

        if (!pdu_type_is_control(type)) {
                LOG_ERR("CommonRCVControl policy received a non-control PDU!");
                pdu_destroy(pdu);
                return -1;
        }

        seq_num = pci_sequence_number_get(pci);

        if (seq_num < dtcp->sv->last_rcv_ctl_seq) {
                switch (type) {
                case PDU_TYPE_FC:
                        dup_flow_ctrl_inc(dtcp);
                        break;
                case PDU_TYPE_ACK:
                        dtcp->sv->dup_acks++;
                        break;
                case PDU_TYPE_ACK_AND_FC:
                        dtcp->sv->dup_acks++;
                        dup_flow_ctrl_inc(dtcp);
                        break;
                default:
                        break;
                }

                pdu_destroy(pdu);
                return 0;

        } else if (seq_num > dtcp->sv->last_rcv_ctl_seq) {
                dtcp->policies->lost_control_pdu(dtcp);
        } else {
                if (last_rcv_ctrl_seq_set(dtcp, seq_num)) {
                        pdu_destroy(pdu);
                        return -1;
                }
        }

        /*
         * FIXME: Missing step described in the specs: retrieve the time
         * of this Ack and calculate the RTT with RTTEstimator policy
         */

        switch (type) {
        case PDU_TYPE_FC:
                return rcv_flow_ctl(dtcp, pci, seq_num, pdu);
        case PDU_TYPE_ACK:
                return rcv_ack_ctl(dtcp, pdu);
        case PDU_TYPE_ACK_AND_FC:
                return rcv_ack_and_flow_ctl(dtcp, pci, seq_num, pdu);
        default:
                return -1;
        }
}

static int default_lost_control_pdu(struct dtcp * dtcp)
{
        struct pdu * pdu_ctrl;

        pdu_ctrl = pdu_ctrl_ack_create(dtcp,
                                       dtcp->sv->last_rcv_ctl_seq,
                                       dtcp->sv->send_left_wind_edge,
                                       dtcp->sv->snd_rt_wind_edge,
                                       dt_sv_rcv_lft_win(dtcp->parent),
                                       dtcp->sv->snd_rt_wind_edge);
        if (!pdu_ctrl) {
                LOG_ERR("Failed Lost Control PDU");
                return -1;
        }

        return 0;
}

static int default_rcvr_ack(struct dtcp * dtcp, seq_num_t seq)
{
        struct pdu * pdu_ctrl;

        LOG_MISSING;

        pdu_ctrl = pdu_ctrl_ack_create(dtcp,
                                       dtcp->sv->last_rcv_ctl_seq,
                                       dtcp->sv->send_left_wind_edge,
                                       dtcp->sv->snd_rt_wind_edge,
                                       dt_sv_rcv_lft_win(dtcp->parent),
                                       dtcp->sv->rcvr_rt_wind_edge);
        if (!pdu_ctrl)
                return -1;

        return -1;
}

static int default_rcvr_flow_control(struct dtcp * dtcp, seq_num_t seq)
{
        struct pdu * pdu_ctrl;

        pdu_ctrl = pdu_ctrl_ack_flow(dtcp,
                                     dtcp->sv->last_rcv_ctl_seq,
                                     seq,
                                     seq + dtcp->sv->rcvr_credit,
                                     dtcp->sv->send_left_wind_edge,
                                     dtcp->sv->snd_rt_wind_edge);
        if (!pdu_ctrl)
                return -1;

        rmt_send(dtcp->rmt, 0, 0, pdu_ctrl);

        return -1;
}

static int default_rate_reduction(struct dtcp * instance)
{
        LOG_MISSING;

        return 0;
}

static int default_sv_update(struct dtcp * dtcp, seq_num_t seq)
{
        int retval = 0;

        if (!dtcp)
                return -1;

        /* FIXME: here it goes rcvr_flow_control_policy */
        if (dtcp->conn->policies_params.flow_ctrl) {
                if (dtcp->conn->policies_params.window_based_fctrl)
                        if (dtcp->policies->rcvr_flow_control(dtcp, seq)) {
                                LOG_ERR("Failed Rcvr Flow Control");
                                retval = -1;
                        }

                if (dtcp->conn->policies_params.rate_based_fctrl)
                        if (dtcp->policies->rate_reduction(dtcp)) {
                                LOG_ERR("Failed Rate Reduction");
                                retval = -1;
                        }
        }

        if (dtcp->conn->policies_params.rtx_ctrl)
                if (dtcp->policies->rcvr_ack(dtcp, seq)) {
                        LOG_ERR("Failed Rcvr Ack");
                        retval = -1;
                }

        if (dtcp->conn->policies_params.flow_ctrl &&
            !dtcp->conn->policies_params.rtx_ctrl)
                if (dtcp->policies->receiving_flow_control(dtcp)) {
                        LOG_ERR("Failed Receiving Flow Control");
                        retval = -1;
                }

        return retval;
}

static struct dtcp_sv default_sv = {
        .trd                    = 0,
        .pdus_per_time_unit     = 0,
        .next_snd_ctl_seq       = 0,
        .last_rcv_ctl_seq       = 0,
        .last_snd_data_ack      = 0,
        .send_left_wind_edge    = 0,
        .data_retransmit_max    = 0,
        .last_rcv_data_ack      = 0,
        .time_unit              = 0,
        .sndr_credit            = 0,
        .snd_rt_wind_edge       = 0,
        .sndr_rate              = 0,
        .pdus_sent_in_time_unit = 0,
        .rcvr_credit            = 0,
        .rcvr_rt_wind_edge      = 0,
        .rcvr_rate              = 0,
        .pdus_rcvd_in_time_unit = 0,
        .dup_acks               = 0,
        .dup_flow_ctl           = 0,
};

static struct dtcp_policies default_policies = {
        .flow_init                   = NULL,
        .sv_update                   = default_sv_update,
        .lost_control_pdu            = default_lost_control_pdu,
        .rtt_estimator               = NULL,
        .retransmission_timer_expiry = NULL,
        .received_retransmission     = NULL,
        .sending_ack                 = NULL,
        .sending_ack_list            = NULL,
        .initial_credit              = NULL,
        .initial_rate                = NULL,
        .receiving_flow_control      = NULL,
        .update_credit               = NULL,
        .flow_control_overrun        = NULL,
        .reconcile_flow_conflict     = NULL,
        .rcvr_ack                    = default_rcvr_ack,
        .rcvr_flow_control           = default_rcvr_flow_control,
        .rate_reduction              = default_rate_reduction,
};

struct dtcp * dtcp_create(struct dt *         dt,
                          struct connection * conn,
                          struct rmt *        rmt)
{
        struct dtcp * tmp;

        if (!dt) {
                LOG_ERR("No DT passed, bailing out");
                return NULL;
        }

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp) {
                LOG_ERR("Cannot create DTCP state-vector");
                return NULL;
        }

        tmp->parent = dt;

        tmp->sv = rkzalloc(sizeof(*tmp->sv), GFP_KERNEL);
        if (!tmp->sv) {
                LOG_ERR("Cannot create DTCP state-vector");
                dtcp_destroy(tmp);
                return NULL;
        }
        tmp->policies = rkzalloc(sizeof(*tmp->policies), GFP_KERNEL);
        if (!tmp->policies) {
                LOG_ERR("Cannot create DTCP policies");
                dtcp_destroy(tmp);
                return NULL;
        }

        *tmp->sv       = default_sv;
        /* FIXME: fixups to the state-vector should be placed here */

        *tmp->policies = default_policies;
        /* FIXME: fixups to the policies should be placed here */

        tmp->conn      = conn;

        LOG_DBG("Instance %pK created successfully", tmp);

        return tmp;
}

int dtcp_destroy(struct dtcp * instance)
{
        if (!instance) {
                LOG_ERR("Bad instance passed, bailing out");
                return -1;
        }

        if (instance->sv)       rkfree(instance->sv);
        if (instance->policies) rkfree(instance->policies);
        rkfree(instance);

        LOG_DBG("Instance %pK destroyed successfully", instance);

        return 0;
}

int dtcp_send(struct dtcp * instance,
              struct sdu *  sdu)
{
        LOG_MISSING;

        /* Takes the pdu and enqueue in its internal queues */

        return -1;
}

int dtcp_sv_update(struct dtcp * instance,
                   seq_num_t     seq)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        if (instance->policies->sv_update(instance, seq))
                return -1;

        return 0;
}

int dtcp_ack_flow_control_pdu_send(struct dtcp * dtcp)
{
        if (!dtcp)
                return -1;

        LOG_MISSING;

        return 0;
}

seq_num_t dtcp_rcv_rt_win(struct dtcp * instance)
{
        seq_num_t tmp;

        if (!instance || !instance->sv)
                return 0;

        spin_lock(&instance->sv->lock);
        tmp = instance->sv->rcvr_rt_wind_edge;
        spin_unlock(&instance->sv->lock);

        return tmp;
}
seq_num_t dtcp_snd_rt_win(struct dtcp * instance)
{
        seq_num_t tmp;

        if (!instance || !instance->sv)
                return 0;

        spin_lock(&instance->sv->lock);
        tmp = instance->sv->snd_rt_wind_edge;
        spin_unlock(&instance->sv->lock);

        return tmp;
}
