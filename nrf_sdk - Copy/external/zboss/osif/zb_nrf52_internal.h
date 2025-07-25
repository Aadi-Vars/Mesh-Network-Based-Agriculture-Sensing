/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
PURPOSE: Platform specific for NRF52 SoC.
*/

#ifndef ZB_NRF52_INTERNAL_H
#define ZB_NRF52_INTERNAL_H 1

#ifdef __GNUC__
/* Switch off warnings produced by Nordic headers */
#pragma GCC system_header
#endif

/* switch off warnings in Nordic headers */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "nrf.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_rng.h"
#include "nrf_assert.h"
#if defined SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "zboss_api.h"

void zb_nrf52_sched_sleep(zb_uint32_t sleep_tmo);
zb_uint32_t zb_nrf52_get_time_slept(void);

#if (defined(DEBUG_NRF) || defined(DEBUG_NRF_USER))
#define NRF_ERR_CHECK(err_code) ASSERT(err_code == NRF_SUCCESS)
#define NRF_ERR_CHECK_BOOL(err_code) ASSERT(err_code == true)
#else
#define NRF_ERR_CHECK(err_code) if (err_code != NRF_SUCCESS) {ZB_ABORT();}
#define NRF_ERR_CHECK_BOOL(err_code) if (err_code != true) {ZB_ABORT();}
#endif

#endif /* ZB_NRF52_INTERNAL_H */
