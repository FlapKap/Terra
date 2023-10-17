
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
#ifndef BOARD_NATIVE
#include "sensors.h"
#endif
#include "print_utils.h"




// Testing
#ifdef APPLICATION_RUN_TEST
#include "embUnit.h"
#include "protocol_tests.h"
#include "expression_tests.h"
#endif

// RIOT includes

#include "ztimer.h"
#include "ztimer/stopwatch.h"
// Power tracking
#include "power_sync.h"

#include "log.h"


#ifdef APPLICATION_RUN_TEST
void test_encode_input(void);
void test_encode_output(void);

int main(void)
{
  ztimer_sleep(ZTIMER_SEC, 1); // wait one second before starting
  puts("Start tests");
  TESTS_START();
  TESTS_RUN(tests_protocol());
  TESTS_RUN(tests_expression());
  TESTS_END();

  return 0;
}
#else


Message msg;
bool valid_msg = false;

ztimer_stopwatch_t stopwatch;

int main(void)
{
  ztimer_stopwatch_init(ZTIMER_MSEC,&stopwatch);
  ztimer_stopwatch_start(&stopwatch);
  //play_syncword();
  uint32_t sync_word_time = ztimer_stopwatch_measure(&stopwatch);
  LOG_INFO("Terra (Build Date: %s, Time of Build: %s)\n", __DATE__, __TIME__);
  LOG_INFO("=====================================\n");
  
  ztimer_stopwatch_reset(&stopwatch);
  sensors_initialize_enabled();
  uint32_t sensor_init_time = ztimer_stopwatch_measure(&stopwatch);
  
  LOG_INFO("enabled sensors\n");
  sensors_print_enabled();

  ztimer_stopwatch_reset(&stopwatch); 
  Env *global_env = init_env();
  LOG_INFO("environment initialized\n");
  uint32_t env_init_time = ztimer_stopwatch_reset(&stopwatch);

  network_initialize_network();
  LOG_INFO("network initialized\n");
  uint32_t net_init_time = ztimer_stopwatch_reset(&stopwatch);
  network_send_heartbeat();
  // Initialize global variable environment
  LOG_DEBUG("initializating done. Timings: sync word: %" PRIu32 " ms, sensor init: %" PRIu32 " ms, env init: %" PRIu32 " ms, net init: %" PRIu32 " ms\n", sync_word_time, sensor_init_time, env_init_time, net_init_time);

  // main loop
  const int timeout_s = EXECUTION_EPOCH_S;
  ztimer_stopwatch_t loop_stopwatch;
  ztimer_stopwatch_init(ZTIMER_MSEC, &loop_stopwatch);
  ztimer_stopwatch_start(&loop_stopwatch);
  while (1)
  {
    LOG_INFO("Main loop iteration\n");
    ztimer_stopwatch_reset(&stopwatch);
    ztimer_stopwatch_reset(&loop_stopwatch);
    play_single_blink();
    uint32_t sync_word_time = ztimer_stopwatch_reset(&stopwatch);
    msg = network_get_message();
    uint32_t listen_time = ztimer_stopwatch_reset(&stopwatch);
    //Collect measurements
    LOG_INFO("collecting measurements...\n");

    ztimer_stopwatch_reset(&stopwatch);
    sensors_collect_into_env(global_env);
    uint32_t sensor_collect_time = ztimer_stopwatch_reset(&stopwatch);
    // Execute queries
    OutputMessage out;
    LOG_INFO("Execute Queries...\n");
    //play_syncword();
    ztimer_stopwatch_reset(&stopwatch);
    executeQueries(msg, &out, global_env);
    uint32_t exec_time = ztimer_stopwatch_reset(&stopwatch);
    if (out.amount > 0)
    {
      network_send_message(out);
    } else {
      network_send_heartbeat();
    }
    uint32_t send_time = ztimer_stopwatch_reset(&stopwatch);
    // figure out how long the iteration took and sleep for the remaining time
    uint32_t end_time = ztimer_stopwatch_reset(&loop_stopwatch);

    LOG_INFO("--Iteration stats--\n");
    LOG_INFO("Sync word time: %" PRIu32 " ms\n", sync_word_time);
    LOG_INFO("listen took %" PRIu32 " ms\n", listen_time);
    LOG_INFO("sensor collect took %" PRIu32 " ms\n", sensor_collect_time);
    LOG_INFO("Execution took %" PRIu32 " ms\n", exec_time);
    LOG_INFO("Send took %" PRIu32 " ms\n", send_time);
    LOG_INFO("Iteration took %" PRIu32 " ms\n", end_time);
    int sleep_time_tmp = timeout_s - (end_time / 1000);
    uint32_t sleep_time =  MAX(sleep_time_tmp, 0);
    LOG_INFO("Sleeping for %" PRIu32 " seconds\n", sleep_time);
    ztimer_sleep(ZTIMER_SEC, sleep_time);
  }
  return 0;
}
#endif
