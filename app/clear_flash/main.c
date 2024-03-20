#include <mtd_flashpage.h>
#include <mtd.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

#ifndef CONFIGURATION_MTD_DEVICE
#define CONFIGURATION_MTD_DEVICE mtd_aux
#endif

int main(void)
{
    printf("Hello There! Build time: %s\n",__TIME__);
    const uint32_t num_sectors = CONFIGURATION_MTD_DEVICE->sector_count;
    printf("erasing %"PRIu32" sectors in flash...\n", num_sectors);
    int res = mtd_erase_sector(CONFIGURATION_MTD_DEVICE, 0, num_sectors);
    switch (res)
    {
    case -EIO:
        printf("mtd: I/O error when erasing flash\n");
        break;
    case -ENODEV:
        printf("mtd: is not a valid device error when erasing flash\n");
        break;
    case -ENOTSUP:
        printf("mtd: operation is not supported when erasing flash\n");
        break;
    case -EOVERFLOW:
        printf("mtd: overflow error when erasing flash\n");
        break;
    case 0:
        printf("done successfully!\n");
        break;
    default:
        printf("mtd: Unknown error when erasing flash. Error code: %d\n", res);
    }
    return 0;
}