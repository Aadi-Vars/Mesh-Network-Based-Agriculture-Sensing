/**
 * Copyright (c) 2017 - 2022, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.6-dev at Tue Sep 11 14:37:18 2018. */

#ifndef PB_DFU_CC_PB_H_INCLUDED
#define PB_DFU_CC_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum
{
    DFU_FW_TYPE_APPLICATION = 0,
    DFU_FW_TYPE_SOFTDEVICE = 1,
    DFU_FW_TYPE_BOOTLOADER = 2,
    DFU_FW_TYPE_SOFTDEVICE_BOOTLOADER = 3,
    DFU_FW_TYPE_EXTERNAL_APPLICATION = 4
} dfu_fw_type_t;
#define DFU_FW_TYPE_MIN DFU_FW_TYPE_APPLICATION
#define DFU_FW_TYPE_MAX DFU_FW_TYPE_EXTERNAL_APPLICATION
#define DFU_FW_TYPE_ARRAYSIZE ((dfu_fw_type_t)(DFU_FW_TYPE_EXTERNAL_APPLICATION+1))

typedef enum
{
    DFU_HASH_TYPE_NO_HASH = 0,
    DFU_HASH_TYPE_CRC = 1,
    DFU_HASH_TYPE_SHA128 = 2,
    DFU_HASH_TYPE_SHA256 = 3,
    DFU_HASH_TYPE_SHA512 = 4
} dfu_hash_type_t;
#define DFU_HASH_TYPE_MIN DFU_HASH_TYPE_NO_HASH
#define DFU_HASH_TYPE_MAX DFU_HASH_TYPE_SHA512
#define DFU_HASH_TYPE_ARRAYSIZE ((dfu_hash_type_t)(DFU_HASH_TYPE_SHA512+1))

typedef enum
{
    DFU_OP_CODE_INIT = 1
} dfu_op_code_t;
#define DFU_OP_CODE_MIN DFU_OP_CODE_INIT
#define DFU_OP_CODE_MAX DFU_OP_CODE_INIT
#define DFU_OP_CODE_ARRAYSIZE ((dfu_op_code_t)(DFU_OP_CODE_INIT+1))

typedef enum
{
    DFU_VALIDATION_TYPE_NO_VALIDATION = 0,
    DFU_VALIDATION_TYPE_VALIDATE_GENERATED_CRC = 1,
    DFU_VALIDATION_TYPE_VALIDATE_SHA256 = 2,
    DFU_VALIDATION_TYPE_VALIDATE_ECDSA_P256_SHA256 = 3
} dfu_validation_type_t;
#define DFU_VALIDATION_TYPE_MIN DFU_VALIDATION_TYPE_NO_VALIDATION
#define DFU_VALIDATION_TYPE_MAX DFU_VALIDATION_TYPE_VALIDATE_ECDSA_P256_SHA256
#define DFU_VALIDATION_TYPE_ARRAYSIZE ((dfu_validation_type_t)(DFU_VALIDATION_TYPE_VALIDATE_ECDSA_P256_SHA256+1))

typedef enum
{
    DFU_SIGNATURE_TYPE_ECDSA_P256_SHA256 = 0,
    DFU_SIGNATURE_TYPE_ED25519 = 1
} dfu_signature_type_t;
#define DFU_SIGNATURE_TYPE_MIN DFU_SIGNATURE_TYPE_ECDSA_P256_SHA256
#define DFU_SIGNATURE_TYPE_MAX DFU_SIGNATURE_TYPE_ED25519
#define DFU_SIGNATURE_TYPE_ARRAYSIZE ((dfu_signature_type_t)(DFU_SIGNATURE_TYPE_ED25519+1))

/* Struct definitions */
typedef PB_BYTES_ARRAY_T(64) dfu_boot_validation_bytes_t;
typedef struct {
    dfu_validation_type_t type;
    dfu_boot_validation_bytes_t bytes;
/* @@protoc_insertion_point(struct:dfu_boot_validation_t) */
} dfu_boot_validation_t;

typedef PB_BYTES_ARRAY_T(32) dfu_hash_hash_t;
typedef struct {
    dfu_hash_type_t hash_type;
    dfu_hash_hash_t hash;
/* @@protoc_insertion_point(struct:dfu_hash_t) */
} dfu_hash_t;

typedef struct {
    bool has_fw_version;
    uint32_t fw_version;
    bool has_hw_version;
    uint32_t hw_version;
    pb_size_t sd_req_count;
    uint32_t sd_req[16];
    bool has_type;
    dfu_fw_type_t type;
    bool has_sd_size;
    uint32_t sd_size;
    bool has_bl_size;
    uint32_t bl_size;
    bool has_app_size;
    uint32_t app_size;
    bool has_hash;
    dfu_hash_t hash;
    bool has_is_debug;
    bool is_debug;
    pb_size_t boot_validation_count;
    dfu_boot_validation_t boot_validation[3];
/* @@protoc_insertion_point(struct:dfu_init_command_t) */
} dfu_init_command_t;

typedef struct {
    bool has_op_code;
    dfu_op_code_t op_code;
    bool has_init;
    dfu_init_command_t init;
/* @@protoc_insertion_point(struct:dfu_command_t) */
} dfu_command_t;

