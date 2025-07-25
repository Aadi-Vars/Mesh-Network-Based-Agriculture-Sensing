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
/** @file
 *
 * @defgroup ble_ancs_c Apple Notification Service Client
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief Apple Notification Center Service Client module.
 *
 * @details Disclaimer: This client implementation of the Apple Notification Center Service can
 *          be changed at any time by Nordic Semiconductor ASA. Server implementations such as the
 *          ones found in iOS can be changed at any time by Apple and may cause this client
 *          implementation to stop working.
 *
 * This module implements the Apple Notification Center Service (ANCS) client.
 * This client can be used as a Notification Consumer (NC) that receives data
 * notifications from a Notification Provider (NP). The NP is typically an iOS
 * device that is acting as a server. For terminology and up-to-date specs, see
 * http://developer.apple.com.
 *
 * The term "notification" is used in two different meanings:
 * - An <i>iOS notification</i> is the data received from the Notification Provider.
 * - A <i>GATTC notification</i> is a way to transfer data with <i>Bluetooth</i> Smart.
 * In this module, iOS notifications are received through the GATTC notifications.
 * The full term (iOS notification or GATTC notification) is used where required to avoid confusion.
 *
 * Upon initializing the module, you must add the different iOS notification attributes you
 * would like to receive for iOS notifications (see @ref nrf_ble_ancs_c_attr_add).
 *
 * Once a connection is established with a central device, the module does a service discovery to
 * discover the ANCS server handles. If this succeeds (@ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE),
 * the handles for the ANCS server are part of the @ref ble_ancs_c_evt_t structure and must be
 * assigned to an ANCS_C instance using the @ref nrf_ble_ancs_c_handles_assign function. For more
 * information about service discovery, see the @ref lib_ble_db_discovery documentation.
 *
 * The application can now subscribe to iOS notifications with 
 * @ref ble_ancs_c_notif_source_notif_enable. The notifications arrive in the @ref BLE_ANCS_C_EVT_NOTIF event.
 * @ref nrf_ble_ancs_c_request_attrs can be used to request attributes for the notifications. The attributes
 * arrive in the @ref BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE event.
 * Use @ref nrf_ble_ancs_c_app_attr_request to request attributes of the app that issued
 * the notifications. The app attributes arrive in the @ref BLE_ANCS_C_EVT_APP_ATTRIBUTE event.
 * Use @ref nrf_ancs_perform_notif_action to make the Notification Provider perform an
 * action based on the provided notification.
 *
 * @msc
 * hscale = "1.5";
 * Application, ANCS_C;
 * |||;
 * Application=>ANCS_C   [label = "ble_ancs_c_attr_add(attribute)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_init(ancs_instance, event_handler)"];
 * ...;
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_DISCOVERY_COMPLETE"];
 * Application=>ANCS_C   [label = "ble_ancs_c_handles_assign(ancs_instance, conn_handle, service_handles)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_notif_source_notif_enable(ancs_instance)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_data_source_notif_enable(ancs_instance)"];
 * |||;
 * ...;
 * |||;
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_NOTIF"];
 * |||;
 * ...;
 * |||;
 * Application=>ANCS_C   [label = "ble_ancs_c_request_attrs(attr_id, buffer)"];
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE"];
 * |||;
 * @endmsc
 *
 * @note    The application must register this module as the BLE event observer by using the
 *          NRF_SDH_BLE_OBSERVER macro. Example:
 *          @code
 *              ble_ancs_c_t instance;
 *              NRF_SDH_BLE_OBSERVER(anything, BLE_ANCS_C_BLE_OBSERVER_PRIO,
 *                                   ble_ancs_c_on_ble_evt, &instance);
 *          @endcode
 */
#ifndef BLE_ANCS_C_H__
#define BLE_ANCS_C_H__

