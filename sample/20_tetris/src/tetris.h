// Tetris Avalanche
// Gregory McIntyre
// 1999, 2000

#include <allegro.h>  // graphics
#include <stdio.h>	  // standard I/O
#include "data.h"     // stored game data


///  DEFINITIONS  //////////////////////////////////////////////////

#define TETRIS_VERSION   "v0.73"

#define EMAIL "greg@puyo.cjb.net"
#define URL   "http://puyo.cjb.net"

#define AUTHOR  "Gregory McIntyre"

#define DATES  "1999, 2000, 2002"

#define MAX_PLAYERS   2  // allow room for up to 2 players


// default player keys
#define P1_KEY_LEFT   KEY_J
#define P1_KEY_RIGHT  KEY_L
#define P1_KEY_DOWN   KEY_K
#define P1_KEY_ROTL   KEY_Y
#define P1_KEY_ROTR   KEY_U

#define P2_KEY_LEFT   KEY_S
#define P2_KEY_RIGHT  KEY_F
#define P2_KEY_DOWN   KEY_D
#define P2_KEY_ROTL   KEY_Q
#define P2_KEY_ROTR   KEY_W


// colours
#define C0   8
#define C1  16
#define C2  24
#define C3  32
#define C4  40
#define C5  48
#define C6  56
#define C7  64

enum PROCESSES {
    NOTHING = 0,
    DELETING_LINES,
    ANIMATION
};


///  GLOBAL VARIABLES/CONSTANTS  ///////////////////////////////////

#define WIN_W         10
#define WIN_H         20

#define T_SIZE         8

#define MAX_BITS     200

#define NAMELEN       16


// global constants
extern const int cSlowest;
extern const int cMoveDelay;
extern const int cRotDelay;
extern const int cDropDelay;
extern const unsigned int cScores[4];
extern const int cDeleteTime;


///  TYPE DEFINITIONS  /////////////////////////////////////////////

#define BOOL short


typedef struct TILE {
	int xoff, yoff;
	int colour;
}
TILE;


typedef struct PIECE {
	TILE tile[4];
}
PIECE;


typedef struct STACK {
	int tile[WIN_H][WIN_W];
}
STACK;


typedef struct NEXT {
	int x;
	int y;
	int piece;            // next piece id
	int visible;
}
NEXT;


typedef struct PLAYER {
	// THE CURRENT PIECE
	int current;          // current piece id
	int piece_x;          // current piece x position
	int piece_y;          // current piece x position
	int piece_r;          // current piece rotation

	// FUNCTIONALITY
	int process;          // process type id
	int fall_delay;       // automatic falling

	// THE STACK
	STACK stack;           // the stack
	// (the cumulating blocks at the bottom)

	int full_lines[4];    // any lines currently being deleted
	int full_lines_count; // a tally of the current number of full_lines lines

	int penalty;

	// SCORING
	unsigned int total_lines;
	unsigned long int total_score;

	// THE PLAYER'S WINDOW POSITION
	int win_x;
	int win_y;

	// INPUT
	int key_left;
	int key_right;
	int key_down;
	int key_rotl;
	int key_rotr;

}
PLAYER;


// high scores
typedef struct HIGH_SCORE {
	char name[NAMELEN];
	unsigned long int score;
}
HIGH_SCORE;



// global variables
extern PLAYER     p[];              // the players (leave room for max number)
extern BITMAP    *gBuff;            // screen buffer
extern DATAFILE  *gData;            // game's datafile
extern int        gPlayers;         // number of players actually participating
extern int        gDifficulty;      // difficulty level (speed of falling blocks)
extern NEXT       gNext;            // the next box (and piece)
extern int        gWinner;          // zero until somebody wins
extern int        gLines;           // backup of the number of screen lines


// timers
extern volatile unsigned int gFallCounter[];
extern volatile unsigned int gMoveCounter[];
extern volatile unsigned int gRotCounter[];
extern volatile          int gDropCounter[];
extern volatile unsigned int gDeleteCounter[];


extern const PIECE cPieceDef[19];
extern const PIECE * gPieceRef[4][7];
extern HIGH_SCORE gTable[10];


///  FUNCTION PROTOTYPES  ///////////////////////////////////////////////////

// initialisation & cleanup functions

void   main_init();
void   main_exit();
void   alleg_init();
void   alleg_exit();
void   graphics_init();
void   graphics_exit();
void   data_init();
void   data_exit();

void   game_reset();


// primary level functions

int    title_page();
void   key_check();
void   screen_draw();
void   record_scores();


// tiles

void   tile_draw(int x, int y, int c);


// player

void   player_update(int i);
void   player_key_check(int i);
char * player_read_name(int i, char * newname);
void   player_kill(int i);


//  stack

void   player_stack_linecheck(int i);
void   player_stack_deletelines(int i);
void   player_stack_draw(int i);


//  next box

void   player_next_update(int i);
void   next_draw();


//  player's current piece

BOOL   player_piece_move(int i, int dx, int dy);
void   player_piece_rotate(int i, int dir);
void   player_piece_stack(int i);
void   player_piece_limit(int i);


// any piece

BOOL   piece_sidehit(int pieceid, int r, int x);
BOOL   piece_bottomhit(int pieceid, int r, int y);
BOOL   piece_stackhit(STACK * stack, int pieceid, int r, int x, int y);
void   piece_draw(int x, int y, int r, int pieceid);


// high scores table

void   table_save();
void   table_display();


// timer handlers

void   t1_proc(void);
void   t2_proc(void);
void   t3_proc(void);


