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

/**
 * This library provides definitions common to all the other RINA
 * related libraries presented in this document. Common
 * functionalities shared among framework components (i.e. applications,
 * daemons and libraries) might be made available from this library
 * as well.
 */

#ifndef LIBRINA_COMMON_H
#define LIBRINA_COMMON_H

#ifdef __cplusplus

#include <string>
#include <vector>
#include <list>

#include "exceptions.h"
#include "patterns.h"

namespace rina {

/**
 * Returns the version number of librina
 */
std::string getVersion();

/**
 * Contains application naming information
 */
class ApplicationProcessNamingInformation {
	/**
	 * The process_name identifies an application process within the
	 * application process namespace. This value is required, it
	 * cannot be NULL. This name has global scope (it is defined by
	 * the chain of IDD databases that are linked together), and is
	 * assigned by an authority that manages the namespace that
	 * particular application name belongs to.
	 */
	std::string processName;

	/**
	 * The process_instance identifies a particular instance of the
	 * process. This value is optional, it may be NULL.
	 */
	std::string processInstance;

	/**
	 * The entity_name identifies an application entity within the
	 * application process. This value is optional, it may be NULL.
	 */
	std::string entityName;

	/**
	 * The entity_name identifies a particular instance of an entity
	 * within the application process. This value is optional, it
	 * may be NULL
	 */
	std::string entityInstance;

public:
	ApplicationProcessNamingInformation();
	ApplicationProcessNamingInformation(const std::string& processName,
			const std::string& processInstance);
	ApplicationProcessNamingInformation & operator=(
			const ApplicationProcessNamingInformation & other);
	bool operator==(const ApplicationProcessNamingInformation &other) const;
	bool operator!=(const ApplicationProcessNamingInformation &other) const;
	bool operator>(const ApplicationProcessNamingInformation &other) const;
	bool operator<=(const ApplicationProcessNamingInformation &other) const;
	bool operator<(const ApplicationProcessNamingInformation &other) const;
	bool operator>=(const ApplicationProcessNamingInformation &other) const;
	const std::string& getEntityInstance() const;
	void setEntityInstance(const std::string& entityInstance);
	const std::string& getEntityName() const;
	void setEntityName(const std::string& entityName);
	const std::string& getProcessInstance() const;
	void setProcessInstance(const std::string& processInstance);
	const std::string& getProcessName() const;
	void setProcessName(const std::string& processName);
	std::string getProcessNamePlusInstance();
	std::string getEncodedString();
	const std::string toString();
};

/**
 * This class defines the characteristics of a flow
 */
class FlowSpecification {
	/** Average bandwidth in bytes/s. A value of 0 means don't care. */
	unsigned int averageBandwidth;

	/** Average bandwidth in SDUs/s. A value of 0 means don't care */
	unsigned int averageSDUBandwidth;

	/** In milliseconds. A value of 0 means don't care*/
	unsigned int peakBandwidthDuration;

	/** In milliseconds. A value of 0 means don't care */
	unsigned int peakSDUBandwidthDuration;

	/** A value of 0 indicates 'do not care' */
	double undetectedBitErrorRate;

	/** Indicates if partial delivery of SDUs is allowed or not */
	bool partialDelivery;

	/** Indicates if SDUs have to be delivered in order */
	bool orderedDelivery;

	/**
	 * Indicates the maximum gap allowed among SDUs, a gap of N SDUs
	 * is considered the same as all SDUs delivered. A value of -1
	 * indicates 'Any'
	 */
	int maxAllowableGap;

	/**
	 * In milliseconds, indicates the maximum delay allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int delay;

	/**
	 * In milliseconds, indicates the maximum jitter allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int jitter;

	/**
	 * The maximum SDU size for the flow. May influence the choice
	 * of the DIF where the flow will be created.
	 */
	unsigned int maxSDUsize;

public:
	FlowSpecification();
	bool operator==(const FlowSpecification &other) const;
	bool operator!=(const FlowSpecification &other) const;
	unsigned int getAverageBandwidth() const;
	void setAverageBandwidth(unsigned int averageBandwidth);
	unsigned int getAverageSduBandwidth() const;
	void setAverageSduBandwidth(unsigned int averageSduBandwidth);
	unsigned int getDelay() const;
	void setDelay(unsigned int delay);
	unsigned int getJitter() const;
	void setJitter(unsigned int jitter);
	int getMaxAllowableGap() const;
	void setMaxAllowableGap(int maxAllowableGap);
	unsigned int getMaxSDUSize() const;
	void setMaxSDUSize(unsigned int maxSduSize);
	bool isOrderedDelivery() const;
	void setOrderedDelivery(bool orderedDelivery);
	bool isPartialDelivery() const;
	void setPartialDelivery(bool partialDelivery);
	unsigned int getPeakBandwidthDuration() const;
	void setPeakBandwidthDuration(unsigned int peakBandwidthDuration);
	unsigned int getPeakSduBandwidthDuration() const;
	void setPeakSduBandwidthDuration(unsigned int peakSduBandwidthDuration);
	double getUndetectedBitErrorRate() const;
	void setUndetectedBitErrorRate(double undetectedBitErrorRate);
	const std::string toString();
};

/**
 * Contains the information of an allocated flow
 */
class FlowInformation {

