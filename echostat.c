/* echostat.c
 * reports current state of echo bit in tty driver for fd 0
 * shows how to read attributes from driver and test a bit
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main() {
	struct termios info;
	int rv;

	rv = tcgetattr(0,&info);		// read values from driver
	if(rv == -1) {
		perror("tcgetattr");
		exit(1);
	}

	if(info.c_lflag & ECHO)
		printf("echo is on, since bits is 1\n");
	else
		printf("echo is off, since bits is 0\n");

	return 0;
}
