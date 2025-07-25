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
#ifndef ZIGBEE_HELPERS_H__
#define ZIGBEE_HELPERS_H__

#include <stdint.h>
#include <stdbool.h>

#include "zboss_api.h"

/**
 *  @addtogroup special_nordic_functions
 *  @{
 */

/**@brief Name of the log module related to Zigbee. */
#define ZIGBEE_NRF_LOG_MODULE_NAME  zigbee

/**@brief Function for setting the Erase persistent storage, depending on the erase pin.
 * If the erase pin (1.39 by default, defined in zigbee_helpers.c) is shortened to
 * the ground, then the persistent storage is erased. Otherwise, whether the storage
 * is erased is decided upon the input parameter 'erase'. This behaviour is only valid
 * if PCA10056 is used.
 * 
 * @param[in] erase  Whether to erase the persistent storage in case the erase pin is
 *                   not shortened to the ground.
 */
void zigbee_erase_persistent_storage(zb_bool_t erase);

/**@brief Function for converting an input buffer to a hex string.
 *
 * @param[out] p_out     Pointer to the output buffer.
 * @param[in]  out_size  Size of the output buffer.
 * @param[in]  p_in      Pointer to the input buffer.
 * @param[in]  in_size   Size of the input buffer.
 * @param[in]  reverse   If true, data output happens in the reverse order.
 *
 * @return     snprintf-compatible value. Less than zero means encoding error.
 *             Non-negative value is the number of characters that would have been written
 *             if the supplied buffer had been large enough. Value greater than or equal to
 *             buf_len means that the supplied buffer was too small.
 *
 * @note    Null terminator is written if buf_len is large enough, but does not
 *          count for the return value.
 */
int to_hex_str(char * p_out, uint16_t out_size,
                  const uint8_t * p_in, uint8_t in_size,
                  bool reverse);

/**@brief Read array of uint8_t from hex string.
 *
 * @param p_in_str      Pointer to the input hex string.
 * @param in_str_len    Length, in characters, of the input string.
 * @param p_out_buff    Pointer to the output uint8_t array.
 * @param out_buff_size Size, in bytes, of the output uint8_t array.
 * @param reverse       If true then parse from end to start.
 *
 * @retval true   if the conversion succeed
 * @retval false  if the conversion failed
 */
bool parse_hex_str(char const * p_in_str, uint8_t in_str_len,
                   uint8_t * p_out_buff, uint8_t out_buff_size,
                   bool reverse);

/**@brief Parse a hex string to uint8_t.
 *
 * The function verifies if input is valid, i.e., if all input characters
 * are valid hex digits. If an invalid character is found then function fails.
 *
 * @param s        Pointer to input string.
 * @param p_value  Pointer to output value.
 *
 * @retval true   if the conversion succeed
 * @retval false  if the conversion failed
 */
static inline bool parse_hex_u8(char const * s, uint8_t * p_value)
{
    return parse_hex_str(s, strlen(s), p_value, sizeof(*p_value), true);
}

/**@brief Parse a hex string to uint16_t.
 *
 * The function verifies if input is valid, i.e., if all input characters
 * are valid hex digits. If an invalid character is found then function fails.
 *
 * @param s        Pointer to input string.
 * @param p_value  Pointer to output value.
 *
 * @retval true   if the conversion succeed
 * @retval false  if the conversion failed
 */
static inline bool parse_hex_u16(char const * s, uint16_t * p_value)
{
    return parse_hex_str(s, strlen(s), (uint8_t *)p_value, sizeof(*p_value), true);
}

/**@brief Function for converting 64-bit address to hex string.
 *
 * @param[out] p_str_buf    Pointer to output buffer.
 * @param[in]  buf_len      Length of the buffer pointed by p_str_buf.
 * @param[in]  p_in         Zigbee IEEE address to be converted to string.
 *
 * @return     snprintf-compatible value. Less than zero means encoding error.
 *             Non-negative value is the number of characters that would have been written
 *             if the supplied buffer had been large enough. Value greater than or equal to
 *             buf_len means that the supplied buffer was too small.
 *
 * @note    Null terminator is written if buf_len is large enough, but does not
 *          count for the return value.
 */