	/** The local application name */
	ApplicationProcessNamingInformation localAppName;

	/** The remote application name */
	ApplicationProcessNamingInformation remoteAppName;

	/** The flow characteristics */
	FlowSpecification flowSpecification;

	/** The portId of the flow */
	int portId;

	/** The name of the DIF where the flow has been allocated */
	ApplicationProcessNamingInformation difName;

public:
	bool operator==(const FlowInformation &other) const;
	bool operator!=(const FlowInformation &other) const;
	const ApplicationProcessNamingInformation& getDifName() const;
	void setDifName(const ApplicationProcessNamingInformation& difName);
	const FlowSpecification& getFlowSpecification() const;
	void setFlowSpecification(const FlowSpecification& flowSpecification);
	const ApplicationProcessNamingInformation& getLocalAppName() const;
	void setLocalAppName(
			const ApplicationProcessNamingInformation& localAppName);
	int getPortId() const;
	void setPortId(int portId);
	const ApplicationProcessNamingInformation& getRemoteAppName() const;
	void setRemoteAppName(
			const ApplicationProcessNamingInformation& remoteAppName);
	const std::string toString();
};

/**
 * Defines the properties that a QoSCube is able to provide
 */
class QoSCube {

	/** The name of the QoS cube*/
	std::string name;

	/** The id of the QoS cube */
	int id;

	/** Average bandwidth in bytes/s. A value of 0 means don't care. */
	unsigned int averageBandwidth;

	/** Average bandwidth in SDUs/s. A value of 0 means don't care */
	unsigned int averageSDUBandwidth;

	/** In milliseconds. A value of 0 means don't care*/
	unsigned int peakBandwidthDuration;

	/** In milliseconds. A value of 0 means don't care */
	unsigned int peakSDUBandwidthDuration;

	/** A value of 0 indicates 'do not care' */
	double undetectedBitErrorRate;

	/** Indicates if partial delivery of SDUs is allowed or not */
	bool partialDelivery;

	/** Indicates if SDUs have to be delivered in order */
	bool orderedDelivery;

	/**
	 * Indicates the maximum gap allowed among SDUs, a gap of N SDUs
	 * is considered the same as all SDUs delivered. A value of -1
	 * indicates 'Any'
	 */
	int maxAllowableGap;

	/**
	 * In milliseconds, indicates the maximum delay allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int delay;

	/**
	 * In milliseconds, indicates the maximum jitter allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int jitter;
public:
	QoSCube();
	QoSCube(const std::string& name, int id);
	bool operator==(const QoSCube &other) const;
	bool operator!=(const QoSCube &other) const;
	void setId(int id);
	int getId() const;
	const std::string& getName() const;
	void setName(const std::string& name);
	unsigned int getAverageBandwidth() const;
	void setAverageBandwidth(unsigned int averageBandwidth);
	unsigned int getAverageSduBandwidth() const;
	void setAverageSduBandwidth(unsigned int averageSduBandwidth);
	unsigned int getDelay() const;
	void setDelay(unsigned int delay);
	unsigned int getJitter() const;
	void setJitter(unsigned int jitter);
	int getMaxAllowableGap() const;
	void setMaxAllowableGap(int maxAllowableGap);
	bool isOrderedDelivery() const;
	void setOrderedDelivery(bool orderedDelivery);
	bool isPartialDelivery() const;
	void setPartialDelivery(bool partialDelivery);
	unsigned int getPeakBandwidthDuration() const;
	void setPeakBandwidthDuration(unsigned int peakBandwidthDuration);
	unsigned int getPeakSduBandwidthDuration() const;
	void setPeakSduBandwidthDuration(unsigned int peakSduBandwidthDuration);
	double getUndetectedBitErrorRate() const;
	void setUndetectedBitErrorRate(double undetectedBitErrorRate);
	const std::string toString();
};

/**
 * This class contains the properties of a single DIF
 */
class DIFProperties {
	/** The name of the DIF */
	ApplicationProcessNamingInformation DIFName;

	/**
	 * The maximum SDU size this DIF can handle (writes with bigger
	 * SDUs will return an error, and read will never return an SDUs
	 * bigger than this size
	 */
	unsigned int maxSDUSize;

