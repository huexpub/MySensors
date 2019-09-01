/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2019 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */
extern "C" void __libc_init_array(void);

extern "C" void startup_late_hook(void)
{
	while (millis() < 300); // wait at least 300ms before calling user code
	__libc_init_array();
	_begin(); // Startup MySensors library
	for (;;) {
		_process();  // Process incoming data
		if (loop) {
			loop(); // Call sketch loop
		}
		yield();
	}
}
