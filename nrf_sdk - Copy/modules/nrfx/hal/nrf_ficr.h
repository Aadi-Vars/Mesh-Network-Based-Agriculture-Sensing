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

#ifndef NRF_FICR_H__
#define NRF_FICR_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_ficr_hal FICR HAL
 * @{
 * @ingroup nrf_ficr
 * @brief   Hardware access layer (HAL) for getting data from
 *          the Factory Information Configuration Registers (FICR).
 */

/**
 * @brief Function for getting the size of the code memory page.
 *
 * @param[in] p_reg Pointer to the structure of registers of the peripheral.
 *
 * @return Code memory page size in bytes.
 */
__STATIC_INLINE uint32_t nrf_ficr_codepagesize_get(NRF_FICR_Type const * p_reg);

/**
 * @brief Function for getting the size of the code memory rendered as number of pages.
 *
 * @param[in] p_reg Pointer to the structure of registers of the peripheral.
 *
 * @return Code memory size rendered as number of pages.
 */
__STATIC_INLINE uint32_t nrf_ficr_codesize_get(NRF_FICR_Type const * p_reg);

/**
 * @brief Function for getting the unique device identifier.
 *
 * @param[in] p_reg  Pointer to the structure of registers of the peripheral.
 * @param[in] reg_id Register index.
 *
 * @return Unique device identifier.
 */
__STATIC_INLINE uint32_t nrf_ficr_deviceid_get(NRF_FICR_Type const * p_reg, uint32_t reg_id);

#if defined(FICR_NFC_TAGHEADER0_MFGID_Msk) || defined(__NRFX_DOXYGEN__)
/**
 * @brief Function for getting the default header values for the NFC tag.
 *
 * @param[in] p_reg        Pointer to the structure of registers of the peripheral.
 * @param[in] tagheader_id Tag header index.
 *
 * @return The default header value of the NFC tag for the specified header index.
 */
__STATIC_INLINE uint32_t nrf_ficr_nfc_tagheader_get(NRF_FICR_Type const * p_reg,
                                                    uint32_t              tagheader_id);
#endif // defined(FICR_NFC_TAGHEADER0_MFGID_Msk) || defined(__NRFX_DOXYGEN__)

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE uint32_t nrf_ficr_codepagesize_get(NRF_FICR_Type const * p_reg)
{
#if defined(FICR_INFO_CODEPAGESIZE_CODEPAGESIZE_Msk)
    return p_reg->INFO.CODEPAGESIZE;
#else
    return p_reg->CODEPAGESIZE;
#endif
}

__STATIC_INLINE uint32_t nrf_ficr_codesize_get(NRF_FICR_Type const * p_reg)
{
#if defined(FICR_INFO_CODESIZE_CODESIZE_Msk)
    return p_reg->INFO.CODESIZE;
#else
    return p_reg->CODESIZE;
#endif
}

__STATIC_INLINE uint32_t nrf_ficr_deviceid_get(NRF_FICR_Type const * p_reg, uint32_t reg_id)
{
#if defined(FICR_INFO_DEVICEID_DEVICEID_Msk)
    return p_reg->INFO.DEVICEID[reg_id];
#else
    return p_reg->DEVICEID[reg_id];
#endif
}

#if defined(FICR_NFC_TAGHEADER0_MFGID_Msk)
__STATIC_INLINE uint32_t nrf_ficr_nfc_tagheader_get(NRF_FICR_Type const * p_reg,
                                                    uint32_t              tagheader_id)
{
    switch(tagheader_id) {
        case 0:
            return p_reg->NFC.TAGHEADER0;
        case 1:
            return p_reg->NFC.TAGHEADER1;
        case 2:
            return p_reg->NFC.TAGHEADER2;
        case 3:
            return p_reg->NFC.TAGHEADER3;
        default:
            return 0;
    }
}
#endif // defined(FICR_NFC_TAGHEADER0_MFGID_Msk)

#endif // SUPPRESS_INLINE_IMPLEMENTATION

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_FICR_H__
