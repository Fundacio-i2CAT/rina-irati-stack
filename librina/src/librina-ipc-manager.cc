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

#define RINA_PREFIX "ipc-manager"

#include <algorithm>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "logs.h"
#include "librina-ipc-manager.h"
#include "core.h"
#include "concurrency.h"
#include "rina-syscalls.h"

namespace rina{

std::string _installationPath;
std::string _libraryPath;

void initializeIPCManager(unsigned int localPort,
                const std::string& installationPath,
                const std::string& libraryPath)
	throw (IPCManagerInitializationException){
	initialize(localPort);

	_installationPath = installationPath;
	_libraryPath = libraryPath;

	IpcmIPCManagerPresentMessage message;
	message.setDestPortId(0);
	message.setNotificationMessage(true);

	try{
		rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
		throw IPCManagerInitializationException(e.what());
	}
}

/* CLASS IPC PROCESS*/
const std::string IPCProcess::error_assigning_to_dif =
		"Error assigning IPC Process to DIF";
const std::string IPCProcess::error_update_dif_config =
                "Error updating DIF Configuration";
const std::string IPCProcess::error_registering_app =
		"Error registering application";
const std::string IPCProcess::error_unregistering_app =
		"Error unregistering application";
const std::string IPCProcess::error_not_a_dif_member =
		"Error: the IPC Process is not member of a DIF";
const std::string IPCProcess::error_allocating_flow =
		"Error allocating flow";
const std::string IPCProcess::error_deallocating_flow =
		"Error deallocating flow";
const std::string IPCProcess::error_querying_rib =
		"Error querying rib";

/** Return the information of a registration request */
ApplicationProcessNamingInformation IPCProcess::getPendingRegistration(
                unsigned int seqNumber) throw (IPCException) {
        std::map<unsigned int, ApplicationProcessNamingInformation>::iterator iterator;

        iterator = pendingRegistrations.find(seqNumber);
        if (iterator == pendingRegistrations.end()) {
                throw IPCException("Could not find pending registration");
        }

        return iterator->second;
}

FlowInformation IPCProcess::getPendingFlowOperation(unsigned int seqNumber)
throw (IPCException) {
	std::map<unsigned int, FlowInformation>::iterator iterator;

	iterator = pendingFlowOperations.find(seqNumber);
	if (iterator == pendingFlowOperations.end()) {
		throw IPCException("Could not find pending flow operation");
	}

	return iterator->second;
}

IPCProcess::IPCProcess() {
	id = 0;
	portId = 0;
	pid = 0;
	difMember = false;
	assignInProcess = false;
	configureInProcess = false;
	initialized = false;
}

IPCProcess::IPCProcess(unsigned short id, unsigned int portId,
		pid_t pid, const std::string& type,
		const ApplicationProcessNamingInformation& name) {
	this->id = id;
	this->portId = portId;
	this->pid = pid;
	this->type = type;
	this->name = name;
	initialized = false;
	difMember = false;
	assignInProcess = false;
	configureInProcess = false;
}

bool IPCProcess::isDIFMember() const{
	return difMember;
}

void IPCProcess::setDIFMember(bool difMember){
	this->difMember = difMember;
}

unsigned int IPCProcess::getId() const {
	return id;
}

const std::string& IPCProcess::getType() const {
	return type;
}

const ApplicationProcessNamingInformation& IPCProcess::getName() const {
	return name;
}

unsigned int IPCProcess::getPortId() const{
	return portId;
}

void IPCProcess::setPortId(unsigned int portId){
	this->portId = portId;
}

pid_t IPCProcess::getPid() const{
	return pid;
}

void IPCProcess::setPid(pid_t pid){
	this->pid = pid;
}

const DIFInformation& IPCProcess::getDIFInformation() const{
	return difInformation;
}

void IPCProcess::setDIFInformation(const DIFInformation& difInformation){
	this->difInformation = difInformation;
}

void IPCProcess::setInitialized() {
        initialized = true;
}

unsigned int IPCProcess::assignToDIF(
                const DIFInformation& difInformation)
throw (AssignToDIFException) {
        unsigned int seqNum = 0;

        if (!initialized)
                throw AssignToDIFException("IPC Process not yet initialized");

        std::string currentDIFName =
                        this->difInformation.getDifName().getProcessName();
        LOG_DBG("Current DIF name is %s", currentDIFName.c_str());

        if(difMember || assignInProcess) {
                std::string message;
                message =  message + "This IPC Process is already assigned "+
                                "to the DIF " + currentDIFName;
                LOG_ERR("%s", message.c_str());
                throw AssignToDIFException(message);
        }

#if STUB_API
        //Do nothing
#else
	IpcmAssignToDIFRequestMessage message;
	message.setDIFInformation(difInformation);
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw AssignToDIFException(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	this->difInformation = difInformation;
	assignInProcess = true;
	return seqNum;
}

void IPCProcess::assignToDIFResult(bool success) throw (AssignToDIFException) {
        if (!assignInProcess) {
                throw AssignToDIFException(
                                "There was no assignment operation in process");
        }

        if (!success) {
                ApplicationProcessNamingInformation noDIF;
                difInformation.setDifName(noDIF);
                DIFConfiguration noConfig;
                difInformation.setDifConfiguration(noConfig);
        } else {
                difMember = true;
        }

        assignInProcess = false;
}

unsigned int IPCProcess::updateDIFConfiguration(
                        const DIFConfiguration& difConfiguration)
        throw (UpdateDIFConfigurationException)
{
        unsigned int seqNum=0;

        std::string currentDIFName =
                        this->difInformation.getDifName().getProcessName();
        LOG_DBG("Current DIF name is %s", currentDIFName.c_str());

        if(!difMember || configureInProcess) {
                std::string message;
                message =  message + "This IPC Process is not yet assigned "+
                                "to any DIF, or a DIF configuration " +
                                "operation is ongoing";
                LOG_ERR("%s", message.c_str());
                throw UpdateDIFConfigurationException(message);
        }

#if STUB_API
        //Do nothing
#else
        IpcmUpdateDIFConfigurationRequestMessage message;
        message.setDIFConfiguration(difConfiguration);
        message.setDestIpcProcessId(id);
        message.setDestPortId(portId);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
                throw UpdateDIFConfigurationException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        configureInProcess = true;
        newConfiguration = difConfiguration;

        return seqNum;
}

void IPCProcess::updateDIFConfigurationResult(bool success)
        throw (UpdateDIFConfigurationException) {

        if(!configureInProcess){
                throw UpdateDIFConfigurationException(
                                "No config operation in process");
        }

        if (success){
                difInformation.setDifConfiguration(newConfiguration);
        }

        newConfiguration = DIFConfiguration();
        configureInProcess = false;
}

void IPCProcess::notifyRegistrationToSupportingDIF(
		const ApplicationProcessNamingInformation& ipcProcessName,
		const ApplicationProcessNamingInformation& difName)
throw (NotifyRegistrationToDIFException) {
        std::list<ApplicationProcessNamingInformation>::iterator it =
                        std::find(nMinusOneDIFs.begin(),
                                  nMinusOneDIFs.end(), difName);
        if (it != nMinusOneDIFs.end()) {
                throw NotifyRegistrationToDIFException(
                                "IPCProcess already registered to N-1 DIF"
                                + difName.getProcessName());
        }

#if STUB_API
	//Do nothing
#else
	IpcmDIFRegistrationNotification message;
	message.setIpcProcessName(ipcProcessName);
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setDifName(difName);
	message.setRegistered(true);
	message.setNotificationMessage(true);

	try{
		rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
		throw NotifyRegistrationToDIFException(e.what());
	}
#endif
	nMinusOneDIFs.push_back(difName);
}

void IPCProcess::notifyUnregistrationFromSupportingDIF(
		const ApplicationProcessNamingInformation& ipcProcessName,
		const ApplicationProcessNamingInformation& difName)
throw (NotifyUnregistrationFromDIFException) {
#if STUB_API
	//Do nothing
#else
	IpcmDIFRegistrationNotification message;
	message.setIpcProcessName(ipcProcessName);
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setDifName(difName);
	message.setRegistered(false);
	message.setNotificationMessage(true);

	try{
		rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
		throw NotifyUnregistrationFromDIFException(e.what());
	}
#endif
}

void IPCProcess::enroll(const ApplicationProcessNamingInformation& difName,
		const ApplicationProcessNamingInformation& supportinDifName)
throw (EnrollException) {
	LOG_DBG("IPCProcess::enroll called");
	throw IPCException(IPCException::operation_not_implemented_error);
}

void IPCProcess::disconnectFromNeighbor(
		const ApplicationProcessNamingInformation& neighbor)
throw (DisconnectFromNeighborException) {
	LOG_DBG("IPCProcess::disconnect from neighbour called");
	throw IPCException(IPCException::operation_not_implemented_error);
}

unsigned int IPCProcess::registerApplication(
		const ApplicationProcessNamingInformation& applicationName)
throw (IpcmRegisterApplicationException) {
	if (!difMember){
		throw IpcmRegisterApplicationException(
		                IPCProcess::error_not_a_dif_member);
	}

	unsigned int seqNum = 0;

#if STUB_API
	//Do nothing
#else
	IpcmRegisterApplicationRequestMessage message;
	message.setApplicationName(applicationName);
	message.setDifName(difInformation.getDifName());
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw IpcmRegisterApplicationException(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif
	pendingRegistrations[seqNum] = applicationName;
	return seqNum;
}

void IPCProcess::registerApplicationResult(
                unsigned int sequenceNumber, bool success)
throw (IpcmRegisterApplicationException) {
        if (!difMember){
                throw IpcmRegisterApplicationException(
                                IPCProcess::error_not_a_dif_member);
        }

        ApplicationProcessNamingInformation appName;
        try {
                appName = getPendingRegistration(sequenceNumber);
        } catch(IPCException &e){
                throw IpcmRegisterApplicationException(e.what());
        }

        pendingRegistrations.erase(sequenceNumber);
        if (success) {
                registeredApplications.push_back(appName);
        }
}

std::list<ApplicationProcessNamingInformation>
        IPCProcess::getRegisteredApplications() {
        return registeredApplications;
}

unsigned int IPCProcess::unregisterApplication(
		const ApplicationProcessNamingInformation& applicationName)
throw (IpcmUnregisterApplicationException) {
        if (!difMember){
                throw IpcmUnregisterApplicationException(
                                IPCProcess::error_not_a_dif_member);
        }

        bool found = false;
        std::list<ApplicationProcessNamingInformation>::iterator iterator;
        for (iterator = registeredApplications.begin();
                        iterator != registeredApplications.end();
                        iterator++) {
              if (*iterator == applicationName){
                      found = true;
                      break;
              }
        }

        if (!found)
                throw IpcmUnregisterApplicationException(
                                "The application is not registered");

        unsigned int seqNum = 0;

#if STUB_API
	//Do nothing
#else
        IpcmUnregisterApplicationRequestMessage message;
        message.setApplicationName(applicationName);
        message.setDifName(difInformation.getDifName());
        message.setDestIpcProcessId(id);
        message.setDestPortId(portId);
        message.setRequestMessage(true);

        try{
        	rinaManager->sendMessage(&message);
        }catch(NetlinkException &e){
        	LOG_DBG("Error %s", e.what());
        	throw IpcmUnregisterApplicationException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif
        pendingRegistrations[seqNum] = applicationName;
        return seqNum;
}

void IPCProcess::unregisterApplicationResult(unsigned int sequenceNumber, bool success)
throw (IpcmUnregisterApplicationException) {
        if (!difMember){
                throw IpcmRegisterApplicationException(
                                IPCProcess::error_not_a_dif_member);
        }

        ApplicationProcessNamingInformation appName;
        try {
                appName = getPendingRegistration(sequenceNumber);
        } catch(IPCException &e){
                throw IpcmRegisterApplicationException(e.what());
        }

        pendingRegistrations.erase(sequenceNumber);

        if (success) {
                registeredApplications.remove(appName);
        }
}

unsigned int IPCProcess::allocateFlow(const FlowRequestEvent& flowRequest)
throw (AllocateFlowException) {
	if (!difMember){
		throw AllocateFlowException(IPCProcess::error_not_a_dif_member);
	}

	unsigned int seqNum = 0;

#if STUB_API
	//Do nothing
#else
	IpcmAllocateFlowRequestMessage message;
	message.setSourceAppName(flowRequest.getLocalApplicationName());
	message.setDestAppName(flowRequest.getRemoteApplicationName());
	message.setFlowSpec(flowRequest.getFlowSpecification());
	message.setDifName(flowRequest.getDIFName());
	message.setPortId(flowRequest.getPortId());
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw AllocateFlowException(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	FlowInformation flowInformation;
	flowInformation.setLocalAppName(flowRequest.getLocalApplicationName());
	flowInformation.setRemoteAppName(flowRequest.getRemoteApplicationName());
	flowInformation.setDifName(difInformation.getDifName());
	flowInformation.setFlowSpecification(flowRequest.getFlowSpecification());
	flowInformation.setPortId(flowRequest.getPortId());

	pendingFlowOperations[seqNum] = flowInformation;

	return seqNum;
}

void IPCProcess::allocateFlowResult(unsigned int sequenceNumber, bool success)
	throw (AllocateFlowException) {
	if (!difMember){
		throw AllocateFlowException(
				IPCProcess::error_not_a_dif_member);
	}

	FlowInformation flowInformation;
	try {
		flowInformation = getPendingFlowOperation(sequenceNumber);
	} catch(IPCException &e){
		throw AllocateFlowException(e.what());
	}

	pendingFlowOperations.erase(sequenceNumber);
	if (success) {
		allocatedFlows.push_back(flowInformation);
	}
}

void IPCProcess::allocateFlowResponse(const FlowRequestEvent& flowRequest,
		int result, bool notifySource)
		throw(AllocateFlowException){

	if (result == 0) {
		FlowInformation flowInformation;
		flowInformation.setLocalAppName(flowRequest.getLocalApplicationName());
		flowInformation.setRemoteAppName(flowRequest.getRemoteApplicationName());
		flowInformation.setDifName(difInformation.getDifName());
		flowInformation.setFlowSpecification(flowRequest.getFlowSpecification());
		flowInformation.setPortId(flowRequest.getPortId());

		allocatedFlows.push_back(flowInformation);
	}

#if STUB_API
	//Do nothing
#else
	IpcmAllocateFlowResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setPortId(flowRequest.getPortId());
	responseMessage.setNotifySource(notifySource);
	responseMessage.setDestIpcProcessId(id);
	responseMessage.setDestPortId(portId);
	responseMessage.setSequenceNumber(flowRequest.getSequenceNumber());
	responseMessage.setResponseMessage(true);

	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw AllocateFlowException(e.what());
	}
#endif

}

std::list<FlowInformation> IPCProcess::getAllocatedFlows() {
	return allocatedFlows;
}

FlowInformation IPCProcess::getFlowInformation(int flowPortId)
throw(IPCException) {
	std::list<FlowInformation>::const_iterator iterator;
	for (iterator = allocatedFlows.begin();
			iterator != allocatedFlows.end(); ++iterator) {
	    if (iterator->getPortId() == flowPortId)
	    	return *iterator;
	}

	throw IPCException("Unknown flow");
}

unsigned int IPCProcess::deallocateFlow(int flowPortId)
	throw (IpcmDeallocateFlowException){
	unsigned int seqNum = 0;
	FlowInformation flowInformation;

	try{
		flowInformation = getFlowInformation(flowPortId);
	}catch (IPCException &e) {
		LOG_ERR("Could not find flow with port-id %d", flowPortId);
		throw IpcmDeallocateFlowException("Unknown flow");
	}

#if STUB_API
	//Do nothing
#else
	IpcmDeallocateFlowRequestMessage message;
	message.setPortId(flowPortId);
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw IpcmDeallocateFlowException(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	pendingFlowOperations[seqNum] = flowInformation;
	return seqNum;
}

void IPCProcess::deallocateFlowResult(unsigned int sequenceNumber, bool success)
	throw (IpcmDeallocateFlowException) {
	FlowInformation flowInformation;

	try {
		flowInformation = getPendingFlowOperation(sequenceNumber);
	} catch(IPCException &e){
		throw IpcmDeallocateFlowException(e.what());
	}

	pendingFlowOperations.erase(sequenceNumber);
	if (success) {
		allocatedFlows.remove(flowInformation);
	}
}

FlowInformation IPCProcess::flowDeallocated(int flowPortId)
throw (IpcmDeallocateFlowException) {
	FlowInformation flowInformation;

	try {
		flowInformation = getFlowInformation(flowPortId);
		allocatedFlows.remove(flowInformation);
		return flowInformation;
	} catch (IPCException &e) {
		throw IpcmDeallocateFlowException(e.what());
	}
}

unsigned int IPCProcess::queryRIB(const std::string& objectClass,
		const std::string& objectName, unsigned long objectInstance,
		unsigned int scope, const std::string& filter)
			throw (QueryRIBException){
#if STUB_API
	return 0;
#else
	IpcmDIFQueryRIBRequestMessage message;
	message.setObjectClass(objectClass);
	message.setObjectName(objectName);
	message.setObjectInstance(objectInstance);
	message.setScope(scope);
	message.setFilter(filter);
	message.setDestIpcProcessId(id);
	message.setDestPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw QueryRIBException(e.what());
	}

	return message.getSequenceNumber();
#endif
}

/** CLASS IPC PROCESS FACTORY */
const std::string IPCProcessFactory::unknown_ipc_process_error =
		"Could not find an IPC Process with the provided id";
const std::string IPCProcessFactory::path_to_ipc_process_types =
		"/sys/rina/personalities/default/ipcp-factories/";
const std::string IPCProcessFactory::normal_ipc_process_type =
		"normal";

IPCProcessFactory::IPCProcessFactory(): Lockable(){
}

IPCProcessFactory::~IPCProcessFactory() throw(){
}

std::list<std::string> IPCProcessFactory::getSupportedIPCProcessTypes(){
	std::list<std::string> result;

	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(path_to_ipc_process_types.c_str())) == 0) {
		LOG_ERR("Error %d opening %s", errno,
				path_to_ipc_process_types.c_str());
		return result;
	}

	std::string name;
	while ((dirp = readdir(dp)) != 0) {
		name = std::string(dirp->d_name);
		if (name.compare(".") != 0 && name.compare("..") != 0){
			result.push_back(name);
		}
	}

	closedir(dp);

	return result;
}

IPCProcess * IPCProcessFactory::create(
		const ApplicationProcessNamingInformation& ipcProcessName,
		const std::string& difType) throw (CreateIPCProcessException) {
	lock();
	int ipcProcessId = 1;
	unsigned int portId = 0;
	pid_t pid=0;
	for (int i = 1; i < 1000; i++) {
		if (ipcProcesses.find(i) == ipcProcesses.end()) {
			ipcProcessId = i;
			break;
		}
	}

#if STUB_API
	//Do nothing
#else
	int result = syscallCreateIPCProcess(
	                ipcProcessName, ipcProcessId, difType);
	if (result != 0)
	{
	        unlock();
	        throw CreateIPCProcessException();
	}

	if (difType.compare(NORMAL_IPC_PROCESS) == 0){
		pid = fork();
		if (pid == 0){
			//This is the OS process that has to execute the IPC Process
			//program and then exit
			LOG_DBG("New OS Process created, executing IPC Process ...");

			char * argv[] =
			{
                                /* FIXME: These hardwired things must disappear */
				stringToCharArray("/usr/bin/java"),
				stringToCharArray("-jar"),
				stringToCharArray(_installationPath +
					          "/ipcprocess/rina.ipcprocess.impl-1.0.0-irati-SNAPSHOT.jar"),
				stringToCharArray(ipcProcessName.getProcessName()),
				stringToCharArray(ipcProcessName.getProcessInstance()),
				intToCharArray(ipcProcessId),
				intToCharArray(getNelinkPortId()),
				0
			};

			char * envp[] =
			{
                                /* FIXME: These hardwired things must disappear */
				stringToCharArray("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"),
				stringToCharArray("LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"
					          +_libraryPath),
				(char*) 0
			};

			execve(argv[0], &argv[0], envp);

			LOG_ERR("Problems loading IPC Process program, finalizing OS Process");

			//Destroy the IPC Process in the kernel
			syscallDestroyIPCProcess(ipcProcessId);

			exit(-1);
		}else if (pid < 0){
			//This is the IPC Manager, and fork failed
		        //Try to destroy the IPC Process in the kernel and return error
		        syscallDestroyIPCProcess(ipcProcessId);

			unlock();
			throw CreateIPCProcessException();
		}else{
			//This is the IPC Manager, and fork was successful
		        portId = pid;
			LOG_DBG("Craeted a new IPC Process with pid = %d", pid);
		}
	}
#endif

	IPCProcess * ipcProcess = new IPCProcess(ipcProcessId, portId, pid, difType,
			ipcProcessName);
	ipcProcesses[ipcProcessId] = ipcProcess;
	unlock();

	return ipcProcess;
}

void IPCProcessFactory::destroy(unsigned int ipcProcessId)
throw (DestroyIPCProcessException) {
	lock();

	int resultUserSpace = 0;
	int resultKernel = 0;
	std::map<int, IPCProcess*>::iterator iterator;
	iterator = ipcProcesses.find(ipcProcessId);
	if (iterator == ipcProcesses.end())
	{
		unlock();
		throw DestroyIPCProcessException(
		                IPCProcessFactory::unknown_ipc_process_error);
	}

#if STUB_API
	//Do nothing
#else
	IPCProcess * ipcProcess = iterator->second;

	resultKernel = syscallDestroyIPCProcess(ipcProcessId);

	if (ipcProcess->getType().compare(NORMAL_IPC_PROCESS) == 0)
		resultUserSpace = kill(ipcProcess->getPid(), SIGKILL);
#endif

	delete iterator->second;
	ipcProcesses.erase(ipcProcessId);

	unlock();

	if (resultKernel || resultUserSpace)
	{
	        std::string error = "Problems destroying IPCP.";
	        error = error + "Result in the kernel: " +
	                        intToCharArray(resultKernel);
	        error = error + "Result in user space:  " +
	                        intToCharArray(resultUserSpace);
	        LOG_ERR("%s", error.c_str());
	        throw DestroyIPCProcessException(error);
	}
}

std::vector<IPCProcess *> IPCProcessFactory::listIPCProcesses() {
	std::vector<IPCProcess *> response;

	lock();
	for (std::map<int, IPCProcess*>::iterator it = ipcProcesses.begin();
			it != ipcProcesses.end(); ++it) {
		response.push_back(it->second);
	}
	unlock();

	return response;
}

IPCProcess * IPCProcessFactory::getIPCProcess(unsigned int ipcProcessId)
        throw (GetIPCProcessException)
{
        std::map<int, IPCProcess*>::iterator iterator;

        lock();
        iterator = ipcProcesses.find(ipcProcessId);
        unlock();

        if (iterator == ipcProcesses.end())
                throw GetIPCProcessException();

        return iterator->second;
}

Singleton<IPCProcessFactory> ipcProcessFactory;

/** CLASS APPLICATION MANAGER */
void ApplicationManager::applicationRegistered(
		const ApplicationRegistrationRequestEvent& event,
		const ApplicationProcessNamingInformation& difName, int result)
			throw (NotifyApplicationRegisteredException) {
	LOG_DBG("ApplicationManager::applicationRegistered called");

#if STUB_API
	//Do nothing
#else
	AppRegisterApplicationResponseMessage responseMessage;
	responseMessage.setApplicationName(event.
	                getApplicationRegistrationInformation().
	                getApplicationName());
	responseMessage.setDifName(difName);
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw NotifyApplicationRegisteredException(e.what());
	}
#endif
}

void ApplicationManager::applicationUnregistered(
		const ApplicationUnregistrationRequestEvent& event,
		int result)
			throw (NotifyApplicationUnregisteredException) {
	LOG_DBG("ApplicationManager::applicationUnregistered called");

#if STUB_API
	//Do nothing
#else
	AppUnregisterApplicationResponseMessage responseMessage;
	responseMessage.setApplicationName(event.getApplicationName());
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw NotifyApplicationUnregisteredException(e.what());
	}
#endif
}

