/**
 * Copyright (c) 2015 - 2022, Nordic Semiconductor ASA
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

#ifndef NRF_EGU_H__
#define NRF_EGU_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @defgroup nrf_egu_hal EGU HAL
* @{
* @ingroup nrf_swi_egu
* @brief   Hardware access layer for managing the Event Generator Unit (EGU) peripheral.
*/

/** @brief EGU tasks. */
typedef enum
{
    NRF_EGU_TASK_TRIGGER0  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[0]),  /**< Trigger 0 for triggering the corresponding TRIGGERED[0] event. */
    NRF_EGU_TASK_TRIGGER1  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[1]),  /**< Trigger 1 for triggering the corresponding TRIGGERED[1] event. */
    NRF_EGU_TASK_TRIGGER2  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[2]),  /**< Trigger 2 for triggering the corresponding TRIGGERED[2] event. */
    NRF_EGU_TASK_TRIGGER3  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[3]),  /**< Trigger 3 for triggering the corresponding TRIGGERED[3] event. */
    NRF_EGU_TASK_TRIGGER4  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[4]),  /**< Trigger 4 for triggering the corresponding TRIGGERED[4] event. */
    NRF_EGU_TASK_TRIGGER5  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[5]),  /**< Trigger 5 for triggering the corresponding TRIGGERED[5] event. */
    NRF_EGU_TASK_TRIGGER6  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[6]),  /**< Trigger 6 for triggering the corresponding TRIGGERED[6] event. */
    NRF_EGU_TASK_TRIGGER7  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[7]),  /**< Trigger 7 for triggering the corresponding TRIGGERED[7] event. */
    NRF_EGU_TASK_TRIGGER8  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[8]),  /**< Trigger 8 for triggering the corresponding TRIGGERED[8] event. */
    NRF_EGU_TASK_TRIGGER9  = offsetof(NRF_EGU_Type, TASKS_TRIGGER[9]),  /**< Trigger 9 for triggering the corresponding TRIGGERED[9] event. */
    NRF_EGU_TASK_TRIGGER10 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[10]), /**< Trigger 10 for triggering the corresponding TRIGGERED[10] event. */
    NRF_EGU_TASK_TRIGGER11 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[11]), /**< Trigger 11 for triggering the corresponding TRIGGERED[11] event. */
    NRF_EGU_TASK_TRIGGER12 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[12]), /**< Trigger 12 for triggering the corresponding TRIGGERED[12] event. */
    NRF_EGU_TASK_TRIGGER13 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[13]), /**< Trigger 13 for triggering the corresponding TRIGGERED[13] event. */
    NRF_EGU_TASK_TRIGGER14 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[14]), /**< Trigger 14 for triggering the corresponding TRIGGERED[14] event. */
    NRF_EGU_TASK_TRIGGER15 = offsetof(NRF_EGU_Type, TASKS_TRIGGER[15])  /**< Trigger 15 for triggering the corresponding TRIGGERED[15] event. */
} nrf_egu_task_t;

/** @brief EGU events. */
typedef enum
{
    NRF_EGU_EVENT_TRIGGERED0  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[0]),  /**< Event number 0 generated by triggering the corresponding TRIGGER[0] task. */
    NRF_EGU_EVENT_TRIGGERED1  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[1]),  /**< Event number 1 generated by triggering the corresponding TRIGGER[1] task. */
    NRF_EGU_EVENT_TRIGGERED2  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[2]),  /**< Event number 2 generated by triggering the corresponding TRIGGER[2] task. */
    NRF_EGU_EVENT_TRIGGERED3  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[3]),  /**< Event number 3 generated by triggering the corresponding TRIGGER[3] task. */
    NRF_EGU_EVENT_TRIGGERED4  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[4]),  /**< Event number 4 generated by triggering the corresponding TRIGGER[4] task. */
    NRF_EGU_EVENT_TRIGGERED5  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[5]),  /**< Event number 5 generated by triggering the corresponding TRIGGER[5] task. */
    NRF_EGU_EVENT_TRIGGERED6  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[6]),  /**< Event number 6 generated by triggering the corresponding TRIGGER[6] task. */
    NRF_EGU_EVENT_TRIGGERED7  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[7]),  /**< Event number 7 generated by triggering the corresponding TRIGGER[7] task. */
    NRF_EGU_EVENT_TRIGGERED8  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[8]),  /**< Event number 8 generated by triggering the corresponding TRIGGER[8] task. */
    NRF_EGU_EVENT_TRIGGERED9  = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[9]),  /**< Event number 9 generated by triggering the corresponding TRIGGER[9] task. */
    NRF_EGU_EVENT_TRIGGERED10 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[10]), /**< Event number 10 generated by triggering the corresponding TRIGGER[10] task. */
    NRF_EGU_EVENT_TRIGGERED11 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[11]), /**< Event number 11 generated by triggering the corresponding TRIGGER[11] task. */
    NRF_EGU_EVENT_TRIGGERED12 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[12]), /**< Event number 12 generated by triggering the corresponding TRIGGER[12] task. */
    NRF_EGU_EVENT_TRIGGERED13 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[13]), /**< Event number 13 generated by triggering the corresponding TRIGGER[13] task. */
    NRF_EGU_EVENT_TRIGGERED14 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[14]), /**< Event number 14 generated by triggering the corresponding TRIGGER[14] task. */
    NRF_EGU_EVENT_TRIGGERED15 = offsetof(NRF_EGU_Type, EVENTS_TRIGGERED[15])  /**< Event number 15 generated by triggering the corresponding TRIGGER[15] task. */
} nrf_egu_event_t;

