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

#ifndef LIBRINA_CDAP_H
#define LIBRINA_CDAP_H

#ifdef __cplusplus

#include "exceptions.h"
#include "librina-common.h"

namespace rina {

/// Encapsulates the data of an AuthValue
class AuthValue {
public:
	AuthValue();
private:
	/// Authentication name
	std::string auth_name_;
	/// Authentication password
	std::string auth_password_;
	/// Additional authentication information
	std::string auth_other_;
};

/// Encapsulates the data to set an object value
class ObjectValue {
public:
	ObjectValue();
	int get_intval() const;
	int get_sintval() const;
	long get_int64val() const;
	long get_sint64val() const;
	std::string get_strval() const;
	char* get_byteval() const;
	int get_floatval() const;
	long get_doubleval() const;
	bool is_booleanval() const;
	bool operator==(const ObjectValue &other) const;
private:
	int intval_;
	int sintval_;
	long int64val_;
	long sint64val_;
	std::string strval_;
	char* byteval_;
	int floatval_;
	long doubleval_;
	bool booleanval_;
};

/// Exception produced in the CDAP
class CDAPMessage;
class CDAPException: public Exception {
public:
	CDAPException();
	CDAPException(std::string result_reason);
	CDAPException(int result, std::string error_message);
	virtual ~CDAPException() throw () {};
private:
	/// Operation result code
	int result_;
};

/// Validates that a CDAP message is well formed
class CDAPMessageValidator {
public:
	/// Validates a CDAP message
	/// @param message
	/// @throws CDAPException thrown if the CDAP message is not valid, indicating the reason
	static void validate(const CDAPMessage &message) throw (CDAPException);
private:
	static void validateAbsSyntax(const CDAPMessage &message)
			throw (CDAPException);
	static void validateAuthMech(const CDAPMessage &message)
			throw (CDAPException);
	static void validateAuthValue(const CDAPMessage &message)
			throw (CDAPException);
	static void validateDestAEInst(const CDAPMessage &message)
			throw (CDAPException);
	static void validateDestAEName(const CDAPMessage &message)
			throw (CDAPException);
	static void validateDestApInst(const CDAPMessage &message)
			throw (CDAPException);
	static void validateDestApName(const CDAPMessage &message)
			throw (CDAPException);
	static void validateFilter(const CDAPMessage &message) throw (CDAPException);
	static void validateInvokeID(const CDAPMessage &message)
			throw (CDAPException);
	static void validateObjClass(const CDAPMessage &message)
			throw (CDAPException);
	static void validateObjInst(const CDAPMessage &message)
			throw (CDAPException);
	static void validateObjName(const CDAPMessage &message)
			throw (CDAPException);
	static void validateObjValue(const CDAPMessage &message)
			throw (CDAPException);
	static void validateOpcode(const CDAPMessage &message) throw (CDAPException);
	static void validateResult(const CDAPMessage &message) throw (CDAPException);
	static void validateResultReason(const CDAPMessage &message)
			throw (CDAPException);
	static void validateScope(const CDAPMessage &message) throw (CDAPException);
	static void validateSrcAEInst(const CDAPMessage &message)
			throw (CDAPException);
	static void validateSrcAEName(const CDAPMessage &message)
			throw (CDAPException);
	static void validateSrcApInst(const CDAPMessage &message)
			throw (CDAPException);
	static void validateSrcApName(const CDAPMessage &message)
			throw (CDAPException);
	static void validateVersion(const CDAPMessage &message)
			throw (CDAPException);
};

/// CDAP message. Depending on the opcode, the following messages are possible:
/// M_CONNECT -> Common Connect Request. Initiate a connection from a source application to a destination application
/// M_CONNECT_R -> Common Connect Response. Response to M_CONNECT, carries connection information or an error indication
/// M_RELEASE -> Common Release Request. Orderly close of a connection
/// M_RELEASE_R -> Common Release Response. Response to M_RELEASE, carries final resolution of close operation
/// M_CREATE -> Create Request. Create an application object
/// M_CREATE_R -> Create Response. Response to M_CREATE, carries result of create request, including identification of the created object
/// M_DELETE -> Delete Request. Delete a specified application object
/// M_DELETE_R -> Delete Response. Response to M_DELETE, carries result of a deletion attempt
/// M_READ -> Read Request. Read the value of a specified application object
/// M_READ_R -> Read Response. Response to M_READ, carries part of all of object value, or error indication
/// M_CANCELREAD -> Cancel Read Request. Cancel a prior read issued using the M_READ for which a value has not been completely returned
/// M_CANCELREAD_R -> Cancel Read Response. Response to M_CANCELREAD, indicates outcome of cancellation
/// M_WRITE -> Write Request. Write a specified value to a specified application object
/// M_WRITE_R -> Write Response. Response to M_WRITE, carries result of write operation
/// M_START -> Start Request. Start the operation of a specified application object, used when the object has operational and non operational states
/// M_START_R -> Start Response. Response to M_START, indicates the result of the operation
/// M_STOP -> Stop Request. Stop the operation of a specified application object, used when the object has operational an non operational states
/// M_STOP_R -> Stop Response. Response to M_STOP, indicates the result of the operation.
class CDAPMessage {

public:
	enum Opcode {
		NONE_OPCODE,
		M_CONNECT,
		M_CONNECT_R,
		M_RELEASE,
		M_RELEASE_R,
		M_CREATE,
		M_CREATE_R,
		M_DELETE,
		M_DELETE_R,
		M_READ,
		M_READ_R,
		M_CANCELREAD,
		M_CANCELREAD_R,
		M_WRITE,
		M_WRITE_R,
		M_START,
		M_START_R,
		M_STOP,
		M_STOP_R
	};
	enum AuthTypes {
		AUTH_NONE, AUTH_PASSWD, AUTH_SSHRSA, AUTH_SSHDSA
	};
	enum Flags {
		NONE_FLAGS, F_SYNC, F_RD_INCOMPLETE
	};
	CDAPMessage();
	static void getOpenConnectionRequestMessage(CDAPMessage &cdapMessage,
			AuthTypes auth_mech, const AuthValue &auth_value,
			const std::string &dest_ae_inst, const std::string &dest_ae_name,
			const std::string &dest_ap_inst, const std::string &dest_ap_name,
			const std::string &src_ae_inst, const std::string &src_ae_name,
			const std::string &src_ap_inst, const std::string &src_ap_name,
			int invoke_id) throw (CDAPException);
	static void getOpenConnectionResponseMessage(CDAPMessage &cdapMessage,
			AuthTypes auth_mech, const AuthValue &auth_value,
			const std::string &dest_ae_inst, const std::string &dest_ae_name,
			const std::string &dest_ap_inst, const std::string &dest_ap_name,
			int result, const std::string &result_reason,
			const std::string &src_ae_inst, const std::string &src_ae_name,
			const std::string &src_ap_inst, const std::string &src_ap_name,
			int invoke_id) throw (CDAPException);
	static void getReleaseConnectionRequestMessage(CDAPMessage &cdapMessage,
			Flags flags) throw (CDAPException);
	static void getReleaseConnectionResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, int result, const std::string &result_reason,
			int invoke_id) throw (CDAPException);
	static void getCreateObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			long obj_inst, const std::string &obj_name,
			const ObjectValue &obj_value, int scope) throw (CDAPException);
	static void getCreateObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, const std::string &obj_class, long obj_inst,
			const std::string &obj_name, const ObjectValue &obj_value,
			int result, const std::string &result_reason, int invoke_id)
					throw (CDAPException);
	static void getDeleteObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			long obj_inst, const std::string &obj_name,
			const ObjectValue &object_value, int scope) throw (CDAPException);
	static void getDeleteObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, const std::string &obj_class, long obj_inst,
			const std::string &obj_name, int result,
			const std::string &result_reason, int invoke_id)
					throw (CDAPException);
	static void getStartObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			const ObjectValue &obj_value, long obj_inst,
			const std::string &obj_name, int scope) throw (CDAPException);
	static void getStartObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, int result, const std::string &result_reason,
			int invoke_id) throw (CDAPException);
	static void getStartObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, const std::string &objectClass,
			const ObjectValue &object_value, long obj_inst,
			const std::string &obj_name, int result,
			const std::string &result_reason, int invoke_id)
					throw (CDAPException);
	static void getStopObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			const ObjectValue &obj_value, long obj_inst,
			const std::string &obj_name, int scope) throw (CDAPException);
	static void getStopObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, int result, const std::string &result_reason,
			int invoke_id) throw (CDAPException);
	static void getReadObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			long obj_inst, const std::string &obj_name, int scope)
					throw (CDAPException);
	static void getReadObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, const std::string &obj_class, long obj_inst,
			const std::string &obj_name, const ObjectValue &obj_value,
			int result, const std::string &result_reason, int invoke_id)
					throw (CDAPException);
	static void getWriteObjectRequestMessage(CDAPMessage &cdapMessage,
			char filter[], Flags flags, const std::string &obj_class,
			long obj_inst, const ObjectValue &obj_value,
			const std::string &obj_name, int scope) throw (CDAPException);
	static void getWriteObjectResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, int result, int invoke_id,
			const std::string &result_reason) throw (CDAPException);
	static void getCancelReadRequestMessage(CDAPMessage &cdapMessage,
			Flags flags, int invoke_id) throw (CDAPException);
	static void getCancelReadResponseMessage(CDAPMessage &cdapMessage,
			Flags flags, int invoke_id, int result,
			const std::string &result_reason) throw (CDAPException);
	std::string toString();
	/// Returns a reply message from the request message, copying all the fields except for: Opcode (it will be the
	/// request message counterpart), result (it will be 0) and result_reason (it will be null)
	/// @param requestMessage
	/// @return
	/// @throws CDAPException
	CDAPMessage getReplyMessage();
	int get_abs_syntax() const;
	void set_abs_syntax(int abs_syntax);
	AuthTypes get_auth_mech() const;
	void set_auth_mech(AuthTypes auth_mech);
	const AuthValue& get_auth_value() const;
	void set_auth_value(AuthValue auth_value);
	std::string get_dest_ae_inst() const;
	void set_dest_ae_inst(std::string dest_ae_inst);
	std::string get_dest_ae_name() const;
	void set_dest_ae_name(std::string dest_ae_name);
	std::string get_dest_ap_inst() const;
	void set_dest_ap_inst(std::string dest_ap_inst);
	std::string get_dest_ap_name() const;
	void set_dest_ap_name(std::string dest_ap_name);
	char* get_filter() const;
	void set_filter(char filter[]);
	Flags get_flags() const;
	void set_flags(Flags flags);
	int get_invoke_id() const;
	void set_invoke_id(int invoke_id);
	std::string get_obj_class() const;
	void set_obj_class(std::string obj_class);
	long get_obj_inst() const;
	void set_obj_inst(long obj_inst);
	std::string get_obj_name() const;
	void set_obj_name(std::string obj_name);
	const ObjectValue& get_obj_value() const;
	void set_obj_value(const ObjectValue &obj_value);
	Opcode get_op_code() const;
	void set_op_code(Opcode op_code);
	int get_result() const;
	void set_result(int result);
	std::string get_result_reason() const;
	void set_result_reason(std::string result_reason);
	int get_scope() const;
	void set_scope(int scope);
	std::string get_src_ae_inst() const;
	void set_src_ae_inst(std::string src_ae_inst);
	std::string get_src_ae_name() const;
	void set_src_ae_name(std::string src_ae_name);
	std::string get_src_ap_inst() const;
	void set_src_ap_inst(std::string src_ap_inst);
	std::string get_src_ap_name() const;
	void set_src_ap_name(std::string src_ap_name);
	long get_version() const;
	void set_version(long version);
