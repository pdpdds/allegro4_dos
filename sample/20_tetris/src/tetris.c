// Tetris Avalanche
// Gregory McIntyre
// 1999, 2000

#include "tetris.h"
#include "timer.h"

#include <stdlib.h>   // } random number generator
#include <string.h>   // high score table

#ifdef _WIN32
extern int random();
#endif
///  GLOBAL VARIABLES/CONSTANTS  ///////////////////////////////////

// global constants
const int cSlowest            = 20;  // easiest difficulty
const int cMoveDelay          = 5;
const int cRotDelay           = 10;
const int cDropDelay          = 1;
const unsigned int cScores[4] = { 50, 200, 500, 1000 };
const int cDeleteTime         = 64;


///  GLOBAL VARIABLES/CONSTANTS  ///////////////////////////////////

// piece definitions
const PIECE cPieceDef[19] = {
    // rotated 0
    { { { 2, 1, C1 }, { 3, 1, C1 }, { 1, 2, C1 }, { 2, 2, C1 } } },
    { { { 0, 1, C2 }, { 1, 1, C2 }, { 1, 2, C2 }, { 2, 2, C2 } } },
    { { { 0, 1, C3 }, { 1, 1, C3 }, { 2, 1, C3 }, { 2, 2, C3 } } },
    { { { 1, 1, C4 }, { 2, 1, C4 }, { 3, 1, C4 }, { 1, 2, C4 } } },
    { { { 0, 1, C5 }, { 1, 1, C5 }, { 2, 1, C5 }, { 1, 2, C5 } } },
    { { { 1, 1, C6 }, { 1, 2, C6 }, { 2, 1, C6 }, { 2, 2, C6 } } },
    { { { 0, 1, C7 }, { 1, 1, C7 }, { 2, 1, C7 }, { 3, 1, C7 } } },
    // rotated 90 degrees
    { { { 1, 1, C1 }, { 1, 2, C1 }, { 2, 2, C1 }, { 2, 3, C1 } } },
    { { { 2, 1, C2 }, { 1, 2, C2 }, { 2, 2, C2 }, { 1, 3, C2 } } },
    { { { 2, 0, C3 }, { 2, 1, C3 }, { 1, 2, C3 }, { 2, 2, C3 } } },
    { { { 1, 1, C4 }, { 2, 1, C4 }, { 2, 2, C4 }, { 2, 3, C4 } } },
    { { { 1, 0, C5 }, { 0, 1, C5 }, { 1, 1, C5 }, { 1, 2, C5 } } },
    { { { 1, 0, C7 }, { 1, 1, C7 }, { 1, 2, C7 }, { 1, 3, C7 } } },
    // roated 180 degrees
    { { { 1, 1, C3 }, { 1, 2, C3 }, { 2, 2, C3 }, { 3, 2, C3 } } },
    { { { 2, 1, C4 }, { 0, 2, C4 }, { 1, 2, C4 }, { 2, 2, C4 } } },
    { { { 1, 0, C5 }, { 0, 1, C5 }, { 1, 1, C5 }, { 2, 1, C5 } } },
    // roated 270 degrees
    { { { 1, 1, C3 }, { 2, 1, C3 }, { 1, 2, C3 }, { 1, 3, C3 } } },
    { { { 1, 0, C4 }, { 1, 1, C4 }, { 1, 2, C4 }, { 2, 2, C4 } } },
    { { { 1, 0, C5 }, { 1, 1, C5 }, { 2, 1, C5 }, { 1, 2, C5 } } }
};