/** @brief EGU interrupts. */
typedef enum
{
    NRF_EGU_INT_TRIGGERED0  = EGU_INTENSET_TRIGGERED0_Msk,  /**< Interrupt on EVENTS_TRIGGERED[0] event. */
    NRF_EGU_INT_TRIGGERED1  = EGU_INTENSET_TRIGGERED1_Msk,  /**< Interrupt on EVENTS_TRIGGERED[1] event. */
    NRF_EGU_INT_TRIGGERED2  = EGU_INTENSET_TRIGGERED2_Msk,  /**< Interrupt on EVENTS_TRIGGERED[2] event. */
    NRF_EGU_INT_TRIGGERED3  = EGU_INTENSET_TRIGGERED3_Msk,  /**< Interrupt on EVENTS_TRIGGERED[3] event. */
    NRF_EGU_INT_TRIGGERED4  = EGU_INTENSET_TRIGGERED4_Msk,  /**< Interrupt on EVENTS_TRIGGERED[4] event. */
    NRF_EGU_INT_TRIGGERED5  = EGU_INTENSET_TRIGGERED5_Msk,  /**< Interrupt on EVENTS_TRIGGERED[5] event. */
    NRF_EGU_INT_TRIGGERED6  = EGU_INTENSET_TRIGGERED6_Msk,  /**< Interrupt on EVENTS_TRIGGERED[6] event. */
    NRF_EGU_INT_TRIGGERED7  = EGU_INTENSET_TRIGGERED7_Msk,  /**< Interrupt on EVENTS_TRIGGERED[7] event. */
    NRF_EGU_INT_TRIGGERED8  = EGU_INTENSET_TRIGGERED8_Msk,  /**< Interrupt on EVENTS_TRIGGERED[8] event. */
    NRF_EGU_INT_TRIGGERED9  = EGU_INTENSET_TRIGGERED9_Msk,  /**< Interrupt on EVENTS_TRIGGERED[9] event. */
    NRF_EGU_INT_TRIGGERED10 = EGU_INTENSET_TRIGGERED10_Msk, /**< Interrupt on EVENTS_TRIGGERED[10] event. */
    NRF_EGU_INT_TRIGGERED11 = EGU_INTENSET_TRIGGERED11_Msk, /**< Interrupt on EVENTS_TRIGGERED[11] event. */
    NRF_EGU_INT_TRIGGERED12 = EGU_INTENSET_TRIGGERED12_Msk, /**< Interrupt on EVENTS_TRIGGERED[12] event. */
    NRF_EGU_INT_TRIGGERED13 = EGU_INTENSET_TRIGGERED13_Msk, /**< Interrupt on EVENTS_TRIGGERED[13] event. */
    NRF_EGU_INT_TRIGGERED14 = EGU_INTENSET_TRIGGERED14_Msk, /**< Interrupt on EVENTS_TRIGGERED[14] event. */
    NRF_EGU_INT_TRIGGERED15 = EGU_INTENSET_TRIGGERED15_Msk, /**< Interrupt on EVENTS_TRIGGERED[15] event. */
    NRF_EGU_INT_ALL         = 0xFFFFuL
} nrf_egu_int_mask_t;


/**
 * @brief Function for getting the maximum channel number of the given EGU.
 *
 * @param NRF_EGUx EGU instance.
 *
 * @return Number of available channels.
 */
__STATIC_INLINE uint32_t nrf_egu_channel_count(NRF_EGU_Type * NRF_EGUx);

