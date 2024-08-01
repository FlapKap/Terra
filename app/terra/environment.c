#include <stdlib.h>
#include <stdio.h>
#include "environment.h"
#include "print_utils.h"
typedef struct _EnvUnit
{
    Number number;
    bool fromQuery;
    bool fromSensor;
} EnvEntry;


static EnvEntry envMemory[ENVIRONMENT_LEN];


void env_clear_env(void)
{
    for (int i = 0; i < ENVIRONMENT_LEN; i++)
    {
        envMemory[i].number = (Number) {.type._uint32 = 0, .unionCase = NUMBER_UINT32};
        envMemory[i].fromQuery = false;
        envMemory[i].fromSensor = false;
    }
}

bool env_get_value(int index, Number* out)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    if (envMemory[index].fromQuery || envMemory[index].fromSensor){
        *out = envMemory[index].number;
        return true;
    }
    else {
        return false;
    }
}

bool env_get_query_value(int index, Number* out)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    if (envMemory[index].fromQuery){
        *out = envMemory[index].number;
        return true;
    }
    else {
        return false;
    }
}

void env_set_value(int index, const Number val)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    envMemory[index].number = val;
    envMemory[index].fromQuery = true;
}

void env_set_sensor_value(int index, const Number val)
{
    assert(index >= 0 && index < ENVIRONMENT_LEN);
    envMemory[index].number = val;
    envMemory[index].fromSensor = true;
}

void env_print_env(void){
    printf("Environment (size: %d): \n", ENVIRONMENT_LEN);
    for (int i = 0; i < ENVIRONMENT_LEN; i++)
    {
        if (envMemory[i].fromQuery || envMemory[i].fromSensor)
        {
            printf("  %d, source %s: ", i, envMemory[i].fromQuery ? "query" : "sensor");
            print_number_value_and_ucase(envMemory[i].number);
        }
    }
}