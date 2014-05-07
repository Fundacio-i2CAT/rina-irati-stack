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

#include <ostream>
#include <sstream>

#define RINA_PREFIX "ipc-process"
#define PAGE_SIZE 4096

#include "logs.h"
#include "librina-ipc-process.h"
#include "core.h"
#include "rina-syscalls.h"

namespace rina{

/* CLASS ASSIGN TO DIF REQUEST EVENT */
AssignToDIFRequestEvent::AssignToDIFRequestEvent(
		const DIFInformation& difInformation,
			unsigned int sequenceNumber):
		IPCEvent(ASSIGN_TO_DIF_REQUEST_EVENT, sequenceNumber)
{
	this->difInformation = difInformation;
}

const DIFInformation&
AssignToDIFRequestEvent::getDIFInformation() const{
	return difInformation;
}

/* CLASS ENROLL TO DIF REQUEST EVENT */
EnrollToDIFRequestEvent::EnrollToDIFRequestEvent(
                const ApplicationProcessNamingInformation& difName,
                const ApplicationProcessNamingInformation& supportingDIFName,
                const ApplicationProcessNamingInformation& neighborName,
                unsigned int sequenceNumber):
                IPCEvent(ENROLL_TO_DIF_REQUEST_EVENT, sequenceNumber)
{
        this->difName = difName;
        this->supportingDIFName = supportingDIFName;
        this->neighborName = neighborName;
}

const ApplicationProcessNamingInformation&
EnrollToDIFRequestEvent::getDifName() const {
        return difName;
}

const ApplicationProcessNamingInformation&
EnrollToDIFRequestEvent::getNeighborName() const {
        return neighborName;
}

const ApplicationProcessNamingInformation&
EnrollToDIFRequestEvent::getSupportingDifName() const {
        return supportingDIFName;
}

/* CLASS UPDATE DIF CONFIGURATION REQUEST EVENT */
const DIFConfiguration&
UpdateDIFConfigurationRequestEvent::getDIFConfiguration() const
{
        return difConfiguration;
}

UpdateDIFConfigurationRequestEvent::UpdateDIFConfigurationRequestEvent(
                const DIFConfiguration& difConfiguration,
                        unsigned int sequenceNumber):
                IPCEvent(UPDATE_DIF_CONFIG_REQUEST_EVENT, sequenceNumber)
{
        this->difConfiguration = difConfiguration;
}

/* CLASS IPC PROCESS DIF REGISTRATION EVENT */
IPCProcessDIFRegistrationEvent::IPCProcessDIFRegistrationEvent(
		const ApplicationProcessNamingInformation& ipcProcessName,
		const ApplicationProcessNamingInformation& difName,
		bool registered,
		unsigned int sequenceNumber): IPCEvent(
		                IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION,
		                sequenceNumber){
        this->ipcProcessName = ipcProcessName;
        this->difName = difName;
        this->registered = registered;
}

const ApplicationProcessNamingInformation&
IPCProcessDIFRegistrationEvent::getIPCProcessName() const{
	return ipcProcessName;
}

const ApplicationProcessNamingInformation&
IPCProcessDIFRegistrationEvent::getDIFName() const{
	return difName;
}

bool IPCProcessDIFRegistrationEvent::isRegistered() const {
        return registered;
}

/* CLASS QUERY RIB REQUEST EVENT */
QueryRIBRequestEvent::QueryRIBRequestEvent(const std::string& objectClass,
		const std::string& objectName, long objectInstance,
		int scope, const std::string& filter,
		unsigned int sequenceNumber):
				IPCEvent(IPC_PROCESS_QUERY_RIB, sequenceNumber){
	this->objectClass = objectClass;
	this->objectName = objectName;
	this->objectInstance = objectInstance;
	this->scope = scope;
	this->filter = filter;
}

const std::string& QueryRIBRequestEvent::getObjectClass() const{
	return objectClass;
}

const std::string& QueryRIBRequestEvent::getObjectName() const{
	return objectName;
}

long QueryRIBRequestEvent::getObjectInstance() const{
	return objectInstance;
}

int QueryRIBRequestEvent::getScope() const{
	return scope;
}

const std::string& QueryRIBRequestEvent::getFilter() const{
	return filter;
}

/* CLASS CREATE CONNECTION RESPONSE EVENT */
CreateConnectionResponseEvent::CreateConnectionResponseEvent(int portId,
        int cepId, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_CREATE_CONNECTION_RESPONSE,
                                sequenceNumber) {
        this->cepId = cepId;
        this->portId = portId;
}

int CreateConnectionResponseEvent::getCepId() const {
        return cepId;
}

int CreateConnectionResponseEvent::getPortId() const {
        return portId;
}

/* CLASS UPDATE CONNECTION RESPONSE EVENT */
UpdateConnectionResponseEvent::UpdateConnectionResponseEvent(int portId,
        int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_UPDATE_CONNECTION_RESPONSE,
                                sequenceNumber) {
        this->result = result;
        this->portId = portId;
}

int UpdateConnectionResponseEvent::getResult() const {
        return result;
}

int UpdateConnectionResponseEvent::getPortId() const {
        return portId;
}

/* CLASS CREATE CONNECTION RESULT EVENT */
CreateConnectionResultEvent::CreateConnectionResultEvent(int portId,
        int sourceCepId, int destCepId, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_CREATE_CONNECTION_RESULT,
                                sequenceNumber) {
        this->sourceCepId = sourceCepId;
        this->destCepId = destCepId;
        this->portId = portId;
}

int CreateConnectionResultEvent::getSourceCepId() const {
        return sourceCepId;
}

int CreateConnectionResultEvent::getDestCepId() const {
        return destCepId;
}

int CreateConnectionResultEvent::getPortId() const {
        return portId;
}

/* CLASS DESTROY CONNECTION RESULT EVENT */
DestroyConnectionResultEvent::DestroyConnectionResultEvent(int portId,
        int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_DESTROY_CONNECTION_RESULT,
                                sequenceNumber) {
        this->result = result;
        this->portId = portId;
}

int DestroyConnectionResultEvent::getResult() const {
        return result;
}

int DestroyConnectionResultEvent::getPortId() const {
        return portId;
}

/* CLASS DUMP PDU FT RESULT EVENT*/
DumpFTResponseEvent::DumpFTResponseEvent(const std::list<PDUForwardingTableEntry>& entries,
                int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_DUMP_FT_RESPONSE,
                                sequenceNumber) {
        this->entries = entries;
        this->result = result;
}

const std::list<PDUForwardingTableEntry>&
DumpFTResponseEvent::getEntries() const {
        return entries;
}

int DumpFTResponseEvent::getResult() const {
        return result;
}

/* CLASS EXTENDED IPC MANAGER */
const std::string ExtendedIPCManager::error_allocate_flow =
		"Error allocating flow";

ExtendedIPCManager::ExtendedIPCManager() {
        ipcManagerPort = 0;
        ipcProcessId = 0;
        ipcProcessInitialized = false;
}

ExtendedIPCManager::~ExtendedIPCManager() throw(){
}

const DIFInformation& ExtendedIPCManager::getCurrentDIFInformation() const{
	return currentDIFInformation;
}

void ExtendedIPCManager::setCurrentDIFInformation(
		const DIFInformation& currentDIFInformation){
	this->currentDIFInformation = currentDIFInformation;
}

unsigned short ExtendedIPCManager::getIpcProcessId() const{
	return ipcProcessId;
}

void ExtendedIPCManager::setIpcProcessId(unsigned short ipcProcessId){
	this->ipcProcessId = ipcProcessId;
}

void ExtendedIPCManager::setIPCManagerPort(
                unsigned int ipcManagerPort) {
        this->ipcManagerPort = ipcManagerPort;
}

void ExtendedIPCManager::notifyIPCProcessInitialized(
                const ApplicationProcessNamingInformation& name)
throw(IPCException){
        lock();
        if (ipcProcessInitialized) {
                unlock();
                throw IPCException("IPC Process already initialized");
        }

#if STUB_API
        //Do nothing
#else
        IpcmIPCProcessInitializedMessage message;
        message.setName(name);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestPortId(ipcManagerPort);
        message.setNotificationMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                unlock();
                throw IPCException(e.what());
        }
#endif
        ipcProcessInitialized = true;
        unlock();
}

