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
*
* Radio wiring Teensy4: RF24, RFM69, RFM95:
* MISO	12
* MOSI	11
* SCK	13
* CSN	10
* CE	9 (RF24)
* IRQ	8 (opt. RF24, RFM69, RFM95)
*/

#ifndef MyHwTeensy4_h
#define MyHwTeensy4_h

#include <SPI.h>
#include <EEPROM.h>
#include "util/atomic.h"
#include "avr/wdt.h"

#ifdef __cplusplus
#include <Arduino.h>
#endif

#define CRYPTO_LITTLE_ENDIAN

#ifndef MY_SERIALDEVICE
#define MY_SERIALDEVICE Serial
#endif

#ifndef MY_DEBUGDEVICE
#define MY_DEBUGDEVICE MY_SERIALDEVICE
#endif

// redefine 8 bit types of inttypes.h (as of Teensy loader 1.48
#undef PRId8
#undef PRIi8
#undef PRIo8
#undef PRIu8
#undef PRIx8
#undef PRIX8
#undef PRIdLEAST8
#undef PRIiLEAST8
#undef PRIoLEAST8
#undef PRIuLEAST8
#undef PRIxLEAST8
#undef PRIXLEAST8
#undef PRIdFAST8
#undef PRIiFAST8
#undef PRIoFAST8
#undef PRIuFAST8
#undef PRIxFAST8
#undef PRIXFAST8
#define PRId8		"d"
#define PRIi8		"i"
#define PRIo8		"o"
#define PRIu8		"u"
#define PRIx8		"x"
#define PRIX8		"X"
#define PRIdLEAST8	"d"
#define PRIiLEAST8	"i"
#define PRIoLEAST8	"o"
#define PRIuLEAST8	"u"
#define PRIxLEAST8	"x"
#define PRIXLEAST8	"X"
#define PRIdFAST8	"d"
#define PRIiFAST8	"i"
#define PRIoFAST8	"o"
#define PRIuFAST8	"u"
#define PRIxFAST8	"x"
#define PRIXFAST8	"X"

// Define these as macros to save valuable space
#define hwDigitalWrite(__pin, __value) digitalWriteFast(__pin, __value)
#define hwDigitalRead(__pin) digitalReadFast(__pin)
#define hwPinMode(__pin, __value) pinMode(__pin, __value)
#define hwMillis() millis()
#define hwGetSleepRemaining() (0ul)

void hwRandomNumberInit(void);
bool hwInit(void);
void hwWatchdogReset();
void hwReboot(void);

#define hwReadConfig(__pos) eeprom_read_byte((const uint8_t *)__pos)
#define hwWriteConfig(__pos, __val) eeprom_write_byte((uint8_t *)__pos, (uint8_t)__val)
#define hwReadConfigBlock(__buf, __pos, __length) eeprom_read_block((void *)__buf, (const void *)__pos, (uint32_t)__length)
#define hwWriteConfigBlock(__buf, __pos, __length) eeprom_write_block((const void *)__buf, (void *)__pos, (uint32_t)__length)

// SOFTSPI
#ifdef MY_SOFTSPI
#error Soft SPI is not available on this architecture!
#endif
#define hwSPI SPI //!< hwSPI

#define MY_HW_HAS_GETENTROPY

#define MY_CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

// missing HW register definitions

#define __IO volatile
#define __I  volatile