#include "ble_types.h"
#include "ble_srv_common.h"
#include "sdk_errors.h"
#include "ble_db_discovery.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gq.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   Macro for defining a ble_ancs_c instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_ANCS_C_DEF(_name)                                                                       \
static ble_ancs_c_t _name;                                                                          \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_ANCS_C_BLE_OBSERVER_PRIO,                                                  \
                     ble_ancs_c_on_ble_evt, &_name)

/** @brief Macro for defining multiple ble_ancs_c instances.
 *
 * @param   _name   Name of the array of instances.
 * @param   _cnt    Number of instances to define.
 * @hideinitializer
 */
#define BLE_ANCS_C_ARRAY_DEF(_name, _cnt)                       \
sstatic ble_ancs_c_t _name[_cnt];                               \
NRF_SDH_BLE_OBSERVERS(_name ## _obs,                            \
                      BLE_ANCS_C_BLE_OBSERVER_PRIO,             \
                      ble_ancs_c_on_ble_evt, &_name, _cnt)

#define BLE_ANCS_ATTR_DATA_MAX              32  //!< Maximum data length of an iOS notification attribute.
#define BLE_ANCS_NB_OF_CATEGORY_ID          12  //!< Number of iOS notification categories: Other, Incoming Call, Missed Call, Voice Mail, Social, Schedule, Email, News, Health and Fitness, Business and Finance, Location, Entertainment.
#define BLE_ANCS_NB_OF_NOTIF_ATTR           8   //!< Number of iOS notification attributes: AppIdentifier, Title, Subtitle, Message, MessageSize, Date, PositiveActionLabel, NegativeActionLabel.
#define BLE_ANCS_NB_OF_APP_ATTR             1   //!< Number of iOS application attributes: DisplayName.
#define BLE_ANCS_NB_OF_EVT_ID               3   //!< Number of iOS notification events: Added, Modified, Removed.

/** @brief Length of the iOS notification data.
 *
 * @details 8 bytes:
 * Event ID |Event flags |Category ID |Category count|Notification UID
 * ---------|------------|------------|--------------|----------------
 * 1 byte   | 1 byte     | 1 byte     | 1 byte       | 4 bytes
 */
#define BLE_ANCS_NOTIFICATION_DATA_LENGTH   8

#define ANCS_UUID_SERVICE                   0xF431  //!< 16-bit service UUID for the Apple Notification Center Service.
#define ANCS_UUID_CHAR_CONTROL_POINT        0xD8F3  //!< 16-bit control point UUID.
#define ANCS_UUID_CHAR_DATA_SOURCE          0xC6E9  //!< 16-bit data source UUID.
#define ANCS_UUID_CHAR_NOTIFICATION_SOURCE  0x120D  //!< 16-bit notification source UUID.

#define BLE_ANCS_EVENT_FLAG_SILENT          0       //!< 0b.......1 Silent: First (LSB) bit is set. All flags can be active at the same time.
#define BLE_ANCS_EVENT_FLAG_IMPORTANT       1       //!< 0b......1. Important: Second (LSB) bit is set. All flags can be active at the same time.
#define BLE_ANCS_EVENT_FLAG_PREEXISTING     2       //!< 0b.....1.. Pre-existing: Third (LSB) bit is set. All flags can be active at the same time.
#define BLE_ANCS_EVENT_FLAG_POSITIVE_ACTION 3       //!< 0b....1... Positive action: Fourth (LSB) bit is set. All flags can be active at the same time.
#define BLE_ANCS_EVENT_FLAG_NEGATIVE_ACTION 4       //!< 0b...1.... Negative action: Fifth (LSB) bit is set. All flags can be active at the same time.

/** @defgroup BLE_ANCS_NP_ERROR_CODES Notification Provider (iOS) Error Codes
 * @{ */
#define BLE_ANCS_NP_UNKNOWN_COMMAND         0x01A0  //!< The command ID is unknown to the NP.
#define BLE_ANCS_NP_INVALID_COMMAND         0x01A1  //!< The command format is invalid.
#define BLE_ANCS_NP_INVALID_PARAMETER       0x01A2  //!< One or more parameters do not exist in the NP.
#define BLE_ANCS_NP_ACTION_FAILED           0x01A3  //!< The action failed to be performed by the NP.
/** @} */