private:
	static const int ABSTRACT_SYNTAX_VERSION;
	/**
	 * AbstractSyntaxID (int32), mandatory. The specific version of the
	 * CDAP protocol message declarations that the message conforms to
	 */
	int abs_syntax_;
	/**
	 * AuthenticationMechanismName (authtypes), optional, not validated by CDAP.
	 * Identification of the method to be used by the destination application to
	 * authenticate the source application
	 */
	AuthTypes auth_mech_;
	/**
	 * AuthenticationValue (authvalue), optional, not validated by CDAP.
	 * Authentication information accompanying auth_mech, format and value
	 * appropiate to the selected auth_mech
	 */
	AuthValue auth_value_;
	/**
	 * DestinationApplication-Entity-Instance-Id (string), optional, not validated by CDAP.
	 * Specific instance of the Application Entity that the source application
	 * wishes to connect to in the destination application.
	 */
	std::string dest_ae_inst_;
	/**
	 * DestinationApplication-Entity-Name (string), mandatory (optional for the response).
	 * Name of the Application Entity that the source application wishes
	 * to connect to in the destination application.
	 */
	std::string dest_ae_name_;
	/**
	 * DestinationApplication-Process-Instance-Id (string), optional, not validated by CDAP.
	 * Name of the Application Process Instance that the source wishes to
	 * connect to a the destination.
	 */
	std::string dest_ap_inst_;
	/**
	 * DestinationApplication-Process-Name (string), mandatory (optional for the response).
	 * Name of the application process that the source application wishes to connect to
	 * in the destination application
	 */
	std::string dest_ap_name_;
	/**
	 * Filter (bytes). Filter predicate function to be used to determine whether an operation
	 * is to be applied to the designated object (s).
	 */
	char* filter_;
	/**
	 * flags (enm, int32), conditional, may be required by CDAP.
	 * set_ of Boolean values that modify the meaning of a
	 * message in a uniform way when true.
	 */
	Flags flags_;
	/**
	 * InvokeID, (int32). Unique identifier provided to identify a request, used to
	 * identify subsequent associated messages.
	 */
	int invoke_id_;
	/**
	 * ObjectClass (string). Identifies the object class definition of the
	 * addressed object.
	 */
	std::string obj_class_;
	/**
	 * ObjectInstance (int64). Object instance uniquely identifies a single object
	 * with a specific ObjectClass and ObjectName in an application's RIB. Either
	 * the ObjectClass and ObjectName or this value may be used, if the ObjectInstance
	 * value is known. If a class and name are supplied in an operation,
	 * an ObjectInstance value may be returned, and that may be used in future operations
	 * in lieu of obj_class and obj_name for the duration of this connection.
	 */
	long obj_inst_;
	/**
	 * ObjectName (string). Identifies a named object that the operation is
	 * to be applied to. Object names identify a unique object of the designated
	 * ObjectClass within an application.
	 */
	std::string obj_name_;
	/**
	 * ObjectValue (ObjectValue). The value of the object.
	 */
	ObjectValue obj_value_;
	/**
	 * Opcode (enum, int32), mandatory.
	 * Message type of this message.
	 */
	Opcode op_code_;
	/**
	 * Result (int32). Mandatory in the responses, forbidden in the requests
	 * The result of an operation, indicating its success (which has the value zero,
	 * the default for this field), partial success in the case of
	 * synchronized operations, or reason for failure
	 */
	int result_;
	/**
	 * Result-Reason (string), optional in the responses, forbidden in the requests
	 * Additional explanation of the result_
	 */
	std::string result_reason_;
	/**
	 * Scope (int32). Specifies the depth of the object tree at
	 * the destination application to which an operation (subject to filtering)
	 * is to apply (if missing or present and having the value 0, the default,
	 * only the target_ed application's objects are addressed)
	 */
	int scope_;
	/**
	 * SourceApplication-Entity-Instance-Id (string).
	 * AE instance within the application originating the message
	 */
	std::string src_ae_inst_;
	/**
	 * SourceApplication-Entity-Name (string).
	 * Name of the AE within the application originating the message
	 */
	std::string src_ae_name_;
	/**
	 * SourceApplication-Process-Instance-Id (string), optional, not validated by CDAP.
	 * Application instance originating the message
	 */
	std::string src_ap_inst_;
	/**
	 * SourceApplicatio-Process-Name (string), mandatory (optional in the response).
	 * Name of the application originating the message
	 */
	std::string src_ap_name_;
	/**
	 * Version (int32). Mandatory in connect request and response, optional otherwise.
	 * Version of RIB and object set_ to use in the conversation. Note that the
	 * abstract syntax refers to the CDAP message syntax, while version refers to the
	 * version of the AE RIB objects, their values, vocabulary of object id's, and
	 * related behaviors that are subject to change over time. See text for details
	 * of use.
	 */
	long version_;

};

