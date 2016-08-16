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

#include "MyDiscover.h"

// MY_HWID
#if defined(ARDUINO_ARCH_AVR)
	#include <avr/boot.h>
	#define MY_DISCOVER_HARDWARE_ID			MY_DISCOVER_HWID_AVR
#elif defined (ARDUINO_ARCH_ESP8266)
	#define MY_DISCOVER_HARDWARE_ID			MY_DISCOVER_HWID_ESP8266
#elif defined (ARDUINO_ARCH_SAMD)
	#define MY_DISCOVER_HARDWARE_ID			MY_DISCOVER_HWID_SAMD
#elif defined (ARDUINO_ARCH_RASPBERRY)								// check
	#define MY_DISCOVER_HARDWARE_ID			MY_DISCOVER_HWID_RPI
#else
	#define MY_DISCOVER_HARDWARE_ID			MY_DISCOVER_HWID_UNKNOWN
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

#if defined(MY_RADIO_NRF24)
	#define MY_DISCOVER_TRANSPORT_NRF24				MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_TRANSPORT_NRF24				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RADIO_RFM69)
	#define MY_DISCOVER_TRANSPORT_RFM69				MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_TRANSPORT_RFM69				MY_DISCOVER_NOT_AVAILABLE
#endif

#if defined(MY_RS485)
	#define MY_DISCOVER_TRANSPORT_RS485			MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_TRANSPORT_RS485			MY_DISCOVER_NOT_AVAILABLE
#endif

#define MY_DISCOVER_TRANSPORT_COUNT (MY_DISCOVER_TRANSPORT_RS232 + MY_DISCOVER_TRANSPORT_TCPIP + MY_DISCOVER_TRANSPORT_NRF24 + MY_DISCOVER_TRANSPORT_RFM69 + MY_DISCOVER_TRANSPORT_RS485)

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
#if !defined(MY_GATEWAY_FEATURE) || defined(MY_SENSOR_NODE)
	#define MY_DISCOVER_SENSORS				MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_SENSORS				MY_DISCOVER_NOT_AVAILABLE
#endif

// node power
#if defined(MY_POWER_PSU)
	#define MY_DISCOVER_PSU					MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_PSU					MY_DISCOVER_NOT_AVAILABLE
#endif
#if defined(MY_POWER_BATTERY)
	#define MY_DISCOVER_BATTERY				MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_BATTERY				MY_DISCOVER_NOT_AVAILABLE
#endif
#if defined(MY_POWER_SOLAR)
	#define MY_DISCOVER_SOLAR				MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_SOLAR				MY_DISCOVER_NOT_AVAILABLE
#endif
#define MY_DISCOVER_NODE_TYPE ((MY_DISCOVER_SOLAR<<6) |(MY_DISCOVER_BATTERY<<5) |(MY_DISCOVER_PSU<<4) | (MY_DISCOVER_BRIDGE << 4)| (MY_DISCOVER_GATEWAY << 2) | (MY_DISCOVER_REPEATER << 1) | MY_DISCOVER_SENSORS)

// node features
#if defined(MY_OTA_FIRMWARE_FEATURE)
	#define MY_DISCOVER_OTA_FIRMWARE_FEATURE	MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_OTA_FIRMWARE_FEATURE	MY_DISCOVER_NOT_AVAILABLE
#endif
#if defined(MY_SLEEPING_NODE)
	#define MY_DISCOVER_SLEEPING_NODE			MY_DISCOVER_AVAILABLE
#else
	#define MY_DISCOVER_SLEEPING_NODE			MY_DISCOVER_NOT_AVAILABLE
#endif
#define MY_DISCOVER_NODE_FEATURES (MY_DISCOVER_SLEEPING_NODE<<2) |(MY_DISCOVER_REMOTE_RESET<<1) | MY_DISCOVER_OTA_FIRMWARE_FEATURE

