#include <stdio.h>
#include <string.h>


#define SDPATH "/media/sdcard/mmcblk1p1/log.txt"

int main(int argc, char **argv)
{
    FILE *fp;
    char temp[] = "35.6";
    char hum[] = "70%";
    int i;
    char buf[64];

    //打开文件
    fp = fopen(SDPATH, "a");
    if(fp == NULL){
        perror("open file failed");
        return -1;
    }

    //写入文件
    for(i=0; i<10; i++){
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "这是第%d次写入,温度=%s,湿度=%s\n", i, temp, hum);
        fwrite(buf, sizeof(buf), 1, fp);
    }
    
    fflush(fp);   
    fclose(fp);

    system("sync &");
    printf("写入完毕\n");

    return 0;
}







