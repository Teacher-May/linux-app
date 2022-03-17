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

#include <termios.h>
#include <termios.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/time.h>

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <net/if.h>

#define ODD_PARITY 		'O'  /* 奇校验 */
#define EVEN_PARITY 	'E'  /* 偶校验 */
#define NONE_PARITY     'N'  /* 无校验 */

int uart_read(int fd, char *buf, int size);
int uart_write(int fd, char *buf, int size);
int uart_close(int fd);
int uart_open(char *uart_path, int uart_baudrate, int uart_databits, char uart_parity, int uart_stopbits);
speed_t uart_get_baudrate(int baudrate);

#ifdef __cplusplus
}

#endif
#endif                          // __I2C_H__