///Describes a CDAPSession, by identifying the source and destination application processes.
///Note that "source" and "destination" are just placeholders, as both parties in a CDAP exchange have
///the same role, because the CDAP session is bidirectional.
class CDAPSessionDescriptor {
public:
	~CDAPSessionDescriptor();
	/// The source naming information is always the naming information of the local Application process
	const ApplicationProcessNamingInformation* get_source_application_process_naming_info();
	/// The destination naming information is always the naming information of the remote application process
	const ApplicationProcessNamingInformation* get_destination_application_process_naming_info();
private:
	/// AbstractSyntaxID (int32), mandatory. The specific version of the
	/// CDAP protocol message declarations that the message conforms to
	///
	int abs_syntax_;
	/// AuthenticationMechanismName (authtypes), optional, not validated by CDAP.
	/// Identification of the method to be used by the destination application to
	/// authenticate the source application
	CDAPMessage::AuthTypes auth_mech_;
	/**
	 * AuthenticationValue (authvalue), optional, not validated by CDAP.
	 * Authentication information accompanying auth_mech, format and value
	 * appropiate to the selected auth_mech
	 */
	AuthValue auth_value_;
	/// DestinationApplication-Entity-Instance-Id (string), optional, not validated by CDAP.
	/// Specific instance of the Application Entity that the source application
	/// wishes to connect to in the destination application.
	std::string dest_ae_inst_;
	/// DestinationApplication-Entity-Name (string), mandatory (optional for the response).
	/// Name of the Application Entity that the source application wishes
	/// to connect to in the destination application.
	std::string dest_ae_name_;
	/// DestinationApplication-Process-Instance-Id (string), optional, not validated by CDAP.
	/// Name of the Application Process Instance that the source wishes to
	/// connect to a the destination.
	std::string dest_ap_inst_;
	/// DestinationApplication-Process-Name (string), mandatory (optional for the response).
	/// Name of the application process that the source application wishes to connect to
	/// in the destination application
	std::string dest_ap_name_;
	/// SourceApplication-Entity-Instance-Id (string).
	/// AE instance within the application originating the message
	std::string src_ae_inst_;
	/// SourceApplication-Entity-Name (string).
	/// Name of the AE within the application originating the message
	std::string src_ae_name_;
	/// SourceApplication-Process-Instance-Id (string), optional, not validated by CDAP.
	/// Application instance originating the message
	std::string src_ap_inst_;
	/// SourceApplicatio-Process-Name (string), mandatory (optional in the response).
	/// Name of the application originating the message
	std::string src_ap_name_;
	/// Version (int32). Mandatory in connect request and response, optional otherwise.
	/// Version of RIB and object set_ to use in the conversation. Note that the
	/// abstract syntax refers to the CDAP message syntax, while version refers to the
	/// version of the AE RIB objects, their values, vocabulary of object id's, and
	/// related behaviors that are subject to change over time. See text for details
	/// of use.
	long version_;
	/// Uniquely identifies this CDAP session in this IPC process. It matches the port_id
	/// of the (N-1) flow that supports the CDAP Session
	int port_id_;