/**@brief Event types that are passed from client to application on an event. */
typedef enum
{
    BLE_ANCS_C_EVT_DISCOVERY_COMPLETE,         /**< A successful connection has been established and the service was found on the connected peer. */
    BLE_ANCS_C_EVT_DISCOVERY_FAILED,           /**< It was not possible to discover the service or characteristics of the connected peer. */
    BLE_ANCS_C_EVT_NOTIF,                      /**< An iOS notification was received on the notification source control point. */
    BLE_ANCS_C_EVT_INVALID_NOTIF,              /**< An iOS notification was received on the notification source control point, but the format is invalid. */
    BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE,            /**< A received iOS notification attribute has been parsed. */
    BLE_ANCS_C_EVT_APP_ATTRIBUTE,              /**< An iOS app attribute has been parsed. */
    BLE_ANCS_C_EVT_NP_ERROR,                   /**< An error has been sent on the ANCS Control Point from the iOS Notification Provider. */
} ble_ancs_c_evt_type_t;

/**@brief Category IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_CATEGORY_ID_OTHER,                /**< The iOS notification belongs to the "Other" category.  */
    BLE_ANCS_CATEGORY_ID_INCOMING_CALL,        /**< The iOS notification belongs to the "Incoming Call" category. */
    BLE_ANCS_CATEGORY_ID_MISSED_CALL,          /**< The iOS notification belongs to the "Missed Call" category. */
    BLE_ANCS_CATEGORY_ID_VOICE_MAIL,           /**< The iOS notification belongs to the "Voice Mail" category. */
    BLE_ANCS_CATEGORY_ID_SOCIAL,               /**< The iOS notification belongs to the "Social" category. */
    BLE_ANCS_CATEGORY_ID_SCHEDULE,             /**< The iOS notification belongs to the "Schedule" category. */
    BLE_ANCS_CATEGORY_ID_EMAIL,                /**< The iOS notification belongs to the "Email" category. */
    BLE_ANCS_CATEGORY_ID_NEWS,                 /**< The iOS notification belongs to the "News" category. */
    BLE_ANCS_CATEGORY_ID_HEALTH_AND_FITNESS,   /**< The iOS notification belongs to the "Health and Fitness" category. */
    BLE_ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE, /**< The iOS notification belongs to the "Business and Finance" category. */
    BLE_ANCS_CATEGORY_ID_LOCATION,             /**< The iOS notification belongs to the "Location" category. */
    BLE_ANCS_CATEGORY_ID_ENTERTAINMENT         /**< The iOS notification belongs to the "Entertainment" category. */
} ble_ancs_c_category_id_val_t;

/**@brief Event IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_EVENT_ID_NOTIFICATION_ADDED,     /**< The iOS notification was added. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_MODIFIED,  /**< The iOS notification was modified. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_REMOVED    /**< The iOS notification was removed. */
} ble_ancs_c_evt_id_values_t;

/**@brief Control point command IDs that the Notification Consumer can send to the Notification Provider. */
typedef enum
{
    BLE_ANCS_COMMAND_ID_GET_NOTIF_ATTRIBUTES,      /**< Requests attributes to be sent from the NP to the NC for a given notification. */
    BLE_ANCS_COMMAND_ID_GET_APP_ATTRIBUTES,        /**< Requests attributes to be sent from the NP to the NC for a given iOS app. */
    BLE_ANCS_COMMAND_ID_GET_PERFORM_NOTIF_ACTION,  /**< Requests an action to be performed on a given notification. For example, dismiss an alarm. */
} ble_ancs_c_cmd_id_val_t;

/**@brief IDs for actions that can be performed for iOS notifications. */
typedef enum
{
    ACTION_ID_POSITIVE = 0,  /**< Positive action. */
    ACTION_ID_NEGATIVE       /**< Negative action. */
} ble_ancs_c_action_id_values_t;

