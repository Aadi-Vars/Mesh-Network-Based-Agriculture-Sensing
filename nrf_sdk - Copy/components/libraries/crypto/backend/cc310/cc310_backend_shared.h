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

#ifndef CC310_BACKEND_SHARED_H__
#define CC310_BACKEND_SHARED_H__

/** @file
 *
 * @defgroup nrf_crypto_cc310_backend_shared nrf_crypto CC310 backend shared
 * @{
 * @ingroup nrf_crypto_cc310_backend
 *
 * @brief Shared functionality for the nrf_crypto CC310 backend.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**@internal @brief Macro definition for largest possible input data on CC310 DMA. */
#define CC310_MAX_LENGTH_DMA_OPERATIONS         (0xFFFF)
#define CC310_MAX_LENGTH_DMA_AES_OPERATIONS     (0xFFF0)


/**@internal @brief Function to enable CC310 (in HW)
 */
void cc310_backend_enable(void);


/**@internal @brief Function to disable CC310 (in HW)
 */
void cc310_backend_disable(void);


/**@internal @brief Function to pass to CC310 library API as random number generator. It uses
 * nrf_crypto libary frontend API to generate random number.
 * @param[in,out] p_state Unused. Required by CC310 library API.
 * @param[in]     size    Number of bytes in generated vector.
 * @param[out]    p_data  Place where generated bytes will be written.
 */
uint32_t nrf_crypto_backend_cc310_rng(void * p_state, uint16_t size, uint8_t * p_data);


#ifdef __cplusplus
}
#endif

/**@} */

#endif // CC310_BACKEND_SHARED_H__
