#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "gpio.h"

#define BEEP_GPIO	(40)



//计算cpu利用率
int get_cpu_percent()
{
	FILE *fp1, *fp2;
	char cpu1[4], cpu2[4];
	int user1, user2;
	int nice1, nice2;
	int system1, system2;
	int idle1, idle2;
	int iowait1, iowait2;
	int irq1, irq2;
	int softirq1, softirq2;
	int unsed1[3], unsed2[3];
	int c1, c2, T1, T2;
	int percent;
	
	

	fp1 = fopen("/proc/stat", "r");
	if(fp1 == NULL){
		perror("fopen error:");
		return -1;
	}

	//cpu  4342 661 4248 6392495 933 1 628 0 0 0
	fscanf(fp1, "%s %d %d %d %d %d %d %d %d %d %d", 
		cpu1, &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1, unsed1+0, unsed1+1, unsed1+2);

	//要关了再开,因为文件内容变了,如果不重新打开数据还是旧的
	fclose(fp1);

	usleep(100000);

	fp2 = fopen("/proc/stat", "r");
	if(fp2 == NULL){
		perror("fopen error:");
		return -1;
	}
	
	
	fscanf(fp2, "%s %d %d %d %d %d %d %d %d %d %d", 
		cpu2, &user2, &nice2, &system2, &idle2, &iowait2, &irq2, &softirq2, unsed2+0, unsed2+1, unsed2+2);


	c1 = user1 + nice1 + system1 + idle1 + iowait1 + irq1 + softirq1;
	c2 = user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2;

	T1 = c2 - c1;
	T2 = idle2 - idle1;

	//printf("c1=%d c2=%d T1=%d T2=%d\n", c1, c2, T1, T2);
	
	percent = 100 - ((float)T2 / (float)T1)*100;
	
	fclose(fp2);
	
	return percent;
}

int delay_ms = (5 * 1000);

void *beep_thread(void *args)
{
	while(1){
		gpio_set_value(BEEP_GPIO, 1);
		usleep(delay_ms * 1000);

		gpio_set_value(BEEP_GPIO, 0);
		usleep(delay_ms * 1000);
	}
		
}




int main(int argc, char **argv)
{
	int cpu;
	pthread_t tid;

	gpio_open(BEEP_GPIO);
	gpio_set_direction(BEEP_GPIO, "out");

	pthread_create(&tid, NULL, beep_thread, NULL);
	
	while(1){
		cpu = get_cpu_percent();

		//由于效果关系,设置最大为1秒响
		delay_ms = (100 - cpu) * (1 * 1000) / 100;
		printf("percent=%d%%\n", cpu);
		//sleep(1);
	}
	

	return 0;
}

