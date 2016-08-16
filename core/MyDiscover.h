/*
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2016 Sensnology AB
* Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*/

/**
* @file MyDiscover.h
*
* @defgroup MyDiscovergrp MyDiscover
* @ingroup internals
* @{
*/

#ifndef MyDiscover_h
#define MyDiscover_h

#include "MySensorsCore.h"
#include "MyOTAFirmwareUpdate.h"	// NodeFirmwareConfig


#define MY_DISCOVER_HEADER_VERSION					(1)
#define MY_DISCOVER_HEADER_SIZE							(2)

/// @brief MyDiscover data structure
typedef struct {
	uint8_t PAGEID_VERSION;									//!< Page identifier & Header revision
	uint8_t PAGETYPEID_HWID;								//!< Page type ID & Hardware ID
	uint8_t data[MAX_PAYLOAD - MY_DISCOVER_HEADER_SIZE];	//!< Payload
} __attribute__((packed)) MyDiscover;

#define dSetPageID(_discover, _pid)					BF_SET(_discover.PAGEID_VERSION, _pid, 0, 5)				//!<
#define dGetPageID(_discover)								((uint8_t)BF_GET(_discover.PAGEID_VERSION, 0, 5))		//!<
#define dSetVersion(_discover, _ver)				BF_SET(_discover.PAGEID_VERSION, _ver, 5, 3)				//!<
#define dGetVersion(_discover)							((uint8_t)BF_GET(_discover.PAGEID_VERSION, 5, 3))		//!<

#define dSetPageTypeID(_discover, _ptid)		BF_SET(_discover.PAGETYPEID_HWID, _ptid, 0, 4)			//!<
#define dGetPageTypeID(_discover)						((uint8_t)BF_GET(_discover.PAGETYPEID_HWID, 0, 4))	//!<
#define dSetHWID(_discover, _rev)						BF_SET(_discover.PAGETYPEID_HWID, _rev, 4, 4)				//!<
#define dGetHWID(_discover)									((uint8_t)BF_GET(_discover.PAGETYPEID_HWID, 4, 4))	//!<

// total pages
#define MY_DISCOVER_TOTAL_PAGES							(5)
// page id
#define MY_DISCOVER_PAGEID_PARENT						(0)
#define MY_DISCOVER_PAGEID_GENERAL					(1)
#define MY_DISCOVER_PAGEID_ARCHITECTURE			(2)
#define MY_DISCOVER_PAGEID_TRANSPORT_UPLINK	(3)
#define MY_DISCOVER_PAGEID_BOOTLOADER				(4)
// page types
#define MY_DISCOVER_TYPEID_PARENT						(0)
#define MY_DISCOVER_TYPEID_GENERAL					(1)
#define MY_DISCOVER_TYPEID_ARCHITECTURE			(2)
#define MY_DISCOVER_TYPEID_BOOTLOADER				(3)
#define MY_DISCOVER_TYPEID_TRANSPORT				(4)
#define MY_DISCOVER_TYPEID_PERIPHERY				(5)
// hardware ids
#define MY_DISCOVER_HWID_UNKNOWN						(0)
#define MY_DISCOVER_HWID_AVR								(1)
#define MY_DISCOVER_HWID_ESP8266						(2)
#define MY_DISCOVER_HWID_SAMD								(3)
#define MY_DISCOVER_HWID_RTL8710						(4)
#define MY_DISCOVER_HWID_RPI								(5)
#define MY_DISCOVER_HWID_NRF24LE1						(6)	// experimental

// parent types
#define MY_DISCOVER_TCPIP										(0)
#define MY_DISCOVER_RS232										(1)
#define MY_DISCOVER_RS485										(2)
#define MY_DISCOVER_NRF24										(3)
#define MY_DISCOVER_RFM69										(4)
// general
#define MY_DISCOVER_NOT_AVAILABLE						(0)
#define MY_DISCOVER_AVAILABLE								(1)
#define MY_DISCOVER_UNDEFINED								(0)
// data rates
#define MY_DISCOVER_BAUD_UNKNOWN						(0)
#define MY_DISCOVER_BAUD_9600								(1)
#define MY_DISCOVER_BAUD_19200							(2)
#define MY_DISCOVER_BAUD_38400							(3)
#define MY_DISCOVER_BAUD_57600							(4)
#define MY_DISCOVER_BAUD_115200							(5)


/**
* @brief Prepare I_DISCOVER_RESPONSE
* @param sender sender node ID
* @param page page number
* @return true if response generated
*/
bool prepareDiscoverResponse(MyMessage &message, const uint8_t sender, const uint8_t page);


#endif // MyDiscover_h
/** @}*/
