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
#include "fep_metamodel/fep_element.h"

using namespace fep::metamodel;

TEST(FEPElementTypeTester, testOK)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_TRUE(parsed);

	EXPECT_EQ(fep_element._id._name, "RadarSensor");
	EXPECT_EQ(fep_element._id._version, "1.0.0");
	EXPECT_EQ(fep_element._id._user, "user");
	EXPECT_EQ(fep_element._id._channel, "stable");

	EXPECT_EQ(fep_element._display_name, "Radar Sensor");
	EXPECT_EQ(fep_element._description, "This is a description of the whole functionality this Element is able to provide.");
	EXPECT_EQ(fep_element._author, "fep_team");

	ASSERT_EQ(fep_element._configuration_interface._properties.size(), 1u);
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._name, "parameter_1");
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._type, "int");
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._value, "34");

	ASSERT_EQ(fep_element._data_interfaces._inputs.size(), 2u);

	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._name, "input_value_plain");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._metatype_name, "plain-ctype");
	ASSERT_EQ(fep_element._data_interfaces._inputs[0]._type._properties.size(), 1u);
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._name, "datatype");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._value, "int8_t");

	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._name, "input_value_structured");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._metatype_name, "ddl");
	ASSERT_EQ(fep_element._data_interfaces._inputs[1]._type._properties.size(), 3u);
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._name, "ddlstruct");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._value, "tSimpleStruct");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._name, "ddldescription");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._value, "");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._name, "ddlfileref");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._type, "filename");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._value, "./ddl/example.ddl");

	ASSERT_EQ(fep_element._data_interfaces._outputs.size(), 2u);
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._name, "output_value_plain");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._metatype_name, "plain-array-ctype");
	ASSERT_EQ(fep_element._data_interfaces._outputs[0]._type._properties.size(), 2u);
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._name, "datatype");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._value, "uint64_t");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._name, "max_array_size");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._type, "uint");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._value, "32");
 
	EXPECT_EQ(fep_element._data_interfaces._outputs[1]._name, "output_value_raw");
	EXPECT_EQ(fep_element._data_interfaces._outputs[1]._type._metatype_name, "anonymous");
	ASSERT_EQ(fep_element._data_interfaces._outputs[1]._type._properties.size(), 0u);

	ASSERT_EQ(fep_element._timing_interface._jobs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._name, "my_callable_function");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._cycle_time, 100000ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._cycle_delay_time, 20000ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._max_run_realtime, 0ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._run_realtime_violation, "TS_IGNORE_RUNTIME_VIOLATION");
	ASSERT_TRUE(fep_element._timing_interface._jobs[0]._data_references.get()); // optional element is present -> ptr is not null
	ASSERT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs[0]._name, "input_value_plain");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs[0]._queue_size, 10ull);
	ASSERT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs[0]._name, "output_value_plain");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs[0]._queue_size, 10ull);

	ASSERT_EQ(fep_element._service_interfaces._service_interfaces.size(), 1u);
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._name, "SensorService");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._type, "AUDISensorInterface");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._version, "1.0.0");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._interface_description._protocol, "JSON-RPC");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._interface_description._file_reference, "audi_sensor_interface.json");

	ASSERT_EQ(fep_element._required_service_interfaces._required_service_interfaces.size(), 1u);
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._type, "other_service_type");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._version, "2.3.0");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._interface_description._protocol, "JSON-RPC");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._interface_description._file_reference, "other_service_type.json");
}

