/**
 * Copyright (c) 2012 - 2022, Nordic Semiconductor ASA
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
/* Attention!
 * To maintain compliance with Nordic Semiconductor ASA's Bluetooth profile
 * qualification listings, this section of source code must not be modified.
 */
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(BLE_HTS)
#include "ble_err.h"
#include "ble_hts.h"
#include <string.h>
#include "ble_srv_common.h"


#define OPCODE_LENGTH 1                                                           /**< Length of opcode inside Health Thermometer Measurement packet. */
#define HANDLE_LENGTH 2                                                           /**< Length of handle inside Health Thermometer Measurement packet. */
#define MAX_HTM_LEN   (BLE_GATT_ATT_MTU_DEFAULT - OPCODE_LENGTH - HANDLE_LENGTH)  /**< Maximum size of a transmitted Health Thermometer Measurement. */

// Health Thermometer Measurement flag bits
#define HTS_MEAS_FLAG_TEMP_UNITS_BIT (0x01 << 0)  /**< Temperature Units flag. */
#define HTS_MEAS_FLAG_TIME_STAMP_BIT (0x01 << 1)  /**< Time Stamp flag. */
#define HTS_MEAS_FLAG_TEMP_TYPE_BIT  (0x01 << 2)  /**< Temperature Type flag. */


/**@brief Function for interception of GATT errors and @ref nrf_ble_gq errors.
 *
 * @param[in] nrf_error   Error code.
 * @param[in] p_ctx       Parameter from the event handler.
 * @param[in] conn_handle Connection handle.
 */