/**
 * @brief Function for triggering the specified EGU task.
 *
 * @param NRF_EGUx EGU instance.
 * @param egu_task EGU task.
 */
__STATIC_INLINE void nrf_egu_task_trigger(NRF_EGU_Type * NRF_EGUx, nrf_egu_task_t egu_task);

/**
 * @brief Function for returning the address of the specified EGU task register.
 *
 * @param NRF_EGUx EGU instance.
 * @param egu_task EGU task.
 *
 * @return Address of the specified EGU task register.
 */
__STATIC_INLINE uint32_t * nrf_egu_task_address_get(NRF_EGU_Type * NRF_EGUx,
                                                    nrf_egu_task_t egu_task);

/**
 * @brief Function for returning the address of the specified EGU TRIGGER task register.
 *
 * @param NRF_EGUx EGU instance.
 * @param channel  Channel number.
 *
 * @return Address of the specified EGU TRIGGER task register.
 */
__STATIC_INLINE uint32_t * nrf_egu_task_trigger_address_get(NRF_EGU_Type * NRF_EGUx,
                                                            uint8_t        channel);

/**
 * @brief Function for returning the specified EGU TRIGGER task.
 *
 * @param NRF_EGUx EGU instance.
 * @param channel  Channel number.
 *
 * @return The specified EGU TRIGGER task.
 */
__STATIC_INLINE nrf_egu_task_t nrf_egu_task_trigger_get(NRF_EGU_Type * NRF_EGUx, uint8_t channel);

/**
 * @brief Function for retrieving the state of the UARTE event.
 *
 * @param NRF_EGUx  EGU instance.
 * @param egu_event EGU event to be checked.
 *
 * @retval true  The event has been generated.
 * @retval false The event has not been generated.
 */
__STATIC_INLINE bool nrf_egu_event_check(NRF_EGU_Type *  NRF_EGUx,
                                         nrf_egu_event_t egu_event);

/**
 * @brief Function for clearing the specified EGU event.
 *
 * @param NRF_EGUx  EGU instance.
 * @param egu_event EGU event to clear.
 */
__STATIC_INLINE void nrf_egu_event_clear(NRF_EGU_Type *  NRF_EGUx,
                                         nrf_egu_event_t egu_event);

/**
 * @brief Function for returning the address of the specified EGU event register.
 *
 * @param NRF_EGUx  EGU instance.
 * @param egu_event EGU event.
 *
 * @return Address of the specified EGU event register.
 */
__STATIC_INLINE uint32_t * nrf_egu_event_address_get(NRF_EGU_Type *  NRF_EGUx,
                                                     nrf_egu_event_t egu_event);

/**
 * @brief Function for returning address of the specified EGU TRIGGERED event register.
 *
 * @param NRF_EGUx EGU instance.
 * @param channel  Channel number.
 *
 * @return Address of the specified EGU TRIGGERED event register.
 */
__STATIC_INLINE uint32_t * nrf_egu_event_triggered_address_get(NRF_EGU_Type * NRF_EGUx,
                                                               uint8_t        channel);

/**
 * @brief Function for returning the specified EGU TRIGGERED event.
 *
 * @param NRF_EGUx EGU instance.
 * @param channel  Channel number.
 *
 * @return The specified EGU TRIGGERED event.
 */
__STATIC_INLINE nrf_egu_event_t nrf_egu_event_triggered_get(NRF_EGU_Type * NRF_EGUx,
                                                            uint8_t        channel);

/**
 * @brief Function for enabling one or more of the EGU interrupts.
 *
 * @param NRF_EGUx EGU instance.
 * @param mask     Mask of interrupts to be enabled.
 */
__STATIC_INLINE void nrf_egu_int_enable(NRF_EGU_Type * NRF_EGUx, uint32_t mask);

/**
 * @brief Function for retrieving the state of one or more of the EGU interrupts.
 *
 * @param NRF_EGUx EGU instance.
 * @param mask     Mask of interrupts to be checked.
 *
 * @retval true  All of the specified interrupts are enabled.
 * @retval false At least one of the specified interrupts is disabled.
 */
__STATIC_INLINE bool nrf_egu_int_enable_check(NRF_EGU_Type * NRF_EGUx, uint32_t mask);

/**
 * @brief Function for disabling one or more of the EGU interrupts.
 *
 * @param NRF_EGUx EGU instance.
 * @param mask     Mask of interrupts to be disabled.
 */
__STATIC_INLINE void nrf_egu_int_disable(NRF_EGU_Type * NRF_EGUx, uint32_t mask);

