/**
 * Copyright (c) 2014 - 2022, Nordic Semiconductor ASA
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
#ifndef SER_CONFIG_H__
#define SER_CONFIG_H__

#include <stdint.h>

#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************//**
 * General parameters configuration.
 **************************************************************************************************/

/** Value used as error code on SoftDevice stack dump. Can be used to identify stack location on
 *  stack unwind.*/
#define SER_SD_ERROR_CODE    (0xDEADBEEFUL)

/** Value used as error code indicating warning - unusual situation but not critical so system
 *  should NOT be reset. */
#define SER_WARNING_CODE     (0xBADDCAFEUL)

/***********************************************************************************************//**
 * HAL Transport layer configuration.
 **************************************************************************************************/

/** Max packets size in serialization HAL Transport layer (packets before adding PHY header i.e.
 *  packet length). */
#ifdef S112
#define SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE    (128UL)
#define SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE    (128UL)
#else
#define SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE    (512UL)
#define SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE    (512UL)
#endif

#define SER_HAL_TRANSPORT_MAX_PKT_SIZE ((SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE) >= \
                                        (SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE)    \
                                        ?                                               \
                                        (SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE) :  \
                                        (SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE))
#ifdef SER_CONNECTIVITY
    #define SER_HAL_TRANSPORT_TX_MAX_PKT_SIZE         SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE
    #define SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE         SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE

#else /* APPLICATION SIDE */
    #define SER_HAL_TRANSPORT_TX_MAX_PKT_SIZE         SER_HAL_TRANSPORT_APP_TO_CONN_MAX_PKT_SIZE
    #define SER_HAL_TRANSPORT_RX_MAX_PKT_SIZE         SER_HAL_TRANSPORT_CONN_TO_APP_MAX_PKT_SIZE
#endif /* SER_CONNECTIVITY */


/***********************************************************************************************//**
 * SER_PHY layer configuration.
 **************************************************************************************************/

#define SER_PHY_HEADER_SIZE             2

#define SER_PHY_HCI_SLIP_TX_BUF_SIZE    30

#define SER_PHY_SPI_FREQUENCY           NRF_DRV_SPI_FREQ_1M

/** Max transfer unit for SPI MASTER and SPI SLAVE. */
#define SER_PHY_SPI_MTU_SIZE            255

/** UART transmission parameters */
#define SER_PHY_UART_FLOW_CTRL          NRF_UART_HWFC_ENABLED
#define SER_PHY_UART_PARITY             NRF_UART_PARITY_INCLUDED
#define SER_PHY_UART_BAUDRATE_VAL       1000000

#define SER_PHY_UART_BAUDRATE CONCAT_2(NRF_UART_BAUDRATE_,SER_PHY_UART_BAUDRATE_VAL)

/** Configuration timeouts of connectivity MCU. */
#define CONN_CHIP_RESET_TIME            50      /**< Time to keep the reset line to the connectivity chip low (in milliseconds). */
#define CONN_CHIP_WAKEUP_TIME           500     /**< Time for the connectivity chip to reset and become ready to receive serialized commands (in milliseconds). */

#ifdef S112
#define SER_MAX_CONNECTIONS 2
#else
#define SER_MAX_CONNECTIONS 8
#endif

#ifndef SER_MAX_ADV_DATA
#define SER_MAX_ADV_DATA 256
#endif
 
#ifdef __cplusplus
}
#endif

#endif /* SER_CONFIG_H__ */
