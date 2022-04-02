#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>


unsigned int *fb_base;


int main(int argc, char **argv)
{
	int fd;
	int size;
	int x,y;
	int color[3] = {0xff0000, 0x00ff00, 0x0000ff}; 
	
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

    //打开lcd
	fd = open("/dev/fb0", O_RDWR);
	if(fd < 0){
		perror("open error:");
		return fd;
	}

	ioctl(fd, FBIOGET_FSCREENINFO, &finfo); //获取固定信息
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo); //获取可变信息

	printf("xres=%d yres=%d xres_virtual=%d yres_virtual=%d\n", 
			vinfo.xres, vinfo.yres, vinfo.xres_virtual, vinfo.yres_virtual);
	printf("line_length=%d\n", finfo.line_length);

	//屏幕多大就映射多大
	size = vinfo.xres * vinfo.yres * 4;

	//把基地址映射出来,后续刷新颜色根据基地址偏移即可
	fb_base = (unsigned int *)mmap(NULL, 
									size, 
									PROT_READ | PROT_WRITE, 
									MAP_SHARED, 
									fd, 
									0);
	if(fb_base == NULL){
		perror("mmap failed:");
		return -1;
	}

	memset(fb_base, 0, size);

	for(y=0; y<600; y++)
	    for(x=0; x<1024; x++){
            *(fb_base + 1024*y + x) = 0x00ff00;
	    }


	munmap(fb_base, size);
	close(fd);

	return 0;
}