TEST(FEPElementTypeTester, testOKlist)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type_2");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_TRUE(parsed);

	EXPECT_EQ(fep_element._id._name, "RadarSensor");
	EXPECT_EQ(fep_element._id._version, "1.0.0");
	EXPECT_EQ(fep_element._id._user, "user");
	EXPECT_EQ(fep_element._id._channel, "stable");

	EXPECT_EQ(fep_element._display_name, "Radar Sensor");
	EXPECT_EQ(fep_element._description, "This is a description of the whole functionality this Element is able to provide.");
	EXPECT_EQ(fep_element._author, "fep_team");

	ASSERT_EQ(fep_element._configuration_interface._properties.size(), 1u);
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._name, "parameter_1");
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._type, "int");
	EXPECT_EQ(fep_element._configuration_interface._properties[0]._value, "34");
	ASSERT_EQ(fep_element._configuration_interface._properties[0]._property_list_enumeration.size(), 0u); // cannot differentiate between empty and non-existing 'list' element

	ASSERT_EQ(fep_element._data_interfaces._inputs.size(), 2u);

	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._name, "input_value_plain");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._metatype_name, "plain-ctype");
	ASSERT_EQ(fep_element._data_interfaces._inputs[0]._type._properties.size(), 1u);
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._name, "datatype");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._value, "int8_t");
	ASSERT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._property_list_enumeration.size(), 2u);
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._property_list_enumeration[0]._name, "propA");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._property_list_enumeration[0]._value, 1ull);
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._property_list_enumeration[1]._name, "propB");
	EXPECT_EQ(fep_element._data_interfaces._inputs[0]._type._properties[0]._property_list_enumeration[1]._value, 2ull);

	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._name, "input_value_structured");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._metatype_name, "ddl");
	ASSERT_EQ(fep_element._data_interfaces._inputs[1]._type._properties.size(), 3u);
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._name, "ddlstruct");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[0]._value, "tSimpleStruct");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._name, "ddldescription");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[1]._value, "");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._name, "ddlfileref");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._type, "filename");
	EXPECT_EQ(fep_element._data_interfaces._inputs[1]._type._properties[2]._value, "./ddl/example.ddl");

	ASSERT_EQ(fep_element._data_interfaces._outputs.size(), 2u);
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._name, "output_value_plain");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._metatype_name, "plain-array-ctype");
	ASSERT_EQ(fep_element._data_interfaces._outputs[0]._type._properties.size(), 2u);
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._name, "datatype");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._type, "string");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[0]._value, "uint64_t");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._name, "max_array_size");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._type, "uint");
	EXPECT_EQ(fep_element._data_interfaces._outputs[0]._type._properties[1]._value, "32");

	EXPECT_EQ(fep_element._data_interfaces._outputs[1]._name, "output_value_raw");
	EXPECT_EQ(fep_element._data_interfaces._outputs[1]._type._metatype_name, "anonymous");
	ASSERT_EQ(fep_element._data_interfaces._outputs[1]._type._properties.size(), 0u);

	ASSERT_EQ(fep_element._timing_interface._jobs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._name, "my_callable_function");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._cycle_time, 100000ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._cycle_delay_time, 20000ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._max_run_realtime, 0ull);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._run_realtime_violation, "TS_IGNORE_RUNTIME_VIOLATION");
	ASSERT_TRUE(fep_element._timing_interface._jobs[0]._data_references.get()); // optional element is present -> ptr is not null
	ASSERT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs[0]._name, "input_value_plain");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_inputs[0]._queue_size, 10ull);
	ASSERT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs.size(), 1u);
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs[0]._name, "output_value_plain");
	EXPECT_EQ(fep_element._timing_interface._jobs[0]._data_references->_outputs[0]._queue_size, 10ull);

	ASSERT_EQ(fep_element._service_interfaces._service_interfaces.size(), 1u);
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._name, "SensorService");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._type, "AUDISensorInterface");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._version, "1.0.0");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._interface_description._protocol, "JSON-RPC");
	EXPECT_EQ(fep_element._service_interfaces._service_interfaces[0]._interface_description._file_reference, "audi_sensor_interface.json");

	ASSERT_EQ(fep_element._required_service_interfaces._required_service_interfaces.size(), 1u);
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._type, "other_service_type");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._version, "2.3.0");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._interface_description._protocol, "JSON-RPC");
	EXPECT_EQ(fep_element._required_service_interfaces._required_service_interfaces[0]._interface_description._file_reference, "other_service_type.json");
}

TEST(FEPElementTypeTester, testError1)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type_bad_1");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element.getLastError(), tagPrefix("element_type") + errorMsgMissingTag("id"));
}

TEST(FEPElementTypeTester, testError2)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type_bad_2");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element.getLastError(), tagPrefix("element_type", "id") + "element \"version\" (1.0) is not properly formatted version string");
}

TEST(FEPElementTypeTester, testError3)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type_bad_3");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element.getLastError(), tagPrefix("element_type", "timing_interface", "jobs", "job") + "element \"cycle_delay_time\" cannot be interpreted as unsigned long long");
}

