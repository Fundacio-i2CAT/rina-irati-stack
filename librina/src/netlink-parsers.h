//
// Core librina logic
//
//    Eduard Grasa          <eduard.grasa@i2cat.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef LIBRINA_NETLINK_PARSERS_H
#define LIBRINA_NETLINK_PARSERS_H

#include <netlink/msg.h>
#include <netlink/attr.h>
#include <netlink/genl/genl.h>

#include "netlink-messages.h"

namespace rina {

int putBaseNetlinkMessage(nl_msg* netlinkMessage, BaseNetlinkMessage * message);

BaseNetlinkMessage * parseBaseNetlinkMessage(nlmsghdr* netlinkMesasgeHeader);

/* APPLICATION PROCESS NAMING INFORMATION CLASS */
enum ApplicationProcessNamingInformationAttributes {
	APNI_ATTR_PROCESS_NAME = 1,
	APNI_ATTR_PROCESS_INSTANCE,
	APNI_ATTR_ENTITY_NAME,
	APNI_ATTR_ENTITY_INSTANCE,
	__APNI_ATTR_MAX,
};

#define APNI_ATTR_MAX (__APNI_ATTR_MAX -1)

int putApplicationProcessNamingInformationObject(nl_msg* netlinkMessage,
		const ApplicationProcessNamingInformation& object);

ApplicationProcessNamingInformation *
parseApplicationProcessNamingInformationObject(nlattr *nested);

/* AppAllocateFlowRequestMessage CLASS*/
enum AppAllocateFlowRequestAttributes {
	AAFR_ATTR_SOURCE_APP_NAME = 1,
	AAFR_ATTR_DEST_APP_NAME,
	AAFR_ATTR_FLOW_SPEC,
	AAFR_ATTR_DIF_NAME,
	__AAFR_ATTR_MAX,
};

#define AAFR_ATTR_MAX (__AAFR_ATTR_MAX -1)

int putAppAllocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestMessage& object);

AppAllocateFlowRequestMessage * parseAppAllocateFlowRequestMessage(
		nlmsghdr *hdr);

/* FLOW SPECIFICATION CLASS */
enum FlowSpecificationAttributes {
	FSPEC_ATTR_AVG_BWITH = 1,
	FSPEC_ATTR_AVG_SDU_BWITH,
	FSPEC_ATTR_DELAY,
	FSPEC_ATTR_JITTER,
	FSPEC_ATTR_MAX_GAP,
	FSPEC_ATTR_MAX_SDU_SIZE,
	FSPEC_ATTR_IN_ORD_DELIVERY,
	FSPEC_ATTR_PART_DELIVERY,
	FSPEC_ATTR_PEAK_BWITH_DURATION,
	FSPEC_ATTR_PEAK_SDU_BWITH_DURATION,
	FSPEC_ATTR_UNDETECTED_BER,
	__FSPEC_ATTR_MAX,
};

#define FSPEC_ATTR_MAX (__FSPEC_ATTR_MAX -1)

int putFlowSpecificationObject(nl_msg* netlinkMessage,
		const FlowSpecification& object);

FlowSpecification * parseFlowSpecificationObject(nlattr *nested);

/* PARAMETER CLASS */
enum ParameterAttributes {
	PARAM_ATTR_NAME = 1,
	PARAM_ATTR_VALUE,
	__PARAM_ATTR_MAX,
};

#define PARAM_ATTR_MAX (__PARAM_ATTR_MAX -1)

int putListOfParameters(
		nl_msg* netlinkMessage, const std::list<Parameter>& parameters);

int putParameterObject(nl_msg* netlinkMessage, const Parameter& object);

int parseListOfDIFConfigurationParameters(nlattr *nested,
		DIFConfiguration * difConfiguration);

Parameter * parseParameter(nlattr *nested);

/* AppAllocateFlowRequestResultMessage CLASS*/
enum AppAllocateFlowRequestResultAttributes {
	AAFRR_ATTR_SOURCE_APP_NAME = 1,
	AAFRR_ATTR_PORT_ID,
	AAFRR_ATTR_ERROR_DESCRIPTION,
	AAFRR_ATTR_DIF_NAME,
	__AAFRR_ATTR_MAX,
};

#define AAFRR_ATTR_MAX (__AAFRR_ATTR_MAX -1)

int putAppAllocateFlowRequestResultMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestResultMessage& object);

AppAllocateFlowRequestResultMessage * parseAppAllocateFlowRequestResultMessage(
		nlmsghdr *hdr);

/* AppAllocateFlowRequestArrivedMessage CLASS*/
enum AppAllocateFlowRequestArrivedAttributes {
	AAFRA_ATTR_SOURCE_APP_NAME = 1,
	AAFRA_ATTR_DEST_APP_NAME,
	AAFRA_ATTR_FLOW_SPEC,
	AAFRA_ATTR_PORT_ID,
	AAFRA_ATTR_DIF_NAME,
	__AAFRA_ATTR_MAX,
};

#define AAFRA_ATTR_MAX (__AAFRA_ATTR_MAX -1)

int putAppAllocateFlowRequestArrivedMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestArrivedMessage& object);

AppAllocateFlowRequestArrivedMessage * parseAppAllocateFlowRequestArrivedMessage(
		nlmsghdr *hdr);

/* AppAllocateFlowResponseMessage CLASS*/
enum AppAllocateFlowResponseAttributes {
	AAFRE_ATTR_RESULT = 1,
	AAFRE_ATTR_NOTIFY_SOURCE,
	__AAFRE_ATTR_MAX,
};

#define AAFRE_ATTR_MAX (__AAFRE_ATTR_MAX -1)

int putAppAllocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowResponseMessage& object);

AppAllocateFlowResponseMessage * parseAppAllocateFlowResponseMessage(
		nlmsghdr *hdr);

/* AppDeallocateFlowRequestMessage CLASS*/
enum AppDeallocateFlowRequestMessageAttributes {
	ADFRT_ATTR_PORT_ID = 1,
	ADFRT_ATTR_APP_NAME,
	__ADFRT_ATTR_MAX,
};

#define ADFRT_ATTR_MAX (__ADFRT_ATTR_MAX -1)

int putAppDeallocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const AppDeallocateFlowRequestMessage& object);

AppDeallocateFlowRequestMessage * parseAppDeallocateFlowRequestMessage(
		nlmsghdr *hdr);

/* AppDeallocateFlowResponseMessage CLASS*/
enum AppDeallocateFlowResponseMessageAttributes {
	ADFRE_ATTR_RESULT = 1,
	ADFRE_ATTR_APP_NAME,
	ADFRE_ATTR_PORT_ID,
	__ADFRE_ATTR_MAX,
};

#define ADFRE_ATTR_MAX (__ADFRE_ATTR_MAX -1)

int putAppDeallocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const AppDeallocateFlowResponseMessage& object);

AppDeallocateFlowResponseMessage * parseAppDeallocateFlowResponseMessage(
		nlmsghdr *hdr);

/* AppFlowDeallocatedNotificationMessage CLASS*/
enum AppFlowDeallocatedNotificationMessageAttributes {
	AFDN_ATTR_PORT_ID = 1,
	AFDN_ATTR_CODE,
	AFDN_ATTR_APP_NAME,
	__AFDN_ATTR_MAX,
};

#define AFDN_ATTR_MAX (__AFDN_ATTR_MAX -1)

int putAppFlowDeallocatedNotificationMessageObject(nl_msg* netlinkMessage,
		const AppFlowDeallocatedNotificationMessage& object);

AppFlowDeallocatedNotificationMessage * parseAppFlowDeallocatedNotificationMessage(
		nlmsghdr *hdr);

/* ApplicationRegistrationInformation CLASS*/
enum ApplicationRegistrationInformationAttributes {
        ARIA_ATTR_APP_NAME = 1,
	ARIA_ATTR_APP_REG_TYPE,
	ARIA_ATTR_APP_DIF_NAME,
	__ARIA_ATTR_MAX,
};

#define ARIA_ATTR_MAX (__ARIA_ATTR_MAX -1)

int putApplicationRegistrationInformationObject(nl_msg* netlinkMessage,
		const ApplicationRegistrationInformation& object);

ApplicationRegistrationInformation * parseApplicationRegistrationInformation(
		nlattr *nested);

/* AppRegisterApplicationRequestMessage CLASS*/
enum AppRegisterApplicationRequestMessageAttributes {
	ARAR_ATTR_APP_REG_INFO = 1,
	__ARAR_ATTR_MAX,
};

#define ARAR_ATTR_MAX (__ARAR_ATTR_MAX -1)

int putAppRegisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const AppRegisterApplicationRequestMessage& object);

AppRegisterApplicationRequestMessage * parseAppRegisterApplicationRequestMessage(
		nlmsghdr *hdr);

/* AppRegisterApplicationResponseMessage CLASS*/
enum AppRegisterApplicationResponseMessageAttributes {
	ARARE_ATTR_APP_NAME = 1,
	ARARE_ATTR_RESULT,
	ARARE_ATTR_DIF_NAME,
	__ARARE_ATTR_MAX,
};

#define ARARE_ATTR_MAX (__ARARE_ATTR_MAX -1)

int putAppRegisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const AppRegisterApplicationResponseMessage& object);

AppRegisterApplicationResponseMessage *
	parseAppRegisterApplicationResponseMessage(nlmsghdr *hdr);


/* AppUnregisterApplicationRequestMessage CLASS*/
enum AppUnregisterApplicationRequestMessageAttributes {
	AUAR_ATTR_APP_NAME = 1,
	AUAR_ATTR_DIF_NAME,
	__AUAR_ATTR_MAX,
};

#define AUAR_ATTR_MAX (__AUAR_ATTR_MAX -1)

int putAppUnregisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const AppUnregisterApplicationRequestMessage& object);

AppUnregisterApplicationRequestMessage * parseAppUnregisterApplicationRequestMessage(
		nlmsghdr *hdr);


