/**
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

#include "MyHwTeensy4.h"


/*
int8_t pinIntTrigger = 0;
void wakeUp()	 //place to send the interrupts
{
	pinIntTrigger = 1;
}
void wakeUp2()	 //place to send the second interrupts
{
	pinIntTrigger = 2;
}

// Watchdog Timer interrupt service routine. This routine is required
// to allow automatic WDIF and WDIE bit clearance in hardware.
ISR (WDT_vect)
{
	// WDIE & WDIF is cleared in hardware upon entering this ISR
	wdt_disable();
}
*/

bool hwInit(void)
{
#if !defined(MY_DISABLED_SERIAL)
	MY_SERIALDEVICE.begin(MY_BAUD_RATE);
#if defined(MY_GATEWAY_SERIAL)
	while (!MY_SERIALDEVICE) {}
#endif
#endif
	tempmon_Start();
	return true;
}

void hwWatchdogReset(void)
{
	wdt_reset();
}

void hwReboot(void)
{
	SCB_AIRCR = 0x05FA0004;
	while (true);
}

int8_t hwSleep(uint32_t ms)
{
	// TODO: Not supported!
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

int8_t hwSleep(const uint8_t interrupt, const uint8_t mode, uint32_t ms)
{
	// TODO: Not supported!
	(void)interrupt;
	(void)mode;
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

int8_t hwSleep(const uint8_t interrupt1, const uint8_t mode1, const uint8_t interrupt2,
               const uint8_t mode2,
               uint32_t ms)
{
	// TODO: Not supported!
	(void)interrupt1;
	(void)mode1;
	(void)interrupt2;
	(void)mode2;
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

bool hwUniqueID(unique_id_t *uniqueID)
{
	//(void)memcpy((uint8_t *)uniqueID, &SIM_UIDH, 16);
	return true;
}

uint16_t hwCPUVoltage(void)
{
	/*
		analogReference(0);
		analogReadResolution(12);
		analogReadAveraging(32);
		// Teensy 3.6
		return (uint16_t)(1195u * 4096u / analogRead(71));
		*/
	// not supported
	return FUNCTION_NOT_SUPPORTED;
}

uint16_t hwCPUFrequency(void)
{
	// TODO: currently reporting compile time frequency (in 1/10MHz)
	return F_CPU / 100000UL;
}

int8_t hwCPUTemperature(void)
{
	return (int8_t)tempmonGetTemp();
}

uint16_t hwFreeMem(void)
{
	// TODO: Not supported!
	return FUNCTION_NOT_SUPPORTED;
}

static uint32_t rng_index;

void trng_init()
{
	CCM_CCGR6 |= CCM_CCGR6_TRNG(CCM_CCGR_ON);
	// program mode
	TRNG->MCTL = MASKVAL((TRNG_MCTL_PRGM_MASK | TRNG_MCTL_ERR_MASK), TRNG_MCTL_PRGM(1));
	TRNG->MCTL |= TRNG_MCTL_RST_DEF_MASK;  // reset  ? clear ERR_MASK
	// config regs ...
	TRNG->MCTL &= ~TRNG_MCTL_SAMP_MODE_MASK; // vonneuman  SDK
	// run mode
	TRNG->MCTL = MASKVAL((TRNG_MCTL_PRGM_MASK | TRNG_MCTL_ERR_MASK), TRNG_MCTL_PRGM(0));
	volatile uint32_t tmp;
	tmp = TRNG->ENT[TRNG_ENT_COUNT - 1]; // start gen cycle
}

void trng_deinit()
{
	TRNG->MCTL = MASKVAL((TRNG_MCTL_PRGM_MASK | TRNG_MCTL_ERR_MASK), TRNG_MCTL_PRGM(1));
}

uint32_t trng_word()
{
	uint32_t r;
	while ((TRNG->MCTL & TRNG_MCTL_ENT_VAL_MASK) == 0 &&
	        (TRNG->MCTL & TRNG_MCTL_ERR_MASK) == 0); // wait for entropy ready
	r = TRNG->ENT[rng_index++];
	if (rng_index >= TRNG_ENT_COUNT) {
		rng_index = 0;
	}
	return r;
}

void hwRandomNumberInit(void)
{
	// enable TRNG
	trng_init();
	randomSeed(trng_word());
	// disbale TRNG
	trng_deinit();
}


ssize_t hwGetentropy(void *buffer, const size_t len)
{
	// enable TRNG
	trng_init();
	size_t pos = 0;
	while (pos < len) {
		const uint32_t rndVar = trng_word();
		const uint8_t bsize = (len - pos) > sizeof(rndVar) ? sizeof(rndVar) : (len - pos);
		(void)memcpy((uint8_t *)buffer + pos, &rndVar, bsize);
		pos += bsize;
	}
	// disbale TRNG
	trng_deinit();
	return pos;
}



/*
void trng512(uint32_t *udata) {

	while ((TRNG->MCTL & TRNG_MCTL_ENT_VAL_MASK) == 0 &
		(TRNG->MCTL & TRNG_MCTL_ERR_MASK) == 0); // wait for entropy ready
	for (int i = 0; i < TRNG_ENT_COUNT; i++) udata[i] = TRNG->ENT[i];
	// reading last ENT word will start a new entropy cycle
	uint32_t tmp = TRNG->ENT[0];  // dummy read work-around from SDK
}
*/

/*
void entropy(uint32_t *w, int bytes) {
	uint8_t *byte = (uint8_t *)w;  // 64 bytes
	int i, j, v, cnts[256], bits[8];
	int vmax = 0, vmin = 999999;
	double p, e = 0, avrg = 0;
	unsigned long t;


	for (j = 0; j < 8; j++) bits[j] = 0;
	for (j = 0; j < 256; j++) cnts[j] = 0;

	for (i = 0; i < bytes; i++) {
		v = byte[i];
		if (v < vmin) vmin = v;
		if (v > vmax) vmax = v;
		cnts[v & 0xff]++;
		for (j = 0; j < 8; j++) if (v & 1 << j) bits[j]++;
	}

	for (i = 0; i < 256; i++) {
		avrg += 1.0 * i * cnts[i];
		if (cnts[i]) {
			// Serial.print(i); Serial.print(" "); Serial.println(cnts[i]);
			p = 1.0 * cnts[i] / bytes;
			e -= p * log(p) / log(2.0);
		}
	}
	Serial.print(bytes);
	Serial.print(" bytes  avrg "); Serial.print(avrg / bytes, 3);
	Serial.print(" min max "); Serial.print(vmin); Serial.print(" "); Serial.print(vmax);
	Serial.print("  entropy "); Serial.println(e, 3);
	for (j = 0; j < 8; j++) {
		Serial.print(bits[j]);
		Serial.print(" ");
	}
	Serial.println();
}
*/
