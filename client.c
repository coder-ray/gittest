#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 2048
//a little change
int main(int argc, char const *argv[])
{
	/* code */
	int connfd;
	struct sockaddr_in serveraddr;
	char sendbuff[BUFFER_SIZE];
	char recvbuff[BUFFER_SIZE];

	if((connfd = socket(AF_INET,SOCK_STREAM,0)) <0 ){
		printf("create socket error %s (errno:%d)\n",strerror(errno),errno );
		exit(1);
	}

	//init server address
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(PORT);

	if(connect(connfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) <0 ){
		printf("connect to server error %s (errno:%d)\n",strerror(errno),errno );
		exit(1);
	}

	printf("----------input msg to send to server(\"exit\" means to quit)-----------\n");
	while(fgets(sendbuff,sizeof(sendbuff),stdin)!=NULL){
		memset(recvbuff,0,sizeof(recvbuff));
		if(strcmp(sendbuff,"exit\n")==0)
			break;
		send(connfd,sendbuff,strlen(sendbuff),0);
		recv(connfd,recvbuff,sizeof(recvbuff),0);
		fputs(recvbuff,stdout);
		memset(sendbuff,0,sizeof(sendbuff));
	}
	close(connfd);
	return 0;
}