bool ExtendedIPCManager::isIPCProcessInitialized() const {
        return ipcProcessInitialized;
}

ApplicationRegistration * ExtendedIPCManager::appRegistered(
                        const ApplicationProcessNamingInformation& appName,
                        const ApplicationProcessNamingInformation& DIFName)
throw (ApplicationRegistrationException) {
        ApplicationRegistration * applicationRegistration;

        lock();

        applicationRegistration = getApplicationRegistration(
                        appName);

        if (!applicationRegistration){
                applicationRegistration = new ApplicationRegistration(
                                appName);
                putApplicationRegistration(appName,
                                applicationRegistration);
        }

        applicationRegistration->addDIFName(DIFName);
        unlock();

        return applicationRegistration;
}

void ExtendedIPCManager::appUnregistered(
                const ApplicationProcessNamingInformation& appName,
                const ApplicationProcessNamingInformation& DIFName)
                throw (ApplicationUnregistrationException) {
        lock();
        ApplicationRegistration * applicationRegistration =
                        getApplicationRegistration(appName);
        if (!applicationRegistration){
                unlock();
                throw ApplicationUnregistrationException(
                                IPCManager::application_not_registered_error);
        }

        std::list<ApplicationProcessNamingInformation>::const_iterator iterator;
        for (iterator = applicationRegistration->getDIFNames() .begin();
                        iterator != applicationRegistration->getDIFNames().end();
                        ++iterator) {
                if (*iterator == DIFName) {
                        applicationRegistration->removeDIFName(DIFName);
                        if (applicationRegistration->getDIFNames().size() == 0) {
                                removeApplicationRegistration(appName);
                        }

                        break;
                }
        }

        unlock();
}