/**
 * @brief Function for retrieving one or more of the EGU interrupts.
 *
 * @param NRF_EGUx EGU instance.
 * @param channel  Channel number.
 *
 * @return EGU interrupt mask.
 */
__STATIC_INLINE nrf_egu_int_mask_t nrf_egu_int_get(NRF_EGU_Type * NRF_EGUx, uint8_t channel);

#if defined(DPPI_PRESENT) || defined(__NRFX_DOXYGEN__)
/**
 * @brief Function for setting the subscribe configuration for a given
 *        EGU task.
 *
 * @param[in] p_reg   Pointer to the structure of registers of the peripheral.
 * @param[in] task    Task for which to set the configuration.
 * @param[in] channel Channel through which to subscribe events.
 */
__STATIC_INLINE void nrf_egu_subscribe_set(NRF_EGU_Type * p_reg,
                                           nrf_egu_task_t task,
                                           uint8_t        channel);

/**
 * @brief Function for clearing the subscribe configuration for a given
 *        EGU task.
 *
 * @param[in] p_reg Pointer to the structure of registers of the peripheral.
 * @param[in] task  Task for which to clear the configuration.
 */
__STATIC_INLINE void nrf_egu_subscribe_clear(NRF_EGU_Type * p_reg,
                                             nrf_egu_task_t task);

/**
 * @brief Function for setting the publish configuration for a given
 *        EGU event.
 *
 * @param[in] p_reg   Pointer to the structure of registers of the peripheral.
 * @param[in] event   Event for which to set the configuration.
 * @param[in] channel Channel through which to publish the event.
 */
__STATIC_INLINE void nrf_egu_publish_set(NRF_EGU_Type *  p_reg,
                                         nrf_egu_event_t event,
                                         uint8_t         channel);

/**
 * @brief Function for clearing the publish configuration for a given
 *        EGU event.
 *
 * @param[in] p_reg Pointer to the structure of registers of the peripheral.
 * @param[in] event Event for which to clear the configuration.
 */
__STATIC_INLINE void nrf_egu_publish_clear(NRF_EGU_Type *  p_reg,
                                           nrf_egu_event_t event);
#endif // defined(DPPI_PRESENT) || defined(__NRFX_DOXYGEN__)

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE uint32_t nrf_egu_channel_count(NRF_EGU_Type * NRF_EGUx)
{
    if (NRF_EGUx == NRF_EGU0){
        return EGU0_CH_NUM;
    }
    if (NRF_EGUx ==  NRF_EGU1){
        return EGU1_CH_NUM;
    }
#if EGU_COUNT > 2
    if (NRF_EGUx ==  NRF_EGU2){
        return EGU2_CH_NUM;
    }
    if (NRF_EGUx ==  NRF_EGU3){
        return EGU3_CH_NUM;
    }
    if (NRF_EGUx ==  NRF_EGU4){
        return EGU4_CH_NUM;
    }
    if (NRF_EGUx ==  NRF_EGU5){
        return EGU5_CH_NUM;
    }
#endif
    return 0;
}

__STATIC_INLINE void nrf_egu_task_trigger(NRF_EGU_Type * NRF_EGUx, nrf_egu_task_t egu_task)
{
    NRFX_ASSERT(NRF_EGUx);
    *((volatile uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_task)) = 0x1UL;
}

__STATIC_INLINE uint32_t * nrf_egu_task_address_get(NRF_EGU_Type * NRF_EGUx,
                                                    nrf_egu_task_t egu_task)
{
    NRFX_ASSERT(NRF_EGUx);
    return (uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_task);
}

__STATIC_INLINE uint32_t * nrf_egu_task_trigger_address_get(NRF_EGU_Type * NRF_EGUx,
                                                            uint8_t channel)
{
    NRFX_ASSERT(NRF_EGUx);
    NRFX_ASSERT(channel < nrf_egu_channel_count(NRF_EGUx));
    return (uint32_t*)&NRF_EGUx->TASKS_TRIGGER[channel];
}

__STATIC_INLINE nrf_egu_task_t nrf_egu_task_trigger_get(NRF_EGU_Type * NRF_EGUx, uint8_t channel)
{
    NRFX_ASSERT(NRF_EGUx);
    NRFX_ASSERT(channel < nrf_egu_channel_count(NRF_EGUx));
    return (nrf_egu_task_t)((uint32_t) NRF_EGU_TASK_TRIGGER0 + (channel * sizeof(uint32_t)));
}

__STATIC_INLINE bool nrf_egu_event_check(NRF_EGU_Type * NRF_EGUx,
                                         nrf_egu_event_t egu_event)
{
    NRFX_ASSERT(NRF_EGUx);
    return (bool)*(volatile uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_event);
}

