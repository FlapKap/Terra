#include "periph/gpio.h"
#include "periph/uart.h"
#include "cpu_conf.h"
#include "dht.h"
#define DHT_PARAM_PIN GPIO13
#define DHT_PARAM_TYPE AM2301
#include "dht_params.h"
#include <errno.h>
#include <stdio.h>
#include "ztimer.h"
#include <time_units.h>
#include "sps30.h"
#include "sps30_params.h"

#include "scd30.h"
#include "scd30_params.h"
#include "scd30_internal.h"

// #define SDS011_PARAM_UART_DEV UART_DEV(1)
// #include "sds011.h"
// #include "sds011_params.h"

// static void _print_measurement(sds011_data_t *data)
// {
//     uint16_t pm10_ug_int = data->pm_10 / 10;
//     uint16_t pm10_ug_dec = data->pm_10 - 10 * pm10_ug_int;
//     uint16_t pm2_5_ug_int = data->pm_2_5 / 10;
//     uint16_t pm2_5_ug_dec = data->pm_2_5 - 10 * pm2_5_ug_int;
//     printf("==> PM2.5: %d.%0d ug/m^3 | PM10: %d.%0d ug/m^3\n",
//            pm2_5_ug_int, pm2_5_ug_dec, pm10_ug_int, pm10_ug_dec);
// }
#define SENSOR_RESET_DELAY_MS       (10 * MS_PER_SEC)
#define TYPE_MC_STR  "MC PM"
#define TYPE_NC_STR  "NC PM"
#define TYPE_TPS_STR "TPS"
#define MC_UNIT_STR  "[µg/m³]"
#define NC_UNIT_STR  "[#/cm³]"
#define TPS_UNIT_STR "[µm]"
/* prints the result of an operation and returns true if an error occurred */
static bool _print_error(const char *msg, sps30_error_code_t ec)
{
    printf("sps30_%s: [%s]\n", msg, (ec == SPS30_OK) ? "OK" :
                              (ec == SPS30_CRC_ERROR ? "CRC_ERROR"
                                                     : "I2C_ERROR"));
    return ec != SPS30_OK;
}

static void _print_val_row(char *typ1, char *typ2, char *unit, float val)
{
    printf("| %-5s %4s:%3"PRIu32".%03"PRIu32" %-8s |\n", typ1, typ2,
           (uint32_t)val, ((uint32_t)((val + 0.0005) * 1000)) % 1000, unit);
}

sps30_t dev;
sps30_data_t data;
sps30_error_code_t ec;
char str[SPS30_SER_ART_LEN];
// uint32_t ci = 0;   /* clean interval */
// uint32_t nci = 0;  /* new clean interval */
bool error = false;

