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
#include <gtest/gtest.h>
#include "fep_metamodel/fep_system.h"

using namespace fep::metamodel;

TEST(FEPSystemSDKTester, testOK)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_TRUE(parsed);

	EXPECT_EQ(fep_system._schema_version, "2.0.0");
	EXPECT_EQ(fep_system._name, "Signal Mapping System");
	EXPECT_EQ(fep_system._id, "signal_mapping_system");
	EXPECT_EQ(fep_system._description, "This is an example of a FEP system.");
	EXPECT_EQ(fep_system._version, "1.0.0");
	EXPECT_EQ(fep_system._author, "fep_team");

	ASSERT_EQ(fep_system._participants.size(), 5u);

	EXPECT_EQ(fep_system._participants[0]._address, "x1");
	EXPECT_EQ(fep_system._participants[0]._init_priority, 1u);
	EXPECT_EQ(fep_system._participants[0]._start_priority, 1u);
	EXPECT_EQ(fep_system._participants[0]._element_instance._id, "x1");
	EXPECT_EQ(fep_system._participants[0]._element_instance._type, "type_id");
	ASSERT_TRUE(fep_system._participants[0]._element_instance._input_mapping);
	EXPECT_EQ(fep_system._participants[0]._element_instance._input_mapping->_file_reference, "x1_input.map");
	ASSERT_TRUE(fep_system._participants[0]._element_instance._output_mapping);
	EXPECT_EQ(fep_system._participants[0]._element_instance._output_mapping->_file_reference, "x1_output.map");
	ASSERT_TRUE(fep_system._participants[0]._element_instance._timing);
	EXPECT_EQ(fep_system._participants[0]._element_instance._timing->_file_reference, "x1.fep_element_timing");

	EXPECT_EQ(fep_system._participants[1]._address, "x2");
	EXPECT_EQ(fep_system._participants[1]._init_priority, 2u);
	EXPECT_EQ(fep_system._participants[1]._start_priority, 2u);
	EXPECT_EQ(fep_system._participants[1]._element_instance._id, "x2");
	EXPECT_EQ(fep_system._participants[1]._element_instance._type, "type_id");
	ASSERT_FALSE(fep_system._participants[1]._element_instance._input_mapping);
	ASSERT_TRUE(fep_system._participants[1]._element_instance._output_mapping);
	ASSERT_EQ(fep_system._participants[1]._element_instance._output_mapping->_file_reference, "x2_output.map");
	ASSERT_FALSE(fep_system._participants[1]._element_instance._timing);

	EXPECT_EQ(fep_system._participants[2]._address, "y1");
	EXPECT_EQ(fep_system._participants[2]._init_priority, 3u);
	EXPECT_EQ(fep_system._participants[2]._start_priority, 3u);
	EXPECT_EQ(fep_system._participants[2]._element_instance._id, "y1");
	EXPECT_EQ(fep_system._participants[2]._element_instance._type, "type_id");
	ASSERT_TRUE(fep_system._participants[2]._element_instance._input_mapping);
	EXPECT_EQ(fep_system._participants[2]._element_instance._input_mapping->_file_reference, "y1_input.map");
	ASSERT_FALSE(fep_system._participants[2]._element_instance._output_mapping);
	ASSERT_FALSE(fep_system._participants[2]._element_instance._timing);

	EXPECT_EQ(fep_system._participants[3]._address, "y2");
	EXPECT_EQ(fep_system._participants[3]._init_priority, 4u);
	EXPECT_EQ(fep_system._participants[3]._start_priority, 4u);
	EXPECT_EQ(fep_system._participants[3]._element_instance._id, "y2");
	EXPECT_EQ(fep_system._participants[3]._element_instance._type, "type_id");
	ASSERT_TRUE(fep_system._participants[3]._element_instance._input_mapping);
	EXPECT_EQ(fep_system._participants[3]._element_instance._input_mapping->_file_reference, "y2_input.map");
	ASSERT_FALSE(fep_system._participants[3]._element_instance._output_mapping);
	ASSERT_FALSE(fep_system._participants[3]._element_instance._timing);

	EXPECT_EQ(fep_system._participants[4]._address, "w");
	EXPECT_EQ(fep_system._participants[4]._init_priority, 5u);
	EXPECT_EQ(fep_system._participants[4]._start_priority, 5u);
	EXPECT_EQ(fep_system._participants[4]._element_instance._id, "w");
	EXPECT_EQ(fep_system._participants[4]._element_instance._type, "type_id");
	ASSERT_FALSE(fep_system._participants[4]._element_instance._input_mapping);
	ASSERT_TRUE(fep_system._participants[4]._element_instance._output_mapping);
	EXPECT_EQ(fep_system._participants[4]._element_instance._output_mapping->_file_reference, "w_output.map");
	ASSERT_FALSE(fep_system._participants[4]._element_instance._timing);
}

TEST(FEPSystemSDKTester, testError1)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk_bad_1");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), tagPrefix("system", "participants", "participant", "element_instance", "mappings") + errorMsgUnknownTag("null"));

}

TEST(FEPSystemSDKTester, testError2)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk_bad_2");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), tagPrefix("system", "participants", "participant", "element_instance", "mappings", "input") +
		"element \"file_reference\" cannot be interpreted as URI");
}

TEST(FEPSystemSDKTester, testError3)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk_bad_3");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), tagPrefix("system", "participants", "participant") + "element \"init_priority\" cannot be interpreted as unsigned integer");
}

