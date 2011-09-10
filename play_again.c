/* play_again.c
 * purpose: ask if user wants another transaction
 * mthod:
 * 	set tty into chr-by-chr,no-echo mode
 * 	set tty into no-delay mode
 * 	read char,return result
 * 	reset terminal modes on SIGINT,ignores SIGQUIT
 * returns: 0=>yes, 1=>no, 2=>timeout
 * better: reset terminal mode on Interrupt
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define ASK "Do you want another transaction"
#define TRIES 3					// max tries
#define SLEEPTIME 2				// time pertry
#define BEEP putchar('\a')			// alter user

int get_response(char *,int);
char get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();
void tty_mode();
void ctrl_c_handler();

int main() {
	int response;
	void ctrl_c_handler(int);
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	signal(SIGINT,ctrl_c_handler);
	signal(SIGQUIT,SIG_IGN);
	response = get_response(ASK,TRIES);
	tty_mode(1);
	
	return response;
}

/* purpose: ask question and wait for a y/n answer or timeoout
 * method: use getchar and complain about non-y/n input
 * reuturn: 0=>yes, 1=>no
 */
int get_response(char *question,int maxtries) {
	int input;

	printf("%s(y/n)?",question);			// ask
	fflush(stdout);					// force output
	while(1) {
		sleep(SLEEPTIME);			// wait a bit
		input = tolower(get_ok_char());		// get next char
		if(input == 'y')
			return 0;
		if(input == 'n')
			return 1;
		if(maxtries-- == 0)			// outattime
			return 2;			// sayso
		BEEP;
	}
}

/*
 * skip over non-legal chars return y,Y,n,N or EOF
 */
char get_ok_char() {
	int c;
	while((c = getchar()) != EOF && strchr("yYnN",c) == NULL);
	
	return c;
}

/*
 * purpose:put file decription 0 into chr-by-chr mode and noecho mode
 * mthod: use bits in termios
 */
void set_cr_noecho_mode() {
	struct termios ttystate;

	tcgetattr(0,&ttystate);				// read cuur. setting
	ttystate.c_lflag &= ~ICANON;			// no buffering
	ttystate.c_lflag &= ~ECHO;			// no echo either
	ttystate.c_cc[VMIN] = 1;			// get 1 char at a time
	tcsetattr(0,TCSANOW,&ttystate);			// install settngs
}

/* 
 * purpose: put file description into no-delay mode
 * method: use fcntl to set bits
 * notes: tcsetattr() will do somrthing similar, but is is complicated
 */
void set_nodelay_mode() {
	int termflags;
	termflags = fcntl(0,F_GETFL);			// read cuur. setting
	termflags |= O_NDELAY;				// flip on nodelay bit
	fcntl(0,F_SETFL,termflags);			// and install them
}

/*
 * how == 0 => save current mode,how == 1 => restore mode
 * this version handles termios and fcntl flag
 */
void tty_mode(int how) {
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if(how == 0) {
		tcgetattr(0,&original_mode);
		original_flags = fcntl(0,F_GETFL);
		stored = 1;
	} else if(stored) {
		tcsetattr(0,TCSANOW,&original_mode);
		fcntl(0,F_SETFL,original_flags);
	}
}

/* 
 * purpose: called if SIGINT is detected
 * action: reset tty and scam
 */
void ctrl_c_handler(int signum) {
	tty_mode(1);
	exit(1);
}
