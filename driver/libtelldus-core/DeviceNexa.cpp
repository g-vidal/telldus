// #include "StdAfx.h"
#include "DeviceNexa.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace TelldusCore;

/*
* Constructor
*/
DeviceNexa::DeviceNexa(int id, int model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceNexa::~DeviceNexa(void)
{}

bool DeviceNexa::setHouse(const std::string &newHouse) {
	if (newHouse.length() > 0) {
		intHouse = newHouse[0] - 'A';
	} else {
		intHouse = 0;
	}
	return true;
}

bool DeviceNexa::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	return true;
}

bool DeviceNexa::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("nexa_house") == 0) {
		return setHouse(strValue);
	} else if (strName.compare("nexa_unit") == 0) {
		return setUnit(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceNexa::turnOn(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Turn off this device
*/
int DeviceNexa::turnOff(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Send a bell
*/
int DeviceNexa::bell(void){

	try{
		std::string strCode = getStringCode(intHouse);

		strCode.append("$kk$$kk$$kk$$k$k"); //the unit-code is always 7, doesn't have to be regenerated each time
		strCode.insert(0, "S");
		strCode.append("$kk$$kk$$kk$$kk$$k+");	//the "bell"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
std::string DeviceNexa::getStringCode(int intToConvert){

	std::string strReturn = "";

	try{
		std::bitset<4> bs ((long)intToConvert);

		strReturn = bs.to_string();
		reverse(strReturn.begin(), strReturn.end());

		size_t intPos = strReturn.find("0");
		while (intPos < std::string::npos){
			strReturn.replace(intPos, 1, "$k");
			intPos = strReturn.find("0", intPos + 1);
		}

		intPos = strReturn.find("1");
		while (intPos < std::string::npos){
			strReturn.replace(intPos, 1, "k$");
			intPos = strReturn.find("1", intPos + 1);
		}

		intPos = 0;
		while (intPos < strReturn.length()){
			strReturn.insert(intPos, "$k");
			intPos = intPos + 4;
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

bool DeviceNexa::parameterMatches( const std::string &name, const std::string &value ) const {
	if (value.length() == 0) {
		return false;
	}
	
	if (name.compare("arctech_house") == 0) {
		return intHouse == value[0] - 'A';
	} else if (name.compare("arctech_unit") == 0) {
		return intCode == atoi(value.c_str()) - 1;
	}
	return true;
}

/*
* Has the device got the method?
*/
int DeviceNexa::methods(int methodsSupported){
	int intModel = this->getModel();
	
	if( (intModel == TELLSTICK_DEVICE_YCR3500) ||
		(intModel == TELLSTICK_DEVICE_YCR300D) ||
		(intModel == TELLSTICK_DEVICE_WSR1000) ||
		(intModel == TELLSTICK_DEVICE_CMR1000) ||
		(intModel == TELLSTICK_DEVICE_CMR300) ||
		(intModel == TELLSTICK_DEVICE_PA33300) ||
		(intModel == TELLSTICK_DEVICE_EL2000) ||
		(intModel == TELLSTICK_DEVICE_EL2005) ||
		(intModel == TELLSTICK_DEVICE_EL2006) ||
		(intModel == TELLSTICK_DEVICE_SYCR3500) ||
		(intModel == TELLSTICK_DEVICE_SYCR300) ||
		(intModel == TELLSTICK_DEVICE_HDR105) ||
		(intModel == TELLSTICK_DEVICE_EL2004) ||
		(intModel == TELLSTICK_DEVICE_EL2016) ||
		(intModel == TELLSTICK_DEVICE_EL2010) ||
		(intModel == TELLSTICK_DEVICE_LYCR1000) ||
		(intModel == TELLSTICK_DEVICE_LYCR300) ||
		(intModel == TELLSTICK_DEVICE_LCMR1000) ||
		(intModel == TELLSTICK_DEVICE_LCMR300) ||
		(intModel == TELLSTICK_DEVICE_EL2023) ||
		(intModel == TELLSTICK_DEVICE_EL2024) ||
		(intModel == TELLSTICK_DEVICE_EL2021) ||
		(intModel == TELLSTICK_DEVICE_EL2017) ||
		(intModel == TELLSTICK_DEVICE_EL2019)
 		)
	{
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
	} else if ( intModel == TELLSTICK_DEVICE_ML7100) {
		if (methodsSupported & TELLSTICK_BELL) {
			return TELLSTICK_BELL;
		} else if (methodsSupported & TELLSTICK_TURNON) {
			return TELLSTICK_TURNON;
		}
	}
	return 0;
}

std::string DeviceNexa::getProtocol() const {
	return "arctech";
}