	ApplicationProcessNamingInformation *ap_naming_info_;
};

/**
 * Manages the invoke ids of a session.
 */
class CDAPSessionInvokeIdManagerInterface {
public:
	/*	Constructors and Destructors	*/
	virtual ~CDAPSessionInvokeIdManagerInterface() {
	}
	;
	/*	Accessors	*/
	/**
	 * Obtains a valid invoke id for this session
	 * @return
	 */
	virtual int get_InvokeId() const = 0;

	/**
	 * Allows an invoke id to be reused for this session
	 * @param invoke_id
	 */
	virtual void freeInvokeId(int invoke_id) = 0;

	/**
	 * Mark an invoke_id as reserved (don't use it)
	 * @param invoke_id
	 */
	virtual void reserveInvokeId(int invoke_id) = 0;
};

/**
 * Represents a CDAP session. Clients of the library are the ones managing the invoke ids. Application entities must
 * use the CDAP library this way:
 *  1) when sending a message:
 *     a) create the CDAPMessage
 *     b) call serializeNextMessageToBeSent()
 *     c) if it is successful, send the byte[] through the underlying transport connection
 *     d) if successful, update the CDAPSession state machine by calling messageSent()
 *  2) when receiving a message:
 *     a) call the messageReceived operation
 *     b) if successful, you can already use the cdap message; if not, look at the exception
 */