/** DCP - Register Layout Typedef */
typedef struct {
	__IO uint32_t CTRL;                              /**< DCP control register 0, offset: 0x0 */
	uint8_t RESERVED_0[12];
	__IO uint32_t STAT;                              /**< DCP status register, offset: 0x10 */
	uint8_t RESERVED_1[12];
	__IO uint32_t CHANNELCTRL;                       /**< DCP channel control register, offset: 0x20 */
	uint8_t RESERVED_2[12];
	__IO uint32_t CAPABILITY0;                       /**< DCP capability 0 register, offset: 0x30 */
	uint8_t RESERVED_3[12];
	__I  uint32_t CAPABILITY1;                       /**< DCP capability 1 register, offset: 0x40 */
	uint8_t RESERVED_4[12];
	__IO uint32_t CONTEXT;                           /**< DCP context buffer pointer, offset: 0x50 */
	uint8_t RESERVED_5[12];
	__IO uint32_t KEY;                               /**< DCP key index, offset: 0x60 */
	uint8_t RESERVED_6[12];
	__IO uint32_t KEYDATA;                           /**< DCP key data, offset: 0x70 */
	uint8_t RESERVED_7[12];
	__I  uint32_t
	PACKET0;                           /**< DCP work packet 0 status register, offset: 0x80 */
	uint8_t RESERVED_8[12];
	__I  uint32_t
	PACKET1;                           /**< DCP work packet 1 status register, offset: 0x90 */
	uint8_t RESERVED_9[12];
	__I  uint32_t
	PACKET2;                           /**< DCP work packet 2 status register, offset: 0xA0 */
	uint8_t RESERVED_10[12];
	__I  uint32_t
	PACKET3;                           /**< DCP work packet 3 status register, offset: 0xB0 */
	uint8_t RESERVED_11[12];
	__I  uint32_t
	PACKET4;                           /**< DCP work packet 4 status register, offset: 0xC0 */
	uint8_t RESERVED_12[12];
	__I  uint32_t
	PACKET5;                           /**< DCP work packet 5 status register, offset: 0xD0 */
	uint8_t RESERVED_13[12];
	__I  uint32_t
	PACKET6;                           /**< DCP work packet 6 status register, offset: 0xE0 */
	uint8_t RESERVED_14[28];
	__IO uint32_t
	CH0CMDPTR;                         /**< DCP channel 0 command pointer address register, offset: 0x100 */
	uint8_t RESERVED_15[12];
	__IO uint32_t
	CH0SEMA;                           /**< DCP channel 0 semaphore register, offset: 0x110 */
	uint8_t RESERVED_16[12];
	__IO uint32_t
	CH0STAT;                           /**< DCP channel 0 status register, offset: 0x120 */
	uint8_t RESERVED_17[12];
	__IO uint32_t
	CH0OPTS;                           /**< DCP channel 0 options register, offset: 0x130 */
	uint8_t RESERVED_18[12];
	__IO uint32_t
	CH1CMDPTR;                         /**< DCP channel 1 command pointer address register, offset: 0x140 */
	uint8_t RESERVED_19[12];
	__IO uint32_t
	CH1SEMA;                           /**< DCP channel 1 semaphore register, offset: 0x150 */
	uint8_t RESERVED_20[12];
	__IO uint32_t
	CH1STAT;                           /**< DCP channel 1 status register, offset: 0x160 */
	uint8_t RESERVED_21[12];
	__IO uint32_t
	CH1OPTS;                           /**< DCP channel 1 options register, offset: 0x170 */
	uint8_t RESERVED_22[12];
	__IO uint32_t
	CH2CMDPTR;                         /**< DCP channel 2 command pointer address register, offset: 0x180 */
	uint8_t RESERVED_23[12];
	__IO uint32_t
	CH2SEMA;                           /**< DCP channel 2 semaphore register, offset: 0x190 */
	uint8_t RESERVED_24[12];
	__IO uint32_t
	CH2STAT;                           /**< DCP channel 2 status register, offset: 0x1A0 */
	uint8_t RESERVED_25[12];
	__IO uint32_t
	CH2OPTS;                           /**< DCP channel 2 options register, offset: 0x1B0 */
	uint8_t RESERVED_26[12];
	__IO uint32_t
	CH3CMDPTR;                         /**< DCP channel 3 command pointer address register, offset: 0x1C0 */
	uint8_t RESERVED_27[12];
	__IO uint32_t
	CH3SEMA;                           /**< DCP channel 3 semaphore register, offset: 0x1D0 */
	uint8_t RESERVED_28[12];
	__IO uint32_t
	CH3STAT;                           /**< DCP channel 3 status register, offset: 0x1E0 */
	uint8_t RESERVED_29[12];
	__IO uint32_t
	CH3OPTS;                           /**< DCP channel 3 options register, offset: 0x1F0 */
	uint8_t RESERVED_30[524];
	__IO uint32_t DBGSELECT;                         /**< DCP debug select register, offset: 0x400 */
	uint8_t RESERVED_31[12];
	__I  uint32_t DBGDATA;                           /**< DCP debug data register, offset: 0x410 */
	uint8_t RESERVED_32[12];
	__IO uint32_t PAGETABLE;                         /**< DCP page table register, offset: 0x420 */
	uint8_t RESERVED_33[12];
	__I  uint32_t VERSION;                           /**< DCP version register, offset: 0x430 */
} DCP_Type;

