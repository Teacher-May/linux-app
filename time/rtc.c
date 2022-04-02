#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <linux/rtc.h>

int main(int argc, char **argv)
{
    int fd;
    int ret;
    struct rtc_time rtc_tm;
    
    fd = open("/dev/rtc0", O_RDWR);
    if(fd < 0){
        perror("open error: ");
        return fd;
    }
    
    while(1){
        ret = ioctl(fd, RTC_RD_TIME, &rtc_tm);
        printf("%d-%d-%d %d-%d-%d\n", 
                rtc_tm.tm_year+1900, 
                rtc_tm.tm_mon+1,
                rtc_tm.tm_mday, 
                rtc_tm.tm_hour, 
                rtc_tm.tm_min, 
                rtc_tm.tm_sec);
        sleep(1);
    }

    close(fd);
    return 0;
}

