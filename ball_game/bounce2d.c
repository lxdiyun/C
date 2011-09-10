/* bounce2d 1.0
 * bounce a character (defaults is 'o') around the screen
 * defined by some paraments
 *
 * user input: 
 * 	s slow down x component, S: slow y component
 * 	f speed up x component, F: speed y component
 * 	Q quit
 * 
 * purpose animation with use control, using aio_read() or O_ASYNC
 * timer tick sends SIGALRM, handler does animation
 * keybords sends SIGIO
 * main only class pause()
 * build: cc bounce2d.c set_ticker.c -lcurses -l rt-o bounce2d
 */
#include "bounce.h"


// the main loop

void set_up();
void wrap_up();

int main() {
	set_up();

	while(!done) 					// the main loop
		pause();
	
	wrap_up();

	return 0;
}

/*
 * init structure and other stuff
 */
void set_up() {
	void ball_move(int);
	void enable_kdb_signals();
	void setup_aio_buffer();
	void on_input(int);

	// setup the ball's state
	the_ball.y_pos = Y_INIT;		
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = DFL_SYMBOL;

	done = 0;

	initscr();					// setup the screen
	noecho();
	crmode();
	clear();
	
	// setup the player's state and draw the player
	the_player.x_pos = LEFT_EDGE;
	the_player.y_pos = TOP_ROW;
	the_player.symbol = PL_SYMBOL;
	draw_player();

	// draw the wall
	draw_wall();

	// setup and draw the score
	score = 0;
	draw_score();

	signal(SIGINT,SIG_IGN);
	mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
	refresh();

	signal(SIGIO,on_input);				// install a handler
	
	// use O_ASYNC
//	enable_kdb_signals();				// turn on kbd signals
	
	// use aio_read
	setup_aio_buffer();				// initialize aio ctrl buff
	aio_read(&kbcbuf);				// place a read quest

	signal(SIGALRM,ball_move);			// install alarm handler
	set_ticker(1000/TICKS_PER_SEC);			// send millisecs per tick
}

void wrap_up() {
	set_ticker(0);
	endwin();					// put back normal
}

void ball_move(int signum) {
	int y_cur,x_cur,moved;
	
	int bounce_or_lose(struct ppball *);

	signal(SIGALRM,ball_move);			// don't get caught now
	y_cur = the_ball.y_pos;				// old spot
	x_cur = the_ball.x_pos;
	moved = 0;

	if(the_ball.y_ttm > 0 && --the_ball.y_ttg == 0) {
		the_ball.y_pos += the_ball.y_dir;	// move
		the_ball.y_ttg = the_ball.y_ttm;	// reset
		moved = 1;
	}

	if(the_ball.x_ttm > 0 && --the_ball.x_ttg == 0) {
		the_ball.x_pos += the_ball.x_dir;	// move
		the_ball.x_ttg = the_ball.x_ttm;	// reset
		moved = 1;
	}

	if(moved) {
		mvaddch(y_cur,x_cur,BLANK);
		// mvaddch(y_cur,x_cur,BLANK);
		mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
		if(bounce_or_lose(&the_ball)) {
			++score;
			draw_score();
			draw_wall();
		}
		move(LINES - 1,COLS - 1);
		refresh();
	}
	signal(SIGALRM,ball_move);			//  for unreliable systems
}

int bounce_or_lose(struct ppball *bp) {
	int return_val = 0;

	if(bp -> y_pos <= TOP_ROW) {
		bp -> y_dir = 1;
	} else if(bp -> y_pos >= BOT_ROW) {
		bp -> y_dir = -1;
	}

	if(bp -> x_pos <= LEFT_EDGE) {
		bp -> x_dir = 1;
	} else if(bp -> x_pos >= RIGHT_EDGE) {
		bp -> x_dir = -1;
	}

	if(bp -> x_pos == the_player.x_pos 
		&& bp -> y_pos == the_player.y_pos)
		return_val = 1;

	return return_val;
}

