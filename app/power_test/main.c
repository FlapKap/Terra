#include <stdio.h>
#include <periph/rtc.h>
#include <periph/pm.h>
#include <periph/uart.h>
#include <led.h>
#include "stm32_leds.h"
static void _rtc_alarm(void* arg)
{
    (void) arg;
    puts("rebooting...");
    pm_reboot();
    return;
}

int main(void)
{
    LED0_OFF;
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
    LED5_OFF;
    LED6_OFF;
    LED7_OFF;
    struct tm time;
    while (1)
    {
        puts("Hello World!");
        puts(__TIME__);
        rtc_get_time(&time);
        time.tm_sec += 5;
        rtc_set_alarm(&time, _rtc_alarm, NULL);
        uart_poweroff(UART_DEV(0));
        pm_set(STM32_PM_STANDBY);
    }
        
        puts("sleeping failed");

    }
    return 0;
}