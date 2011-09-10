/*******************************************************************
 * dgram.c
 * support functions for datagram based programs
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define HOSTLEN 256

int make_internet_address(char *,int,struct sockaddr_in *);

/*******************************************************************
 * make sever dgram socket
 */
int make_dgram_server_socket(int portnum) {
	struct sockaddr_in saddr;			// build our address here
	char hostname[HOSTLEN];				// addrss
	int sock_id;					// the socket

	sock_id = socket(PF_INET,SOCK_DGRAM,0);		// get a socket
	if(sock_id == -1) {
		perror("Create socket error");
		return -1;
	}

	// build address and bind it to socket
	gethostname(hostname,HOSTLEN);			// where am I?
	make_internet_address(hostname,portnum,&saddr);

	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr)) != 0) {
		perror("Bind socket error");
		return -1;
	}

	return sock_id;
}// make_dgram_server_socket

/*******************************************************************
 * make client dgram socket
 */
int make_dgram_client_socket() {	
	return socket(PF_INET,SOCK_DGRAM,0);
}

/*******************************************************************
 * constructor for an Internet socket address, uses hostname and port
 * (host,port) -> *addrp
 */
int make_internet_address(char * hostname,int port,struct sockaddr_in *addrp) {
	struct hostent *hp;

	bzero((void *)addrp,sizeof(struct sockaddr_in));
	hp = gethostbyname(hostname);
	if(hp == NULL) {
		perror("Get host name error");
		return -1;
	}

	bcopy(hp -> h_addr,(void *)&addrp->sin_addr,hp->h_length);
	addrp->sin_port = htons(port);
	addrp->sin_family = AF_INET;

	return 0;
}

/*******************************************************************
 * extracts host and port from an internet socket address
 * addrp -> (host,port)
 */
int get_internet_address(char *host,int len,int *portp,struct sockaddr_in *addrp) {
	strncpy(host,inet_ntoa(addrp->sin_addr),len);
	*portp = ntohs(addrp->sin_port);

	return 0;
}
