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
#include "dht11.h"
#include "bsp.h"
#include "nrfx_saadc.h"
#include "nrf_saadc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"
//#include "nrf_sdh.h"
//#include "nrf_sdh_soc.h"

#define SCHED_QUEUE_SIZE      32                              
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE

#define SAADC_CHANNEL1 0
#define SAADC_CHANNEL2 1
#define SAADC_CHANNEL3 2

#define timer_delay 8000 //in milli seconds
#define receiverIpV6Addr "fdde:ad00:beef:0:9377:8fc1:534d:b251"
#define boardID "n4"

char mystr[10] = "";

static void bsp_event_handler(bsp_event_t event);
APP_TIMER_DEF(m_our_char_timer_id);
APP_TIMER_DEF(m_reboot_timer_id);
#include "nrf_nvmc.h"

// Non-volatile memory address for storing power-fail flag
#define POWER_FAIL_FLAG_ADDR    (NRF_UICR->CUSTOMER[0])

// Power-fail flag value
#define POWER_FAIL_FLAG_VALUE   0xA5A5A5A5

static void reboot_timer_handler(void *p_context)
{
    NRF_LOG_INFO("Reboot timer triggered. Rebooting now...");
    otCliOutputFormat("Reboot timer triggered. Rebooting now..."); // Ensure logs are printed before reset
    NRF_LOG_FLUSH(); 
    nrf_delay_ms(100);  // Allow log flush to complete
    NVIC_SystemReset();  // Perform software reset
}


static void timer_start_on_power_restore(void)
{
    if (POWER_FAIL_FLAG_ADDR == POWER_FAIL_FLAG_VALUE)
    {
        // Power failure occurred, start the timer
        timer_start();

        // Clear the flag in non-volatile memory
        nrf_nvmc_write_word(POWER_FAIL_FLAG_ADDR, 0);
    }
}

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}

void saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel1_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

    nrf_saadc_channel_config_t channel2_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

    nrf_saadc_channel_config_t channel3_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel1_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(1, &channel2_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(2, &channel3_config);
    APP_ERROR_CHECK(err_code);
}

nrf_saadc_value_t sample1;
nrf_saadc_value_t sample2;
nrf_saadc_value_t sample3;

void read_rain(int *samp1, int *samp2, int *samp3)
{
    nrf_saadc_value_t sample1;
    nrf_saadc_value_t sample2;
    nrf_saadc_value_t sample3;

    nrfx_saadc_sample_convert(SAADC_CHANNEL1, &sample1);
    nrfx_saadc_sample_convert(SAADC_CHANNEL2, &sample2);
    nrfx_saadc_sample_convert(SAADC_CHANNEL3, &sample3);

    *samp1 = sample1;
    *samp2 = sample2;
    *samp3 = sample3;
}

static void thread_state_changed_callback(uint32_t flags, void *p_context)
{
    NRF_LOG_INFO("State changed! Flags: 0x%08x Current role: %d\r\n",
                 flags, otThreadGetDeviceRole(p_context));
}

static void timer_init(void)
{
    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

///@brief Function for initializing the LEDs.
// */
static void leds_init(void)
{
    LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);
}

///@brief Function for initializing the nrf log module.
// */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

///@brief Function for initializing the Thread Stack.
// */
static void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .radio_mode = THREAD_RADIO_MODE_RX_ON_WHEN_IDLE,
        .autocommissioning = true,
        .autostart_disable = false,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);

    uint32_t err_code = bsp_thread_init(thread_ot_instance_get());
    APP_ERROR_CHECK(err_code);
}

///@brief Function for initializing scheduler module.
// */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/***********
 * @section Main
 ************/
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

void udp_send(const char *destAddr, const char *data)
{
    char command[256];
    sprintf(command, "udp send %s 1234 %s\n", destAddr, data);
    otCliConsoleInputLine(command, strlen(command));
}

static void timer_timeout_handler(void * p_context)
{
    NRF_LOG_FLUSH();
    if (udp) {
        allCommands();   
    }

    read_rain(&sample1, &sample2, &sample3);

    char myrain[50];
    sprintf(myrain, "%s:%i:%i:%i:%s","1234", sample1, sample2, sample3, ":n4");
    
    //sprintf(myrain, "%s:%i:%s","1234", sample1, ":n2");
    const char *receiverAddr = "fdde:ad00:beef:0:3352:22a6:bd4b:9960";  // Update with the actual address from your network

    // replace with actual receiver address
    udp_send(receiverAddr, myrain);

    NRF_LOG_FLUSH();
}

void timer_start()
{
    ret_code_t err_code;
    err_code = app_timer_start(m_our_char_timer_id, APP_TIMER_TICKS(timer_delay), NULL);
    APP_ERROR_CHECK(err_code);
}

void timer_stop()
{
    ret_code_t err_code;
    app_timer_stop(m_our_char_timer_id);
    APP_ERROR_CHECK(err_code);
}

static void bsp_event_handler(bsp_event_t event)
{
    switch (event)
    {
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

static void power_manage(void)
{
    // Handle power management, enters low power state
    nrf_pwr_mgmt_run();
}

int main(int argc, char *argv[])
{
    log_init();
    saadc_init();
    scheduler_init();
    timer_init();
    leds_init();
    ret_code_t err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
    err_code = bsp_init(BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);
    thread_instance_init();
    app_timer_create(&m_our_char_timer_id, APP_TIMER_MODE_REPEATED, timer_timeout_handler);
    timer_start();

    // Create reboot timer
    app_timer_create(&m_reboot_timer_id, APP_TIMER_MODE_REPEATED, reboot_timer_handler);

// Start reboot timer: 30 minutes = 30 * 60 * 1000 = 1,800,000 ms
    ret_code_t reboot_err_code = app_timer_start(m_reboot_timer_id, APP_TIMER_TICKS(1800000), NULL);
    APP_ERROR_CHECK(reboot_err_code);

    while (true)
    {
        // Process Thread stack and application scheduler events
        thread_process();
        app_sched_execute();

        // Process logs
        if (NRF_LOG_PROCESS() == false)
        {
            // Enter System ON sleep mode
            power_manage();
        }
    }
}