	/**
	 * The different QoS cubes supported by the DIF
	 */
	std::list<QoSCube> qosCubes;
public:
	DIFProperties();
	DIFProperties(const ApplicationProcessNamingInformation& DIFName,
			int maxSDUSize);
	const ApplicationProcessNamingInformation& getDifName() const;
	unsigned int getMaxSduSize() const;
	const std::list<QoSCube>& getQoSCubes() const;
	void addQoSCube(const QoSCube& qosCube);
	void removeQoSCube(const QoSCube& qosCube);
};

/**
 * Enum type that identifies the different types of events
 */
enum IPCEventType {
	FLOW_ALLOCATION_REQUESTED_EVENT,
	ALLOCATE_FLOW_REQUEST_RESULT_EVENT,
	ALLOCATE_FLOW_RESPONSE_EVENT,
	FLOW_DEALLOCATION_REQUESTED_EVENT,
	DEALLOCATE_FLOW_RESPONSE_EVENT,
	APPLICATION_UNREGISTERED_EVENT,
	FLOW_DEALLOCATED_EVENT,
	APPLICATION_REGISTRATION_REQUEST_EVENT,
	REGISTER_APPLICATION_RESPONSE_EVENT,
	APPLICATION_UNREGISTRATION_REQUEST_EVENT,
	UNREGISTER_APPLICATION_RESPONSE_EVENT,
	APPLICATION_REGISTRATION_CANCELED_EVENT,
	ASSIGN_TO_DIF_REQUEST_EVENT,
	ASSIGN_TO_DIF_RESPONSE_EVENT,
	UPDATE_DIF_CONFIG_REQUEST_EVENT,
	UPDATE_DIF_CONFIG_RESPONSE_EVENT,
	ENROLL_TO_DIF_REQUEST_EVENT,
	ENROLL_TO_DIF_RESPONSE_EVENT,
	NEIGHBORS_MODIFIED_NOTIFICAITON_EVENT,
	IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION,
	IPC_PROCESS_QUERY_RIB,
	GET_DIF_PROPERTIES,
	GET_DIF_PROPERTIES_RESPONSE_EVENT,
	OS_PROCESS_FINALIZED,
	IPCM_REGISTER_APP_RESPONSE_EVENT,
	IPCM_UNREGISTER_APP_RESPONSE_EVENT,
	IPCM_DEALLOCATE_FLOW_RESPONSE_EVENT,
	IPCM_ALLOCATE_FLOW_REQUEST_RESULT,
	QUERY_RIB_RESPONSE_EVENT,
	IPC_PROCESS_DAEMON_INITIALIZED_EVENT,
	TIMER_EXPIRED_EVENT,
	IPC_PROCESS_CREATE_CONNECTION_RESPONSE,
	IPC_PROCESS_UPDATE_CONNECTION_RESPONSE,
	IPC_PROCESS_CREATE_CONNECTION_RESULT,
	IPC_PROCESS_DESTROY_CONNECTION_RESULT,
	IPC_PROCESS_DUMP_FT_RESPONSE
};

/**
 * Base class for IPC Events
 */
class IPCEvent {
	/** The type of event */
	IPCEventType eventType;

	/**
	 * If the event is a request, this is the number to relate it
	 * witht the response
	 */
	unsigned int sequenceNumber;

public:
	virtual ~IPCEvent(){}

        IPCEvent() { }

	IPCEvent(IPCEventType eventType, unsigned int sequenceNumber) {
		this->eventType = eventType;
		this->sequenceNumber = sequenceNumber;
	}

	IPCEventType getType() const {
		return eventType;
	}

	unsigned int getSequenceNumber() const{
		return sequenceNumber;
	}
};

class BaseResponseEvent: public IPCEvent {
        /** The result of the operation */
        int result;

public:
        BaseResponseEvent(
                        int result,
                        IPCEventType eventType,
                        unsigned int sequenceNumber);
        int getResult() const;
};

/**
 * Event informing about an incoming flow request
 */
class FlowRequestEvent: public IPCEvent {
	/** The port-id that locally identifies the flow */
	int portId;

	/** The name of the DIF that is providing this flow */
	ApplicationProcessNamingInformation DIFName;

	/** The local application name*/
	ApplicationProcessNamingInformation localApplicationName;

	/** The remote application name*/
	ApplicationProcessNamingInformation remoteApplicationName;

	/** The characteristics of the flow */
	FlowSpecification flowSpecification;

	/** True if it is a local application, false if it is a remote one */
	bool localRequest;

	/** 0 if it is an application, or the ID of the IPC Process otherwise */
	int flowRequestorIpcProcessId;