int ieee_addr_to_str(char * p_str_buf, uint16_t buf_len, const zb_ieee_addr_t p_in);

/**@brief Address type.
 *
 * @note  ADDR_SHORT and ADDR_LONG correspond to APS addressing mode constants
 *        level_control_client_attr_listand must not be changed.
 */
typedef enum
{
    ADDR_INVALID = 0,
    ADDR_ANY     = 1,
    ADDR_SHORT   = 2, // ZB_APS_ADDR_MODE_16_ENDP_PRESENT
    ADDR_LONG    = 3, // ZB_APS_ADDR_MODE_64_ENDP_PRESENT
} addr_type_t;

/**@brief Function for parsing a null-terminated string of hex characters
 *        into 64-bit or 16-bit address.
 *
 * The function will skip 0x suffix from input if present.
 *
 * @param input     Pointer to the input string string representing the address in big endian.
 * @param output    Pointer to the resulting zb_addr_u variable.
 * @param addr_type Expected address type.
 *
 * @return Conversion result.
 */
addr_type_t parse_address(const char * input, zb_addr_u * output, addr_type_t addr_type);

/**@brief Function for parsing a null-terminated string of hex characters
 *        into a 64-bit address.
 *
 * The function will skip 0x suffix from input if present.
 *
 * @param input  Pointer to the input string representing the address in big endian.
 * @param addr   Variable where the address will be placed.
 *
 * @retval true   if the conversion succeed
 * @retval false  if the conversion failed
 */
static inline bool parse_long_address(const char * input, zb_ieee_addr_t addr)
{
    return (parse_address(input, (zb_addr_u *)addr, ADDR_LONG) != ADDR_INVALID);
}

/**@brief Function for parsing a null-terminated string of hex characters
 *        into 16-bit address.
 *
 * The function will skip 0x suffix from input if present.
 *
 * @param input  Pointer to the input string representing the address in big endian.
 * @param addr   Pointer to the a variable where address will be placed.
 *
 * @retval true   if the conversion succeed
 * @retval false  if the conversion failed
 */
static inline bool parse_short_address(const char * input, zb_uint16_t * addr)
{
    return (parse_address(input, (zb_addr_u *)addr, ADDR_SHORT) != ADDR_INVALID);
}

/**@brief Function for passing signals to the default Zigbee stack event handler.
 *
 * @note This function does not free the Zigbee buffer.
 *
 * @param[in] bufid Reference to the Zigbee stack buffer used to pass signal.
 *
 * @return RET_OK on success or error code on failure.
 */
zb_ret_t zigbee_default_signal_handler(zb_bufid_t bufid);

/**@brief Function for indicating the Zigbee network connection status on LED.
 *        If the device is successfully commissioned, the LED is turned on.
 *        If the device is not commissioned or has left the network, the LED is turned off.
 *
 * @note This function does not free the Zigbee buffer.
 *
 * @param[in] bufid   Reference to the Zigbee stack buffer used to pass signal.
 * @param[in] led_idx LED index, as defined in the board-specific BSP header. The index starts from 0.
 */
void zigbee_led_status_update(zb_bufid_t bufid, uint32_t led_idx);

/**@brief Function for indicating the default signal handler about user input on the device.
 *        If the device is not commissioned, the rejoin procedure is started.
 *
 * @note This function is to be used with End Devices only.
 *
 */
#if defined ZB_ED_ROLE
void user_input_indicate(void);
#endif

/**@brief Function for powering down unused RAM. Powers down all unused sections in RAM banks.
 */
void zigbee_power_down_unused_ram(void);

/** @} */ /* End of group special_nordic_functions */

#endif /* ZIGBEE_HELPERS_H__ */