/**@brief App attribute ID values.
 * @details Currently, only one value is defined. However, the number of app
 * attributes might increase. For this reason, they are stored in an enumeration.
 */
typedef enum
{
    BLE_ANCS_APP_ATTR_ID_DISPLAY_NAME = 0          /**< Command used to get the display name for an app identifier. */
} ble_ancs_c_app_attr_id_val_t;

/**@brief IDs for iOS notification attributes. */
typedef enum
{
    BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER = 0,     /**< Identifies that the attribute data is of an "App Identifier" type. */
    BLE_ANCS_NOTIF_ATTR_ID_TITLE,                  /**< Identifies that the attribute data is a "Title". */
    BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE,               /**< Identifies that the attribute data is a "Subtitle". */
    BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,                /**< Identifies that the attribute data is a "Message". */
    BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE,           /**< Identifies that the attribute data is a "Message Size". */
    BLE_ANCS_NOTIF_ATTR_ID_DATE,                   /**< Identifies that the attribute data is a "Date". */
    BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL,  /**< The notification has a "Positive action" that can be executed associated with it. */
    BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL,  /**< The notification has a "Negative action" that can be executed associated with it. */
} ble_ancs_c_notif_attr_id_val_t;

/**@brief Flags for iOS notifications. */
typedef struct
{
    uint8_t silent          : 1;  //!< If this flag is set, the notification has a low priority.
    uint8_t important       : 1;  //!< If this flag is set, the notification has a high priority.
    uint8_t pre_existing    : 1;  //!< If this flag is set, the notification is pre-existing.
    uint8_t positive_action : 1;  //!< If this flag is set, the notification has a positive action that can be taken.
    uint8_t negative_action : 1;  //!< If this flag is set, the notification has a negative action that can be taken.
} ble_ancs_c_notif_flags_t;

/**@brief Parsing states for received iOS notification and app attributes. */
typedef enum
{
    COMMAND_ID,    /**< Parsing the command ID. */
    NOTIF_UID,     /**< Parsing the notification UID. */
    APP_ID,        /**< Parsing app ID. */
    ATTR_ID,       /**< Parsing attribute ID. */
    ATTR_LEN1,     /**< Parsing the LSB of the attribute length. */
    ATTR_LEN2,     /**< Parsing the MSB of the attribute length. */
    ATTR_DATA,     /**< Parsing the attribute data. */
    ATTR_SKIP,     /**< Parsing is skipped for the rest of an attribute (or entire attribute). */
    DONE,          /**< Parsing for one attribute is done. */
} ble_ancs_c_parse_state_t;

/**@brief iOS notification structure. */
typedef struct
{
    uint32_t                        notif_uid;       //!< Notification UID.
    ble_ancs_c_evt_id_values_t      evt_id;          //!< Whether the notification was added, removed, or modified.
    ble_ancs_c_notif_flags_t        evt_flags;       //!< Bitmask to signal whether a special condition applies to the notification. For example, "Silent" or "Important".
    ble_ancs_c_category_id_val_t    category_id;     //!< Classification of the notification type. For example, email or location.
    uint8_t                         category_count;  //!< Current number of active notifications for this category ID.
} ble_ancs_c_evt_notif_t;

/**@brief iOS attribute structure. This type is used for both notification attributes and app attributes. */
typedef struct
{
    uint16_t                          attr_len;     //!< Length of the received attribute data.
    uint32_t                          attr_id;      //!< Classification of the attribute type. For example, "Title" or "Date".
    uint8_t                         * p_attr_data;  //!< Pointer to where the memory is allocated for storing incoming attributes.
} ble_ancs_c_attr_t;

/**@brief iOS notification attribute structure for incoming attributes. */
typedef struct
{
    uint32_t                          notif_uid;    //!< UID of the notification that the attribute belongs to.
    ble_ancs_c_attr_t                 attrs;        //!< A received attribute.
} ble_ancs_c_evt_attr_t;