class CDAPSessionInterface {

	/*	Constructors and Destructors	*/
public:
	virtual ~CDAPSessionInterface() {
	}
	;
	/*	Accessors	*/
	/**
	 * Getter for the port_id
	 * @return a String that identifies a CDAP session within an IPC process
	 */
	virtual int getPortId() = 0;
	/**
	 * Getter for the sessionDescriptor
	 * @return the SessionDescriptor, provides all the data that describes this CDAP session (src and dest naming info,
	 * authentication type, version, ...)
	 */
	virtual const CDAPSessionDescriptor& getSessionDescriptor() = 0;
	virtual const CDAPSessionInvokeIdManagerInterface& getInvokeIdManager() = 0;
	/*	Methods	*/
	/**
	 * Encodes the next CDAP message to be sent, and checks against the
	 * CDAP state machine that this is a valid message to be sent
	 * @param message
	 * @return the serialized request message, ready to be sent to a flow
	 * @throws CDAPException if the message is bad formed or inconsistent with the protocol state machine
	 */
	virtual char& encodeNextMessageToBeSent(const CDAPMessage &message)
			throw (CDAPException) = 0;
	/**
	 * Tell the CDAP state machine that we've just sent the cdap Message,
	 * so the internal state machine will be updated
	 * @param message
	 * @throws CDAPException if the message is bad formed or inconsistent with the protocol state machine
	 */
	virtual void messageSent(const CDAPMessage &message) throw (CDAPException) = 0;
	/**
	 * Tell the CDAP state machine that we've received a message, and get
	 * the deserialized CDAP message. The state of the CDAP state machine will be updated
	 * @param cdapMessage
	 * @return
	 * @throws CDAPException if the message is bad formed or inconsistent with the protocol state machine
	 */
	virtual const CDAPMessage& messageReceived(char cdap_message[])
			throw (CDAPException) = 0;
	/**
	 * Tell the CDAP state machine that we've received a message. The state of the CDAP state machine will be updated
	 * @param cdapMessage
	 * @return
	 * @throws CDAPException if the message is bad formed or inconsistent with the protocol state machine
	 */
	virtual const CDAPMessage& messageReceived(const CDAPMessage &cdap_message)
			throw (CDAPException) = 0;

};