typedef PB_BYTES_ARRAY_T(64) dfu_signed_command_signature_t;
typedef struct {
    dfu_command_t command;
    dfu_signature_type_t signature_type;
    dfu_signed_command_signature_t signature;
/* @@protoc_insertion_point(struct:dfu_signed_command_t) */
} dfu_signed_command_t;

typedef struct {
    bool has_command;
    dfu_command_t command;
    bool has_signed_command;
    dfu_signed_command_t signed_command;
/* @@protoc_insertion_point(struct:dfu_packet_t) */
} dfu_packet_t;

/* Default values for struct fields */
extern const bool dfu_init_command_is_debug_default;

/* Initializer values for message structs */
#define DFU_HASH_INIT_DEFAULT                    {(dfu_hash_type_t)0, {0, {0}}}
#define DFU_BOOT_VALIDATION_INIT_DEFAULT         {(dfu_validation_type_t)0, {0, {0}}}
#define DFU_INIT_COMMAND_INIT_DEFAULT            {false, 0, false, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, (dfu_fw_type_t)0, false, 0, false, 0, false, 0, false, DFU_HASH_INIT_DEFAULT, false, false, 0, {DFU_BOOT_VALIDATION_INIT_DEFAULT, DFU_BOOT_VALIDATION_INIT_DEFAULT, DFU_BOOT_VALIDATION_INIT_DEFAULT}}
#define DFU_COMMAND_INIT_DEFAULT                 {false, (dfu_op_code_t)0, false, DFU_INIT_COMMAND_INIT_DEFAULT}
#define DFU_SIGNED_COMMAND_INIT_DEFAULT          {DFU_COMMAND_INIT_DEFAULT, (dfu_signature_type_t)0, {0, {0}}}
#define DFU_PACKET_INIT_DEFAULT                  {false, DFU_COMMAND_INIT_DEFAULT, false, DFU_SIGNED_COMMAND_INIT_DEFAULT}
#define DFU_HASH_INIT_ZERO                       {(dfu_hash_type_t)0, {0, {0}}}
#define DFU_BOOT_VALIDATION_INIT_ZERO            {(dfu_validation_type_t)0, {0, {0}}}
#define DFU_INIT_COMMAND_INIT_ZERO               {false, 0, false, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, (dfu_fw_type_t)0, false, 0, false, 0, false, 0, false, DFU_HASH_INIT_ZERO, false, 0, 0, {DFU_BOOT_VALIDATION_INIT_ZERO, DFU_BOOT_VALIDATION_INIT_ZERO, DFU_BOOT_VALIDATION_INIT_ZERO}}
#define DFU_COMMAND_INIT_ZERO                    {false, (dfu_op_code_t)0, false, DFU_INIT_COMMAND_INIT_ZERO}
#define DFU_SIGNED_COMMAND_INIT_ZERO             {DFU_COMMAND_INIT_ZERO, (dfu_signature_type_t)0, {0, {0}}}
#define DFU_PACKET_INIT_ZERO                     {false, DFU_COMMAND_INIT_ZERO, false, DFU_SIGNED_COMMAND_INIT_ZERO}

/* Field tags (for use in manual encoding/decoding) */
#define DFU_BOOT_VALIDATION_TYPE_TAG             1
#define DFU_BOOT_VALIDATION_BYTES_TAG            2
#define DFU_HASH_HASH_TYPE_TAG                   1
#define DFU_HASH_HASH_TAG                        2
#define DFU_INIT_COMMAND_FW_VERSION_TAG          1
#define DFU_INIT_COMMAND_HW_VERSION_TAG          2
#define DFU_INIT_COMMAND_SD_REQ_TAG              3
#define DFU_INIT_COMMAND_TYPE_TAG                4
#define DFU_INIT_COMMAND_SD_SIZE_TAG             5
#define DFU_INIT_COMMAND_BL_SIZE_TAG             6
#define DFU_INIT_COMMAND_APP_SIZE_TAG            7
#define DFU_INIT_COMMAND_HASH_TAG                8
#define DFU_INIT_COMMAND_IS_DEBUG_TAG            9
#define DFU_INIT_COMMAND_BOOT_VALIDATION_TAG     10
#define DFU_COMMAND_OP_CODE_TAG                  1
#define DFU_COMMAND_INIT_TAG                     2
#define DFU_SIGNED_COMMAND_COMMAND_TAG           1
#define DFU_SIGNED_COMMAND_SIGNATURE_TYPE_TAG    2
#define DFU_SIGNED_COMMAND_SIGNATURE_TAG         3
#define DFU_PACKET_COMMAND_TAG                   1
#define DFU_PACKET_SIGNED_COMMAND_TAG            2

/* Struct field encoding specification for nanopb */
extern const pb_field_t dfu_hash_fields[3];
extern const pb_field_t dfu_boot_validation_fields[3];
extern const pb_field_t dfu_init_command_fields[11];
extern const pb_field_t dfu_command_fields[3];
extern const pb_field_t dfu_signed_command_fields[4];
extern const pb_field_t dfu_packet_fields[3];

/* Maximum encoded size of messages (where known) */
#define DFU_HASH_SIZE                            36
#define DFU_BOOT_VALIDATION_SIZE                 68
#define DFU_INIT_COMMAND_SIZE                    378
#define DFU_COMMAND_SIZE                         383
#define DFU_SIGNED_COMMAND_SIZE                  454
#define DFU_PACKET_SIZE                          843

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define DFU_CC_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