typedef struct
{
    uint16_t                          attr_len;     //!< Length of the received attribute data.
    uint32_t                          attr_id;      //!< Classification of the attribute type. For example, "Title" or "Date".
    uint8_t                         * p_attr_data;  //!< Pointer to where the memory is allocated for storing incoming attributes.
} ble_ancs_c_evt_app_attr_t;

/**@brief iOS notification attribute content requested by the application. */
typedef struct
{
    bool                              get;          //!< Boolean to determine whether this attribute will be requested from the Notification Provider.
    uint32_t                          attr_id;      //!< Attribute ID: AppIdentifier(0), Title(1), Subtitle(2), Message(3), MessageSize(4), Date(5), PositiveActionLabel(6), NegativeActionLabel(7).
    uint16_t                          attr_len;     //!< Length of the attribute. If more data is received from the Notification Provider, all the data beyond this length is discarded.
    uint8_t                         * p_attr_data;  //!< Pointer to where the memory is allocated for storing incoming attributes.
} ble_ancs_c_attr_list_t;

/**@brief Structure used for holding the Apple Notification Center Service found during the
          discovery process.
 */
typedef struct
{
    ble_gattc_service_t service;            //!< The GATT Service holding the discovered Apple Notification Center Service. (0xF431).
    ble_gattc_char_t    control_point_char; //!< ANCS Control Point Characteristic. Allows interaction with the peer (0xD8F3).
    ble_gattc_char_t    notif_source_char;  //!< ANCS Notification Source Characteristic. Keeps track of arrival, modification, and removal of notifications (0x120D).
    ble_gattc_desc_t    notif_source_cccd;  //!< ANCS Notification Source Characteristic Descriptor. Enables or disables GATT notifications.
    ble_gattc_char_t    data_source_char;   //!< ANCS Data Source Characteristic, where attribute data for the notifications is received from peer (0xC6E9).
    ble_gattc_desc_t    data_source_cccd;   //!< ANCS Data Source Characteristic Descriptor. Enables or disables GATT notifications.
} ble_ancs_c_service_t;

/**@brief ANCS client module event structure.
 *
 * @details The structure contains the event that is to be handled by the main application.
 */
typedef struct
{
    ble_ancs_c_evt_type_t  evt_type;                       //!< Type of event.
    uint16_t               conn_handle;                    //!< Connection handle on which the ANCS service was discovered on the peer device. This is filled if the @p evt_type is @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.
    ble_ancs_c_evt_notif_t notif;                          //!< iOS notification. This is filled if @p evt_type is @ref BLE_ANCS_C_EVT_NOTIF.
    uint16_t               err_code_np;                    //!< An error coming from the Notification Provider. This is filled with @ref BLE_ANCS_NP_ERROR_CODES if @p evt_type is @ref BLE_ANCS_C_EVT_NP_ERROR.
    ble_ancs_c_attr_t      attr;                           //!< iOS notification attribute or app attribute, depending on the event type.
    uint32_t               notif_uid;                      //!< Notification UID.
    uint8_t                app_id[BLE_ANCS_ATTR_DATA_MAX]; //!< App identifier.
    ble_ancs_c_service_t   service;                        //!< Information on the discovered Alert Notification Service. This is filled if the @p evt_type is @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.
} ble_ancs_c_evt_t;

/**@brief iOS notification event handler type. */
typedef void (*ble_ancs_c_evt_handler_t) (ble_ancs_c_evt_t * p_evt);

