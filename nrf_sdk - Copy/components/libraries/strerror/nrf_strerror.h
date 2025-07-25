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

/**
 * @defgroup nrf_strerror Error code to string converter
 * @ingroup app_common
 *
 * @brief Module for converting error code into a printable string.
 * @{
 */
#ifndef NRF_STRERROR_H__
#define NRF_STRERROR_H__

#include "sdk_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function for getting a printable error string.
 *
 * @param code Error code to convert.
 *
 * @note This function cannot fail.
 *       For the function that may fail with error translation, see @ref nrf_strerror_find.
 *
 * @return Pointer to the printable string.
 *         If the string is not found,
 *         it returns a simple string that says that the error is unknown.
 */
char const * nrf_strerror_get(ret_code_t code);

/**
 * @brief Function for finding a printable error string.
 *
 * This function gets the error string in the same way as @ref nrf_strerror_get,
 * but if the string is not found, it returns NULL.
 *
 * @param code  Error code to convert.
 * @return      Pointer to the printable string.
 *              If the string is not found, NULL is returned.
 */
char const * nrf_strerror_find(ret_code_t code);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_STRERROR_H__ */
