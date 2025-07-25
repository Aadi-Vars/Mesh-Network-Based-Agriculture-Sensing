/**
 * Copyright (c) 2013 - 2022, Nordic Semiconductor ASA
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
/**@file
 *
 * @defgroup soc_app SOC Application command request encoders and command response decoders
 * @{
 * @ingroup  ser_app_s130_codecs
 *
 * @brief    SOC Application command request encoders and command response decoders.
 */

#ifndef NRF_SOC_APP_H__
#define NRF_SOC_APP_H__

#include <stdint.h>
#include "nrf_soc.h"

#ifdef __cplusplus
extern "C" {
#endif
/**@brief Encodes @ref sd_power_system_off command request.
 *
 *
 * @param[in] p_buf          Pointer to buffer where encoded data command will be returned.
 * @param[in,out] p_buf_len  \c in: size of p_buf buffer. \c out: Length of encoded command packet.
 *
 * @retval NRF_SUCCESS                Encoding success.
 * @retval NRF_ERROR_NULL             Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH   Encoding failure. Incorrect buffer length.
 */
uint32_t power_system_off_req_enc(uint8_t * const p_buf, uint32_t * const p_buf_len);


/**@brief Encodes @ref sd_temp_get command request.
 *
 * @sa @ref temp_get_rsp_dec for command response decoder.
 *
 * @param[in] p_temp         Pointer to result of temperature measurement.
 * @param[in] p_buf          Pointer to buffer where encoded data command will be returned.
 * @param[in,out] p_buf_len  \c in: size of p_buf buffer. \c out: Length of encoded command packet.
 *
 * @retval NRF_SUCCESS                Encoding success.
 * @retval NRF_ERROR_NULL             Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH   Encoding failure. Incorrect buffer length.
 */
uint32_t temp_get_req_enc(int32_t const * const p_temp,
                          uint8_t * const       p_buf,
                          uint32_t * const      p_buf_len);

/**@brief Decodes response to @ref sd_temp_get command.
 *
 * @sa @ref temp_get_req_enc for command request encoder.
 *
 * @param[in] p_buf          Pointer to beginning of command response packet.
 * @param[in] packet_len     Length (in bytes) of response packet.
 * @param[out] p_result_code Command result code.
 * @param[out] pp_temp       Pointer to result of temperature measurement.
 *
 * @retval NRF_SUCCESS              Version information stored successfully.
 * @retval NRF_ERROR_INVALID_LENGTH Decoding failure. Incorrect buffer length.
 * @retval NRF_ERROR_DATA_SIZE      Decoding failure. Length of \p p_event is too small to
 *                                  hold decoded event.
 */
uint32_t temp_get_rsp_dec(uint8_t const * const p_buf,
                          uint32_t              packet_len,
                          uint32_t * const      p_result_code,
                          int32_t * * const     pp_temp);

/**@brief Encodes @ref sd_ecb_block_encrypt command request.
 *
 * @sa @ref ecb_block_encrypt_rsp_dec for command response decoder.
 *
 * @param[in] p_ecb_data     Pointer to ECB data.
 * @param[in] p_buf          Pointer to buffer where encoded data command will be returned.
 * @param[in,out] p_buf_len  \c in: size of p_buf buffer. \c out: Length of encoded command packet.
 *
 * @retval NRF_SUCCESS                Encoding success.
 * @retval NRF_ERROR_NULL             Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH   Encoding failure. Incorrect buffer length.
 */
uint32_t ecb_block_encrypt_req_enc(nrf_ecb_hal_data_t * p_ecb_data,
                                     uint8_t * const              p_buf,
                                     uint32_t * const             p_buf_len);

/**@brief Decodes response to @ref sd_ecb_block_encrypt command.
 *
 * @sa @ref ecb_block_encrypt_req_enc for command request encoder.
 *
 * @param[in] p_buf            Pointer to beginning of command response packet.
 * @param[in] packet_len       Length (in bytes) of response packet.
 * @param[out] p_ecb_data     Pointer to ECB data.
 * @param[out] p_result_code   Command result code.
 *
 * @retval NRF_SUCCESS              Version information stored successfully.
 * @retval NRF_ERROR_INVALID_LENGTH Decoding failure. Incorrect buffer length.
 * @retval NRF_ERROR_DATA_SIZE      Decoding failure. Length of \p p_event is too small to
 *                                  hold decoded event.
 */
uint32_t ecb_block_encrypt_rsp_dec(uint8_t const * const        p_buf,
                                   uint32_t                     packet_len,
                                   nrf_ecb_hal_data_t * * const p_ecb_data,
                                   uint32_t * const             p_result_code);
/** @} */


#ifdef __cplusplus
}
#endif

#endif // NRF_SOC_APP_H__
