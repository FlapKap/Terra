
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
#include <container.h>

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
#include "serialization.h"

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

// tflite
#if MODULE_TFLITE_MODEL
#include "tflite_model/tflite_model.h"
#endif

// Testing
#ifdef APPLICATION_RUN_TEST
// #include "embUnit.h"

// #include "expression_tests.h"
// #include "configuration_tests.h"
// void test_encode_input(void);
// void test_encode_output(void);

// int main(void)
// {
//   //ztimer_sleep(ZTIMER_MSEC, 1000); // wait one second before starting
//   puts("Start tests");
//   TESTS_START();
//   TESTS_RUN(tests_expression());
//   TESTS_RUN(tests_configuration());
//   TESTS_END();

//   return 0;
// }
#else
static TerraProtocol_Output out = TerraProtocol_Output_init_zero;
static TerraProtocol_Message msg;
static TerraConfiguration config = {
    .raw_message_size = 0,
    .raw_message_buffer = {0},
    .loop_counter = 0};

static bool raw_message_changed = false;

// tflite model memory
#if MODULE_TFLITE_MODEL
static Number tflite_output[1];
#endif

static Number sensor_reads[SENSORS_ARRAY_LENGTH];
static Stack stack = {
    .stack_memory = { {} },
    .top = -1
};

static const uint32_t timeout_ms = EXECUTION_EPOCH_S * 1000;
static int32_t sleep_time_s = 0;
// tracking stuff
static ztimer_stopwatch_t stopwatch = {0};

// timing measurements
static int32_t conf_load_time_ms = -1;
static int32_t deserialize_msg_ms = -1;
static int32_t sensor_init_time_ms = -1;
static int32_t net_init_time_ms = -1;
static int32_t sensor_collect_time_ms = -1;
static int32_t exec_time_ms = -1;
static int32_t sync_word_time_ms = -1;
static int32_t listen_time_ms = -1;
static int32_t send_time_ms = -1;
static int32_t conf_save_time_ms = -1;

static void startup(void)
{
  // CONFIGURATION LOAD
  ztimer_stopwatch_reset(&stopwatch);
  print_build_info();
  print_device_info();
  configuration_load(&config, &loramac);

  conf_load_time_ms = ztimer_stopwatch_reset(&stopwatch);
  
  // DESERIALIZE MESSAGE
  LOG_INFO("Deserialize message if any...\n");
  if (config.raw_message_size > 0)
  {
    LOG_INFO("Message there! Deserializing...\n");
    bool res = serialization_deserialize_message(config.raw_message_buffer, config.raw_message_size, &msg);
    if (!res)
    {
      LOG_ERROR("Failed to deserialize\n");
    }
  }

  deserialize_msg_ms = ztimer_stopwatch_reset(&stopwatch);

  // SENSOR INITIALIZATION
  sensors_print_available();
  sensors_initialize_enabled();
  sensor_init_time_ms = ztimer_stopwatch_reset(&stopwatch);
  LOG_INFO("enabled sensors\n");
  sensors_print_enabled();

  // NETWORK INITIALIZATION
  network_initialize_network();
  LOG_INFO("network initialized\n");
  net_init_time_ms = ztimer_stopwatch_reset(&stopwatch);

  LOG_INFO("Startup complete. Loaded config\n");
  print_configuration(&config);
  // network_send_heartbeat();
}