void ApplicationManager::flowAllocated(const FlowRequestEvent& flowRequestEvent)
throw (NotifyFlowAllocatedException) {
	LOG_DBG("ApplicationManager::flowAllocated called");

#if STUB_API
	//Do nothing
#else
	AppAllocateFlowRequestResultMessage responseMessage;
	responseMessage.setPortId(flowRequestEvent.getPortId());
	responseMessage.setSourceAppName(flowRequestEvent.getLocalApplicationName());
	responseMessage.setDifName(flowRequestEvent.getDIFName());
	responseMessage.setSequenceNumber(flowRequestEvent.getSequenceNumber());
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw NotifyFlowAllocatedException(e.what());
	}
#endif
}

unsigned int ApplicationManager::flowRequestArrived(
			const ApplicationProcessNamingInformation& localAppName,
			const ApplicationProcessNamingInformation& remoteAppName,
			const FlowSpecification& flowSpec,
			const ApplicationProcessNamingInformation& difName,
			int portId) throw (AppFlowArrivedException){
#if STUB_API
	return 0;
#else
	AppAllocateFlowRequestArrivedMessage message;
	message.setSourceAppName(remoteAppName);
	message.setDestAppName(localAppName);
	message.setFlowSpecification(flowSpec);
	message.setDifName(difName);
	message.setPortId(portId);
	message.setRequestMessage(true);

	try{
	        rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
	        throw NotifyFlowDeallocatedException(e.what());
	}

	return message.getSequenceNumber();
#endif
}