/**
 * Manages the creation/deletion of CDAP sessions within an IPC process
 *
 */
class CDAPSessionManagerInterface {
public:
	/*	Constructors and Destructors	*/
	virtual ~CDAPSessionManagerInterface() {
	}
	;
	/*	Methods	*/
	/**
	 * Depending on the message received, it will create a new CDAP state machine (CDAP Session), or update
	 * an existing one, or terminate one.
	 * @param encodedCDAPMessage
	 * @param port_id
	 * @return Decoded CDAP Message
	 * @throws CDAPException if the message is not consistent with the appropriate CDAP state machine
	 */
	virtual const CDAPMessage& messageReceived(char encoded_cdap_message[],
			int port_id) throw (CDAPException) = 0;
	/**
	 * Encodes the next CDAP message to be sent, and checks against the
	 * CDAP state machine that this is a valid message to be sent
	 * @param cdapMessage The cdap message to be serialized
	 * @param port_id
	 * @return encoded version of the CDAP Message
	 * @throws CDAPException
	 */
	virtual char* encodeNextMessageToBeSent(const CDAPMessage &cdap_message,
			int port_id) throw (CDAPException) = 0;
	/**
	 * Update the CDAP state machine because we've sent a message through the
	 * flow identified by port_id
	 * @param cdapMessage The CDAP message to be serialized
	 * @param port_id
	 * @return encoded version of the CDAP Message
	 * @throws CDAPException
	 */
	virtual void messageSent(const CDAPMessage &cdap_message, int port_id)
			throw (CDAPException) = 0;
	/**
	 * Get a CDAP session that matches the port_id
	 * @param port_id
	 * @return
	 */
	virtual const CDAPSessionInterface& getCDAPSession(int port_id) = 0;
	/**
	 * Get the identifiers of all the CDAP sessions
	 * @return
	 */
	virtual int* getAllCDAPSessionIds() = 0;
	/**
	 * Called by the CDAPSession state machine when the cdap session is terminated
	 * @param port_id
	 */
	virtual void removeCDAPSession(int port_id) = 0;
	/**
	 * Encodes a CDAP message. It just converts a CDAP message into a byte
	 * array, without caring about what session this CDAP message belongs to (and
	 * therefore it doesn't update any CDAP session state machine). Called by
	 * functions that have to relay CDAP messages, and need to
	 * encode its contents to make the relay decision and maybe modify some
	 * message values.
	 * @param cdapMessage
	 * @return
	 * @throws CDAPException
	 */
	virtual char* encodeCDAPMessage(const CDAPMessage &cdap_message)
			throw (CDAPException) = 0;
	/**
	 * Decodes a CDAP message. It just converts the byte array into a CDAP
	 * message, without caring about what session this CDAP message belongs to (and
	 * therefore it doesn't update any CDAP session state machine). Called by
	 * functions that have to relay CDAP messages, and need to serialize/
	 * decode its contents to make the relay decision and maybe modify some
	 * @param cdapMessage
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& decodeCDAPMessage(char cdap_message[])
			throw (CDAPException) = 0;
	/**
	 * Return the port_id of the (N-1) Flow that supports the CDAP Session
	 * with the IPC process identified by destinationApplicationProcessName and destinationApplicationProcessInstance
	 * @param destinationApplicationProcessName
	 * @throws CDAPException
	 */
	virtual int getPortId(std::string destination_application_process_name)
			throw (CDAPException) = 0;
	/**
	 * Create a M_CONNECT CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param auth_mech
	 * @param auth_value
	 * @param dest_ae_inst
	 * @param dest_ae_name
	 * @param dest_ap_inst
	 * @param dest_ap_name
	 * @param src_ae_inst
	 * @param src_ae_name
	 * @param src_ap_inst
	 * @param src_ap_name
	 * @param invoke_id true if one is requested, false otherwise
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getOpenConnectionRequestMessage(int port_id,
			CDAPMessage::AuthTypes auth_mech, const AuthValue &auth_value,
			std::string dest_ae_inst, std::string dest_ae_name,
			std::string dest_ap_inst, std::string dest_ap_name,
			std::string src_ae_inst, std::string src_ae_name,
			std::string src_ap_inst, std::string src_ap_name)
					throw (CDAPException) = 0;
	/**
	 * Create a M_CONNECT_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param auth_mech
	 * @param auth_value
	 * @param dest_ae_inst
	 * @param dest_ae_name
	 * @param dest_ap_inst
	 * @param dest_ap_name
	 * @param result
	 * @param result_reason
	 * @param src_ae_inst
	 * @param src_ae_name
	 * @param src_ap_inst
	 * @param src_ap_name
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getOpenConnectionResponseMessage(int port_id,
			CDAPMessage::AuthTypes auth_mech, const AuthValue &auth_value,
			std::string dest_ae_inst, std::string dest_ae_name,
			std::string dest_ap_inst, std::string dest_ap_name, int result,
			std::string result_reason, std::string src_ae_inst,
			std::string src_ae_name, std::string src_ap_inst,
			std::string src_ap_name, int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create an M_RELEASE CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getReleaseConnectionRequestMessage(int port_id,
			CDAPMessage::Flags flags, bool invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_RELEASE_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getReleaseConnectionResponseMessage(int port_id,
			CDAPMessage::Flags flags, int result, std::string result_reason,
			int invoke_id) throw (CDAPException);
	/**
	 * Create a M_CREATE CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param obj_value
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getCreateObjectRequestMessage(int port_id,
			char filter[], CDAPMessage::Flags flags, std::string obj_class,
			long obj_inst, std::string obj_name, const ObjectValue &obj_value,
			int scope, bool invoke_id) throw (CDAPException) = 0;
	/**
	 * Crate a M_CREATE_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param obj_value
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getCreateObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, std::string obj_class, long obj_inst,
			std::string obj_name, const ObjectValue &obj_value, int result,
			std::string result_reason, int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_DELETE CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param object_value
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getDeleteObjectRequestMessage(int port_id,
			char* filter, CDAPMessage::Flags flags, std::string obj_class,
			long obj_inst, std::string obj_name,
			const ObjectValue &object_value, int scope, bool invoke_id)
					throw (CDAPException) = 0;
	/**
	 * Create a M_DELETE_R CDAP MESSAGE
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getDeleteObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, std::string obj_class, long obj_inst,
			std::string obj_name, int result, std::string result_reason,
			int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_START CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_value
	 * @param obj_inst
	 * @param obj_name
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getStartObjectRequestMessage(int port_id,
			char filter[], CDAPMessage::Flags flags, std::string obj_class,
			const ObjectValue &obj_value, long obj_inst, std::string obj_name,
			int scope, bool invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_START_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getStartObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, int result, std::string result_reason,
			int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_START_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getStartObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, std::string obj_class,
			const ObjectValue &obj_value, long obj_inst, std::string obj_name,
			int result, std::string result_reason, int invoke_id)
					throw (CDAPException) = 0;
	/**
	 * Create a M_STOP CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_value
	 * @param obj_inst
	 * @param obj_name
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getStopObjectRequestMessage(int port_id,
			char* filter, CDAPMessage::Flags flags, std::string obj_class,
			const ObjectValue &obj_value, long obj_inst, std::string obj_name,
			int scope, bool invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_STOP_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getStopObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, int result, std::string result_reason,
			int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_READ CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getReadObjectRequestMessage(int port_id,
			char filter[], CDAPMessage::Flags flags, std::string obj_class,
			long obj_inst, std::string obj_name, int scope, bool invoke_id)
					throw (CDAPException) = 0;
	/**
	 * Crate a M_READ_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_name
	 * @param obj_value
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getReadObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, std::string obj_class, long obj_inst,
			std::string obj_name, const ObjectValue &obj_value, int result,
			std::string result_reason, int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_WRITE CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param filter
	 * @param flags
	 * @param obj_class
	 * @param obj_inst
	 * @param obj_value
	 * @param obj_name
	 * @param scope
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getWriteObjectRequestMessage(int port_id,
			char filter[], CDAPMessage::Flags flags, std::string obj_class,
			long obj_inst, const ObjectValue &obj_value, std::string obj_name,
			int scope, bool invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_WRITE_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param result
	 * @param result_reason
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getWriteObjectResponseMessage(int port_id,
			CDAPMessage::Flags flags, int result, std::string result_reason,
			int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_CANCELREAD CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param invoke_id
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getCancelReadRequestMessage(int port_id,
			CDAPMessage::Flags flags, int invoke_id) throw (CDAPException) = 0;
	/**
	 * Create a M_CANCELREAD_R CDAP Message
	 * @param port_id identifies the CDAP Session that this message is part of
	 * @param flags
	 * @param invoke_id
	 * @param result
	 * @param result_reason
	 * @return
	 * @throws CDAPException
	 */
	virtual const CDAPMessage& getCancelReadResponseMessage(int port_id,
			CDAPMessage::Flags flags, int invoke_id, int result,
			std::string result_reason) throw (CDAPException);
};