// pointers to piece definitions
const PIECE * gPieceRef[4][7] = {
    // rotated 0
    {
        &cPieceDef[0],
        &cPieceDef[1],
        &cPieceDef[2],
        &cPieceDef[3],
        &cPieceDef[4],
        &cPieceDef[5],
        &cPieceDef[6]
    },
	// rotated 1
	{
		&cPieceDef[7],
		&cPieceDef[8],
		&cPieceDef[9],
		&cPieceDef[10],
		&cPieceDef[11],
		&cPieceDef[5],
		&cPieceDef[12]
	},
	// rotated 2
	{
		&cPieceDef[0],
		&cPieceDef[1],
		&cPieceDef[13],
		&cPieceDef[14],
		&cPieceDef[15],
		&cPieceDef[5],
		&cPieceDef[6]
	},
	// rotated 3
	{
		&cPieceDef[7],
		&cPieceDef[8],
		&cPieceDef[16],
		&cPieceDef[17],
		&cPieceDef[18],
		&cPieceDef[5],
		&cPieceDef[12]
	}
};


HIGH_SCORE gTable[10] = {
	{ "Lucy", 10000 },
	{ "Greg",  9000 },
	{ "Puyo",  8000 },
	{ "Haju",  7000 },
	{ "Vera",  6000 },
	{ "Chuck", 5000 },
	{ "Dave",  4000 },
	{ "Peter", 3000 },
	{ "Paul",  2000 },
	{ "Mary",  1000 }
};


// global variables
PLAYER     p[MAX_PLAYERS];   // the players (leave room for max number)
BITMAP    *gBuff;            // screen buffer
DATAFILE  *gData;            // game's datafile
int        gPlayers;         // number of players actually participating
int        gDifficulty;      // difficulty level (speed of falling blocks)
NEXT       gNext;            // the next box (and piece)
int        gWinner;          // zero until somebody wins


// timers
volatile unsigned int gFallCounter[MAX_PLAYERS];
volatile unsigned int gMoveCounter[MAX_PLAYERS];
volatile unsigned int gRotCounter[MAX_PLAYERS];
volatile          int gDropCounter[MAX_PLAYERS];
volatile unsigned int gDeleteCounter[MAX_PLAYERS];


///  MAIN FUNCTION  /////////////////////////////////////////////////////////

