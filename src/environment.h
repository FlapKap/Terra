#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "number.h"
typedef struct _Env
{
    Number *memory;
    int size;
} Env;

void env_init_env(Number* memory, size_t memory_size, Env* out);
void env_clear_env(Env *env);
Number env_get_value(Env *env, int index);
void env_set_value(Env *env, int index, Number val);

#endif /* ENVIRONMENT_H */
