#include "i2c.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    int i2c_fd;
    char value[2] = {0};
    short xmax;
    char slave_addr = 0x14;
    int reg_addr = 0x8048;

    i2c_fd = i2c_open(1);

    i2c_read(i2c_fd, slave_addr, reg_addr, value, sizeof(value));

    xmax = value[1] << 8 | value[0]; //看寄存器得知value[1]是高8位
    
    printf("xmax=%hd\n", xmax);

    i2c_close(i2c_fd);
    
    return 0;
}