/* test socket client*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define HOST "http://hx.hxgame.net/showdown.asp?id=1718"
#define PORT 8888

int connect_to_server(char *,int);

int main() {
	int fd;
	char buf[BUFSIZ];

	fd  = connect_to_server(HOST,PORT);		// connect to server

	if(fd == -1) 
		exit(1);				// or die
	
	read(fd,buf,BUFSIZ);				// chat with the server	

	printf("%s",buf);				

	close(fd);					// hang up when done

	return 0;

}
