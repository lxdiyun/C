#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

// Linux 的默认个人邮箱地址是 /var/spool/mail/*/
#define MAIL_DIR "/var/spool/mail"
// 睡眠10秒钟
#define SLEEP_TIME 10
#define MAX_FILENAME 255

unsigned char notifyflag = 1;

long get_file_size(const char *filename) {
	struct stat buf;

	if(stat(filename,&buf) == -1) {
		if(errno == ENOENT)
			return 0;
		else return -1;
	}

	return (long)buf.st_size;
}

void send_mail_notify(void) {
	fprintf(stderr,"New mail has arrived\007\n");
}

void turn_on_notify(int signo) {
	notifyflag = 1;
}

void turn_off_notify(int signo) {
	notifyflag = 0;
}

int check_mail(const char *filename) {
	long old_mail_size,new_mail_size;
	sigset_t blockset,emptyset;

	sigemptyset(&blockset);
	sigemptyset(&emptyset);
	sigaddset(&blockset,SIGUSR1);
	sigaddset(&blockset,SIGUSR2);
	old_mail_size = get_file_size(filename);
	if(old_mail_size < 0) return 1;
	if(old_mail_size > 0) send_mail_notify();
	sleep(SLEEP_TIME);

	while(1) {
		if(sigprocmask(SIG_BLOCK,&blockset,NULL) < 0)
			return 1;

		while(notifyflag == 0)
			sigsuspend(&emptyset);
		
		if(sigprocmask(SIG_SETMASK,&emptyset,NULL) <0)
			return 1;
		new_mail_size = get_file_size(filename);
		if(new_mail_size > old_mail_size)
			send_mail_notify();
		old_mail_size = new_mail_size;
		sleep(SLEEP_TIME);
	}
}

int main(void) {
	char mailfile[MAX_FILENAME];
	struct sigaction newact;
	struct passwd *pw;

	if((pw = getpwuid(getuid())) == NULL) {
		fprintf(stderr,"Get Login Name Error:%s\n\a",
			strerror(errno));
		exit(1);
	}
	
	strcpy(mailfile,MAIL_DIR);
	strcat(mailfile,pw->pw_name);
	newact.sa_handler = turn_on_notify;
	newact.sa_flags = 0;
	sigaddset(&newact.sa_mask,SIGUSR1);
	sigaddset(&newact.sa_mask,SIGUSR2);
	if(sigaction(SIGUSR1,&newact,NULL) < 0)
		fprintf(stderr,"Turn On Error:%s\n\a",strerror(errno));
	newact.sa_handler = turn_off_notify;
	if(sigaction(SIGUSR1,&newact,NULL) < 0)
		fprintf(stderr,"Turn off Error:%s\n\a",strerror(errno));
	check_mail(mailfile);
	exit(0);
}
