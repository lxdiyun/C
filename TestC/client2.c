#include   <stdio.h>    
#include   <stdlib.h>    
#include   <string.h>    
#include   <sys/types.h>    
#include   <sys/socket.h>    
#include   <netinet/in.h>    
#include   <netdb.h>    

int main() {

	int socketfd;
	struct sockaddr_in des_addr;
	struct hostent *dest;
	unsigned long ip;
	// char   s[50];  

	dest = gethostbyname("192.168.1.8");
	if (dest==0)
		exit(0);

	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Can   not   create   a   socket!\n");
		exit(1);
	}
	/*   set   destination   info   */
	bzero((char *)&des_addr, sizeof(struct sockaddr_in));
	des_addr.sin_family = AF_INET;
	des_addr.sin_port = htons(4123);
	des_addr.sin_addr = *((struct in_addr *)dest->h_addr);

	printf("fd   is   .....%d\n", socketfd);

	if (connect(socketfd, (struct sockaddr *)&des_addr, sizeof(struct sockaddr))
			== -1) {
		printf("Can   not   connect!\n");
		exit(1);
	}

	printf("111111111111111111111");

	return 0;

}
