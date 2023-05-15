#ifndef PROTOCOLTESTS_H
#define PROTOCOLTESTS_H

#include "testType.h"

TestResult runProtocolTests(void);
// Global

Test data_is_initialised(void);

// Encode output tests
Test query_is_initialised(void);
Test output_is_initialised(void);
Test output_messae_gets_encoded(void);
Test output_message_gets_decoded(void);

// Encode/decode input tests
Test message_is_initialised(void);
Test input_query_is_initialised(void);
Test operation_is_initialised(void);
Test window_operation_is_initialised(void);
Test filter_operation_is_initialised(void);
Test map_operation_is_initialised(void);
Test message_gets_encoded(void);
Test message_gets_decoded(void);

// Original tests
Test og_decode_map(void);
Test og_test_filter(void);
Test og_test_map_filter_msg(void);
Test og_window_message(void);
Test og_multiple_queries(void);
Test og_output_single_response(void);
Test og_output_multiple_response(void);
#endif // PROTOCOLTESTS_H