/**
 * Copyright (c) 2018 - 2022, Nordic Semiconductor ASA
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

#include "sdk_common.h"
#include "sdk_config.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include "nrf_crypto_init.h"
#include "nrf_crypto_rng.h"
#include "nrf_log.h"

/*lint -save -e????*/
#include "optiga/optiga_util.h"
#include "optiga/ifx_i2c/ifx_i2c.h"
/*lint -restore*/

optiga_comms_t optiga_comms = {(void*)&ifx_i2c_context_0, NULL, NULL};

// need to forward declare these, because they are not exported through the PAL API
void pal_gpio_init(void);
void pal_os_event_init(void);

/*************************************************************************
*  functions
*************************************************************************/

static int32_t optiga_init(void)
{
    int32_t status = (int32_t) OPTIGA_LIB_ERROR;

    // Initialize PAL
    pal_gpio_init();
    pal_os_event_init();

    status = optiga_util_open_application(&optiga_comms);
    if (OPTIGA_LIB_SUCCESS != status)
    {
        NRF_LOG_INFO("Failure: CmdLib_OpenApplication(): 0x%04X", status);
        return status;
    }

    NRF_LOG_INFO("Success: CmdLib_OpenApplication(): 0x%04X", status);

    return OPTIGA_LIB_SUCCESS;
}

/** @internal @brief Function to initialize OPTIGA backend - open the application.
 */
static ret_code_t optiga_backend_init(void)
{
    if(optiga_init() != OPTIGA_LIB_SUCCESS)
    {
        return NRF_ERROR_INTERNAL;
    }

#if defined(NRF_CRYPTO_RNG_AUTO_INIT_ENABLED) && (NRF_CRYPTO_RNG_AUTO_INIT_ENABLED == 1)

    ret_code_t ret_val;
    ret_val = nrf_crypto_rng_init(NULL, NULL);
    return ret_val;

#elif defined(NRF_CRYPTO_RNG_AUTO_INIT_ENABLED) && (NRF_CRYPTO_RNG_AUTO_INIT_ENABLED == 0)

    return NRF_SUCCESS;

#else

    #warning NRF_CRYPTO_RNG_AUTO_INIT_ENABLED define not found in sdk_config.h (Is the sdk_config.h valid?).

#endif // NRF_CRYPTO_RNG_AUTO_INIT_ENABLED

}


/** @internal @brief Function to uninitialize OPTIGA backend - currently no implementation is required.
 */
static ret_code_t optiga_backend_uninit(void)
{
    // Empty implementation
    return NRF_SUCCESS;
}


CRYPTO_BACKEND_REGISTER(nrf_crypto_backend_info_t const optiga_backend) =
{
    .init_fn    = optiga_backend_init,
    .uninit_fn  = optiga_backend_uninit,
};

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)
