#include <stdlib.h>

#include "environment.h"

void env_init_env(Number* memory, size_t memory_size, Env* out)
{
    out->memory = memory;
    out->size = memory_size;
    env_clear_env(out);
}

void env_clear_env(Env *env)
{
    for (int i = 0; i < env->size; i++)
    {
        Number number;
        number.type._uint32 = 0;
        number.unionCase = NUMBER_UINT32;
        env->memory[i] = number;
    }
}

Number env_get_value(Env *env, int index)
{
    return env->memory[index];
}

void env_set_value(Env *env, int index, Number val)
{
    env->memory[index] = val;
}
