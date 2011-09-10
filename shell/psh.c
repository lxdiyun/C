/* prompting shell version 2
 *
 * Solves the 'one-shot' problem of version 1
 * 	Uses execvp(), but fork() first so that the 
 * 	shell waits around to perform another command 
 * New problem: shell catches signals. Run vi, press ^C.
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 20					// cmdline args
#define ARGLEN 100					// token length

int main() {
	char *arglist[MAXARGS + 1];			// an array of ptrs
	int numargs;					// index into array
	char argbuf[ARGLEN];				// read stuff here
	
	char *makestring();				// malloc etc
	void execute(char **);

	numargs = 0;

	while(numargs < MAXARGS) {
		printf("Arg[%d]",numargs);
		if(fgets(argbuf,ARGLEN,stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else {
			if(numargs > 0) {		// any args?
				arglist[numargs] = NULL;// close list
				execute(arglist);	// do it
				numargs = 0;		// and reset
			}
		}
	}
	return 0;
}

/* 
 * use fork and execvp and wait to do it
 */
void execute(char *arglist[]) {
	int pid,exitstatus;				// of child

	pid = fork();

	switch(pid) {
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0],arglist);	// do it
			perror("execvp failed");
			exit(1);
		default:
			while(wait(&exitstatus) != pid);
			printf("child exited with status %d,%d\n",
				exitstatus >> 8,exitstatus & 0377);
	}
}

/*
 * trim off newline and create storage for the string
 */
char *makestring(char *buf) {
	char *cp;

	buf[strlen(buf) - 1] = '\0';			// trim newline
	cp = malloc(strlen(buf) + 1);			// get memory
	if(cp == NULL) {				// or die
		fprintf(stderr,"memory\n");
		exit(1);
	}
	strcpy(cp,buf);					// copy chars

	return cp;
}
