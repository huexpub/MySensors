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

#include "MyDiscover.h"

// hardware
#if defined(ARDUINO_ARCH_AVR)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_AVR						//!< MY_DISCOVER_HWID_AVR
#elif defined (ARDUINO_ARCH_ESP8266)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_ESP8266				//!< MY_DISCOVER_HWID_ESP8266
#elif defined (ARDUINO_ARCH_SAMD)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_SAMD						//!< MY_DISCOVER_HWID_SAMD
#elif defined(ARDUINO_ARCH_STM32F1)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_STM32F1				//!< MY_DISCOVER_HWID_STM32F1
#elif defined(ARDUINO_ARCH_NRF5)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_NRF5						//!< MY_DISCOVER_HWID_NRF5
#elif defined(__arm__) && defined(TEENSYDUINO)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_TEENSYDUINO		//!< MY_DISCOVER_HWID_TEENSYDUINO
#elif defined(__linux__)
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_LINUX					//!< MY_DISCOVER_HWID_LINUX
#else
#define MY_DISCOVER_HWID			MY_DISCOVER_HWID_UNKNOWN				//!< MY_DISCOVER_HWID_UNKNOWN
#endif

// transport count
#if defined(MY_GATEWAY_SERIAL)
#define MY_DISCOVER_TRANSPORT_RS232					MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_RS232					MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_GATEWAY_W5100) || defined(MY_GATEWAY_ENC28J60) || defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_MQTT_CLIENT)
#define MY_DISCOVER_TRANSPORT_TCPIP				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_TCPIP				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RADIO_RF24)
#define MY_DISCOVER_TRANSPORT_RF24				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_RF24				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RADIO_RFM69)
#define MY_DISCOVER_TRANSPORT_RFM69				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_RFM69				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RADIO_RFM95)
#define MY_DISCOVER_TRANSPORT_RFM95					MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_RFM95					MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RADIO_NRF5_ESB)
#define MY_DISCOVER_TRANSPORT_NRF5_ESB			MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_NRF5_ESB			MY_DISCOVER_NOT_AVAILABLE
#endif


#if defined(MY_RS485)
#define MY_DISCOVER_TRANSPORT_RS485				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_TRANSPORT_RS485				MY_DISCOVER_NOT_AVAILABLE
#endif

#define MY_DISCOVER_TRANSPORT_COUNT (MY_DISCOVER_TRANSPORT_RS232 + MY_DISCOVER_TRANSPORT_TCPIP + MY_DISCOVER_TRANSPORT_RF24 + MY_DISCOVER_TRANSPORT_RFM69 + MY_DISCOVER_TRANSPORT_RFM95 + MY_DISCOVER_TRANSPORT_RS485)
#define MY_DISCOVER_TRANSPORT_ENABLED (uint8_t)( (MY_DISCOVER_TRANSPORT_RS232<<6) | (MY_DISCOVER_TRANSPORT_RS485<<5) | (MY_DISCOVER_TRANSPORT_TCPIP<<4) | (MY_DISCOVER_TRANSPORT_RF24<<3) | (MY_DISCOVER_TRANSPORT_NRF5_ESB<<2) | (MY_DISCOVER_TRANSPORT_RFM69<<1) | MY_DISCOVER_TRANSPORT_RFM95 )

// features
#if defined(MY_BRIDGE_FEATURE)	// new
#define MY_DISCOVER_BRIDGE				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_BRIDGE				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_REPEATER_FEATURE)
#define MY_DISCOVER_REPEATER			MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_REPEATER			MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_GATEWAY_FEATURE)
#define MY_DISCOVER_GATEWAY				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_GATEWAY				MY_DISCOVER_NOT_AVAILABLE
#endif

// no way of discovering if sensors attached, i.e. gateway + sensor excluded
#if defined(MY_SENSOR_NODE)
#define MY_DISCOVER_SENSORS				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_SENSORS				MY_DISCOVER_NOT_AVAILABLE
#endif

// node power: PSU
#if defined(MY_POWER_PSU)
#define MY_DISCOVER_PSU						MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_PSU						MY_DISCOVER_NOT_AVAILABLE
#endif

// node power: battery
#if defined(MY_POWER_BATTERY)
#define MY_DISCOVER_BATTERY				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_BATTERY				MY_DISCOVER_NOT_AVAILABLE
#endif

// node power: solar panel
#if defined(MY_POWER_SOLAR)
#define MY_DISCOVER_SOLAR					MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_SOLAR					MY_DISCOVER_NOT_AVAILABLE
#endif
#define MY_DISCOVER_NODE_TYPE (uint8_t)( (MY_DISCOVER_SOLAR<<6) |(MY_DISCOVER_BATTERY<<5) |(MY_DISCOVER_PSU<<4) | (MY_DISCOVER_BRIDGE<<3)| (MY_DISCOVER_GATEWAY<<2) | (MY_DISCOVER_REPEATER<<1) | MY_DISCOVER_SENSORS )

// node features
#if defined(MY_OTA_FIRMWARE_FEATURE)
#define MY_DISCOVER_OTA_FIRMWARE_FEATURE	MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_OTA_FIRMWARE_FEATURE	MY_DISCOVER_NOT_AVAILABLE
#endif

// remote reset
#if !defined(MY_DISABLE_REMOTE_RESET)
#define MY_DISCOVER_REMOTE_RESET					MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_REMOTE_RESET					MY_DISCOVER_NOT_AVAILABLE
#endif

