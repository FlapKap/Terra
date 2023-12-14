#include <stdio.h>
#include <periph/eeprom.h>

int main(void)
{
    printf("Hello There! Build time: %s\n",__TIME__);
    printf("erasing eeprom...\n");
    eeprom_erase();
    printf("done!\n");
    return 0;
}