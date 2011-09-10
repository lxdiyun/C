/**************************************
 * UDPEchoClient.c
 * UDP Echo 客户端程序.
 * 	客户端接受终端输入，将一行一行
 * 	的数据发送给EchoServer,同时将从
 * 	EchoServer中获得的数据显示于屏
 * 	幕,直到用户中止程序.
 *
 * Created By: AD_Li
 * Created At: 07-11-21 15:45
 * E-mail: ad_li@foxmail.com
 **************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

/******************************************************************
 * 作用: 从键盘读入,并用UDP发送到服务端,再从服务端读回,并显示于
 * 	监视器上.
 * 参数: sockfd - 用于服务的 socket id, 
 * 	addr - 标识服务端的sockaddr_in指针, len - addr结构长度
 */
void udpc_request(int sockfd,const struct sockaddr_in *addr,
	int len) {
	char buffer[MAX_BUF_SIZE];
	int n;

	while(1) {
	// 从键盘读入,写到服务端
		fgets(buffer,MAX_BUF_SIZE,stdin);
		sendto(sockfd,buffer,strlen(buffer),0,
			(struct sockaddr *)addr,len);
		bzero(buffer,MAX_BUF_SIZE);
		// 从网络上读,写到屏幕上
		n = recvfrom(sockfd,buffer,MAX_BUF_SIZE,0,NULL,NULL);
		buffer[n - 1] = '\0';
		fprintf(stdout,"I received %s from server.\n",buffer);
	}
}

int main(int argc,char **argv) {
	int sockfd,port;
	struct sockaddr_in addr;

	if(argc != 3)  {
		fprintf(stderr,"Usage:%s server_ip server_port\n",argv[0]);
		exit(1);
	}
	if((port = atoi(argv[2])) < 0) {
		fprintf(stderr,"Usage:%s server_ip server_port\n",argv[0]);
		exit(1);
	}
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0) {
		fprintf(stderr,"Socket Error:%s\n",strerror(errno));
		exit(1);
	}
	// 填充服务端的资料
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if(inet_aton(argv[1],&addr.sin_addr) < 0) {
		fprintf(stderr,"IP error:%s\n",strerror(errno));
		exit(1);
	}
	udpc_request(sockfd,&addr,sizeof(struct sockaddr_in));
	close(sockfd);
	exit(0);
}
