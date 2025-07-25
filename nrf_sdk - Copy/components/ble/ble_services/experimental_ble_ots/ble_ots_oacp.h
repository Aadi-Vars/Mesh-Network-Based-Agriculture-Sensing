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

/**@file
 *
 * @defgroup ble_sdk_srv_ots_oacp Object Transfer Service, OACP handling
 * @{
 * @ingroup  ble_ots
 * @brief    Object Transfer Service module
 *
 * @details  This module is responsible for handling the Object Transfer Service
 *           Object Action Control Point.
 */
#ifndef BLE_OTS_OACP_H__
#define BLE_OTS_OACP_H__

#include <stdint.h>
#include "ble_ots.h"
#include "nrf_ble_gq.h"

/**@brief Function for initializing the Object Transfer OACP characteristic.
 *
 * @param[out]  p_ots_oacp      Object Transfer Service OACP structure. This structure will have
 *                              to be supplied by the application. It will be initialized by this function,
 *                              and will later be used to identify this particular instance.
 * @param[in]   p_ots_oacp_init Information needed to initialize the module.
 *
 * @return      NRF_SUCCESS on successful initialization, otherwise an error code.
 */
uint32_t ble_ots_oacp_init(ble_ots_oacp_t * p_ots_oacp, ble_ots_oacp_init_t * p_ots_oacp_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the OACP module.
 *
 * @param[in]  p_ots_oacp   Object Transfer Service OACP structure
 * @param[in]  p_ble_evt    Event received from the BLE stack.
 */
void ble_ots_oacp_on_ble_evt(ble_ots_oacp_t * p_ots_oacp, ble_evt_t const * p_ble_evt);

/**@brief Execute an Object Action Control Point procedure
 *
 * @param[in]  p_ots_oacp           Object Transfer Service OACP structure.
 * @param[in]  p_oacp_proc          Pointer to the procedure to be applied.
 *
 * @return BLE_OTS_WRITE_SUCCESS on success, otherwise an error.
 */
ble_ots_oacp_res_code_t ble_ots_oacp_do_proc(ble_ots_oacp_t      * p_ots_oacp,
                                             ble_ots_oacp_proc_t * p_oacp_proc);


#endif // BLE_OTS_OACP_H__

/** @} */ // End tag for the file.