typedef struct
{
    ble_ancs_c_attr_list_t * p_attr_list;              //!< The current list of attributes that are being parsed. This will point to either @ref ble_ancs_c_t::ancs_notif_attr_list or @ref  ble_ancs_c_t::ancs_app_attr_list.
    uint32_t                 nb_of_attr;               //!< Number of possible attributes. When parsing begins, it is set to either @ref BLE_ANCS_NB_OF_NOTIF_ATTR or @ref BLE_ANCS_NB_OF_APP_ATTR.
    uint32_t                 expected_number_of_attrs; //!< The number of attributes expected upon receiving attributes. Keeps track of when to stop reading incoming attributes.
    ble_ancs_c_parse_state_t parse_state;              //!< ANCS notification attribute parsing state.
    ble_ancs_c_cmd_id_val_t  command_id;               //!< Variable to keep track of what command type is being parsed ( @ref BLE_ANCS_COMMAND_ID_GET_NOTIF_ATTRIBUTES or @ref BLE_ANCS_COMMAND_ID_GET_APP_ATTRIBUTES).
    uint8_t                * p_data_dest;              //!< Attribute that the parsed data is copied into.
    uint16_t                 current_attr_index;       //!< Variable to keep track of the parsing progress, for the given attribute.
    uint32_t                 current_app_id_index;     //!< Variable to keep track of the parsing progress, for the given app identifier.
} ble_ancs_parse_sm_t;

/**@brief iOS notification structure, which contains various status information for the client. */
typedef struct
{
    ble_ancs_c_evt_handler_t         evt_handler;                                     //!< Event handler to be called for handling events in the Apple Notification client application.
    ble_srv_error_handler_t          error_handler;                                   //!< Function to be called in case of an error.
    nrf_ble_gq_req_error_cb_t        gatt_err_handler;                                //!< Error handler to be called in case of an error from SoftDevice.
    uint16_t                         conn_handle;                                     //!< Handle of the current connection. Set with @ref nrf_ble_ancs_c_handles_assign when connected.
    ble_ancs_c_service_t             service;                                         //!< Structure to store the different handles and UUIDs related to the service.
    ble_ancs_c_attr_list_t           ancs_notif_attr_list[BLE_ANCS_NB_OF_NOTIF_ATTR]; //!< For all attributes: contains information about whether the attributes are to be requested upon attribute request, and the length and buffer of where to store attribute data.
    ble_ancs_c_attr_list_t           ancs_app_attr_list[BLE_ANCS_NB_OF_APP_ATTR];     //!< For all app attributes: contains information about whether the attributes are to be requested upon attribute request, and the length and buffer of where to store attribute data.
    uint32_t                         number_of_requested_attr;                        //!< The number of attributes that are to be requested when an iOS notification attribute request is made.
    ble_ancs_parse_sm_t              parse_info;                                      //!< Structure containing different information used to parse incoming attributes correctly (from data_source characteristic).
    ble_ancs_c_evt_t                 evt;                                             //!< Allocate memory for the event here. The event is filled with several iterations of the @ref ancs_parse_get_attrs_response function when requesting iOS notification attributes. 
    nrf_ble_gq_t                   * p_gatt_queue;                                    //!< Pointer to the BLE GATT Queue instance.

} ble_ancs_c_t;

/**@brief Apple Notification client init structure, which contains all options and data needed for
 *        initialization of the client. */
typedef struct
{
    ble_ancs_c_evt_handler_t   evt_handler;    //!< Event handler to be called for handling events in the Battery Service.
    ble_srv_error_handler_t    error_handler;  //!< Function to be called in case of an error.
    nrf_ble_gq_t             * p_gatt_queue;   //!< Pointer to the BLE GATT Queue instance.
} ble_ancs_c_init_t;


/**@brief Apple Notification Center Service UUIDs. */
extern const ble_uuid128_t ble_ancs_base_uuid128;     //!< Service UUID.
extern const ble_uuid128_t ble_ancs_cp_base_uuid128;  //!< Control Point UUID.
extern const ble_uuid128_t ble_ancs_ns_base_uuid128;  //!< Notification Source UUID.
extern const ble_uuid128_t ble_ancs_ds_base_uuid128;  //!< Data Source UUID.


/**@brief Function for handling BLE stack events of the application.
 *
 * @details Handles all events from the BLE stack that are of interest to the ANCS client.
 *
 * @param[in] p_ble_evt  Event received from the BLE stack.
 * @param[in] p_context     ANCS client structure.
 */
