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
#include "fep_metamodel/fep_system.h"

bool fep::metamodel::FepSystem::internalReadConfig(const a_util::xml::DOM& document)
{
    _loaded = false;
	_error_msg.clear();

    auto root = document.getRoot();
	static const char tag_name[] = "system";
	if (root.getName() != tag_name)
	{
		_error_msg = errorMsgRootnameMismatch(root.getName().c_str(), tag_name);
		return false;
	}

	std::pair<bool, std::string> result;

	if (!(result=parseVersionString(root, "schema_version", _schema_version)).first ||
		!(result=parseNonemptyString(root, "name", _name)).first ||
		!(result=parseNonemptyString(root, "id", _id)).first ||
		!(result=parseString(root, "description", _description)).first ||
		!(result=parseVersionString(root, "version", _version)).first || 
		!(result=parseNonemptyString(root, "author", _author)).first)
	{
		_error_msg = tagPrefix(tag_name) + result.second;
		return false;
	}

	{
		static const char child_tag_name[] = "participants";
		auto participants_element = root.getChild(child_tag_name);
		if (participants_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

		for (auto part_element : participants_element.getChildren())
		{
			static const char enum_tag_name[] = "participant";
			if (part_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(part_element.getName().c_str(), enum_tag_name);
				return false;
			}

			_participants.emplace_back();
			if (!_participants.back().parseDOM(part_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _participants.back().getLastError();
				return false;
			}
		}
	}

    return _loaded = true;
}

bool fep::metamodel::FepParticipant::parseDOM(const a_util::xml::DOMElement& participant_element)
{
	std::pair<bool, std::string> result;

    if (!(result=parseNonemptyString(participant_element, "address", _address)).first ||
		!(result=parseUnsignedInt(participant_element, "init_priority", _init_priority)).first ||
		!(result=parseUnsignedInt(participant_element, "start_priority", _start_priority)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "element_instance";
    auto element_instance_element = participant_element.getChild(child_tag_name);
	if (element_instance_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
	if (!_element_instance.parseDOM(element_instance_element))
	{
		_error_msg = tagPrefix(child_tag_name) + _element_instance.getLastError();
		return false;
	}
	return true;
}

bool fep::metamodel::FepParticipant::ElementInstance::parseDOM(const a_util::xml::DOMElement& element_instance_element)
{
	std::pair<bool, std::string> result;

    if (!(result=parseNonemptyString(element_instance_element, "id", _id)).first ||
		!(result=parseNonemptyString(element_instance_element, "type", _type)).first)
	{
		_error_msg = result.second;
		return false;
	}

	{
		static const char child_tag_name[] = "mappings";
		auto mappings_element = element_instance_element.getChild(child_tag_name);
    
		// mappings is optional
		if (!mappings_element.isNull())
		{
			for (auto mapping_element : mappings_element.getChildren())
			{
				static const char enum_tag_name_input[] = "input";
				static const char enum_tag_name_output[] = "output";
				if (mapping_element.getName() == enum_tag_name_input)
				{
					if (_input_mapping)
					{
						_error_msg = tagPrefix(child_tag_name) + errorMsgDuplicateElement(enum_tag_name_input);
						return false;
					}
					_input_mapping.reset(new FileReference);
					if (!_input_mapping->parseDOM(mapping_element))
					{
						_error_msg = tagPrefix(child_tag_name, enum_tag_name_input) + _input_mapping->getLastError();
						return false;
					}
				}
				else if (mapping_element.getName() == enum_tag_name_output)
				{
					if (_output_mapping)
					{
						_error_msg = tagPrefix(child_tag_name) + errorMsgDuplicateElement(enum_tag_name_output);
						return false;
					}
					_output_mapping.reset(new FileReference);
					if (!_output_mapping->parseDOM(mapping_element))
					{
						_error_msg = tagPrefix(child_tag_name, enum_tag_name_output) + _output_mapping->getLastError();
						return false;
					}
				}
				else
				{
					_error_msg = tagPrefix(child_tag_name) + errorMsgUnknownTag(mapping_element.getName().c_str());
					return false;
				}
			}
		}
	}
	{
		static const char child_tag_name[] = "timing";
		auto timing_element = element_instance_element.getChild(child_tag_name);
		if (!timing_element.isNull())
		{
			_timing.reset(new FileReference);
			if (!_timing->parseDOM(timing_element))
			{
				_error_msg = tagPrefix(child_tag_name) + _timing->getLastError();
				return false;
			}
		}
	}
    return true;
}

bool fep::metamodel::FileReference::parseDOM(const a_util::xml::DOMElement& fileref_element)
{
	std::pair<bool, std::string> result;

	if (!(result = parseURI(fileref_element, "file_reference", _file_reference)).first)
	{
		_error_msg = result.second;
		return false;
	}
	return true;
}

bool fep::metamodel::PropertyFile::internalReadConfig(const a_util::xml::DOM& document)
{
    _loaded = false;
	_error_msg.clear();

    auto root = document.getRoot();
	static const char tag_name[] = "property_file";
	if (root.getName() != tag_name)
	{
		_error_msg = errorMsgRootnameMismatch(root.getName().c_str(), tag_name);
		return false;
	}

	std::pair<bool, std::string> result;
	if (!(result=parseVersionString(root, "schema_version", _schema_version)).first)
	{
		_error_msg = tagPrefix(tag_name) + result.second;
		return false;
	}
    
    {
		static const char child_tag_name[] = "system_timing_properties";
        auto sys_timing_props_element = root.getChild(child_tag_name);
		if (sys_timing_props_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

        for (auto sys_timing_prop_element : sys_timing_props_element.getChildren())
        {
			static const char enum_tag_name[] = "property";
			if (sys_timing_prop_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(sys_timing_prop_element.getName().c_str(), enum_tag_name);
				return false;
			}
            _system_timing_properties.emplace_back();
			if (!_system_timing_properties.back().parseDOM(sys_timing_prop_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _system_timing_properties.back().getLastError();
				return false;
			}
        }
    }

	{
		static const char child_tag_name[] = "system_properties";
		auto sys_props_element = root.getChild(child_tag_name);
		if (sys_props_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}
        for (auto sys_prop_element : sys_props_element.getChildren())
        {
			static const char enum_tag_name[] = "property";
			if (sys_prop_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(sys_prop_element.getName().c_str(), enum_tag_name);
				return false;
			}
            _system_properties.emplace_back();
			if (!_system_properties.back().parseDOM(sys_prop_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _system_properties.back().getLastError();
				return false;
			}
        }
    }

    {
		static const char child_tag_name[] = "element_instances_properties";
        auto element_props_element = root.getChild(child_tag_name);
		if (element_props_element.isNull())
		{
			_error_msg = tagPrefix(tag_name) + errorMsgMissingTag(child_tag_name);
			return false;
		}

        for (auto element_prop_element : element_props_element.getChildren())
        {
			static const char enum_tag_name[] = "element_instance";
			if (element_prop_element.getName() != enum_tag_name)
			{
				_error_msg = tagPrefix(tag_name, child_tag_name) + errorMsgElementNameMismatch(element_prop_element.getName().c_str(), enum_tag_name);
				return false;
			}
            _element_instances_properties.emplace_back();
			if (!_element_instances_properties.back().parseDOM(element_prop_element))
			{
				_error_msg = tagPrefix(tag_name, child_tag_name, enum_tag_name) + _element_instances_properties.back().getLastError();
				return false;
			}
        }
    }
    return _loaded = true;
}

bool fep::metamodel::PropertyFile::ElementInstance::parseDOM(const a_util::xml::DOMElement& element_instance_element)
{
	std::pair<bool, std::string> result;
	if (!(result=parseNonemptyString(element_instance_element, "id", _id)).first)
	{
		_error_msg = result.second;
		return false;
	}

	static const char child_tag_name[] = "properties";
    auto properties_element = element_instance_element.getChild(child_tag_name);
	if (properties_element.isNull())
	{
		_error_msg = errorMsgMissingTag(child_tag_name);
		return false;
	}
    for (auto prop_element : properties_element.getChildren())
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
