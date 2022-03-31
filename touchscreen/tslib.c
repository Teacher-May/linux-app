#include <stdio.h>
#include "tslib.h"

int main(int argc, char **argv)
{
	struct tsdev *ts;
	struct ts_sample point;
	int ret;

	//打开触摸屏
	ts = ts_open("/dev/input/event4", 0);
	if(ts == NULL){
		perror("ts open error:");
		return -1;
	}

	//使用默认配置
	ts_config(ts);

	//读取坐标
	while(1){
		ts_read(ts, &point, 1);

		printf("x=%d y=%d pressure=%d\n", point.x, point.y, point.pressure);
	}

	//关闭触摸屏
	ts_close(ts);

	return 0;
}



