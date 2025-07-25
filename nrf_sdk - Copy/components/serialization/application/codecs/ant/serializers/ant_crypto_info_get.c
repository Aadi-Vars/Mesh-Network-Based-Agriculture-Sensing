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

#include <stdlib.h>
#include <string.h>
#include "ant_app.h"
#include "ble_serialization.h"
#include "ant_struct_serialization.h"
#include "cond_field_serialization.h"
#include "app_util.h"

uint32_t ant_crypto_info_get_req_enc(uint8_t          type,
                                     uint8_t * const  p_buf,
                                     uint32_t * const p_buf_len)
{
    SER_ASSERT_NOT_NULL(p_buf);
    SER_ASSERT_NOT_NULL(p_buf_len);

    uint8_t svc_number = SVC_ANT_CRYPTO_INFO_GET;
    uint32_t err_code = NRF_SUCCESS;
    uint32_t buf_len = *p_buf_len;
    uint32_t index = 0;

    err_code = uint8_t_enc(&svc_number, p_buf, buf_len, &index);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    err_code = uint8_t_enc(&type, p_buf, buf_len, &index);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    SER_ASSERT_LENGTH_LEQ(index, buf_len);

    *p_buf_len = index;

    return err_code;
}

uint32_t ant_crypto_info_get_rsp_dec(uint8_t const * const p_buf,
                                     uint32_t              packet_len,
                                     void * const          p_info,
                                     uint32_t * const      p_result_code)
{
    SER_ASSERT_NOT_NULL(p_buf);
    SER_ASSERT_NOT_NULL(p_result_code);

    uint32_t index    = 0;
    uint32_t err_code = ser_ble_cmd_rsp_result_code_dec(p_buf, &index, packet_len,
                                                        SVC_ANT_CRYPTO_INFO_GET, p_result_code);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    if (*p_result_code != NRF_SUCCESS)
    {
        SER_ASSERT_LENGTH_EQ(index, packet_len);

        return NRF_SUCCESS;
    }

    uint8_t type;
    uint8_t crypto_info_size;

    err_code = uint8_t_dec(p_buf, packet_len, &index, &type);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    switch (type)
    {
        case ENCRYPTION_INFO_GET_SUPPORTED_MODE:
            crypto_info_size = MESG_CONFIG_ENCRYPT_REQ_CAPABILITIES_SIZE - MESG_CHANNEL_NUM_SIZE;
            break;
        case ENCRYPTION_INFO_GET_CRYPTO_ID:
            crypto_info_size = MESG_CONFIG_ENCRYPT_REQ_CONFIG_ID_SIZE - MESG_CHANNEL_NUM_SIZE;
            break;
        case ENCRYPTION_INFO_GET_CUSTOM_USER_DATA:
            crypto_info_size = (MESG_CONFIG_ENCRYPT_REQ_CONFIG_USER_DATA_SIZE -
                                MESG_CHANNEL_NUM_SIZE);
            break;
        default:
           crypto_info_size = 0;
           break;
    }

    err_code = uint8_vector_dec(p_buf, packet_len, &index, (uint8_t *)p_info, crypto_info_size);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    SER_ASSERT_LENGTH_LEQ(index, packet_len);

    return NRF_SUCCESS;
}
