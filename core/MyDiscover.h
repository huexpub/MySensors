/*
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2017 Sensnology AB
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
#include "MyOTAFirmwareUpdate.h"


#define MY_DISCOVER_HEADER_VERSION					(1u)		//!<
#define MY_DISCOVER_HEADER_SIZE							(2u)		//!<

/// @brief MyDiscover data structure
typedef struct {
	uint8_t PAGEID_VERSION;									//!< Page identifier & Header revision
	uint8_t PAGETYPEID_HWID;								//!< Page type ID & Hardware ID
	uint8_t data[MAX_PAYLOAD - MY_DISCOVER_HEADER_SIZE];	//!< Payload
} __attribute__((packed)) myDiscover_t;

#define dSetPageID(_discover, _pid)					BF_SET(_discover.PAGEID_VERSION, _pid, 0, 5)				//!<
#define dGetPageID(_discover)								((uint8_t)BF_GET(_discover.PAGEID_VERSION, 0, 5))		//!<
#define dSetVersion(_discover, _ver)				BF_SET(_discover.PAGEID_VERSION, _ver, 5, 3)				//!<
#define dGetVersion(_discover)							((uint8_t)BF_GET(_discover.PAGEID_VERSION, 5, 3))		//!<

#define dSetPageTypeID(_discover, _ptid)		BF_SET(_discover.PAGETYPEID_HWID, _ptid, 0, 4)			//!<
#define dGetPageTypeID(_discover)						((uint8_t)BF_GET(_discover.PAGETYPEID_HWID, 0, 4))	//!<
#define dSetHWID(_discover, _rev)						BF_SET(_discover.PAGETYPEID_HWID, _rev, 4, 4)				//!<
#define dGetHWID(_discover)									((uint8_t)BF_GET(_discover.PAGETYPEID_HWID, 4, 4))	//!<

// total pages
#define MY_DISCOVER_TOTAL_PAGES						(5u) //!<
// page id

/// @brief discover page id
typedef enum {
	MY_DISCOVER_PAGEID_PARENT							= 0,	//!<
	MY_DISCOVER_PAGEID_GENERAL						= 1,	//!<
	MY_DISCOVER_PAGEID_FWCONFIG						= 2,	//!<
	MY_DISCOVER_PAGEID_HWPARAMS						= 3,	//!<
	MY_DISCOVER_PAGEID_HWID								= 4,	//!<
	MY_DISCOVER_PAGEID_TRANSPORT_UPLINK		= 5,	//!<
	MY_DISCOVER_PAGEID_BOOTLOADER					= 6		//!<
} discover_pageid_t;

/// @brief discover page type
typedef enum {
	MY_DISCOVER_TYPEID_PARENT							= 0,	//!<
	MY_DISCOVER_TYPEID_GENERAL						= 1,	//!<
	MY_DISCOVER_TYPEID_ARCHITECTURE				= 2,	//!<
	MY_DISCOVER_TYPEID_BOOTLOADER					= 3,	//!<
	MY_DISCOVER_TYPEID_TRANSPORT					= 4,	//!<
	MY_DISCOVER_TYPEID_PERIPHERY					= 5		//!<
} discover_type_t;

/// @brief discover hardware ids
typedef enum {
	MY_DISCOVER_HWID_UNKNOWN							= 0,	//!<
	MY_DISCOVER_HWID_AVR									= 1,	//!<
	MY_DISCOVER_HWID_ESP8266							= 2,	//!<
	MY_DISCOVER_HWID_SAMD									= 3,	//!<
	MY_DISCOVER_HWID_LINUX								= 4,	//!<
	MY_DISCOVER_HWID_NRF5									= 5,	//!<
	MY_DISCOVER_HWID_STM32F1							= 6,	//!<
	MY_DISCOVER_HWID_TEENSYDUINO					= 7		//!<
} hardware_id_t;

/// @brief discover parent types
typedef enum {
	MY_DISCOVER_TCPIP											= 0,	//!<
	MY_DISCOVER_RS232											= 1,	//!<
	MY_DISCOVER_RS485											= 2,	//!<
	MY_DISCOVER_NRF24											= 3,	//!<
	MY_DISCOVER_RFM69											= 4,	//!<
	MY_DISCOVER_RFM95											= 5		//!<
} radio_id_t;

/// @brief discover general id
typedef enum {
	MY_DISCOVER_NOT_AVAILABLE							= 0,	//!<
	MY_DISCOVER_UNDEFINED									= 0,	//!<
	MY_DISCOVER_AVAILABLE									= 1		//!<
} general_id_t;

/// @brief discover data rates
typedef enum {
	MY_DISCOVER_BAUD_UNKNOWN							= 0,	//!<
	MY_DISCOVER_BAUD_9600									= 1,	//!<
	MY_DISCOVER_BAUD_19200								= 2,	//!<
	MY_DISCOVER_BAUD_38400								= 3,	//!<
	MY_DISCOVER_BAUD_57600								= 4,	//!<
	MY_DISCOVER_BAUD_115200								= 5		//!<
} data_rate_t;


/**
* @brief Prepare I_DISCOVER_RESPONSE
* @param sender sender node ID
* @param page page number
*/
void prepareDiscoverResponse(MyMessage &message, const uint8_t sender, const uint8_t page);

#endif // MyDiscover_h
/** @}*/
