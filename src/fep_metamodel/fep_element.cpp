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
#include "fep_metamodel/fep_element.h"
#include <a_util/regex.h>
#include <sstream>
#include <iomanip>

// returns true and overwrites dest if element found, else returns false and dest unchanged
std::pair<bool, std::string> fep::metamodel::parseString(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest)
{
	auto child_element = parent_element.getChild(element_name);
	if (child_element.isNull())
	{
		return std::make_pair(false, errorMsgMissingTag(element_name));
	}
	dest = child_element.getData();
	return noError();
}

std::pair<bool,std::string> fep::metamodel::parseNormalizedString(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest, bool optional)
{
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest);
	if (!ret.first)
	{
		if (optional) ret.first = true;
		return ret;
	}
	auto invalid_char_pos = dest.find_first_of("\t\n\r");
	if (invalid_char_pos == std::string::npos)
	{
		return noError();
	}

	std::stringstream error_message;
	error_message << "element \"" << element_name << "\" contains invalid character at position " << invalid_char_pos << " (";
	error_message << std::hex << int(dest[invalid_char_pos]) << ")";
	return std::make_pair(false, error_message.str());
}

std::pair<bool, std::string> fep::metamodel::parseNonemptyString(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest, bool optional)
{
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest);
	if (!ret.first)
	{
		if (optional) ret.first = true;
		return ret;
	}
	if (!dest.empty())
	{
		return noError();
	}
	return std::make_pair(false, std::string("element \"") + element_name + "\" is empty");
}

std::pair<bool, std::string> fep::metamodel::parseVersionString(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest, bool optional)
{
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest);
	if (!ret.first)
	{
		if (optional)
		{
			ret.first = true;
		}
		return ret;
	}
	static const a_util::regex::RegularExpression version_regex("[0-9]+\\.[0-9]+\\.[0-9]+");
	if (version_regex.fullMatch(dest))
	{
		return noError();
	}
	return std::make_pair(false, std::string("element \"") + element_name + "\" ("+ dest +") is not properly formatted version string");
}

namespace
{
	// no default type name
	template<typename T>
	struct MyTypeName
	{
	};

	template<>
	struct MyTypeName<unsigned>
	{
		static const char* getName()
		{
			return "unsigned integer";
		}
	};

	template<>
	struct MyTypeName<unsigned long long>
	{
		static const char* getName()
		{
			return "unsigned long long";
		}
	};

	template<typename T>
	std::pair<bool, std::string> parseWithSStream(const a_util::xml::DOMElement& parent_element, const char* element_name, T& dest, bool optional)
	{
		std::string dest_string;
		std::pair<bool, std::string> ret = fep::metamodel::parseString(parent_element, element_name, dest_string);
		if (!ret.first)
		{
			if (optional) ret.first = true;
			return ret;
		}

		std::stringstream dest_stream(dest_string);
		dest_stream >> dest >> std::ws;
		if (!dest_stream.fail() && dest_stream.eof())
		{
			return fep::metamodel::noError();
		}
		return std::make_pair(false, std::string("element \"") + element_name + "\" cannot be interpreted as " + MyTypeName<T>::getName());
	}

	bool isConanCompatibleString(const std::string& dest)
	{
		if (dest.size() < 2u || dest.size() > 51u)
		{
			return false;
		}
		static const a_util::regex::RegularExpression conan_pattern_regex("[0-9a-zA-Z._-]+");
		return conan_pattern_regex.fullMatch(dest);
	}
} // end of unnamed namespace

std::pair<bool, std::string> fep::metamodel::parseUnsignedLongLong(const a_util::xml::DOMElement& parent_element, const char* element_name, unsigned long long& dest, bool optional)
{
	return parseWithSStream(parent_element, element_name, dest, optional);
}


std::pair<bool, std::string> fep::metamodel::parseUnsignedInt(const a_util::xml::DOMElement& parent_element, const char* element_name, unsigned& dest, bool optional)
{
	return parseWithSStream(parent_element, element_name, dest, optional);
}

