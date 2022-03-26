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
	int sock;
	char buf[64];

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket error:");
		return sock;
	}

	struct sockaddr_in send_addr;
	send_addr.sin_family = AF_INET;                     //地址类型为AF_INET
	send_addr.sin_port = htons(PORT);              //服务器端口
	send_addr.sin_addr.s_addr = inet_addr(argv[1]); 

 
	while(1){
		memset(buf, 0, sizeof(buf));
		
		fgets(buf, sizeof(buf), stdin);

		sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
	}




	return 0;
}





