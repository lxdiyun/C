/* mywho.c - read /etc/utmp and list info therein
 *	   - suppresses empty records
 *	   - formats time nicely
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main() {
	struct utmp utbuf;// read info into here
	int utmpfd;

	if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfd,&utbuf,sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	close(utmpfd);
	
	return 0;
}

/*
 * show_info()
 * 	display the contents of the utmp struct
 * 	in human readable form
 * 	* display nothing if record has no user name
 */
void show_info(struct utmp *utbufp) {
	if(utbufp -> ut_type != USER_PROCESS)
		return;
	
	printf("%-8.8s",utbufp -> ut_name);	// the logname
	printf(" ");				// a space
	printf("%-12.12s",utbufp -> ut_line);	// the tty
	printf(" ");				// a space
	showtime(utbufp -> ut_time);		// display time

#ifdef SHOWHOST
	if(utbufp -> ut_host[0] != '\0')
		printf(" (%s)",utbufp -> ut_host);
#endif
	printf("\n");
}

/*
 * display time in a format fit for human consumption
 * uses ctime to build string then picks parts ouf of it
 * Note:%12.12s prints a string chars a wide and LIMITS
 * it to 12 chars
 */
void showtime(long timeval) {
	char cp[17];				// to hold address of time
	
	strftime(cp,sizeof(cp),"%Y-%m-%d %H:%M"
		,gmtime(&timeval));		// conver time to string
						// string looks like
						// 2007-09-18 22:21
	printf("%s",cp);			// print time
}
