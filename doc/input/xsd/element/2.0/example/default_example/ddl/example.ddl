<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
<!--
   Copyright @ 2020 Audi AG. All rights reserved.

       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.

   You may add additional accurate notices of copyright ownership.
-->
<adtf:ddl xmlns:adtf="adtf">
	<header>
		<language_version>3.00</language_version>
		<author>fep_team</author>
		<date_creation>09.10.2014</date_creation>
		<date_change>09.10.2014</date_change>
		<description>ADTF generated</description>
	</header>
	<units>
		<baseunit description="Fundamental unit for electric current" name="Ampere" symbol="A" />
		<baseunit description="Fundamental unit for luminous intensity" name="Candela" symbol="cd" />
		<baseunit description="Non-SI standard unit for angle" name="Degree" symbol="deg" />
		<baseunit description="Fundamental unit for thermodynamic temperature" name="Kelvin" symbol="K" />
		<baseunit description="Fundamental unit for mass" name="Kilogram" symbol="kg" />
		<baseunit description="Fundamental unit for length" name="Metre" symbol="m" />
		<baseunit description="Fundamental unit for amount of substance" name="Mole" symbol="mol" />
		<baseunit description="Non-SI standard unit for angle" name="Radiant" symbol="rad" />
		<baseunit description="Fundamental unit for time" name="Second" symbol="s" />
		<baseunit description="No SI, but needed for own unit definitions" name="Unitless" symbol="" />
		<baseunit description="No SI, but needed for no unit definitions" name="nou" symbol="" />
		<prefixes name="atto" power="-18" symbol="a" />
		<prefixes name="centi" power="-2" symbol="c" />
		<prefixes name="deca" power="1" symbol="da" />
		<prefixes name="deci" power="-1" symbol="d" />
		<prefixes name="exa" power="18" symbol="E" />
		<prefixes name="femto" power="-15" symbol="f" />
		<prefixes name="giga" power="9" symbol="G" />
		<prefixes name="hecto" power="2" symbol="h" />
		<prefixes name="kilo" power="3" symbol="k" />
		<prefixes name="mega" power="6" symbol="M" />
		<prefixes name="micro" power="-6" symbol="u" />
		<prefixes name="milli" power="-3" symbol="m" />
		<prefixes name="nano" power="-9" symbol="n" />
		<prefixes name="peta" power="15" symbol="P" />
		<prefixes name="pico" power="-12" symbol="p" />
		<prefixes name="tera" power="12" symbol="T" />
		<prefixes name="yocto" power="-24" symbol="y" />
		<prefixes name="yotta" power="24" symbol="Y" />
		<prefixes name="zepto" power="-21" symbol="z" />
		<prefixes name="zetta" power="21" symbol="Z" />
	</units>
	<datatypes>
		<datatype description="predefined ADTF tBool datatype" max="tTrue" min="tFalse" name="tBool" size="8" />
		<datatype description="predefined ADTF tChar datatype" max="127" min="-128" name="tChar" size="8" />
		<datatype description="predefined ADTF tFloat32 datatype" max="3.402823e+38" min="-3.402823e+38" name="tFloat32" size="32" />
		<datatype description="predefined ADTF tFloat64 datatype" max="1.797693e+308" min="-1.797693e+308" name="tFloat64" size="64" />
		<datatype description="predefined ADTF tInt16 datatype" max="32767" min="-32768" name="tInt16" size="16" />
		<datatype description="predefined ADTF tInt32 datatype" max="2147483647" min="-2147483648" name="tInt32" size="32" />
		<datatype description="predefined ADTF tInt64 datatype" max="9223372036854775807" min="-9223372036854775808" name="tInt64" size="64" />
		<datatype description="predefined ADTF tInt8 datatype" max="127" min="-128" name="tInt8" size="8" />
		<datatype description="predefined ADTF tUInt16 datatype" max="65535" min="0" name="tUInt16" size="16" />
		<datatype description="predefined ADTF tUInt32 datatype" max="4294967295" min="0" name="tUInt32" size="32" />
		<datatype description="predefined ADTF tUInt64 datatype" max="18446744073709551615" min="0" name="tUInt64" size="64" />
		<datatype description="predefined ADTF tUInt8 datatype" max="255" min="0" name="tUInt8" size="8" />
	</datatypes>
	<enums>
		<enum name="tPixelFormat" type="tInt16">
			<element name="PF_16BIT" value="20" />
			<element name="PF_24BIT" value="40" />
			<element name="PF_32BIT" value="50" />
			<element name="PF_8BIT" value="10" />
			<element name="PF_ABGR_4444" value="29" />
			<element name="PF_ABGR_8888" value="52" />
			<element name="PF_ARGB_8888" value="51" />
			<element name="PF_BGRA_8888" value="54" />
			<element name="PF_BGR_555" value="34" />
			<element name="PF_BGR_565" value="35" />
			<element name="PF_BGR_888" value="46" />
			<element name="PF_CUSTOM" value="1000" />
			<element name="PF_GREYSCALE_10" value="21" />
			<element name="PF_GREYSCALE_12" value="22" />
			<element name="PF_GREYSCALE_14" value="23" />
			<element name="PF_GREYSCALE_16" value="24" />
			<element name="PF_GREYSCALE_18" value="41" />
			<element name="PF_GREYSCALE_20" value="42" />
			<element name="PF_GREYSCALE_22" value="43" />
			<element name="PF_GREYSCALE_24" value="44" />
			<element name="PF_GREYSCALE_32" value="55" />
			<element name="PF_GREYSCALE_8" value="11" />
			<element name="PF_GREYSCALE_FLOAT32" value="56" />
			<element name="PF_RGBA_4444" value="28" />
			<element name="PF_RGBA_8888" value="53" />
			<element name="PF_RGB_444" value="25" />
			<element name="PF_RGB_555" value="26" />
			<element name="PF_RGB_565" value="27" />
			<element name="PF_RGB_8" value="12" />
			<element name="PF_RGB_888" value="45" />
			<element name="PF_RIII_10" value="30" />
			<element name="PF_RIII_12" value="31" />
			<element name="PF_RIII_14" value="32" />
			<element name="PF_RIII_16" value="33" />
			<element name="PF_UNKNOWN" value="0" />
			<element name="PF_YUV420P_888" value="60" />
		</enum>
	</enums>
	<structs>
		<struct alignment="1" name="adtf.core.media_type" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="mediatype" type="tMediaTypeInfo" />
		</struct>
		<struct alignment="1" name="tMediaTypeInfo" version="2">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="ui32MajorType" type="tUInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="4" name="ui32SubType" type="tUInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="8" name="ui32Flags" type="tUInt32" />
		</struct>
		<struct alignment="1" name="adtf.type.video" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="sMediatype" type="tMediaTypeInfo" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="12" name="sBitmapFormat" type="tBitmapFormat" />
		</struct>
		<struct alignment="1" name="tBitmapFormat" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="nWidth" type="tInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="4" name="nHeight" type="tInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="8" name="nBitsPerPixel" type="tInt16" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="10" name="nPixelFormat" type="tPixelFormat" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="12" name="nBytesPerLine" type="tInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="16" name="nSize" type="tInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="20" name="nPaletteSize" type="tInt32" />
		</struct>
		<struct alignment="1" name="tHeaderStruct" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="ui32HeaderVal" type="tUInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="4" name="f64HeaderVal" type="tFloat64" />
		</struct>
		<struct alignment="1" name="tNestedStruct" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="sHeaderStruct" type="tHeaderStruct" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="12" name="sSimpleStruct" type="tSimpleStruct" />
		</struct>
		<struct alignment="1" name="tSimpleStruct" version="1">
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="ui8Val" type="tUInt8" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="1" name="ui16Val" type="tUInt16" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="3" name="ui32Val" type="tUInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="7" name="i32Val" type="tInt32" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="11" name="i64Val" type="tInt64" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="19" name="f64Val" type="tFloat64" />
			<element alignment="1" arraysize="1" byteorder="LE" bytepos="27" name="f32Val" type="tFloat32" />
		</struct>
	</structs>
	<streams>
	</streams>
</adtf:ddl>