void ApplicationManager::flowDeallocated(
		const FlowDeallocateRequestEvent& event, int result)
		throw (NotifyFlowDeallocatedException){
	LOG_DBG("ApplicationManager::flowdeallocated called");

#if STUB_API
	//Do nothing
#else
	AppDeallocateFlowResponseMessage responseMessage;
	responseMessage.setApplicationName(event.getApplicationName());
	responseMessage.setResult(result);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw NotifyFlowDeallocatedException(e.what());
	}
#endif
}

void ApplicationManager::flowDeallocatedRemotely(
		int portId, int code,
		const ApplicationProcessNamingInformation& appName)
	throw (NotifyFlowDeallocatedException){
	LOG_DBG("ApplicationManager::flowDeallocatedRemotely called");
#if STUB_API
	//Do nothing
#else
	AppFlowDeallocatedNotificationMessage message;
	message.setPortId(portId);
	message.setCode(code);
	message.setApplicationName(appName);
	message.setNotificationMessage(true);
	try{
		rinaManager->sendMessage(&message);
	}catch(NetlinkException &e){
		throw NotifyFlowDeallocatedException(e.what());
	}
#endif
}

void ApplicationManager::getDIFPropertiesResponse(
		const GetDIFPropertiesRequestEvent &event,
			int result, const std::list<DIFProperties>& difProperties)
			throw (GetDIFPropertiesResponseException){
#if STUB_API
	//Do nothing
#else
	AppGetDIFPropertiesResponseMessage responseMessage;
	responseMessage.setResult(result);
	responseMessage.setApplicationName(event.getApplicationName());
	responseMessage.setDIFProperties(difProperties);
	responseMessage.setSequenceNumber(event.getSequenceNumber());
	responseMessage.setResponseMessage(true);
	try{
		rinaManager->sendMessage(&responseMessage);
	}catch(NetlinkException &e){
		throw GetDIFPropertiesResponseException(e.what());
	}
#endif
}

