#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define PROMPT "你想终止程序吗?"

char *prompt = PROMPT;

void ctrl_c_op(int signo) {
	write(STDERR_FILENO,prompt,strlen(prompt));
}

int main() {
	struct sigaction act;

	act.sa_handler = ctrl_c_op;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(sigaction(SIGINT,&act,NULL) < 0) {
		fprintf(stderr,"Install Signal Aciton Error:%s\n\a"
			,strerror(errno));
		exit(1);
	}

	while(1);

	return 0;
}
