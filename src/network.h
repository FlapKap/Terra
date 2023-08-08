#include "operators.h"
#include <stdbool.h>

bool network_initialize_network(void);

bool network_is_connected(void);

bool network_has_valid_message(void);

Message network_get_message(void);
bool network_send_message(OutputMessage msg);

