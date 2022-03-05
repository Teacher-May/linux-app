#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

#define PORT 55555


int main(int argc, char **argv)
{
	int fd;
	int ret;
	char buf[64];
	int sock,sin_size;
	struct sockaddr_in server_addr,client_addr;


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket error:");
		return sock;
	}

	memset(&server_addr, 0, sizeof(server_addr));


	//我们要连接服务器,首先要知道服务器的IP和端口
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(PORT);  //服务器的端口号,htons转成大端序
	server_addr.sin_addr.s_addr = inet_addr(argv[1]); //把字符串转成点分十进制

	ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0){
		perror("connect error:");
		return ret;
	}

	while(1){
		memset(buf, 0, sizeof(buf));

		fgets(buf, sizeof(buf), stdin);

		write(sock, buf, sizeof(buf));
	}


	return 0;
}






