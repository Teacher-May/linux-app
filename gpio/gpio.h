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


int gpio_open(int    gpionum);
int gpio_close(int gpionum);
int gpio_set_direction(int gpionum, char *inout);
int gpio_set_value(int gpionum, int value);
int gpio_get_value(int gpionum);


#ifdef __cplusplus
}

#endif
#endif                          // __I2C_H__

