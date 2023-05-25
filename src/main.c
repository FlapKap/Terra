#define LOG_LEVEL LOG_DEBUG
// Standard library includes
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <kernel_defines.h>

// Local includes
#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "execution.h"
#include "operators.h"
#include "number.h"
#include "encodeInput.h"
#include "encodeOutput.h"
#ifndef BOARD_NATIVE
#include "lora.h"
#include "semtech_loramac.h"
#endif

#include <time.h>

// Testing
#include "embUnit.h"
#include "protocol_tests.h"

// RIOT includes
#include "EndDeviceProtocol.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "ztimer.h"

// Power tracking
#include "power_sync.h"

void test_encode_input(void);
void test_encode_output(void);

#ifndef BOARD_NATIVE
extern semtech_loramac_t loramac;
#endif

int main(void)
{
  ztimer_sleep(ZTIMER_SEC, 1); // wait one second before starting
  run_sync();
  TESTS_START();
  TESTS_RUN(tests_protocol());
  TESTS_END();

  return 0;
}

int main2(void)
{
  ztimer_sleep(ZTIMER_SEC, 3);
  puts("NebulaStream End Device Runtime");
  puts("=====================================");

  // Run Tests (Only on native)
#if defined(BOARD_NATIVE)

#else
  // Connect lorawan and receive first message
  connect_lorawan();
#endif

#ifndef BOARD_NATIVE
  // Trigger first send
  uint8_t msg[1];
  send_message(msg, (uint8_t)1);
#endif

  // Initialize global variable environment
  Env *global_env = init_env();

  while (1)
  {
    puts("Main loop iteration");
    ztimer_sleep(ZTIMER_SEC, 5);

// Check for received messages
#ifndef BOARD_NATIVE
    pb_istream_t istream = pb_istream_from_buffer(loramac.rx_data.payload, loramac.rx_data.payload_len);
#else
    uint8_t buffer[1024];
    pb_istream_t istream = pb_istream_from_buffer(buffer, sizeof(buffer));
// Since nothing is in the bugger it cannot be decoded.
#endif
    Message msg;
    bool status = decode_input_message(&istream, &msg);

    if (!status)
    {
      printf("no message\n");
      continue;
    }

    // Execute queries
    OutputMessage out;
    executeQueries(msg, &out, global_env);

    if (out.amount > 0)
    {
      uint8_t buffer[256];
      pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
      encode_output_message(&ostream, &out);
#ifndef BOARD_NATIVE
      send_message(buffer, (uint8_t)256);
#endif
    }
  }

  return 0;
}
