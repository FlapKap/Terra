
// Standard library includes
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <kernel_defines.h>
#include <fmt.h>
#include <macros/utils.h>
#include <inttypes.h>

// Local includes
#include "configuration.h"
#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "execution.h"
#include "operators.h"
#include "number.h"
#include "network.h"
#include "sensors.h"
#include "print_utils.h"


// RIOT includes
#include "periph/pm.h"
#include "pm_layered.h"
#include <periph/rtc.h>
#include "ztimer.h"
#include "ztimer/stopwatch.h"

#include "periph/i2c.h"
#include "periph/spi.h"
#include "periph/uart.h"
#include "periph/gpio.h"

// Power tracking
#include "power_sync.h"

#include "log.h"

// Testing
#ifdef APPLICATION_RUN_TEST
#include "embUnit.h"

#include "expression_tests.h"
#include "configuration_tests.h"
void test_encode_input(void);
void test_encode_output(void);

int main(void)
{
  //ztimer_sleep(ZTIMER_MSEC, 1000); // wait one second before starting
  puts("Start tests");
  TESTS_START();
  TESTS_RUN(tests_expression());
  TESTS_RUN(tests_configuration());
  TESTS_END();

  return 0;
}
#else
static TerraProtocol_Message msg = TerraProtocol_Message_init_zero;
static TerraProtocol_Output out = TerraProtocol_Output_init_zero;
static TerraConfiguration config = {
    .loop_counter = 0,
    .message = &msg,
    .message_size = UINT8_MAX,
#ifndef DISABLE_LORA
    .loramac = &loramac
#endif
};

//static bool valid_msg = false;

static Number stack_memory[20];
static Stack stack = { 0 };
static Number env_memory[20];
static Env env = { 0 };

static const uint32_t timeout_ms = EXECUTION_EPOCH_S*1000;
static uint32_t sleep_time_s = 0;
// tracking stuff
static ztimer_stopwatch_t stopwatch = { 0 };

// timing measurements
static int32_t conf_load_time_ms = -1;
static int32_t sensor_init_time_ms = -1;
static int32_t env_init_time_ms = -1;
static int32_t net_init_time_ms = -1;
static int32_t sensor_collect_time_ms = -1;
static int32_t exec_time_ms = -1;
static int32_t sync_word_time_ms = -1;
static int32_t listen_time_ms = -1;
static int32_t send_time_ms = -1;
static int32_t conf_save_time_ms = -1;

void startup(void){

  ztimer_stopwatch_reset(&stopwatch);
  print_build_info();
  print_device_info();
  
  configuration_load(&config);
  conf_load_time_ms = ztimer_stopwatch_reset(&stopwatch);
  
  sensors_initialize_enabled();
  sensor_init_time_ms = ztimer_stopwatch_reset(&stopwatch);

  LOG_INFO("enabled sensors\n");
  sensors_print_enabled();

  ztimer_stopwatch_reset(&stopwatch);
  // initialize env and stack
  env_init_env(env_memory, sizeof(Number)/sizeof(env_memory), &env);
  stack_init_stack(stack_memory, sizeof(Number)/sizeof(stack_memory), &stack);
  env_init_time_ms = ztimer_stopwatch_reset(&stopwatch);
  LOG_INFO("environment and stack initialized:\n");
  print_env(&env);
  print_stack(&stack);
  

  network_initialize_network();
  LOG_INFO("network initialized\n");
  net_init_time_ms = ztimer_stopwatch_reset(&stopwatch);

  
  //network_send_heartbeat();
}

void teardown(void){
  LOG_INFO("teardown\n");
  ztimer_stopwatch_reset(&stopwatch);
  configuration_save(&config);
  conf_save_time_ms = ztimer_stopwatch_reset(&stopwatch);
}

void run_activities(void){
  LOG_INFO("Running activities...\n");
  
  //if we have queries to execute
  if (config.message->queries_count > 0)
  {
    // Collect measurements
    LOG_INFO("collecting measurements...\n");

    ztimer_stopwatch_reset(&stopwatch);
    sensors_collect_into_env(&env);
    sensor_collect_time_ms = ztimer_stopwatch_reset(&stopwatch);

    // Execute queries
    LOG_INFO("Execute Queries...\n");
    // play_syncword();
    ztimer_stopwatch_reset(&stopwatch);
    executeQueries(&msg, &env, &stack);
    exec_time_ms = ztimer_stopwatch_reset(&stopwatch);
  }
  LOG_INFO("Sending Responses if any...\n");

  // if we have responses send them. if not, send heartbeat to make sure we get responses
  if ( out.responses_count > 0)
  {
    network_send_message(&out);
  }
  else
  {
    network_send_heartbeat();
  }
  send_time_ms = ztimer_stopwatch_reset(&stopwatch);
  //check for new messages
  network_get_message(config.message, &config.message_size);

  // figure out how long the iteration took and sleep for the remaining time
  int sleep_time_ms_tmp = timeout_ms - (sync_word_time_ms + listen_time_ms + sensor_collect_time_ms + exec_time_ms + send_time_ms);
  uint32_t sleep_time_ms = MAX(sleep_time_ms_tmp, 0);
  // convert sleep_time_ms to second
  sleep_time_s = sleep_time_ms / 1000;
  LOG_INFO("Done with everything! saving config!\n");
  ++config.loop_counter;

}

void disable_peripherals(void)
{
    //i2c_release(I2C_DEV(0));
    
    // spi should automatically be disabled whenever it is not used
    
    uart_poweroff(UART_DEV(0));
    // TODO: figure out how to handle sensors
    // TODO: possibly also keep lora network stack turned off until actual usage
}


static void _rtc_alarm(void* arg)
{
    (void) arg;
    pm_reboot();
    return;
}

/**
 * @brief Main function. Initializes everything and runs the main loop
 *
 */
int main(void)
{
  struct tm time;
  
  ztimer_acquire(ZTIMER_MSEC);
  ztimer_stopwatch_init(ZTIMER_MSEC, &stopwatch);
  ztimer_stopwatch_start(&stopwatch);
  play_single_blink();
  play_single_blink();
  sync_word_time_ms = ztimer_stopwatch_reset(&stopwatch);
  startup();
  run_activities();
  teardown();
  ztimer_release(ZTIMER_MSEC);

  LOG_INFO(
      "TIMINGS> Loop: %" PRIi32 ", "
      "Sync: %" PRIi32 " ms, "
      "Load: %" PRIi32 " ms, "
      "sensor init: %" PRIi32 " ms, "
      "env init: %" PRIi32 " ms, "
      "net init: %" PRIi32 " ms, "
      "Collect: %" PRIi32 " ms, "
      "Exec: %" PRIi32 " ms, "
      "Send: %" PRIi32 " ms, "
      "save config: %" PRIi32 " ms, "
      "Sleep: %" PRIu32 " s\n",
      config.loop_counter-1, //since we already incremented the counter in run_activities
      sync_word_time_ms,
      conf_load_time_ms,
      sensor_init_time_ms,
      env_init_time_ms,
      net_init_time_ms,
      sensor_collect_time_ms,
      exec_time_ms,
      send_time_ms,
      conf_save_time_ms,
      sleep_time_s
      );

  puts("sleeping");
  disable_peripherals();
  rtc_get_time(&time);
  time.tm_sec += sleep_time_s;
  rtc_set_alarm(&time, _rtc_alarm, NULL);
  pm_set(STM32_PM_STANDBY);
  puts("sleeping failed");

  return 0;
}
#endif
