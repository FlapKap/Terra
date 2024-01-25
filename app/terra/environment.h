#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "number.h"

#ifndef ENVIRONMENT_LEN
#define ENVIRONMENT_LEN 16
#endif



void env_clear_env(void);
bool env_get_value(int index, Number* out);
void env_set_value(int index, Number val);
void env_print_env(void);
#endif /* ENVIRONMENT_H */
