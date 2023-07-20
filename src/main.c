#define LOG_LEVEL LOG_DEBUG
// Standard library includes
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <kernel_defines.h>
#include <fmt.h>
 
// Local includes
#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "execution.h"
#include "operators.h"
#include "number.h"
#include "encodeInput.h"
#include "encodeOutput.h"
#include "print_utils.h"

#include <time.h>

#if IS_USED(MODULE_AUTO_INIT_LORAMAC)
#include "lora.h"
#include "semtech_loramac.h"
#endif

// Testing
#ifdef APPLICATION_RUN_TEST
#include "embUnit.h"
#include "protocol_tests.h"
#include "expression_tests.h"
#endif

// RIOT includes
#include "EndDeviceProtocol.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "ztimer.h"

// Power tracking
#include "power_sync.h"
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
#elif IS_USED(MODULE_AUTO_INIT_LORAMAC) // we have to have lorawan enabled for the main to run
extern semtech_loramac_t loramac;

static bool receive_and_decode(Message *msg)
{
  // Check for received messages
  pb_istream_t istream = pb_istream_from_buffer(loramac.rx_data.payload, loramac.rx_data.payload_len);
  return decode_input_message(&istream, msg);
}

Message msg;
bool valid_msg = false;
int main(void)
{
  run_sync();
  ztimer_sleep(ZTIMER_SEC, 3);
  puts("NebulaStream End Device Runtime");
  puts("=====================================");
  puts("initializing lorawan");
  init_lorawan();
  puts("LoRaWAN Info:");
  //fputs to avoid newlines in the end
  fputs("  Device EUI: ", stdout);
  print_bytes_hex(loramac.deveui, LORAMAC_DEVEUI_LEN);
  fputs("\n  Application EUI: ", stdout);
  print_bytes_hex(loramac.appeui, LORAMAC_APPEUI_LEN);
  fputs("\n  Application Key: ", stdout);
  print_bytes_hex(loramac.appkey, LORAMAC_APPKEY_LEN);
  puts("\n");

  // Run Tests (Only on native)

  // Connect lorawan and receive first message
  connect_lorawan();

  // Initialize global variable environment
  Env *global_env = init_env();

  // main loop
  while (1)
  {
    puts("Main loop iteration");
    //ztimer_sleep(ZTIMER_SEC, 5);
    if (!valid_msg)
    {
      printf("no valid message received\n");
      // if not a valid query
      //  send message to receive
      uint8_t tmp[1];
      puts("Main loop: sending message");
      send_message(tmp, (uint8_t)1);
      Message msg_tmp;
      if (receive_and_decode(&msg_tmp))
      {
        printf("received message\n");
        
        if (msg_tmp.amount > 0){
          valid_msg = true;
          msg = msg_tmp;
          printMessage(&msg);
        }
        }
      }
      else
      {
        printf("couldn't decode message. Ignoring...\n");
      }
    }
    
    if (valid_msg)
    {
      printf("message is valid. Running queries...\n");

      // Execute queries
      OutputMessage out;
      executeQueries(msg, &out, global_env);

      if (out.amount > 0)
      {
        uint8_t buffer[256];
        pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        encode_output_message(&ostream, &out);
        send_message(buffer, (uint8_t)256);
        Message msg_tmp;
        // check if there is new messages
        if (receive_and_decode(&msg_tmp))
        {
        if (msg_tmp.amount > 0){
          valid_msg = true;
          msg = msg_tmp;
          printMessage(&msg);
        }
      }
    }
    ztimer_sleep(ZTIMER_SEC, 5);
  }
  return 0;
}
#endif
