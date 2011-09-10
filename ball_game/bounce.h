/* bounce.h
 * somet settings for the game
 */
#ifndef bounce_h
	#define bounce_h

	#include <aio.h>
	#include <curses.h>
	#include <fcntl.h>
	#include <string.h>
	#include <unistd.h>

	#define BLANK ' '
	#define DFL_SYMBOL 'o'
	#define PL_SYMBOL '*'
	#define TOP_ROW 5
	#define BOT_ROW 20
	#define LEFT_EDGE 10
	#define RIGHT_EDGE 70
	#define X_INIT 10				// starting col
	#define Y_INIT 10				// starting row
	#define TICKS_PER_SEC 50			// affects speed
	#define X_TTM 8
	#define Y_TTM 8

	int done = 0;
	int score = 0;

	// the ping pong ball
	struct ppball{
		int y_pos,x_pos,
		    y_ttm,x_ttm,
		    y_ttg,x_ttg,
		    y_dir,x_dir;
		char symbol;
	};

	// the player
	struct player {
		int y_pos,x_pos;
		char symbol;
	};

	struct aiocb kbcbuf;
	struct ppball the_ball;
	struct player the_player;

	int set_ticker(int);
	void draw_wall();
	void draw_score();
	void draw_player();
	void move_player(char);
#endif