/* 
 * intstall a handler, tell kernel who to notify on input, enable signals
 */
void enable_kdb_signals() {
	int fd_flags;

	fcntl(0,F_SETOWN,getpid());
	fd_flags = fcntl(0,F_GETFL);
	fcntl(0,F_SETFL,(fd_flags | O_ASYNC));
}

/*
 * handler called to when aio_read() has stuff to read
 * First check for any errors codes, and if ok, then get the return code
 */
void on_input(int signum) {
	int c;

	while((c = getchar()) != 'Q') {
		if(c == 'f') --the_ball.x_ttm;
		else if(c == 's') ++the_ball.x_ttm;
		else if(c == 'F') --the_ball.y_ttm;
		else if(c == 'S') ++the_ball.y_ttm;
		else if(c == 'z') 
			the_ball.x_dir = -the_ball.x_dir;
		else if(c == 'x')
			the_ball.y_dir = -the_ball.y_dir;
		else if(c == 'i' || c == 'j' || c == 'k' || c == 'l')
			move_player(c);
	}

	done = 1;
}

/*
 * set memebers of struct
 * First specify args like those for read(fd,buf,num) and offset
 * Then specify what to do(send signal) and what signal(SIGIO)
 */
void setup_aio_buffer() {
	static char input[1];				// 1 char of input

	// describe what to read
	kbcbuf.aio_fildes = 0;				// standard input
	kbcbuf.aio_buf = input;				// buffer
	kbcbuf.aio_nbytes = 1;				// number to read
	kbcbuf.aio_offset = 0;				// offset in file

	// describe what to do when read is ready
	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	kbcbuf.aio_sigevent.sigev_signo = SIGIO;	// send SIGIO
}

/*
 * draw the wall when setup.
 */
void draw_wall() {
	int i,j;
	
	// draw four corner
	mvaddch(TOP_ROW - 1,LEFT_EDGE - 1,'+');
	mvaddch(BOT_ROW + 1,LEFT_EDGE - 1,'+');
	mvaddch(TOP_ROW - 1,RIGHT_EDGE + 1,'+');
	mvaddch(BOT_ROW + 1,RIGHT_EDGE + 1,'+');

	// draw top row
	for(i = LEFT_EDGE,j = TOP_ROW - 1; i <= RIGHT_EDGE; ++i) {
		mvaddch(j,i,'-');
	}

	// draw left column
	for(i = LEFT_EDGE - 1,j = TOP_ROW; j <= BOT_ROW; ++j) {
		mvaddch(j,i,'|');
	}

	// draw botton row
	for(i = LEFT_EDGE,j = BOT_ROW + 1; i <= RIGHT_EDGE; ++i) {
		mvaddch(j,i,'-');
	}

	// draw right column
	for(i = RIGHT_EDGE + 1,j = TOP_ROW; j <= BOT_ROW; ++j) {
		mvaddch(j,i,'|');
	}
}

/*
 * draw score on the left top corner
 */
void draw_score() {
	char score_string[100];
	sprintf(score_string,"score:%d",score);
	move(1,1);
	addstr(score_string);
}

/*
 * draw the player
 */
void draw_player() {
	mvaddch(the_player.y_pos,the_player.x_pos,the_player.symbol);	
}

/*
 * move the player
 */
void move_player(char input) {
	mvaddch(the_player.y_pos,the_player.x_pos,BLANK);	
	switch(input) {
		case 'i':
			if(the_player.y_pos > TOP_ROW)
				--the_player.y_pos;
			break;
		case 'k':
			if(the_player.y_pos < BOT_ROW)
				++the_player.y_pos;
			break;
		case 'j':
			if(the_player.x_pos > LEFT_EDGE)
				--the_player.x_pos;
			break;
		case 'l':
			if(the_player.x_pos < RIGHT_EDGE)
				++the_player.x_pos;
			break;
	}
	draw_player();
}