static void run_activities(void)
{
  LOG_INFO("Running activities...\n");

  LOG_INFO("if any queries, or tflite model, collect measurements...\n");
  if (msg.queries_count > 0)
  {
    LOG_INFO("queries there!");
  }
  if (IS_ACTIVE(MODULE_TFLITE_MODEL))
  {
    LOG_INFO("tflite model there!");
  }

  if (msg.queries_count > 0 || IS_ACTIVE(MODULE_TFLITE_MODEL))
  {
    // Collect measurements
    LOG_INFO("collecting measurements...\n");
    sensors_collect_into_array(sensor_reads, ARRAY_SIZE(sensor_reads));
    sensor_collect_time_ms = ztimer_stopwatch_reset(&stopwatch);
  }
  // Execute tflite model if there
  if (IS_ACTIVE(MODULE_TFLITE_MODEL))
  {
      // TFLITE MODEL INITIALIZATION
    LOG_INFO("Initialize TFLITE model...\n");
    if (!tflite_model_init())
    {
      LOG_DEBUG("Failed to initialize TFLITE model\n");
    }
    LOG_DEBUG("Running TFLITE model...\n");
    tflite_model_run(sensor_reads, ARRAY_SIZE(sensor_reads), tflite_output, ARRAY_SIZE(tflite_output));
    for (size_t j = ARRAY_SIZE(sensor_reads); j < ARRAY_SIZE(sensor_reads) + ARRAY_SIZE(tflite_output); j++)
    {
      env_set_value(j, tflite_output[j - ARRAY_SIZE(sensor_reads)]);
    }
  }

  if (msg.queries_count > 0)
  {
    // Execute queries
    LOG_INFO("Execute Queries...\n");
    // play_syncword();

    // for each query
    // 1. clear stack
    // 2. clear env and copy sensor values into env
    // 3. execute
    // 4. copy values from env into output
    int8_t response_id = 0;
    for (int8_t query_id = 0; query_id < msg.queries_count; query_id++)
    {
      // 1.
      stack_clear_stack(&stack);
      // 2.
      env_clear_env();
      for (uint8_t j = 0; j < ARRAY_SIZE(sensor_reads); j++)
      {
        env_set_value(j, sensor_reads[j]);
      }
      // 3.
      bool finished = executeQuery(&msg.queries[query_id], &stack);
      printf("finished: %s\n", finished ? "true" : "false");
      if (finished)
      {
        // get first free response field from output.
        TerraProtocol_Output_QueryResponse resp = out.responses[response_id];
        resp.id = query_id;

        // 4. for each env value copy into response
        for (int8_t env_idx = 0; env_idx < ENVIRONMENT_LEN; env_idx++)
        {
          Number num;
          if (env_get_value(env_idx, &num))
          {
            copy_number_to_instruction(&num, &(resp.response[resp.response_count]));
            ++resp.response_count;
          }
        }
        // increment response count and id
        out.responses_count++;
        response_id++;
      }
    }
  }
  exec_time_ms = ztimer_stopwatch_reset(&stopwatch);

  LOG_INFO("Sending Responses if any...\n");

  // if we have responses send them. if not, send heartbeat to make sure we have an opportunity to receive new messages
  if (out.responses_count > 0)
  {
    LOG_INFO("Sending Responses...\n");
    uint8_t buffer[LORAWAN_APP_DATA_MAX_SIZE] = {0};
    size_t bytes_written;
    serialization_serialize_output(&out, buffer, sizeof(buffer), &bytes_written);
    network_send_message(buffer, bytes_written);
  }
  // if no responses, send heartbeat if we haven't sent one for n loops.
  else if (config.loop_counter % FORCED_LISTEN_EVERY_N_LOOP == 0)
  {
    network_send_heartbeat();

    thread_yield_higher();
    ztimer_sleep(ZTIMER_MSEC, 2000);
    // check for new messages
    raw_message_changed = network_get_message(config.raw_message_buffer, sizeof(config.raw_message_buffer), &(config.raw_message_size));
    if (raw_message_changed)
    {
      LOG_INFO("Received message!\n");
    }
    
  }
  send_time_ms = ztimer_stopwatch_reset(&stopwatch);  
  
  // figure out how long the iteration took and sleep for the remaining time
  // Note: since the default values are negative, they might subtract from the total if not set. however -1 ms is negligible so it is ignored
  int sleep_time_ms_tmp = timeout_ms - (sync_word_time_ms + listen_time_ms + sensor_collect_time_ms + exec_time_ms + send_time_ms);
  uint32_t sleep_time_ms = MAX(sleep_time_ms_tmp, 0);
  // convert sleep_time_ms to second
  sleep_time_s = sleep_time_ms / 1000;
  LOG_INFO("Done with everything\n");
  ++config.loop_counter;
}

static void teardown(void)
{
  LOG_INFO("teardown\n");
  ztimer_stopwatch_reset(&stopwatch);
  LOG_INFO("saving config\n");
  configuration_save(&config, &loramac, raw_message_changed);
  conf_save_time_ms = ztimer_stopwatch_reset(&stopwatch);
}

static void disable_peripherals(void)
{
  // i2c_release(I2C_DEV(0));

  // spi should automatically be disabled whenever it is not used

  uart_poweroff(UART_DEV(0));
  // TODO: figure out how to handle sensors
  // TODO: possibly also keep lora network stack turned off until actual usage
}

static void _rtc_alarm(void *arg)
{
  (void)arg;
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
      "deserialize: %" PRIi32 " ms, "
      "sensor init: %" PRIi32 " ms, "
      "net init: %" PRIi32 " ms, "
      "Collect: %" PRIi32 " ms, "
      "Exec: %" PRIi32 " ms, "
      "Send: %" PRIi32 " ms, "
      "save config: %" PRIi32 " ms, "
      "Sleep: %" PRIi32 " s\n",
      config.loop_counter - 1, // since we already incremented the counter in run_activities
      sync_word_time_ms,
      conf_load_time_ms,
      deserialize_msg_ms,
      sensor_init_time_ms,
      net_init_time_ms,
      sensor_collect_time_ms,
      exec_time_ms,
      send_time_ms,
      conf_save_time_ms,
      sleep_time_s);

  puts("sleeping");
  disable_peripherals();
  rtc_get_time(&time);
  time.tm_sec += sleep_time_s;
  rtc_set_alarm(&time, _rtc_alarm, NULL);
#ifdef CPU_ESP32
  pm_set(ESP_PM_DEEP_SLEEP);
#else
  pm_set(0);
#endif
  puts("sleeping failed");

  return 0;
}
#endif
