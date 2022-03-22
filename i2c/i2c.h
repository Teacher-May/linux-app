#ifndef __UART_H__
#define __UART_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>
    
#include <linux/i2c-dev.h> 
#include <linux/i2c.h>


int i2c_write(int fd, int slave_addr, int reg_addr, char *buf, int size);
int i2c_read(int fd, int slave_addr, short reg_addr, char *buf, int size);
int i2c_open(int busnum);
int i2c_close(int fd);


#ifdef __cplusplus
}

#endif
#endif                          // __I2C_H__