Singleton<ApplicationManager> applicationManager;

/* CLASS GET DIF PROPERTIES REQUEST EVENT */
GetDIFPropertiesRequestEvent::GetDIFPropertiesRequestEvent(
		const ApplicationProcessNamingInformation& appName,
		const ApplicationProcessNamingInformation& DIFName,
		unsigned int sequenceNumber):
				IPCEvent(GET_DIF_PROPERTIES, sequenceNumber){
	this->applicationName = appName;
	this->DIFName = DIFName;
}

const ApplicationProcessNamingInformation&
	GetDIFPropertiesRequestEvent::getApplicationName() const{
	return applicationName;
}

const ApplicationProcessNamingInformation&
	GetDIFPropertiesRequestEvent::getDIFName() const{
	return DIFName;
}

/* CLASS IPCM REGISTER APPLICATION RESPONSE EVENT */
IpcmRegisterApplicationResponseEvent::IpcmRegisterApplicationResponseEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        IPCM_REGISTER_APP_RESPONSE_EVENT,
                                        sequenceNumber) {
}

/* CLASS IPCM UNREGISTER APPLICATION RESPONSE EVENT */
IpcmUnregisterApplicationResponseEvent::IpcmUnregisterApplicationResponseEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        IPCM_UNREGISTER_APP_RESPONSE_EVENT,
                                        sequenceNumber) {
}