// uplink transport - serial
#if defined(MY_GATEWAY_SERIAL) || defined(MY_RS485)
	#if defined(MY_GATEWAY_SERIAL)
		#define MY_DISCOVER_TRANSPORT_PARENT_TYPE	MY_DISCOVER_RS232
	#else
		#define MY_DISCOVER_TRANSPORT_PARENT_TYPE	MY_DISCOVER_RS485
	#endif
	// undefined properties
	#define MY_DISCOVER_PARENT_POWER_LEVEL		MY_DISCOVER_UNDEFINED
	#define MY_DISCOVER_PARENT_CHANNEL			((uint16_t)MY_DISCOVER_UNDEFINED)

	#if (MY_BAUD_RATE==9600)
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_9600
	#elif (MY_BAUD_RATE==19200)
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_19200
	#elif (MY_BAUD_RATE==38400)
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_38400
	#elif (MY_BAUD_RATE==57600)
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_57600
	#elif (MY_BAUD_RATE==115200)
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_115200
	#else
		#define MY_DISCOVER_PARENT_DATA_RATE	MY_DISCOVER_BAUD_UNKNOWN
	#endif
#elif defined(MY_GATEWAY_W5100) || defined(MY_GATEWAY_ENC28J60) || defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_MQTT_CLIENT)
	#define MY_DISCOVER_TRANSPORT_PARENT_TYPE	MY_DISCOVER_TCPIP
	#define MY_DISCOVER_PARENT_DATA_RATE		MY_DISCOVER_UNDEFINED
	#define MY_DISCOVER_PARENT_POWER_LEVEL		MY_DISCOVER_UNDEFINED
	#define MY_DISCOVER_PARENT_CHANNEL			((uint16_t)MY_PORT)
#elif defined(MY_RADIO_NRF24)
	#define MY_DISCOVER_TRANSPORT_PARENT_TYPE	MY_DISCOVER_NRF24
	#define MY_DISCOVER_PARENT_CHANNEL			((uint16_t)MY_RF24_CHANNEL)
	// data rate
	#if (MY_RF24_DATARATE==RF24_250KBPS)
		#define MY_DISCOVER_PARENT_DATA_RATE	(1)
	#elif (MY_RF24_DATARATE==RF24_1MBPS)
		#define MY_DISCOVER_PARENT_DATA_RATE	(2)
	#elif (MY_RF24_DATARATE==RF24_2MBPS)
		#define MY_DISCOVER_PARENT_DATA_RATE	(3)
	#else
		#define MY_DISCOVER_PARENT_DATA_RATE	(0)
	#endif
	// tx level
	#if (MY_RF24_PA_LEVEL==RF24_PA_MIN)
		#define MY_DISCOVER_PARENT_POWER_LEVEL	(1)
	#elif (MY_RF24_PA_LEVEL==RF24_PA_LOW)
		#define MY_DISCOVER_PARENT_POWER_LEVEL	(2)
	#elif (MY_RF24_PA_LEVEL==RF24_PA_HIGH)
		#define MY_DISCOVER_PARENT_POWER_LEVEL	(3)
	#elif (MY_RF24_PA_LEVEL==RF24_PA_MAX)
		#define MY_DISCOVER_PARENT_POWER_LEVEL	(4)
	#else
		#define MY_DISCOVER_PARENT_POWER_LEVEL	(0)
	#endif
#elif defined(MY_RADIO_RFM69)
	#define MY_DISCOVER_TRANSPORT_PARENT_TYPE	MY_DISCOVER_RFM69
	#define MY_DISCOVER_PARENT_DATA_RATE		MY_DISCOVER_UNDEFINED
	#define MY_DISCOVER_PARENT_POWER_LEVEL		MY_DISCOVER_UNDEFINED
	#define MY_DISCOVER_PARENT_CHANNEL			((uint16_t)MY_RFM69_FREQUENCY)
#endif

#define MY_DISCOVER_TRANSPORT_PARENT MY_DISCOVER_TRANSPORT_PARENT_TYPE

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

#define MY_DISCOVER_SIGNATURES (MY_DISCOVER_NODE_WHITELISTING << 2) | (MY_DISCOVER_REQUEST_SIGNATURES << 1) | (MY_DISCOVER_SIGNING) 

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

// remote reset
#if !defined(MY_DISABLE_REMOTE_RESET)
	#define MY_DISCOVER_REMOTE_RESET			MY_DISCOVER_AVAILABLE	
#else
	#define MY_DISCOVER_REMOTE_RESET			MY_DISCOVER_NOT_AVAILABLE