void ExtendedIPCManager::assignToDIFResponse(
		const AssignToDIFRequestEvent& event, int result)
	throw(AssignToDIFResponseException){
	if (result == 0){
		this->currentDIFInformation = event.getDIFInformation();
	}
#if STUB_API
	//Do nothing
#else
	IpcmAssignToDIFResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw AssignToDIFResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::enrollToDIFResponse(const EnrollToDIFRequestEvent& event,
                        int result, const std::list<Neighbor> & newNeighbors,
                        const DIFInformation& difInformation)
        throw (EnrollException) {
#if STUB_API
        //Do nothing
#else
        IpcmEnrollToDIFResponseMessage responseMessage;
        responseMessage.setResult(result);
        responseMessage.setNeighbors(newNeighbors);
        responseMessage.setDIFInformation(difInformation);
        responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
        responseMessage.setSequenceNumber(event.getSequenceNumber());
        responseMessage.setResponseMessage(true);
        try{
                rinaManager->sendMessage(&responseMessage);
        }catch(NetlinkException &e){
                throw EnrollException(e.what());
        }
#endif
}

void ExtendedIPCManager::notifyNeighborsModified(bool added,
                        const std::list<Neighbor> & neighbors)
        throw (EnrollException) {
#if STUB_API
        //Do nothing
#else
        IpcmNotifyNeighborsModifiedMessage message;
        message.setAdded(added);
        message.setNeighbors(neighbors);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestPortId(ipcManagerPort);
        message.setSequenceNumber(0);
        message.setNotificationMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw EnrollException(e.what());
        }
#endif
}

void ExtendedIPCManager::registerApplicationResponse(
		const ApplicationRegistrationRequestEvent& event, int result)
	throw(RegisterApplicationResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmRegisterApplicationResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw RegisterApplicationResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::unregisterApplicationResponse(
		const ApplicationUnregistrationRequestEvent& event, int result)
	throw(UnregisterApplicationResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmUnregisterApplicationResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw UnregisterApplicationResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::allocateFlowRequestResult(
		const FlowRequestEvent& event, int result)
	throw(AllocateFlowResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmAllocateFlowRequestResultMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setPortId(event.getPortId());
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw AllocateFlowResponseException(e.what());
	}
#endif
}

unsigned int ExtendedIPCManager::allocateFlowRequestArrived(
			const ApplicationProcessNamingInformation& localAppName,
			const ApplicationProcessNamingInformation& remoteAppName,
			const FlowSpecification& flowSpecification,
			int portId)
		throw (AllocateFlowRequestArrivedException){
#if STUP_API
	return 0;
#else
	IpcmAllocateFlowRequestArrivedMessage message;
	message.setSourceAppName(remoteAppName);
	message.setDestAppName(localAppName);
	message.setFlowSpecification(flowSpecification);
	message.setDifName(currentDIFInformation.getDifName());
	message.setPortId(portId);
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestPortId(ipcManagerPort);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw AllocateFlowRequestArrivedException(e.what());
	}

	return message.getSequenceNumber();
#endif
}

