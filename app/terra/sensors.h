#ifndef __SENSORS_H
#define __SENSORS_H

#include "saul_reg.h"
#include "stdbool.h"
#include "assert.h"
#include "operators.h"
#include "number.h"
void sensors_print_available(void);
void sensors_print_enabled(void);
bool sensors_initialize_enabled(void);
bool sensors_deinitialize_enabled(void);
bool sensors_collect_into_array(Number*, size_t);

#endif