/* AppUnregisterApplicationResponseMessage CLASS*/
enum AppUnregisterApplicationResponseMessageAttributes {
	AUARE_ATTR_RESULT = 1,
	AUARE_ATTR_APP_NAME,
	__AUARE_ATTR_MAX,
};

#define AUARE_ATTR_MAX (__AUARE_ATTR_MAX -1)

int putAppUnregisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const AppUnregisterApplicationResponseMessage& object);

AppUnregisterApplicationResponseMessage * parseAppUnregisterApplicationResponseMessage(
		nlmsghdr *hdr);


/* AppRegistrationCanceledNotificationMessage CLASS*/
enum AppRegistrationCanceledNotificationMessageAttributes {
	ARCN_ATTR_CODE = 1,
	ARCN_ATTR_REASON,
	ARCN_ATTR_APP_NAME,
	ARCN_ATTR_DIF_NAME,
	__ARCN_ATTR_MAX,
};

#define ARCN_ATTR_MAX (__ARCN_ATTR_MAX -1)

int putAppRegistrationCanceledNotificationMessageObject(nl_msg* netlinkMessage,
		const AppRegistrationCanceledNotificationMessage& object);

AppRegistrationCanceledNotificationMessage * parseAppRegistrationCanceledNotificationMessage(
		nlmsghdr *hdr);

/* AppGetDIFPropertiesRequestMessage CLASS*/
enum AppGetDIFPropertiesRequestMessageAttributes {
	AGDP_ATTR_APP_NAME = 1,
	AGDP_ATTR_DIF_NAME,
	__AGDP_ATTR_MAX,
};

#define AGDP_ATTR_MAX (__AGDP_ATTR_MAX -1)

int putAppGetDIFPropertiesRequestMessageObject(nl_msg* netlinkMessage,
		const AppGetDIFPropertiesRequestMessage& object);

AppGetDIFPropertiesRequestMessage * parseAppGetDIFPropertiesRequestMessage(
		nlmsghdr *hdr);

/* QOSCUBE CLASS*/
enum QoSCubesAttributes {
	QOS_CUBE_ATTR_NAME = 1,
	QOS_CUBE_ATTR_ID,
	QOS_CUBE_ATTR_AVG_BAND,
	QOS_CUBE_ATTR_AVG_SDU_BAND,
	QOS_CUBE_ATTR_PEAK_BAND_DUR,
	QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR,
	QOS_CUBE_ATTR_UND_BER,
	QOS_CUBE_ATTR_PART_DEL,
	QOS_CUBE_ATTR_ORD_DEL,
	QOS_CUBE_ATTR_MAX_GAP,
	QOS_CUBE_ATTR_DELAY,
	QOS_CUBE_ATTR_JITTER,
	__QOS_CUBE_ATTR_MAX,
};

#define QOS_CUBE_ATTR_MAX (__QOS_CUBE_ATTR_MAX -1)

int putQoSCubeObject(nl_msg* netlinkMessage,
		const QoSCube& object);

QoSCube * parseQoSCubeObject(nlmsghdr *hdr);

/* DIFPROPERTIES CLASS*/
enum DIFPropertiesAttributes {
	DIF_PROP_ATTR_DIF_NAME = 1,
	DIF_PROP_ATTR_MAX_SDU_SIZE,
	DIF_PROP_ATTR_QOS_CUBES,
	__DIF_PROP_ATTR_MAX,
};

#define DIF_PROP_ATTR_MAX (__DIF_PROP_ATTR_MAX -1)

int putDIFPropertiesObject(nl_msg* netlinkMessage,
		const DIFProperties& object);

DIFProperties * parseDIFPropertiesObject(nlmsghdr *hdr);

/* NEIGHBOR CLASS */
enum NeighborAttributes {
        NEIGH_ATTR_NAME = 1,
        NEIGH_ATTR_SUPP_DIF,
        __NEIGH_ATTR_MAX,
};

#define NEIGH_ATTR_MAX (__NEIGH_ATTR_MAX -1)

int putNeighborObject(nl_msg* netlinkMessage,
                const Neighbor& object);

Neighbor * parseNeighborObject(nlmsghdr *hdr);

/* AppGetDIFPropertiesResponseMessage CLASS*/
enum AppGetDIFPropertiesResponseMessageAttributes {
	AGDPR_ATTR_RESULT = 1,
	AGDPR_ATTR_APP_NAME,
	AGDPR_ATTR_DIF_PROPERTIES,
	__AGDPR_ATTR_MAX,
};

#define AGDPR_ATTR_MAX (__AGDPR_ATTR_MAX -1)

int putAppGetDIFPropertiesResponseMessageObject(nl_msg* netlinkMessage,
		const AppGetDIFPropertiesResponseMessage& object);

AppGetDIFPropertiesResponseMessage * parseAppGetDIFPropertiesResponseMessage(
		nlmsghdr *hdr);

