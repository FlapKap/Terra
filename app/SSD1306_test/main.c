#include "u8g2.h"

#include "u8x8_riotos.h"

#include "periph/i2c.h"
#include <stdio.h>
u8g2_t u8g2;

u8x8_riotos_t user_data =
    {
        .device_index = 0,
        .pin_cs = GPIO_UNDEF,
        .pin_dc = GPIO_UNDEF,
        .pin_reset = GPIO_UNDEF};

int main(void)
{
    printf("set user data.\n");
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);
    u8g2_SetUserPtr(&u8g2, &user_data);
    u8g2_SetI2CAddress(&u8g2, 0x3c);
    printf("setup.\n");

    printf("init.\n");

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    printf("draw.\n");
    while (1)
    {
        u8g2_FirstPage(&u8g2);
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_SetFont(&u8g2, u8g2_font_helvB24_tf);
        do
        {
            u8g2_DrawStr(&u8g2, 22, 32, "BONK");

        } while (u8g2_NextPage(&u8g2));
    };

    return 0;
}