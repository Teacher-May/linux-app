#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>

int fd;
unsigned int *fb_base;


#define X_RES (1024)
#define Y_RES (600)
#define BYTES_PER_PIXER (4)

int show_bmp(char *path)
{
	int bmp_fd;
	char bmp_buf[1024 * 600 * 3 + 54];
	int lcd_buf[1024 * 600];
	int w,h;
	char *p;
	int i,x,y;

	//1.先把图片的数据全部读出来
	bmp_fd = open(path, O_RDONLY);
	if(bmp_fd < 0){
		perror("open bmp error:");
		return bmp_fd;
	}

	memset(bmp_buf, 0, sizeof(bmp_buf));
	read(bmp_fd, bmp_buf, sizeof(bmp_buf));

	w = bmp_buf[0x12] | bmp_buf[0x13] << 8 | bmp_buf[0x14] << 16 | bmp_buf[0x15] << 24;
	h = bmp_buf[0x16] | bmp_buf[0x17] << 8 | bmp_buf[0x18] << 16 | bmp_buf[0x19] << 24;
#if 0
	printf("%#x %#x %#x %#x %#x %#x %#x %#x\n", 
	bmp_buf[0x12], bmp_buf[0x13], bmp_buf[0x14], bmp_buf[0x15], 
	bmp_buf[0x16], bmp_buf[0x17], bmp_buf[0x18], bmp_buf[0x19]);
#endif
	printf("w=%d h=%d w=%#x h=%#x\n", w, h, w, h);
	
	//2.把原始RGB合成4个字节的像素点数据
	p = bmp_buf + 54; //跳过54个字节头

	//bmp原始数据是蓝绿红蓝绿红,像素点数据是红在高位
	memset(lcd_buf, 0, sizeof(lcd_buf));
	#if 0
	lcd_buf[0] = bmp_buf[0] | bmp_buf[1] << 8 | bmp_buf[2] << 16;
	lcd_buf[1] = bmp_buf[3] | bmp_buf[4] << 8 | bmp_buf[5] << 16;
	lcd_buf[2] = bmp_buf[6] | bmp_buf[7] << 8 | bmp_buf[8] << 16;
	lcd_buf[3] = bmp_buf[9] | bmp_buf[10] << 8 | bmp_buf[11] << 16;
	lcd_buf[4] = bmp_buf[12] | bmp_buf[13] << 8 | bmp_buf[14] << 16;
	//....写够1024x600为止
	#endif

	for(i=0; i<1024*600; i++){
		lcd_buf[i] = p[i*3] | p[i*3+1] << 8 | p[i*3+2] << 16;
	}
	
	//3.把像素点数据根据偏移写到显存上
	memset(fb_base, 0, sizeof(fb_base));
	
	for(y=0; y<600; y++)
		for(x=0; x<1024; x++)
			*(fb_base + y*1024 + x) = lcd_buf[y*1024 + x];

	close(bmp_fd);
}


int init_lcd(void)
{
	int size = X_RES * Y_RES * BYTES_PER_PIXER;
	
	fd = open("/dev/fb0", O_RDWR);
	if(fd < 0){
		perror("open fb0 error:");
		return fd;
	}

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

	return 0;

}

int close_lcd(void)
{
	munmap(fb_base, X_RES * Y_RES * BYTES_PER_PIXER);
	close(fd);

}

int main(int argc, char **argv)
{
	init_lcd();

	show_bmp(argv[1]);

	close_lcd();

	return 0;
}