static void gatt_error_handler(uint32_t   nrf_error,
                               void     * p_ctx,
                               uint16_t   conn_handle)
{
    ble_hts_t * p_hts = (ble_hts_t *)p_ctx;

    if (p_hts->error_handler != NULL)
    {
        p_hts->error_handler(nrf_error);
    }
}


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_hts       Health Thermometer Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_hts_t * p_hts, ble_evt_t const * p_ble_evt)
{
    ret_code_t err_code;

    p_hts->conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;

    err_code = nrf_ble_gq_conn_handle_register(p_hts->p_gatt_queue, p_hts->conn_handle);

    if ((p_hts->error_handler != NULL) &&
        (err_code != NRF_SUCCESS))
    {
        p_hts->error_handler(err_code);
    }
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_hts       Health Thermometer Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_hts_t * p_hts, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_hts->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling write events to the Blood Pressure Measurement characteristic.
 *
 * @param[in]   p_hts         Health Thermometer Service structure.
 * @param[in]   p_evt_write   Write event received from the BLE stack.
 */
static void on_cccd_write(ble_hts_t * p_hts, ble_gatts_evt_write_t const * p_evt_write)
{
    if (p_evt_write->len == 2)
    {
        // CCCD written, update indication state
        if (p_hts->evt_handler != NULL)
        {
            ble_hts_evt_t evt;

            if (ble_srv_is_indication_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_HTS_EVT_INDICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_HTS_EVT_INDICATION_DISABLED;
            }

            p_hts->evt_handler(p_hts, &evt);
        }
    }
}


/**@brief Function for handling the Write event.
 *
 * @param[in]   p_hts       Health Thermometer Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_hts_t * p_hts, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_evt_write->handle == p_hts->meas_handles.cccd_handle)
    {
        on_cccd_write(p_hts, p_evt_write);
    }
}


/**@brief Function for handling the HVC event.
 *
 * @details Handles HVC events from the BLE stack.
 *
 * @param[in]   p_hts       Health Thermometer Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_hvc(ble_hts_t * p_hts, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_hvc_t const * p_hvc = &p_ble_evt->evt.gatts_evt.params.hvc;

    if (p_hvc->handle == p_hts->meas_handles.value_handle)
    {
        ble_hts_evt_t evt;

        evt.evt_type = BLE_HTS_EVT_INDICATION_CONFIRMED;
        p_hts->evt_handler(p_hts, &evt);
    }
}


void ble_hts_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_hts_t * p_hts = (ble_hts_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_hts, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_hts, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_hts, p_ble_evt);
            break;

        case BLE_GATTS_EVT_HVC:
            on_hvc(p_hts, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for encoding a Health Thermometer Measurement.
 *
 * @param[in]   p_hts              Health Thermometer Service structure.
 * @param[in]   p_hts_meas         Measurement to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t hts_measurement_encode(ble_hts_t      * p_hts,
                                      ble_hts_meas_t * p_hts_meas,
                                      uint8_t        * p_encoded_buffer)
{
    uint8_t  flags = 0;
    uint8_t  len   = 1;
    uint32_t encoded_temp;

    // Flags field
    if (p_hts_meas->temp_in_fahr_units)
    {
        flags |= HTS_MEAS_FLAG_TEMP_UNITS_BIT;
    }
    if (p_hts_meas->time_stamp_present)
    {
        flags |= HTS_MEAS_FLAG_TIME_STAMP_BIT;
    }

    // Temperature Measurement Value field
    if (p_hts_meas->temp_in_fahr_units)
    {
        flags |= HTS_MEAS_FLAG_TEMP_UNITS_BIT;

        encoded_temp = ((p_hts_meas->temp_in_fahr.exponent << 24) & 0xFF000000) |
                       ((p_hts_meas->temp_in_fahr.mantissa <<  0) & 0x00FFFFFF);
    }
    else
    {
        encoded_temp = ((p_hts_meas->temp_in_celcius.exponent << 24) & 0xFF000000) |
                       ((p_hts_meas->temp_in_celcius.mantissa <<  0) & 0x00FFFFFF);
    }
    len += uint32_encode(encoded_temp, &p_encoded_buffer[len]);

    // Time Stamp field
    if (p_hts_meas->time_stamp_present)
    {
        flags |= HTS_MEAS_FLAG_TIME_STAMP_BIT;
        len   += ble_date_time_encode(&p_hts_meas->time_stamp, &p_encoded_buffer[len]);
    }

    // Temperature Type field
    if (p_hts_meas->temp_type_present)
    {
        flags                  |= HTS_MEAS_FLAG_TEMP_TYPE_BIT;
        p_encoded_buffer[len++] = p_hts_meas->temp_type;
    }

    // Flags field
    p_encoded_buffer[0] = flags;

    return len;
}


uint32_t ble_hts_init(ble_hts_t * p_hts, ble_hts_init_t const * p_hts_init)
{
    VERIFY_PARAM_NOT_NULL(p_hts);
    VERIFY_PARAM_NOT_NULL(p_hts_init);
    VERIFY_PARAM_NOT_NULL(p_hts_init->p_gatt_queue);

    uint32_t              err_code;
    uint8_t               init_value[MAX_HTM_LEN];
    ble_hts_meas_t        initial_htm;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    // Initialize service structure
    p_hts->evt_handler   = p_hts_init->evt_handler;
    p_hts->p_gatt_queue  = p_hts_init->p_gatt_queue;
    p_hts->error_handler = p_hts_init->error_handler;
    p_hts->conn_handle   = BLE_CONN_HANDLE_INVALID;
    p_hts->temp_type     = p_hts_init->temp_type;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_HEALTH_THERMOMETER_SERVICE);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_hts->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add measurement characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));
    memset(&initial_htm, 0, sizeof(initial_htm));

    add_char_params.uuid                = BLE_UUID_TEMPERATURE_MEASUREMENT_CHAR;
    add_char_params.init_len            = hts_measurement_encode(p_hts, &initial_htm, init_value);
    add_char_params.max_len             = MAX_HTM_LEN;
    add_char_params.p_init_value        = init_value;
    add_char_params.is_var_len          = true;
    add_char_params.char_props.indicate = 1;
    add_char_params.cccd_write_access   = p_hts_init->ht_meas_cccd_wr_sec;

    err_code = characteristic_add(p_hts->service_handle, &add_char_params, &p_hts->meas_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add temperature type characteristic
    if (p_hts_init->temp_type_as_characteristic)
    {
        memset(&add_char_params, 0, sizeof(add_char_params));

        add_char_params.uuid            = BLE_UUID_TEMPERATURE_TYPE_CHAR;
        add_char_params.max_len         = sizeof(uint8_t);
        add_char_params.char_props.read = 1;
        add_char_params.read_access     = p_hts_init->ht_type_rd_sec;
        add_char_params.init_len        = sizeof(uint8_t);
        add_char_params.p_init_value    = (uint8_t *) &(p_hts_init->temp_type);

        err_code = characteristic_add(p_hts->service_handle,
                                      &add_char_params,
                                      &p_hts->temp_type_handles);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    return NRF_SUCCESS;
}


uint32_t ble_hts_measurement_send(ble_hts_t * p_hts, ble_hts_meas_t * p_hts_meas)
{
    uint32_t err_code;

    // Send value if connected
    if (p_hts->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint8_t          encoded_hts_meas[MAX_HTM_LEN];
        uint16_t         len;
        nrf_ble_gq_req_t hts_req;

        len     = hts_measurement_encode(p_hts, p_hts_meas, encoded_hts_meas);

        memset(&hts_req, 0, sizeof(nrf_ble_gq_req_t));

        hts_req.type                               = NRF_BLE_GQ_REQ_GATTS_HVX;
        hts_req.error_handler.cb                   = gatt_error_handler;
        hts_req.error_handler.p_ctx                = p_hts;
        hts_req.params.gatts_hvx.handle  = p_hts->meas_handles.value_handle;
        hts_req.params.gatts_hvx.offset  = 0;
        hts_req.params.gatts_hvx.p_data  = encoded_hts_meas;
        hts_req.params.gatts_hvx.p_len   = &len;
        hts_req.params.gatts_hvx.type    = BLE_GATT_HVX_INDICATION;

        err_code = nrf_ble_gq_item_add(p_hts->p_gatt_queue, &hts_req, p_hts->conn_handle);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}


uint32_t ble_hts_is_indication_enabled(ble_hts_t * p_hts, bool * p_indication_enabled)
{
    uint32_t err_code;
    uint8_t  cccd_value_buf[BLE_CCCD_VALUE_LEN];
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = BLE_CCCD_VALUE_LEN;
    gatts_value.offset  = 0;
    gatts_value.p_value = cccd_value_buf;

    err_code = sd_ble_gatts_value_get(p_hts->conn_handle,
                                      p_hts->meas_handles.cccd_handle,
                                      &gatts_value);
    if (err_code == NRF_SUCCESS)
    {
        *p_indication_enabled = ble_srv_is_indication_enabled(cccd_value_buf);
    }
    if (err_code == BLE_ERROR_GATTS_SYS_ATTR_MISSING)
    {
        *p_indication_enabled = false;
        return NRF_SUCCESS;
    }
    return err_code;
}
#endif // NRF_MODULE_ENABLED(BLE_HTS)
