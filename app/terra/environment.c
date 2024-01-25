#include <stdlib.h>

#include "environment.h"
#include "print_utils.h"
typedef struct _EnvUnit
{
    Number number;
    bool valid;
} EnvEntry;


static EnvEntry envMemory[ENVIRONMENT_LEN];


void env_clear_env(void)
{
    for (int i = 0; i < ENVIRONMENT_LEN; i++)
    {
        envMemory[i].valid = false;
    }
}

bool env_get_value(int index, Number* out)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    if (!envMemory[index].valid){
        return false;
    }
    else {
        *out = envMemory[index].number;
    }
    return true;
}

void env_set_value(int index, Number val)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    envMemory[index].number = val;
    envMemory[index].valid = true;
}

void env_print_env(void){
    printf("Environment (size: %d): \n", ENVIRONMENT_LEN);
    for (int i = 0; i < ENVIRONMENT_LEN; i++)
    {
        if (envMemory[i].valid)
        {
            printf("  %d: ", i);
            print_number_value_and_ucase(envMemory[i].number);
        }
    }
}