#include "operators.h"
#include <stdbool.h>

bool network_initialize_network(void);

bool network_is_connected(void);

//bool network_has_valid_message(void);

bool network_get_message(TerraProtocol_Message*);
bool network_send_message(TerraProtocol_Output*);
bool network_send_heartbeat(void);