// sleeping
#if defined(MY_SLEEPING_NODE)
#define MY_DISCOVER_SLEEPING_NODE				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_SLEEPING_NODE				MY_DISCOVER_NOT_AVAILABLE
#endif

// encryption
#if defined(MY_RF24_ENABLE_ENCRYPTION) || defined(MY_RFM69_ENABLE_ENCRYPTION)
#define MY_DISCOVER_ENCRYPTION_AES			MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_ENCRYPTION_AES			MY_DISCOVER_NOT_AVAILABLE
#endif

#define MY_DISCOVER_ENCRYPTION MY_DISCOVER_ENCRYPTION_AES

// RX message buffer
#if defined(MY_RX_MESSAGE_BUFFER_FEATURE)
#define MY_DISCOVER_RX_QUEUE				MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_RX_QUEUE				MY_DISCOVER_NOT_AVAILABLE
#endif

#define MY_DISCOVER_NODE_FEATURES (uint8_t)( (MY_DISCOVER_RX_QUEUE<<4) | (MY_DISCOVER_ENCRYPTION_AES<<3) | (MY_DISCOVER_SLEEPING_NODE<<2) | (MY_DISCOVER_REMOTE_RESET<<1) | MY_DISCOVER_OTA_FIRMWARE_FEATURE)


// signing
#if defined(MY_SIGNING_FEATURE)
#define MY_DISCOVER_SIGNING					MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_SIGNING					MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_SIGNING_REQUEST_SIGNATURES)
#define MY_DISCOVER_REQUEST_SIGNATURES		MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_REQUEST_SIGNATURES		MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_SIGNING_NODE_WHITELISTING)
#define MY_DISCOVER_NODE_WHITELISTING		MY_DISCOVER_AVAILABLE
#else
#define MY_DISCOVER_NODE_WHITELISTING		MY_DISCOVER_NOT_AVAILABLE
#endif

#define MY_DISCOVER_SIGNATURES (uint8_t)( (MY_DISCOVER_NODE_WHITELISTING << 2) | (MY_DISCOVER_REQUEST_SIGNATURES << 1) | MY_DISCOVER_SIGNING )


void prepareDiscoverResponse(MyMessage &message, const uint8_t sender, const uint8_t page)
{
	myDiscover_t _myDiscover;

	_myDiscover.PAGEID_VERSION = 0;
	_myDiscover.PAGETYPEID_HWID = 0;

	dSetPageID(_myDiscover, page);
	dSetVersion(_myDiscover, MY_DISCOVER_HEADER_VERSION);
	dSetHWID(_myDiscover, MY_DISCOVER_HWID);

	uint8_t _len = MY_DISCOVER_HEADER_SIZE;

	switch (page) {
	case MY_DISCOVER_PAGEID_GENERAL: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_GENERAL);
		_myDiscover.data[0] = (MY_DISCOVER_TRANSPORT_COUNT << 4) | MY_DISCOVER_TOTAL_PAGES;
		_myDiscover.data[1] = MY_DISCOVER_NODE_TYPE;
		_myDiscover.data[2] = getParentNodeId();
		_myDiscover.data[3] = getDistanceGW();
		_myDiscover.data[4] = MY_DISCOVER_NODE_FEATURES;
		_myDiscover.data[5] = MY_DISCOVER_TRANSPORT_ENABLED;
		const uint32_t data32 = hwMillis();
		(void)memcpy(&_myDiscover.data[6], &data32, 4);
		_len += 10;
		break;
	}
	case MY_DISCOVER_PAGEID_FWCONFIG: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_FWCONFIG);

		nodeFirmwareConfig_t _firmwareConfig;
		hwReadConfigBlock((void*)&_firmwareConfig, (void*)EEPROM_FIRMWARE_TYPE_ADDRESS,
		                  sizeof(nodeFirmwareConfig_t));
		(void)memcpy(&_myDiscover.data[0], &_firmwareConfig, sizeof(nodeFirmwareConfig_t));
		_len += sizeof(nodeFirmwareConfig_t);
		break;
	}
	case MY_DISCOVER_PAGEID_HWPARAMS: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_HWPARAMS);

		uint16_t data16 = hwCPUVoltage();
		(void)memcpy(&_myDiscover.data[0], &data16, sizeof(uint16_t));
		data16 = hwCPUFrequency();
		(void)memcpy(&_myDiscover.data[2], &data16, sizeof(uint16_t));
		data16 = hwFreeMem();
		(void)memcpy(&_myDiscover.data[4], &data16, sizeof(uint16_t));
		_len += 3* sizeof(uint16_t);
		break;
	}
	case MY_DISCOVER_PAGEID_HWID: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_HWID);
		(void)hwUniqueID((unique_id_t*)&_myDiscover.data[0]);
		_len += sizeof(unique_id_t);
		break;
	}
	case MY_DISCOVER_PAGEID_TRANSPORT_UPLINK: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_TRANSPORT_UPLINK);
		// not implemented yet
		break;
	}
	case MY_DISCOVER_PAGEID_BOOTLOADER: {
		dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_BOOTLOADER);
		// not implemented yet
		break;
	}

	default: {
		// legacy and page = 0 (=MY_DISCOVER_PAGEID_PARENT)
		(void)build(message, sender, NODE_SENSOR_ID, C_INTERNAL, I_DISCOVER_RESPONSE,
		            false).set(getParentNodeId());
		return; //no further processing needed
	}
	}
	(void)build(message, sender, NODE_SENSOR_ID, C_INTERNAL, I_DISCOVER_RESPONSE,
	            false).set((uint8_t*)&_myDiscover, _len);
	return;
}