scd30_t scd30_dev;
scd30_params_t params = SCD30_PARAMS;
scd30_measurement_t result;
uint16_t value = 0;
int main(void)
{



    ec = sps30_init(&dev, &sps30_params[0]);
    error |= _print_error("init", ec);

    ec = sps30_read_article_code(&dev, str, sizeof(str));
    if (ec == SPS30_OK) {
        printf("Article code: %s\n", str);
    } else {
        error |= _print_error("read_article_code", ec);
    }

    ec = sps30_read_serial_number(&dev, str, sizeof(str));
    if (ec == SPS30_OK) {
        printf("Serial: %s\n", str);
    } else {
        error |= _print_error("read_serial_number", ec);
    }

    ec = sps30_start_fan_clean(&dev);
    error |= _print_error("start_fan_clean", ec);

    /* wait long enough for the fan clean to be done and the fan to settle */
    ztimer_sleep(ZTIMER_MSEC, 2 * SPS30_FAN_CLEAN_S);

    // /* read the currently set value from the sensor */
    // ec = sps30_read_ac_interval(&dev, &ci);
    // error |= _print_error("read_ac_interval", ec);

    // nci = ci + 1;
    // ec = sps30_write_ac_interval(&dev, nci);
    // error |= _print_error("write_ac_interval", ec);

    // /* resetting the sensor so the updated value can be read */
    // ec = sps30_reset(&dev);
    // error |= _print_error("reset", ec);

    // ztimer_sleep(ZTIMER_MSEC, SENSOR_RESET_DELAY_MS);

    scd30_init(&scd30_dev, &params);
    scd30_get_param(&scd30_dev, SCD30_INTERVAL, &value);
    printf("[test] Interval: %u s\n", value);
    scd30_get_param(&scd30_dev, SCD30_T_OFFSET, &value);
    printf("[test] Temperature Offset: %u.%02u C\n", value / 100u,
           value % 100u);
    scd30_get_param(&scd30_dev, SCD30_A_OFFSET, &value);
    printf("[test] Altitude Compensation: %u m\n", value);
    scd30_get_param(&scd30_dev, SCD30_ASC, &value);
    printf("[test] ASC: %u\n", value);
    scd30_get_param(&scd30_dev, SCD30_FRC, &value);
    printf("[test]FRC: %u ppm\n", value);
    while (1)
    {
        scd30_read_triggered(&scd30_dev, &result);
        printf(
            "[scd30_] Triggered measurements co2: %.02fppm,"
            " temp: %.02f°C, hum: %.02f%%. \n", result.co2_concentration,
            result.temperature, result.relative_humidity);

        int err_code;
        bool ready = sps30_data_ready(&dev, &err_code);

        if (!ready) {
            if (err_code != SPS30_OK) {
                error |= _print_error("data_ready", err_code);
            }
            /* try again after some time */
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        ec = sps30_read_measurement(&dev, &data);

        if (ec == SPS30_OK) {
            puts("\nv==== SPS30 measurements ====v");
            _print_val_row(TYPE_MC_STR, "1.0", MC_UNIT_STR, data.mc_pm1);
            _print_val_row(TYPE_MC_STR, "2.5", MC_UNIT_STR, data.mc_pm2_5);
            _print_val_row(TYPE_MC_STR, "4.0", MC_UNIT_STR, data.mc_pm4);
            _print_val_row(TYPE_MC_STR, "10.0", MC_UNIT_STR, data.mc_pm10);
            _print_val_row(TYPE_NC_STR, "0.5", NC_UNIT_STR, data.nc_pm0_5);
            _print_val_row(TYPE_NC_STR, "1.0", NC_UNIT_STR, data.nc_pm1);
            _print_val_row(TYPE_NC_STR, "2.5", NC_UNIT_STR, data.nc_pm2_5);
            _print_val_row(TYPE_NC_STR, "4.0", NC_UNIT_STR, data.nc_pm4);
            _print_val_row(TYPE_NC_STR, "10.0", NC_UNIT_STR, data.nc_pm10);
            _print_val_row(TYPE_TPS_STR, "", TPS_UNIT_STR, data.ps);
            puts("+----------------------------+");
            puts("| MC:  Mass Concentration    |");
            puts("| NC:  Number Concentration  |");
            puts("| TPS: Typical Particle Size |");
        } else {
            error |= _print_error("read_measurement", ec);
        }
    }
    

    // dht_t dht;
    // int res = dht_init(&dht, &dht_params[0]);
    // int16_t temp;
    // int16_t hum;
    // switch (res)
    // {
    // case 0:
    //     puts("DHT initialized");
    //     break;
    // case -EXDEV:
    //     puts("-EXDEV");
    //     break;
    // case -ENODEV:
    //     puts("-ENODEV");
    //     break;
    // default:
    //     break;
    // }

    // sds011_t sds011;
    // if (sds011_init(&sds011, &sds011_params[0]) == SDS011_OK)
    // {
    //     puts("init [OK]");
    // }
    // else
    // {
    //     puts("initialization [ERROR]");
    //     return -1;
    // }
    // puts("setting reporting mode to 'QUERY'...");
    // while (sds011_set_reporting_mode(&sds011, SDS011_RMODE_QUERY) != SDS011_OK)
    // {
    //     ztimer_sleep(ZTIMER_MSEC, 1000);
    //     puts("[RETRY]");
    // }

    // puts("setting working mode to 'WORK'...");
    // sds011_set_working_mode(&sds011, SDS011_WMODE_WORK);
    // sds011_data_t data;

    while (1)
    {
        ztimer_sleep(ZTIMER_MSEC, 1000);

        // if (dht_read(&dht, &temp, &hum) != DHT_OK)
        // {
        //     puts("Error reading values");
        //     continue;
        // }
        // if (sds011_read(&sds011, &data) == SDS011_OK)
        // {
        //     _print_measurement(&data);
        // }
        // else
        // {
        //     puts("reading of sds011 failed");
        // }

        // printf("DHT values - temp: %d.%d°C - relative humidity: %d.%d%%\n",
        //        temp / 10, temp % 10, hum / 10, hum % 10);
    }
}