#endif

// OTA update feature
#if defined(MY_OTA_FIRMWARE_FEATURE)
	#define MY_DISCOVER_OTA_UPDATE_FEATURE		MY_DISCOVER_AVAILABLE		
#else
	#define MY_DISCOVER_OTA_UPDATE_FEATURE		MY_DISCOVER_NOT_AVAILABLE
#endif




bool prepareDiscoverResponse(MyMessage &message, const uint8_t sender, const uint8_t page) {
	MyDiscover _myDiscover;

	dSetPageID(_myDiscover, page);
	dSetVersion(_myDiscover, MY_DISCOVER_HEADER_VERSION);
	dSetPageTypeID(_myDiscover, MY_DISCOVER_PAGEID_GENERAL);
	dSetHWID(_myDiscover, MY_DISCOVER_HARDWARE_ID);

	uint8_t _len = MY_DISCOVER_HEADER_SIZE;


	switch (page) {
		case MY_DISCOVER_PAGEID_PARENT: {
			_myDiscover.data[0] = getParentNodeId();
			_len += 1;
			break;
		}
		case MY_DISCOVER_PAGEID_GENERAL: {
			// for all platforms
			_myDiscover.data[0] = (MY_DISCOVER_TRANSPORT_COUNT << 5) | MY_DISCOVER_TOTAL_PAGES;
			_myDiscover.data[1] = MY_DISCOVER_NODE_TYPE;
			_myDiscover.data[2] = getParentNodeId();
			_myDiscover.data[3] = getDistanceGW();
			_myDiscover.data[4] = MY_DISCOVER_NODE_FEATURES;
			uint32_t data32 = hwMillis();
			(void)memcpy(&_myDiscover.data[5], &data32, 4);
			nodeFirmwareConfig_t _firmwareConfig;
			hwReadConfigBlock((void*)&_firmwareConfig, (void*)EEPROM_FIRMWARE_TYPE_ADDRESS, sizeof(nodeFirmwareConfig_t));
			(void)memcpy(&_myDiscover.data[9], &_firmwareConfig, 8);

			_len += 17;

			break;
		}
		case MY_DISCOVER_PAGEID_ARCHITECTURE: {
			uint16_t data16 = hwCPUVoltage();
			(void)memcpy(&_myDiscover.data[0], &data16, 2);
			data16 = hwCPUFrequency();
			(void)memcpy(&_myDiscover.data[2], &data16, 2);
			data16 = hwFreeMem();
			(void)memcpy(&_myDiscover.data[4], &data16, 2);

			_len += 6;

			#if defined(MY_DISCOVER_HWID_AVR)
				_myDiscover.data[6] = boot_signature_byte_get(0x00);
				_myDiscover.data[7] = boot_signature_byte_get(0x02);
				_myDiscover.data[8] = boot_signature_byte_get(0x04);
				_myDiscover.data[9] = 0xFF;
				_myDiscover.data[10] = CLKPR;
				_myDiscover.data[11] = GET_LOW_FUSE_BITS;
				_myDiscover.data[12] = GET_LOCK_BITS;
				_myDiscover.data[13] = GET_HIGH_FUSE_BITS;
				_myDiscover.data[14] = GET_EXTENDED_FUSE_BITS;

				_len += 9;

			#elif defined(MY_DISCOVER_HWID_ESP8266)
				uint32_t data32 = ESP.getChipId();
				memcpy(&_myDiscover.data[6], &data32, 4);
				data32 = ESP.getFlashChipId();
				memcpy(&_myDiscover.data[10], &data32, 4);

				_len += 8;
		
			#elif defined(MY_DISCOVER_HWID_SAMD)
				// backlog
			#elif defined(MY_DISCOVER_HWID_RPI)
				// backlog
			#else
				// backlog
			#endif
			
			break;
		}
		case MY_DISCOVER_PAGEID_TRANSPORT_UPLINK: {
			
			break;
		}

		default: {
			return false;
		}
	}

	(void)build(message, sender, NODE_SENSOR_ID, C_INTERNAL, I_DISCOVER_RESPONSE, false).set((uint8_t*)&_myDiscover, _len);

	return true;
}