TEST(FEPSystemSDKTester, testError4)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk_bad_4");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), tagPrefix("system", "participants", "participant", "element_instance", "mappings") + errorMsgDuplicateElement("input"));
}

TEST(FEPSystemSDKTester, testError5)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk_bad_5");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), tagPrefix("system", "participants", "participant", "element_instance", "mappings") + errorMsgDuplicateElement("output"));
}

TEST(FEPSystemSDKTester, testErrorRootname)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_properties");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepSystem fep_system;
	const bool parsed = fep_system.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_system.getLastError(), errorMsgRootnameMismatch("property_file", "system"));
}

TEST(FEPSystemPropertiesTester, testOK)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_properties");
	ASSERT_TRUE(loaded);

	fep::metamodel::PropertyFile prop_file;
	const bool parsed = prop_file.internalReadConfig(dom);
	ASSERT_TRUE(parsed);

	EXPECT_EQ(prop_file._schema_version, "2.0.0");
	ASSERT_EQ(prop_file._system_timing_properties.size(), 5u);
	EXPECT_EQ(prop_file._system_timing_properties[0]._name, "timing_configuration_type");
	EXPECT_EQ(prop_file._system_timing_properties[0]._type, "string");
	EXPECT_EQ(prop_file._system_timing_properties[0]._value, "Timing2SystemTime");
	EXPECT_EQ(prop_file._system_timing_properties[1]._name, "master_element_id");
	EXPECT_EQ(prop_file._system_timing_properties[1]._type, "string");
	EXPECT_EQ(prop_file._system_timing_properties[1]._value, "element1");
	EXPECT_EQ(prop_file._system_timing_properties[2]._name, "master_clock_name");
	EXPECT_EQ(prop_file._system_timing_properties[2]._type, "string");
	EXPECT_EQ(prop_file._system_timing_properties[2]._value, "element1");
	EXPECT_EQ(prop_file._system_timing_properties[3]._name, "master_time_factor");
	EXPECT_EQ(prop_file._system_timing_properties[3]._type, "double");
	EXPECT_EQ(prop_file._system_timing_properties[3]._value, "1.0");
	EXPECT_EQ(prop_file._system_timing_properties[4]._name, "master_time_stepsize");
	EXPECT_EQ(prop_file._system_timing_properties[4]._type, "uint");
	EXPECT_EQ(prop_file._system_timing_properties[4]._value, "1");

	ASSERT_EQ(prop_file._system_properties.size(), 1u);
	EXPECT_EQ(prop_file._system_properties[0]._name, "system_parameter");
	EXPECT_EQ(prop_file._system_properties[0]._type, "int");
	EXPECT_EQ(prop_file._system_properties[0]._value, "42");


	ASSERT_EQ(prop_file._element_instances_properties.size(), 2u);

	EXPECT_EQ(prop_file._element_instances_properties[0]._id, "FrontSensor");
	ASSERT_EQ(prop_file._element_instances_properties[0]._properties.size(), 4u);
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[0]._name, "sensor_position/X");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[0]._type, "float");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[0]._value, "1.234");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[1]._name, "sensor_position/Y");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[1]._type, "float");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[1]._value, "0.00");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[2]._name, "sensor_position/Z");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[2]._type, "float");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[2]._value, "5.00");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[3]._name, "parameter1");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[3]._type, "string");
	EXPECT_EQ(prop_file._element_instances_properties[0]._properties[3]._value, "value1");

	EXPECT_EQ(prop_file._element_instances_properties[1]._id, "BackSensor");
	ASSERT_EQ(prop_file._element_instances_properties[1]._properties.size(), 1u);
	EXPECT_EQ(prop_file._element_instances_properties[1]._properties[0]._name, "sensor_position/X");
	EXPECT_EQ(prop_file._element_instances_properties[1]._properties[0]._type, "float");
	EXPECT_EQ(prop_file._element_instances_properties[1]._properties[0]._value, "1.0");
}

TEST(FEPSystemPropertiesTester, testError1)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_properties_bad_1");
	ASSERT_TRUE(loaded);

	fep::metamodel::PropertyFile prop_file;
	const bool parsed = prop_file.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(prop_file.getLastError(), tagPrefix("property_file", "system_timing_properties", "property") + errorMsgMissingTag("type"));
}

TEST(FEPSystemPropertiesTester, testError2)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_properties_bad_2");
	ASSERT_TRUE(loaded);

	fep::metamodel::PropertyFile prop_file;
	const bool parsed = prop_file.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(prop_file.getLastError(), tagPrefix("property_file", "system_timing_properties") + errorMsgElementNameMismatch("properti", "property"));
}

TEST(FEPSystemPropertiesTester, testError3)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_properties_bad_3");
	ASSERT_TRUE(loaded);

	fep::metamodel::PropertyFile prop_file;
	const bool parsed = prop_file.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(prop_file.getLastError(), tagPrefix("property_file", "system_timing_properties", "property") + "element \"name\" is empty");
}

TEST(FEPSystemPropertiesTester, testErrorRootname)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_system_sdk");
	ASSERT_TRUE(loaded);

	fep::metamodel::PropertyFile prop_file;
	const bool parsed = prop_file.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(prop_file.getLastError(), errorMsgRootnameMismatch("system","property_file"));
}