TEST(FEPElementTypeTester, testError4)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type_bad_4");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element.getLastError(), tagPrefix("element_type", "data_interfaces", "inputs", "input", "type", "properties", "property", "list", "property_list_enumeration") +
		"element \"name\" is empty");
}


TEST(FEPElementTypeTester, testErrorRootname)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_timing");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementType fep_element;
	const bool parsed = fep_element.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element.getLastError(), errorMsgRootnameMismatch("element_timing", "element_type"));
}

TEST(FEPElementTimingTester, testOK)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_timing");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementTiming fep_element_timing;
	const bool parsed = fep_element_timing.internalReadConfig(dom);
	ASSERT_TRUE(parsed);

	EXPECT_EQ(fep_element_timing._header._author, "");
	EXPECT_EQ(fep_element_timing._header._date_creation, "");
	EXPECT_EQ(fep_element_timing._header._date_change, "");
	EXPECT_EQ(fep_element_timing._header._description, "Default Timing Configuration for Demo Timing 30 Example Driver");

	ASSERT_EQ(fep_element_timing._jobs.size(), 1u);
	EXPECT_EQ(fep_element_timing._jobs[0]._name, "CheckDistanceAndDecide");
	EXPECT_EQ(fep_element_timing._jobs[0]._cycle_time, 100000ull);
	EXPECT_EQ(fep_element_timing._jobs[0]._cycle_delay_time, 0ull);
	EXPECT_EQ(fep_element_timing._jobs[0]._max_run_realtime, 0ull);
	EXPECT_EQ(fep_element_timing._jobs[0]._run_realtime_violation, "TS_IGNORE_RUNTIME_VIOLATION");
	ASSERT_TRUE(fep_element_timing._jobs[0]._data_references.get());

	ASSERT_EQ(fep_element_timing._jobs[0]._data_references->_inputs.size(), 2u);
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_inputs[0]._name, "FrontDistance");
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_inputs[0]._queue_size, 10ull);
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_inputs[1]._name, "BackDistance");
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_inputs[1]._queue_size, 10ull);

	ASSERT_EQ(fep_element_timing._jobs[0]._data_references->_outputs.size(), 1u);
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_outputs[0]._name, "DriverCommand");
	EXPECT_EQ(fep_element_timing._jobs[0]._data_references->_outputs[0]._queue_size, 10ull);
}

TEST(FEPElementTimingTester, testError1)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_timing_bad_1");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementTiming fep_element_timing;
	const bool parsed = fep_element_timing.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element_timing.getLastError(), tagPrefix("element_timing") + "element \"schema_version\" () is not properly formatted version string");
}

TEST(FEPElementTimingTester, testError2)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_timing_bad_2");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementTiming fep_element_timing;
	const bool parsed = fep_element_timing.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element_timing.getLastError(), tagPrefix("element_timing") + errorMsgMissingTag("header"));
}

TEST(FEPElementTimingTester, testError3)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_timing_bad_3");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementTiming fep_element_timing;
	const bool parsed = fep_element_timing.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element_timing.getLastError(), tagPrefix("element_timing", "jobs", "job") + errorMsgMissingTag("run_realtime_violation"));
}

TEST(FEPElementTimingTester, testErrorRootname)
{
	a_util::xml::DOM dom;
	const bool loaded = dom.load("files/example_2.0.fep_element_type");
	ASSERT_TRUE(loaded);

	fep::metamodel::FepElementTiming fep_element_timing;
	const bool parsed = fep_element_timing.internalReadConfig(dom);
	ASSERT_FALSE(parsed);
	EXPECT_EQ(fep_element_timing.getLastError(), errorMsgRootnameMismatch("element_type", "element_timing"));
}

TEST(FEPElementTypeTester, testDriver)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_element_driver.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}

TEST(FEPElementTypeTester, testEnvironment)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_element_environment.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}

TEST(FEPElementTypeTester, testObserver)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_element_observer.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}

TEST(FEPElementTypeTester, testSensorBack)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_element_sensor_back.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}

TEST(FEPElementTypeTester, testSensorFront)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_element_sensor_front.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}

TEST(FEPElementTypeTester, testTimingMaster)
{
	a_util::xml::DOM dom;
	ASSERT_TRUE(dom.load("files/fep_timing_master.fep_element_type"));

	fep::metamodel::FepElementType fep_element;
	ASSERT_TRUE(fep_element.internalReadConfig(dom));
}
