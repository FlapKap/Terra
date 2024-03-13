#include <stdio.h>
#include "periph/rtc.h"
#include "periph/rtc_mem.h"
#include "periph/pm.h"
#include <inttypes.h>

static void reboot(void* arg){
    (void) arg;
    puts("Rebooting...\n");
    pm_reboot();
}

int main(void)
{
    printf("Hello There! Build time: %s\n",__TIME__);
    rtc_init();
    size_t size = rtc_mem_size();
    printf("rtc_mem size: %" PRIu16 "\n", size);
    printf("Checking rtc_mem contents just after reboot...\n");

    // check if number saved in the register matches the register index itself
    uint8_t val;
    for (size_t i = 2; i < size; i++)
    {
        rtc_mem_read(i, &val, sizeof(val));
        printf("val at index %d: %d \n", i, val);
    }
    printf("--------------------------------------\n");

    printf("Writing to rtc_mem...\n");
    //save the index in the registers
    for (uint8_t i = 2; i < size; i++)
    {
        printf("Writing val %d at index %d \n", i, i);
        rtc_mem_write(i, &i, sizeof(i));
    }
    
    printf("--------------------------------------\n");
    printf("Checking rtc_mem contents after writing...\n");
    // check if number saved in the register matches the register index itself
    for (size_t i = 2; i < size; i++)
    {
        rtc_mem_read(i, &val, sizeof(val));
        printf("val at index %d: %d \n", i, val);
    }

    //set an alarm
    struct tm time = { 0 };
    rtc_get_time(&time);
    time.tm_sec += 10;
    rtc_set_alarm(&time, &reboot, NULL);
    printf("Going to sleep...\n");
    pm_set(STM32_PM_STANDBY);
    printf("this should not be reached\n");
    return 0;
}