/**

   @copyright
   @verbatim
   Copyright @ 2019 Audi AG. All rights reserved.
   
       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
   
   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.
   
   You may add additional accurate notices of copyright ownership.
   @endverbatim
 */
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <a_util/xml/dom.h>

namespace fep
{
namespace metamodel
{
	inline std::pair<bool, std::string> noError() { return std::make_pair(true, std::string()); }

	std::pair<bool, std::string> parseString(const a_util::xml::DOMElement&, const char*, std::string&);
	std::pair<bool, std::string> parseNormalizedString(const a_util::xml::DOMElement&, const char*, std::string&, bool optional = false);
	std::pair<bool, std::string> parseNonemptyString(const a_util::xml::DOMElement&, const char*, std::string&, bool optional = false);
	std::pair<bool, std::string> parseVersionString(const a_util::xml::DOMElement&, const char*, std::string&, bool optional = false);
	std::pair<bool, std::string> parseUnsignedLongLong(const a_util::xml::DOMElement&, const char*, unsigned long long&, bool optional = false);
	std::pair<bool, std::string> parseUnsignedInt(const a_util::xml::DOMElement&, const char*, unsigned&, bool optional = false);
	std::pair<bool, std::string> parseURI(const a_util::xml::DOMElement&, const char*, std::string&, bool optional = false);
	std::pair<bool, std::string> parseConanCompatibleString(const a_util::xml::DOMElement&, const char*, std::string&, bool optional = false);
	std::pair<bool, std::string> parseBool(const a_util::xml::DOMElement&, const char*, bool&, bool optional = false);

	std::string tagPrefix(const char*);

	template<typename... Args>
	std::string tagPrefix(const char* first, Args... args)
	{
		return tagPrefix(first) + tagPrefix(args...);
	}

	std::string errorMsgRootnameMismatch(const char*, const char*);
	std::string errorMsgMissingTag(const char*);
	std::string errorMsgElementNameMismatch(const char*, const char*);
	std::string errorMsgUnknownTag(const char*);
	std::string errorMsgDuplicateElement(const char*);

	class ErrorHandlerBase
	{
	protected:
		std::string _error_msg;
	public:
		std::string getLastError() const;
	};

	class Property;
	class DataDescription;
	class Job;
	class ServiceInterface;
	class RequiredServiceInterface;
	class DataReference;

	class ConanIdentifier : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		std::string _name;
		std::string _version;
		std::string _user, _channel;
	};

	/**
	 * @brief FEP element type descriptor
	 */
	class FepElementType : public ErrorHandlerBase
	{
	private:
		bool _loaded = false;
	public:
		class ConfigurationInterface : public ErrorHandlerBase
		{
		public:
			bool addPropFromDomElement(const a_util::xml::DOMElement&);
		public:
			// Published properties of the element type
			std::vector<Property> _properties;
		};

		class DataInterfaces : public ErrorHandlerBase
		{
		public:
			bool addInputFromDomElement(const a_util::xml::DOMElement&);
			bool addOutputFromDomElement(const a_util::xml::DOMElement&);
		public:
			// Published data inputs
			std::vector<DataDescription> _inputs;

			// Published data outputs
			std::vector<DataDescription> _outputs;
		};

		class TimingInterface : public ErrorHandlerBase
		{
		public:
			bool addJobFromDomElement(const a_util::xml::DOMElement&);
		public:
			std::vector<Job> _jobs;
		};

		class ServiceInterfaces : public ErrorHandlerBase
		{
		public:
			bool addInterfaceFromDomElement(const a_util::xml::DOMElement&);
		public:
			std::vector<ServiceInterface> _service_interfaces;
		};

		class RequiredServiceInterfaces : public ErrorHandlerBase
		{
		public:
			bool addInterfaceFromDomElement(const a_util::xml::DOMElement&);
		public:
			std::vector<RequiredServiceInterface> _required_service_interfaces;
		};

		bool internalReadConfig(const a_util::xml::DOM&);
	public:
		// The schema version of the current file.
		std::string _schema_version;

		// The type name of the element
		ConanIdentifier _id;