unsigned int ExtendedIPCManager::requestFlowAllocation(
                const ApplicationProcessNamingInformation& localAppName,
                const ApplicationProcessNamingInformation& remoteAppName,
                const FlowSpecification& flowSpec)
throw (FlowAllocationException) {
        return internalRequestFlowAllocation(
                        localAppName, remoteAppName, flowSpec, ipcProcessId);
}

unsigned int ExtendedIPCManager::requestFlowAllocationInDIF(
                const ApplicationProcessNamingInformation& localAppName,
                const ApplicationProcessNamingInformation& remoteAppName,
                const ApplicationProcessNamingInformation& difName,
                const FlowSpecification& flowSpec)
throw (FlowAllocationException) {
        return internalRequestFlowAllocationInDIF(localAppName,
                        remoteAppName, difName, ipcProcessId, flowSpec);
}

Flow * ExtendedIPCManager::allocateFlowResponse(
                const FlowRequestEvent& flowRequestEvent, int result,
                bool notifySource) throw (FlowAllocationException) {
        return internalAllocateFlowResponse(
                        flowRequestEvent, result, notifySource, ipcProcessId);
}

void ExtendedIPCManager::notifyflowDeallocated(
		const FlowDeallocateRequestEvent flowDeallocateEvent,
		int result)
	throw (DeallocateFlowResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmDeallocateFlowResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setSequenceNumber(flowDeallocateEvent.getSequenceNumber());
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw DeallocateFlowResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::flowDeallocatedRemotely(
		int portId, int code)
		throw (DeallocateFlowResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmFlowDeallocatedNotificationMessage message;
	message.setPortId(portId);
	message.setCode(code);
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestPortId(ipcManagerPort);
	message.setNotificationMessage(true);
	try{
		rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
		throw DeallocateFlowResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::queryRIBResponse(
		const QueryRIBRequestEvent& event, int result,
		const std::list<RIBObject>& ribObjects)
	throw(QueryRIBResponseException){
#if STUB_API
	//Do nothing
#else
	IpcmDIFQueryRIBResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setRIBObjects(ribObjects);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try{
	        //FIXME, compute maximum message size dynamically
		rinaManager->sendMessageOfMaxSize(&responseMessage,
		                5*PAGE_SIZE);
	}catch(NetlinkException &e){
		throw QueryRIBResponseException(e.what());
	}
#endif
}

int ExtendedIPCManager::allocatePortId(const ApplicationProcessNamingInformation& appName)
        throw (PortAllocationException) {
#if STUB_API
        //Do nothing
        return 1;
#else
        int result = syscallAllocatePortId(ipcProcessId, appName);
        if (result < 0){
                throw PortAllocationException();
        }

        return result;
#endif
}

void ExtendedIPCManager::deallocatePortId(int portId)
        throw (PortAllocationException) {
#if STUB_API
        //Do nothing
        return;
#else
        int result = syscallDeallocatePortId(portId);
        if (result < 0){
                throw PortAllocationException();
        }
#endif
}

Singleton<ExtendedIPCManager> extendedIPCManager;


/* CLASS CONNECTION */
ConnectionPoliciesParameters::ConnectionPoliciesParameters() {
        DTCPpresent = false;
        flowControl = false;
        RTXcontrol = false;
        windowBasedFlowControl = false;
        rateBasedFlowControl = false;
}

/*
ConnectionPoliciesParameters::ConnectionPoliciesParameters(bool DTCPpresent,
                bool flowControl,
                bool RTXcontrol,
                bool windowBasedFlowControl,
                bool rateBasedFlowControl) {
        this->DTCPpresent = DTCPpresent;
        this->flowControl = flowControl;
        this->RTXcontrol = RTXcontrol;
        this->windowBasedFlowControl = windowBasedFlowControl;
        this->rateBasedFlowControl = rateBasedFlowControl;
}*/

bool ConnectionPoliciesParameters::isDTCPpresent() const {
	return DTCPpresent;
}

void ConnectionPoliciesParameters::setDTCPpresent(bool DTCPpresent) {
	this->DTCPpresent = DTCPpresent;
}

bool ConnectionPoliciesParameters::isFlowControl() const {
	return flowControl;
}

void ConnectionPoliciesParameters::setFlowControl(bool flowControl) {
	this->flowControl = flowControl;
}

bool ConnectionPoliciesParameters::isRTXcontrol() const {
	return RTXcontrol;
}

void ConnectionPoliciesParameters::setRTXcontrol(bool RTXcontrol) {
	this->RTXcontrol = RTXcontrol;
}

bool ConnectionPoliciesParameters::isWindowBasedFlowControl() const {
	return windowBasedFlowControl;
}

void ConnectionPoliciesParameters::setWindowBasedFlowControl(bool windowBasedFlowControl) {
	this->windowBasedFlowControl = windowBasedFlowControl;
}

bool ConnectionPoliciesParameters::isRateBasedFlowControl() const {
	return rateBasedFlowControl;
}

void ConnectionPoliciesParameters::setRateBasedFlowControl(bool rateBasedFlowControl) {
	this->rateBasedFlowControl = rateBasedFlowControl;
}

/* CLASS CONNECTION */
Connection::Connection() {
        portId = 0;
        sourceAddress = 0;
        destAddress = 0;
        qosId = 0;
        sourceCepId = 0;
        destCepId = 0;
        flowUserIpcProcessId = 0;
}

unsigned int Connection::getDestAddress() const {
        return destAddress;
}

void Connection::setDestAddress(unsigned int destAddress) {
        this->destAddress = destAddress;
}

int Connection::getPortId() const {
        return portId;
}

void Connection::setPortId(int portId) {
        this->portId = portId;
}

unsigned int Connection::getQosId() const {
        return qosId;
}

void Connection::setQosId(unsigned int qosId) {
        this->qosId = qosId;
}

unsigned int Connection::getSourceAddress() const {
        return sourceAddress;
}

void Connection::setSourceAddress(unsigned int sourceAddress){
        this->sourceAddress = sourceAddress;
}

int Connection::getDestCepId() const {
        return destCepId;
}

void Connection::setDestCepId(int destCepId) {
        this->destCepId = destCepId;
}

unsigned short Connection::getFlowUserIpcProcessId() const {
        return flowUserIpcProcessId;
}

void Connection::setFlowUserIpcProcessId(unsigned short flowUserIpcProcessId) {
        this->flowUserIpcProcessId = flowUserIpcProcessId;
}

int Connection::getSourceCepId() const {
        return sourceCepId;
}

void Connection::setSourceCepId(int sourceCepId) {
        this->sourceCepId = sourceCepId;
}

const std::string Connection::toString() {
        std::stringstream ss;
        ss<<"Source address: "<<sourceAddress;
        ss<<"; Source cep-id: "<<sourceCepId;
        ss<<"; Dest address: "<<destAddress;
        ss<<"; Dest cep-id: "<<destCepId<<std::endl;
        ss<<"Por-id: "<<portId<<"; QoS-id: "<<qosId;
        ss<<"; Flow user IPC Process id: "<<flowUserIpcProcessId<<std::endl;
        return ss.str();
}

/* CLASS PDU FORWARDING TABLE ENTRY */
PDUForwardingTableEntry::PDUForwardingTableEntry() {
        address = 0;
        qosId = 0;
}

bool PDUForwardingTableEntry::operator==(
                const PDUForwardingTableEntry &other) const {
        if (address != other.getAddress()) {
                return false;
        }

        if (qosId != other.getQosId()) {
                return false;
        }

        return true;
}

bool PDUForwardingTableEntry::operator!=(
                const PDUForwardingTableEntry &other) const {
        return !(*this == other);
}

unsigned int PDUForwardingTableEntry::getAddress() const {
        return address;
}

void PDUForwardingTableEntry::setAddress(unsigned int address) {
        this->address = address;
}

const std::list<unsigned int> PDUForwardingTableEntry::getPortIds() const {
        return portIds;
}

void PDUForwardingTableEntry::
setPortIds(const std::list<unsigned int>& portIds){
        this->portIds = portIds;
}

void PDUForwardingTableEntry::addPortId(unsigned int portId) {
        portIds.push_back(portId);
}

unsigned int PDUForwardingTableEntry::getQosId() const {
        return qosId;
}

void PDUForwardingTableEntry::setQosId(unsigned int qosId) {
        this->qosId = qosId;
}

const std::string PDUForwardingTableEntry::toString() {
        std::stringstream ss;

        ss<<"Address: "<<address<<" QoS-id: "<<qosId;
        ss<<"List of N-1 port-ids: ";
        for (std::list<unsigned int>::iterator it = portIds.begin();
                        it != portIds.end(); it++)
                ss<< *it << "; ";
        ss<<std::endl;

        return ss.str();
}

/* CLASS READ MANAGEMENT SDU RESULT */
ReadManagementSDUResult::ReadManagementSDUResult() {
        bytesRead = 0;
        portId = 0;
}

int ReadManagementSDUResult::getBytesRead() const {
        return bytesRead;
}

void ReadManagementSDUResult::setBytesRead(int bytesRead) {
        this->bytesRead = bytesRead;
}

int ReadManagementSDUResult::getPortId() const {
        return portId;
}

void ReadManagementSDUResult::setPortId(int portId) {
        this->portId = portId;
}

/* CLASS KERNEL IPC PROCESS */
void KernelIPCProcess::setIPCProcessId(unsigned short ipcProcessId) {
        this->ipcProcessId = ipcProcessId;
}

unsigned short KernelIPCProcess::getIPCProcessId() const {
        return ipcProcessId;
}

unsigned int KernelIPCProcess::assignToDIF(
                const DIFInformation& difInformation)
throw (AssignToDIFException) {
        unsigned int seqNum = 0;

#if STUB_API
        //Do nothing
#else
        IpcmAssignToDIFRequestMessage message;
        message.setDIFInformation(difInformation);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw AssignToDIFException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif
        return seqNum;
}

unsigned int KernelIPCProcess::updateDIFConfiguration(
                const DIFConfiguration& difConfiguration)
throw (UpdateDIFConfigurationException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcmUpdateDIFConfigurationRequestMessage message;
        message.setDIFConfiguration(difConfiguration);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw UpdateDIFConfigurationException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::createConnection(const Connection& connection)
throw (CreateConnectionException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcpConnectionCreateRequestMessage message;
        message.setPortId(connection.getPortId());
        message.setSourceAddress(connection.getSourceAddress());
        message.setDestAddress(connection.getDestAddress());
        message.setQosId(connection.getQosId());
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw CreateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::updateConnection(const Connection& connection)
throw (UpdateConnectionException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcpConnectionUpdateRequestMessage message;
        message.setPortId(connection.getPortId());
        message.setSourceCepId(connection.getSourceCepId());
        message.setDestinationCepId(connection.getDestCepId());
        message.setFlowUserIpcProcessId(connection.getFlowUserIpcProcessId());
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw UpdateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::
createConnectionArrived(const Connection& connection)
throw (CreateConnectionException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcpConnectionCreateArrivedMessage message;
        message.setPortId(connection.getPortId());
        message.setSourceAddress(connection.getSourceAddress());
        message.setDestAddress(connection.getDestAddress());
        message.setQosId(connection.getQosId());
        message.setDestCepId(connection.getDestCepId());
        message.setFlowUserIpcProcessId(connection.getFlowUserIpcProcessId());
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw CreateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::
destroyConnection(const Connection& connection)
        throw (DestroyConnectionException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcpConnectionDestroyRequestMessage message;
        message.setPortId(connection.getPortId());
        message.setCepId(connection.getSourceCepId());
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw DestroyConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

void KernelIPCProcess::
modifyPDUForwardingTableEntries(const std::list<PDUForwardingTableEntry>& entries,
                        int mode) throw (PDUForwardingTableException) {
#if STUB_API
        //Do nothing
#else
        RmtModifyPDUFTEntriesRequestMessage message;
        message.setEntries(entries);
        message.setMode(mode);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw PDUForwardingTableException(e.what());
        }
#endif
}

unsigned int KernelIPCProcess::dumptPDUFT()
        throw (PDUForwardingTableException) {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        RmtDumpPDUFTEntriesRequestMessage message;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw PDUForwardingTableException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif

        return seqNum;
}

void KernelIPCProcess::writeManagementSDU(void * sdu, int size, int portId)
                throw (WriteSDUException) {
#if STUB_API
        //Do nothing
#else
        int result = syscallWriteManagementSDU(ipcProcessId, sdu, portId,
                        size);
        if (result < 0){
                throw WriteSDUException();
        }
#endif
}

ReadManagementSDUResult KernelIPCProcess::readManagementSDU(void * sdu,
                int maxBytes)
                throw (ReadSDUException) {
        ReadManagementSDUResult readResult;

#if STUB_API
        unsigned char buffer[] = { 0, 23, 43, 32, 45, 23, 78 };
        sdu = buffer;
        readResult.setPortId(14);
        readResult.setBytesRead(7);
        return readResult;
#else
        int portId = 0;
        int result = syscallReadManagementSDU(ipcProcessId, sdu, &portId,
                        maxBytes);
        if (result < 0){
                throw ReadSDUException();
        }

        readResult.setPortId(portId);
        readResult.setBytesRead(result);
        return readResult;
#endif
}

/*	CLASS ADataUnitPDU	*/
const std::string ADataUnitPDU::ADataUnitPDUObjectName = "/daf/adataunitpdu";

ADataUnitPDU::ADataUnitPDU()
{
	this->sourceAddress = 0;
	this->destinationAddress = 0;
	this->payload = 0;
}

ADataUnitPDU::ADataUnitPDU(long sourceAddress, long destinationAddress, char payload[])
{
	this->sourceAddress = sourceAddress;
	this->destinationAddress = destinationAddress;
	this->payload = payload;
}

long ADataUnitPDU::getSourceAddress() const
{
	return sourceAddress;
}

void ADataUnitPDU::setSourceAddress(const long sourceAddress)
{
	this->sourceAddress = sourceAddress;
}

long ADataUnitPDU::getDestinationAddress() const
{
	return destinationAddress;
}

void ADataUnitPDU::setDestinationAddress(const long destinationAddress)
{
	this->destinationAddress = destinationAddress;
}

char* ADataUnitPDU::getPayload() const
{
	return payload;
}

void ADataUnitPDU::setPayload(char payload[])
{
	this->payload = payload;
}

/*	CLASS RIBObjectNames	*/
const std::string RIBObjectNames::ADDRESS = "address";
const std::string RIBObjectNames::APNAME = "applicationprocessname";
const std::string RIBObjectNames::CONSTANTS = "constants";
const std::string RIBObjectNames::DATA_TRANSFER = "datatransfer";
const std::string RIBObjectNames::DAF = "daf";
const std::string RIBObjectNames::DIF = "dif";
const std::string RIBObjectNames::DIF_REGISTRATIONS = "difregistrations";
const std::string RIBObjectNames::DIRECTORY_FORWARDING_TABLE_ENTRIES = "directoryforwardingtableentries";
const std::string RIBObjectNames::ENROLLMENT = "enrollment";
const std::string RIBObjectNames::FLOWS = "flows";
const std::string RIBObjectNames::FLOW_ALLOCATOR = "flowallocator";
const std::string RIBObjectNames::IPC = "ipc";
const std::string RIBObjectNames::MANAGEMENT = "management";
const std::string RIBObjectNames::NEIGHBORS = "neighbors";
const std::string RIBObjectNames::NAMING = "naming";
const std::string RIBObjectNames::NMINUSONEFLOWMANAGER = "nminusoneflowmanager";
const std::string RIBObjectNames::NMINUSEONEFLOWS = "nminusoneflows";
const std::string RIBObjectNames::OPERATIONAL_STATUS = "operationalStatus";
const std::string RIBObjectNames::PDU_FORWARDING_TABLE = "pduforwardingtable";
const std::string RIBObjectNames::QOS_CUBES = "qoscubes";
const std::string RIBObjectNames::RESOURCE_ALLOCATION = "resourceallocation";
const std::string RIBObjectNames::ROOT = "root";
const std::string RIBObjectNames::SEPARATOR = "/";
const std::string RIBObjectNames::SYNONYMS = "synonyms";
const std::string RIBObjectNames::WHATEVERCAST_NAMES = "whatevercastnames";
const std::string RIBObjectNames::ROUTING = "routing";
const std::string RIBObjectNames::FLOWSTATEOBJECTGROUP = "flowstateobjectgroup";
const std::string RIBObjectNames::OPERATIONAL_STATUS_RIB_OBJECT_NAME = RIBObjectNames::SEPARATOR + RIBObjectNames::DAF +
			RIBObjectNames::SEPARATOR + RIBObjectNames::MANAGEMENT + RIBObjectNames::SEPARATOR + RIBObjectNames::OPERATIONAL_STATUS;

const std::string RIBObjectNames::OPERATIONAL_STATUS_RIB_OBJECT_CLASS = "operationstatus";

const std::string RIBObjectNames::PDU_FORWARDING_TABLE_RIB_OBJECT_CLASS = "pdu forwarding table";
const std::string RIBObjectNames::PDU_FORWARDING_TABLE_RIB_OBJECT_NAME = RIBObjectNames::SEPARATOR + RIBObjectNames::DIF +
			RIBObjectNames::SEPARATOR + RIBObjectNames::RESOURCE_ALLOCATION + RIBObjectNames::SEPARATOR + RIBObjectNames::PDU_FORWARDING_TABLE;

/*	CLASS EnrollmentInformationRequest	*/
const std::string EnrollmentInformationRequest::ENROLLMENT_INFO_OBJECT_NAME = RIBObjectNames::SEPARATOR + RIBObjectNames::DAF +
			RIBObjectNames::SEPARATOR + RIBObjectNames::MANAGEMENT + RIBObjectNames::SEPARATOR + RIBObjectNames::ENROLLMENT;

EnrollmentInformationRequest::EnrollmentInformationRequest(){
	this->address = 0L;
	this->supportingDifs = NULL;
}

unsigned int EnrollmentInformationRequest::getAddress() const {
	return address;
}

void EnrollmentInformationRequest::setAddress(unsigned int address) {
	this->address = address;
}

std::list<ApplicationProcessNamingInformation>* EnrollmentInformationRequest::getSupportingDifs() const {
	return supportingDifs;
}

void EnrollmentInformationRequest::setSupportingDifs(std::list<ApplicationProcessNamingInformation> &supportingDifs) {
	this->supportingDifs = &supportingDifs;
}

/*	CLASS EnrollmentRequest	*/
EnrollmentRequest::EnrollmentRequest(Neighbor &neighbor, EnrollToDIFRequestEvent &event){
	this->neighbor = &neighbor;
	this->event = &event;
	this->neighbor = NULL;
	this->event = NULL;
}

Neighbor* EnrollmentRequest::getNeighbor() const{
	return neighbor;
}

void EnrollmentRequest::setNeighbor(Neighbor &neighbor) {
	this->neighbor = &neighbor;
}

EnrollToDIFRequestEvent* EnrollmentRequest::getEvent() const{
	return event;
}

void EnrollmentRequest::setEvent(EnrollToDIFRequestEvent &event) {
	this->event = &event;
}

/*	CLASS Event	*/
const std::string CONNECTIVITY_TO_NEIGHBOR_LOST = "Connectivity to Neighbor Lost";
const std::string EFCP_CONNECTION_CREATED = "EFCP Connection Created";
const std::string EFCP_CONNECTION_DELETED = "EFCP Connection Deleted";
const std::string MANAGEMENT_FLOW_ALLOCATED = "Management Flow Allocated";
const std::string MANAGEMENT_FLOW_DEALLOCATED = "Management Flow Deallocated";
const std::string N_MINUS_1_FLOW_ALLOCATED = "N minus 1 Flow Allocated";
const std::string N_MINUS_1_FLOW_ALLOCATION_FAILED = "N minus 1 Flow Allocation Failed";
const std::string N_MINUS_1_FLOW_DEALLOCATED = "N minus 1 Flow Deallocated";
const std::string NEIGHBOR_DECLARED_DEAD = "Neighbor declared dead";
const std::string NEIGHBOR_ADDED = "Neighbor added";

/*	CLASS BaseEvent	*/
BaseEvent::BaseEvent()
{
}

BaseEvent::BaseEvent(std::string id)
{
	this->id = id;
}

std::string BaseEvent::getId() const
{
	return id;
}

Singleton<KernelIPCProcess> kernelIPCProcess;
}
