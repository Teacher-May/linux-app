#include "i2c.h"

/*
    busnum: 总线号,例如0,1,2,对应i2c0,i2c1,i2c2
*/
int i2c_open(int busnum)
{
    int fd;
    char path[32] = {0};

    sprintf(path, "/dev/i2c-%d", busnum);

    fd = open(path, O_RDWR);
    if(fd < 0){
        printf("open i2c-%d error!\n", busnum);
    }
        
    return fd;
}

/*
    fd: 已打开的i2c
    slave_addr:器件地址
    reg_addr: 寄存器地址
    buf: 数据存放地址
    size: 数据大小
*/
int i2c_read(int fd, int slave_addr, short reg_addr, char *buf, int size)
{
    int ret;

	//我们要把16位地址分成2个char,高位在前,低位在后
	unsigned char reg[2];

	reg[0] = (reg_addr >> 8) & 0xff; //高八位
	reg[1] = reg_addr & 0xff; //低八位

	struct i2c_msg msgs[2];

	//第0次,先把要读取寄存器地址发送到总线上
	msgs[0].addr = slave_addr; //器件地址
	msgs[0].flags = 0;  //写
	msgs[0].buf = reg; //写入的数据
	msgs[0].len = sizeof(reg); //写入数据的大小

	//第1次,从总线上读出数据
	msgs[1].addr = slave_addr;
	msgs[1].flags = 1;
	msgs[1].buf = buf;
	msgs[1].len = size;

	//封装到i2c_rdwr_ioctl_data里,然后发送
	struct i2c_rdwr_ioctl_data data;

	data.msgs = msgs;
	data.nmsgs = 2; //2个消息

    ret = ioctl(fd, I2C_RDWR, &data);
	if(ret < 0){
		perror("ioctl error:");
	}

	return ret;
}


/*
    fd: 已打开的i2c
    slave_addr:器件地址
    reg_addr: 寄存器地址
    buf: 数据存放地址
    size: 数据大小
*/
int i2c_write(int fd, int slave_addr, int reg_addr, char *buf, int size)
{
    int ret;

	//我们要把16位地址分成2个char,高位在前,低位在后
	unsigned char reg[2];

	reg[0] = (reg_addr >> 8) & 0xff; //高八位
	reg[1] = reg_addr & 0xff; //低八位

	struct i2c_msg msgs[2];

	//第0次,先把要写入的寄存器地址发送到总线上
	msgs[0].addr = slave_addr;  //器件地址
	msgs[0].flags = 0;          //写
	msgs[0].buf = reg;          //写入的数据
	msgs[0].len = sizeof(reg);  //写入数据的大小

	//第1次,写入数据到总线上
	msgs[1].addr = slave_addr;
	msgs[1].flags = 0;
	msgs[1].buf = buf;
	msgs[1].len = size;

	//封装到i2c_rdwr_ioctl_data里,然后发送
	struct i2c_rdwr_ioctl_data data;

	data.msgs = msgs;
	data.nmsgs = 2; //2个消息

    ret = ioctl(fd, I2C_RDWR, &data);
	if(ret < 0){
		perror("ioctl error:");
	}

	return ret;

}

int i2c_close(int fd)
{
    return close(fd);
}


