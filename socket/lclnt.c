/*******************************************************************
 * lclnt1.c
 * Listen server client version 1
 * link with lcntl_funcs.o dgram.o
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_regular_work();

void setup();
int get_ticket();
int release_ticket();
void shut_down();
int do_validate();

int main(int arc,char *av[]) {
	setup();

	if(get_ticket() != 0)
		exit(0);

	do_regular_work();

	release_ticket();

	shut_down();

	return 0;
}

/*******************************************************************
 * do_regular_work the main work of the application goes here
 */
void do_regular_work() {
	printf("SuperSleep version 1.0 Running-Licensed Software\n");
	sleep(5);					// out patented sleep algorithm
	printf("Validating the ticket\n");
	if(do_validate() == -1) {
		fprintf(stderr,"Validate failed");
		exit(0);
	}
	sleep(5);
}

