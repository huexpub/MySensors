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

#ifndef MyCryptoTeensy4_h
#define MyCryptoTeensy4_h

enum _generic_status {
	kStatus_Success = 0,
	kStatus_Fail = 1,
	kStatus_ReadOnly = 2,
	kStatus_OutOfRange = 3,
	kStatus_InvalidArgument = 4,
	kStatus_Timeout = 5,
	kStatus_DCP_Again = 6,
};


typedef enum _dcp_ch_enable {
	kDCP_chDisable = 0U,    /*!< DCP channel disable */
	kDCP_ch0Enable = 1U,    /*!< DCP channel 0 enable */
	kDCP_ch1Enable = 2U,    /*!< DCP channel 1 enable */
	kDCP_ch2Enable = 4U,    /*!< DCP channel 2 enable */
	kDCP_ch3Enable = 8U,    /*!< DCP channel 3 enable */
	kDCP_chEnableAll = 15U, /*!< DCP channel enable all */
} _dcp_ch_enable_t;

typedef enum _dcp_channel {
	kDCP_Channel0 = (1u << 16), /*!< DCP channel 0. */
	kDCP_Channel1 = (1u << 17), /*!< DCP channel 1. */
	kDCP_Channel2 = (1u << 18), /*!< DCP channel 2. */
	kDCP_Channel3 = (1u << 19), /*!< DCP channel 3. */
} dcp_channel_t;


typedef enum _dcp_key_slot {
	kDCP_KeySlot0 = 0U,     /*!< DCP key slot 0. */
	kDCP_KeySlot1 = 1U,     /*!< DCP key slot 1. */
	kDCP_KeySlot2 = 2U,     /*!< DCP key slot 2.*/
	kDCP_KeySlot3 = 3U,     /*!< DCP key slot 3. */
	kDCP_OtpKey = 4U,       /*!< DCP OTP key. */
	kDCP_OtpUniqueKey = 5U, /*!< DCP unique OTP key. */
	kDCP_PayloadKey = 6U,   /*!< DCP payload key. */
} dcp_key_slot_t;

typedef enum _dcp_swap {
	kDCP_NoSwap = 0x0U,
	kDCP_KeyByteSwap = 0x40000U,
	kDCP_KeyWordSwap = 0x80000U,
	kDCP_InputByteSwap = 0x100000U,
	kDCP_InputWordSwap = 0x200000U,
	kDCP_OutputByteSwap = 0x400000U,
	kDCP_OutputWordSwap = 0x800000U,
} dcp_swap_t;

typedef enum _dcp_hash_algo_t {
	kDCP_Sha1,   /*!< SHA_1 */
	kDCP_Sha256, /*!< SHA_256 */
	kDCP_Crc32,  /*!< CRC_32 */
} dcp_hash_algo_t;

enum _dcp_hash_digest_len {
	kDCP_OutLenSha1 = 20u,
	kDCP_OutLenSha256 = 32u,
	kDCP_OutLenCrc32 = 4u,
};

enum _dcp_work_packet_bit_definitions {
	kDCP_CONTROL0_DECR_SEMAPHOR = 1u << 1, /* DECR_SEMAPHOR */
	kDCP_CONTROL0_ENABLE_HASH = 1u << 6,   /* ENABLE_HASH */
	kDCP_CONTROL0_HASH_INIT = 1u << 12,    /* HASH_INIT */
	kDCP_CONTROL0_HASH_TERM = 1u << 13,    /* HASH_TERM */
	kDCP_CONTROL1_HASH_SELECT_SHA256 = 2u << 16,
	kDCP_CONTROL1_HASH_SELECT_SHA1 = 0u << 16,
	kDCP_CONTROL1_HASH_SELECT_CRC32 = 1u << 16,
};

typedef struct _dcp_hash_ctx_t {
	uint32_t x[58];
} dcp_hash_ctx_t;

typedef union _dcp_hash_block {
	uint32_t w[DCP_HASH_BLOCK_SIZE / 4]; /*!< array of 32-bit words */
	uint8_t b[DCP_HASH_BLOCK_SIZE];      /*!< byte array */
} dcp_hash_block_t;

typedef enum _dcp_hash_algo_state {
	kDCP_StateHashInit = 1u, /*!< Init state. */
	kDCP_StateHashUpdate,    /*!< Update state. */
} dcp_hash_algo_state_t;

typedef struct _dcp_handle {
	dcp_channel_t channel;  /*!< Specify DCP channel. */
	dcp_key_slot_t
	keySlot; /*!< For operations with key (such as AES encryption/decryption), specify DCP key slot. */
	uint32_t swapConfig;    /*!< For configuration of key, input, output byte/word swap options */
	uint32_t keyWord[4];
	uint32_t iv[4];
} dcp_handle_t;

typedef struct _dcp_hash_ctx_internal {
	dcp_hash_block_t
	blk;        /*!< memory buffer. only full blocks are written to DCP during hash updates */
	size_t blksz;                /*!< number of valid bytes in memory buffer */
	dcp_hash_algo_t algo;        /*!< selected algorithm from the set of supported algorithms */
	dcp_hash_algo_state_t state; /*!< finite machine state of the hash software process */
	uint32_t fullMessageSize;    /*!< track message size */
	uint32_t ctrl0;              /*!< HASH_INIT and HASH_TERM flags */
	uint32_t runningHash[9];     /*!< running hash. up to SHA-256 plus size, that is 36 bytes. */
	dcp_handle_t *handle;
} dcp_hash_ctx_internal_t;

typedef struct _dcp_work_packet {
	uint32_t nextCmdAddress;
	uint32_t control0;
	uint32_t control1;
	uint32_t sourceBufferAddress;
	uint32_t destinationBufferAddress;
	uint32_t bufferSize;
	uint32_t payloadPointer;
	uint32_t status;
} dcp_work_packet_t;

#endif