/* CLASS IPCM DEALLOCATE FLOW RESPONSE EVENT */
IpcmDeallocateFlowResponseEvent::IpcmDeallocateFlowResponseEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        IPCM_DEALLOCATE_FLOW_RESPONSE_EVENT,
                                        sequenceNumber) {
}

/* CLASS IPCM ALLOCATE FLOW REQUEST RESULT EVENT */
IpcmAllocateFlowRequestResultEvent::IpcmAllocateFlowRequestResultEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        IPCM_ALLOCATE_FLOW_REQUEST_RESULT,
                                        sequenceNumber) {
}

/* CLASS QUERY RIB RESPONSE EVENT */
QueryRIBResponseEvent::QueryRIBResponseEvent(
                const std::list<RIBObject>& ribObjects,
                int result,
                unsigned int sequenceNumber) :
                BaseResponseEvent(result,
                                QUERY_RIB_RESPONSE_EVENT,
                                sequenceNumber){
        this->ribObjects = ribObjects;
}

const std::list<RIBObject>& QueryRIBResponseEvent::getRIBObject() const {
        return ribObjects;
}

/* CLASS ASSIGN TO DIF RESPONSE EVENT */
AssignToDIFResponseEvent::AssignToDIFResponseEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        ASSIGN_TO_DIF_RESPONSE_EVENT,
                                        sequenceNumber) {
}

/* CLASS UPDATE DIF CONFIGURATION RESPONSE EVENT */
UpdateDIFConfigurationResponseEvent::UpdateDIFConfigurationResponseEvent(
                int result, unsigned int sequenceNumber):
                        BaseResponseEvent(result,
                                        UPDATE_DIF_CONFIG_RESPONSE_EVENT,
                                        sequenceNumber) {
}

/* CLASS IPC PROCESS DAEMON INITIALIZED EVENT */
IPCProcessDaemonInitializedEvent::IPCProcessDaemonInitializedEvent(
                unsigned short ipcProcessId, unsigned int sequenceNumber):
                        IPCEvent(IPC_PROCESS_DAEMON_INITIALIZED_EVENT,
                                        sequenceNumber) {
        this->ipcProcessId = ipcProcessId;
}

unsigned short IPCProcessDaemonInitializedEvent::getIPCProcessId() const {
        return ipcProcessId;
}

}
