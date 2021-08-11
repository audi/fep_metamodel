## ℹ️ This repository is archived 

It is now maintained at https://github.com/cariad-tech


---
<!---
  Copyright @ 2019 Audi AG. All rights reserved.
  
      This Source Code Form is subject to the terms of the Mozilla
      Public License, v. 2.0. If a copy of the MPL was not distributed
      with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
  
  If it is not possible or desirable to put the notice in a particular file, then
  You may include the notice in a location (such as a LICENSE file in a
  relevant directory) where a recipient would be likely to look for such a notice.
  
  You may add additional accurate notices of copyright ownership.
  -->
# FEP SDK Metamodel Library  {#mainpage}

# Description

## FEP SDK Meta Model Library delivers: 
* FEP SDK Metamodel as XSD
* Data Model and Parser for C++ to use it in further developments (see /include and /lib directory)

_____________________

## XSD Content and there meaning

The valid Metamodel 2.0 version is a bundle of:
* xsd/common/2.0
* xsd/element/2.0
* xsd/system/2.0
* xsd/ddl/4.0
* xsd/mapping/1.0

The Metamodel 1.0 and 1.1 version is currently delivered, but not yet verified!

### XSD Overview 2.0

<table>

<tr>
<td><b>COMMON</b></td>
<td></td>
<td></td>
</tr>

<tr>
<td>Common Type Defintions</td>
<td>
    xsd/common/2.0/conan_types.xsd<br>
    xsd/common/2.0/fep_types.xsd<br>
    xsd/common/2.0/timing_interface_types.xsd</td>
<td>
Type definitions to be used element and system relevant XSDs
</td>
</tr>

<tr>
<td><b>ELEMENT RELEVANT</b></td>
<td></td>
<td></td>
</tr>

<tr>
<td>FEP Element Type Description</td>
<td>xsd/element/2.0/fep_element_type.xsd<br>
    xsd/element/2.0/timing_interface_types.xsd</td>
<td>
FEP Element Type Description to describe the default behaviour and interfaces of a FEP Element Type.
(Within C++ it is a type like a class type!) Such a type can be instanciated more than once and will commonly describe the functionality of a concrete FEP Element.
</td>

<tr>
<td>FEP Element Timing Configuration</td>
<td>xsd/element/2.0/fep_element_timing.xsd
    xsd/element/2.0/timing_interface_types.xsd</td>
<td>
Configuration File to change the timing behaviour of a instanciated element. 
This file is only necessary if you want to change the default behaviour of the timing interface defined within FEP Element Type Description.<br>
<b>REMARK:</b> A instance of this file can be used within the FEP System SDK Description (see below under <b>SYSTEM RELEVANT</b>)
</td>
</tr>
</tr>

<tr>
<td>FEP Element Mapping Configuration</td>
<td>xsd/mapping/1.0/mapping_configuration.xsd</td>
<td>
Configuration File for the data input and output configuration while connecting to the simulation data bus.
This file is only necessary if you want to change the default data input and output names or remap some content of the data to other data inputs or data outputs.
<b>REMARK:</b> A instance of this file can be used within the FEP System SDK Description (see below under <b>SYSTEM RELEVANT</b>)
</td>
</tr>
</tr>

<tr>
<td>Data Defintion Language Description</td>
<td>xsd/ddl/4.0/ddl4.0.xsd</td>
<td>
Description File to describe the layout of inputs and outputs of an element type. 
<b>REMARK:</b> A instance of this file is used within the FEP Element Type Description.
</td>
</tr>
</tr>


<tr>
<td><b>SYSTEM RELEVANT</b></td>
<td></td>
<td></td>
</tr>

<tr>
<td>FEP System SDK Description</td>
<td>xsd/system/2.0/fep_system_sdk.xsd</td>
<td>
Description File to describe and configure one concrete FEP SDK System.
The FEP SDK System is a bundle of participants with concrete loaded element instances.<br><br>
These instances <b>may</b> be reconfigured with a data mapping. Otherwise the instances will use the default data mapping defined within the FEP Element Type Description. To configure a data mapping per element instance the <b>FEP Element Mapping Configuration</b> can be used.<br><br>
The element instances <b>may</b> have a reconfigured scheduling and timing behaviour where the <b>FEP Element Timing Configuration</b> can be used. If the timing is not reconfigured, the default timing behaviour will be used described in the <b>FEP Element Type Description</b>.
</td>
</tr>
</tr>

<tr>
<td>FEP System Properties Configuration</td>
<td>xsd/system/2.0/fep_system_properties.xsd</td>
<td>
Description File to describe a concrete parameter set of a concrete launched FEP System.
A instance of this file contains the properties of the element instances (described in the <b>FEP Element Type Description</b>) that have to be changed <b>after</b> the system was launched.
<br>
Additionally, the file will contain System Properties which will be distributed to all element instances.
<br>
Since FEP Participaants and its Elements are (runtime-) configurable via properties, and also a timing behaviour is configurable via properties, the file will also contain pre-defined property sets that take care of system timing behaviour.
</td>
</tr>
</tr>
</tr>

</table>

_____________________

# Additional information 

## Changelog 
* For any software changes have a look at [changelog](doc/changelog.md)

## License 
* This software is published under [Modzilla Public Lisense 2.0](LICENSE.md)

## Contribution rules
* To join the project see [Contribution rules](doc/license/CONTRIBUTION.md)