/* IpcmRegisterApplicationRequestMessage CLASS*/
enum IpcmRegisterApplicationRequestMessageAttributes {
	IRAR_ATTR_APP_NAME = 1,
	IRAR_ATTR_DIF_NAME,
	IRAR_ATTR_REG_IPC_ID,
	__IRAR_ATTR_MAX,
};

#define IRAR_ATTR_MAX (__IRAR_ATTR_MAX -1)

int putIpcmRegisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmRegisterApplicationRequestMessage& object);

IpcmRegisterApplicationRequestMessage *
	parseIpcmRegisterApplicationRequestMessage(nlmsghdr *hdr);

/* IpcmRegisterApplicationResponseMessage CLASS*/
enum IpcmRegisterApplicationResponseMessageAttributes {
	IRARE_ATTR_RESULT = 1,
	__IRARE_ATTR_MAX,
};

#define IRARE_ATTR_MAX (__IRARE_ATTR_MAX -1)

int putIpcmRegisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmRegisterApplicationResponseMessage& object);

IpcmRegisterApplicationResponseMessage *
	parseIpcmRegisterApplicationResponseMessage(nlmsghdr *hdr);

/* IpcmUnregisterApplicationRequestMessage CLASS*/
enum IpcmUnregisterApplicationRequestMessageAttributes {
	IUAR_ATTR_APP_NAME = 1,
	IUAR_ATTR_DIF_NAME,
	__IUAR_ATTR_MAX,
};

#define IUAR_ATTR_MAX (__IUAR_ATTR_MAX -1)

int putIpcmUnregisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmUnregisterApplicationRequestMessage& object);

IpcmUnregisterApplicationRequestMessage *
	parseIpcmUnregisterApplicationRequestMessage(nlmsghdr *hdr);

/* IpcmUnregisterApplicationResponseMessage CLASS*/
enum IpcmUnregisterApplicationResponseMessageAttributes {
	IUARE_ATTR_RESULT = 1,
	__IUARE_ATTR_MAX,
};

#define IUARE_ATTR_MAX (__IUARE_ATTR_MAX -1)

int putIpcmUnregisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmUnregisterApplicationResponseMessage& object);

IpcmUnregisterApplicationResponseMessage *
	parseIpcmUnregisterApplicationResponseMessage(nlmsghdr *hdr);

/* DataTransferConstants CLASS */
enum DataTransferConstantsAttributes {
        DTC_ATTR_QOS_ID = 1,
        DTC_ATTR_PORT_ID,
        DTC_ATTR_CEP_ID,
        DTC_ATTR_SEQ_NUM,
        DTC_ATTR_ADDRESS,
        DTC_ATTR_LENGTH,
        DTC_ATTR_MAX_PDU_SIZE,
        DTC_ATTR_MAX_PDU_LIFE,
        DTC_ATTR_DIF_INTEGRITY,
        __DTC_ATTR_MAX,
};

#define DTC_ATTR_MAX (__DTC_ATTR_MAX -1)

int putDataTransferConstantsObject(nl_msg* netlinkMessage,
                const DataTransferConstants& object);

DataTransferConstants * parseDataTransferConstantsObject(nlattr *nested);

/* DIF Configuration CLASS */
enum DIFConfigurationAttributes {
	DCONF_ATTR_PARAMETERS = 1,
	DCONF_ATTR_DATA_TRANS_CONST,
	DCONF_ATTR_ADDRESS,
	DCONF_ATTR_QOS_CUBES,
	__DCONF_ATTR_MAX,
};

#define DCONF_ATTR_MAX (__DCONF_ATTR_MAX -1)

int putDIFConfigurationObject(nl_msg* netlinkMessage,
		const DIFConfiguration& object);

DIFConfiguration * parseDIFConfigurationObject(nlattr *nested);

/* DIF INFORMATION CLASS */
enum DIFInformationAttributes {
	DINFO_ATTR_DIF_TYPE = 1,
	DINFO_ATTR_DIF_NAME,
	DINFO_ATTR_DIF_CONFIG,
	__DINFO_ATTR_MAX,
};

#define DINFO_ATTR_MAX (__DINFO_ATTR_MAX -1)

int putDIFInformationObject(nl_msg* netlinkMessage,
		const DIFInformation& object);

DIFInformation * parseDIFInformationObject(nlattr *nested);

/* IpcmAssignToDIFRequestMessage CLASS*/
enum IpcmAssignToDIFRequestMessageAttributes {
	IATDR_ATTR_DIF_INFORMATION = 1,
	__IATDR_ATTR_MAX,
};

#define IATDR_ATTR_MAX (__IATDR_ATTR_MAX -1)

int putIpcmAssignToDIFRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmAssignToDIFRequestMessage& object);

IpcmAssignToDIFRequestMessage *
	parseIpcmAssignToDIFRequestMessage(nlmsghdr *hdr);

/* IpcmAssignToDIFResponseMessage CLASS*/
enum IpcmAssignToDIFResponseMessageAttributes {
	IATDRE_ATTR_RESULT = 1,
	__IATDRE_ATTR_MAX,
};

#define IATDRE_ATTR_MAX (__IATDRE_ATTR_MAX -1)

int putIpcmAssignToDIFResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmAssignToDIFResponseMessage& object);

IpcmAssignToDIFResponseMessage *
	parseIpcmAssignToDIFResponseMessage(nlmsghdr *hdr);

/* IpcmUpdateDIFConfiguraiotnRequestMessage CLASS*/
enum IpcmUpdateDIFConfigurationRequestMessageAttributes {
        IUDCR_ATTR_DIF_CONFIGURATION = 1,
        __IUDCR_ATTR_MAX,
};

#define IUDCR_ATTR_MAX (__IUDCR_ATTR_MAX -1)

int putIpcmUpdateDIFConfigurationRequestMessageObject(nl_msg* netlinkMessage,
                const IpcmUpdateDIFConfigurationRequestMessage& object);

IpcmUpdateDIFConfigurationRequestMessage *
        parseIpcmUpdateDIFConfigurationRequestMessage(nlmsghdr *hdr);

/* IpcmUpdateDIFConfigurationResponseMessage CLASS*/
enum IpcmUpdateDIFConfigurationResponseMessageAttributes {
        IUDCRE_ATTR_RESULT = 1,
        __IUDCRE_ATTR_MAX,
};

#define IUDCRE_ATTR_MAX (__IUDCRE_ATTR_MAX -1)

int putIpcmUpdateDIFConfigurationResponseMessageObject(nl_msg* netlinkMessage,
                const IpcmUpdateDIFConfigurationResponseMessage& object);

IpcmUpdateDIFConfigurationResponseMessage *
        parseIpcmUpdateDIFConfigurationResponseMessage(nlmsghdr *hdr);

/* IpcmEnrollToDIFRequestMessage CLASS*/
enum IpcmEnrollToDIFRequestMessageAttributes {
        IETDR_ATTR_DIF_NAME = 1,
        IETDR_ATTR_SUP_DIF_NAME,
        IETDR_ATTR_NEIGH,
        __IETDR_ATTR_MAX,
};

#define IETDR_ATTR_MAX (__IETDR_ATTR_MAX -1)

int putIpcmEnrollToDIFRequestMessageObject(nl_msg* netlinkMessage,
                const IpcmEnrollToDIFRequestMessage& object);

IpcmEnrollToDIFRequestMessage *
        parseIpcmEnrollToDIFRequestMessage(nlmsghdr *hdr);

/* IpcmEnrollToDIFResponseMessage CLASS*/
enum IpcmEnrollToDIFResponseMessageAttributes {
        IETDRE_ATTR_RESULT = 1,
        IETDRE_ATTR_NEIGHBORS,
        IETDRE_ATTR_DIF_INFO,
        __IETDRE_ATTR_MAX,
};

#define IETDRE_ATTR_MAX (__IETDRE_ATTR_MAX -1)

int putIpcmEnrollToDIFResponseMessageObject(nl_msg* netlinkMessage,
                const IpcmEnrollToDIFResponseMessage& object);

IpcmEnrollToDIFResponseMessage *
        parseIpcmEnrollToDIFResponseMessage(nlmsghdr *hdr);

/* IpcmNotifyNeighborsModifiedMessage CLASS*/
enum IpcmNotifyNeighborsModifiedMessageAttributes {
        INNMM_ATTR_ADDED = 1,
        INNMM_ATTR_NEIGHBORS,
        __INNMM_ATTR_MAX,
};

#define INNMM_ATTR_MAX (__INNMM_ATTR_MAX -1)

int putIpcmNotifyNeighborsModifiedMessageObject(nl_msg* netlinkMessage,
                const IpcmNotifyNeighborsModifiedMessage& object);

IpcmNotifyNeighborsModifiedMessage *
        parseIpcmNotifyNeighborsModifiedMessage(nlmsghdr *hdr);

/* IpcmAllocateFlowRequestMessage CLASS*/
enum IpcmAllocateFlowRequestMessageAttributes {
	IAFRM_ATTR_SOURCE_APP_NAME = 1,
	IAFRM_ATTR_DEST_APP_NAME,
	IAFRM_ATTR_FLOW_SPEC,
	IAFRM_ATTR_DIF_NAME,
	__IAFRM_ATTR_MAX,
};

#define IAFRM_ATTR_MAX (__IAFRM_ATTR_MAX -1)

int putIpcmAllocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestMessage& object);

IpcmAllocateFlowRequestMessage *
	parseIpcmAllocateFlowRequestMessage(nlmsghdr *hdr);

/* IpcmAllocateFlowRequestResultMessage CLASS*/
enum IpcmAllocateFlowRequestResultMessageAttributes {
	IAFRRM_ATTR_RESULT = 1,
	IAFRRM_ATTR_PORT_ID,
	__IAFRRM_ATTR_MAX,
};

#define IAFRRM_ATTR_MAX (__IAFRRM_ATTR_MAX -1)

int putIpcmAllocateFlowRequestResultMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestResultMessage& object);

IpcmAllocateFlowRequestResultMessage *
	parseIpcmAllocateFlowRequestResultMessage(nlmsghdr *hdr);

