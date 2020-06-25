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
#include "fep_metamodel/fep_element.h"

namespace fep
{
namespace metamodel
{
	class FepParticipant;
	class FileReference;

	class FepSystem : public ErrorHandlerBase
	{
	private:
		bool _loaded = false;
	public:
		bool internalReadConfig(const a_util::xml::DOM&);
	public:
		std::string _schema_version;

		// Human readable name of the FEP System
		std::string  _name;

		// Part of the unique system identification
		std::string  _id;

		// Transfer information about the system to the user
		std::string _description;

		// Version of the FEP System Description file
		std::string _version;

		// Author of the FEP System Description file
		std::string _author;

		// List of all participant addresses with the element instances are loaded! Within FEP 2 the participants address and the element ids are the same. - We do not describe participants in here this now here - Within FEP 3 the participants address might be different to the elements id (because we change the state machine!)
		std::vector<FepParticipant> _participants;
	};

	class FepParticipant : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
		class ElementInstance : public ErrorHandlerBase
		{
		public:
			bool parseDOM(const a_util::xml::DOMElement&);
		public:
			std::string _id, _type;
			std::unique_ptr<FileReference> _input_mapping, _output_mapping, _timing;
		};
	public:
		std::string _address;
		unsigned _init_priority, _start_priority;
		ElementInstance _element_instance;
	};

	class FileReference : public ErrorHandlerBase
	{
	public:
		bool parseDOM(const a_util::xml::DOMElement&);
	public:
		// xs:anyURI
		std::string _file_reference;
	};

	class PropertyFile : public ErrorHandlerBase
	{
	private:
		bool _loaded = false;
	public:
		class ElementInstance : public ErrorHandlerBase
		{
		public:
			bool parseDOM(const a_util::xml::DOMElement&);
		public:
			std::string _id;
			std::vector<Property> _properties;
		};

		bool internalReadConfig(const a_util::xml::DOM&);
	public:
		// The schema version of the current file.
		std::string _schema_version;

		// Enumeration for all timing properties
		// System Properties will be set after the System is in IDLE(FEP2) (or LOADED(FEP3)) State
		std::vector<Property> _system_timing_properties;

		// Enumeration for all system properties
		// System Properties will be set after the System is in IDLE (FEP2) (or LOADED (FEP3)) State
		// System Properties will be set to the whole system (depending on technology used)
		std::vector<Property> _system_properties;

		// Enumeration for all properties of a uniquely named element_instance Enumeration for all system properties Element instance properties will be set after the System is in IDLE(FEP2) (or LOADED(FEP3)) State. this property list must NOT defined every element property out of the element_type!(only the one which changed)
		std::vector<ElementInstance> _element_instances_properties;
	};
}
} //end of fep namespace
