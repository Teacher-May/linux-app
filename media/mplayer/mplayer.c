#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


//要跟着\r\n,不然会失败
#define CMD_NEXT  "pt_step 1\r\n"
#define CMD_PREV  "pt_step -1\r\n"

int main(int argc, char **argv)
{
    int fd;
    int ret;

    fd = open("/tmp/cmd", O_RDWR);
    if(fd < 0){
        perror("open cmd error");
        return fd;
    }

    //启动mplayer
    system("mkfifo /tmp/cmd");
    system("mplayer -quiet -slave -zoom -x 1024 -y 600 -input file=/tmp/cmd -playlist /root/movie/mplayer.list -loop 0 > /dev/null 2>&1 < /dev/null &");

    while(1){
        printf("请输入你的操作:"
                "[1] 上一曲"
                "[2] 下一曲\n");

        scanf("%d", &ret);
        switch(ret){
            case 1:
                write(fd, CMD_PREV, strlen(CMD_PREV));
                break;

            case 2:
                write(fd, CMD_NEXT, strlen(CMD_NEXT));
                break;

            default:
                printf("**error! unknow cmd\n");
                break;
        }
    }

    system("killall -9 mplayer");
    close(fd);
    
    return 0;
}



