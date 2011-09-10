/* Test socket server */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888

int make_server_socket(int);

int main() {
	int sock,fd;					// socket and connection
	const char *HELLO = "HELLO\n";

	sock = make_server_socket(PORT);

	if(sock == -1 )
		exit(-1);
	
	while(1) {
		fd = accept(sock,NULL,NULL);
		if(fd == -1)
			break;

		write(fd,HELLO,strlen(HELLO) * sizeof(HELLO[0]));

		close(fd);
	}

	return 0;
}