void ble_ancs_c_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**@brief     Function for handling events from the Database Discovery module.
 *
 * @details   This function handles an event from the Database Discovery module and determines
 *            whether it relates to the discovery of ANCS at the peer. If it does, this function 
 *            calls the application's event handler to indicate that ANCS was
 *            discovered at the peer. The function also populates the event with service-related
 *            information before providing it to the application.
 *
 * @param[in] p_ancs Pointer to the ANCS client structure.
 * @param[in] p_evt  Pointer to the event received from the Database Discovery module.
 */
 void ble_ancs_c_on_db_disc_evt(ble_ancs_c_t * p_ancs, ble_db_discovery_evt_t * p_evt);


/**@brief Function for initializing the ANCS client.
 *
 * @param[out] p_ancs       ANCS client structure. This structure must be
 *                          supplied by the application. It is initialized by this function
 *                          and will later be used to identify this particular client instance.
 * @param[in]  p_ancs_init  Information needed to initialize the client.
 *
 * @retval NRF_SUCCESS  If the client was initialized successfully. Otherwise, an error code is returned.
 */
ret_code_t ble_ancs_c_init(ble_ancs_c_t * p_ancs, ble_ancs_c_init_t const * p_ancs_init);


/**@brief Function for writing to the CCCD to enable notifications from the Apple Notification Service.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, 
 *                     this API propagates the error code returned by function
 *                     @ref nrf_ble_gq_item_add.
 */
ret_code_t ble_ancs_c_notif_source_notif_enable(ble_ancs_c_t const * p_ancs);


/**@brief Function for writing to the CCCD to enable data source notifications from the ANCS.
 *
 * @param[in] p_ancs iOS notification structure. This structure must be supplied by
 *                   the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, 
 *                     this API propagates the error code returned by function
 *                     @ref nrf_ble_gq_item_add.
 */
ret_code_t ble_ancs_c_data_source_notif_enable(ble_ancs_c_t const * p_ancs);


/**@brief Function for writing to the CCCD to disable notifications from the ANCS.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise 
 *                     this API propagates the error code returned by function
 *                     @ref nrf_ble_gq_item_add.
 */
ret_code_t ble_ancs_c_notif_source_notif_disable(ble_ancs_c_t const * p_ancs);


/**@brief Function for writing to the CCCD to disable data source notifications from the ANCS.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, 
 *                     this API propagates the error code returned by function
 *                     @ref nrf_ble_gq_item_add.
 */
ret_code_t ble_ancs_c_data_source_notif_disable(ble_ancs_c_t const * p_ancs);


/**@brief Function for registering attributes that will be requested when @ref nrf_ble_ancs_c_request_attrs
 *        is called.
 *
 * @param[in] p_ancs ANCS client instance on which the attribute is registered.
 * @param[in] id     ID of the attribute that is added.
 * @param[in] p_data Pointer to the buffer where the data of the attribute can be stored.
 * @param[in] len    Length of the buffer where the data of the attribute can be stored.

 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_attr_add(ble_ancs_c_t                       * p_ancs,
                                   ble_ancs_c_notif_attr_id_val_t const id,
                                   uint8_t                            * p_data,
                                   uint16_t const                       len);


/**@brief Function for removing attributes, so that they are no longer requested when
 *        @ref nrf_ble_ancs_c_request_attrs is called.
 *
 * @param[in] p_ancs ANCS client instance on which the attribute is removed.
 * @param[in] id     ID of the attribute that is removed.
 *
 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_attr_remove(ble_ancs_c_t                       * p_ancs,
                                      ble_ancs_c_notif_attr_id_val_t const id);

/**@brief Function for removing attributes, so that they are no longer requested when
 *        @ref nrf_ble_ancs_c_app_attr_request is called.
 *
 * @param[in] p_ancs ANCS client instance on which the attribute is removed.
 * @param[in] id     ID of the attribute that is removed.
 *
 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_app_attr_remove(ble_ancs_c_t                     * p_ancs,
                                          ble_ancs_c_app_attr_id_val_t const id);


/**@brief Function for registering attributes that will be requested when @ref nrf_ble_ancs_c_app_attr_request
 *        is called.
 *
 * @param[in] p_ancs ANCS client instance on which the attribute is registered.
 * @param[in] id     ID of the attribute that is added.
 * @param[in] p_data Pointer to the buffer where the data of the attribute can be stored.
 * @param[in] len    Length of the buffer where the data of the attribute can be stored.
 *
 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_app_attr_add(ble_ancs_c_t                     * p_ancs,
                                       ble_ancs_c_app_attr_id_val_t const id,
                                       uint8_t                          * p_data,
                                       uint16_t const                     len);

/**@brief Function for clearing the list of notification attributes and app attributes that
 *        would be requested from NP.
 *
 * @param[in] p_ancs   iOS notification structure. This structure must be supplied by
 *                     the application. It identifies the particular client instance to use.
**/
ret_code_t nrf_ble_ancs_c_attr_req_clear_all(ble_ancs_c_t * p_ancs);

