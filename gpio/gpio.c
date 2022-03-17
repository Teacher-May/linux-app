#include "gpio.h"

//打开一个gpio: 往export里写入gpio口号
int gpio_open(int    gpionum)
{
	FILE *fp;
	char buf[64] = {0};

	fp = fopen("/sys/class/gpio/export", "w");
	if(fp == NULL){
		perror("fopen export error:");
		return -1;
	}

	//不能直接往文件里写int,要先把int转char数组
	// 字符串 -> 数字 : atoi
	// 数字 -> 字符串 : sprintf
	sprintf(buf, "%d", gpionum);

	fwrite(buf, strlen(buf), 1, fp);

	fflush(fp); //刷新缓冲区
	fclose(fp);

	return 0;
}


//关闭一个gpio: 往unexport里写入gpio口号
int gpio_close(int gpionum)
{
    FILE *fp;
    char buf[64] = {0};

    fp = fopen("/sys/class/gpio/unexport", "w");
    if(fp == NULL){
        perror("fopen export error:");
        return -1;
    }

    //不能直接往文件里写int,要先把int转char数组
    // 字符串 -> 数字 : atoi
    // 数字 -> 字符串 : sprintf
    sprintf(buf, "%d", gpionum);

    fwrite(buf, strlen(buf), 1, fp);

    fflush(fp); //刷新缓冲区
    fclose(fp);

	return 0;
}


//设置输入输出: 往direction里写入in/out
int gpio_set_direction(int gpionum, char *inout)
{
	char direction_path[128] = {0};
	FILE *fp;

	//要先拼出路径: /sys/class/gpio/gpio205/direction
	//上边的205是会变的,不同的gpio口号不同
	//如何让它根据gpionum参数一起变?

	sprintf(direction_path, "/sys/class/gpio/gpio%d/direction", gpionum);

	fp = fopen(direction_path, "w");
	if(fp == NULL){
		perror("fopen direction error:");
		return -1;
	}

	fwrite(inout, strlen(inout), 1, fp);

	fflush(fp); //刷新缓冲区
	fclose(fp);

	return 0;
}

//设置gpio的值: 往value里写0或者1
int gpio_set_value(int gpionum, int value)
{
	char value_path[128] = {0};
	char buf[8] = {0};
	FILE *fp;

	//要先拼出路径: /sys/class/gpio/gpio205/direction
	//上边的205是会变的,不同的gpio口号不同
	//如何让它根据gpionum参数一起变?

	sprintf(value_path, "/sys/class/gpio/gpio%d/value", gpionum);

	fp = fopen(value_path, "w");
	if(fp == NULL){
		perror("fopen value_path error:");
		return -1;
	}

	sprintf(buf, "%d", value);

	fwrite(buf, strlen(buf), 1, fp);

	fflush(fp); //刷新缓冲区
	fclose(fp);

	return 0;
}


//读取gpio的值: 读value的值
int gpio_get_value(int gpionum)
{
	char value_path[128] = {0};
	char buf[8] = {0};
	FILE *fp;

	sprintf(value_path, "/sys/class/gpio/gpio%d/value", gpionum);

	fp = fopen(value_path, "r");
	if(fp == NULL){
		perror("fopen value_path error:");
		return -1;
	}


	fread(buf, sizeof(buf), 1, fp);

	fflush(fp); //刷新缓冲区
	fclose(fp);

	return !!atoi(buf); //二值化
	
}