		// The type name of the element by using a unique identifier
		std::string _display_name;

		// A short description of the element type
		std::string _description;

		// Author information for this element type
		std::string _author;

		// Optional category for this element type
		std::string _category;

		ConfigurationInterface _configuration_interface;

		DataInterfaces _data_interfaces;

		// Published default jobs of this element types and its default job settings
		TimingInterface _timing_interface;

		// Published service interfaces of the element type
		ServiceInterfaces _service_interfaces;

		// The service interfaces required of the element type. If used within a system description one of the other elements must provided this interface
		RequiredServiceInterfaces _required_service_interfaces;
	};

	class PropertyListEnumeration : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		std::string _name;
		unsigned long long _value;
	};

	class Property : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// Name of the property by PATH
		std::string _name;

		// The property type
		std::string _type;

		// The property value encoded as string
		std::string _value;
		std::vector<PropertyListEnumeration> _property_list_enumeration;
		bool _list_extendable;
	};

	class DatatypeDescription : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		std::string _metatype_name;
		std::vector<Property> _properties;
	};

	class DataDescription : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// The protocol of the interface
		std::string _name;

		// The data type description for input or output
		DatatypeDescription _type;
	};

	// A single data input/output of the job
	class DataReference : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// The reference to the input or output in data
		std::string _name;

		// The queue size used for scheduling
		unsigned long long _queue_size;
	};

	class Job : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
		class DataReferences : public ErrorHandlerBase
		{
		public:
			bool addInputFromDomElement(const a_util::xml::DOMElement&);
			bool addOutputFromDomElement(const a_util::xml::DOMElement&);
		public:
			// All data inputs of the job.
			std::vector<DataReference> _inputs;

			// All data outputs of the job.
			std::vector<DataReference> _outputs;
		};
	public:
		std::string _name;

		// microseconds in simulation time
		unsigned long long _cycle_time;

		// microseconds in simulation of the delay to the cycle begin
		unsigned long long _cycle_delay_time;

		// 0 = do not watch; >0 = the run_realtime will be watched (can be cycle_time as maximum)
		unsigned long long _max_run_realtime;

		// TS_IGNORE_RUNTIME_VIOLATION/TS_WARN_ABOUT_RUNTIME_VIOLATION/TS_SKIP_OUTPUT_PUBLISH/TS_SET_STM_TO_ERROR
		std::string _run_realtime_violation;

		std::unique_ptr<DataReferences> _data_references;
	};

	class InterfaceDescription : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// The protocol of the interface
		std::string _protocol;

		// The file reference to the description of the service interface.This depends on the protocol used - for JSON - RPC a reference to a json file must be provided.
		std::string _file_reference;
	};

	class ServiceInterface : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// The name of the published interface
		std::string _name;

		// The type of thes interface (fep2 will use 'configuration' - description of properties to set 'service' - service interface on the bus (usually described by a json file)
		std::string _type;

		// The version of the published interface
		std::string _version;

		// the interface description
		InterfaceDescription _interface_description;
	};

	class RequiredServiceInterface : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// The type of the interface required.
		std::string _type;

		// The version of the interface required
		std::string _version;

		// the interface description
		InterfaceDescription _interface_description;
	};

	/**
	 * @brief this file is an optional file to reconfigure the default timing_interface configuration.
	 */
	class FepElementTiming : public ErrorHandlerBase
	{
	private:
		bool _loaded = false;
	public:
		class Header : public ErrorHandlerBase
		{
		public:
			bool parseDOM(const a_util::xml::DOMElement&);
		public:
			// The author of the timing configuration.
			std::string _author;

			// The creation date of the timing configuration.
			std::string _date_creation;

			// The last change date of the timing configuration.
			std::string _date_change;

			//A short description of the timing configuration.
			std::string _description;
		};

		bool internalReadConfig(const a_util::xml::DOM&);
	public:
		// The schema version of the current file.
		std::string _schema_version;

		// The header section containing general information regarding the timing configuration.
		Header _header;

		// A configuration part of a single job of the element instance (it is important the job exists)
		std::vector<Job> _jobs;
	};
}
} //end of fep namespace