	/** the ID of the IPC Process that will provide the flow*/
	unsigned short ipcProcessId;

public:
	FlowRequestEvent(const FlowSpecification& flowSpecification,
			bool localRequest,
			const ApplicationProcessNamingInformation& localApplicationName,
			const ApplicationProcessNamingInformation& remoteApplicationName,
			int flowRequestorIpcProcessId,
			unsigned int sequenceNumber);
	FlowRequestEvent(int portId,
			const FlowSpecification& flowSpecification,
			bool localRequest,
			const ApplicationProcessNamingInformation& localApplicationName,
			const ApplicationProcessNamingInformation& remoteApplicationName,
			const ApplicationProcessNamingInformation& DIFName,
			unsigned short ipcProcessId,
			unsigned int sequenceNumber);
	int getPortId() const;
	bool isLocalRequest() const;
	const FlowSpecification& getFlowSpecification() const;
	void setPortId(int portId);
	void setDIFName(const ApplicationProcessNamingInformation& difName);
	const ApplicationProcessNamingInformation& getDIFName() const;
	const ApplicationProcessNamingInformation& getLocalApplicationName() const;
	const ApplicationProcessNamingInformation& getRemoteApplicationName() const;
	int getFlowRequestorIPCProcessId() const;
	unsigned short getIPCProcessId() const;
};

/**
 * Event informing the IPC Process about a flow deallocation request
 */
class FlowDeallocateRequestEvent: public IPCEvent {
	/** The port-id that locally identifies the flow */
	int portId;

	/** The application that requested the flow deallocation*/
	ApplicationProcessNamingInformation applicationName;

public:
	FlowDeallocateRequestEvent(int portId,
			const ApplicationProcessNamingInformation& appName,
			unsigned int sequenceNumber);
	FlowDeallocateRequestEvent(int portId,
				unsigned int sequenceNumber);
	int getPortId() const;
	const ApplicationProcessNamingInformation& getApplicationName() const;
};

/**
 * Event informing that a flow has been deallocated by an IPC Process, without
 * the application having requested it
 */
class FlowDeallocatedEvent: public IPCEvent {
	/** The port id of the deallocated flow */
	int portId;

	/** An error code indicating why the flow was deallocated */
	int code;

public:
	FlowDeallocatedEvent(int portId, int code);
	int getPortId() const;
	int getCode() const;
	const ApplicationProcessNamingInformation getDIFName() const;
};

/**
 * Identifies the types of application registrations
 * APPLICATION_REGISTRATION_SINGLE_DIF - registers the application in a single
 * DIF, specified by the application
 * APPLICATION_REGISTRATION_ANY_DIF - registers the application in any of the
 * DIFs available to the application, chosen by the IPC Manager
 */
enum ApplicationRegistrationType {
	APPLICATION_REGISTRATION_SINGLE_DIF,
	APPLICATION_REGISTRATION_ANY_DIF
};

/**
 * Contains information about the registration of an application
 */
class ApplicationRegistrationInformation {

        /** The name of the application being registered */
        ApplicationProcessNamingInformation appName;

        /**
         * The id of the IPC process being registered (0 if it is
         * an application
         */
        unsigned short ipcProcessId;

	/** The type of registration requested */
	ApplicationRegistrationType applicationRegistrationType;

	/** Optional DIF name where the application wants to register */
	ApplicationProcessNamingInformation difName;

public:
	ApplicationRegistrationInformation();
	ApplicationRegistrationInformation(
		ApplicationRegistrationType applicationRegistrationType);
	const ApplicationProcessNamingInformation& getApplicationName() const;
	void setApplicationName(
	                const ApplicationProcessNamingInformation& appName);
	ApplicationRegistrationType getRegistrationType() const;
	const ApplicationProcessNamingInformation& getDIFName() const;
	void setDIFName(const ApplicationProcessNamingInformation& difName);
        unsigned short getIpcProcessId() const;
        void setIpcProcessId(unsigned short ipcProcessId);
        const std::string toString();
};

/**
 * Event informing that an application has requested the
 * registration to a DIF
 */
class ApplicationRegistrationRequestEvent: public IPCEvent {

	/** The application registration information*/
	ApplicationRegistrationInformation applicationRegistrationInformation;

public:
	ApplicationRegistrationRequestEvent(
		const ApplicationRegistrationInformation&
		applicationRegistrationInformation, unsigned int sequenceNumber);
	const ApplicationRegistrationInformation&
		getApplicationRegistrationInformation() const;
};

class BaseApplicationRegistrationEvent: public IPCEvent {
        /** The application that wants to unregister */
        ApplicationProcessNamingInformation applicationName;

