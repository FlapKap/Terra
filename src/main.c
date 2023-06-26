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
#include "lora.h"
#include "semtech_loramac.h"

#include <time.h>

// Testing
#include "embUnit.h"
#include "protocol_tests.h"
#include "expression_tests.h"

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

bool running_query = false;

int main2(void)
{
  ztimer_sleep(ZTIMER_SEC, 1); // wait one second before starting

  TESTS_START();
  TESTS_RUN(tests_protocol());
  TESTS_RUN(tests_expression());
  TESTS_END();

  return 0;
}

int main(void)
{
  run_sync();
  ztimer_sleep(ZTIMER_SEC, 3);
  puts("NebulaStream End Device Runtime");
  puts("=====================================");

  // Run Tests (Only on native)

  // Connect lorawan and receive first message
  connect_lorawan();

  // Initialize global variable environment
  Env *global_env = init_env();

  bool valid_query = false;
  // main loop
  while (1)
  {
    puts("Main loop iteration");
    ztimer_sleep(ZTIMER_SEC, 5);

    if (valid_query)
    {
      // Check for received messages
      pb_istream_t istream = pb_istream_from_buffer(loramac.rx_data.payload, loramac.rx_data.payload_len);
      Message msg;
      bool status = decode_input_message(&istream, &msg);

      if (!status)
      {
        printf("couldn't decode\n");
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
        send_message(buffer, (uint8_t)256);
        // TODO: it never listens
      }
    } else { //if not a valid query
      // send message to receive
      uint8_t msg[1];
      if (send_message(msg, (uint8_t)1) != 0)
      {
        return -1;
      };
    }
  }

  return 0;
}