/* IpcmAllocateFlowRequestArrivedMessage CLASS*/
enum IpcmAllocateFlowRequestArrivedMessageAttributes {
	IAFRA_ATTR_SOURCE_APP_NAME = 1,
	IAFRA_ATTR_DEST_APP_NAME,
	IAFRA_ATTR_FLOW_SPEC,
	IAFRA_ATTR_DIF_NAME,
	IAFRA_ATTR_PORT_ID,
	__IAFRA_ATTR_MAX,
};

#define IAFRA_ATTR_MAX (__IAFRA_ATTR_MAX -1)

int putIpcmAllocateFlowRequestArrivedMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestArrivedMessage& object);

IpcmAllocateFlowRequestArrivedMessage * parseIpcmAllocateFlowRequestArrivedMessage(
		nlmsghdr *hdr);

/* IpcmAllocateFlowResponseMessage CLASS*/
enum IpcmAllocateFlowResponseAttributes {
	IAFRE_ATTR_RESULT = 1,
	IAFRE_ATTR_NOTIFY_SOURCE,
	__IAFRE_ATTR_MAX,
};

#define IAFRE_ATTR_MAX (__IAFRE_ATTR_MAX -1)

int putIpcmAllocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowResponseMessage& object);

IpcmAllocateFlowResponseMessage * parseIpcmAllocateFlowResponseMessage(
		nlmsghdr *hdr);

/* IpcmDeallocateFlowRequestMessage CLASS*/
enum IpcmDeallocateFlowRequestMessageAttributes {
	IDFRT_ATTR_PORT_ID = 1,
	__IDFRT_ATTR_MAX,
};

#define IDFRT_ATTR_MAX (__IDFRT_ATTR_MAX -1)

int putIpcmDeallocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmDeallocateFlowRequestMessage& object);

IpcmDeallocateFlowRequestMessage * parseIpcmDeallocateFlowRequestMessage(
		nlmsghdr *hdr);

/* IpcmDeallocateFlowResponseMessage CLASS*/
enum IpcmDeallocateFlowResponseMessageAttributes {
	IDFRE_ATTR_RESULT = 1,
	__IDFRE_ATTR_MAX,
};

#define IDFRE_ATTR_MAX (__IDFRE_ATTR_MAX -1)

int putIpcmDeallocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmDeallocateFlowResponseMessage& object);

IpcmDeallocateFlowResponseMessage * parseIpcmDeallocateFlowResponseMessage(
		nlmsghdr *hdr);

/* IpcmFlowDeallocatedNotificationMessage CLASS*/
enum IpcmFlowDeallocatedNotificationMessageAttributes {
	IFDN_ATTR_PORT_ID = 1,
	IFDN_ATTR_CODE,
	__IFDN_ATTR_MAX,
};

#define IFDN_ATTR_MAX (__IFDN_ATTR_MAX -1)

int putIpcmFlowDeallocatedNotificationMessageObject(nl_msg* netlinkMessage,
		const IpcmFlowDeallocatedNotificationMessage& object);

IpcmFlowDeallocatedNotificationMessage * parseIpcmFlowDeallocatedNotificationMessage(
		nlmsghdr *hdr);

/* IpcmDIFRegistrationNotification CLASS*/
enum IpcmDIFRegistrationNotificationAttributes {
	IDRN_ATTR_IPC_PROCESS_NAME = 1,
	IDRN_ATTR_DIF_NAME,
	IDRN_ATTR_REGISTRATION,
	__IDRN_ATTR_MAX,
};

#define IDRN_ATTR_MAX (__IDRN_ATTR_MAX -1)

int putIpcmDIFRegistrationNotificationObject(nl_msg* netlinkMessage,
		const IpcmDIFRegistrationNotification& object);

IpcmDIFRegistrationNotification *
	parseIpcmDIFRegistrationNotification(nlmsghdr *hdr);

/* IpcmDIFQueryRIBRequestMessage CLASS*/
enum IpcmDIFQueryRIBRequestMessageAttributes {
	IDQR_ATTR_OBJECT_CLASS = 1,
	IDQR_ATTR_OBJECT_NAME,
	IDQR_ATTR_OBJECT_INSTANCE,
	IDQR_ATTR_SCOPE,
	IDQR_ATTR_FILTER,
	__IDQR_ATTR_MAX,
};

#define IDQR_ATTR_MAX (__IDQR_ATTR_MAX -1)

int putIpcmDIFQueryRIBRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmDIFQueryRIBRequestMessage& object);

IpcmDIFQueryRIBRequestMessage *
	parseIpcmDIFQueryRIBRequestMessage(nlmsghdr *hdr);

/* RIBObject CLASS*/
enum RIBObjectAttributes {
	RIBO_ATTR_OBJECT_CLASS = 1,
	RIBO_ATTR_OBJECT_NAME,
	RIBO_ATTR_OBJECT_INSTANCE,
	RIBO_ATTR_OBJECT_DISPLAY_VALUE,
	__RIBO_ATTR_MAX,
};