int main(void) {
	int i;
	char choice;
	char quit;

	main_init();

	quit = FALSE;
	do {
		choice = title_page();

		switch (choice) {
		case (0) :
		case (1) :
			gPlayers = choice + 1;
			break;
		case (2) :
			continue;
		default :
			quit = TRUE;
		};

		if (!quit) {
			game_reset();

			// main game loop
			do {
				key_check();

				for (i = 0; i != gPlayers; ++i) {
					player_key_check(i);
					player_stack_linecheck(i);
					player_update(i);
				}

				screen_draw();
				blit(gBuff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
			} while (gWinner == 0);

			record_scores();
			table_display();
		}
	} while (!quit);

	table_display();
	main_exit();
	return 0;
}

END_OF_MAIN();


///  FUNCTION DEFINITIONS  //////////////////////////////////////////////////

void main_init() {
	PALETTE * palette;
	FILE * fp;
	int i, n;

	alleg_init();
	graphics_init();
	data_init();


	palette = (PALETTE *)gData[PAL].dat;
	set_palette(*palette);

	// make sure a high score table exists (write a new one if it doesn't)
	if (!file_exists("hiscore.tbl", 0, NULL))
		table_save();

	// read in the high score table
	fp = fopen("hiscore.tbl", "rb");

	for (n = 0; n != 10; ++n) {
		// write the player's name
		fread(&gTable[n].name[0], sizeof(char), NAMELEN, fp);
		// write the player's score
		fread(&gTable[n].score, sizeof(unsigned long int), 1, fp);
	}
	fclose(fp);

	set_config_file("tetris.cfg");

	text_mode(-1);

	// seed random number generator
#ifdef _WIN32
	srand(time(NULL));
#else
	srandom(time(NULL));
#endif

	gPlayers = 1;

	p[0].key_left  = get_config_int("player1", "key_left",  P1_KEY_LEFT);
	p[0].key_right = get_config_int("player1", "key_right", P1_KEY_RIGHT);
	p[0].key_down  = get_config_int("player1", "key_down",  P1_KEY_DOWN);
	p[0].key_rotl  = get_config_int("player1", "key_rotl",  P1_KEY_ROTL);
	p[0].key_rotr  = get_config_int("player1", "key_rotr",  P1_KEY_ROTR);

	p[1].key_left  = get_config_int("player2", "key_left",  P2_KEY_LEFT);
	p[1].key_right = get_config_int("player2", "key_right", P2_KEY_RIGHT);
	p[1].key_down  = get_config_int("player2", "key_down",  P2_KEY_DOWN);
	p[1].key_rotl  = get_config_int("player2", "key_rotl",  P2_KEY_ROTL);
	p[1].key_rotr  = get_config_int("player2", "key_rotr",  P2_KEY_ROTR);

	n = get_config_int("options", "show_next", 1);

	if (n == 0)
		gNext.visible = FALSE;
	else
		gNext.visible = TRUE;

	for (i = 0; i != gPlayers; ++i) {
		LOCK_VARIABLE(gFallCounter[i]);
		LOCK_VARIABLE(gMoveCounter[i]);
		LOCK_VARIABLE(gRotCounter[i]);
		LOCK_VARIABLE(gDropCounter[i]);
		LOCK_VARIABLE(gDeleteCounter[i]);
	}

	LOCK_FUNCTION(t1_proc);
	LOCK_FUNCTION(t2_proc);
	LOCK_FUNCTION(t3_proc);

	install_int(t1_proc, 20);
	install_int(t2_proc, 25);
	install_int(t3_proc, 4);
}



void main_exit() {
	graphics_exit();
	data_exit();
	alleg_exit();

	printf("Thankyou for playing Tetris Avalanche!\n");
	printf("\n");
	printf("Version " TETRIS_VERSION ", " DATES "\n");
	printf(AUTHOR "\n");
	printf("------------------------------\n");
	printf(EMAIL "\n");
	printf(URL "\n");
	printf("------------------------------\n");
	printf("Kupo! ^_^\n");

	exit(0);
}



void alleg_init() {
	allegro_init();
	install_timer();
	install_keyboard();
}



void alleg_exit() {
	clear_keybuf();
	remove_keyboard();
	remove_timer();
	allegro_exit();
}



void graphics_init() 
{
#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0))
#else
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0))
#endif	
	{
		fprintf(stderr, "Error setting screen mode:\n");
		fprintf(stderr, allegro_error);
		exit(-1);
	}

	if ((gBuff = create_bitmap(SCREEN_W, SCREEN_H)) == NULL) {
		fprintf(stderr, "Could not allocate memory for a screen buffer.");
		exit(-1);
	}
	clear(gBuff);
}



void graphics_exit() {
	if (gBuff != NULL)
		destroy_bitmap(gBuff);
}



void data_init() {
	if ((gData = load_datafile("tetris.dat")) == NULL) {
		fprintf(stderr, "Error loading datafile.");
		exit(-1);
	}
}



void data_exit() {
	if (gData != NULL)
		unload_datafile(gData);
}



void game_reset() {
	int i;
	int x, y;

	for (i = 0; i != gPlayers; ++i) {
		p[i].process = NOTHING;
		p[i].fall_delay = cSlowest - gDifficulty;

		for (y = 0; y != WIN_H; ++y)
			for (x = 0; x != WIN_W; ++x)
				p[i].stack.tile[y][x] = 0;

		for (x = 0; x != 4; ++x)
			p[i].full_lines[x] = -1;
		p[i].full_lines_count = 0;

		p[i].total_lines = 0;
		p[i].total_score = 0;

		p[i].penalty     = 0;

		// generate 1st piece
		gNext.piece = random() % 7;
		player_next_update(i);
	}

	if (gPlayers == 1) {
		p[0].win_x = 72;
		gNext.x = 168;
	} else {
		p[0].win_x = 40;
		gNext.x = 136;
	}

	p[0].win_y = 24;
	p[1].win_x = 200;
	p[1].win_y = 24;

	gNext.y = 24;

	gWinner = 0;

	p[1].current = p[0].current;

	gDifficulty = 0;

	// init timers
	for (i = 0; i != gPlayers; ++i) {
		gFallCounter[i]   = 0;
		gMoveCounter[i]   = 0;
		gRotCounter[i]    = 0;
		gDropCounter[i]   = 0;
		gDeleteCounter[i] = 0;
	}
}


