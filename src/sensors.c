#include "sensors.h"
#include "environment.h"
#include "stdio.h"
#include "math.h"
#include "log.h"
#define ENABLE_DEBUG 0
#include "debug.h"


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
    LOG_INFO("Initializing enabled sensors...");
    for (size_t i = 0; i < SENSORS_ARRAY_LENGTH; i++)
    {
        sensors[i] = saul_reg_find_type_and_name(sensor_types[i], sensor_names[i]);
        assert(sensors[i] != NULL);
        LOG_INFO("Sensor: %s, type: %d initialised", sensors[i]->name, sensors[i]->driver->type);
    }

    return true;
}

bool sensors_collect_into_env(Env *env)
{
    for (size_t i = 0; i < SENSORS_ARRAY_LENGTH; i++)
    {
        LOG_INFO("collecting data from sensor %s\n", sensors[i]->name);
        phydat_t data;
        Number num;
        //DEBUG("is the function pointer null: %d", sensors[i]->driver->read == NULL);
        sensors[i]->driver->read(sensors[i]->dev, &data);
        double val = data.val[0] * 1 / pow(10, data.scale);
        if (ceil(val) == val)
        {
            if (val > 0)
            {
                num.type._uint32 = (uint32_t)val;
                num.unionCase = NUMBER_UINT32;
                DEBUG("Read value %lu from Sensor %s. Saving in env position %d\n", (uint32_t)val, sensors[i]->name, i);
            }
            else
            {
                num.type._int = (int)val;
                num.unionCase = NUMBER_INT;
                DEBUG("Read value %d from Sensor %s. Saving in env position %d\n", (int)val, sensors[i]->name, i);
            }
        }
        else
        {
            // check if we can store the double as a float
            float tmp = (float)val;
            if (tmp == val)
            {
                // we can tore it as a float
                num.type._float = val;
                num.unionCase = NUMBER_FLOAT;
            }
            else
            {
                num.type._double = val;
                num.unionCase = NUMBER_DOUBLE;
            }
            DEBUG("Read value %f from Sensor %s. Saving in env position %d\n", val, sensors[i]->name, i);
        }

        set_value(env, i, num);
    }
    return true;
}
