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
	int sock,size;
	char buf[64];

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket error:");
		return sock;
	}

	struct sockaddr_in send_addr,src_addr;
	send_addr.sin_family = AF_INET;                     //地址类型为AF_INET
	send_addr.sin_port = htons(PORT);           
//	send_addr.sin_addr.s_addr = inet_addr("192.168.2.108"); 

	bind(sock,(struct sockaddr*)&send_addr, sizeof(send_addr));

	size = sizeof(src_addr);

	while(1){
		memset(buf, 0, sizeof(buf));
		
		if(recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&src_addr, &size) <= 0){
			perror("client is offline:");
			break;
		}

		printf("buf=%s\n", buf);
	}

	close(sock);

	return 0;
}