///  PRIMARY FUNCTION DEFINITIONS  //////////////////////////////////////////

int title_page() {
	BITMAP *  title     = (BITMAP *)gData[TITLE].dat;
	BITMAP *  avalanche = (BITMAP *)gData[AVALANCHE].dat;
	int highlight = 0;
	int choice    = -1;
	int k;

	clear_keybuf();

	while (choice == -1) {
		// redraw the titlepage
		clear(gBuff);
		blit(title,     gBuff, 0, 0, (SCREEN_W - title->w)/2,     10, title->w, title->h);
		blit(avalanche, gBuff, 0, 0, (SCREEN_W - avalanche->w)/2, 10 + title->h + 10, avalanche->w, avalanche->h);

		textout(gBuff, (FONT *)gData[FONT_NORM].dat, "1 Player Game", 120, 120, 15);
		textout(gBuff, (FONT *)gData[FONT_NORM].dat, "2 Player Game", 120, 130, 15);
		textout(gBuff, (FONT *)gData[FONT_NORM].dat, "Options",       120, 140, 8);
		textout(gBuff, (FONT *)gData[FONT_NORM].dat, "Exit",          120, 150, 15);

		textout(gBuff, (FONT *)gData[FONT_NORM].dat, "@", 110, 120 + highlight*10 - 1, 39);

		textout_centre(gBuff, (FONT *)gData[FONT_NORM].dat, TETRIS_VERSION " " AUTHOR ", " DATES, SCREEN_W/2, SCREEN_H - 10, 15);

		blit(gBuff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		// accept input
		k = readkey() >> 8;

		if ((k == KEY_UP) && (highlight > 0))
			--highlight;

		if ((k == KEY_DOWN) && (highlight < 3))
			++highlight;

		if (k == KEY_ENTER)
			choice = highlight;
	}

	return choice;
}



void key_check() {
	// check for general key presses

	//PALETTE pal;  // for screen capture

	while (key[KEY_PAUSE])
		textout_centre(screen, (FONT *)gData[FONT_TINY].dat, "PAUSED", SCREEN_W/2, (SCREEN_H - 8)/2, 15);

	/*
	// SCREEN CAPTURE
	if (key[KEY_F12])
	  {
	   get_palette(pal);
	   save_bitmap("scrnshot.pcx", gBuff, pal);

	   textout_centre(screen, (FONT *)gData[FONT_TINY].dat, "Screen captured. Press the spacebar to continue...",
	                  SCREEN_W/2, SCREEN_H/2 - 4, 15);

	   do { } while (!key[KEY_SPACE]);
	  }
	*/
}



void screen_draw() {
	int x, y;
	int i;
	unsigned long top;
	int halfwin = WIN_W*T_SIZE/2;

	if (gPlayers == 1) {
		blit((BITMAP *)gData[BACK_1PLAYER].dat, gBuff, 0, 0, 0, 0, gBuff->w, gBuff->h);

		// information
		textprintf(gBuff, (FONT *)gData[FONT_TINY].dat, 180, 77,  15, "LEVEL: %d", gDifficulty + 1);
		textprintf(gBuff, (FONT *)gData[FONT_TINY].dat, 180, 109, 15, "LINES: %d", p[0].total_lines);
		textprintf(gBuff, (FONT *)gData[FONT_TINY].dat, 180, 141, 15, "SCORE: %ld", p[0].total_score);

		if (p[0].total_score > gTable[0].score)
			top = p[0].total_score;
		else
			top = gTable[0].score;
		textprintf(gBuff, (FONT *)gData[FONT_TINY].dat, 180, 173, 15, "TOP  : %ld", top);
	} else
		blit((BITMAP *)gData[BACK_2PLAYER].dat, gBuff, 0, 0, 0, 0, gBuff->w, gBuff->h);

	for (i = 0; i != gPlayers; ++i) {
		// draw game area

		// grid
		for (y = 0; y != WIN_H; ++y) {
			hline(gBuff, p[i].win_x, p[i].win_y + y*T_SIZE, p[i].win_x + WIN_W*T_SIZE, 4);
			for (x = 0; x != WIN_W; ++x)
				vline(gBuff, p[i].win_x + x*T_SIZE, p[i].win_y, p[i].win_y + WIN_H*T_SIZE, 4);
		}

		// next piece box
		next_draw();

		// the stack
		player_stack_draw(i);

		// the deleting line effect
		if (p[i].process == DELETING_LINES) {
			for (y = 0; y != 4; ++y) {
				if (p[i].full_lines[y] != -1)
					rectfill(gBuff, p[i].win_x + halfwin - halfwin * gDeleteCounter[i]/cDeleteTime, p[i].win_y + p[i].full_lines[y]*T_SIZE,
					         p[i].win_x + halfwin + halfwin * gDeleteCounter[i]/cDeleteTime, p[i].win_y + p[i].full_lines[y]*T_SIZE + T_SIZE - 1, 0);
			}
		}

		// the current piece
		set_clip(gBuff, p[i].win_x, p[i].win_y,
		         p[i].win_x + WIN_W*T_SIZE,
		         p[i].win_y + WIN_H*T_SIZE);

		piece_draw(p[i].win_x + p[i].piece_x*T_SIZE,
		           p[i].win_y + p[i].piece_y*T_SIZE, p[i].piece_r, p[i].current);
		set_clip(gBuff, 0, 0, gBuff->w, gBuff->h);

		//textprintf(gBuff, font, SCREEN_W/2*i, 0, 15, "(%d, %d, %d)", p[i].piece_x, p[i].piece_y, p[i].piece_r);
	}

}



void record_scores() {
	int i, n, place;

	for (i = 0; i != gPlayers; ++i) {
		if (p[i].total_score >= gTable[9].score) {
			place = 9;
			// find where they should be
			for (n = 9; n >= 0; --n) {
				if (p[i].total_score >= gTable[n].score)
					place = n;
				else
					break;
			}

			// move entries down
			for (n = 9; n > place; --n) {
				strcpy(gTable[n].name, gTable[n - 1].name);
				gTable[n].score = gTable[n - 1].score;
			}

			gTable[place].score = p[i].total_score;

			// name entry
			player_read_name(i, gTable[place].name);
		}
	}

	// save the table
	table_save();
}



///  TILE FUNCTION DEFINITIONS  /////////////////////////////////////////////

void tile_draw(int x, int y, int c) {
	// draw a tile at x, y

	// basic rectangle
	rectfill(gBuff, x,              y,              x + T_SIZE - 1, y + T_SIZE - 1, c + 4);

	// top
	hline(gBuff, x + 1,          y,              x + T_SIZE - 2, c + 6);
	// bottom
	hline(gBuff, x + 1,          y + T_SIZE - 1, x + T_SIZE - 2, c + 2);

	// left
	vline(gBuff, x,              y + 1,          y + T_SIZE - 2, c + 6);
	// right
	vline(gBuff, x + T_SIZE - 1, y + 1,          y + T_SIZE - 2, c + 2);

	// top + 1
	hline(gBuff, x + 1,          y + 1,          x + T_SIZE - 3, c + 5);
	// bottom - 1
	hline(gBuff, x + 2,          y + T_SIZE - 2, x + T_SIZE - 2, c + 3);

	// left + 1
	vline(gBuff, x + 1,          y + 2,          y + T_SIZE - 3, c + 5);
	// right - 1
	vline(gBuff, x + T_SIZE - 2, y + 2,          y + T_SIZE - 3, c + 3);

	// top left corner
	putpixel(gBuff, x,              y,              c + 5);

	// bottom right corner
	putpixel(gBuff, x + T_SIZE - 1, y + T_SIZE - 1, c + 1);
}