#define DCP_BASE                                (0x402FC000u)
#define DCP                                      ((DCP_Type *)DCP_BASE)

#define DCP_CH0SEMA_VALUE_MASK                   (0xFF0000U)
#define DCP_CH0STAT_ERROR_CODE_MASK              (0xFF0000U)
#define DCP_HASH_BLOCK_SIZE  128
#define DCP_STAT_OTP_KEY_READY_MASK              (0x10000000U)
#define DCP_KEY_INDEX_MASK                       (0x30U)
#define DCP_KEY_INDEX_SHIFT                      (4U)
#define DCP_KEY_INDEX(x)                         (((uint32_t)(((uint32_t)(x)) << DCP_KEY_INDEX_SHIFT)) & DCP_KEY_INDEX_MASK)


/** TRNG - Register Layout Typedef */
#define TRNG_ENT_COUNT 16

typedef struct {
	__IO uint32_t MCTL;                              /**< Miscellaneous Control Register, offset: 0x0 */
	__IO uint32_t
	SCMISC;                            /**< Statistical Check Miscellaneous Register, offset: 0x4 */
	__IO uint32_t PKRRNG;                            /**< Poker Range Register, offset: 0x8 */
	union {                                          /* offset: 0xC */
		__IO uint32_t PKRMAX;                            /**< Poker Maximum Limit Register, offset: 0xC */
		__I  uint32_t
		PKRSQ;                             /**< Poker Square Calculation Result Register, offset: 0xC */
	};
	__IO uint32_t SDCTL;                             /**< Seed Control Register, offset: 0x10 */
	union {                                          /* offset: 0x14 */
		__IO uint32_t SBLIM;                             /**< Sparse Bit Limit Register, offset: 0x14 */
		__I  uint32_t TOTSAM;                            /**< Total Samples Register, offset: 0x14 */
	};
	__IO uint32_t
	FRQMIN;                            /**< Frequency Count Minimum Limit Register, offset: 0x18 */
	union {                                          /* offset: 0x1C */
		__I  uint32_t FRQCNT;                            /**< Frequency Count Register, offset: 0x1C */
		__IO uint32_t
		FRQMAX;                            /**< Frequency Count Maximum Limit Register, offset: 0x1C */
	};
	union {                                          /* offset: 0x20 */
		__I  uint32_t
		SCMC;                              /**< Statistical Check Monobit Count Register, offset: 0x20 */
		__IO uint32_t
		SCML;                              /**< Statistical Check Monobit Limit Register, offset: 0x20 */
	};
	union {                                          /* offset: 0x24 */
		__I  uint32_t
		SCR1C;                             /**< Statistical Check Run Length 1 Count Register, offset: 0x24 */
		__IO uint32_t
		SCR1L;                             /**< Statistical Check Run Length 1 Limit Register, offset: 0x24 */
	};
	union {                                          /* offset: 0x28 */
		__I  uint32_t
		SCR2C;                             /**< Statistical Check Run Length 2 Count Register, offset: 0x28 */
		__IO uint32_t
		SCR2L;                             /**< Statistical Check Run Length 2 Limit Register, offset: 0x28 */
	};
	union {                                          /* offset: 0x2C */
		__I  uint32_t
		SCR3C;                             /**< Statistical Check Run Length 3 Count Register, offset: 0x2C */
		__IO uint32_t
		SCR3L;                             /**< Statistical Check Run Length 3 Limit Register, offset: 0x2C */
	};
	union {                                          /* offset: 0x30 */
		__I  uint32_t
		SCR4C;                             /**< Statistical Check Run Length 4 Count Register, offset: 0x30 */
		__IO uint32_t
		SCR4L;                             /**< Statistical Check Run Length 4 Limit Register, offset: 0x30 */
	};
	union {                                          /* offset: 0x34 */
		__I  uint32_t
		SCR5C;                             /**< Statistical Check Run Length 5 Count Register, offset: 0x34 */
		__IO uint32_t
		SCR5L;                             /**< Statistical Check Run Length 5 Limit Register, offset: 0x34 */
	};
	union {                                          /* offset: 0x38 */
		__I  uint32_t
		SCR6PC;                            /**< Statistical Check Run Length 6+ Count Register, offset: 0x38 */
		__IO uint32_t
		SCR6PL;                            /**< Statistical Check Run Length 6+ Limit Register, offset: 0x38 */
	};
	__I  uint32_t STATUS;                            /**< Status Register, offset: 0x3C */
	__I  uint32_t
	ENT[TRNG_ENT_COUNT];                           /**< Entropy Read Register, array offset: 0x40, array step: 0x4 */
	__I  uint32_t
	PKRCNT10;                          /**< Statistical Check Poker Count 1 and 0 Register, offset: 0x80 */
	__I  uint32_t
	PKRCNT32;                          /**< Statistical Check Poker Count 3 and 2 Register, offset: 0x84 */
	__I  uint32_t
	PKRCNT54;                          /**< Statistical Check Poker Count 5 and 4 Register, offset: 0x88 */
	__I  uint32_t
	PKRCNT76;                          /**< Statistical Check Poker Count 7 and 6 Register, offset: 0x8C */
	__I  uint32_t
	PKRCNT98;                          /**< Statistical Check Poker Count 9 and 8 Register, offset: 0x90 */
	__I  uint32_t
	PKRCNTBA;                          /**< Statistical Check Poker Count B and A Register, offset: 0x94 */
	__I  uint32_t
	PKRCNTDC;                          /**< Statistical Check Poker Count D and C Register, offset: 0x98 */
	__I  uint32_t
	PKRCNTFE;                          /**< Statistical Check Poker Count F and E Register, offset: 0x9C */
	__IO uint32_t
	SEC_CFG;                           /**< Security Configuration Register, offset: 0xA0 */
	__IO uint32_t INT_CTRL;                          /**< Interrupt Control Register, offset: 0xA4 */
	__IO uint32_t INT_MASK;                          /**< Mask Register, offset: 0xA8 */
	__I  uint32_t INT_STATUS;                        /**< Interrupt Status Register, offset: 0xAC */
	uint8_t RESERVED_0[64];
	__I  uint32_t VID1;                              /**< Version ID Register (MS), offset: 0xF0 */
	__I  uint32_t VID2;                              /**< Version ID Register (LS), offset: 0xF4 */
} TRNG_Type;