std::pair<bool, std::string> fep::metamodel::parseURI(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest, bool optional)
{
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest);
	if (!ret.first)
	{
		if (optional) ret.first = true;
		return ret;
	}

	static const a_util::regex::RegularExpression uri_regex("\\w+:/?/?[^\\s]+");
	static const a_util::regex::RegularExpression file_regex("[^\\s]+");

	if (uri_regex.fullMatch(dest) || file_regex.fullMatch(dest))
	{
		return noError();
	}
	return std::make_pair(false, std::string("element \"") + element_name + "\" cannot be interpreted as URI");
}

std::pair<bool, std::string> fep::metamodel::parseConanCompatibleString(const a_util::xml::DOMElement& parent_element, const char* element_name, std::string& dest, bool optional)
{
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest);
	if (!ret.first)
	{
		if (optional) ret.first = true;
		return ret;
	}
	if (isConanCompatibleString(dest)) return noError();
	return std::make_pair(false, std::string("element \"") + element_name + "\" is not conan compatible string");
}

std::pair<bool, std::string> fep::metamodel::parseBool(const a_util::xml::DOMElement& parent_element, const char* element_name, bool& dest, bool optional)
{
	std::string dest_string;
	std::pair<bool, std::string> ret = parseString(parent_element, element_name, dest_string);
	if (!ret.first)
	{
		if (optional) ret.first = true;
		return ret;
	}
	if (dest_string == "true")
	{
		dest = true;
	}
	else if (dest_string == "false")
	{
		dest = false;
	}
	else
	{
		return std::make_pair(false, std::string("element \"") + element_name + "\" is not a boolean value");
	}
	return noError();
}

std::string fep::metamodel::tagPrefix(const char* element_name)
{
	return std::string("<") + element_name + ">: ";
}

std::string fep::metamodel::errorMsgRootnameMismatch(const char *got, const char *expected)
{
	return std::string("Root element is \"") + got + "\" instead of \""+ expected +"\"";
}

std::string fep::metamodel::errorMsgMissingTag(const char* missing)
{
	return std::string("element \"") + missing + "\" not found";
}

std::string fep::metamodel::errorMsgElementNameMismatch(const char *got, const char *expected)
{
	return std::string("Element name is \"") + got + "\" instead of \"" + expected + "\"";
}

std::string fep::metamodel::errorMsgUnknownTag(const char *got)
{
	return std::string("Unknown element \"") + got + "\"";
}

std::string fep::metamodel::errorMsgDuplicateElement(const char *got)
{
	return std::string("Duplicate element \"") + got + "\"";
}

std::string fep::metamodel::ErrorHandlerBase::getLastError() const
{
	return _error_msg;
}

