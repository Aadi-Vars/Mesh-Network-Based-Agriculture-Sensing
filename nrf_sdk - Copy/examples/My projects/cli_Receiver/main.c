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
/** @file
 *
 * @defgroup cli_example_main main.c
 * @{
 * @ingroup cli_example
 * @brief An example presenting OpenThread CLI.
 *
 */
#include "app_scheduler.h"
#include "app_timer.h"
#include "bsp_thread.h"
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"
#include "thread_utils.h"
#include <openthread/cli.h>
#include <openthread/thread.h>
#include <openthread/udp.h>
#include <openthread/ip6.h> 
#include "dht11.h"
#include "bsp.h"

static otUdpSocket m_udpSocket;

bool run = true;

#define SCHED_QUEUE_SIZE      32                              
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE

char mystr[10]="";

static void bsp_event_handler(bsp_event_t event);
APP_TIMER_DEF(m_our_char_timer_id);

/*********************************
 * @section State
 **********************************/

uint16_t temp=0;

void temp_humid_sense(){
    uint16_t temperature, humidity;
    DHTxx_ErrorCode dhtErrCode;
    int i=2;
    while(i>0){
        dhtErrCode = DHTxx_Read(&temperature, &humidity);
        if(dhtErrCode == DHT11_OK) {
            break;
        }
        else{
            i--;
        }
    }
    char myhum[10];
    temp = temperature;
    sprintf(mystr, "%d", temperature);
    sprintf(myhum, "%d", humidity);
    strcat(mystr, ":");
    strcat(mystr, myhum);
    strcat(mystr, ":n1"); // Change node name
}

static void thread_state_changed_callback(uint32_t flags, void * p_context)
{
    NRF_LOG_INFO("State changed! Flags: 0x%08x Current role: %d\r\n",
                 flags, otThreadGetDeviceRole(p_context));
if ((flags & OT_CHANGED_THREAD_ROLE) && (otThreadGetDeviceRole(p_context) != OT_DEVICE_ROLE_DISABLED))
{
    NRF_LOG_INFO("Receiver device attached to network. Attempting to add static IP address.\r\n");

    otError error = OT_ERROR_NONE;
    otIp6Address static_addr;

    // Convert static IP address string to otIp6Address
    otIp6AddressFromString("fdde:ad00:beef:0:3352:22a6:bd4b:9960", &static_addr);

    // Prepare Netif Address structure
    otNetifAddress netifAddr;
    memset(&netifAddr, 0, sizeof(netifAddr));
    netifAddr.mAddress = static_addr;
    netifAddr.mPrefixLength = 64;
    netifAddr.mPreferred = true;
    netifAddr.mValid = true;

    // Add the address
    error = otIp6AddUnicastAddress((otInstance *)p_context, &netifAddr);
    if (error == OT_ERROR_NONE)
    {
        NRF_LOG_INFO("Static IPv6 address added to receiver: %s\r\n", "fdde:ad00:beef:0:3352:22a6:bd4b:9960");
    }
    else
    {
        NRF_LOG_ERROR("Failed to add static IPv6 address to receiver: %s\r\n", otThreadErrorToString(error));
    }

     // Ensure CLI UART is (re)initialized
  //  otCliUartInit((otInstance *)p_context);  // This ensures UART resumes after network change
}

}


/*********************************
 * @section Initialization
 **********************************/


static void timer_init(void)
{
    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


static void leds_init(void)
{
    LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);
}


static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


static void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .radio_mode        = THREAD_RADIO_MODE_RX_ON_WHEN_IDLE,
        .autocommissioning = true,
        .autostart_disable = false,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);

    uint32_t err_code = bsp_thread_init(thread_ot_instance_get());
    APP_ERROR_CHECK(err_code);
}


static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/*********************************
 * @section MultiHop
 **********************************/

// UDP receive callback
void udp_receive_callback(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo) {
    otInstance *instance = (otInstance *)aContext;
    char receivedData[128];
    int length = otMessageRead(aMessage, otMessageGetOffset(aMessage), receivedData, sizeof(receivedData) - 1);
    receivedData[length] = '\0'; // Null-terminate the received data

    NRF_LOG_INFO("Received UDP data: %s", receivedData);
    otCliOutputFormat("Received UDP data: %s\r\n", receivedData);

    // Since this is the final node, we simply process the received data
    // Process the data as needed
    // For example, if it's temperature and humidity data, you can log it or use it for some other purpose
    NRF_LOG_INFO("Final node received data: %s", receivedData);
}

/*********************************
 * @section Main
 **********************************/
int count=0;
bool udp=true;
void allCommands(){
    if (udp){
        char command2[] = "udp open\n";
        otCliConsoleInputLine(command2, strlen(command2));
        char command3[] = "udp bind :: 1234\n";
        otCliConsoleInputLine(command3, strlen(command3));
        udp = false;
    }
}

void udp_send(){
    char command2[] = "udp send fdde:ad00:beef:0:9367:4f3:953:18d3 1234 ";
    sprintf(command2,"%s%s\n",command2,mystr);
    //otCliOutput(command2,strlen(command2));
    otCliOutputFormat("%s", command2);  // safer and intended for formatted CLI output
    otCliConsoleInputLine(command2,strlen(command2));
}

static void timer_timeout_handler(void * p_context)
{
    NRF_LOG_FLUSH();
    if (udp) {
        allCommands();
    }
    
    NRF_LOG_FLUSH();

    
}

void timer_start()
{
    ret_code_t err_code;
    err_code = app_timer_start(m_our_char_timer_id, APP_TIMER_TICKS(8000), NULL);
    APP_ERROR_CHECK(err_code);
}

void timer_stop()
{
    ret_code_t err_code;
    app_timer_stop(m_our_char_timer_id);
    APP_ERROR_CHECK(err_code);
}

static void bsp_event_handler(bsp_event_t event){
    switch (event) {
        case BSP_EVENT_KEY_0:
            timer_start();
            break;
        case BSP_EVENT_KEY_1:
            timer_stop();
            break;
        default:
            timer_start();
            return; // no implementation needed
    }
}

int main(int argc, char *argv[])
{
    log_init();
    scheduler_init();
    timer_init();
    leds_init();
    uint32_t err_code = bsp_init(BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);
    thread_instance_init();
    app_timer_create(&m_our_char_timer_id, APP_TIMER_MODE_REPEATED, timer_timeout_handler);
    timer_start();
    timer_stop();

    // Set the UDP receive callback
    otUdpSocket socket;
    otSockAddr sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.mPort = 1234;
    otUdpOpen(thread_ot_instance_get(), &socket, udp_receive_callback, thread_ot_instance_get());
    otUdpBind(&socket, &sockaddr);

    while (true)
    {
        thread_process();
        app_sched_execute();
        if (NRF_LOG_PROCESS() == false)
        {
            thread_sleep();
        }
    }
}

/**
 *@}
 **/