#define TRNG_BASE                                (0x400CC000u)
#define TRNG                                     ((TRNG_Type *)TRNG_BASE)

#define TRNG_MCTL_SAMP_MODE_MASK                 (0x3U)
#define TRNG_MCTL_OSC_DIV_MASK                   (0xCU)
#define TRNG_MCTL_UNUSED4_MASK                   (0x10U)
#define TRNG_MCTL_UNUSED5_MASK                   (0x20U)
#define TRNG_MCTL_RST_DEF_MASK                   (0x40U)
#define TRNG_MCTL_FOR_SCLK_MASK                  (0x80U)
#define TRNG_MCTL_FCT_FAIL_MASK                  (0x100U)
#define TRNG_MCTL_FCT_VAL_MASK                   (0x200U)
#define TRNG_MCTL_ENT_VAL_MASK                   (0x400U)
#define TRNG_MCTL_TST_OUT_MASK                   (0x800U)
#define TRNG_MCTL_ERR_MASK                       (0x1000U)
#define TRNG_MCTL_TSTOP_OK_MASK                  (0x2000U)
#define TRNG_MCTL_LRUN_CONT_MASK                 (0x4000U)
#define TRNG_MCTL_PRGM_MASK                      (0x10000U)

// PRGM 0 run mode   1 program mode
#define TRNG_MCTL_PRGM_SHIFT                     (16U)
#define MASKVAL(mask,val)  ((~(mask) & TRNG->MCTL) | val)
#define TRNG_MCTL_PRGM(x) (((uint32_t)(((uint32_t)(x)) << TRNG_MCTL_PRGM_SHIFT)) & TRNG_MCTL_PRGM_MASK)



#endif
