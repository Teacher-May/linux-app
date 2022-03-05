#include "uart.h"

int nread,nwrite;

/* 注意,不要在子线程里scanf或者fgets     */
void *read_thread(void *args)
{
    char buf[128];
    int fd;

    fd = (int)args;

    while(1){
        memset(buf, 0, sizeof(buf));
        nread += read_uart(fd, buf, sizeof(buf));
        printf("nread=%d uart_read=%s\n", nread, buf);
    }
}

int main(int argc, char **argv)
{
    int fd;
    char cmd[64] = "hahahaha";
    pthread_t tid;

    nread = 0;
    nwrite = 0;

    fd = open_uart(argv[1], 9600, 8, NONE_PARITY, 1);

    pthread_create(&tid, NULL, read_thread, (void *)fd);

    while(1){
        nwrite += write_uart(fd, cmd, strlen(cmd));
        printf("nwrite=%d\n", nwrite);
        sleep(1);
    }

        return 0;
}