bool fep::metamodel::FepElementType::internalReadConfig(const a_util::xml::DOM& document)
{
	_loaded = false;
	_error_msg.clear();

	auto root = document.getRoot();
	static const char tag_name[] = "element_type";

	if (root.getName() != tag_name)
	{
		_error_msg = errorMsgRootnameMismatch(root.getName().c_str(), tag_name);
		return false;
	}

	std::pair<bool, std::string> result;
	if (!(result = parseVersionString(root, "schema_version", _schema_version)).first)
	{
		_error_msg = tagPrefix(tag_name) + result.second;
		return false;
	}

	{
		static const char child_tag_name[] = "id";
		auto id_element = root.getChild(child_tag_name);
		if (id_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
		if (!_id.parseDOM(id_element))
		{
			_error_msg = tagPrefix(tag_name, child_tag_name) + _id.getLastError();
			return false;
		}
	}

	if (!(result=parseNonemptyString(root, "display_name", _display_name)).first ||
		!(result=parseString(root, "description", _description)).first ||
		!(result=parseNormalizedString(root, "author", _author)).first ||
		!(result=parseNonemptyString(root, "category", _category, true)).first)
	{
		_error_msg = tagPrefix(tag_name) + result.second;
		return false;
	}

	// CONFIGURATION INTERFACE
	{
		static const char child_tag_name[] = "configuration_interface";
		auto conf_if_element = root.getChild(child_tag_name);
		if (conf_if_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
		{
			static const char grandchild_tag_name[] = "properties";
			auto properties_element = conf_if_element.getChild(grandchild_tag_name);
			if (properties_element.isNull())
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
				return false;
			}
			for (auto prop_element : properties_element.getChildren())
			{
				static const char enum_tag_name[] = "property";
				if (prop_element.getName() != enum_tag_name)
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(prop_element.getName().c_str(), enum_tag_name);
					return false;
				}
				if (!_configuration_interface.addPropFromDomElement(prop_element))
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name, enum_tag_name) + _configuration_interface.getLastError();
					return false;
				}
			}
		}
	}
	
	// DATA INTERFACES
	{
		static const char child_tag_name[] = "data_interfaces";
		auto data_ifs_element = root.getChild(child_tag_name);
		if (data_ifs_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

		{
			static const char grandchild_tag_name[] = "inputs";
			auto inputs_element = data_ifs_element.getChild(grandchild_tag_name);
			if (inputs_element.isNull())
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
				return false;
			}
			for (auto in_element : inputs_element.getChildren())
			{
				static const char enum_tag_name[] = "input";
				if (in_element.getName() != enum_tag_name)
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(in_element.getName().c_str(), enum_tag_name);
					return false;
				}
				if (!_data_interfaces.addInputFromDomElement(in_element))
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name, enum_tag_name) + _data_interfaces.getLastError();
					return false;
				}
			}
		}

		{
			static const char grandchild_tag_name[] = "outputs";
			auto outputs_element = data_ifs_element.getChild(grandchild_tag_name);
			if (outputs_element.isNull())
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
				return false;
			}
			for (auto out_element : outputs_element.getChildren())
			{
				static const char enum_tag_name[] = "output";
				if (out_element.getName() != enum_tag_name)
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(out_element.getName().c_str(), enum_tag_name);
					return false;
				}
				if (!_data_interfaces.addOutputFromDomElement(out_element))
				{
					_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name, enum_tag_name) + _data_interfaces.getLastError();
					return false;
				}
			}
		}
	}

	// TIMING INTERFACE
	{
		static const char child_tag_name[] = "timing_interface";
		auto timing_if_element = root.getChild(child_tag_name);
		if (timing_if_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

		static const char grandchild_tag_name[] = "jobs";
		auto jobs_element = timing_if_element.getChild(grandchild_tag_name);
		if (jobs_element.isNull())
		{
			_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
			return false;
		}
		for (auto job_element : jobs_element.getChildren())
		{
			static const char enum_tag_name[] = "job";
			if (job_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(job_element.getName().c_str(), enum_tag_name);
				return false;
			}
			if (!_timing_interface.addJobFromDomElement(job_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, grandchild_tag_name, enum_tag_name) + _timing_interface.getLastError();
				return false;
			}
		}
	}

	// SERVICE INTERFACES
	{
		static const char child_tag_name[] = "service_interfaces";
		auto service_ifs_element = root.getChild(child_tag_name);
		if (service_ifs_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
		for (auto service_element : service_ifs_element.getChildren())
		{
			static const char enum_tag_name[] = "service_interface";
			if (service_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(service_element.getName().c_str(), enum_tag_name);
				return false;
			}
			if (!_service_interfaces.addInterfaceFromDomElement(service_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _service_interfaces.getLastError();
				return false;
			}
		}
	}

	// REQUIRED SERVICE INTERFACES
	{
		static const char child_tag_name[] = "required_service_interfaces";
		auto req_service_ifs_element = root.getChild(child_tag_name);
		if (req_service_ifs_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

		for (auto req_service_element : req_service_ifs_element.getChildren())
		{
			static const char enum_tag_name[] = "required_service_interface";
			if (req_service_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(req_service_element.getName().c_str(), enum_tag_name);
				return false;
			}
			if (!_required_service_interfaces.addInterfaceFromDomElement(req_service_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _required_service_interfaces.getLastError();
				return false;
			}
		}
	}

	return _loaded = true;
}

bool fep::metamodel::FepElementType::ConfigurationInterface::addPropFromDomElement(const a_util::xml::DOMElement& prop_element)
{
	_properties.emplace_back();
	if (!_properties.back().parseDOM(prop_element))
	{
		_error_msg = _properties.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::Property::parseDOM(const a_util::xml::DOMElement& prop_element)
{
	std::pair<bool, std::string> result;

	if (!(result=parseNonemptyString(prop_element, "name", _name)).first ||
		!(result=parseNonemptyString(prop_element, "type", _type)).first ||
		!(result=parseString(prop_element, "value", _value)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "list";
	auto list_element = prop_element.getChild(child_tag_name);
	if (!list_element.isNull()) // optional element
	{
		for (auto prop_enum_element : list_element.getChildren())
		{
			static const char enum_tag_name[] = "property_list_enumeration";
			if (prop_enum_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(child_tag_name) + errorMsgElementNameMismatch(prop_enum_element.getName().c_str(), enum_tag_name);
				return false;
			}

			_property_list_enumeration.emplace_back();
			if (!_property_list_enumeration.back().parseDOM(prop_enum_element))
			{
				_error_msg = tagPrefix(child_tag_name, enum_tag_name) + _property_list_enumeration.back().getLastError();
				return false;
			}
		}
	}
	_list_extendable = false;
	if (!(result = parseBool(prop_element, "list_extendable", _list_extendable, true)).first)
	{
		_error_msg = tagPrefix(child_tag_name) + result.second;
		return false;
	}
	return true;
}

bool fep::metamodel::PropertyListEnumeration::parseDOM(const a_util::xml::DOMElement& prop_enum_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseNonemptyString(prop_enum_element, "name", _name)).first ||
		!(result = parseUnsignedLongLong(prop_enum_element, "value", _value)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}


bool fep::metamodel::FepElementType::DataInterfaces::addInputFromDomElement(const a_util::xml::DOMElement& input_element)
{
	_inputs.emplace_back();
	if (!_inputs.back().parseDOM(input_element))
	{
		_error_msg = _inputs.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::FepElementType::DataInterfaces::addOutputFromDomElement(const a_util::xml::DOMElement& output_element)
{
	_outputs.emplace_back();
	if (!_outputs.back().parseDOM(output_element))
	{
		_error_msg = _outputs.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::DataDescription::parseDOM(const a_util::xml::DOMElement& datadesc_element)
{
	std::pair<bool, std::string> result;
	if (!(result=parseNonemptyString(datadesc_element, "name", _name)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "type";
	auto type_element = datadesc_element.getChild(child_tag_name);
	if (type_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
	if (!_type.parseDOM(type_element))
	{
		_error_msg = tagPrefix(child_tag_name) + _type.getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::DatatypeDescription::parseDOM(const a_util::xml::DOMElement& datatype_desc_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseNonemptyString(datatype_desc_element, "metatype_name", _metatype_name)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "properties";
	auto props_element = datatype_desc_element.getChild(child_tag_name);
	if (props_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
	for (auto prop_element : props_element.getChildren())
	{
		static const char enum_tag_name[] = "property";
		if (prop_element.getName() != enum_tag_name)
		{
			_error_msg = tagPrefix(child_tag_name) + errorMsgElementNameMismatch(prop_element.getName().c_str(), enum_tag_name);
			return false;
		}
		_properties.emplace_back();
		if (!_properties.back().parseDOM(prop_element))
		{
			_error_msg = tagPrefix(child_tag_name, enum_tag_name) + _properties.back().getLastError();
			return false;
		}
	}
	return true;
}

bool fep::metamodel::FepElementType::TimingInterface::addJobFromDomElement(const a_util::xml::DOMElement& job_element)
{
	_jobs.emplace_back();
	if (!_jobs.back().parseDOM(job_element))
	{
		_error_msg = _jobs.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::Job::parseDOM(const a_util::xml::DOMElement& job_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseNonemptyString(job_element, "name", _name)).first ||
		!(result=parseUnsignedLongLong(job_element, "cycle_time", _cycle_time)).first ||
		!(result=parseUnsignedLongLong(job_element, "cycle_delay_time", _cycle_delay_time)).first ||
		!(result=parseUnsignedLongLong(job_element, "max_run_realtime", _max_run_realtime)).first)
	{
		_error_msg = result.second;
		return false;
	}

	{
		static const char child_tag_name[] = "run_realtime_violation";
		if (!(result = parseString(job_element, child_tag_name, _run_realtime_violation)).first)
		{
			_error_msg = result.second;
			return false;
		}
		if (_run_realtime_violation != "TS_IGNORE_RUNTIME_VIOLATION" &&
			_run_realtime_violation != "TS_WARN_ABOUT_RUNTIME_VIOLATION" &&
			_run_realtime_violation != "TS_SKIP_OUTPUT_PUBLISH" &&
			_run_realtime_violation != "TS_SET_STM_TO_ERROR")
		{
			_error_msg = tagPrefix(child_tag_name) + "not a run_realtime_violation_type option";
			return false;
		}
	}

	{
		static const char child_tag_name[] = "data_references";
		auto dataref_element = job_element.getChild(child_tag_name);
		if (!dataref_element.isNull())
		{
			_data_references.reset(new DataReferences);

			{
				static const char grandchild_tag_name[] = "inputs";
				auto input_elements = dataref_element.getChild(grandchild_tag_name);
				if (input_elements.isNull())
				{
					_error_msg = tagPrefix(child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
					return false;
				}
				for (auto in_element : input_elements.getChildren())
				{
					static const char enum_tag_name[] = "input_reference";
					if (in_element.getName() != enum_tag_name)
					{
						_error_msg = tagPrefix(child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(in_element.getName().c_str(), enum_tag_name);
						return false;
					}
					if (!_data_references->addInputFromDomElement(in_element))
					{
						_error_msg = tagPrefix(child_tag_name, grandchild_tag_name, enum_tag_name) + _data_references->getLastError();
						return false;
					}
				}
			}
			{
				static const char grandchild_tag_name[] = "outputs";
				auto output_elements = dataref_element.getChild(grandchild_tag_name);
				if (output_elements.isNull())
				{
					_error_msg = tagPrefix(child_tag_name) + errorMsgMissingTag(grandchild_tag_name);
					return false;
				}
				for (auto out_element : output_elements.getChildren())
				{
					static const char enum_tag_name[] = "output_reference";
					if (out_element.getName() != enum_tag_name)
					{
						_error_msg = tagPrefix(child_tag_name, grandchild_tag_name) + errorMsgElementNameMismatch(out_element.getName().c_str(), enum_tag_name);
						return false;
					}
					if (!_data_references->addOutputFromDomElement(out_element))
					{
						_error_msg = tagPrefix(child_tag_name, grandchild_tag_name, enum_tag_name) + _data_references->getLastError();
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool fep::metamodel::Job::DataReferences::addInputFromDomElement(const a_util::xml::DOMElement& input_element)
{
	_inputs.emplace_back();
	if (!_inputs.back().parseDOM(input_element))
	{
		_error_msg = _inputs.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::Job::DataReferences::addOutputFromDomElement(const a_util::xml::DOMElement& output_element)
{
	_outputs.emplace_back();
	if (!_outputs.back().parseDOM(output_element))
	{
		_error_msg = _outputs.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::DataReference::parseDOM(const a_util::xml::DOMElement& dataref_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseNonemptyString(dataref_element, "name", _name)).first ||
		!(result = parseUnsignedLongLong(dataref_element, "queue_size", _queue_size)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}

bool fep::metamodel::FepElementType::ServiceInterfaces::addInterfaceFromDomElement(const a_util::xml::DOMElement& if_element)
{
	_service_interfaces.emplace_back();
	if (!_service_interfaces.back().parseDOM(if_element))
	{
		_error_msg = _service_interfaces.back().getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::FepElementType::RequiredServiceInterfaces::addInterfaceFromDomElement(const a_util::xml::DOMElement& if_element)
{
	_required_service_interfaces.emplace_back();
	if (!_required_service_interfaces.back().parseDOM(if_element))
	{
		_error_msg = _required_service_interfaces.back().getLastError();
		return false;
	}
	return true;
}


bool fep::metamodel::ServiceInterface::parseDOM(const a_util::xml::DOMElement& service_element)
{
	std::pair<bool, std::string> result;
	if (!(result=parseNonemptyString(service_element, "name", _name)).first ||
		!(result=parseNonemptyString(service_element, "type", _type)).first ||
		!(result=parseVersionString(service_element, "version", _version)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "interface_description";
	auto if_desc_element = service_element.getChild(child_tag_name);
	if (if_desc_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
	if (!_interface_description.parseDOM(if_desc_element))
	{
		_error_msg = tagPrefix(child_tag_name) + _interface_description.getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::RequiredServiceInterface::parseDOM(const a_util::xml::DOMElement& req_service_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseNonemptyString(req_service_element, "type", _type)).first ||
		!(result = parseVersionString(req_service_element, "version", _version)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "interface_description";
	auto if_desc_element = req_service_element.getChild(child_tag_name);
	if (if_desc_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
	if (!_interface_description.parseDOM(if_desc_element))
	{
		_error_msg = tagPrefix(child_tag_name) + _interface_description.getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::InterfaceDescription::parseDOM(const a_util::xml::DOMElement& if_desc_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseString(if_desc_element, "protocol", _protocol)).first ||
		!(result = parseString(if_desc_element, "file_reference", _file_reference)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}

bool fep::metamodel::ConanIdentifier::parseDOM(const a_util::xml::DOMElement& conan_identifier_element)
{
	std::pair<bool, std::string> result;
	if (!(result = parseConanCompatibleString(conan_identifier_element, "name", _name)).first ||
		!(result = parseVersionString(conan_identifier_element, "version", _version)).first)
	{
		_error_msg = result.second;
		return false;
	}
	_user = "vwgroup";
	if (!(result = parseConanCompatibleString(conan_identifier_element, "user", _user, true)).first)
	{
		_error_msg = result.second;
		return false;
	}
	_channel = "stable";
	if (!(result = parseConanCompatibleString(conan_identifier_element, "channel", _channel, true)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}

bool fep::metamodel::FepElementTiming::internalReadConfig(const a_util::xml::DOM& document)
{
	_loaded = false;
	_error_msg.clear();

	auto root = document.getRoot();
	static const char tag_name[] = "element_timing";
	if (root.getName() != tag_name)
	{
		_error_msg = errorMsgRootnameMismatch(root.getName().c_str(), tag_name);
		return false;
	}

	std::pair<bool, std::string> result;
	if (!(result = parseVersionString(root, "schema_version", _schema_version)).first)
	{
		_error_msg = tagPrefix(tag_name) + result.second;
		return false;
	}

	{
		static const char child_tag_name[] = "header";
		auto header_element = root.getChild(child_tag_name);
		if (header_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
		if (!_header.parseDOM(header_element))
		{
			_error_msg = tagPrefix(tag_name, child_tag_name) + _header.getLastError();
			return false;
		}
	}

	{
		static const char child_tag_name[] = "jobs";
		auto jobs_element = root.getChild(child_tag_name);
		if (jobs_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
		for (auto job_element : jobs_element.getChildren())
		{
			static const char enum_tag_name[] = "job";
			if (job_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(job_element.getName().c_str(), enum_tag_name);
				return false;
			}
			_jobs.emplace_back();
			if (!_jobs.back().parseDOM(job_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _jobs.back().getLastError();
				return false;
			}
		}
	}
	return _loaded = true;
}

bool fep::metamodel::FepElementTiming::Header::parseDOM(const a_util::xml::DOMElement& header_element)
{
	std::pair<bool, std::string> result;
	if (!(result=parseString(header_element, "author", _author)).first ||
		!(result=parseString(header_element, "date_creation", _date_creation)).first ||
		!(result=parseString(header_element, "date_change", _date_change)).first ||
		!(result=parseString(header_element, "description", _description)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}