/**@brief Function for requesting attributes for a notification.
 *
 * @param[in] p_ancs   iOS notification structure. This structure must be supplied by
 *                     the application. It identifies the particular client instance to use.
 * @param[in] p_notif  Pointer to the notification whose attributes will be requested from
 *                     the Notification Provider.
 *
 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_request_attrs(ble_ancs_c_t                 * p_ancs,
                                        ble_ancs_c_evt_notif_t const * p_notif);

/**@brief Function for requesting attributes for a given app.
 *
 * @param[in] p_ancs   iOS notification structure. This structure must be supplied by
 *                     the application. It identifies the particular client instance to use.
 * @param[in] p_app_id App identifier of the app for which the app attributes are requested.
 * @param[in] len      Length of the app identifier.
 *
 * @retval NRF_SUCCESS If all operations are successful. Otherwise, an error code is returned.
 */
ret_code_t nrf_ble_ancs_c_app_attr_request(ble_ancs_c_t  * p_ancs,
                                           uint8_t const * p_app_id,
                                           uint32_t        len);


/**@brief Function for performing a notification action.
 *
 * @param[in] p_ancs    iOS notification structure. This structure must be supplied by
 *                      the application. It identifies the particular client instance to use.
 * @param[in] uuid      The UUID of the notification for which to perform the action.
 * @param[in] action_id Perform a positive or negative action.
 *
 * @retval    NRF_SUCCESS    If the operation is successful.
 * @retval    NRF_ERROR_NULL If @p p_ancs is a NULL pointer.
 * @retval    err_code       Otherwise, this API propagates the error code returned by function
 *                           @ref nrf_ble_gq_item_add.
 */
ret_code_t nrf_ancs_perform_notif_action(ble_ancs_c_t * p_ancs,
                                         uint32_t uuid,
                                         ble_ancs_c_action_id_values_t action_id);

/**@brief Function for assigning a handle to this instance of ancs_c.
 *
 * @details Call this function when a link has been established with a peer to
 *          associate the link to this instance of the module. This makes it
 *          possible to handle several links and associate each link to a particular
 *          instance of this module. The connection handle and attribute handles are
 *          provided from the discovery event @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.
 *
 * @param[in] p_ancs      Pointer to the ANCS client structure instance for associating the link.
 * @param[in] conn_handle Connection handle to associated with the given ANCS instance.
 * @param[in] p_service   Attribute handles on the ANCS server that you want this ANCS client to
 *                        interact with.
 *
 * @retval    NRF_SUCCESS    If the operation is successful.
 * @retval    NRF_ERROR_NULL If @p p_ancs is a NULL pointer.
 * @retval    err_code		 Otherwise, this API propagates the error code returned by function
 *                           @ref nrf_ble_gq_conn_handle_register.
 */
ret_code_t nrf_ble_ancs_c_handles_assign(ble_ancs_c_t               * p_ancs,
                                         uint16_t const               conn_handle,
                                         ble_ancs_c_service_t const * p_service);

#ifdef __cplusplus
}
#endif

#endif // BLE_ANCS_C_H__

/** @} */

