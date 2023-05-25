#include "board.h"
#include "ztimer.h"
void run_sync(void){
    
    LED0_ON;
    ztimer_sleep(ZTIMER_MSEC,500);
    LED0_OFF;
    ztimer_sleep(ZTIMER_MSEC,500);
    LED0_ON;
    ztimer_sleep(ZTIMER_MSEC,500);
    LED0_OFF;
    ztimer_sleep(ZTIMER_MSEC,500);

}