        /** The DIF to which the application wants to cancel the registration */
        ApplicationProcessNamingInformation DIFName;

public:
        BaseApplicationRegistrationEvent(
                        const ApplicationProcessNamingInformation& appName,
                        const ApplicationProcessNamingInformation& DIFName,
                        IPCEventType eventType,
                        unsigned int sequenceNumber);
        BaseApplicationRegistrationEvent(
                        const ApplicationProcessNamingInformation& appName,
                        IPCEventType eventType,
                        unsigned int sequenceNumber);
        const ApplicationProcessNamingInformation& getApplicationName() const;
        const ApplicationProcessNamingInformation& getDIFName() const;
};

/**
 * Event informing that an application has requested the
 * unregistration from a DIF
 */
class ApplicationUnregistrationRequestEvent:
                public BaseApplicationRegistrationEvent {
public:
	ApplicationUnregistrationRequestEvent(
			const ApplicationProcessNamingInformation& appName,
			const ApplicationProcessNamingInformation& DIFName,
			unsigned int sequenceNumber);
};

class BaseApplicationRegistrationResponseEvent:
                public BaseApplicationRegistrationEvent {
        /** The result of the operation */
        int result;

public:
        BaseApplicationRegistrationResponseEvent(
                        const ApplicationProcessNamingInformation& appName,
                        const ApplicationProcessNamingInformation& DIFName,
                        int result,
                        IPCEventType eventType,
                        unsigned int sequenceNumber);
        BaseApplicationRegistrationResponseEvent(
                        const ApplicationProcessNamingInformation& appName,
                        int result,
                        IPCEventType eventType,
                        unsigned int sequenceNumber);
        int getResult() const;
};

/**
 * Event informing about the result of an application registration request
 */
class RegisterApplicationResponseEvent:
                public BaseApplicationRegistrationResponseEvent {
public:
        RegisterApplicationResponseEvent(
                        const ApplicationProcessNamingInformation& appName,
                        const ApplicationProcessNamingInformation& difName,
                        int result, unsigned int sequenceNumber);
};

/**
 * Event informing about the result of an application unregistration request
 */
class UnregisterApplicationResponseEvent:
                public BaseApplicationRegistrationResponseEvent {
public:
        UnregisterApplicationResponseEvent(
                        const ApplicationProcessNamingInformation& appName,
                        int result, unsigned int sequenceNumber);
};

/**
 * Event informing about the application decision regarding the
 * acceptance/denial of a flow request
 */
class AllocateFlowResponseEvent: public BaseResponseEvent {
        /**
         * If the flow was denied, this field controls wether the application
         * wants the IPC Process to reply to the source or not
         */
        bool notifySource;

        /** 0 if it is an application, or the ID of the IPC Process otherwise */
        int flowAcceptorIpcProcessId;

public:
        AllocateFlowResponseEvent(
                        int result,
                        bool notifysource,
                        int flowAcceptorIpcProcessId,
                        unsigned int sequenceNumber);
        bool isNotifySource() const;
        int getFlowAcceptorIpcProcessId() const;
};

/**
 * Event informing that an OS process (an application or an
 * IPC Process daemon) has finalized
 */
class OSProcessFinalizedEvent: public IPCEvent {
	/**
	 * The naming information of the application that has
	 * finalized
	 */
	ApplicationProcessNamingInformation applicationName;

	/**
	 * If this id is greater than 0, it means that the process
	 * that finalized was an IPC Process Daemon. Otherwise it is an
	 * application process.
	 */
	unsigned int ipcProcessId;

public:
	OSProcessFinalizedEvent(const ApplicationProcessNamingInformation& appName,
			unsigned int ipcProcessId, unsigned int sequenceNumber);
	const ApplicationProcessNamingInformation& getApplicationName() const;
	unsigned int getIPCProcessId() const;
};

/**
 * Stores IPC Events that have happened, ready to be consumed and
 * processed by client classes.
 */
class IPCEventProducer {
public:
	/** Retrieves the next available event, if any */
	IPCEvent * eventPoll();

	/** Blocks until there is an event available */
	IPCEvent * eventWait();

	/**
	 * Blocks until there is an event available, no more than the
	 * time specified
	 */
	IPCEvent * eventTimedWait(int seconds, int nanoseconds);
};

/**
 * Make IPCManager singleton
 */
extern Singleton<IPCEventProducer> ipcEventProducer;

/**
 * Base class for all RINA exceptions
 */
class IPCException: public Exception {
public:
	IPCException(const std::string& description);
	static const std::string operation_not_implemented_error;
};

/**
 * Thrown when there are problems assigning an IPC Process to a DIF
 */
class AssignToDIFException: public IPCException {
public:
        AssignToDIFException():
                IPCException("Problems assigning IPC Process to DIF"){
        }
        AssignToDIFException(const std::string& description):
                IPCException(description){
        }
};

/**
 * Thrown when there are problems updating a DIF configuration
 */
class UpdateDIFConfigurationException: public IPCException {
public:
        UpdateDIFConfigurationException():
                IPCException("Problems updating DIF configuration"){
        }
        UpdateDIFConfigurationException(const std::string& description):
                IPCException(description){
        }
};

/**
 * Represents a parameter that has a name and value
 */
class Parameter {
        std::string name;
        std::string value;

public:
        Parameter();
        Parameter(const std::string & name, const std::string & value);
        bool operator==(const Parameter &other) const;
        bool operator!=(const Parameter &other) const;
        const std::string& getName() const;
        void setName(const std::string& name);
        const std::string& getValue() const;
        void setValue(const std::string& value);
};

/**
 * Contains the values of the constants for the Error and Flow Control
 * Protocol (EFCP)
 */
class DataTransferConstants {