/**
 * Exceptions thrown by the RIB Daemon
 *
 */
class RIBDaemonException: public Exception {
	/*	Constants	*/
public:
	/** Error codes **/
	enum ErrorCode {
		UNKNOWN_OBJECT_CLASS,
		MALFORMED_MESSAGE_SUBSCRIPTION_REQUEST,
		MALFORMED_MESSAGE_UNSUBSCRIPTION_REQUEST,
		SUBSCRIBER_WAS_NOT_SUBSCRIBED,
		OBJECTCLASS_AND_OBJECT_NAME_OR_OBJECT_INSTANCE_NOT_SPECIFIED,
		OBJECTNAME_NOT_PRESENT_IN_THE_RIB,
		RESPONSE_REQUIRED_BUT_MESSAGE_HANDLER_IS_NULL,
		PROBLEMS_SENDING_CDAP_MESSAGE,
		OPERATION_NOT_ALLOWED_AT_THIS_OBJECT,
		UNRECOGNIZED_OBJECT_NAME,
		OBJECTCLASS_DOES_NOT_MATCH_OBJECTNAME,
		OBJECT_ALREADY_HAS_THIS_CHILD,
		CHILD_NOT_FOUND,
		OBJECT_ALREADY_EXISTS,
		RIB_OBJECT_AND_OBJECT_NAME_NULL,
		PROBLEMS_DECODING_OBJECT,
		OBJECT_VALUE_IS_NULL
	};
	/* Constructors and Destructors	 */
	RIBDaemonException();
	RIBDaemonException(ErrorCode error_code);
	RIBDaemonException(ErrorCode error_code, const char* error_message);
	/*	Members	*/
private:
	ErrorCode error_code_;
};

/**
 * Handles CDAP messages
 *
 */
class CDAPMessageHandlerInterface {
	/*	Constructors and Destructors	*/
public:
	virtual ~CDAPMessageHandlerInterface() {
	}
	;
	/*	Methods	*/
public:
	virtual void createResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void deleteResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void readResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void cancelReadResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void writeResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void startResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
	virtual void stopResponse(const CDAPMessage &cdap_message,
			const CDAPSessionDescriptor &cdap_session_descriptor)
					throw (RIBDaemonException) = 0;
};

}

#endif

#endif
