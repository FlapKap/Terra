
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

#include "print_utils.h"




// Testing
#ifdef APPLICATION_RUN_TEST
#include "embUnit.h"
#include "protocol_tests.h"
#include "expression_tests.h"
#endif

// RIOT includes

#include "ztimer.h"

// Power tracking
#include "power_sync.h"

// Debug
#define ENABLE_DEBUG 1
#include "debug.h"

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



int main(void)
{
  run_barker11();
  puts("NebulaStream End Device Runtime");
  puts("=====================================");
  network_initialize_network();

  msg = network_get_message();
  // Initialize global variable environment
  Env *global_env = init_env();

  // main loop
  const uint32_t timeout_s = EXECUTION_INTERVAL_S;
  while (1)
  {
    puts("Main loop iteration");
    ztimer_now_t start_time = ztimer_now(ZTIMER_SEC);
    run_barker11();


    // Execute queries
    OutputMessage out;
    executeQueries(msg, &out, global_env);

    if (out.amount > 0)
    {
      network_send_message(out);
    }
    
    // figure out how long the iteration took and sleep for the remaining time
    ztimer_now_t end_time = ztimer_now(ZTIMER_SEC);
    DEBUG("Sleeping for %" PRIu32 " seconds\n", timeout_s - (end_time - start_time));
    ztimer_sleep(ZTIMER_SEC, MAX(timeout_s - (end_time - start_time), 0));
  }
  return 0;
}
#endif