        /** The length of QoS-id field in the DTP PCI, in bytes */
        unsigned short qosIdLenght;

        /** The length of the Port-id field in the DTP PCI, in bytes */
        unsigned short portIdLength;

        /** The length of the CEP-id field in the DTP PCI, in bytes */
        unsigned short cepIdLength;

        /** The length of the sequence number field in the DTP PCI, in bytes */
        unsigned short sequenceNumberLength;

        /** The length of the address field in the DTP PCI, in bytes */
        unsigned short addressLength;

        /** The length of the length field in the DTP PCI, in bytes */
        unsigned short lengthLength;

        /** The maximum length allowed for a PDU in this DIF, in bytes */
        unsigned int maxPDUSize;

        /**
         * True if the PDUs in this DIF have CRC, TTL, and/or encryption. Since
         * headers are encrypted, not just user data, if any flow uses encryption,
         * all flows within the same DIF must do so and the same encryption
         * algorithm must be used for every PDU; we cannot identify which flow
         * owns a particular PDU until it has been decrypted.
         */
        bool DIFIntegrity;

        /**
         * The maximum PDU lifetime in this DIF, in milliseconds. This is MPL
         * in delta-T
         */
        unsigned int maxPDULifetime;

public:
        DataTransferConstants();
        unsigned short getAddressLength() const;
        void setAddressLength(unsigned short addressLength);
        unsigned short getCepIdLength() const;
        void setCepIdLength(unsigned short cepIdLength);
        bool isDifIntegrity() const;
        void setDifIntegrity(bool difIntegrity);
        unsigned short getLengthLength() const;
        void setLengthLength(unsigned short lengthLength);
        unsigned int getMaxPduLifetime() const;
        void setMaxPduLifetime(unsigned int maxPduLifetime);
        unsigned int getMaxPduSize() const;
        void setMaxPduSize(unsigned int maxPduSize);
        unsigned short getPortIdLength() const;
        void setPortIdLength(unsigned short portIdLength);
        unsigned short getQosIdLenght() const;
        void setQosIdLenght(unsigned short qosIdLenght);
        unsigned short getSequenceNumberLength() const;
        void setSequenceNumberLength(unsigned short sequenceNumberLength);
        bool isInitialized();
        const std::string toString();
};

/**
 * A parameter of the policy
 */
class PolicyParameter {
        /** the name of the parameter */
        std::string name;

        /** the value of the parameter */
        std::string value;

public:
        PolicyParameter();
        PolicyParameter(const std::string& name, const std::string& value);
        bool operator==(const PolicyParameter &other) const;
        bool operator!=(const PolicyParameter &other) const;
        const std::string& getName() const;
        void setName(const std::string& name);
        const std::string& getValue() const;
        void setValue(const std::string& value);
};

/**
 * Configuration of a policy (name/version/parameters)
 */
class PolicyConfig {

        /** the name of policy */
        std::string name;

        /** the version of the policy */
        std::string version;

