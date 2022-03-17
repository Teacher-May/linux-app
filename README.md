# linux-app
Linux应用层常用库,用于操作硬件,系统等

关键字: linux embedded app uart i2c spi pwm usb sd gpio ......  

火热更新中,欢迎大家指正.

## 本代码志在如下几点:  
(1) 操作嵌入式Linux各种硬件,例如UART,I2C,SPI等;  
(2) 提供如何操作Linux系统各种及模块;  
(3) 提供各种小模块代码,例如测试ping,获取cpu利用率等,现拆现用.  

## 操作实例  

(1) 操作GPIO:  
```
#include "gpio.h"

#define BEEP_GPIO	(40)

gpio_open(BEEP_GPIO);

gpio_set_direction(BEEP_GPIO, "out");

gpio_set_value(BEEP_GPIO, 1);

printf("gpio value=%d\n", gpio_get_value(BEEP_GPIO));
```  
   

(2) 操作串口: 
```
#include "uart.h"

int fd;
char buf[64] = "hahahaha";

fd = uart_open(argv[1], 115200, 8, NONE_PARITY, 1);

uart_write(fd, buf, strlen(buf));
//uart_read(fd, buf, sizeof(buf));

uart_close(fd);
```

## 联系方式  
作者: 梅日菲  
作者QQ: 375476790  
微信公众号: 梅老师讲嵌入式  
![微信扫一扫](https://github.com/Teacher-May/linux-app/raw/master/pic/WXGZH.jpg)