__STATIC_INLINE void nrf_egu_event_clear(NRF_EGU_Type * NRF_EGUx,
                                         nrf_egu_event_t egu_event)
{
    NRFX_ASSERT(NRF_EGUx);
    *((volatile uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_event)) = 0x0UL;
#if __CORTEX_M == 0x04
    volatile uint32_t dummy = *((volatile uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_event));
    (void)dummy;
#endif
}

__STATIC_INLINE uint32_t * nrf_egu_event_address_get(NRF_EGU_Type * NRF_EGUx,
                                                     nrf_egu_event_t egu_event)
{
    NRFX_ASSERT(NRF_EGUx);
    return (uint32_t *)((uint8_t *)NRF_EGUx + (uint32_t)egu_event);
}

__STATIC_INLINE uint32_t * nrf_egu_event_triggered_address_get(NRF_EGU_Type * NRF_EGUx,
                                                               uint8_t channel)
{
    NRFX_ASSERT(NRF_EGUx);
    NRFX_ASSERT(channel < nrf_egu_channel_count(NRF_EGUx));
    return (uint32_t*)&NRF_EGUx->EVENTS_TRIGGERED[channel];
}

__STATIC_INLINE nrf_egu_event_t nrf_egu_event_triggered_get(NRF_EGU_Type * NRF_EGUx,
                                                            uint8_t channel)
{
    NRFX_ASSERT(NRF_EGUx);
    NRFX_ASSERT(channel < nrf_egu_channel_count(NRF_EGUx));
    return (nrf_egu_event_t)((uint32_t) NRF_EGU_EVENT_TRIGGERED0 + (channel * sizeof(uint32_t)));
}

__STATIC_INLINE void nrf_egu_int_enable(NRF_EGU_Type * NRF_EGUx, uint32_t mask)
{
    NRFX_ASSERT(NRF_EGUx);
    NRF_EGUx->INTENSET = mask;
}

__STATIC_INLINE bool nrf_egu_int_enable_check(NRF_EGU_Type * NRF_EGUx, uint32_t mask)
{
    NRFX_ASSERT(NRF_EGUx);
    return (bool)(NRF_EGUx->INTENSET & mask);
}

__STATIC_INLINE void nrf_egu_int_disable(NRF_EGU_Type * NRF_EGUx, uint32_t mask)
{
    NRFX_ASSERT(NRF_EGUx);
    NRF_EGUx->INTENCLR = mask;
}

__STATIC_INLINE nrf_egu_int_mask_t nrf_egu_int_get(NRF_EGU_Type * NRF_EGUx, uint8_t channel)
{
    NRFX_ASSERT(NRF_EGUx);
    NRFX_ASSERT(channel < nrf_egu_channel_count(NRF_EGUx));
    return (nrf_egu_int_mask_t)((uint32_t) (EGU_INTENSET_TRIGGERED0_Msk << channel));
}

#if defined(DPPI_PRESENT)
__STATIC_INLINE void nrf_egu_subscribe_set(NRF_EGU_Type * p_reg,
                                           nrf_egu_task_t task,
                                           uint8_t        channel)
{
    *((volatile uint32_t *) ((uint8_t *) p_reg + (uint32_t) task + 0x80uL)) =
            ((uint32_t)channel | EGU_SUBSCRIBE_TRIGGER_EN_Msk);
}

__STATIC_INLINE void nrf_egu_subscribe_clear(NRF_EGU_Type * p_reg,
                                             nrf_egu_task_t task)
{
    *((volatile uint32_t *) ((uint8_t *) p_reg + (uint32_t) task + 0x80uL)) = 0;
}

__STATIC_INLINE void nrf_egu_publish_set(NRF_EGU_Type *  p_reg,
                                         nrf_egu_event_t event,
                                         uint8_t         channel)
{
    *((volatile uint32_t *) ((uint8_t *) p_reg + (uint32_t) event + 0x80uL)) =
            ((uint32_t)channel | EGU_PUBLISH_TRIGGERED_EN_Msk);
}

__STATIC_INLINE void nrf_egu_publish_clear(NRF_EGU_Type *  p_reg,
                                           nrf_egu_event_t event)
{
    *((volatile uint32_t *) ((uint8_t *) p_reg + (uint32_t) event + 0x80uL)) = 0;
}
#endif // defined(DPPI_PRESENT)

#endif // SUPPRESS_INLINE_IMPLEMENTATION

/** @} */

#ifdef __cplusplus
}
#endif

#endif