        /** optional name/value parameters to configure the policy */
        std::list<PolicyParameter> parameters;

public:
        PolicyConfig();
        PolicyConfig(const std::string& name, const std::string& version);
        bool operator==(const PolicyConfig &other) const;
        bool operator!=(const PolicyConfig &other) const;
        const std::string& getName() const;
        void setName(const std::string& name);
        //const std::list<PolicyParameter>& getParameters() const;
        //void setParameters(const std::list<PolicyParameter>& parameters);
        //void addParameter(const PolicyParameter& paremeter);
        const std::string& getVersion() const;
        void setVersion(const std::string& version);
};

/**
 * Link State routing configuration
 */
class LinkStateRoutingConfiguration {
private:
        static const int PULSES_UNTIL_FSO_EXPIRATION_DEFAULT = 100000;
        static const int WAIT_UNTIL_READ_CDAP_DEFAULT = 5001;
        static const int WAIT_UNTIL_ERROR_DEFAULT = 5001;
        static const int WAIT_UNTIL_PDUFT_COMPUTATION_DEFAULT = 103;
        static const int WAIT_UNTIL_FSODB_PROPAGATION_DEFAULT = 101;
        static const int WAIT_UNTIL_AGE_INCREMENT_DEFAULT = 997;
        static const std::string DEFAULT_ROUTING_ALGORITHM;
        int objectMaximumAge;
        int waitUntilReadCDAP;
        int waitUntilError;
        int waitUntilPDUFTComputation;
        int waitUntilFSODBPropagation;
        int waitUntilAgeIncrement;
        std::string routingAlgorithm;
public:
        LinkStateRoutingConfiguration();
        const std::string toString();
        int getWaitUntilAgeIncrement() const;
        void setWaitUntilAgeIncrement(const int waitUntilAgeIncrement);
        int getWaitUntilError() const;
        void setWaitUntilError(const int waitUntilError);
        int getWaitUntilFSODBPropagation() const;
        void setWaitUntilFSODBPropagation(const int waitUntilFsodbPropagation);
        int getWaitUntilPDUFTComputation() const;
        void setWaitUntilPDUFTComputation(const int waitUntilPduftComputation);
        int getWaitUntilReadCDAP() const;
        void setWaitUntilReadCDAP(const int waitUntilReadCdap);
        int getObjectMaximumAge() const;
        void setObjectMaximumAge(const int objectMaximumAge);
        const std::string& getRoutingAlgorithm() const;
        void setRoutingAlgorithm(const std::string& routingAlgorithm);
};

/**
 * PDU F Table Generator Configuration
 */
class PDUFTableGeneratorConfiguration {
private:
        /** Name, version and configuration of the PDU FT Generator policy */
        PolicyConfig pduFTGeneratorPolicy;

        /**
         * Link state routing configuration parameters - only relevant if a
         * link-state routing PDU FT Generation policy is used
         */
        LinkStateRoutingConfiguration linkStateRoutingConfiguration;
public:
        PDUFTableGeneratorConfiguration();
        PDUFTableGeneratorConfiguration(const PolicyConfig& pduFTGeneratorPolicy);
        const PolicyConfig& getPduFtGeneratorPolicy() const;
        void setPduFtGeneratorPolicy(const PolicyConfig& pduFtGeneratorPolicy);
        const LinkStateRoutingConfiguration& getLinkStateRoutingConfiguration() const;
        void setLinkStateRoutingConfiguration(
                        const LinkStateRoutingConfiguration& linkStateRoutingConfiguration);
};

/**
 * Contains the data about a DIF Configuration
 * (QoS cubes, policies, parameters, etc)
 */
class DIFConfiguration {

	/** The DIF Data Transfer constants */
	DataTransferConstants dataTransferConstants;

	/** The address of the IPC Process in the DIF */
	unsigned int address;

	/** The QoS cubes supported by the DIF */
	std::list<QoSCube> qosCubes;

	/** The policies of the DIF */
	std::list<PolicyConfig> policies;

	/** Configuration parameters */
	std::list<Parameter> parameters;

	/** PDUFT Configuration parameters of the DIF	*/
	PDUFTableGeneratorConfiguration pdufTableGeneratorConfiguration;

public:
	const std::list<PolicyConfig>& getPolicies();
	void setPolicies(const std::list<PolicyConfig>& policies);
	void addPolicy(const PolicyConfig& policy);
	const std::list<QoSCube>& getQosCubes() const;
	void setQosCubes(const std::list<QoSCube>& qosCubes);
	void addQoSCube(const QoSCube& qosCube);
	const std::list<Parameter>& getParameters() const;
	void setParameters(const std::list<Parameter>& parameters);
	void addParameter(const Parameter& parameter);
	const DataTransferConstants& getDataTransferConstants() const;
	void setDataTransferConstants(
	                const DataTransferConstants& dataTransferConstants);
	unsigned int getAddress() const;
	void setAddress(unsigned int address);
	void setPDUFTableGeneratorConfiguration(
	                const PDUFTableGeneratorConfiguration& pdufTableGeneratorConfiguration);
	const PDUFTableGeneratorConfiguration& getPDUFTableGeneratorConfiguration() const;
};

/**
 * Contains the information about a DIF (name, type, configuration)
 */
class DIFInformation{
	/** The type of DIF */
	std::string difType;

	/** The name of the DIF */
	ApplicationProcessNamingInformation difName;

	/** The DIF Configuration (qoscubes, policies, parameters, etc) */
	DIFConfiguration difConfiguration;

public:
	const ApplicationProcessNamingInformation& getDifName() const;
	void setDifName(const ApplicationProcessNamingInformation& difName);
	const std::string& getDifType() const;
	void setDifType(const std::string& difType);
	const DIFConfiguration& getDifConfiguration() const;
	void setDifConfiguration(const DIFConfiguration& difConfiguration);
};

/**
 * Represents an IPC Process with whom we're enrolled
 */
class Neighbor {

        /** The IPC Process name of the neighbor */
        ApplicationProcessNamingInformation name;

