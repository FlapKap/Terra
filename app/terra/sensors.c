#include <inttypes.h>
#include "sensors.h"
#include "environment.h"
#include "stdio.h"
#include "math.h"
#include "log.h"

#ifndef BOARD_NATIVE
extern saul_reg_t *saul_reg;
// apparently the sensors available are all populated in the registry. However since they are not standard part of the board, they need to be loaded through the module system
// here the connection might change from the default value

static const char *sensor_names[] = SENSORS_ARRAY;
static const uint8_t sensor_types[] = SENSORS_TYPES_ARRAY;

static saul_reg_t *sensors[SENSORS_ARRAY_LENGTH];


void sensors_print_available(void)
{
    assert(saul_reg != NULL);
    saul_reg_t *current = saul_reg;
    while (current != NULL)
    {
        printf("Sensor: %s, type: %d\n", current->name, current->driver->type);
        current = current->next;
    }
}

void sensors_print_enabled(void)
{
    for (size_t i = 0; i < SENSORS_ARRAY_LENGTH; i++)
    {
        printf("Sensor: %s, type: %d\n", sensors[i]->name, sensors[i]->driver->type);
    }
}

bool sensors_initialize_enabled(void)
{
    LOG_INFO("Initializing enabled sensors...\n");
    for (size_t i = 0; i < SENSORS_ARRAY_LENGTH; i++)
    {
        LOG_INFO("initialising sensor type %d, name %s...", sensor_types[i], sensor_names[i]);
        sensors[i] = saul_reg_find_type_and_name(sensor_types[i], sensor_names[i]);
        assert(sensors[i] != NULL);
        LOG_INFO(" initialised\n");
    }

    return true;
}

bool sensors_collect_into_array(Number* arr, size_t arr_size)
{
    assert(arr != NULL);
    assert(arr_size <= SENSORS_ARRAY_LENGTH);

    for (size_t i = 0; i < SENSORS_ARRAY_LENGTH; i++)
    {
        LOG_DEBUG("collecting data from sensor %s\n", sensors[i]->name);
        phydat_t data;
        //DEBUG("is the function pointer null: %d", sensors[i]->driver->read == NULL);
        sensors[i]->driver->read(sensors[i]->dev, &data);
        double val = data.val[0] * 1 / pow(10, data.scale);

        Number* num = &arr[i];
        if (ceil(val) == val)
        {
            if (val > 0)
            {
                num->type._uint32 = (uint32_t)val;
                num->unionCase = NUMBER_UINT32;
                LOG_DEBUG("Read value %" PRIu32 " from Sensor %s. Saving in array position %d\n", (uint32_t)val, sensors[i]->name, i);
            }
            else
            {
                num->type._int = (int)val;
                num->unionCase = NUMBER_INT32;
                LOG_DEBUG("Read value %d from Sensor %s. Saving in array position %d\n", (int)val, sensors[i]->name, i);
            }
        }
        else
        {
            // check if we can store the double as a float
            float tmp = (float)val;
            if (tmp == val)
            {
                // we can tore it as a float
                num->type._float = val;
                num->unionCase = NUMBER_FLOAT;
            }
            else
            {
                num->type._double = val;
                num->unionCase = NUMBER_DOUBLE;
            }
            LOG_DEBUG("Read value %f from Sensor %s. Saving in env position %d\n", val, sensors[i]->name, i);
        }
    }
    return true;
}

#else
void sensors_print_available(void)
{
    printf("Running in native: No sensors available\n");
}
void sensors_print_enabled(void)
{
    printf("Running in native: No sensors enabled\n");
}
bool sensors_initialize_enabled(void)
{
    printf("Running in native: No sensors initialized\n");
    return false;
}
bool sensors_collect_into_env(__attribute__((unused)) Env* env)
{
    
    printf("Running in native: No sensors collected\n");
    return false;
}
#endif
