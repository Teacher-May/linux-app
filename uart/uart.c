/*
	作者: 梅老师
	QQ: 375476790

	此代码通用所有Linux系统操作串口,假如不能使用,原因有如下:
	(1) 硬件损坏,板子损坏,解决办法,返厂维修;
	(2) 接线接错;
	(3) 电平协议不对,拿了TTL的接口接了232的接口(神马( ・ ◇ ・ )？你问我什么是232?);
	(4) 厂家源码的串口驱动有问题,解决方法有2,
		1.自己硬肛改串口驱动,改好来;
		2.和厂家联系,让它提供支持;
*/


#include "uart.h"

speed_t uart_get_baudrate(int baudrate)
{
    switch(baudrate) {
        case 0: 		return B0;
        case 50: 		return B50;
        case 75: 		return B75;
        case 110: 		return B110;
        case 134: 		return B134;
        case 150: 		return B150;
        case 200: 		return B200;
        case 300: 		return B300;
        case 600: 		return B600;
        case 1200: 		return B1200;
        case 1800: 		return B1800;
        case 2400: 		return B2400;
        case 4800: 		return B4800;
        case 9600: 		return B9600;
        case 19200: 	return B19200;
        case 38400: 	return B38400;
        case 57600: 	return B57600;
        case 115200: 	return B115200;
        case 230400: 	return B230400;
        case 460800: 	return B460800;
        case 500000: 	return B500000;
        case 576000: 	return B576000;
        case 921600: 	return B921600;
        case 1000000: 	return B1000000;
        case 1152000: 	return B1152000;
        case 1500000: 	return B1500000;
        case 2000000: 	return B2000000;
        case 2500000: 	return B2500000;
        case 3000000: 	return B3000000;
        case 3500000: 	return B3500000;
        case 4000000: 	return B4000000;
        default: 		return -1;
    }
}


/*
        uart_path: 	串口驱动节点,例如/dev/ttySAC*,不是所有的板子都叫ttySAC*,有的叫ttyO*,ttyS*,ttyAMA*等等
	uart_baudrate: 	波特率,比如9600,115200等
	uart_databits:	数据位,只有5,6,7,8
	uart_parity: 	校验位
	uart_stopbits: 	停止位,只有1, 1.5, 2
	
	
	例如:
	char a = 'O';
	uart_open("/dev/ttySAC2", 115200, 8, NONE_PARITY, 1);

*/
int uart_open(char *uart_path, int uart_baudrate, int uart_databits, char uart_parity, int uart_stopbits)
{
    int fd, result;
    struct termios newtio,oldtio;   //创建
    speed_t uart_speed = B115200;

    fd = open(uart_path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0){
    	perror("opens serial failed");
        return fd;
    }

	//再重新设置成非阻塞,保险
    printf("fcntl: %d\n", fcntl(fd, F_SETFL, 0));

	//判断你的驱动节点是不是一个串口设备,假如不反回0,就不是串口设备
    if(tcgetattr(fd, &oldtio) != 0){ 
    	printf("%s is not a uart device\n", uart_path);
    	close(fd);
        return -1;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= (CLOCAL | CREAD);  
    newtio.c_cflag &= ~CSIZE;
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    newtio.c_oflag &= ~OPOST;

    switch(uart_databits){
        case 5:
            newtio.c_cflag |= CS5;
            break;

        case 6:
            newtio.c_cflag |= CS6;
            break;

        case 7:
            newtio.c_cflag |= CS7;
            break;

        case 8:
            newtio.c_cflag |= CS8;
            break;

        default:
            newtio.c_cflag |= CS8;
            break;
    }

    switch(uart_parity){
        case ODD_PARITY:
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;

        case EVEN_PARITY:
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;

        case NONE_PARITY:
            newtio.c_cflag &= ~PARENB;
            break;

        default:
            printf("**Warning: parity bit error, default to no parity, use ODD_PARITY or ODD_PARITY or NONE_PARITY\n");
            newtio.c_cflag &= ~PARENB;
            break;
    }

    uart_speed = uart_get_baudrate(uart_baudrate);

    cfsetispeed(&newtio, uart_speed);
    cfsetospeed(&newtio, uart_speed);


    switch (uart_stopbits){
        case 1:
            newtio.c_cflag &= ~CSTOPB;
            break;

        case 2:
            newtio.c_cflag |= CSTOPB;
            break;

        default:
            newtio.c_cflag &= ~CSTOPB;
            break;
    }

	/*
        下面两句代码就是核心部分:
        先来了解一种情况,比如a发送给b 64个字节的数据,正常情况下我们都希望b一次性收完,但是实际情况
        不可能是一次收完的,b都会断断续续的收到;
        下面这两句代码就是为了防止上面的这种情况发生,希望b会"等一下",等待a发完,但是假如b等了,a又不发了
        这样就很难受了,所以如下2个参数就是为了防止这种情况发生;

        c_cc[VTIME] = 3;
        c_cc[VMIN] = 64;
        首先read是会阻塞的;
        64的意思是最小要读取到64个字节才返回,否则阻塞在read函数上;
        3的意思就是假如在3*100ms内收不到64个字节,就强制返回,解除read阻塞;

        其实这两个参数还有其他含义,比如:
        newtio.c_cc[VTIME]	= 0;
    	newtio.c_cc[VMIN] 	= 64;
    	代表的意思是: 
    	首先read是会阻塞的;
    	死等,必须等到64个字节再返回,没有接受超时时间;

    	newtio.c_cc[VTIME]	= 0;
    	newtio.c_cc[VMIN] 	= 0;
    	代表的意思是:
    	首先read是不会阻塞的;
    	收不到就算了,read是不阻塞的;

    	newtio.c_cc[VTIME]	= 4;
    	newtio.c_cc[VMIN] 	= 0;
    	
	*/

    newtio.c_cc[VTIME]	= 2;
    newtio.c_cc[VMIN] 	= 64;

    tcflush(fd, TCIOFLUSH); //清空缓冲区

	//把newtio的属性设置到打开的fd
    if((tcsetattr(fd, TCSANOW, &newtio)) != 0){ 
    	perror("set attr to uart error:");
        return -1;
    }
	
    return fd;
}


int uart_read(int fd, char *buf, int size)
{
    return read(fd, buf, size);
}


int uart_write(int fd, char *buf, int size)
{
    return write(fd, buf, size);
}

int uart_close(int fd)
{
    return close(fd);
}