        /**
         * The name of the supporting DIF used to exchange data
         * with the neighbor
         */
        ApplicationProcessNamingInformation supportingDifName;

        /**
         * The names of all the supporting DIFs of this neighbor
         */
        std::list<ApplicationProcessNamingInformation> supportingDifs;

        /** The address */
        unsigned int address;

        /** Tells if it is enrolled or not */
        bool enrolled;

        /** The average RTT in ms */
        unsigned int averageRTTInMs;

        /** The underlying portId used to communicate with this neighbor */
        int underlyingPortId;

        /**
         * The last time a KeepAlive message was received from
         * that neighbor, in ms
         */
        long long lastHeardFromTimeInMs;

        /**
         * The number of times we have tried to re-enroll with the
         * neighbor after the connectivity has been lost
         */
        unsigned int numberOfEnrollmentAttempts;

public:
        Neighbor();
        bool operator==(const Neighbor &other) const;
        bool operator!=(const Neighbor &other) const;
        const ApplicationProcessNamingInformation& getName() const;
        void setName(const ApplicationProcessNamingInformation& name);
        const ApplicationProcessNamingInformation&
                getSupportingDifName() const;
        void setSupportingDifName(
                const ApplicationProcessNamingInformation& supportingDifName);
        const std::list<ApplicationProcessNamingInformation>& getSupportingDifs();
        void setSupportingDifs(
                        const std::list<ApplicationProcessNamingInformation>& supportingDifs);
        void addSupoprtingDif(const ApplicationProcessNamingInformation& supportingDif);
        unsigned int getAddress() const;
        void setAddress(unsigned int address);
        unsigned int getAverageRttInMs() const;
        void setAverageRttInMs(unsigned int averageRttInMs);
        bool isEnrolled() const;
        void setEnrolled(bool enrolled);
        long long getLastHeardFromTimeInMs() const;
        void setLastHeardFromTimeInMs(long long lastHeardFromTimeInMs);
        int getUnderlyingPortId() const;
        void setUnderlyingPortId(int underlyingPortId);
        unsigned int getNumberOfEnrollmentAttempts() const;
        void setNumberOfEnrollmentAttempts(
                        unsigned int numberOfEnrollmentAttempts);
        const std::string toString();
};

/**
 * Represents the value of an object stored in the RIB
 */
class RIBObjectValue{
	//TODO
};

/**
 * Represents an object in the RIB
 */
class RIBObject{

	/** The class (type) of object */
	std::string clazz;

	/** The name of the object (unique within a class)*/
	std::string name;

	/** A synonim for clazz+name (unique within the RIB) */
	unsigned long instance;

	/** The value of the object */
	RIBObjectValue value;

	/**
	 * The value of the object, encoded in an string for
	 * displayable purposes
	 */
	std::string displayableValue;

	/** Geneartes a unique object instance */
	unsigned long generateObjectInstance();

public:
	RIBObject();
	RIBObject(std::string clazz, std::string name,
	                long long instance, RIBObjectValue value);
	bool operator==(const RIBObject &other) const;
	bool operator!=(const RIBObject &other) const;
	const std::string& getClazz() const;
	void setClazz(const std::string& clazz);
	unsigned long getInstance() const;
	void setInstance(unsigned long  instance);
	const std::string& getName() const;
	void setName(const std::string& name);
	RIBObjectValue getValue() const;
	void setValue(RIBObjectValue value);
        const std::string& getDisplayableValue() const;
        void setDisplayableValue(const std::string& displayableValue);
};

/**
 * Thrown when there are problems initializing librina
 */
class InitializationException: public IPCException {
public:
	InitializationException():
		IPCException("Problems initializing librina"){
	}
	InitializationException(const std::string& description):
		IPCException(description){
	}
};

/**
 * Thrown when there are problems instructing an IPC Process to enroll to a DIF
 */
class EnrollException: public IPCException {
public:
        EnrollException():
                IPCException("Problems causing an IPC Process to enroll to a DIF"){
        }
        EnrollException(const std::string& description):
                IPCException(description){
        }
};

/**
 * Initialize librina providing the local Netlink port-id where this librina
 * instantiation will be bound
 * @param localPort NL port used by this instantiation of librina
 * @param logLevel librina log level
 * @param pathToLogFile the path to the log file
 */
void initialize(unsigned int localPort, const std::string& logLevel,
                const std::string& pathToLogFile)
        throw (InitializationException);

/**
 * Initialize librina letting the OS choose the Netlink port-id where this
 * librina instantiation will be bound
 * @param logLevel librina log level
 * @param pathToLogFile the path to the log file
 */
void initialize(const std::string& logLevel,
                const std::string& pathToLogFile)
        throw (InitializationException);

}
#endif

#endif
