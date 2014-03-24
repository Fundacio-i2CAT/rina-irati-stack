/*
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

/*
 * librina-netlink-parsers.cc
 *
 *  Created on: 14/06/2013
 *      Author: eduardgrasa
 */

#define RINA_PREFIX "netlink-parsers"

#include "logs.h"
#include "netlink-parsers.h"

namespace rina {

int putBaseNetlinkMessage(nl_msg* netlinkMessage,
		BaseNetlinkMessage * message) {
	switch (message->getOperationCode()) {
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST: {
		AppAllocateFlowRequestMessage * allocateObject =
				dynamic_cast<AppAllocateFlowRequestMessage *>(message);
		if (putAppAllocateFlowRequestMessageObject(netlinkMessage,
				*allocateObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_RESULT: {
		AppAllocateFlowRequestResultMessage * allocateFlowRequestResultObject =
				dynamic_cast<AppAllocateFlowRequestResultMessage *>(message);
		if (putAppAllocateFlowRequestResultMessageObject(netlinkMessage,
				*allocateFlowRequestResultObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_ARRIVED: {
		AppAllocateFlowRequestArrivedMessage * allocateFlowRequestArrivedObject =
				dynamic_cast<AppAllocateFlowRequestArrivedMessage *>(message);
		if (putAppAllocateFlowRequestArrivedMessageObject(netlinkMessage,
				*allocateFlowRequestArrivedObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_ALLOCATE_FLOW_RESPONSE: {
		AppAllocateFlowResponseMessage * allocateFlowResponseObject =
				dynamic_cast<AppAllocateFlowResponseMessage *>(message);
		if (putAppAllocateFlowResponseMessageObject(netlinkMessage,
				*allocateFlowResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_DEALLOCATE_FLOW_REQUEST: {
		AppDeallocateFlowRequestMessage * deallocateFlowRequestObject =
				dynamic_cast<AppDeallocateFlowRequestMessage *>(message);
		if (putAppDeallocateFlowRequestMessageObject(netlinkMessage,
				*deallocateFlowRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_DEALLOCATE_FLOW_RESPONSE: {
		AppDeallocateFlowResponseMessage * deallocateFlowResponseObject =
				dynamic_cast<AppDeallocateFlowResponseMessage *>(message);
		if (putAppDeallocateFlowResponseMessageObject(netlinkMessage,
				*deallocateFlowResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_FLOW_DEALLOCATED_NOTIFICATION: {
		AppFlowDeallocatedNotificationMessage * flowDeallocatedNotificationObject =
				dynamic_cast<AppFlowDeallocatedNotificationMessage *>(message);
		if (putAppFlowDeallocatedNotificationMessageObject(netlinkMessage,
				*flowDeallocatedNotificationObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_REGISTER_APPLICATION_REQUEST: {
		AppRegisterApplicationRequestMessage * registerApplicationRequestObject =
				dynamic_cast<AppRegisterApplicationRequestMessage *>(message);
		if (putAppRegisterApplicationRequestMessageObject(netlinkMessage,
				*registerApplicationRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_REGISTER_APPLICATION_RESPONSE: {
		AppRegisterApplicationResponseMessage * registerApplicationResponseObject =
				dynamic_cast<AppRegisterApplicationResponseMessage *>(message);
		if (putAppRegisterApplicationResponseMessageObject(netlinkMessage,
				*registerApplicationResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_UNREGISTER_APPLICATION_REQUEST: {
		AppUnregisterApplicationRequestMessage * unregisterApplicationRequestObject =
				dynamic_cast<AppUnregisterApplicationRequestMessage *>(message);
		if (putAppUnregisterApplicationRequestMessageObject(netlinkMessage,
				*unregisterApplicationRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_UNREGISTER_APPLICATION_RESPONSE: {
		AppUnregisterApplicationResponseMessage *
			unregisterApplicationResponseObject =
				dynamic_cast<AppUnregisterApplicationResponseMessage *>(message);
		if (putAppUnregisterApplicationResponseMessageObject(netlinkMessage,
				*unregisterApplicationResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_APPLICATION_REGISTRATION_CANCELED_NOTIFICATION: {
		AppRegistrationCanceledNotificationMessage *
		appRegistrationCanceledNotificationObject =
				dynamic_cast<AppRegistrationCanceledNotificationMessage *>(message);
		if (putAppRegistrationCanceledNotificationMessageObject(netlinkMessage,
				*appRegistrationCanceledNotificationObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_GET_DIF_PROPERTIES_REQUEST: {
		AppGetDIFPropertiesRequestMessage * getDIFPropertiesRequestMessage =
				dynamic_cast<AppGetDIFPropertiesRequestMessage *>(message);
		if (putAppGetDIFPropertiesRequestMessageObject(netlinkMessage,
				*getDIFPropertiesRequestMessage) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_APP_GET_DIF_PROPERTIES_RESPONSE: {
		AppGetDIFPropertiesResponseMessage * getDIFPropertiesResonseMessage =
				dynamic_cast<AppGetDIFPropertiesResponseMessage *>(message);
		if (putAppGetDIFPropertiesResponseMessageObject(netlinkMessage,
				*getDIFPropertiesResonseMessage) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_REGISTER_APPLICATION_REQUEST: {
		IpcmRegisterApplicationRequestMessage *
			registerApplicationRequestObject =
			dynamic_cast<IpcmRegisterApplicationRequestMessage *>(message);
		if (putIpcmRegisterApplicationRequestMessageObject(netlinkMessage,
				*registerApplicationRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_REGISTER_APPLICATION_RESPONSE: {
		IpcmRegisterApplicationResponseMessage *
			registerApplicationResponseObject =
			dynamic_cast<IpcmRegisterApplicationResponseMessage *>(message);
		if (putIpcmRegisterApplicationResponseMessageObject(netlinkMessage,
				*registerApplicationResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_UNREGISTER_APPLICATION_REQUEST: {
		IpcmUnregisterApplicationRequestMessage *
			unregisterApplicationRequestObject =
			dynamic_cast<IpcmUnregisterApplicationRequestMessage *>(message);
		if (putIpcmUnregisterApplicationRequestMessageObject(netlinkMessage,
				*unregisterApplicationRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_UNREGISTER_APPLICATION_RESPONSE: {
		IpcmUnregisterApplicationResponseMessage *
		unregisterApplicationResponseObject =
				dynamic_cast<IpcmUnregisterApplicationResponseMessage *>(message);
		if (putIpcmUnregisterApplicationResponseMessageObject(netlinkMessage,
				*unregisterApplicationResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_ASSIGN_TO_DIF_REQUEST: {
		IpcmAssignToDIFRequestMessage * assignToDIFRequestObject =
				dynamic_cast<IpcmAssignToDIFRequestMessage *>(message);
		if (putIpcmAssignToDIFRequestMessageObject(netlinkMessage,
				*assignToDIFRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_ASSIGN_TO_DIF_RESPONSE: {
		IpcmAssignToDIFResponseMessage * assignToDIFResponseObject =
				dynamic_cast<IpcmAssignToDIFResponseMessage *>(message);
		if (putIpcmAssignToDIFResponseMessageObject(netlinkMessage,
				*assignToDIFResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_REQUEST: {
                IpcmUpdateDIFConfigurationRequestMessage * updateDIFConfigurationRequest =
                                dynamic_cast<IpcmUpdateDIFConfigurationRequestMessage *>(message);
                if (putIpcmUpdateDIFConfigurationRequestMessageObject(netlinkMessage,
                                *updateDIFConfigurationRequest) < 0) {
                        return -1;
                }
                return 0;
	}
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_RESPONSE: {
	        IpcmUpdateDIFConfigurationResponseMessage * updateDIFConfigurationResponse =
	                        dynamic_cast<IpcmUpdateDIFConfigurationResponseMessage *>(message);
	        if (putIpcmUpdateDIFConfigurationResponseMessageObject(netlinkMessage,
	                        *updateDIFConfigurationResponse) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCM_ENROLL_TO_DIF_REQUEST: {
	        IpcmEnrollToDIFRequestMessage * request =
	                        dynamic_cast<IpcmEnrollToDIFRequestMessage *>(message);
	        if (putIpcmEnrollToDIFRequestMessageObject(netlinkMessage, *request) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCM_ENROLL_TO_DIF_RESPONSE: {
	        IpcmEnrollToDIFResponseMessage * request =
	                        dynamic_cast<IpcmEnrollToDIFResponseMessage *>(message);
	        if (putIpcmEnrollToDIFResponseMessageObject(netlinkMessage, *request) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCM_NEIGHBORS_MODIFIED_NOTIFICATION: {
	        IpcmNotifyNeighborsModifiedMessage * request =
	                        dynamic_cast<IpcmNotifyNeighborsModifiedMessage *>(message);
	        if (putIpcmNotifyNeighborsModifiedMessageObject(netlinkMessage, *request) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST: {
		IpcmAllocateFlowRequestMessage * allocateFlowRequestObject =
				dynamic_cast<IpcmAllocateFlowRequestMessage *>(message);
		if (putIpcmAllocateFlowRequestMessageObject(netlinkMessage,
				*allocateFlowRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_RESULT: {
		IpcmAllocateFlowRequestResultMessage * allocateFlowResponseObject =
				dynamic_cast<IpcmAllocateFlowRequestResultMessage *>(message);
		if (putIpcmAllocateFlowRequestResultMessageObject(netlinkMessage,
				*allocateFlowResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_ARRIVED: {
		IpcmAllocateFlowRequestArrivedMessage *
			allocateFlowRequestArrivedObject =
			dynamic_cast<IpcmAllocateFlowRequestArrivedMessage *>(message);
		if (putIpcmAllocateFlowRequestArrivedMessageObject(netlinkMessage,
				*allocateFlowRequestArrivedObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_RESPONSE: {
		IpcmAllocateFlowResponseMessage * allocateFlowResponseObject =
				dynamic_cast<IpcmAllocateFlowResponseMessage *>(message);
		if (putIpcmAllocateFlowResponseMessageObject(netlinkMessage,
				*allocateFlowResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_DEALLOCATE_FLOW_REQUEST: {
		IpcmDeallocateFlowRequestMessage * deallocateFlowRequestObject =
				dynamic_cast<IpcmDeallocateFlowRequestMessage *>(message);
		if (putIpcmDeallocateFlowRequestMessageObject(netlinkMessage,
				*deallocateFlowRequestObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_DEALLOCATE_FLOW_RESPONSE: {
		IpcmDeallocateFlowResponseMessage * deallocateFlowResponseObject =
				dynamic_cast<IpcmDeallocateFlowResponseMessage *>(message);
		if (putIpcmDeallocateFlowResponseMessageObject(netlinkMessage,
				*deallocateFlowResponseObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_FLOW_DEALLOCATED_NOTIFICATION: {
		IpcmFlowDeallocatedNotificationMessage * flowDeallocatedNotificationObject =
				dynamic_cast<IpcmFlowDeallocatedNotificationMessage *>(message);
		if (putIpcmFlowDeallocatedNotificationMessageObject(netlinkMessage,
				*flowDeallocatedNotificationObject) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION: {
		IpcmDIFRegistrationNotification * notificationMessage =
			dynamic_cast<IpcmDIFRegistrationNotification *>(message);
		if (putIpcmDIFRegistrationNotificationObject(netlinkMessage,
				*notificationMessage) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_QUERY_RIB_REQUEST: {
		IpcmDIFQueryRIBRequestMessage * queryRIBMessage =
				dynamic_cast<IpcmDIFQueryRIBRequestMessage *>(message);
		if (putIpcmDIFQueryRIBRequestMessageObject(netlinkMessage,
				*queryRIBMessage) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_QUERY_RIB_RESPONSE: {
		IpcmDIFQueryRIBResponseMessage * queryRIBMessage =
				dynamic_cast<IpcmDIFQueryRIBResponseMessage *>(message);
		if (putIpcmDIFQueryRIBResponseMessageObject(netlinkMessage,
				*queryRIBMessage) < 0) {
			return -1;
		}
		return 0;
	}
	case RINA_C_IPCM_IPC_MANAGER_PRESENT: {
		return 0;
	}
	case RINA_C_IPCM_IPC_PROCESS_INITIALIZED: {
	        IpcmIPCProcessInitializedMessage * queryRIBMessage =
	                        dynamic_cast<IpcmIPCProcessInitializedMessage *>(message);
	        if (putIpcmIPCProcessInitializedMessageObject(netlinkMessage,
	                        *queryRIBMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_CREATE_REQUEST: {
	        IpcpConnectionCreateRequestMessage * connCreateMessage =
	                        dynamic_cast<IpcpConnectionCreateRequestMessage *>(message);
	        if (putIpcpConnectionCreateRequestMessageObject(netlinkMessage,
	                        *connCreateMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_CREATE_RESPONSE: {
	        IpcpConnectionCreateResponseMessage * connCreateMessage =
	                        dynamic_cast<IpcpConnectionCreateResponseMessage *>(message);
	        if (putIpcpConnectionCreateResponseMessageObject(netlinkMessage,
	                        *connCreateMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_UPDATE_REQUEST: {
	        IpcpConnectionUpdateRequestMessage * connUpdateMessage =
	                        dynamic_cast<IpcpConnectionUpdateRequestMessage *>(message);
	        if (putIpcpConnectionUpdateRequestMessageObject(netlinkMessage,
	                        *connUpdateMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_UPDATE_RESULT: {
	        IpcpConnectionUpdateResultMessage * connUpdateMessage =
	                        dynamic_cast<IpcpConnectionUpdateResultMessage *>(message);
	        if (putIpcpConnectionUpdateResultMessageObject(netlinkMessage,
	                        *connUpdateMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_CREATE_ARRIVED: {
	        IpcpConnectionCreateArrivedMessage * connArrivedMessage =
	                        dynamic_cast<IpcpConnectionCreateArrivedMessage *>(message);
	        if (putIpcpConnectionCreateArrivedMessageObject(netlinkMessage,
	                        *connArrivedMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_CREATE_RESULT: {
	        IpcpConnectionCreateResultMessage * connArrivedMessage =
	                        dynamic_cast<IpcpConnectionCreateResultMessage *>(message);
	        if (putIpcpConnectionCreateResultMessageObject(netlinkMessage,
	                        *connArrivedMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_DESTROY_REQUEST: {
	        IpcpConnectionDestroyRequestMessage * connDestroyMessage =
	                        dynamic_cast<IpcpConnectionDestroyRequestMessage *>(message);
	        if (putIpcpConnectionDestroyRequestMessageObject(netlinkMessage,
	                        *connDestroyMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_IPCP_CONN_DESTROY_RESULT: {
	        IpcpConnectionDestroyResultMessage * connDestroyMessage =
	                        dynamic_cast<IpcpConnectionDestroyResultMessage *>(message);
	        if (putIpcpConnectionDestroyResultMessageObject(netlinkMessage,
	                        *connDestroyMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_RMT_MODIFY_FTE_REQUEST: {
	        RmtModifyPDUFTEntriesRequestMessage * rmtMERMessage =
	                        dynamic_cast<RmtModifyPDUFTEntriesRequestMessage *>(message);
	        if (putRmtModifyPDUFTEntriesRequestObject(netlinkMessage,
	                        *rmtMERMessage) < 0) {
	                return -1;
	        }
	        return 0;
	}
	case RINA_C_RMT_DUMP_FT_REQUEST: {
	        return 0;
	}
	case RINA_C_RMT_DUMP_FT_REPLY: {
	        RmtDumpPDUFTEntriesResponseMessage * rmtDumReply =
	                        dynamic_cast<RmtDumpPDUFTEntriesResponseMessage *>(message);
	        if (putRmtDumpPDUFTEntriesResponseObject(netlinkMessage,
	                        *rmtDumReply) < 0) {
	                return -1;
	        }
	        return 0;
	}
	default: {
		return -1;
	}

	}
}

BaseNetlinkMessage * parseBaseNetlinkMessage(nlmsghdr* netlinkMessageHeader) {
	struct genlmsghdr *nlhdr;
	nlhdr = (genlmsghdr *) nlmsg_data(netlinkMessageHeader);

	switch (nlhdr->cmd) {
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST: {
		return parseAppAllocateFlowRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_RESULT: {
		return parseAppAllocateFlowRequestResultMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_ARRIVED: {
		return parseAppAllocateFlowRequestArrivedMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_ALLOCATE_FLOW_RESPONSE: {
		return parseAppAllocateFlowResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_APP_DEALLOCATE_FLOW_REQUEST: {
		return parseAppDeallocateFlowRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_APP_DEALLOCATE_FLOW_RESPONSE: {
		return parseAppDeallocateFlowResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_APP_FLOW_DEALLOCATED_NOTIFICATION: {
		return parseAppFlowDeallocatedNotificationMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_REGISTER_APPLICATION_REQUEST: {
		return parseAppRegisterApplicationRequestMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_REGISTER_APPLICATION_RESPONSE: {
		return parseAppRegisterApplicationResponseMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_UNREGISTER_APPLICATION_REQUEST: {
		return parseAppUnregisterApplicationRequestMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_UNREGISTER_APPLICATION_RESPONSE: {
		return parseAppUnregisterApplicationResponseMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_GET_DIF_PROPERTIES_REQUEST: {
		return parseAppGetDIFPropertiesRequestMessage(
				netlinkMessageHeader);
	}
	case RINA_C_APP_GET_DIF_PROPERTIES_RESPONSE: {
		return parseAppGetDIFPropertiesResponseMessage(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_REGISTER_APPLICATION_REQUEST: {
		return parseIpcmRegisterApplicationRequestMessage(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_REGISTER_APPLICATION_RESPONSE: {
		return parseIpcmRegisterApplicationResponseMessage(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_UNREGISTER_APPLICATION_REQUEST: {
			return parseIpcmUnregisterApplicationRequestMessage(
					netlinkMessageHeader);
	}
	case RINA_C_IPCM_UNREGISTER_APPLICATION_RESPONSE: {
			return parseIpcmUnregisterApplicationResponseMessage(
					netlinkMessageHeader);
	}
	case RINA_C_APP_APPLICATION_REGISTRATION_CANCELED_NOTIFICATION: {
		return parseAppRegistrationCanceledNotificationMessage(
						netlinkMessageHeader);
	}
	case RINA_C_IPCM_ASSIGN_TO_DIF_REQUEST: {
		return parseIpcmAssignToDIFRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_ASSIGN_TO_DIF_RESPONSE: {
		return parseIpcmAssignToDIFResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_REQUEST: {
	        return parseIpcmUpdateDIFConfigurationRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_RESPONSE: {
	        return parseIpcmUpdateDIFConfigurationResponseMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCM_ENROLL_TO_DIF_REQUEST: {
	        return parseIpcmEnrollToDIFRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCM_ENROLL_TO_DIF_RESPONSE: {
	        return parseIpcmEnrollToDIFResponseMessage
	                        (netlinkMessageHeader);
	}
	case RINA_C_IPCM_NEIGHBORS_MODIFIED_NOTIFICATION: {
	        return parseIpcmNotifyNeighborsModifiedMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST: {
		return parseIpcmAllocateFlowRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_RESULT: {
		return parseIpcmAllocateFlowRequestResultMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_ARRIVED: {
		return parseIpcmAllocateFlowRequestArrivedMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_RESPONSE: {
		return parseIpcmAllocateFlowResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_DEALLOCATE_FLOW_REQUEST: {
		return parseIpcmDeallocateFlowRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_DEALLOCATE_FLOW_RESPONSE: {
		return parseIpcmDeallocateFlowResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_FLOW_DEALLOCATED_NOTIFICATION: {
		return parseIpcmFlowDeallocatedNotificationMessage(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION: {
		return parseIpcmDIFRegistrationNotification(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_QUERY_RIB_REQUEST: {
		return parseIpcmDIFQueryRIBRequestMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_QUERY_RIB_RESPONSE: {
		return parseIpcmDIFQueryRIBResponseMessage(
		                netlinkMessageHeader);
	}
	case RINA_C_IPCM_SOCKET_CLOSED_NOTIFICATION: {
		return parseIpcmNLSocketClosedNotificationMessage(
				netlinkMessageHeader);
	}
	case RINA_C_IPCM_IPC_PROCESS_INITIALIZED: {
	        return parseIpcmIPCProcessInitializedMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_CREATE_REQUEST: {
	        return parseIpcpConnectionCreateRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_CREATE_RESPONSE: {
	        return parseIpcpConnectionCreateResponseMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_UPDATE_REQUEST: {
	        return parseIpcpConnectionUpdateRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_UPDATE_RESULT: {
	        return parseIpcpConnectionUpdateResultMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_CREATE_ARRIVED: {
	        return parseIpcpConnectionCreateArrivedMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_CREATE_RESULT: {
	        return parseIpcpConnectionCreateResultMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_DESTROY_REQUEST: {
	        return parseIpcpConnectionDestroyRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_IPCP_CONN_DESTROY_RESULT: {
	        return parseIpcpConnectionDestroyResultMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_RMT_MODIFY_FTE_REQUEST: {
	        return parseRmtModifyPDUFTEntriesRequestMessage(
	                        netlinkMessageHeader);
	}
	case RINA_C_RMT_DUMP_FT_REPLY: {
	        return parseRmtDumpPDUFTEntriesResponseMessage(
	                        netlinkMessageHeader);
	}
	default: {
		LOG_ERR("Generic Netlink message contains unrecognized command code: %d",
			         nlhdr->cmd);
		return NULL;
	}
	}
}

int putApplicationProcessNamingInformationObject(nl_msg* netlinkMessage,
		const ApplicationProcessNamingInformation& object) {
	NLA_PUT_STRING(netlinkMessage, APNI_ATTR_PROCESS_NAME,
			object.getProcessName().c_str());
	NLA_PUT_STRING(netlinkMessage, APNI_ATTR_PROCESS_INSTANCE,
			object.getProcessInstance().c_str());
	NLA_PUT_STRING(netlinkMessage, APNI_ATTR_ENTITY_NAME,
			object.getEntityName().c_str());
	NLA_PUT_STRING(netlinkMessage, APNI_ATTR_ENTITY_INSTANCE,
			object.getEntityInstance().c_str());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building ApplicationProcessNamingInformation Netlink object");
	return -1;
}

ApplicationProcessNamingInformation *
parseApplicationProcessNamingInformationObject(nlattr *nested) {
	struct nla_policy attr_policy[APNI_ATTR_MAX + 1];
	attr_policy[APNI_ATTR_PROCESS_NAME].type = NLA_STRING;
	attr_policy[APNI_ATTR_PROCESS_NAME].minlen = 0;
	attr_policy[APNI_ATTR_PROCESS_NAME].maxlen = 65535;
	attr_policy[APNI_ATTR_PROCESS_INSTANCE].type = NLA_STRING;
	attr_policy[APNI_ATTR_PROCESS_INSTANCE].minlen = 0;
	attr_policy[APNI_ATTR_PROCESS_INSTANCE].maxlen = 65535;
	attr_policy[APNI_ATTR_ENTITY_NAME].type = NLA_STRING;
	attr_policy[APNI_ATTR_ENTITY_NAME].minlen = 0;
	attr_policy[APNI_ATTR_ENTITY_NAME].maxlen = 65535;
	attr_policy[APNI_ATTR_ENTITY_INSTANCE].type = NLA_STRING;
	attr_policy[APNI_ATTR_ENTITY_INSTANCE].minlen = 0;
	attr_policy[APNI_ATTR_ENTITY_INSTANCE].maxlen = 65535;
	struct nlattr *attrs[APNI_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, APNI_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing ApplicationProcessNaming information from Netlink message: %d",
				err);
		return NULL;
	}

	ApplicationProcessNamingInformation * result =
			new ApplicationProcessNamingInformation();
	if (attrs[APNI_ATTR_PROCESS_NAME]) {
		result->setProcessName(nla_get_string(attrs[APNI_ATTR_PROCESS_NAME]));
	}

	if (attrs[APNI_ATTR_PROCESS_INSTANCE]) {
		result->setProcessInstance(
				nla_get_string(attrs[APNI_ATTR_PROCESS_INSTANCE]));
	}

	if (attrs[APNI_ATTR_ENTITY_NAME]) {
		result->setEntityName(nla_get_string(attrs[APNI_ATTR_ENTITY_NAME]));
	}

	if (attrs[APNI_ATTR_ENTITY_INSTANCE]) {
		result->setEntityInstance(
				nla_get_string(attrs[APNI_ATTR_ENTITY_INSTANCE]));
	}

	return result;
}

int putFlowSpecificationObject(nl_msg* netlinkMessage,
		const FlowSpecification& object) {
	if (object.getAverageBandwidth() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_AVG_BWITH,
				object.getAverageBandwidth());
	}
	if (object.getAverageSduBandwidth() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_AVG_SDU_BWITH,
				object.getAverageSduBandwidth());
	}
	if (object.getDelay() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_DELAY, object.getDelay());
	}
	if (object.getJitter() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_JITTER, object.getJitter());
	}
	if (object.getMaxAllowableGap() >= 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_MAX_GAP,
				object.getMaxAllowableGap());
	}
	if (object.getMaxSDUSize() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_MAX_SDU_SIZE,
				object.getMaxSDUSize());
	}
	if (object.isOrderedDelivery()) {
		NLA_PUT_FLAG(netlinkMessage, FSPEC_ATTR_IN_ORD_DELIVERY);
	}
	if (object.isPartialDelivery()) {
		NLA_PUT_FLAG(netlinkMessage, FSPEC_ATTR_PART_DELIVERY);
	}
	if (object.getPeakBandwidthDuration() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_PEAK_BWITH_DURATION,
				object.getPeakBandwidthDuration());
	}
	if (object.getPeakSduBandwidthDuration() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_PEAK_SDU_BWITH_DURATION,
				object.getPeakSduBandwidthDuration());
	}
	if (object.getUndetectedBitErrorRate() > 0) {
		NLA_PUT_U32(netlinkMessage, FSPEC_ATTR_UNDETECTED_BER,
				object.getUndetectedBitErrorRate());
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building ApplicationProcessNamingInformation Netlink object");
	return -1;
}

FlowSpecification * parseFlowSpecificationObject(nlattr *nested) {
	struct nla_policy attr_policy[FSPEC_ATTR_MAX + 1];
	attr_policy[FSPEC_ATTR_AVG_BWITH].type = NLA_U32;
	attr_policy[FSPEC_ATTR_AVG_BWITH].minlen = 4;
	attr_policy[FSPEC_ATTR_AVG_BWITH].maxlen = 4;
	attr_policy[FSPEC_ATTR_AVG_SDU_BWITH].type = NLA_U32;
	attr_policy[FSPEC_ATTR_AVG_SDU_BWITH].minlen = 4;
	attr_policy[FSPEC_ATTR_AVG_SDU_BWITH].maxlen = 4;
	attr_policy[FSPEC_ATTR_DELAY].type = NLA_U32;
	attr_policy[FSPEC_ATTR_DELAY].minlen = 4;
	attr_policy[FSPEC_ATTR_DELAY].maxlen = 4;
	attr_policy[FSPEC_ATTR_JITTER].type = NLA_U32;
	attr_policy[FSPEC_ATTR_JITTER].minlen = 4;
	attr_policy[FSPEC_ATTR_JITTER].maxlen = 4;
	attr_policy[FSPEC_ATTR_MAX_GAP].type = NLA_U32;
	attr_policy[FSPEC_ATTR_MAX_GAP].minlen = 4;
	attr_policy[FSPEC_ATTR_MAX_GAP].maxlen = 4;
	attr_policy[FSPEC_ATTR_MAX_SDU_SIZE].type = NLA_U32;
	attr_policy[FSPEC_ATTR_MAX_SDU_SIZE].minlen = 4;
	attr_policy[FSPEC_ATTR_MAX_SDU_SIZE].maxlen = 4;
	attr_policy[FSPEC_ATTR_IN_ORD_DELIVERY].type = NLA_FLAG;
	attr_policy[FSPEC_ATTR_IN_ORD_DELIVERY].minlen = 0;
	attr_policy[FSPEC_ATTR_IN_ORD_DELIVERY].maxlen = 0;
	attr_policy[FSPEC_ATTR_PART_DELIVERY].type = NLA_FLAG;
	attr_policy[FSPEC_ATTR_PART_DELIVERY].minlen = 0;
	attr_policy[FSPEC_ATTR_PART_DELIVERY].maxlen = 0;
	attr_policy[FSPEC_ATTR_PEAK_BWITH_DURATION].type = NLA_U32;
	attr_policy[FSPEC_ATTR_PEAK_BWITH_DURATION].minlen = 4;
	attr_policy[FSPEC_ATTR_PEAK_BWITH_DURATION].maxlen = 4;
	attr_policy[FSPEC_ATTR_PEAK_SDU_BWITH_DURATION].type = NLA_U32;
	attr_policy[FSPEC_ATTR_PEAK_SDU_BWITH_DURATION].minlen = 4;
	attr_policy[FSPEC_ATTR_PEAK_SDU_BWITH_DURATION].maxlen = 4;
	attr_policy[FSPEC_ATTR_UNDETECTED_BER].type = NLA_U32;
	attr_policy[FSPEC_ATTR_UNDETECTED_BER].minlen = 4;
	attr_policy[FSPEC_ATTR_UNDETECTED_BER].maxlen = 4;
	struct nlattr *attrs[FSPEC_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, FSPEC_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing FlowSpecification object from Netlink message: %d",
				err);
		return NULL;
	}

	FlowSpecification * result = new FlowSpecification();
	if (attrs[FSPEC_ATTR_AVG_BWITH]) {
		result->setAverageBandwidth(nla_get_u32(attrs[FSPEC_ATTR_AVG_BWITH]));
	}

	if (attrs[FSPEC_ATTR_AVG_SDU_BWITH]) {
		result->setAverageSduBandwidth(
				nla_get_u32(attrs[FSPEC_ATTR_AVG_SDU_BWITH]));
	}

	if (attrs[FSPEC_ATTR_DELAY]) {
		result->setDelay(nla_get_u32(attrs[FSPEC_ATTR_DELAY]));
	}

	if (attrs[FSPEC_ATTR_JITTER]) {
		result->setJitter(nla_get_u32(attrs[FSPEC_ATTR_JITTER]));
	}

	if (attrs[FSPEC_ATTR_MAX_GAP]) {
		result->setMaxAllowableGap(nla_get_u32(attrs[FSPEC_ATTR_MAX_GAP]));
	}

	if (attrs[FSPEC_ATTR_MAX_SDU_SIZE]) {
		result->setMaxSDUSize(nla_get_u32(attrs[FSPEC_ATTR_MAX_SDU_SIZE]));
	}

	if (attrs[FSPEC_ATTR_IN_ORD_DELIVERY]) {
		result->setOrderedDelivery(true);
	} else {
		result->setOrderedDelivery(false);
	}

	if (attrs[FSPEC_ATTR_PART_DELIVERY]) {
		result->setPartialDelivery(true);
	} else {
		result->setPartialDelivery(false);
	}

	if (attrs[FSPEC_ATTR_PEAK_BWITH_DURATION]) {
		result->setPeakBandwidthDuration(
				nla_get_u32(attrs[FSPEC_ATTR_PEAK_BWITH_DURATION]));
	}

	if (attrs[FSPEC_ATTR_PEAK_SDU_BWITH_DURATION]) {
		result->setPeakSduBandwidthDuration(
				nla_get_u32(attrs[FSPEC_ATTR_PEAK_SDU_BWITH_DURATION]));
	}

	return result;
}

QoSCube * parseQoSCubeObject(nlattr *nested) {
	struct nla_policy attr_policy[QOS_CUBE_ATTR_MAX + 1];
	attr_policy[QOS_CUBE_ATTR_NAME].type = NLA_STRING;
	attr_policy[QOS_CUBE_ATTR_NAME].minlen = 0;
	attr_policy[QOS_CUBE_ATTR_NAME].maxlen = 65535;
	attr_policy[QOS_CUBE_ATTR_ID].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_ID].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_ID].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_AVG_BAND].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_AVG_BAND].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_AVG_BAND].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_AVG_SDU_BAND].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_AVG_SDU_BAND].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_AVG_SDU_BAND].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_DELAY].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_DELAY].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_DELAY].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_JITTER].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_JITTER].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_JITTER].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_MAX_GAP].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_MAX_GAP].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_MAX_GAP].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_ORD_DEL].type = NLA_FLAG;
	attr_policy[QOS_CUBE_ATTR_ORD_DEL].minlen = 0;
	attr_policy[QOS_CUBE_ATTR_ORD_DEL].maxlen = 0;
	attr_policy[QOS_CUBE_ATTR_PART_DEL].type = NLA_FLAG;
	attr_policy[QOS_CUBE_ATTR_PART_DEL].minlen = 0;
	attr_policy[QOS_CUBE_ATTR_PART_DEL].maxlen = 0;
	attr_policy[QOS_CUBE_ATTR_PEAK_BAND_DUR].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_PEAK_BAND_DUR].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_PEAK_BAND_DUR].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR].maxlen = 4;
	attr_policy[QOS_CUBE_ATTR_UND_BER].type = NLA_U32;
	attr_policy[QOS_CUBE_ATTR_UND_BER].minlen = 4;
	attr_policy[QOS_CUBE_ATTR_UND_BER].maxlen = 4;
	struct nlattr *attrs[QOS_CUBE_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, QOS_CUBE_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing QoS Cube object from Netlink message: %d",
				err);
		return NULL;
	}

	QoSCube * result = new QoSCube(nla_get_string(attrs[QOS_CUBE_ATTR_NAME]),
			nla_get_u32(attrs[QOS_CUBE_ATTR_ID]));

	if (attrs[QOS_CUBE_ATTR_AVG_BAND]) {
		result->setAverageBandwidth(nla_get_u32(attrs[QOS_CUBE_ATTR_AVG_BAND]));
	}

	if (attrs[QOS_CUBE_ATTR_AVG_SDU_BAND]) {
		result->setAverageSduBandwidth(
				nla_get_u32(attrs[QOS_CUBE_ATTR_AVG_SDU_BAND]));
	}

	if (attrs[QOS_CUBE_ATTR_DELAY]) {
		result->setDelay(nla_get_u32(attrs[QOS_CUBE_ATTR_DELAY]));
	}

	if (attrs[QOS_CUBE_ATTR_JITTER]) {
		result->setJitter(nla_get_u32(attrs[QOS_CUBE_ATTR_JITTER]));
	}

	if (attrs[QOS_CUBE_ATTR_MAX_GAP]) {
		result->setMaxAllowableGap(nla_get_u32(attrs[QOS_CUBE_ATTR_MAX_GAP]));
	}

	if (attrs[QOS_CUBE_ATTR_ORD_DEL]) {
		result->setOrderedDelivery(true);
	} else {
		result->setOrderedDelivery(false);
	}

	if (attrs[QOS_CUBE_ATTR_PART_DEL]) {
		result->setPartialDelivery(true);
	} else {
		result->setPartialDelivery(false);
	}

	if (attrs[QOS_CUBE_ATTR_PEAK_BAND_DUR]) {
		result->setPeakBandwidthDuration(
				nla_get_u32(attrs[QOS_CUBE_ATTR_PEAK_BAND_DUR]));
	}

	if (attrs[QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR]) {
		result->setPeakSduBandwidthDuration(
				nla_get_u32(attrs[QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR]));
	}

	return result;
}

int putQoSCubeObject(nl_msg* netlinkMessage,
		const QoSCube& object){

	NLA_PUT_STRING(netlinkMessage, QOS_CUBE_ATTR_NAME,
			object.getName().c_str());

	NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_ID, object.getId());

	if (object.getAverageBandwidth() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_AVG_BAND,
				object.getAverageBandwidth());
	}

	if (object.getAverageSduBandwidth() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_AVG_SDU_BAND,
				object.getAverageSduBandwidth());
	}

	if (object.getDelay() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_DELAY, object.getDelay());
	}

	if (object.getJitter() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_JITTER, object.getJitter());
	}

	if (object.getMaxAllowableGap() >= 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_MAX_GAP,
				object.getMaxAllowableGap());
	}

	if (object.isOrderedDelivery()) {
		NLA_PUT_FLAG(netlinkMessage, QOS_CUBE_ATTR_ORD_DEL);
	}

	if (object.isPartialDelivery()) {
		NLA_PUT_FLAG(netlinkMessage, QOS_CUBE_ATTR_PART_DEL);
	}

	if (object.getPeakBandwidthDuration() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_PEAK_BAND_DUR,
				object.getPeakBandwidthDuration());
	}

	if (object.getPeakSduBandwidthDuration() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_PEAK_SDU_BAND_DUR,
				object.getPeakSduBandwidthDuration());
	}

	/*if (object.getUndetectedBitErrorRate() > 0) {
		NLA_PUT_U32(netlinkMessage, QOS_CUBE_ATTR_UND_BER,
				object.getUndetectedBitErrorRate());
	}*/

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building QosCube Netlink object");
	return -1;
}

int putListOfQoSCubeObjects(
		nl_msg* netlinkMessage, const std::list<QoSCube>& qosCubes){
	std::list<QoSCube>::const_iterator iterator;
	struct nlattr *qosCube;
	int i = 0;

	for (iterator = qosCubes.begin();
			iterator != qosCubes.end();
			++iterator) {
		if (!(qosCube = nla_nest_start(netlinkMessage, i))){
			goto nla_put_failure;
		}
		if (putQoSCubeObject(netlinkMessage, *iterator) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, qosCube);
		i++;
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building QosCubeObject Netlink object");
	return -1;
}

int putDIFPropertiesObject(nl_msg* netlinkMessage,
		const DIFProperties& object){
	struct nlattr *difName, *qosCubes;

	if (!(difName = nla_nest_start(netlinkMessage,
			DIF_PROP_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}

	nla_nest_end(netlinkMessage, difName);

	NLA_PUT_U32(netlinkMessage, DIF_PROP_ATTR_MAX_SDU_SIZE,
			object.getMaxSduSize());

	if (object.getQoSCubes().size() > 0){
		if (!(qosCubes = nla_nest_start(netlinkMessage,
				DIF_PROP_ATTR_QOS_CUBES))) {
			goto nla_put_failure;
		}

		if (putListOfQoSCubeObjects(netlinkMessage,
				object.getQoSCubes()) < 0) {
			goto nla_put_failure;
		}

		nla_nest_end(netlinkMessage, qosCubes);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building DIF Properties Netlink object");
	return -1;
}

int parseListOfQoSCubes(nlattr *nested,
		DIFProperties * difProperties){
	nlattr * nla;
	int rem;
	QoSCube * qosCube;

	for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
		     nla_ok(nla, rem);
		     nla = nla_next(nla, &(rem))){
		/* validate & parse attribute */
		qosCube = parseQoSCubeObject(nla);
		if (qosCube == 0){
			return -1;
		}
		difProperties->addQoSCube(*qosCube);
		delete qosCube;
	}

	if (rem > 0){
		LOG_WARN("Missing bits to parse");
	}

	return 0;
}

int parseListOfQoSCubesForDIFConfiguration(nlattr *nested,
                DIFConfiguration * difConfiguration){
        nlattr * nla;
        int rem;
        QoSCube * qosCube;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                     nla_ok(nla, rem);
                     nla = nla_next(nla, &(rem))){
                /* validate & parse attribute */
                qosCube = parseQoSCubeObject(nla);
                if (qosCube == 0){
                        return -1;
                }
                difConfiguration->addQoSCube(*qosCube);
                delete qosCube;
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

DIFProperties * parseDIFPropertiesObject(nlattr *nested){
	struct nla_policy attr_policy[DIF_PROP_ATTR_MAX + 1];
	attr_policy[DIF_PROP_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[DIF_PROP_ATTR_DIF_NAME].minlen = 0;
	attr_policy[DIF_PROP_ATTR_DIF_NAME].maxlen = 0;
	attr_policy[DIF_PROP_ATTR_MAX_SDU_SIZE].type = NLA_U32;
	attr_policy[DIF_PROP_ATTR_MAX_SDU_SIZE].minlen = 0;
	attr_policy[DIF_PROP_ATTR_MAX_SDU_SIZE].maxlen = 65535;
	attr_policy[DIF_PROP_ATTR_QOS_CUBES].type = NLA_NESTED;
	attr_policy[DIF_PROP_ATTR_QOS_CUBES].minlen = 0;
	attr_policy[DIF_PROP_ATTR_QOS_CUBES].maxlen = 0;
	struct nlattr *attrs[DIF_PROP_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, DIF_PROP_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing DIF Properties object from Netlink message: %d",
				err);
		return 0;
	}

	ApplicationProcessNamingInformation * difName = 0;

	if (attrs[IUAR_ATTR_APP_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[DIF_PROP_ATTR_DIF_NAME]);
		if (difName == 0) {
			return 0;
		}
	}

	DIFProperties * result = new DIFProperties(*difName,
			nla_get_u32(attrs[DIF_PROP_ATTR_MAX_SDU_SIZE]));
	delete difName;

	int status = 0;
	if (attrs[DIF_PROP_ATTR_QOS_CUBES]) {
		status = parseListOfQoSCubes(
				attrs[DIF_PROP_ATTR_QOS_CUBES], result);
		if (status != 0){
			delete result;
			return 0;
		}
	}

	return result;
}

int putParameterObject(nl_msg* netlinkMessage, const Parameter& object){
	NLA_PUT_STRING(netlinkMessage, PARAM_ATTR_NAME,
			object.getName().c_str());

	NLA_PUT_STRING(netlinkMessage, PARAM_ATTR_VALUE,
				object.getValue().c_str());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building Parameter Netlink object");
	return -1;
}

int putListOfParameters(
		nl_msg* netlinkMessage, const std::list<Parameter>& parameters){
	std::list<Parameter>::const_iterator iterator;
	struct nlattr *parameter;
	int i = 0;

	for (iterator = parameters.begin();
			iterator != parameters.end();
			++iterator) {
		if (!(parameter = nla_nest_start(netlinkMessage, i))){
			goto nla_put_failure;
		}
		if (putParameterObject(netlinkMessage, *iterator) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, parameter);
		i++;
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building List of Parameters Netlink object");
	return -1;
}

Parameter * parseParameter(nlattr *nested){
	struct nla_policy attr_policy[PARAM_ATTR_MAX + 1];
	attr_policy[PARAM_ATTR_NAME].type = NLA_STRING;
	attr_policy[PARAM_ATTR_NAME].minlen = 0;
	attr_policy[PARAM_ATTR_NAME].maxlen = 65535;
	attr_policy[PARAM_ATTR_VALUE].type = NLA_STRING;
	attr_policy[PARAM_ATTR_VALUE].minlen = 0;
	attr_policy[PARAM_ATTR_VALUE].maxlen = 65535;
	struct nlattr *attrs[PARAM_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, PARAM_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing Parameter from Netlink message: %d",
				err);
		return 0;
	}

	Parameter * result = new Parameter();

	if (attrs[PARAM_ATTR_NAME]){
		result->setName(
				nla_get_string(attrs[PARAM_ATTR_NAME]));
	}

	if (attrs[PARAM_ATTR_VALUE]){
		result->setValue(
				nla_get_string(attrs[PARAM_ATTR_VALUE]));
	}

	return result;
}

int parseListOfDIFConfigurationParameters(nlattr *nested,
		DIFConfiguration * difConfiguration){
	nlattr * nla;
	int rem;
	Parameter * parameter;

	for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
		     nla_ok(nla, rem);
		     nla = nla_next(nla, &(rem))){
		/* validate & parse attribute */
		parameter = parseParameter(nla);
		if (parameter == 0){
			return -1;
		}
		difConfiguration->addParameter(*parameter);
		delete parameter;
	}

	if (rem > 0){
		LOG_WARN("Missing bits to parse");
	}

	return 0;
}

int putNeighborObject(nl_msg* netlinkMessage,
                const Neighbor& object) {
        struct nlattr *name, *supportingDIFName;

        if (!(name = nla_nest_start(netlinkMessage,
                        NEIGH_ATTR_NAME))) {
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getName()) < 0) {
                goto nla_put_failure;
        }

        nla_nest_end(netlinkMessage, name);

        if (!(supportingDIFName = nla_nest_start(netlinkMessage,
                        NEIGH_ATTR_SUPP_DIF))) {
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getSupportingDifName()) < 0) {
                goto nla_put_failure;
        }

        nla_nest_end(netlinkMessage, supportingDIFName);

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building Neighbor Netlink object");
        return -1;
}

int putListOfNeighbors(
                nl_msg* netlinkMessage, const std::list<Neighbor>& neighbors){
        std::list<Neighbor>::const_iterator iterator;
        struct nlattr *neighbor;
        int i = 0;

        for (iterator = neighbors.begin();
                        iterator != neighbors.end();
                        ++iterator) {
                if (!(neighbor = nla_nest_start(netlinkMessage, i))){
                        goto nla_put_failure;
                }
                if (putNeighborObject(netlinkMessage, *iterator) < 0) {
                        goto nla_put_failure;
                }
                nla_nest_end(netlinkMessage, neighbor);
                i++;
        }

        return 0;

        nla_put_failure: LOG_ERR(
                "Error building List of Neighbors Netlink object");
        return -1;
}

Neighbor * parseNeighborObject(nlattr *nested) {
        struct nla_policy attr_policy[NEIGH_ATTR_MAX + 1];
        attr_policy[NEIGH_ATTR_NAME].type = NLA_NESTED;
        attr_policy[NEIGH_ATTR_NAME].minlen = 0;
        attr_policy[NEIGH_ATTR_NAME].maxlen = 0;
        attr_policy[NEIGH_ATTR_SUPP_DIF].type = NLA_NESTED;
        attr_policy[NEIGH_ATTR_SUPP_DIF].minlen = 0;
        attr_policy[NEIGH_ATTR_SUPP_DIF].maxlen = 0;
        struct nlattr *attrs[NEIGH_ATTR_MAX + 1];

        int err = nla_parse_nested(attrs, NEIGH_ATTR_MAX, nested, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing DIF Properties object from Netlink message: %d",
                        err);
                return 0;
        }

        Neighbor * result = new Neighbor();
        ApplicationProcessNamingInformation * name = 0;
        ApplicationProcessNamingInformation * supportingDIF = 0;

        if (attrs[NEIGH_ATTR_NAME]) {
                name = parseApplicationProcessNamingInformationObject(
                                attrs[NEIGH_ATTR_NAME]);
                if (name == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setName(*name);
                        delete name;
                }
        }

        if (attrs[NEIGH_ATTR_SUPP_DIF]) {
                supportingDIF = parseApplicationProcessNamingInformationObject(
                                attrs[NEIGH_ATTR_SUPP_DIF]);
                if (supportingDIF == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setSupportingDifName(*supportingDIF);
                        delete supportingDIF;
                }
        }

        return result;
}

int parseListOfEnrollToDIFResponseNeighbors(nlattr *nested,
                IpcmEnrollToDIFResponseMessage * message){
        nlattr * nla;
        int rem;
        Neighbor * neighbor;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                     nla_ok(nla, rem);
                     nla = nla_next(nla, &(rem))){
                /* validate & parse attribute */
                neighbor = parseNeighborObject(nla);
                if (neighbor == 0){
                        return -1;
                }
                message->addNeighbor(*neighbor);
                delete neighbor;
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

int parseListOfNotifyNeighborsModifiedMessageNeighbors(nlattr *nested,
                IpcmNotifyNeighborsModifiedMessage * message) {
        nlattr * nla;
        int rem;
        Neighbor * neighbor;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                        nla_ok(nla, rem);
                        nla = nla_next(nla, &(rem))){
                /* validate & parse attribute */
                neighbor = parseNeighborObject(nla);
                if (neighbor == 0){
                        return -1;
                }
                message->addNeighbor(*neighbor);
                delete neighbor;
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

int putApplicationRegistrationInformationObject(nl_msg* netlinkMessage,
		const ApplicationRegistrationInformation& object){
	struct nlattr *appName, *difName;
	if (!(appName = nla_nest_start(netlinkMessage,
	                ARIA_ATTR_APP_NAME))) {
	        goto nla_put_failure;
	}

	if (putApplicationProcessNamingInformationObject(netlinkMessage,
	                object.getApplicationName()) < 0) {
	        goto nla_put_failure;
	}

	nla_nest_end(netlinkMessage, appName);

	NLA_PUT_U32(netlinkMessage, ARIA_ATTR_APP_REG_TYPE,
			object.getRegistrationType());

	if (object.getRegistrationType() == APPLICATION_REGISTRATION_SINGLE_DIF){
		if (!(difName = nla_nest_start(netlinkMessage,
				ARIA_ATTR_APP_DIF_NAME))) {
			goto nla_put_failure;
		}

		if (putApplicationProcessNamingInformationObject(netlinkMessage,
				object.getDIFName()) < 0) {
			goto nla_put_failure;
		}

		nla_nest_end(netlinkMessage, difName);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building DIF Properties Netlink object");
	return -1;
}

ApplicationRegistrationInformation * parseApplicationRegistrationInformation(
		nlattr *nested){
	struct nla_policy attr_policy[ARIA_ATTR_MAX + 1];
	attr_policy[ARIA_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[ARIA_ATTR_APP_NAME].minlen = 0;
	attr_policy[ARIA_ATTR_APP_DIF_NAME].maxlen = 0;
	attr_policy[ARIA_ATTR_APP_REG_TYPE].type = NLA_U32;
	attr_policy[ARIA_ATTR_APP_REG_TYPE].minlen = 0;
	attr_policy[ARIA_ATTR_APP_REG_TYPE].maxlen = 65535;
	attr_policy[ARIA_ATTR_APP_DIF_NAME].type = NLA_NESTED;
	attr_policy[ARIA_ATTR_APP_DIF_NAME].minlen = 0;
	attr_policy[ARIA_ATTR_APP_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[ARIA_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, ARIA_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
			"Error parsing ApplicationRegistrationInformation object from Netlink message: %d",
			err);
		return 0;
	}

	ApplicationRegistrationInformation * result = new ApplicationRegistrationInformation(
			static_cast<ApplicationRegistrationType>(
					nla_get_u32(attrs[ARIA_ATTR_APP_REG_TYPE])));
	ApplicationProcessNamingInformation * difName;
	ApplicationProcessNamingInformation * appName;

	if (attrs[ARIA_ATTR_APP_NAME]) {
	        appName = parseApplicationProcessNamingInformationObject(
	                        attrs[ARIA_ATTR_APP_NAME]);
	        if (appName == 0) {
	                delete result;
	                return 0;
	        } else {
	                result->setApplicationName(*appName);
	                delete appName;
	        }
	}

	if (attrs[ARIA_ATTR_APP_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[ARIA_ATTR_APP_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDIFName(*difName);
			delete difName;
		}
	}

	return result;
}

int putConnectionPoliciesParametersObject(nl_msg* netlinkMessage,
		const ConnectionPoliciesParameters& object) {
	if (object.isDTCPpresent())
	        NLA_PUT_FLAG(netlinkMessage, CPP_ATTR_DTCP_PRESENT);
	if (object.isFlowControl())
	        NLA_PUT_FLAG(netlinkMessage, CPP_ATTR_FLOW_CONTROL);
	if (object.isRTXcontrol())
	        NLA_PUT_FLAG(netlinkMessage, CPP_ATTR_RTX_CONTROL);
	if (object.isWindowBasedFlowControl())
	        NLA_PUT_FLAG(netlinkMessage, CPP_ATTR_WINDOW_BASED_FLOW_CONTROL);
	if (object.isRateBasedFlowControl())
	        NLA_PUT_FLAG(netlinkMessage, CPP_ATTR_RATE_BASED_FLOW_CONTROL);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building ConnectionPoliciesParameters Netlink object");
	return -1;
}

ConnectionPoliciesParameters *
parseConnectionPoliciesParametersObject(nlattr *nested) {
	struct nla_policy attr_policy[CCP_ATTR_MAX + 1];
	attr_policy[CPP_ATTR_DTCP_PRESENT].type = NLA_FLAG;
	attr_policy[CPP_ATTR_DTCP_PRESENT].minlen = 0;
	attr_policy[CPP_ATTR_DTCP_PRESENT].maxlen = 0;
	attr_policy[CPP_ATTR_FLOW_CONTROL].type = NLA_FLAG;
	attr_policy[CPP_ATTR_FLOW_CONTROL].minlen = 0;
	attr_policy[CPP_ATTR_FLOW_CONTROL].maxlen = 0;
	attr_policy[CPP_ATTR_RTX_CONTROL].type = NLA_FLAG;
	attr_policy[CPP_ATTR_RTX_CONTROL].minlen = 0;
	attr_policy[CPP_ATTR_RTX_CONTROL].maxlen = 0;
	attr_policy[CPP_ATTR_WINDOW_BASED_FLOW_CONTROL].type = NLA_FLAG;
	attr_policy[CPP_ATTR_WINDOW_BASED_FLOW_CONTROL].minlen = 0;
	attr_policy[CPP_ATTR_WINDOW_BASED_FLOW_CONTROL].maxlen = 0;
	attr_policy[CPP_ATTR_RATE_BASED_FLOW_CONTROL].type = NLA_FLAG;
	attr_policy[CPP_ATTR_RATE_BASED_FLOW_CONTROL].minlen = 0;
	attr_policy[CPP_ATTR_RATE_BASED_FLOW_CONTROL].maxlen = 0;
	struct nlattr *attrs[CCP_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, CCP_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing ConnectionPoliciesParameters information from Netlink message: %d",
				err);
		return NULL;
	}

	ConnectionPoliciesParameters * result =
			new ConnectionPoliciesParameters();
	if (attrs[CPP_ATTR_DTCP_PRESENT]) {
		result->setDTCPpresent(nla_get_flag(attrs[CPP_ATTR_DTCP_PRESENT]));
	}

	if (attrs[CPP_ATTR_FLOW_CONTROL]) {
		result->setFlowControl(nla_get_flag(attrs[CPP_ATTR_FLOW_CONTROL]));
	}

	if (attrs[CPP_ATTR_RTX_CONTROL]) {
		result->setRTXcontrol(nla_get_flag(attrs[CPP_ATTR_RTX_CONTROL]));
	}

	if (attrs[CPP_ATTR_WINDOW_BASED_FLOW_CONTROL]) {
		result->setWindowBasedFlowControl(nla_get_flag(attrs[CPP_ATTR_WINDOW_BASED_FLOW_CONTROL]));
	}

	if (attrs[CPP_ATTR_RATE_BASED_FLOW_CONTROL]) {
		result->setRateBasedFlowControl(nla_get_flag(attrs[CPP_ATTR_RATE_BASED_FLOW_CONTROL]));
	}

	return result;
}


int putAppAllocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestMessage& object) {
	struct nlattr *sourceAppName, *destinationAppName, *flowSpec ,
	              *difName;

	if (!(sourceAppName = nla_nest_start(netlinkMessage,
			AAFR_ATTR_SOURCE_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getSourceAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, sourceAppName);

	if (!(destinationAppName = nla_nest_start(netlinkMessage,
			AAFR_ATTR_DEST_APP_NAME))) {
		goto nla_put_failure;
	}

	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDestAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, destinationAppName);

	if (!(flowSpec = nla_nest_start(netlinkMessage, AAFR_ATTR_FLOW_SPEC))) {
		goto nla_put_failure;
	}

	if (putFlowSpecificationObject(netlinkMessage,
			object.getFlowSpecification()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, flowSpec);

	if (!(difName = nla_nest_start(netlinkMessage,
	                AAFR_ATTR_DIF_NAME))) {
	        goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
	                object.getDifName()) < 0) {
	        goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppAllocateFlowRequestMessage Netlink object");
	return -1;
}

int putAppAllocateFlowRequestResultMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestResultMessage& object) {

	struct nlattr *sourceAppName, *difName;

	if (!(sourceAppName = nla_nest_start(netlinkMessage,
			AAFRR_ATTR_SOURCE_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getSourceAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, sourceAppName);

	NLA_PUT_U32(netlinkMessage, AAFRR_ATTR_PORT_ID, object.getPortId());

	NLA_PUT_STRING(netlinkMessage, AAFRR_ATTR_ERROR_DESCRIPTION,
			object.getErrorDescription().c_str());

	if (object.getPortId() > 0) {
		if (!(difName = nla_nest_start(netlinkMessage, AAFRR_ATTR_DIF_NAME))) {
			goto nla_put_failure;
		}
		if (putApplicationProcessNamingInformationObject(netlinkMessage,
				object.getDifName()) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, difName);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppAllocateFlowRequestResponseMessage Netlink object");
	return -1;
}

int putAppAllocateFlowRequestArrivedMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowRequestArrivedMessage& object) {
	struct nlattr *sourceAppName, *destinationAppName, *flowSpec, *difName;

	if (!(sourceAppName = nla_nest_start(netlinkMessage,
			AAFRA_ATTR_SOURCE_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getSourceAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, sourceAppName);

	if (!(destinationAppName = nla_nest_start(netlinkMessage,
			AAFRA_ATTR_DEST_APP_NAME))) {
		goto nla_put_failure;
	}

	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDestAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, destinationAppName);

	if (!(flowSpec = nla_nest_start(netlinkMessage, AAFRA_ATTR_FLOW_SPEC))) {
		goto nla_put_failure;
	}

	if (putFlowSpecificationObject(netlinkMessage,
			object.getFlowSpecification()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, flowSpec);

	NLA_PUT_U32(netlinkMessage, AAFRA_ATTR_PORT_ID, object.getPortId());

	if (!(difName = nla_nest_start(netlinkMessage, AAFRA_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppAllocateFlowRequestArrivedMessage Netlink object");
	return -1;
}

int putAppAllocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const AppAllocateFlowResponseMessage& object) {
	NLA_PUT_U32(netlinkMessage, AAFRE_ATTR_RESULT,
			object.getResult());
	NLA_PUT_FLAG(netlinkMessage, AAFRE_ATTR_NOTIFY_SOURCE);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building ApplicationProcessNamingInformation Netlink object");
	return -1;
}

int putAppDeallocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const AppDeallocateFlowRequestMessage& object) {

	struct nlattr *applicationName;

	NLA_PUT_U32(netlinkMessage, ADFRT_ATTR_PORT_ID, object.getPortId());

	if (!(applicationName = nla_nest_start(netlinkMessage, ADFRT_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppDeallocateFlowRequestMessage Netlink object");
	return -1;
}

int putAppDeallocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const AppDeallocateFlowResponseMessage& object) {
	struct nlattr *applicationName;

	NLA_PUT_U32(netlinkMessage, ADFRE_ATTR_RESULT, object.getResult());

	if (!(applicationName = nla_nest_start(netlinkMessage, ADFRT_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	NLA_PUT_U32(netlinkMessage, ADFRE_ATTR_PORT_ID, object.getPortId());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppDeallocateFlowResponseMessage Netlink object");
	return -1;
}

int putAppFlowDeallocatedNotificationMessageObject(nl_msg* netlinkMessage,
		const AppFlowDeallocatedNotificationMessage& object) {
	struct nlattr *applicationName;

	NLA_PUT_U32(netlinkMessage, AFDN_ATTR_PORT_ID, object.getPortId());
	NLA_PUT_U32(netlinkMessage, AFDN_ATTR_CODE, object.getCode());

	if (!(applicationName = nla_nest_start(netlinkMessage, AFDN_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppFlowDeallocatedNotificationMessage Netlink object");
	return -1;
}

int putAppRegisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const AppRegisterApplicationRequestMessage& object) {
	struct nlattr *appRegInfo;

	if (!(appRegInfo = nla_nest_start(netlinkMessage, ARAR_ATTR_APP_REG_INFO))) {
		goto nla_put_failure;
	}
	if (putApplicationRegistrationInformationObject(netlinkMessage,
			object.getApplicationRegistrationInformation()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, appRegInfo);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppRegisterApplicationRequestMessage Netlink object");
	return -1;
}

int putAppRegisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const AppRegisterApplicationResponseMessage& object) {
	struct nlattr *difName, *applicationName;

	NLA_PUT_U32(netlinkMessage, ARARE_ATTR_RESULT, object.getResult());

	if (!(applicationName = nla_nest_start(netlinkMessage, ARARE_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, ARARE_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppRegisterApplicationResponseMessage Netlink object");
	return -1;
}

int putAppUnregisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const AppUnregisterApplicationRequestMessage& object) {
	struct nlattr *difName, *applicationName;

	if (!(applicationName = nla_nest_start(netlinkMessage, AUAR_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, AUAR_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppUnregisterApplicationRequestMessage Netlink object");
	return -1;
}


int putAppUnregisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const AppUnregisterApplicationResponseMessage& object) {
	struct nlattr *applicationName;

	NLA_PUT_U32(netlinkMessage, AUARE_ATTR_RESULT, object.getResult());

	if (!(applicationName = nla_nest_start(netlinkMessage, AUARE_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppFlowDeallocatedNotificationMessage Netlink object");
	return -1;
}


int putAppRegistrationCanceledNotificationMessageObject(nl_msg* netlinkMessage,
		const AppRegistrationCanceledNotificationMessage& object) {
	struct nlattr *difName, *applicationName;

	NLA_PUT_U32(netlinkMessage, ARCN_ATTR_CODE, object.getCode());
	NLA_PUT_STRING(netlinkMessage, ARCN_ATTR_REASON,
			object.getReason().c_str());

	if (!(applicationName = nla_nest_start(netlinkMessage, ARCN_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, ARCN_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppRegistrationCanceledNotificationMessage Netlink object");
	return -1;
}

int putAppGetDIFPropertiesRequestMessageObject(nl_msg* netlinkMessage,
		const AppGetDIFPropertiesRequestMessage& object){
	struct nlattr *difName, *applicationName;

	if (!(applicationName = nla_nest_start(netlinkMessage, AGDP_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, AGDP_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppGetDIFPropertiesRequestMessage Netlink object");
	return -1;
}

int putListOfDIFProperties(
		nl_msg* netlinkMessage, const std::list<DIFProperties>& difProperties){
	std::list<DIFProperties>::const_iterator iterator;
	struct nlattr *ribObject;
	int i = 0;

	for (iterator = difProperties.begin();
			iterator != difProperties.end();
			++iterator) {
		if (!(ribObject = nla_nest_start(netlinkMessage, i))){
			goto nla_put_failure;
		}
		if (putDIFPropertiesObject(netlinkMessage, *iterator) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, ribObject);
		i++;
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building DIFProperties Netlink object");
	return -1;
}


int putAppGetDIFPropertiesResponseMessageObject(nl_msg* netlinkMessage,
		const AppGetDIFPropertiesResponseMessage& object){
	struct nlattr *appName, *difProperties;

	NLA_PUT_U32(netlinkMessage, AGDPR_ATTR_RESULT, object.getResult());

	if (!(appName =
			nla_nest_start(netlinkMessage, AGDPR_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, appName);

	if (object.getDIFProperties().size() > 0){
		if (!(difProperties =
				nla_nest_start(netlinkMessage, AGDPR_ATTR_DIF_PROPERTIES))) {
			goto nla_put_failure;
		}
		if (putListOfDIFProperties(netlinkMessage,
				object.getDIFProperties()) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, difProperties);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building AppGetDIFPropertiesResponseMessage Netlink object");
	return -1;
}

int putIpcmRegisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmRegisterApplicationRequestMessage& object) {
	struct nlattr *difName, *applicationName;

	if (!(applicationName = nla_nest_start(netlinkMessage, IRAR_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, IRAR_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	if (object.getRegIpcProcessId() != 0) {
	        nla_put_u16(netlinkMessage, IRAR_ATTR_REG_IPC_ID,
	                        object.getRegIpcProcessId());
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmRegisterApplicationRequestMessage Netlink object");
	return -1;
}

int putIpcmRegisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmRegisterApplicationResponseMessage& object) {
	NLA_PUT_U32(netlinkMessage, IRARE_ATTR_RESULT, object.getResult());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmRegisterApplicationResponseMessage Netlink object");
	return -1;
}

int putIpcmUnregisterApplicationRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmUnregisterApplicationRequestMessage& object) {
	struct nlattr *difName, *applicationName;

	if (!(applicationName = nla_nest_start(netlinkMessage, IUAR_ATTR_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getApplicationName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, applicationName);

	if (!(difName = nla_nest_start(netlinkMessage, IUAR_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmUnregisterApplicationRequestMessage Netlink object");
	return -1;
}

int putIpcmUnregisterApplicationResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmUnregisterApplicationResponseMessage& object) {
	NLA_PUT_U32(netlinkMessage, IUARE_ATTR_RESULT, object.getResult());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmUnregisterApplicationResponseMessage Netlink object");
	return -1;
}

int putDataTransferConstantsObject(nl_msg* netlinkMessage,
                const DataTransferConstants& object) {
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_QOS_ID, object.getQosIdLenght());
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_PORT_ID,
                        object.getPortIdLength());
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_CEP_ID, object.getCepIdLength());
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_SEQ_NUM,
                        object.getSequenceNumberLength());
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_ADDRESS,
                        object.getAddressLength());
        NLA_PUT_U16(netlinkMessage, DTC_ATTR_LENGTH, object.getLengthLength());
        NLA_PUT_U32(netlinkMessage, DTC_ATTR_MAX_PDU_SIZE,
                        object.getMaxPduSize());
        NLA_PUT_U32(netlinkMessage, DTC_ATTR_MAX_PDU_LIFE,
                                object.getMaxPduLifetime());
        if (object.isDifIntegrity()){
                NLA_PUT_FLAG(netlinkMessage, DTC_ATTR_DIF_INTEGRITY);
        }

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building DataTransferConstants Netlink object");
        return -1;
}

int putDIFConfigurationObject(nl_msg* netlinkMessage,
		const DIFConfiguration& object){
	struct nlattr *parameters, *dataTransferConstants, *qosCubes;

	if  (object.getParameters().size() > 0) {
	        if (!(parameters = nla_nest_start(
	                        netlinkMessage, DCONF_ATTR_PARAMETERS))) {
	                goto nla_put_failure;
	        }
	        if (putListOfParameters(netlinkMessage,
	                        object.getParameters()) < 0) {
	                goto nla_put_failure;
	        }
	        nla_nest_end(netlinkMessage, parameters);
	}

	if (!(dataTransferConstants = nla_nest_start(
	                netlinkMessage, DCONF_ATTR_DATA_TRANS_CONST))) {
	        goto nla_put_failure;
	}
	if (putDataTransferConstantsObject(netlinkMessage,
	                object.getDataTransferConstants()) < 0) {
	        goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, dataTransferConstants);

	NLA_PUT_U32(netlinkMessage, DCONF_ATTR_ADDRESS,
	                object.getAddress());

	if (object.getQosCubes().size() > 0) {
	        if (!(qosCubes = nla_nest_start(
	                        netlinkMessage, DCONF_ATTR_QOS_CUBES))) {
	                goto nla_put_failure;
	        }

	        if (putListOfQoSCubeObjects(netlinkMessage,
	                        object.getQosCubes()) < 0) {
	                goto nla_put_failure;
	        }

	        nla_nest_end(netlinkMessage, qosCubes);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building DIFConfiguration Netlink object");
	return -1;
}

int putDIFInformationObject(nl_msg* netlinkMessage,
		const DIFInformation& object){
	struct nlattr *difName, *configuration;

	NLA_PUT_STRING(netlinkMessage, DINFO_ATTR_DIF_TYPE,
			object.getDifType().c_str());

	if (!(difName = nla_nest_start(netlinkMessage, DINFO_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	if (!(configuration = nla_nest_start(netlinkMessage, DINFO_ATTR_DIF_CONFIG))) {
		goto nla_put_failure;
	}
	if (putDIFConfigurationObject(netlinkMessage,
			object.getDifConfiguration()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, configuration);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building DIFInformation Netlink object");
	return -1;
}

int putIpcmAssignToDIFRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmAssignToDIFRequestMessage& object){
	struct nlattr *difInformation;

	if (!(difInformation =
			nla_nest_start(netlinkMessage, IATDR_ATTR_DIF_INFORMATION))) {
		goto nla_put_failure;
	}

	if (putDIFInformationObject(
			netlinkMessage, object.getDIFInformation()) < 0) {
		goto nla_put_failure;
	}

	nla_nest_end(netlinkMessage, difInformation);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAssignToDIFRequestMessage Netlink object");
	return -1;
}

int putIpcmAssignToDIFResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmAssignToDIFResponseMessage& object){

	NLA_PUT_U32(netlinkMessage, IATDRE_ATTR_RESULT, object.getResult());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAssignToDIFResponseMessage Netlink object");
	return -1;
}

int putIpcmUpdateDIFConfigurationRequestMessageObject(nl_msg* netlinkMessage,
                const IpcmUpdateDIFConfigurationRequestMessage& object){
        struct nlattr *difConfiguration;

        if (!(difConfiguration =
                        nla_nest_start(netlinkMessage, IUDCR_ATTR_DIF_CONFIGURATION))) {
                goto nla_put_failure;
        }

        if (putDIFConfigurationObject(
                        netlinkMessage, object.getDIFConfiguration()) < 0) {
                goto nla_put_failure;
        }

        nla_nest_end(netlinkMessage, difConfiguration);

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building IpcmUpdateDIFConfigurationRequestMessage Netlink object");
        return -1;
}

int putIpcmUpdateDIFConfigurationResponseMessageObject(nl_msg* netlinkMessage,
                const IpcmUpdateDIFConfigurationResponseMessage& object){

        NLA_PUT_U32(netlinkMessage, IUDCRE_ATTR_RESULT, object.getResult());

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building IpcmUpdateDIFConfigurationResponseMessage Netlink object");
        return -1;
}

int putIpcmEnrollToDIFRequestMessageObject(nl_msg* netlinkMessage,
                const IpcmEnrollToDIFRequestMessage& object) {
        struct nlattr *difName, *supDIFName, *neighbourName;

        if (!(difName = nla_nest_start(netlinkMessage, IETDR_ATTR_DIF_NAME))){
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getDifName()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, difName);

        if (!(supDIFName = nla_nest_start(
                        netlinkMessage, IETDR_ATTR_SUP_DIF_NAME))){
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getSupportingDifName()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, supDIFName);

        if (!(neighbourName = nla_nest_start(
                        netlinkMessage, IETDR_ATTR_NEIGH))){
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getNeighborName()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, neighbourName);

        return 0;

        nla_put_failure: LOG_ERR(
                "Error building IpcmEnrollToDIFRequestMessage Netlink object");
        return -1;
}

int putIpcmIPCProcessInitializedMessageObject(nl_msg* netlinkMessage,
                const IpcmIPCProcessInitializedMessage& object) {
        struct nlattr *name;

        if (!(name = nla_nest_start(netlinkMessage, IIPM_ATTR_NAME))){
                goto nla_put_failure;
        }
        if (putApplicationProcessNamingInformationObject(netlinkMessage,
                        object.getName()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, name);

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building IpcmIPCProcessInitializedMessage Netlink object");
        return -1;
}

int putIpcmEnrollToDIFResponseMessageObject(nl_msg* netlinkMessage,
                const IpcmEnrollToDIFResponseMessage& object) {
        struct nlattr *neighbors, *difInformation;

        NLA_PUT_U32(netlinkMessage, IETDRE_ATTR_RESULT, object.getResult());

        if (!(neighbors = nla_nest_start(
                        netlinkMessage, IETDRE_ATTR_NEIGHBORS))){
                goto nla_put_failure;
        }
        if (putListOfNeighbors(netlinkMessage, object.getNeighbors()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, neighbors);

        if (!(difInformation = nla_nest_start(
                        netlinkMessage, IETDRE_ATTR_DIF_INFO))){
                goto nla_put_failure;
        }
        if (putDIFInformationObject(netlinkMessage,
                        object.getDIFInformation()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, difInformation);

        return 0;

        nla_put_failure: LOG_ERR(
                "Error building IpcmEnrollToDIFResponseMessage Netlink object");
        return -1;
}

int putIpcmNotifyNeighborsModifiedMessageObject(nl_msg* netlinkMessage,
                const IpcmNotifyNeighborsModifiedMessage& object) {
        struct nlattr *neighbors;

        if (object.isAdded()) {
                NLA_PUT_FLAG(netlinkMessage, INNMM_ATTR_ADDED);
        }

        if (!(neighbors = nla_nest_start(
                        netlinkMessage, INNMM_ATTR_NEIGHBORS))){
                goto nla_put_failure;
        }
        if (putListOfNeighbors(netlinkMessage, object.getNeighbors()) < 0) {
                goto nla_put_failure;
        }
        nla_nest_end(netlinkMessage, neighbors);

        return 0;

        nla_put_failure: LOG_ERR(
                "Error building IpcmNotifyNeighborsModifiedMessage Netlink object");
        return -1;
}

int putIpcmAllocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestMessage& object){
	struct nlattr *sourceName, *destName, *flowSpec, *difName;

	if (!(sourceName = nla_nest_start(netlinkMessage, IAFRM_ATTR_SOURCE_APP_NAME))){
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getSourceAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, sourceName);

	if (!(destName = nla_nest_start(netlinkMessage, IAFRM_ATTR_DEST_APP_NAME))){
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDestAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, destName);

	if (!(flowSpec = nla_nest_start(netlinkMessage, IAFRM_ATTR_FLOW_SPEC))){
		goto nla_put_failure;
	}
	if (putFlowSpecificationObject(netlinkMessage, object.getFlowSpec()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, flowSpec);

	if (!(difName = nla_nest_start(netlinkMessage, IAFRM_ATTR_DIF_NAME))){
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAllocateFlowRequestMessage Netlink object");
	return -1;
}

int putIpcmAllocateFlowRequestResultMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestResultMessage& object){

	NLA_PUT_U32(netlinkMessage, IAFRRM_ATTR_RESULT, object.getResult());
	NLA_PUT_U32(netlinkMessage, IAFRRM_ATTR_PORT_ID, object.getPortId());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAllocateFlowRequestResultMessage Netlink object");
	return -1;
}

int putIpcmAllocateFlowRequestArrivedMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowRequestArrivedMessage& object) {
	struct nlattr *sourceAppName, *destinationAppName, *flowSpec, *difName;

	if (!(sourceAppName = nla_nest_start(netlinkMessage,
			IAFRA_ATTR_SOURCE_APP_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getSourceAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, sourceAppName);

	if (!(destinationAppName = nla_nest_start(netlinkMessage,
			IAFRA_ATTR_DEST_APP_NAME))) {
		goto nla_put_failure;
	}

	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDestAppName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, destinationAppName);

	if (!(flowSpec = nla_nest_start(netlinkMessage, IAFRA_ATTR_FLOW_SPEC))) {
		goto nla_put_failure;
	}

	if (putFlowSpecificationObject(netlinkMessage,
			object.getFlowSpecification()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, flowSpec);

	if (!(difName = nla_nest_start(netlinkMessage, IAFRA_ATTR_DIF_NAME))) {
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	NLA_PUT_U32(netlinkMessage, IAFRA_ATTR_PORT_ID, object.getPortId());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAllocateFlowRequestArrivedMessage Netlink object");
	return -1;
}

int putIpcmAllocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmAllocateFlowResponseMessage& object) {
	NLA_PUT_U32(netlinkMessage, IAFRE_ATTR_RESULT,
			object.getResult());
	NLA_PUT_FLAG(netlinkMessage, IAFRE_ATTR_NOTIFY_SOURCE);

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmAllocateFlowResponseMessageObject Netlink object");
	return -1;
}

int putIpcmDeallocateFlowRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmDeallocateFlowRequestMessage& object) {

	NLA_PUT_U32(netlinkMessage, IDFRT_ATTR_PORT_ID, object.getPortId());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmDeallocateFlowRequestMessage Netlink object");
	return -1;
}

int putIpcmDeallocateFlowResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmDeallocateFlowResponseMessage& object) {

	NLA_PUT_U32(netlinkMessage, IDFRE_ATTR_RESULT, object.getResult());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmDeallocateFlowResponseMessage Netlink object");
	return -1;
}

int putIpcmFlowDeallocatedNotificationMessageObject(nl_msg* netlinkMessage,
		const IpcmFlowDeallocatedNotificationMessage& object) {

	NLA_PUT_U32(netlinkMessage, IFDN_ATTR_PORT_ID, object.getPortId());
	NLA_PUT_U32(netlinkMessage, IFDN_ATTR_CODE, object.getCode());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmFlowDeallocatedNotificationMessage Netlink object");
	return -1;
}

int putIpcmDIFRegistrationNotificationObject(nl_msg* netlinkMessage,
		const IpcmDIFRegistrationNotification& object){
	struct nlattr *ipcProcessName, *difName;

	if (!(ipcProcessName = nla_nest_start(
			netlinkMessage, IDRN_ATTR_IPC_PROCESS_NAME))){
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getIpcProcessName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, ipcProcessName);

	if (!(difName = nla_nest_start(
			netlinkMessage, IDRN_ATTR_DIF_NAME))){
		goto nla_put_failure;
	}
	if (putApplicationProcessNamingInformationObject(netlinkMessage,
			object.getDifName()) < 0) {
		goto nla_put_failure;
	}
	nla_nest_end(netlinkMessage, difName);

	if (object.isRegistered()){
		NLA_PUT_FLAG(netlinkMessage, IDRN_ATTR_REGISTRATION);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmIPCProcessRegisteredToDIFNotification Netlink object");
	return -1;
}

int putIpcmDIFQueryRIBRequestMessageObject(nl_msg* netlinkMessage,
		const IpcmDIFQueryRIBRequestMessage& object){
	NLA_PUT_STRING(netlinkMessage, IDQR_ATTR_OBJECT_CLASS,
			object.getObjectClass().c_str());
	NLA_PUT_STRING(netlinkMessage, IDQR_ATTR_OBJECT_NAME,
			object.getObjectName().c_str());
	NLA_PUT_U64(netlinkMessage, IDQR_ATTR_OBJECT_INSTANCE,
			object.getObjectInstance());
	NLA_PUT_U32(netlinkMessage, IDQR_ATTR_SCOPE, object.getScope());
	NLA_PUT_STRING(netlinkMessage, IDQR_ATTR_FILTER,
			object.getFilter().c_str());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building IpcmDIFQueryRIBRequestMessage Netlink object");
	return -1;
}

int putRIBObject(nl_msg* netlinkMessage, const RIBObject& object){
        NLA_PUT_STRING(netlinkMessage, RIBO_ATTR_OBJECT_CLASS,
                        object.getClazz().c_str());
        NLA_PUT_STRING(netlinkMessage, RIBO_ATTR_OBJECT_NAME,
                        object.getName().c_str());
        NLA_PUT_U64(netlinkMessage, RIBO_ATTR_OBJECT_INSTANCE,
                        object.getInstance());

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building RIBObject Netlink message attribute: %s %s",
			object.getClazz().c_str(), object.getName().c_str());
	return -1;
}

int putListOfRIBObjects(
		nl_msg* netlinkMessage, const std::list<RIBObject>& ribObjects){
	std::list<RIBObject>::const_iterator iterator;
	struct nlattr *ribObject;
	int i = 0;

	for (iterator = ribObjects.begin();
			iterator != ribObjects.end();
			++iterator) {
		if (!(ribObject = nla_nest_start(netlinkMessage, i))){
			goto nla_put_failure;
		}
		if (putRIBObject(netlinkMessage, *iterator) < 0) {
		        goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, ribObject);
		i++;
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building list of RIBobjects Netlink message attribute");
	return -1;
}

int putIpcmDIFQueryRIBResponseMessageObject(nl_msg* netlinkMessage,
		const IpcmDIFQueryRIBResponseMessage& object){
	struct nlattr *ribObjects;

	NLA_PUT_U32(netlinkMessage, IDQRE_ATTR_RESULT, object.getResult());

	if (object.getRIBObjects().size()>0){
		if (!(ribObjects = nla_nest_start(
				netlinkMessage, IDQRE_ATTR_RIB_OBJECTS))){
			goto nla_put_failure;
		}
		if (putListOfRIBObjects(netlinkMessage, object.getRIBObjects()) < 0) {
			goto nla_put_failure;
		}
		nla_nest_end(netlinkMessage, ribObjects);
	}

	return 0;

	nla_put_failure: LOG_ERR(
			"Error building Query RIB Response Netlink message");
	return -1;
}

int putIpcpConnectionCreateRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateRequestMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICCRM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICCRM_ATTR_SRC_ADDRESS,
                        object.getSourceAddress());
        NLA_PUT_U32(netlinkMessage, ICCRM_ATTR_DEST_ADDRESS,
                                object.getDestAddress());
        NLA_PUT_U32(netlinkMessage, ICCRM_ATTR_QOS_ID, object.getQosId());
        if (putConnectionPoliciesParametersObject(netlinkMessage,
                        object.getConnPoliciesParams()) < 0) {
                goto nla_put_failure;
        }

        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionCreateRequestMessage Netlink object");
        return -1;
}

int putIpcpConnectionCreateResponseMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateResponseMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICCREM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICCREM_ATTR_SRC_CEP_ID, object.getCepId());
        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionCreateResponseMessage Netlink object");
        return -1;
}

int putIpcpConnectionUpdateRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionUpdateRequestMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICURM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICURM_ATTR_SRC_CEP_ID,
                        object.getSourceCepId());
        NLA_PUT_U32(netlinkMessage, ICURM_ATTR_DEST_CEP_ID,
                                object.getDestinationCepId());
        NLA_PUT_U16(netlinkMessage, ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID,
                        object.getFlowUserIpcProcessId());

        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionUpdateRequestMessage Netlink object");
        return -1;
}

int putIpcpConnectionUpdateResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionUpdateResultMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICUREM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICUREM_ATTR_RESULT, object.getResult());

        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionUpdateResultMessage Netlink object");
        return -1;
}

int putIpcpConnectionCreateArrivedMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateArrivedMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICCAM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICCAM_ATTR_SRC_ADDRESS,
                        object.getSourceAddress());
        NLA_PUT_U32(netlinkMessage, ICCAM_ATTR_DEST_ADDRESS,
                                object.getDestAddress());
        NLA_PUT_U32(netlinkMessage, ICCAM_ATTR_QOS_ID, object.getQosId());
        NLA_PUT_U32(netlinkMessage, ICCAM_ATTR_DEST_CEP_ID,
                        object.getDestCepId());
        NLA_PUT_U16(netlinkMessage, ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID,
                        object.getFlowUserIpcProcessId());

        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionCreateArrivedMessage Netlink object");
        return -1;
}

int putIpcpConnectionCreateResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionCreateResultMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICCRES_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICCRES_ATTR_SRC_CEP_ID,
                        object.getSourceCepId());
        NLA_PUT_U32(netlinkMessage, ICCRES_ATTR_DEST_CEP_ID,
                                object.getDestCepId());
        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionCreateResponseMessage Netlink object");
        return -1;
}

int putIpcpConnectionDestroyRequestMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionDestroyRequestMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICDRM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICDRM_ATTR_CEP_ID, object.getCepId());
        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionDestroyRequestMessage Netlink object");
        return -1;
}

int putIpcpConnectionDestroyResultMessageObject(nl_msg* netlinkMessage,
                const IpcpConnectionDestroyResultMessage& object) {
        NLA_PUT_U32(netlinkMessage, ICDREM_ATTR_PORT_ID, object.getPortId());
        NLA_PUT_U32(netlinkMessage, ICDREM_ATTR_RESULT, object.getResult());
        return 0;

        nla_put_failure: LOG_ERR(
            "Error building IpcpConnectionDestroyResultMessage Netlink object");
        return -1;
}

int putPDUForwardingTableEntryObject(nl_msg* netlinkMessage,
                const PDUForwardingTableEntry& object) {
        struct nlattr * portIds;
        std::list<unsigned int>::const_iterator iterator;
        const std::list<unsigned int> portIdsList = object.getPortIds();
        int i=0;

        NLA_PUT_U32(netlinkMessage, PFTE_ATTR_ADDRESS, object.getAddress());
        NLA_PUT_U32(netlinkMessage, PFTE_ATTR_QOS_ID, object.getQosId());

        if (!(portIds = nla_nest_start(netlinkMessage, PFTE_ATTR_PORT_IDS))) {
                goto nla_put_failure;
        }

        for (iterator = portIdsList.begin();
                        iterator != portIdsList.end();
                        ++iterator) {
                NLA_PUT_U32(netlinkMessage, i, *iterator);
                i++;
        }

        nla_nest_end(netlinkMessage, portIds);
        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building PDUForwardingTableEntry Netlink object");
        return -1;
}

int putListOfPFTEntries(nl_msg* netlinkMessage,
                const std::list<PDUForwardingTableEntry>& entries){
        std::list<PDUForwardingTableEntry>::const_iterator iterator;
        struct nlattr *entry;
        int i = 0;

        for (iterator = entries.begin();
                        iterator != entries.end();
                        ++iterator) {
                if (!(entry = nla_nest_start(netlinkMessage, i))){
                        goto nla_put_failure;
                }
                if (putPDUForwardingTableEntryObject(netlinkMessage,
                                *iterator) < 0) {
                        goto nla_put_failure;
                }
                nla_nest_end(netlinkMessage, entry);
                i++;
        }

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building putPDUForwardingTableEntryObject Netlink object");
        return -1;
}

int putRmtModifyPDUFTEntriesRequestObject(nl_msg* netlinkMessage,
                const RmtModifyPDUFTEntriesRequestMessage& object) {
        struct nlattr *entries;

        if (!(entries =
                        nla_nest_start(netlinkMessage, RMPFTE_ATTR_ENTRIES))) {
                goto nla_put_failure;
        }

        if (object.getEntries().size() > 0) {
                if (putListOfPFTEntries(netlinkMessage,
                                object.getEntries()) < 0) {
                        goto nla_put_failure;
                }
        }

        nla_nest_end(netlinkMessage, entries);

        NLA_PUT_U32(netlinkMessage, RMPFTE_ATTR_MODE, object.getMode());

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building RmtModifyPDUFTEntriesRequestMessage Netlink object");
        return -1;
}

int putRmtDumpPDUFTEntriesResponseObject(nl_msg* netlinkMessage,
                const RmtDumpPDUFTEntriesResponseMessage& object) {
        struct nlattr *entries;

        NLA_PUT_U32(netlinkMessage, RDPFTE_ATTR_RESULT, object.getResult());

        if (!(entries =
                        nla_nest_start(netlinkMessage, RDPFTE_ATTR_ENTRIES))) {
                goto nla_put_failure;
        }

        if (putListOfPFTEntries(netlinkMessage,
                        object.getEntries()) < 0) {
                goto nla_put_failure;
        }

        nla_nest_end(netlinkMessage, entries);

        return 0;

        nla_put_failure: LOG_ERR(
                        "Error building RmtDumpPDUFTEntriesResponseMessage Netlink object");
        return -1;
}

AppAllocateFlowRequestMessage * parseAppAllocateFlowRequestMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AAFR_ATTR_MAX + 1];
	attr_policy[AAFR_ATTR_SOURCE_APP_NAME].type = NLA_NESTED;
	attr_policy[AAFR_ATTR_SOURCE_APP_NAME].minlen = 0;
	attr_policy[AAFR_ATTR_SOURCE_APP_NAME].maxlen = 0;
	attr_policy[AAFR_ATTR_DEST_APP_NAME].type = NLA_NESTED;
	attr_policy[AAFR_ATTR_DEST_APP_NAME].minlen = 0;
	attr_policy[AAFR_ATTR_DEST_APP_NAME].maxlen = 0;
	attr_policy[AAFR_ATTR_FLOW_SPEC].type = NLA_NESTED;
	attr_policy[AAFR_ATTR_FLOW_SPEC].minlen = 0;
	attr_policy[AAFR_ATTR_FLOW_SPEC].maxlen = 0;
	attr_policy[AAFR_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[AAFR_ATTR_DIF_NAME].minlen = 0;
	attr_policy[AAFR_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[AAFR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AAFR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppAllocateFlowRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppAllocateFlowRequestMessage * result =
			new AppAllocateFlowRequestMessage();
	ApplicationProcessNamingInformation * sourceName;
	ApplicationProcessNamingInformation * destName;
	FlowSpecification * flowSpec;
	ApplicationProcessNamingInformation * difName;

	if (attrs[AAFR_ATTR_SOURCE_APP_NAME]) {
		sourceName = parseApplicationProcessNamingInformationObject(
				attrs[AAFR_ATTR_SOURCE_APP_NAME]);
		if (sourceName == 0) {
			delete result;
			return 0;
		} else {
			result->setSourceAppName(*sourceName);
			delete sourceName;
		}
	}

	if (attrs[AAFR_ATTR_DEST_APP_NAME]) {
		destName = parseApplicationProcessNamingInformationObject(
				attrs[AAFR_ATTR_DEST_APP_NAME]);
		if (destName == 0) {
			delete result;
			return 0;
		} else {
			result->setDestAppName(*destName);
			delete destName;
		}
	}

	if (attrs[AAFR_ATTR_FLOW_SPEC]) {
		flowSpec = parseFlowSpecificationObject(attrs[AAFR_ATTR_FLOW_SPEC]);
		if (flowSpec == 0) {
			delete result;
			return 0;
		} else {
			result->setFlowSpecification(*flowSpec);
			delete flowSpec;
		}
	}

	if (attrs[AAFR_ATTR_DIF_NAME]) {
	        difName = parseApplicationProcessNamingInformationObject(
	                        attrs[AAFR_ATTR_DIF_NAME]);
	        if (difName == 0) {
	                delete result;
	                return 0;
	        } else {
	                result->setDifName(*difName);
	                delete difName;
	        }
	}

	return result;
}

AppAllocateFlowRequestResultMessage * parseAppAllocateFlowRequestResultMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AAFRR_ATTR_MAX + 1];
	attr_policy[AAFRR_ATTR_SOURCE_APP_NAME].type = NLA_NESTED;
	attr_policy[AAFRR_ATTR_SOURCE_APP_NAME].minlen = 0;
	attr_policy[AAFRR_ATTR_SOURCE_APP_NAME].maxlen = 0;
	attr_policy[AAFRR_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[AAFRR_ATTR_PORT_ID].minlen = 4;
	attr_policy[AAFRR_ATTR_PORT_ID].maxlen = 4;
	attr_policy[AAFRR_ATTR_ERROR_DESCRIPTION].type = NLA_STRING;
	attr_policy[AAFRR_ATTR_ERROR_DESCRIPTION].minlen = 0;
	attr_policy[AAFRR_ATTR_ERROR_DESCRIPTION].maxlen = 65535;
	attr_policy[AAFRR_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[AAFRR_ATTR_DIF_NAME].minlen = 0;
	attr_policy[AAFRR_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[AAFRR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AAFRR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppAllocateFlowRequestResultMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppAllocateFlowRequestResultMessage * result =
			new AppAllocateFlowRequestResultMessage();

	ApplicationProcessNamingInformation * sourceName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[AAFRR_ATTR_SOURCE_APP_NAME]) {
		sourceName = parseApplicationProcessNamingInformationObject(
				attrs[AAFRR_ATTR_SOURCE_APP_NAME]);
		if (sourceName == 0) {
			delete result;
			return 0;
		} else {
			result->setSourceAppName(*sourceName);
			delete sourceName;
		}
	}

	if (attrs[AAFRR_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[AAFRR_ATTR_PORT_ID]));
	}

	if (attrs[AAFRR_ATTR_ERROR_DESCRIPTION]) {
		result->setErrorDescription(
				nla_get_string(attrs[AAFRR_ATTR_ERROR_DESCRIPTION]));
	}

	if (attrs[AAFRR_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[AAFRR_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

AppAllocateFlowRequestArrivedMessage * parseAppAllocateFlowRequestArrivedMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AAFRA_ATTR_MAX + 1];
	attr_policy[AAFRA_ATTR_SOURCE_APP_NAME].type = NLA_NESTED;
	attr_policy[AAFRA_ATTR_SOURCE_APP_NAME].minlen = 0;
	attr_policy[AAFRA_ATTR_SOURCE_APP_NAME].maxlen = 0;
	attr_policy[AAFRA_ATTR_DEST_APP_NAME].type = NLA_NESTED;
	attr_policy[AAFRA_ATTR_DEST_APP_NAME].minlen = 0;
	attr_policy[AAFRA_ATTR_DEST_APP_NAME].maxlen = 0;
	attr_policy[AAFRA_ATTR_FLOW_SPEC].type = NLA_NESTED;
	attr_policy[AAFRA_ATTR_FLOW_SPEC].minlen = 0;
	attr_policy[AAFRA_ATTR_FLOW_SPEC].maxlen = 0;
	attr_policy[AAFRA_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[AAFRA_ATTR_PORT_ID].minlen = 4;
	attr_policy[AAFRA_ATTR_PORT_ID].maxlen = 4;
	attr_policy[AAFRA_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[AAFRA_ATTR_DIF_NAME].minlen = 0;
	attr_policy[AAFRA_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[AAFRA_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AAFRA_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppAllocateFlowRequestArrivedMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppAllocateFlowRequestArrivedMessage * result =
			new AppAllocateFlowRequestArrivedMessage();
	ApplicationProcessNamingInformation * sourceName;
	ApplicationProcessNamingInformation * destName;
	FlowSpecification * flowSpec;
	ApplicationProcessNamingInformation * difName;

	if (attrs[AAFRA_ATTR_SOURCE_APP_NAME]) {
		sourceName = parseApplicationProcessNamingInformationObject(
				attrs[AAFRA_ATTR_SOURCE_APP_NAME]);
		if (sourceName == 0) {
			delete result;
			return 0;
		} else {
			result->setSourceAppName(*sourceName);
			delete sourceName;
		}
	}

	if (attrs[AAFRA_ATTR_DEST_APP_NAME]) {
		destName = parseApplicationProcessNamingInformationObject(
				attrs[AAFRA_ATTR_DEST_APP_NAME]);
		if (destName == 0) {
			delete result;
			return 0;
		} else {
			result->setDestAppName(*destName);
			delete destName;
		}
	}

	if (attrs[AAFRA_ATTR_FLOW_SPEC]) {
		flowSpec = parseFlowSpecificationObject(attrs[AAFRA_ATTR_FLOW_SPEC]);
		if (flowSpec == 0) {
			delete result;
			return 0;
		} else {
			result->setFlowSpecification(*flowSpec);
			delete flowSpec;
		}
	}

	if (attrs[AAFRA_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[AAFRA_ATTR_PORT_ID]));
	}

	if (attrs[AAFRA_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[AAFRA_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

AppAllocateFlowResponseMessage * parseAppAllocateFlowResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AAFRE_ATTR_MAX + 1];
	attr_policy[AAFRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[AAFRE_ATTR_RESULT].minlen = 0;
	attr_policy[AAFRE_ATTR_RESULT].maxlen = 0;
	attr_policy[AAFRE_ATTR_NOTIFY_SOURCE].type = NLA_FLAG;
	attr_policy[AAFRE_ATTR_NOTIFY_SOURCE].minlen = 0;
	attr_policy[AAFRE_ATTR_NOTIFY_SOURCE].maxlen = 0;
	struct nlattr *attrs[AAFRE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AAFRA_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppAllocateFlowResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppAllocateFlowResponseMessage * result =
			new AppAllocateFlowResponseMessage();

	if (attrs[AAFRE_ATTR_RESULT]) {
		result->setResult((nla_get_u32(attrs[AAFRE_ATTR_RESULT])));
	}

	if (attrs[AAFRE_ATTR_NOTIFY_SOURCE]) {
		result->setNotifySource(
				(nla_get_flag(attrs[AAFRE_ATTR_NOTIFY_SOURCE])));
	}

	return result;
}

AppDeallocateFlowRequestMessage * parseAppDeallocateFlowRequestMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[ADFRT_ATTR_MAX + 1];
	attr_policy[ADFRT_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[ADFRT_ATTR_PORT_ID].minlen = 4;
	attr_policy[ADFRT_ATTR_PORT_ID].maxlen = 4;
	attr_policy[ADFRT_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[ADFRT_ATTR_APP_NAME].minlen = 0;
	attr_policy[ADFRT_ATTR_APP_NAME].maxlen = 0;
	struct nlattr *attrs[ADFRT_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			ADFRT_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppDeallocateFlowRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppDeallocateFlowRequestMessage * result =
			new AppDeallocateFlowRequestMessage();

	ApplicationProcessNamingInformation * applicationName;

	if (attrs[ADFRT_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[ADFRT_ATTR_PORT_ID]));
	}

	if (attrs[ADFRT_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[ADFRT_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	return result;
}

AppDeallocateFlowResponseMessage * parseAppDeallocateFlowResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[ADFRE_ATTR_MAX + 1];
	attr_policy[ADFRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[ADFRE_ATTR_RESULT].minlen = 4;
	attr_policy[ADFRE_ATTR_RESULT].maxlen = 4;
	attr_policy[ADFRE_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[ADFRE_ATTR_APP_NAME].minlen = 0;
	attr_policy[ADFRE_ATTR_APP_NAME].maxlen = 0;
	attr_policy[ADFRE_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[ADFRE_ATTR_PORT_ID].minlen = 4;
	attr_policy[ADFRE_ATTR_PORT_ID].maxlen = 4;
	struct nlattr *attrs[ADFRE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			ADFRE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
			"Error parsing AppDeallocateFlowResponseMessage information from Netlink message: %d",
			err);
		return 0;
	}

	AppDeallocateFlowResponseMessage * result =
			new AppDeallocateFlowResponseMessage();

	ApplicationProcessNamingInformation * applicationName;

	if (attrs[ADFRE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[ADFRE_ATTR_RESULT]));
	}

	if (attrs[ADFRE_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[ADFRE_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	if (attrs[ADFRE_ATTR_PORT_ID]) {
	        result->setPortId(nla_get_u32(attrs[ADFRE_ATTR_PORT_ID]));
	}

	return result;
}

AppFlowDeallocatedNotificationMessage * parseAppFlowDeallocatedNotificationMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AFDN_ATTR_MAX + 1];
	attr_policy[AFDN_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[AFDN_ATTR_PORT_ID].minlen = 4;
	attr_policy[AFDN_ATTR_PORT_ID].maxlen = 4;
	attr_policy[AFDN_ATTR_CODE].type = NLA_U32;
	attr_policy[AFDN_ATTR_CODE].minlen = 4;
	attr_policy[AFDN_ATTR_CODE].maxlen = 4;
	attr_policy[AFDN_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[AFDN_ATTR_APP_NAME].minlen = 0;
	attr_policy[AFDN_ATTR_APP_NAME].maxlen = 0;
	struct nlattr *attrs[AFDN_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AFDN_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppFlowDeallocatedNotificationMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppFlowDeallocatedNotificationMessage * result =
			new AppFlowDeallocatedNotificationMessage();

	ApplicationProcessNamingInformation * applicationName;

	if (attrs[AFDN_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[AFDN_ATTR_PORT_ID]));
	}

	if (attrs[AFDN_ATTR_CODE]) {
		result->setCode(nla_get_u32(attrs[AFDN_ATTR_CODE]));
	}

	if (attrs[AFDN_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[AFDN_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	return result;
}

AppRegisterApplicationRequestMessage * parseAppRegisterApplicationRequestMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[ARAR_ATTR_MAX + 1];
	attr_policy[ARAR_ATTR_APP_REG_INFO].type = NLA_NESTED;
	attr_policy[ARAR_ATTR_APP_REG_INFO].minlen = 0;
	attr_policy[ARAR_ATTR_APP_REG_INFO].maxlen = 0;
	struct nlattr *attrs[ARAR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			ARAR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppRegisterApplicationRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppRegisterApplicationRequestMessage * result =
			new AppRegisterApplicationRequestMessage();

	ApplicationRegistrationInformation * appRegInfo;

	if (attrs[ARAR_ATTR_APP_REG_INFO]) {
		appRegInfo = parseApplicationRegistrationInformation(
				attrs[ARAR_ATTR_APP_REG_INFO]);
		if (appRegInfo == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationRegistrationInformation(*appRegInfo);
			delete appRegInfo;
		}
	}

	return result;
}

AppRegisterApplicationResponseMessage * parseAppRegisterApplicationResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[ARARE_ATTR_MAX + 1];
	attr_policy[ARARE_ATTR_RESULT].type = NLA_U32;
	attr_policy[ARARE_ATTR_RESULT].minlen = 4;
	attr_policy[ARARE_ATTR_RESULT].maxlen = 4;
	attr_policy[ARARE_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[ARARE_ATTR_APP_NAME].minlen = 0;
	attr_policy[ARARE_ATTR_APP_NAME].maxlen = 0;
	attr_policy[ARARE_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[ARARE_ATTR_DIF_NAME].minlen = 0;
	attr_policy[ARARE_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[ARARE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			ARARE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppRegisterApplicationResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppRegisterApplicationResponseMessage * result =
			new AppRegisterApplicationResponseMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[ARARE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[ARARE_ATTR_RESULT]));
	}

	if (attrs[ARARE_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[ARARE_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	if (attrs[ARARE_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[ARARE_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

AppUnregisterApplicationRequestMessage * parseAppUnregisterApplicationRequestMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AUAR_ATTR_MAX + 1];
	attr_policy[AUAR_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[AUAR_ATTR_APP_NAME].minlen = 0;
	attr_policy[AUAR_ATTR_APP_NAME].maxlen = 0;
	attr_policy[AUAR_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[AUAR_ATTR_DIF_NAME].minlen = 0;
	attr_policy[AUAR_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[AUAR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AUAR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppUnregisterApplicationRequestMessage information from Netlink message: %d",
				err);
		return NULL;
	}

	AppUnregisterApplicationRequestMessage * result =
			new AppUnregisterApplicationRequestMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[AUAR_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[AUAR_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}
	if (attrs[AUAR_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[AUAR_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

AppUnregisterApplicationResponseMessage * parseAppUnregisterApplicationResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[AUARE_ATTR_MAX + 1];
	attr_policy[AUARE_ATTR_RESULT].type = NLA_U32;
	attr_policy[AUARE_ATTR_RESULT].minlen = 4;
	attr_policy[AUARE_ATTR_RESULT].maxlen = 4;
	attr_policy[AUARE_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[AUARE_ATTR_APP_NAME].minlen = 0;
	attr_policy[AUARE_ATTR_APP_NAME].maxlen = 0;
	struct nlattr *attrs[AUARE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AUARE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppUnregisterApplicationRequestMessage information from Netlink message: %d",
				err);
		return NULL;
	}

	AppUnregisterApplicationResponseMessage * result =
			new AppUnregisterApplicationResponseMessage();
	ApplicationProcessNamingInformation * applicationName;


	if (attrs[AUARE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[AUARE_ATTR_RESULT]));
	}

	if (attrs[AUARE_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[AUARE_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	return result;
}

AppRegistrationCanceledNotificationMessage * parseAppRegistrationCanceledNotificationMessage(
		nlmsghdr *hdr) {
	//TODO
	struct nla_policy attr_policy[ARCN_ATTR_MAX + 1];
	attr_policy[ARCN_ATTR_CODE].type = NLA_U32;
	attr_policy[ARCN_ATTR_CODE].minlen = 4;
	attr_policy[ARCN_ATTR_CODE].maxlen = 4;
	attr_policy[ARCN_ATTR_REASON].type = NLA_STRING;
	attr_policy[ARCN_ATTR_REASON].minlen = 0;
	attr_policy[ARCN_ATTR_REASON].maxlen = 65535;
	attr_policy[ARCN_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[ARCN_ATTR_APP_NAME].minlen = 0;
	attr_policy[ARCN_ATTR_APP_NAME].maxlen = 0;
	attr_policy[ARCN_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[ARCN_ATTR_DIF_NAME].minlen = 0;
	attr_policy[ARCN_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[ARCN_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			ARCN_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppRegistrationCanceledNotificationMessage information from Netlink message: %d",
				err);
		return 0;
	}

	AppRegistrationCanceledNotificationMessage * result =
			new AppRegistrationCanceledNotificationMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[ARCN_ATTR_CODE]) {
		result->setCode(nla_get_u32(attrs[ARCN_ATTR_CODE]));
	}

	if (attrs[ARCN_ATTR_REASON]) {
		result->setReason(nla_get_string(attrs[ARCN_ATTR_REASON]));
	}

	if (attrs[ARCN_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[ARCN_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}
	if (attrs[ARCN_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[ARCN_ATTR_DIF_NAME]);
		if (difName == NULL) {
			delete result;
			return NULL;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

AppGetDIFPropertiesRequestMessage * parseAppGetDIFPropertiesRequestMessage(
		nlmsghdr *hdr){
	struct nla_policy attr_policy[AGDP_ATTR_MAX + 1];
	attr_policy[AGDP_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[AGDP_ATTR_APP_NAME].minlen = 0;
	attr_policy[AGDP_ATTR_APP_NAME].maxlen = 0;
	attr_policy[AGDP_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[AGDP_ATTR_DIF_NAME].minlen = 0;
	attr_policy[AGDP_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[AGDP_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AGDP_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppGetDIFPropertiesRequestMessage information from Netlink message: %d",
				err);
		return NULL;
	}

	AppGetDIFPropertiesRequestMessage * result =
			new AppGetDIFPropertiesRequestMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[AGDP_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[AGDP_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}
	if (attrs[AGDP_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[AGDP_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

int parseListOfDIFProperties(nlattr *nested,
		AppGetDIFPropertiesResponseMessage * message){
	nlattr * nla;
	int rem;
	DIFProperties * difProperties;

	for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
		     nla_ok(nla, rem);
		     nla = nla_next(nla, &(rem))){
		/* validate & parse attribute */
		difProperties = parseDIFPropertiesObject(nla);
		if (difProperties == 0){
			return -1;
		}
		message->addDIFProperty(*difProperties);
		delete difProperties;
	}

	if (rem > 0){
		LOG_WARN("Missing bits to parse");
	}

	return 0;
}

AppGetDIFPropertiesResponseMessage * parseAppGetDIFPropertiesResponseMessage(
		nlmsghdr *hdr){
	struct nla_policy attr_policy[AGDPR_ATTR_MAX + 1];
	attr_policy[AGDPR_ATTR_RESULT].type = NLA_U32;
	attr_policy[AGDPR_ATTR_RESULT].minlen = 4;
	attr_policy[AGDPR_ATTR_RESULT].maxlen = 4;
	attr_policy[AGDPR_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[AGDPR_ATTR_APP_NAME].minlen = 0;
	attr_policy[AGDPR_ATTR_APP_NAME].maxlen = 0;
	attr_policy[AGDPR_ATTR_DIF_PROPERTIES].type = NLA_NESTED;
	attr_policy[AGDPR_ATTR_DIF_PROPERTIES].minlen = 0;
	attr_policy[AGDPR_ATTR_DIF_PROPERTIES].maxlen = 0;
	struct nlattr *attrs[AGDPR_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			AGDPR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing AppGetDIFPropertiesResponse information from Netlink message: %d",
				err);
		return 0;
	}

	AppGetDIFPropertiesResponseMessage * result =
					new AppGetDIFPropertiesResponseMessage();

	if (attrs[AGDPR_ATTR_RESULT]){
		result->setResult(nla_get_u32(attrs[AGDPR_ATTR_RESULT]));
	}

	ApplicationProcessNamingInformation * applicationName;

	if (attrs[AGDPR_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[AGDPR_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	int status = 0;
	if (attrs[AGDPR_ATTR_DIF_PROPERTIES]) {
		status = parseListOfDIFProperties(
				attrs[AGDPR_ATTR_DIF_PROPERTIES], result);
		if (status != 0){
			delete result;
			return 0;
		}
	}

	return result;
}

IpcmRegisterApplicationRequestMessage *
parseIpcmRegisterApplicationRequestMessage(nlmsghdr *hdr) {
	struct nla_policy attr_policy[IRAR_ATTR_MAX + 1];
	attr_policy[IRAR_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[IRAR_ATTR_APP_NAME].minlen = 0;
	attr_policy[IRAR_ATTR_APP_NAME].maxlen = 0;
	attr_policy[IRAR_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[IRAR_ATTR_DIF_NAME].minlen = 0;
	attr_policy[IRAR_ATTR_DIF_NAME].maxlen = 0;
	attr_policy[IRAR_ATTR_REG_IPC_ID].type = NLA_U16;
	attr_policy[IRAR_ATTR_REG_IPC_ID].minlen = 2;
	attr_policy[IRAR_ATTR_REG_IPC_ID].maxlen = 2;
	struct nlattr *attrs[IRAR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IRAR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR("Error parsing IpcmRegisterApplicationRequestMessage information from Netlink message: %d",
		         err);
		return 0;
	}

	IpcmRegisterApplicationRequestMessage * result =
			new IpcmRegisterApplicationRequestMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[IRAR_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[IRAR_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	if (attrs[IRAR_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[IRAR_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	if (attrs[IRAR_ATTR_REG_IPC_ID]) {
	      result->setRegIpcProcessId(
	                      nla_get_u16(attrs[IRAR_ATTR_REG_IPC_ID]));
	}

	return result;
}

IpcmRegisterApplicationResponseMessage *
parseIpcmRegisterApplicationResponseMessage(nlmsghdr *hdr) {
	struct nla_policy attr_policy[IRARE_ATTR_MAX + 1];
	attr_policy[IRARE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IRARE_ATTR_RESULT].minlen = 4;
	attr_policy[IRARE_ATTR_RESULT].maxlen = 4;
	struct nlattr *attrs[IRARE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IRARE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmRegisterApplicationResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmRegisterApplicationResponseMessage * result =
			new IpcmRegisterApplicationResponseMessage();

	if (attrs[IRARE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[IRARE_ATTR_RESULT]));
	}

	return result;
}

IpcmUnregisterApplicationRequestMessage *
parseIpcmUnregisterApplicationRequestMessage(nlmsghdr *hdr) {
	struct nla_policy attr_policy[IUAR_ATTR_MAX + 1];
	attr_policy[IUAR_ATTR_APP_NAME].type = NLA_NESTED;
	attr_policy[IUAR_ATTR_APP_NAME].minlen = 0;
	attr_policy[IUAR_ATTR_APP_NAME].maxlen = 0;
	attr_policy[IUAR_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[IUAR_ATTR_DIF_NAME].minlen = 0;
	attr_policy[IUAR_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[IUAR_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IUAR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmRegisterApplicationRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmUnregisterApplicationRequestMessage * result =
			new IpcmUnregisterApplicationRequestMessage();

	ApplicationProcessNamingInformation * applicationName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[IUAR_ATTR_APP_NAME]) {
		applicationName = parseApplicationProcessNamingInformationObject(
				attrs[IUAR_ATTR_APP_NAME]);
		if (applicationName == 0) {
			delete result;
			return 0;
		} else {
			result->setApplicationName(*applicationName);
			delete applicationName;
		}
	}

	if (attrs[IUAR_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[IUAR_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

IpcmUnregisterApplicationResponseMessage *
parseIpcmUnregisterApplicationResponseMessage(nlmsghdr *hdr) {
	struct nla_policy attr_policy[IUARE_ATTR_MAX + 1];
	attr_policy[IUARE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IUARE_ATTR_RESULT].minlen = 4;
	attr_policy[IUARE_ATTR_RESULT].maxlen = 4;
	struct nlattr *attrs[IUARE_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IUARE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmUnregisterApplicationResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmUnregisterApplicationResponseMessage * result =
			new IpcmUnregisterApplicationResponseMessage();

	if (attrs[IUARE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[IUARE_ATTR_RESULT]));
	}

	return result;
}

DataTransferConstants * parseDataTransferConstantsObject(nlattr *nested) {
        struct nla_policy attr_policy[DTC_ATTR_MAX + 1];
        attr_policy[DTC_ATTR_QOS_ID].type = NLA_U16;
        attr_policy[DTC_ATTR_QOS_ID].minlen = 2;
        attr_policy[DTC_ATTR_QOS_ID].maxlen = 2;
        attr_policy[DTC_ATTR_PORT_ID].type = NLA_U16;
        attr_policy[DTC_ATTR_PORT_ID].minlen = 2;
        attr_policy[DTC_ATTR_PORT_ID].maxlen = 2;
        attr_policy[DTC_ATTR_CEP_ID].type = NLA_U16;
        attr_policy[DTC_ATTR_CEP_ID].minlen = 2;
        attr_policy[DTC_ATTR_CEP_ID].maxlen = 2;
        attr_policy[DTC_ATTR_SEQ_NUM].type = NLA_U16;
        attr_policy[DTC_ATTR_SEQ_NUM].minlen = 2;
        attr_policy[DTC_ATTR_SEQ_NUM].maxlen = 2;
        attr_policy[DTC_ATTR_ADDRESS].type = NLA_U16;
        attr_policy[DTC_ATTR_ADDRESS].minlen = 2;
        attr_policy[DTC_ATTR_ADDRESS].maxlen = 2;
        attr_policy[DTC_ATTR_LENGTH].type = NLA_U16;
        attr_policy[DTC_ATTR_LENGTH].minlen = 2;
        attr_policy[DTC_ATTR_LENGTH].maxlen = 2;
        attr_policy[DTC_ATTR_MAX_PDU_SIZE].type = NLA_U32;
        attr_policy[DTC_ATTR_MAX_PDU_SIZE].minlen = 4;
        attr_policy[DTC_ATTR_MAX_PDU_SIZE].maxlen = 4;
        attr_policy[DTC_ATTR_MAX_PDU_LIFE].type = NLA_U32;
        attr_policy[DTC_ATTR_MAX_PDU_LIFE].minlen = 4;
        attr_policy[DTC_ATTR_MAX_PDU_LIFE].maxlen = 4;
        attr_policy[DTC_ATTR_DIF_INTEGRITY].type = NLA_FLAG;
        attr_policy[DTC_ATTR_DIF_INTEGRITY].minlen = 0;
        attr_policy[DTC_ATTR_DIF_INTEGRITY].maxlen = 0;
        struct nlattr *attrs[DTC_ATTR_MAX + 1];

        int err = nla_parse_nested(attrs, DTC_ATTR_MAX, nested, attr_policy);

        if (err < 0) {
                LOG_ERR(
                        "Error parsing DataTransferConstants information from Netlink message: %d",
                         err);
                return 0;
        }

        DataTransferConstants * result = new DataTransferConstants();

        if (attrs[DTC_ATTR_QOS_ID]) {
                result->setQosIdLenght(nla_get_u16(attrs[DTC_ATTR_QOS_ID]));
        }

        if (attrs[DTC_ATTR_PORT_ID]) {
                result->setPortIdLength(nla_get_u16(attrs[DTC_ATTR_PORT_ID]));
        }

        if (attrs[DTC_ATTR_CEP_ID]) {
                result->setCepIdLength(nla_get_u16(attrs[DTC_ATTR_CEP_ID]));
        }

        if (attrs[DTC_ATTR_SEQ_NUM]) {
                result->setSequenceNumberLength(
                                nla_get_u16(attrs[DTC_ATTR_SEQ_NUM]));
        }

        if (attrs[DTC_ATTR_ADDRESS]) {
                result->setAddressLength(nla_get_u16(attrs[DTC_ATTR_ADDRESS]));
        }

        if (attrs[DTC_ATTR_LENGTH]) {
                result->setLengthLength(nla_get_u16(attrs[DTC_ATTR_LENGTH]));
        }

        if (attrs[DTC_ATTR_MAX_PDU_SIZE]) {
                result->setMaxPduSize(
                                nla_get_u32(attrs[DTC_ATTR_MAX_PDU_SIZE]));
        }

        if (attrs[DTC_ATTR_MAX_PDU_LIFE]) {
                result->setMaxPduLifetime(
                                nla_get_u32(attrs[DTC_ATTR_MAX_PDU_LIFE]));
        }

        if (attrs[DTC_ATTR_DIF_INTEGRITY]) {
                result->setDifIntegrity(true);
        }

        return result;
}

DIFConfiguration * parseDIFConfigurationObject(nlattr *nested){
	struct nla_policy attr_policy[DCONF_ATTR_MAX + 1];
	attr_policy[DCONF_ATTR_PARAMETERS].type = NLA_NESTED;
	attr_policy[DCONF_ATTR_PARAMETERS].minlen = 0;
	attr_policy[DCONF_ATTR_PARAMETERS].maxlen = 0;
	attr_policy[DCONF_ATTR_DATA_TRANS_CONST].type = NLA_NESTED;
	attr_policy[DCONF_ATTR_DATA_TRANS_CONST].minlen = 0;
	attr_policy[DCONF_ATTR_DATA_TRANS_CONST].maxlen = 0;
	attr_policy[DCONF_ATTR_ADDRESS].type = NLA_U32;
	attr_policy[DCONF_ATTR_ADDRESS].minlen = 4;
	attr_policy[DCONF_ATTR_ADDRESS].maxlen = 4;
	attr_policy[DCONF_ATTR_QOS_CUBES].type = NLA_NESTED;
	attr_policy[DCONF_ATTR_QOS_CUBES].minlen = 0;
	attr_policy[DCONF_ATTR_QOS_CUBES].maxlen = 0;
	struct nlattr *attrs[DCONF_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, DCONF_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
			"Error parsing DIFConfiguration information from Netlink message: %d",
			err);
		return 0;
	}

	DIFConfiguration * result = new DIFConfiguration();
	DataTransferConstants * dataTransferConstants;

	int status = 0;
	if (attrs[DCONF_ATTR_PARAMETERS]) {
		status = parseListOfDIFConfigurationParameters(
				attrs[DCONF_ATTR_PARAMETERS], result);
		if (status != 0){
			delete result;
			return 0;
		}
	}

        if (attrs[DCONF_ATTR_DATA_TRANS_CONST]) {
                dataTransferConstants = parseDataTransferConstantsObject(
                                attrs[DCONF_ATTR_DATA_TRANS_CONST]);
                if (dataTransferConstants == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setDataTransferConstants(
                                        *dataTransferConstants);
                        delete dataTransferConstants;
                }
        }

        if (attrs[DCONF_ATTR_ADDRESS]) {
                result->setAddress(nla_get_u32(attrs[DCONF_ATTR_ADDRESS]));
        }

        if (attrs[DCONF_ATTR_QOS_CUBES]) {
                status = parseListOfQoSCubesForDIFConfiguration(
                                attrs[DCONF_ATTR_QOS_CUBES], result);
                if (status != 0){
                        delete result;
                        return 0;
                }
        }

	return result;
}

DIFInformation * parseDIFInformationObject(nlattr *nested){
	struct nla_policy attr_policy[DINFO_ATTR_MAX + 1];
	attr_policy[DINFO_ATTR_DIF_TYPE].type = NLA_STRING;
	attr_policy[DINFO_ATTR_DIF_TYPE].minlen = 0;
	attr_policy[DINFO_ATTR_DIF_TYPE].maxlen = 65535;
	attr_policy[DINFO_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[DINFO_ATTR_DIF_NAME].minlen = 0;
	attr_policy[DINFO_ATTR_DIF_NAME].maxlen = 0;
	attr_policy[DINFO_ATTR_DIF_CONFIG].type = NLA_NESTED;
	attr_policy[DINFO_ATTR_DIF_CONFIG].minlen = 0;
	attr_policy[DINFO_ATTR_DIF_CONFIG].maxlen = 0;
	struct nlattr *attrs[DINFO_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, DINFO_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing DIFInformation information from Netlink message: %d",
				err);
		return 0;
	}

	DIFInformation * result = new DIFInformation();
	ApplicationProcessNamingInformation * difName;
	DIFConfiguration * difConfiguration;

	if (attrs[DINFO_ATTR_DIF_TYPE]) {
		result->setDifType(
				nla_get_string(attrs[DINFO_ATTR_DIF_TYPE]));
	}

	if (attrs[DINFO_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[DINFO_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	if (attrs[DINFO_ATTR_DIF_CONFIG]) {
		difConfiguration = parseDIFConfigurationObject(
				attrs[DINFO_ATTR_DIF_CONFIG]);
		if (difConfiguration == 0) {
			delete result;
			return 0;
		} else {
			result->setDifConfiguration(*difConfiguration);
			delete difConfiguration;
		}
	}

	return result;
}

IpcmAssignToDIFRequestMessage *
parseIpcmAssignToDIFRequestMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IATDR_ATTR_MAX + 1];
	attr_policy[IATDR_ATTR_DIF_INFORMATION].type = NLA_NESTED;
	attr_policy[IATDR_ATTR_DIF_INFORMATION].minlen = 0;
	attr_policy[IATDR_ATTR_DIF_INFORMATION].maxlen = 0;
	struct nlattr *attrs[IRARE_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IATDR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmAssignToDIFRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmAssignToDIFRequestMessage * result =
			new IpcmAssignToDIFRequestMessage();
	DIFInformation * difInformation;

	if (attrs[IATDR_ATTR_DIF_INFORMATION]) {
		difInformation = parseDIFInformationObject(
				attrs[IATDR_ATTR_DIF_INFORMATION]);
		if (difInformation == 0) {
			delete result;
			return 0;
		} else {
			result->setDIFInformation(*difInformation);
			delete difInformation;
		}
	}

	return result;
}

IpcmAssignToDIFResponseMessage *
parseIpcmAssignToDIFResponseMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IATDRE_ATTR_MAX + 1];
	attr_policy[IATDRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IATDRE_ATTR_RESULT].minlen = 4;
	attr_policy[IATDRE_ATTR_RESULT].maxlen = 4;
	struct nlattr *attrs[IATDRE_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IATDRE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmAssignToDIFResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmAssignToDIFResponseMessage * result =
			new IpcmAssignToDIFResponseMessage();

	if (attrs[IATDRE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[IATDRE_ATTR_RESULT]));
	}

	return result;
}

IpcmUpdateDIFConfigurationRequestMessage *
parseIpcmUpdateDIFConfigurationRequestMessage(nlmsghdr *hdr){
        struct nla_policy attr_policy[IUDCR_ATTR_MAX + 1];
        attr_policy[IUDCR_ATTR_DIF_CONFIGURATION].type = NLA_NESTED;
        attr_policy[IUDCR_ATTR_DIF_CONFIGURATION].minlen = 0;
        attr_policy[IUDCR_ATTR_DIF_CONFIGURATION].maxlen = 0;
        struct nlattr *attrs[IUDCR_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        IUDCR_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                        "Error parsing IpcmUpdateDIFConfigurationRequestMessage information from Netlink message: %d",
                        err);
                return 0;
        }

        IpcmUpdateDIFConfigurationRequestMessage * result =
                        new IpcmUpdateDIFConfigurationRequestMessage();
        DIFConfiguration * difConfiguration;

        if (attrs[IUDCR_ATTR_DIF_CONFIGURATION]) {
                difConfiguration = parseDIFConfigurationObject(
                                attrs[IUDCR_ATTR_DIF_CONFIGURATION]);
                if (difConfiguration == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setDIFConfiguration(*difConfiguration);
                        delete difConfiguration;
                }
        }

        return result;
}

IpcmUpdateDIFConfigurationResponseMessage *
parseIpcmUpdateDIFConfigurationResponseMessage(nlmsghdr *hdr){
        struct nla_policy attr_policy[IUDCRE_ATTR_MAX + 1];
        attr_policy[IUDCRE_ATTR_RESULT].type = NLA_U32;
        attr_policy[IUDCRE_ATTR_RESULT].minlen = 4;
        attr_policy[IUDCRE_ATTR_RESULT].maxlen = 4;
        struct nlattr *attrs[IUDCRE_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        IUDCRE_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                        "Error parsing IpcmAssignToDIFResponseMessage information from Netlink message: %d",
                        err);
                return 0;
        }

        IpcmUpdateDIFConfigurationResponseMessage * result =
                        new IpcmUpdateDIFConfigurationResponseMessage();

        if (attrs[IUDCRE_ATTR_RESULT]) {
                result->setResult(nla_get_u32(attrs[IUDCRE_ATTR_RESULT]));
        }

        return result;
}

IpcmEnrollToDIFResponseMessage *
parseIpcmEnrollToDIFResponseMessage(nlmsghdr *hdr){
        struct nla_policy attr_policy[IETDRE_ATTR_MAX + 1];
        attr_policy[IETDRE_ATTR_RESULT].type = NLA_U32;
        attr_policy[IETDRE_ATTR_RESULT].minlen = 4;
        attr_policy[IETDRE_ATTR_RESULT].maxlen = 4;
        attr_policy[IETDRE_ATTR_NEIGHBORS].type = NLA_NESTED;
        attr_policy[IETDRE_ATTR_NEIGHBORS].minlen = 0;
        attr_policy[IETDRE_ATTR_NEIGHBORS].maxlen = 0;
        attr_policy[IETDRE_ATTR_DIF_INFO].type = NLA_NESTED;
        attr_policy[IETDRE_ATTR_DIF_INFO].minlen = 0;
        attr_policy[IETDRE_ATTR_DIF_INFO].maxlen = 0;
        struct nlattr *attrs[IETDRE_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        IETDRE_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                        "Error parsing IpcmEnrollToDIFResponseMessage information from Netlink message: %d",
                        err);
                return 0;
        }

        IpcmEnrollToDIFResponseMessage * result =
                        new IpcmEnrollToDIFResponseMessage();

        DIFInformation * difInformation;

        if (attrs[IETDRE_ATTR_RESULT]) {
                result->setResult(nla_get_u32(attrs[IETDRE_ATTR_RESULT]));
        }

        int status = 0;
        if (attrs[IETDRE_ATTR_NEIGHBORS]) {
                status = parseListOfEnrollToDIFResponseNeighbors(
                                attrs[IETDRE_ATTR_NEIGHBORS], result);
                if (status != 0){
                        delete result;
                        return 0;
                }
        }

        if (attrs[IETDRE_ATTR_DIF_INFO]) {
                difInformation = parseDIFInformationObject(
                                attrs[IETDRE_ATTR_DIF_INFO]);
                if (difInformation == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setDIFInformation(*difInformation);
                        delete difInformation;
                }
        }

        return result;
}

IpcmEnrollToDIFRequestMessage *
parseIpcmEnrollToDIFRequestMessage(nlmsghdr *hdr) {
        struct nla_policy attr_policy[IETDR_ATTR_MAX + 1];
        attr_policy[IETDR_ATTR_DIF_NAME].type = NLA_NESTED;
        attr_policy[IETDR_ATTR_DIF_NAME].minlen = 0;
        attr_policy[IETDR_ATTR_DIF_NAME].maxlen = 0;
        attr_policy[IETDR_ATTR_SUP_DIF_NAME].type = NLA_NESTED;
        attr_policy[IETDR_ATTR_SUP_DIF_NAME].minlen = 0;
        attr_policy[IETDR_ATTR_SUP_DIF_NAME].maxlen = 0;
        attr_policy[IETDR_ATTR_NEIGH].type = NLA_NESTED;
        attr_policy[IETDR_ATTR_NEIGH].minlen = 0;
        attr_policy[IETDR_ATTR_NEIGH].maxlen = 0;
        struct nlattr *attrs[IETDR_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        IETDR_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                        "Error parsing IpcmEnrollToDIFRequestMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcmEnrollToDIFRequestMessage * result =
                        new IpcmEnrollToDIFRequestMessage();
        ApplicationProcessNamingInformation * difName;
        ApplicationProcessNamingInformation * supDifName;
        ApplicationProcessNamingInformation * neighbour;

        if (attrs[IETDR_ATTR_DIF_NAME]) {
                difName = parseApplicationProcessNamingInformationObject(
                                attrs[IETDR_ATTR_DIF_NAME]);
                if (difName == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setDifName(*difName);
                        delete difName;
                }
        }

        if (attrs[IETDR_ATTR_SUP_DIF_NAME]) {
                supDifName = parseApplicationProcessNamingInformationObject(
                                attrs[IETDR_ATTR_SUP_DIF_NAME]);
                if (supDifName == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setSupportingDifName(*supDifName);
                        delete supDifName;
                }
        }

        if (attrs[IETDR_ATTR_NEIGH]) {
                neighbour = parseApplicationProcessNamingInformationObject(
                                attrs[IETDR_ATTR_NEIGH]);
                if (neighbour == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setNeighborName(*neighbour);
                        delete neighbour;
                }
        }

        return result;
}

IpcmNotifyNeighborsModifiedMessage *
        parseIpcmNotifyNeighborsModifiedMessage(nlmsghdr *hdr) {
        struct nla_policy attr_policy[INNMM_ATTR_MAX + 1];
        attr_policy[INNMM_ATTR_ADDED].type = NLA_FLAG;
        attr_policy[INNMM_ATTR_ADDED].minlen = 0;
        attr_policy[INNMM_ATTR_ADDED].maxlen = 0;
        attr_policy[INNMM_ATTR_NEIGHBORS].type = NLA_NESTED;
        attr_policy[INNMM_ATTR_NEIGHBORS].minlen = 0;
        attr_policy[INNMM_ATTR_NEIGHBORS].maxlen = 0;
        struct nlattr *attrs[INNMM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        INNMM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                        "Error parsing IpcmNotifyNeighborsModifiedMessage information from Netlink message: %d",
                        err);
                return 0;
        }

        IpcmNotifyNeighborsModifiedMessage * result =
                        new IpcmNotifyNeighborsModifiedMessage();

        if (attrs[INNMM_ATTR_ADDED]) {
                result->setAdded(true);
        } else {
                result->setAdded(false);
        }

        int status = 0;
        if (attrs[INNMM_ATTR_NEIGHBORS]) {
                status = parseListOfNotifyNeighborsModifiedMessageNeighbors(
                                attrs[INNMM_ATTR_NEIGHBORS], result);
                if (status != 0){
                        delete result;
                        return 0;
                }
        }

        return result;
}

IpcmAllocateFlowRequestMessage *
	parseIpcmAllocateFlowRequestMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IAFRM_ATTR_MAX + 1];
	attr_policy[IAFRM_ATTR_SOURCE_APP_NAME].type = NLA_NESTED;
	attr_policy[IAFRM_ATTR_SOURCE_APP_NAME].minlen = 0;
	attr_policy[IAFRM_ATTR_SOURCE_APP_NAME].maxlen = 0;
	attr_policy[IAFRM_ATTR_DEST_APP_NAME].type = NLA_NESTED;
	attr_policy[IAFRM_ATTR_DEST_APP_NAME].minlen = 0;
	attr_policy[IAFRM_ATTR_DEST_APP_NAME].maxlen = 0;
	attr_policy[IAFRM_ATTR_FLOW_SPEC].type = NLA_NESTED;
	attr_policy[IAFRM_ATTR_FLOW_SPEC].minlen = 0;
	attr_policy[IAFRM_ATTR_FLOW_SPEC].maxlen = 0;
	attr_policy[IAFRM_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[IAFRM_ATTR_DIF_NAME].minlen = 0;
	attr_policy[IAFRM_ATTR_DIF_NAME].maxlen = 0;
	struct nlattr *attrs[IAFRM_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IAFRM_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmAssignToDIFRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmAllocateFlowRequestMessage * result =
			new IpcmAllocateFlowRequestMessage();
	ApplicationProcessNamingInformation * sourceName;
	ApplicationProcessNamingInformation * destName;
	FlowSpecification * flowSpec;
	ApplicationProcessNamingInformation * difName;

	if (attrs[IAFRM_ATTR_SOURCE_APP_NAME]) {
		sourceName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRM_ATTR_SOURCE_APP_NAME]);
		if (sourceName == 0) {
			delete result;
			return 0;
		} else {
			result->setSourceAppName(*sourceName);
			delete sourceName;
		}
	}

	if (attrs[IAFRM_ATTR_DEST_APP_NAME]) {
		destName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRM_ATTR_DEST_APP_NAME]);
		if (destName == 0) {
			delete result;
			return 0;
		} else {
			result->setDestAppName(*destName);
			delete destName;
		}
	}

	if (attrs[IAFRM_ATTR_FLOW_SPEC]) {
		flowSpec = parseFlowSpecificationObject(attrs[IAFRM_ATTR_FLOW_SPEC]);
		if (flowSpec == 0) {
			delete result;
			return 0;
		} else {
			result->setFlowSpec(*flowSpec);
			delete flowSpec;
		}
	}

	if (attrs[IAFRM_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRM_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	return result;
}

IpcmAllocateFlowRequestResultMessage *
	parseIpcmAllocateFlowRequestResultMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IAFRRM_ATTR_MAX + 1];
	attr_policy[IAFRRM_ATTR_RESULT].type = NLA_U32;
	attr_policy[IAFRRM_ATTR_RESULT].minlen = 4;
	attr_policy[IAFRRM_ATTR_RESULT].maxlen = 4;
	attr_policy[IAFRRM_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[IAFRRM_ATTR_PORT_ID].minlen = 4;
	attr_policy[IAFRRM_ATTR_PORT_ID].maxlen = 4;
	struct nlattr *attrs[IAFRRM_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IAFRRM_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
		        "Error parsing IpcmAllocateFlowRequestResultMessage information from Netlink message: %d",
		         err);
		return 0;
	}

	IpcmAllocateFlowRequestResultMessage * result =
				new IpcmAllocateFlowRequestResultMessage();

	if (attrs[IAFRRM_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[IAFRRM_ATTR_RESULT]));
	}

	if (attrs[IAFRRM_ATTR_PORT_ID]) {
	        result->setPortId(nla_get_u32(attrs[IAFRRM_ATTR_PORT_ID]));
	}

	return result;
}

IpcmAllocateFlowRequestArrivedMessage * parseIpcmAllocateFlowRequestArrivedMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[IAFRA_ATTR_MAX + 1];
	attr_policy[IAFRA_ATTR_SOURCE_APP_NAME].type = NLA_NESTED;
	attr_policy[IAFRA_ATTR_SOURCE_APP_NAME].minlen = 0;
	attr_policy[IAFRA_ATTR_SOURCE_APP_NAME].maxlen = 0;
	attr_policy[IAFRA_ATTR_DEST_APP_NAME].type = NLA_NESTED;
	attr_policy[IAFRA_ATTR_DEST_APP_NAME].minlen = 0;
	attr_policy[IAFRA_ATTR_DEST_APP_NAME].maxlen = 0;
	attr_policy[IAFRA_ATTR_FLOW_SPEC].type = NLA_NESTED;
	attr_policy[IAFRA_ATTR_FLOW_SPEC].minlen = 0;
	attr_policy[IAFRA_ATTR_FLOW_SPEC].maxlen = 0;
	attr_policy[IAFRA_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[IAFRA_ATTR_DIF_NAME].minlen = 0;
	attr_policy[IAFRA_ATTR_DIF_NAME].maxlen = 0;
	attr_policy[IAFRA_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[IAFRA_ATTR_PORT_ID].minlen = 0;
	attr_policy[IAFRA_ATTR_PORT_ID].maxlen = 0;
	struct nlattr *attrs[IAFRA_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IAFRA_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmAllocateFlowRequestArrivedMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmAllocateFlowRequestArrivedMessage * result =
			new IpcmAllocateFlowRequestArrivedMessage();
	ApplicationProcessNamingInformation * sourceName;
	ApplicationProcessNamingInformation * destName;
	FlowSpecification * flowSpec;
	ApplicationProcessNamingInformation * difName;

	if (attrs[IAFRA_ATTR_SOURCE_APP_NAME]) {
		sourceName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRA_ATTR_SOURCE_APP_NAME]);
		if (sourceName == 0) {
			delete result;
			return 0;
		} else {
			result->setSourceAppName(*sourceName);
			delete sourceName;
		}
	}

	if (attrs[IAFRA_ATTR_DEST_APP_NAME]) {
		destName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRA_ATTR_DEST_APP_NAME]);
		if (destName == 0) {
			delete result;
			return 0;
		} else {
			result->setDestAppName(*destName);
			delete destName;
		}
	}

	if (attrs[IAFRA_ATTR_FLOW_SPEC]) {
		flowSpec = parseFlowSpecificationObject(attrs[IAFRA_ATTR_FLOW_SPEC]);
		if (flowSpec == 0) {
			delete result;
			return 0;
		} else {
			result->setFlowSpecification(*flowSpec);
			delete flowSpec;
		}
	}

	if (attrs[IAFRA_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[IAFRA_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	if (attrs[IAFRA_ATTR_PORT_ID]) {
	        result->setPortId(nla_get_u32(attrs[IAFRA_ATTR_PORT_ID]));
	}

	return result;
}

IpcmAllocateFlowResponseMessage * parseIpcmAllocateFlowResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[IAFRE_ATTR_MAX + 1];
	attr_policy[IAFRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IAFRE_ATTR_RESULT].minlen = 4;
	attr_policy[IAFRE_ATTR_RESULT].maxlen = 4;
	attr_policy[IAFRE_ATTR_NOTIFY_SOURCE].type = NLA_FLAG;
	attr_policy[IAFRE_ATTR_NOTIFY_SOURCE].minlen = 0;
	attr_policy[IAFRE_ATTR_NOTIFY_SOURCE].maxlen = 0;
	struct nlattr *attrs[IAFRE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IAFRE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmAllocateFlowResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmAllocateFlowResponseMessage * result =
			new IpcmAllocateFlowResponseMessage();

	if (attrs[IAFRE_ATTR_RESULT]) {
		result->setResult((nla_get_u32(attrs[IAFRE_ATTR_RESULT])));
	}

	if (attrs[IAFRE_ATTR_NOTIFY_SOURCE]) {
		result->setNotifySource(
				(nla_get_flag(attrs[IAFRE_ATTR_NOTIFY_SOURCE])));
	}

	return result;
}

IpcmDeallocateFlowRequestMessage * parseIpcmDeallocateFlowRequestMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[IDFRT_ATTR_MAX + 1];
	attr_policy[IDFRT_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[IDFRT_ATTR_PORT_ID].minlen = 4;
	attr_policy[IDFRT_ATTR_PORT_ID].maxlen = 4;
	struct nlattr *attrs[IDFRT_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IDFRT_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmDeallocateFlowRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmDeallocateFlowRequestMessage * result =
			new IpcmDeallocateFlowRequestMessage();

	if (attrs[IDFRT_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[IDFRT_ATTR_PORT_ID]));
	}

	return result;
}

IpcmDeallocateFlowResponseMessage * parseIpcmDeallocateFlowResponseMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[IDFRE_ATTR_MAX + 1];
	attr_policy[IDFRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IDFRE_ATTR_RESULT].minlen = 4;
	attr_policy[IDFRE_ATTR_RESULT].maxlen = 4;
	struct nlattr *attrs[IDFRE_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IDFRE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmDeallocateFlowResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmDeallocateFlowResponseMessage * result =
			new IpcmDeallocateFlowResponseMessage();

	if (attrs[IDFRE_ATTR_RESULT]) {
		result->setResult(nla_get_u32(attrs[IDFRE_ATTR_RESULT]));
	}

	return result;
}

IpcmFlowDeallocatedNotificationMessage * parseIpcmFlowDeallocatedNotificationMessage(
		nlmsghdr *hdr) {
	struct nla_policy attr_policy[IFDN_ATTR_MAX + 1];
	attr_policy[IFDN_ATTR_PORT_ID].type = NLA_U32;
	attr_policy[IFDN_ATTR_PORT_ID].minlen = 4;
	attr_policy[IFDN_ATTR_PORT_ID].maxlen = 4;
	attr_policy[IFDN_ATTR_CODE].type = NLA_U32;
	attr_policy[IFDN_ATTR_CODE].minlen = 4;
	attr_policy[IFDN_ATTR_CODE].maxlen = 4;
	struct nlattr *attrs[IFDN_ATTR_MAX + 1];

	/*
	 * The nlmsg_parse() function will make sure that the message contains
	 * enough payload to hold the header (struct my_hdr), validates any
	 * attributes attached to the messages and stores a pointer to each
	 * attribute in the attrs[] array accessable by attribute type.
	 */
	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IFDN_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmFlowDeallocatedNotificationMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmFlowDeallocatedNotificationMessage * result =
			new IpcmFlowDeallocatedNotificationMessage();

	if (attrs[IFDN_ATTR_PORT_ID]) {
		result->setPortId(nla_get_u32(attrs[IFDN_ATTR_PORT_ID]));
	}

	if (attrs[IFDN_ATTR_CODE]) {
		result->setCode(nla_get_u32(attrs[IFDN_ATTR_CODE]));
	}

	return result;
}

IpcmDIFRegistrationNotification *
parseIpcmDIFRegistrationNotification(nlmsghdr *hdr){
	struct nla_policy attr_policy[IDRN_ATTR_MAX + 1];
	attr_policy[IDRN_ATTR_IPC_PROCESS_NAME].type = NLA_NESTED;
	attr_policy[IDRN_ATTR_IPC_PROCESS_NAME].minlen = 0;
	attr_policy[IDRN_ATTR_IPC_PROCESS_NAME].maxlen = 0;
	attr_policy[IDRN_ATTR_DIF_NAME].type = NLA_NESTED;
	attr_policy[IDRN_ATTR_DIF_NAME].minlen = 0;
	attr_policy[IDRN_ATTR_DIF_NAME].maxlen = 0;
	attr_policy[IDRN_ATTR_REGISTRATION].type = NLA_FLAG;
	attr_policy[IDRN_ATTR_REGISTRATION].minlen = 0;
	attr_policy[IDRN_ATTR_REGISTRATION].maxlen = 0;
	struct nlattr *attrs[IDRN_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IDRN_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmDIFRegistrationNotification information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmDIFRegistrationNotification * result =
			new IpcmDIFRegistrationNotification ();
	ApplicationProcessNamingInformation * ipcProcessName;
	ApplicationProcessNamingInformation * difName;

	if (attrs[IDRN_ATTR_IPC_PROCESS_NAME]) {
		ipcProcessName = parseApplicationProcessNamingInformationObject(
				attrs[IDRN_ATTR_IPC_PROCESS_NAME]);
		if (ipcProcessName == 0) {
			delete result;
			return 0;
		} else {
			result->setIpcProcessName(*ipcProcessName);
			delete ipcProcessName;
		}
	}

	if (attrs[IDRN_ATTR_DIF_NAME]) {
		difName = parseApplicationProcessNamingInformationObject(
				attrs[IDRN_ATTR_DIF_NAME]);
		if (difName == 0) {
			delete result;
			return 0;
		} else {
			result->setDifName(*difName);
			delete difName;
		}
	}

	if (attrs[IDRN_ATTR_REGISTRATION]){
		result->setRegistered(true);
	}else{
		result->setRegistered(false);
	}

	return result;
}

IpcmDIFQueryRIBRequestMessage *
	parseIpcmDIFQueryRIBRequestMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IDQR_ATTR_MAX + 1];
	attr_policy[IDQR_ATTR_OBJECT_CLASS].type = NLA_STRING;
	attr_policy[IDQR_ATTR_OBJECT_CLASS].minlen = 0;
	attr_policy[IDQR_ATTR_OBJECT_CLASS].maxlen = 65535;
	attr_policy[IDQR_ATTR_OBJECT_NAME].type = NLA_STRING;
	attr_policy[IDQR_ATTR_OBJECT_NAME].minlen = 0;
	attr_policy[IDQR_ATTR_OBJECT_NAME].maxlen = 65535;
	attr_policy[IDQR_ATTR_OBJECT_INSTANCE].type = NLA_U64;
	attr_policy[IDQR_ATTR_OBJECT_INSTANCE].minlen = 8;
	attr_policy[IDQR_ATTR_OBJECT_INSTANCE].maxlen = 8;
	attr_policy[IDQR_ATTR_SCOPE].type = NLA_U32;
	attr_policy[IDQR_ATTR_SCOPE].minlen = 4;
	attr_policy[IDQR_ATTR_SCOPE].maxlen = 4;
	attr_policy[IDQR_ATTR_FILTER].type = NLA_STRING;
	attr_policy[IDQR_ATTR_FILTER].minlen = 0;
	attr_policy[IDQR_ATTR_FILTER].maxlen = 65535;
	struct nlattr *attrs[IDQR_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IDQR_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmDIFQueryRIBRequestMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmDIFQueryRIBRequestMessage * result =
				new IpcmDIFQueryRIBRequestMessage ();

	if (attrs[IDQR_ATTR_OBJECT_CLASS]){
		result->setObjectClass(nla_get_string(attrs[IDQR_ATTR_OBJECT_CLASS]));
	}

	if (attrs[IDQR_ATTR_OBJECT_NAME]){
		result->setObjectName(nla_get_string(attrs[IDQR_ATTR_OBJECT_NAME]));
	}

	if (attrs[IDQR_ATTR_OBJECT_INSTANCE]){
		result->setObjectInstance(
				nla_get_u64(attrs[IDQR_ATTR_OBJECT_INSTANCE]));
	}

	if (attrs[IDQR_ATTR_SCOPE]){
		result->setScope(nla_get_u32(attrs[IDQR_ATTR_SCOPE]));
	}

	if (attrs[IDQR_ATTR_FILTER]){
		result->setFilter(
				nla_get_string(attrs[IDQR_ATTR_FILTER]));
	}

	return result;
}

RIBObject * parseRIBObject(nlattr *nested){
	struct nla_policy attr_policy[RIBO_ATTR_MAX + 1];
	attr_policy[RIBO_ATTR_OBJECT_CLASS].type = NLA_STRING;
	attr_policy[RIBO_ATTR_OBJECT_CLASS].minlen = 0;
	attr_policy[RIBO_ATTR_OBJECT_CLASS].maxlen = 65535;
	attr_policy[RIBO_ATTR_OBJECT_NAME].type = NLA_STRING;
	attr_policy[RIBO_ATTR_OBJECT_NAME].minlen = 0;
	attr_policy[RIBO_ATTR_OBJECT_NAME].maxlen = 65535;
	attr_policy[RIBO_ATTR_OBJECT_INSTANCE].type = NLA_U64;
	attr_policy[RIBO_ATTR_OBJECT_INSTANCE].minlen = 8;
	attr_policy[RIBO_ATTR_OBJECT_INSTANCE].maxlen = 8;
	struct nlattr *attrs[RIBO_ATTR_MAX + 1];

	int err = nla_parse_nested(attrs, RIBO_ATTR_MAX, nested, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing RIBObject from Netlink message: %d",
				err);
		return 0;
	}

	RIBObject * result = new RIBObject();

	if (attrs[RIBO_ATTR_OBJECT_CLASS]){
		result->setClazz(
				nla_get_string(attrs[RIBO_ATTR_OBJECT_CLASS]));
	}

	if (attrs[RIBO_ATTR_OBJECT_NAME]){
		result->setName(
				nla_get_string(attrs[RIBO_ATTR_OBJECT_NAME]));
	}

	if (attrs[RIBO_ATTR_OBJECT_INSTANCE]){
		result->setInstance(
				nla_get_u64(attrs[RIBO_ATTR_OBJECT_INSTANCE]));
	}

	return result;
}

int parseListOfRIBObjects(nlattr *nested,
		IpcmDIFQueryRIBResponseMessage * message){
	nlattr * nla;
	int rem;
	RIBObject * ribObject;

	for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
		     nla_ok(nla, rem);
		     nla = nla_next(nla, &(rem))){
		/* validate & parse attribute */
		ribObject = parseRIBObject(nla);
		if (ribObject == 0){
			return -1;
		}
		message->addRIBObject(*ribObject);
		delete ribObject;
	}

	if (rem > 0){
		LOG_WARN("Missing bits to parse");
	}

	return 0;
}

IpcmDIFQueryRIBResponseMessage *
	parseIpcmDIFQueryRIBResponseMessage(nlmsghdr *hdr){
	struct nla_policy attr_policy[IDQRE_ATTR_MAX + 1];
	attr_policy[IDQRE_ATTR_RESULT].type = NLA_U32;
	attr_policy[IDQRE_ATTR_RESULT].minlen = 4;
	attr_policy[IDQRE_ATTR_RESULT].maxlen = 4;
	attr_policy[IDQRE_ATTR_RIB_OBJECTS].type = NLA_NESTED;
	attr_policy[IDQRE_ATTR_RIB_OBJECTS].minlen = 0;
	attr_policy[IDQRE_ATTR_RIB_OBJECTS].maxlen = 0;
	struct nlattr *attrs[IDQRE_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			IDQRE_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmDIFQueryRIBResponseMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmDIFQueryRIBResponseMessage * result =
					new IpcmDIFQueryRIBResponseMessage ();

	if (attrs[IDQRE_ATTR_RESULT]){
		result->setResult(nla_get_u32(attrs[IDQRE_ATTR_RESULT]));
	}

	int status = 0;
	if (attrs[IDQRE_ATTR_RIB_OBJECTS]) {
		status = parseListOfRIBObjects(
				attrs[IDQRE_ATTR_RIB_OBJECTS], result);
		if (status != 0){
			delete result;
			return 0;
		}
	}

	return result;
}

IpcmNLSocketClosedNotificationMessage *
	parseIpcmNLSocketClosedNotificationMessage(nlmsghdr *hdr)
{
	struct nla_policy attr_policy[INSCN_ATTR_MAX + 1];
	attr_policy[INSCN_ATTR_PORT].type = NLA_U32;
	attr_policy[INSCN_ATTR_PORT].minlen = 4;
	attr_policy[INSCN_ATTR_PORT].maxlen = 4;
	struct nlattr *attrs[INSCN_ATTR_MAX + 1];

	int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
			INSCN_ATTR_MAX, attr_policy);
	if (err < 0) {
		LOG_ERR(
				"Error parsing IpcmNLSocketClosedNotificationMessage information from Netlink message: %d",
				err);
		return 0;
	}

	IpcmNLSocketClosedNotificationMessage * result =
					new IpcmNLSocketClosedNotificationMessage ();

	if (attrs[INSCN_ATTR_PORT]){
		result->setPortId(nla_get_u32(attrs[INSCN_ATTR_PORT]));
	}

	return result;
}

IpcmIPCProcessInitializedMessage * parseIpcmIPCProcessInitializedMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[IIPM_ATTR_MAX + 1];
        attr_policy[IIPM_ATTR_NAME].type = NLA_NESTED;
        attr_policy[IIPM_ATTR_NAME].minlen = 0;
        attr_policy[IIPM_ATTR_NAME].maxlen = 0;
        struct nlattr *attrs[IIPM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        IIPM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR(
                                "Error parsing IpcmIPCProcessInitializedMessage information from Netlink message: %d",
                                err);
                return 0;
        }

        IpcmIPCProcessInitializedMessage * result =
                        new IpcmIPCProcessInitializedMessage ();
        ApplicationProcessNamingInformation * ipcProcessName;

        if (attrs[IIPM_ATTR_NAME]) {
                ipcProcessName = parseApplicationProcessNamingInformationObject(
                                attrs[IIPM_ATTR_NAME]);
                if (ipcProcessName == 0) {
                        delete result;
                        return 0;
                } else {
                        result->setName(*ipcProcessName);
                        delete ipcProcessName;
                }
        }

        return result;
}

IpcpConnectionCreateRequestMessage * parseIpcpConnectionCreateRequestMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICCRM_ATTR_MAX + 1];
        attr_policy[ICCRM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICCRM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICCRM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICCRM_ATTR_SRC_ADDRESS].type = NLA_U32;
        attr_policy[ICCRM_ATTR_SRC_ADDRESS].minlen = 4;
        attr_policy[ICCRM_ATTR_SRC_ADDRESS].maxlen = 4;
        attr_policy[ICCRM_ATTR_DEST_ADDRESS].type = NLA_U32;
        attr_policy[ICCRM_ATTR_DEST_ADDRESS].minlen = 4;
        attr_policy[ICCRM_ATTR_DEST_ADDRESS].maxlen = 4;
        attr_policy[ICCRM_ATTR_QOS_ID].type = NLA_U32;
        attr_policy[ICCRM_ATTR_QOS_ID].minlen = 4;
        attr_policy[ICCRM_ATTR_QOS_ID].maxlen = 4;
        attr_policy[ICCRM_ATTR_POLICIES_PARAMETERS].type = NLA_NESTED;
        attr_policy[ICCRM_ATTR_POLICIES_PARAMETERS].minlen = 0;
        attr_policy[ICCRM_ATTR_POLICIES_PARAMETERS].maxlen = 0;
        struct nlattr *attrs[ICCRM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICCRM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionCreateRequestMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionCreateRequestMessage * result =
                        new IpcpConnectionCreateRequestMessage();

        ConnectionPoliciesParameters * connPoliciesParams;

        if (attrs[ICCRM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICCRM_ATTR_PORT_ID]));
        }

        if (attrs[ICCRM_ATTR_SRC_ADDRESS]){
                result->setSourceAddress(
                                nla_get_u32(attrs[ICCRM_ATTR_SRC_ADDRESS]));
        }

        if (attrs[ICCRM_ATTR_DEST_ADDRESS]){
                result->setDestAddress(
                                nla_get_u32(attrs[ICCRM_ATTR_DEST_ADDRESS]));
        }

        if (attrs[ICCRM_ATTR_QOS_ID]){
                result->setQosId(nla_get_u32(attrs[ICCRM_ATTR_QOS_ID]));
        }

	if (attrs[ICCRM_ATTR_POLICIES_PARAMETERS]) {
		connPoliciesParams = parseConnectionPoliciesParametersObject(
				attrs[ICCRM_ATTR_POLICIES_PARAMETERS]);
		if (connPoliciesParams == 0) {
			delete result;
			return 0;
		} else {
			result->setConnPoliciesParams(*connPoliciesParams);
			delete connPoliciesParams;
		}
	}

        return result;
}

IpcpConnectionCreateResponseMessage * parseIpcpConnectionCreateResponseMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICCREM_ATTR_MAX + 1];
        attr_policy[ICCREM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICCREM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICCREM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICCREM_ATTR_SRC_CEP_ID].type = NLA_U32;
        attr_policy[ICCREM_ATTR_SRC_CEP_ID].minlen = 4;
        attr_policy[ICCREM_ATTR_SRC_CEP_ID].maxlen = 4;
        struct nlattr *attrs[ICCREM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICCREM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionCreateResponseMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionCreateResponseMessage * result =
                        new IpcpConnectionCreateResponseMessage();

        if (attrs[ICCREM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICCREM_ATTR_PORT_ID]));
        }

        if (attrs[ICCREM_ATTR_SRC_CEP_ID]){
                result->setCepId(nla_get_u32(attrs[ICCREM_ATTR_SRC_CEP_ID]));
        }

        return result;
}

IpcpConnectionUpdateRequestMessage * parseIpcpConnectionUpdateRequestMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICURM_ATTR_MAX + 1];
        attr_policy[ICURM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICURM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICURM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICURM_ATTR_SRC_CEP_ID].type = NLA_U32;
        attr_policy[ICURM_ATTR_SRC_CEP_ID].minlen = 4;
        attr_policy[ICURM_ATTR_SRC_CEP_ID].maxlen = 4;
        attr_policy[ICURM_ATTR_DEST_CEP_ID].type = NLA_U32;
        attr_policy[ICURM_ATTR_DEST_CEP_ID].minlen = 4;
        attr_policy[ICURM_ATTR_DEST_CEP_ID].maxlen = 4;
        attr_policy[ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID].type = NLA_U16;
        attr_policy[ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID].minlen = 2;
        attr_policy[ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID].maxlen = 2;
        struct nlattr *attrs[ICURM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICURM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionUpdateRequestMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionUpdateRequestMessage * result =
                        new IpcpConnectionUpdateRequestMessage();

        if (attrs[ICURM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICURM_ATTR_PORT_ID]));
        }

        if (attrs[ICURM_ATTR_SRC_CEP_ID]){
                result->setSourceCepId(
                                nla_get_u32(attrs[ICURM_ATTR_SRC_CEP_ID]));
        }

        if (attrs[ICURM_ATTR_DEST_CEP_ID]){
                result->setDestinationCepId(
                                nla_get_u32(attrs[ICURM_ATTR_DEST_CEP_ID]));
        }

        if (attrs[ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID]){
                result->setFlowUserIpcProcessId(
                                nla_get_u16(attrs[ICURM_ATTR_FLOW_USER_IPC_PROCESS_ID]));
        }

        return result;
}

IpcpConnectionUpdateResultMessage * parseIpcpConnectionUpdateResultMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICUREM_ATTR_MAX + 1];
        attr_policy[ICUREM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICUREM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICUREM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICUREM_ATTR_RESULT].type = NLA_U32;
        attr_policy[ICUREM_ATTR_RESULT].minlen = 4;
        attr_policy[ICUREM_ATTR_RESULT].maxlen = 4;
        struct nlattr *attrs[ICUREM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICUREM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionUpdateResultMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionUpdateResultMessage * result =
                        new IpcpConnectionUpdateResultMessage();

        if (attrs[ICUREM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICUREM_ATTR_PORT_ID]));
        }

        if (attrs[ICUREM_ATTR_RESULT]){
                result->setResult(
                                nla_get_u32(attrs[ICUREM_ATTR_RESULT]));
        }

        return result;
}

IpcpConnectionCreateArrivedMessage * parseIpcpConnectionCreateArrivedMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICCAM_ATTR_MAX + 1];
        attr_policy[ICCAM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICCAM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICCAM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICCAM_ATTR_SRC_ADDRESS].type = NLA_U32;
        attr_policy[ICCAM_ATTR_SRC_ADDRESS].minlen = 4;
        attr_policy[ICCAM_ATTR_SRC_ADDRESS].maxlen = 4;
        attr_policy[ICCAM_ATTR_DEST_ADDRESS].type = NLA_U32;
        attr_policy[ICCAM_ATTR_DEST_ADDRESS].minlen = 4;
        attr_policy[ICCAM_ATTR_DEST_ADDRESS].maxlen = 4;
        attr_policy[ICCAM_ATTR_QOS_ID].type = NLA_U32;
        attr_policy[ICCAM_ATTR_QOS_ID].minlen = 4;
        attr_policy[ICCAM_ATTR_QOS_ID].maxlen = 4;
        attr_policy[ICCAM_ATTR_DEST_CEP_ID].type = NLA_U32;
        attr_policy[ICCAM_ATTR_DEST_CEP_ID].minlen = 4;
        attr_policy[ICCAM_ATTR_DEST_CEP_ID].maxlen = 4;
        attr_policy[ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID].type = NLA_U16;
        attr_policy[ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID].minlen = 2;
        attr_policy[ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID].maxlen = 2;
        attr_policy[ICCAM_ATTR_POLICIES_PARAMETERS].type = NLA_NESTED;
        attr_policy[ICCAM_ATTR_POLICIES_PARAMETERS].minlen = 0;
        attr_policy[ICCAM_ATTR_POLICIES_PARAMETERS].maxlen = 0;
        struct nlattr *attrs[ICCAM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICCAM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionCreateArrivedMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionCreateArrivedMessage * result =
                        new IpcpConnectionCreateArrivedMessage();

	ConnectionPoliciesParameters * connPoliciesParams;

        if (attrs[ICCAM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICCAM_ATTR_PORT_ID]));
        }

        if (attrs[ICCAM_ATTR_SRC_ADDRESS]){
                result->setSourceAddress(
                                nla_get_u32(attrs[ICCAM_ATTR_SRC_ADDRESS]));
        }

        if (attrs[ICCAM_ATTR_DEST_ADDRESS]){
                result->setDestAddress(
                                nla_get_u32(attrs[ICCAM_ATTR_DEST_ADDRESS]));
        }

        if (attrs[ICCAM_ATTR_QOS_ID]){
                result->setQosId(nla_get_u32(attrs[ICCAM_ATTR_QOS_ID]));
        }

        if (attrs[ICCAM_ATTR_DEST_CEP_ID]){
                result->setDestCepId(nla_get_u32(attrs[ICCAM_ATTR_DEST_CEP_ID]));
        }

        if (attrs[ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID]){
                result->setFlowUserIpcProcessId(nla_get_u16(attrs[ICCAM_ATTR_FLOW_USER_IPC_PROCESS_ID]));
        }

	if (attrs[ICCAM_ATTR_POLICIES_PARAMETERS]) {
		connPoliciesParams = parseConnectionPoliciesParametersObject(
				attrs[ICCAM_ATTR_POLICIES_PARAMETERS]);
		if (connPoliciesParams == 0) {
			delete result;
			return 0;
		} else {
			result->setConnPoliciesParams(*connPoliciesParams);
			delete connPoliciesParams;
		}
	}

        return result;
}

IpcpConnectionCreateResultMessage * parseIpcpConnectionCreateResultMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICCRES_ATTR_MAX + 1];
        attr_policy[ICCRES_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICCRES_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICCRES_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICCRES_ATTR_SRC_CEP_ID].type = NLA_U32;
        attr_policy[ICCRES_ATTR_SRC_CEP_ID].minlen = 4;
        attr_policy[ICCRES_ATTR_SRC_CEP_ID].maxlen = 4;
        attr_policy[ICCRES_ATTR_DEST_CEP_ID].type = NLA_U32;
        attr_policy[ICCRES_ATTR_DEST_CEP_ID].minlen = 4;
        attr_policy[ICCRES_ATTR_DEST_CEP_ID].maxlen = 4;
        struct nlattr *attrs[ICCRES_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICCRES_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionCreateResultMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionCreateResultMessage * result =
                        new IpcpConnectionCreateResultMessage();

        if (attrs[ICCRES_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICCRES_ATTR_PORT_ID]));
        }

        if (attrs[ICCRES_ATTR_SRC_CEP_ID]){
                result->setSourceCepId(nla_get_u32(attrs[ICCRES_ATTR_SRC_CEP_ID]));
        }

        if (attrs[ICCRES_ATTR_DEST_CEP_ID]){
                result->setDestCepId(nla_get_u32(attrs[ICCRES_ATTR_DEST_CEP_ID]));
        }

        return result;
}

IpcpConnectionDestroyRequestMessage * parseIpcpConnectionDestroyRequestMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICDRM_ATTR_MAX + 1];
        attr_policy[ICDRM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICDRM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICDRM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICDRM_ATTR_CEP_ID].type = NLA_U32;
        attr_policy[ICDRM_ATTR_CEP_ID].minlen = 4;
        attr_policy[ICDRM_ATTR_CEP_ID].maxlen = 4;
        struct nlattr *attrs[ICDRM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICDRM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionDestroyRequestMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionDestroyRequestMessage * result =
                        new IpcpConnectionDestroyRequestMessage();

        if (attrs[ICDRM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICDRM_ATTR_PORT_ID]));
        }

        if (attrs[ICDRM_ATTR_CEP_ID]){
                result->setCepId(nla_get_u32(attrs[ICDRM_ATTR_CEP_ID]));
        }

        return result;
}

IpcpConnectionDestroyResultMessage * parseIpcpConnectionDestroyResultMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[ICDREM_ATTR_MAX + 1];
        attr_policy[ICDREM_ATTR_PORT_ID].type = NLA_U32;
        attr_policy[ICDREM_ATTR_PORT_ID].minlen = 4;
        attr_policy[ICDREM_ATTR_PORT_ID].maxlen = 4;
        attr_policy[ICDREM_ATTR_RESULT].type = NLA_U32;
        attr_policy[ICDREM_ATTR_RESULT].minlen = 4;
        attr_policy[ICDREM_ATTR_RESULT].maxlen = 4;
        struct nlattr *attrs[ICDREM_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        ICDREM_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing IpcpConnectionDestroyResultMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        IpcpConnectionDestroyResultMessage * result =
                        new IpcpConnectionDestroyResultMessage();

        if (attrs[ICDREM_ATTR_PORT_ID]){
                result->setPortId(nla_get_u32(attrs[ICDREM_ATTR_PORT_ID]));
        }

        if (attrs[ICDREM_ATTR_RESULT]){
                result->setResult(nla_get_u32(attrs[ICDREM_ATTR_RESULT]));
        }

        return result;
}

int parseListOfPortIds(nlattr *nested, PDUForwardingTableEntry * entry) {
        nlattr * nla;
        int rem;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                        nla_ok(nla, rem);
                        nla = nla_next(nla, &(rem))){
                entry->addPortId(nla_get_u32(nla));
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

PDUForwardingTableEntry * parsePDUForwardingTableEntry(nlattr *nested) {
        struct nla_policy attr_policy[PFTE_ATTR_MAX + 1];
        attr_policy[PFTE_ATTR_ADDRESS].type = NLA_U32;
        attr_policy[PFTE_ATTR_ADDRESS].minlen = 4;
        attr_policy[PFTE_ATTR_ADDRESS].maxlen = 4;
        attr_policy[PFTE_ATTR_QOS_ID].type = NLA_U32;
        attr_policy[PFTE_ATTR_QOS_ID].minlen = 4;
        attr_policy[PFTE_ATTR_QOS_ID].maxlen = 4;
        attr_policy[PFTE_ATTR_PORT_IDS].type = NLA_NESTED;
        attr_policy[PFTE_ATTR_PORT_IDS].minlen = 0;
        attr_policy[PFTE_ATTR_PORT_IDS].maxlen = 0;
        struct nlattr *attrs[PFTE_ATTR_MAX + 1];

        int err = nla_parse_nested(attrs, PFTE_ATTR_MAX, nested, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing PDUForwardingTableEntry from Netlink message: %d",
                        err);
                return 0;
        }

        PDUForwardingTableEntry * result = new PDUForwardingTableEntry();

        if (attrs[PFTE_ATTR_ADDRESS]){
                result->setAddress(
                                nla_get_u32(attrs[PFTE_ATTR_ADDRESS]));
        }

        if (attrs[PFTE_ATTR_QOS_ID]){
                result->setQosId(
                                nla_get_u32(attrs[PFTE_ATTR_QOS_ID]));
        }

        if (attrs[PFTE_ATTR_PORT_IDS]){
                parseListOfPortIds(attrs[PFTE_ATTR_PORT_IDS], result);
        }

        return result;
}

int parseListOfPDUFTEntries(nlattr *nested,
                RmtModifyPDUFTEntriesRequestMessage * message){
        nlattr * nla;
        int rem;
        PDUForwardingTableEntry * pfte;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                     nla_ok(nla, rem);
                     nla = nla_next(nla, &(rem))){
                /* validate & parse attribute */
                pfte = parsePDUForwardingTableEntry(nla);
                if (pfte == 0){
                        return -1;
                }
                message->addEntry(*pfte);
                delete pfte;
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

RmtModifyPDUFTEntriesRequestMessage * parseRmtModifyPDUFTEntriesRequestMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[RMPFTE_ATTR_MAX + 1];
        attr_policy[RMPFTE_ATTR_ENTRIES].type = NLA_NESTED;
        attr_policy[RMPFTE_ATTR_ENTRIES].minlen = 0;
        attr_policy[RMPFTE_ATTR_ENTRIES].maxlen = 0;
        attr_policy[RMPFTE_ATTR_MODE].type = NLA_U32;
        attr_policy[RMPFTE_ATTR_MODE].minlen = 4;
        attr_policy[RMPFTE_ATTR_MODE].maxlen = 4;
        struct nlattr *attrs[RMPFTE_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        RMPFTE_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing RmtModifyPDUFTEntriesRequestMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        RmtModifyPDUFTEntriesRequestMessage * result =
                        new RmtModifyPDUFTEntriesRequestMessage();

        if (attrs[RMPFTE_ATTR_MODE]){
                result->setMode(nla_get_u32(attrs[RMPFTE_ATTR_MODE]));
        }

        int status = 0;
        if (attrs[RMPFTE_ATTR_ENTRIES]) {
                status = parseListOfPDUFTEntries(
                                attrs[RMPFTE_ATTR_ENTRIES], result);
                if (status != 0){
                        delete result;
                        return 0;
                }
        }

        return result;
}

int parseListOfPDUFTEntries2(nlattr *nested,
                RmtDumpPDUFTEntriesResponseMessage * message){
        nlattr * nla;
        int rem;
        PDUForwardingTableEntry * pfte;

        for (nla = (nlattr*) nla_data(nested), rem = nla_len(nested);
                     nla_ok(nla, rem);
                     nla = nla_next(nla, &(rem))){
                /* validate & parse attribute */
                pfte = parsePDUForwardingTableEntry(nla);
                if (pfte == 0){
                        return -1;
                }
                message->addEntry(*pfte);
                delete pfte;
        }

        if (rem > 0){
                LOG_WARN("Missing bits to parse");
        }

        return 0;
}

RmtDumpPDUFTEntriesResponseMessage * parseRmtDumpPDUFTEntriesResponseMessage(
                nlmsghdr *hdr) {
        struct nla_policy attr_policy[RDPFTE_ATTR_MAX + 1];
        attr_policy[RDPFTE_ATTR_RESULT].type = NLA_U32;
        attr_policy[RDPFTE_ATTR_RESULT].minlen = 4;
        attr_policy[RDPFTE_ATTR_RESULT].maxlen = 4;
        attr_policy[RDPFTE_ATTR_ENTRIES].type = NLA_NESTED;
        attr_policy[RDPFTE_ATTR_ENTRIES].minlen = 0;
        attr_policy[RDPFTE_ATTR_ENTRIES].maxlen = 0;
        struct nlattr *attrs[RDPFTE_ATTR_MAX + 1];

        int err = genlmsg_parse(hdr, sizeof(struct rinaHeader), attrs,
                        RDPFTE_ATTR_MAX, attr_policy);
        if (err < 0) {
                LOG_ERR("Error parsing RmtDumpPDUFTEntriesResponseMessage information from Netlink message: %d",
                         err);
                return 0;
        }

        RmtDumpPDUFTEntriesResponseMessage * result =
                        new RmtDumpPDUFTEntriesResponseMessage();

        if (attrs[RDPFTE_ATTR_RESULT]){
                result->setResult(nla_get_u32(attrs[RDPFTE_ATTR_RESULT]));
        }

        int status = 0;
        if (attrs[RDPFTE_ATTR_ENTRIES]) {
                status = parseListOfPDUFTEntries2(
                                attrs[RDPFTE_ATTR_ENTRIES], result);
                if (status != 0){
                        delete result;
                        return 0;
                }
        }

        return result;
}

}
