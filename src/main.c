
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
#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "execution.h"
#include "operators.h"
#include "number.h"
#include "network.h"
#include "sensors.h"
#include "print_utils.h"
#include "configuration.h"

// RIOT includes
#include "pm_layered.h"
#include "ztimer.h"
#include "ztimer/stopwatch.h"
// Power tracking
#include "power_sync.h"

#include "log.h"

// Testing
#ifdef APPLICATION_RUN_TEST
#include "embUnit.h"
//#include "protocol_tests.h"
#include "expression_tests.h"
void test_encode_input(void);
void test_encode_output(void);

int main(void)
{
  //ztimer_sleep(ZTIMER_MSEC, 1000); // wait one second before starting
  puts("Start tests");
  TESTS_START();
  //TESTS_RUN(tests_protocol());
  TESTS_RUN(tests_expression());
  TESTS_END();

  return 0;
}
#else
static TerraProtocol_Message msg = TerraProtocol_Message_init_zero;
static TerraProtocol_Output out = TerraProtocol_Output_init_zero;
static TerraConfiguration config = {
    .loop_counter = 0,
    .query = &msg,
    .loramac = &loramac
};


//static bool valid_msg = false;

static Number stack_memory[20];
static Stack stack = { 0 };
static Number env_memory[20];
static Env env = { 0 };

// tracking stuff
ztimer_stopwatch_t stopwatch = { 0 };
ztimer_stopwatch_t loop_stopwatch = { 0 };
uint32_t loop_counter = 0;

/**
 * @brief Main function. Initializes everything and runs the main loop
 *
 */
int main(void)
{
  ztimer_stopwatch_init(ZTIMER_MSEC, &stopwatch);
  ztimer_stopwatch_init(ZTIMER_MSEC, &loop_stopwatch);
  
  print_build_info();
  print_device_info();



  ztimer_stopwatch_start(&stopwatch);
  sensors_initialize_enabled();
  uint32_t sensor_init_time = ztimer_stopwatch_measure(&stopwatch);

  LOG_INFO("enabled sensors\n");
  sensors_print_enabled();

  ztimer_stopwatch_reset(&stopwatch);
  // initialize env and stack
  env_init_env(env_memory, sizeof(Number)/sizeof(env_memory), &env);
  stack_init_stack(stack_memory, sizeof(Number)/sizeof(stack_memory), &stack);
  LOG_INFO("environment and stack initialized:\n");
  print_env(&env);
  print_stack(&stack);
  uint32_t env_init_time = ztimer_stopwatch_reset(&stopwatch);

  network_initialize_network();
  LOG_INFO("network initialized\n");
  uint32_t net_init_time = ztimer_stopwatch_reset(&stopwatch);
  network_send_heartbeat();
  // Initialize global variable environment
  LOG_DEBUG("Startup done. Timings: sensor init: %" PRIu32 " ms, env init: %" PRIu32 " ms, net init: %" PRIu32 " ms\n", sensor_init_time, env_init_time, net_init_time);

  // main loop
  const uint32_t timeout_ms = EXECUTION_EPOCH_S*1000;

  
  ztimer_stopwatch_start(&loop_stopwatch);
  configuration_save(&config);
  while (1)
  {
    LOG_INFO("Main loop iteration\n");
    ztimer_stopwatch_reset(&stopwatch);
    ztimer_stopwatch_reset(&loop_stopwatch);
    play_single_blink();
    uint32_t sync_word_time_ms = ztimer_stopwatch_reset(&stopwatch);
    network_get_message(&msg);
    uint32_t listen_time_ms = ztimer_stopwatch_reset(&stopwatch);
    // Collect measurements
    LOG_INFO("collecting measurements...\n");

    ztimer_stopwatch_reset(&stopwatch);
    sensors_collect_into_env(&env);
    uint32_t sensor_collect_time_ms = ztimer_stopwatch_reset(&stopwatch);

    // Execute queries
    LOG_INFO("Execute Queries...\n");
    // play_syncword();
    ztimer_stopwatch_reset(&stopwatch);
    executeQueries(&msg, &env, &stack);
    uint32_t exec_time_ms = ztimer_stopwatch_reset(&stopwatch);
    
    LOG_INFO("Sending Responses if any...\n");
    if ( out.responses_count > 0)
    {
      network_send_message(&out);
    }
    else
    {
      network_send_heartbeat();
    }
    uint32_t send_time_ms = ztimer_stopwatch_reset(&stopwatch);
    // figure out how long the iteration took and sleep for the remaining time
    uint32_t end_time_ms = ztimer_stopwatch_reset(&loop_stopwatch);
    int sleep_time_ms_tmp = timeout_ms - end_time_ms;
    uint32_t sleep_time_ms = MAX(sleep_time_ms_tmp, 0);
    LOG_INFO("Done with everything! Playing sync_word!\n");
    play_single_blink();

    LOG_INFO(
        "TIMINGS> Loop: %" PRIu32 ", "
        "Sync: %" PRIu32 " ms, "
        "Listen: %" PRIu32 " ms, "
        "Collect: %" PRIu32 " ms, "
        "Exec: %" PRIu32 " ms, "
        "Send: %" PRIu32 " ms, "
        "Sleep: %" PRIu32 " ms, "
        "Total: %" PRIu32 " ms\n",
        loop_counter,
        sync_word_time_ms,
        listen_time_ms,
        sensor_collect_time_ms,
        exec_time_ms,
        send_time_ms,
        sleep_time_ms,
        end_time_ms
        );

    ztimer_sleep(ZTIMER_MSEC, sleep_time_ms);

    ++loop_counter;
  }
  return 0;
}
#endif