#define RIBO_ATTR_MAX (__RIBO_ATTR_MAX -1)

int putRIBObject(nl_msg* netlinkMessage, const RIBObject& object);

RIBObject * parseRIBObject(nlattr *nested);

/* IpcmDIFQueryRIBResponseMessage CLASS*/
enum IpcmDIFQueryRIBResponseMessageAttributes {
	IDQRE_ATTR_RESULT = 1,
	IDQRE_ATTR_RIB_OBJECTS,
	__IDQRE_ATTR_MAX,
};

#define IDQRE_ATTR_MAX (__IDQRE_ATTR_MAX -1)

int putIpcmDIFQueryRIBResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmDIFQueryRIBResponseMessage& object);

IpcmDIFQueryRIBResponseMessage *
	parseIpcmDIFQueryRIBResponseMessage(nlmsghdr *hdr);

/* IpcmDIFQueryRIBResponseMessage CLASS*/
enum IpcmNLSocketClosedNotificationMessageAttributes {
	INSCN_ATTR_PORT = 1,
	__INSCN_ATTR_MAX,
};

#define INSCN_ATTR_MAX (__INSCN_ATTR_MAX -1)

IpcmNLSocketClosedNotificationMessage *
	parseIpcmNLSocketClosedNotificationMessage(nlmsghdr *hdr);

/* IpcmIPCProcessInitializedMessage CLASS*/
enum IpcmIPCProcessInitializedMessageAttributes {
        IIPM_ATTR_NAME = 1,
        __IIPM_ATTR_MAX,
};

#define IIPM_ATTR_MAX (__IIPM_ATTR_MAX -1)

int putIpcmIPCProcessInitializedMessageObject(nl_msg* netlinkMessage,
                const IpcmIPCProcessInitializedMessage& object);

IpcmIPCProcessInitializedMessage * parseIpcmIPCProcessInitializedMessage(
                nlmsghdr *hdr);

enum ConnectionPoliciesParametersAttributes {
	CPP_ATTR_DTCP_PRESENT = 1,
	CPP_ATTR_FLOW_CONTROL,
	CPP_ATTR_RTX_CONTROL,
	CPP_ATTR_WINDOW_BASED_FLOW_CONTROL,
	CPP_ATTR_RATE_BASED_FLOW_CONTROL,
	__CCP_ATTR_MAX,
};

#define CCP_ATTR_MAX (__CCP_ATTR_MAX -1)

int putConnectionPoliciesParametersObject(nl_msg * netlinkMessage,
                const ConnectionPoliciesParameters& object); 

ConnectionPoliciesParameters *
parseConnectionPoliciesParametersObject(nlattr *nested);

/* IpcpConnectionCreateRequestMessage CLASS*/
enum IpcpConnectionCreateRequestMessageAttributes {
        ICCRM_ATTR_PORT_ID = 1,
        ICCRM_ATTR_SRC_ADDRESS,
        ICCRM_ATTR_DEST_ADDRESS,
        ICCRM_ATTR_QOS_ID,
        ICCRM_ATTR_POLICIES_PARAMETERS,
        __ICCRM_ATTR_MAX,
};

#define ICCRM_ATTR_MAX (__ICCRM_ATTR_MAX -1)

int putIpcpConnectionCreateRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateRequestMessage& object);

IpcpConnectionCreateRequestMessage * parseIpcpConnectionCreateRequestMessage(
                nlmsghdr *hdr);

/* IpcpConnectionCreateResponseMessage CLASS */
enum IpcpConnectionCreateResponseMessageAttributes {
        ICCREM_ATTR_PORT_ID = 1,
        ICCREM_ATTR_SRC_CEP_ID,
        __ICCREM_ATTR_MAX,
};

#define ICCREM_ATTR_MAX (__ICCREM_ATTR_MAX -1)

int putIpcpConnectionCreateResponseMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateResponseMessage& object);

IpcpConnectionCreateResponseMessage * parseIpcpConnectionCreateResponseMessage(
                nlmsghdr *hdr);

/* IpcpConnectionUpdateRequestMessage CLASS*/
enum IpcpConnectionUpdateRequestMessageAttributes {
        ICURM_ATTR_PORT_ID = 1,
        ICURM_ATTR_SRC_CEP_ID,
        ICURM_ATTR_DEST_CEP_ID,
        ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID,
        __ICURM_ATTR_MAX,
};

#define ICURM_ATTR_MAX (__ICURM_ATTR_MAX -1)

int putIpcpConnectionUpdateRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionUpdateRequestMessage& object);

IpcpConnectionUpdateRequestMessage * parseIpcpConnectionUpdateRequestMessage(
                nlmsghdr *hdr);

/* IpcpConnectionUpdateResultMessage CLASS */
enum IpcpConnectionUpdateResultMessageAttributes {
        ICUREM_ATTR_PORT_ID = 1,
        ICUREM_ATTR_RESULT,
        __ICUREM_ATTR_MAX,
};

#define ICUREM_ATTR_MAX (__ICUREM_ATTR_MAX -1)

int putIpcpConnectionUpdateResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionUpdateResultMessage& object);

IpcpConnectionUpdateResultMessage * parseIpcpConnectionUpdateResultMessage(
                nlmsghdr *hdr);

/* IpcpConnectionCreateArrivedMessage CLASS*/
enum IpcpConnectionCreateArrivedMessageAttributes {
        ICCAM_ATTR_PORT_ID = 1,
        ICCAM_ATTR_SRC_ADDRESS,
        ICCAM_ATTR_DEST_ADDRESS,
        ICCAM_ATTR_DEST_CEP_ID,
        ICCAM_ATTR_QOS_ID,
        ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID,
        ICCAM_ATTR_POLICIES_PARAMETERS,
        __ICCAM_ATTR_MAX,
};

#define ICCAM_ATTR_MAX (__ICCAM_ATTR_MAX -1)

int putIpcpConnectionCreateArrivedMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateArrivedMessage& object);

IpcpConnectionCreateArrivedMessage * parseIpcpConnectionCreateArrivedMessage(
                nlmsghdr *hdr);

/* IpcpConnectionCreateResultMessage CLASS */
enum IpcpConnectionCreateResultMessageAttributes {
        ICCRES_ATTR_PORT_ID = 1,
        ICCRES_ATTR_SRC_CEP_ID,
        ICCRES_ATTR_DEST_CEP_ID,
        __ICCRES_ATTR_MAX,
};

#define ICCRES_ATTR_MAX (__ICCRES_ATTR_MAX -1)

int putIpcpConnectionCreateResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateResultMessage& object);

IpcpConnectionCreateResultMessage * parseIpcpConnectionCreateResultMessage(
                nlmsghdr *hdr);

/* IpcpConnectionDestroyRequestMessage CLASS */
enum IpcpConnectionDestroyRequestMessageAttributes {
        ICDRM_ATTR_PORT_ID = 1,
        ICDRM_ATTR_CEP_ID,
        __ICDRM_ATTR_MAX,
};

#define ICDRM_ATTR_MAX (__ICDRM_ATTR_MAX -1)

int putIpcpConnectionDestroyRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionDestroyRequestMessage& object);

IpcpConnectionDestroyRequestMessage * parseIpcpConnectionDestroyRequestMessage(
                nlmsghdr *hdr);

/* IpcpConnectionDestroyResultMessage CLASS */
enum IpcpConnectionDestroyResultMessageAttributes {
        ICDREM_ATTR_PORT_ID = 1,
        ICDREM_ATTR_RESULT,
        __ICDREM_ATTR_MAX,
};

#define ICDREM_ATTR_MAX (__ICDREM_ATTR_MAX -1)

int putIpcpConnectionDestroyResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionDestroyResultMessage& object);

IpcpConnectionDestroyResultMessage * parseIpcpConnectionDestroyResultMessage(
                nlmsghdr *hdr);

/* PDUForwardingTableEntry CLASS*/
enum PDUForwardingTableEntryAttributes {
        PFTE_ATTR_ADDRESS = 1,
        PFTE_ATTR_QOS_ID,
        PFTE_ATTR_PORT_IDS,
        __PFTE_ATTR_MAX,
};

#define PFTE_ATTR_MAX (__PFTE_ATTR_MAX -1)

int putPDUForwardingTableEntryObject(nl_msg* netlinkMessage,
                const PDUForwardingTableEntry& object);

PDUForwardingTableEntry * parsePDUForwardingTableEntry(nlattr *nested);

/* RmtModifyPDUFTEntriesRequestMessage CLASS */
enum RmtModifyPDUFTEntriesRequestMessageAttributes {
        RMPFTE_ATTR_ENTRIES= 1,
        RMPFTE_ATTR_MODE,
        __RMPFTE_ATTR_MAX,
};

#define RMPFTE_ATTR_MAX (__RMPFTE_ATTR_MAX -1)

int putRmtModifyPDUFTEntriesRequestObject(nl_msg* netlinkMessage,
                const RmtModifyPDUFTEntriesRequestMessage& object);

RmtModifyPDUFTEntriesRequestMessage * parseRmtModifyPDUFTEntriesRequestMessage(
                nlmsghdr *hdr);

/* RmtModifyPDUFTEntriesResponseMessageAttributes CLASS */
enum RmtDumpPDUFTEntriesResponseMessageAttributes {
        RDPFTE_ATTR_RESULT = 1,
        RDPFTE_ATTR_ENTRIES,
        __RDPFTE_ATTR_MAX,
};

#define RDPFTE_ATTR_MAX (__RDPFTE_ATTR_MAX -1)

int putRmtDumpPDUFTEntriesResponseObject(nl_msg* netlinkMessage,
                const RmtDumpPDUFTEntriesResponseMessage& object);

RmtDumpPDUFTEntriesResponseMessage * parseRmtDumpPDUFTEntriesResponseMessage(
                nlmsghdr *hdr);

}


#endif /* LIBRINA_NETLINK_PARSERS_H_ */
