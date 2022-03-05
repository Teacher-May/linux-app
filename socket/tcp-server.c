#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>


//端口号是自定义的,自定义的端口最好是10000以上的,因为1~1024端口是被使用了,四位数的端口也有
#define PORT 55555

int main(int argc, char **argv)
{
	int fd;
	char buf[64];
	int sock,sin_size;
	struct sockaddr_in server_addr,client_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket error:");
		return sock;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(PORT);  //服务器的端口号,htons转成大端序
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //接收来自谁的连接,INADDR_ANY表示任意链接

	bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));


	listen(sock, 5);   //开始监听链接,最大的排队数是5


	sin_size = sizeof(client_addr);

	//accept是会阻塞的,假如没有客户端连接,它是会阻塞的
	//client_addr是用来装客户端的信息的,例如客户端的ip,端口等等信息
	fd = accept(sock, (struct sockaddr*)&client_addr, &sin_size);
	

	while(1){
		memset(buf, 0, sizeof(buf));
		if(read(fd, buf, sizeof(buf)) <= 0){
			printf("client is offline\n");
			break;
		}
		printf("buf=%s\n", buf);
	}

	close(fd);
	close(sock);

	return 0;
}





