/*This is a adventure game made by Tino Restivo
please use whatever you find in my source code
there is no liscences on any of it.
my website is www.geocities.com/tinor123
if you have any questions email me at restivo@conestoga.net
libraries you will need
are allegro by Shawn Hargreaves and
the jpgalleg library by Angelo Mottola
thanks go to them and whoever else contributed with the libraries.
The source was compiled under mingw and dev-cpp.
*/


#include <stdlib.h>
#include <stdio.h>
/*These statements get rid of things that are not
need and are in the allegro library*/
#define alleg_gui_unused
#define alleg_vidmem_unused
#define alleg_flic_unused
/*include statements for libraries*/
//#include "jpgalleg.h"
#include "allegro.h"


FILE *f2;  /*binary files used to record movement of characters*/
FILE *f3;  /*and for in game movies*/
FILE *f4;
FILE *f5;
DATAFILE *datafile;    /*main datafile for main character, fonts and
					   some micellaneous pictures for objects*/
DATAFILE *music;       /*contains midi and sample for game*/

DATAFILE *level1cdat;  /*contains pictures that are used for
					   collisions*/
DATAFILE *level2cdat;  /*contains pictures that are used for
					   collisions*/
BITMAP *redlight;      /*bitmap used for transluency*/

BITMAP *buffer;        /*buffer so that screen does not flicker*/

BITMAP *bufferc;       /*bitmap so drawing can be done on collision
					   pictures without drawing over them*/

BITMAP *dialog;        /*bitmap for when characters talk to the
					   screen*/

BITMAP *intro[3];      /*bitmaps used to convert the files
					   in "intro.dat" from jpg images*/

BITMAP *level1[10];    /*bitmaps used to convert the files
					   in "level1dat.dat" from jpg images*/

BITMAP *level2[10];    /*bitmaps used to convert the files
					   in "level2dat.dat" from jpg images*/

BITMAP *character[30]; /*bitmaps used to convert the files
					   in "chara.dat" from jpg images*/

					   /*set of volatiles for using with locked functions
					   for timing and keeping a steady user input update*/
int showtimeleftmm;
volatile int frame_count, framecheck, fps;

volatile int timeleftm;
volatile int timelefts;
volatile float seconds;
volatile int timeleftmm;

volatile int speed_counter = 0;
volatile int showplayskip;
char msg[80];

#include "source/include/intro1.h" /*header file for intro1.dat*/
#include "source/include/adv.h" /*header file for adv.dat*/
#include "source/include/chara.h"  /*header file for chara.dat*/
#include "source/include/level1.h" /*header file for level1.dat*/
#include "source/include/level1c.h"/*header file for level1c.dat*/
#include "source/include/level2.h" /*header file for level2.dat*/
#include "source/include/level2c.h"/*header file for level2c.dat*/
#include "jpeg.c"
#include <math.h>
#include <string.h>

#define SNOW_MAX 5

#define QUIT 99             /*defines for reason game is exiting*/
#define GAMEOVER 100

#ifndef M_PI
#define M_PI   3.14159
#endif


/* convert radians to degrees */
#define DEG(n)    ((n) * 180.0 / M_PI)
/*convert degrees to rad*/
#define RAD     M_PI/180
/*variable for player*/
int playermove;
int speed = 40;                  /*how fast the player walks*/
int playerx = 50;
int playery = 200;
int showplayer = 1;
int showscreen = 1;              /*integer to say what backround to show
							   and what objects will be on that screen*/

							   /*used to slow down animation of
							  character walking*/

float f_playerx = 50.0;
float f_playery = 200.0;
float playermovex, playermovey;
int moveleft, moveright, moveup, movedown = 0;
int collide, oldmovex, oldmovey;
int mickeyx, mickeyy, mousepressed, mousepressedright;
int oldmovetox, movetox = 50;
int oldmovetoy, movetoy = 200;
int screenupdated;             /*screen is only updated once
							   every 25 frames*/
							   /*variables for movies*/
int movies = 100;
int introcount;
int movie1x;
int car1x;
int truck1x;
int truck2x = 640;
int seekslow, seeking, mover1, mover = 0;
int seeking2 = 482;
int talknotshown;

int timesfire;       /*counts how many times the fire function is called*/

				/*     integers to count where characters should
					 be during movies and when text should show up*/
int moviecount;
int moviecount2;
int moviecount3 = 0;
int pace = 238;
int talkcolor = 1;      /*changes color of text for when character
					  talks to the screen*/

int movie1talk = 0;     /*integer used for putting text on screen
					  when character is talking during a movie*/
int color_pos;        /*integer that checks the color of the pixel
					  that the mouse is on*/
					  /*integers for translucency*/
int redlightcolor, redlightdir;
/*level2*/
int follow = 0;                            /*for following scientist*/

int times_gas_called, times_gas_called2 = 0; /*counts for how many times
										  function is called*/
int gas_off = FALSE;
int lablights = 0;                         /*turns on and off lights for rooms*/
int stocklights = 0;

int level = 0;                             /*what area of the game
										 the player is in*/
int old_level = 0;
int pace2 = 0;                            /*for one of the bad guys pacing*/

int uniform = 0;                         /*integer for what uniform you
									   are wearing*/
struct mover
{
	int x, y, show;
};
struct mover guy1;             /*structer for one character during
							   movies values read from binary file*/


struct mover snow[2][300];     /*array used to store snow position
							   value*/


struct movie1
{
	int x1, x2, y1, y2;
	int show1, show2;
	int atscreen;
};
struct movie1 movie1_1;        /*structer for two characters during
							   movies values read from binary file*/
struct FIRE
{
	int r, g, b;
	int x, y;
};
struct FIRE fire[6500];    /*structure used to hold positions of fire*/

int poly[8];               /*used to fill in areas after they change
						   like when doors are opened*/

						   /*include statements for all functions besides main*/
#include "source/main/objects.c"
#include "source/main/badguy.c"
#include "source/main/display.c"
#include "source/main/input.c"
#include "source/main/menu.c"
#include "source/main/movie.c"
#include "source/main/play.c"
#include "source/main/snow.c"
#include "source/main/fire.c"
#include "source/main/title.c"
#include "source/main/gas.c"
#include "source/main/secur.c"
#include "source/main/tank.c"



	  /*function to keep game updating user
	  inputs at constant speed*/
void increment_speed_counter()
{

	showplayskip++;      /*used so animation is updated every
						 few frames*/
	speed_counter++;     /*used so user input is calculated
						 25 times per second*/
}

END_OF_FUNCTION(increment_speed_counter);


/*function used to count fps and keep time for seconds*/
void fps_proc()
{

	timeleftmm--;
	seconds += 0.001;
	if (timeleftmm < 0)
	{
		fps = frame_count;
		frame_count = 0;
		timeleftmm = 999;
		timelefts--;

		if (seconds > 32000)seconds = 0;
		/*alarm is played when gas is filling
		the room*/
		if (showscreen == 8 && gas_off == FALSE)
		{
			play_sample(music[2].dat, 64, 128, 1000, FALSE);
			times_gas_called++;
		}
	}

	if (timelefts < 0)
	{
		timelefts = 59;
		timeleftm--;
	}
	if (timelefts >= 60)
	{
		timelefts -= 60;
		timeleftm++;
	}

}

END_OF_FUNCTION(fps_proc);




/*tables for drawing translueceny */
RGB_MAP rgb_table;
COLOR_MAP light_table;
COLOR_MAP trans_table;


void print_progress(int pos)
{
	/*prints the stars that show the progress of creating the color maps*/
	if ((pos & 3) == 3) {
		printf("*");
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	DATAFILE *intro1; /*jpegs of intro*/

	DATAFILE *chara;      /*jpegs of characters except main character
						  (which is already bitmaps)*/

	DATAFILE *level1dat;  /*all screens for first section of game*/
	DATAFILE *level2dat;  /*all screens for second section of game*/

	char buf[256];

	PALLETE pal;
	int updatescreen;
	int bpp;             /*I have the snow and fire and movies
						 updated everytime the display is redrawn
						 so I can't update the more than 25fps
						 even if input is not updated*/

	int footstep;        /*integer used to say what voice is
						 assigned to the sound of footsteps*/
	int transparent;

	int c;
	int color, x, y;
	/*This section installs allegro*/
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();

	srand(time(NULL));    /*a timer used for rand numbers*/

   /* I took this section of code from the 3d bouncing cubes example
   that comes with allegro*/
   /* color 0 = black */
	pal[0].r = pal[0].g = pal[0].b = 0;

	/* copy the desktop pallete */
	for (c = 1; c < 64; c++)
		pal[c] = desktop_pallete[c];

	/* make a red gradient */
	for (c = 64; c < 96; c++) {
		pal[c].r = (c - 64) * 2;
		pal[c].g = pal[c].b = 0;
	}

	/* make a green gradient */
	for (c = 96; c < 128; c++) {
		pal[c].g = (c - 96) * 2;
		pal[c].r = pal[c].b = 0;
	}

	/* set up a greyscale in the top half of the pallete */
	for (c = 128; c < 256; c++)
		pal[c].r = pal[c].g = pal[c].b = (c - 128) / 2;

	/* build rgb_map table */
	printf("Generating rgb_map table:\n");
	printf("<................................................................>\r<");
	create_rgb_table(&rgb_table, pal, print_progress);
	rgb_map = &rgb_table;
	printf("\n\n");

	/* build a lighting table */
	printf("Generating lighting table:\n");
	printf("<................................................................>\r<");
	create_light_table(&light_table, pal, 0, 0, 0, print_progress);
	color_map = &light_table;
	printf("\n");

	/*sets the color depth*/
	bpp = 24;
	set_color_depth(bpp);
#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
		bpp = 32;
		set_color_depth(bpp);
		if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
			/*     bpp=24;
			set_color_depth(bpp);
			if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) != 0) {
			bpp=32;
			set_color_depth(bpp);
			if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) != 0) {  */
			allegro_message("could not set_gfx_mode(): %s\n", allegro_error);
			return 1;
		}
	}
#else
	if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) != 0) {
		bpp = 32;
		set_color_depth(bpp);
		if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) != 0) {
			/*     bpp=24;
			  set_color_depth(bpp);
			  if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) != 0) {
				 bpp=32;
				 set_color_depth(bpp);
				 if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) != 0) {  */
			allegro_message("could not set_gfx_mode(): %s\n", allegro_error);
			return 1;
		}
	}
#endif
	/*   }
	  }      */
	if (bpp == 24)transparent = MASK_COLOR_24;
	if (bpp == 32)transparent = MASK_COLOR_32;

	set_mouse_range(0, 0, 639, 599);     /*set the mouse to the limits*/


	argv[0] = "";
	/*following section loads data files*/
	replace_filename(buf, argv[0], "data/main/intro1.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 20, makecol(255, 255, 255));
	intro1 = load_datafile(buf);
	if (!intro1) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 12;
	}

	replace_filename(buf, argv[0], "data/main/adv.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 40, makecol(255, 255, 255));
	datafile = load_datafile(buf);
	if (!datafile) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 12;
	}
	replace_filename(buf, argv[0], "data/main/chara.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 60, makecol(255, 255, 255));
	chara = load_datafile(buf);
	if (!chara) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 12;
	}
	replace_filename(buf, argv[0], "data/level1/level1.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 80, makecol(255, 255, 255));
	level1dat = load_datafile(buf);
	if (!level1dat) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 13;
	}

	replace_filename(buf, argv[0], "data/level1/level1c.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 100, makecol(255, 255, 255));
	level1cdat = load_datafile(buf);
	if (!level1cdat) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 13;
	}

	replace_filename(buf, argv[0], "data/level2/level2.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 120, makecol(255, 255, 255));
	level2dat = load_datafile(buf);
	if (!level2dat) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 23;
	}

	replace_filename(buf, argv[0], "data/level2/level2c.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 140, makecol(255, 255, 255));
	level2cdat = load_datafile(buf);
	if (!level2cdat) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 23;
	}

	replace_filename(buf, argv[0], "data/main/music.dat", sizeof(buf));
	sprintf(msg, "loading %s ", buf);
	textout(screen, font, msg, 0, 160, makecol(255, 255, 255));
	music = load_datafile(buf);
	if (!music) {
		allegro_exit();
		printf("Error loading %s!\n\n", buf);
		return 13;
	}

	/*fps timer this function is called every millisecond to
	update the fps*/
	install_int(fps_proc, 1);

	/* sets the game speed to 25 frames per second*/
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(25));
	/*locks timer variables explained in allegro manuals*/
	LOCK_VARIABLE(timeleftmm);
	LOCK_VARIABLE(timeleftm);
	LOCK_VARIABLE(timelefts);
	LOCK_VARIABLE(seconds);

	LOCK_VARIABLE(showplayskip);
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	LOCK_VARIABLE(frame_count);
	LOCK_VARIABLE(fps);
	LOCK_FUNCTION(fps_proc);

	/*section creates bitmaps*/

	buffer = create_bitmap(640, 600);
	dialog = create_bitmap(600, 300);
	redlight = create_bitmap(640, 480);

	bufferc = create_bitmap(160, 120);

	/*sets the background of the text to transparent*/
	text_mode(-1);

	/*loads the file type for jpg*/
	register_bitmap_file_type("jpg", load_jpg, NULL);

	/*This section loads all the jpg images in the datafiles
	and turns them into bitmaps for use in the game*/
	sprintf(msg, "converting intro jpegs to bitmaps");
	textout(screen, font, msg, 0, 200, makecol(255, 255, 255));
	for (c = 0; c < 4; c++)
	{
		intro[c] = load_memory_jpg(intro1[c].dat, NULL);/*loads intro*/
		if (!intro[c]) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error in the datafile intro1 image %d\n", c);
			return 1;
		}
	}
	sprintf(msg, "converting character jpegs to bitmaps");
	textout(screen, font, msg, 0, 220, makecol(255, 255, 255));
	for (c = 0; c < 30; c++)
	{
		character[c] = load_memory_jpg(chara[c].dat, NULL);/*loads characters*/
		if (!character[c]) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error in the datafile chara image %d\n", c);
			return 1;
		}
	}
	sprintf(msg, "converting level1 jpegs to bitmaps");
	textout(screen, font, msg, 0, 240, makecol(255, 255, 255));
	for (c = 0; c < 9; c++)
	{

		level1[c] = load_memory_jpg(level1dat[c].dat, NULL);/*loads backgrounds*/
		if (!level1[c]) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error in the datafile level1dat image %d\n", c);
			return 1;
		}
	}
	sprintf(msg, "converting level2 jpegs to bitmaps");
	textout(screen, font, msg, 0, 260, makecol(255, 255, 255));
	for (c = 1; c < 7; c++)
	{

		level2[c] = load_memory_jpg(level2dat[c].dat, NULL);/*loads backgrounds*/
		if (!level2[c]) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error in the datafile image level2dat %d\n", c);
			return 1;
		}
	}

	/*The following loads the bitmaps and uses
	the corresponding mask to draw wherever there is black
	on the mask bitmap to draw a transparent pixel in the same
	spot on the bitmap that is shown to the screen*/
	sprintf(msg, "creating main character bitmap masks");
	textout(screen, font, msg, 0, 300, makecol(255, 255, 255));
	/*This one is for the main character*/
	for (c = 1; c < 11; c++)
	{
		for (x = 0; x < 640; x++)
			for (y = 0; y < 480; y++)
			{
				color = getpixel(datafile[c + 10].dat, x, y);
				if (color == -1)y = 480;
				if (color == 0) putpixel(datafile[c].dat, x, y, transparent);
			}
	}
	sprintf(msg, "creating bad guy character bitmap masks");
	textout(screen, font, msg, 0, 320, makecol(255, 255, 255));
	/*This one is for one badguy*/
	for (c = 0; c < 10; c++)
	{

		for (x = 0; x < 640; x++)
			for (y = 0; y < 480; y++)
			{
				color = getpixel(datafile[c + 11].dat, x, y);
				if (color == -1)y = 480;
				if (color == 0) putpixel(character[c], x, y, transparent);
			}
	}
	sprintf(msg, "creating other bad guy character bitmap masks");
	textout(screen, font, msg, 0, 340, makecol(255, 255, 255));
	/*This one is for the other badguy*/
	for (c = 10; c < 20; c++)
	{
		for (x = 0; x < 640; x++)
			for (y = 0; y < 480; y++)
			{
				color = getpixel(datafile[c + 1].dat, x, y);
				if (color == -1)y = 480;
				if (color == 0) putpixel(character[c], x, y, transparent);
			}
	}
	sprintf(msg, "creating scientist character bitmap masks");
	textout(screen, font, msg, 0, 360, makecol(255, 255, 255));
	/*This one is for the scientist*/
	for (c = 20; c < 30; c++)
	{
		for (x = 0; x < 640; x++)
			for (y = 0; y < 480; y++)
			{
				color = getpixel(datafile[c - 9].dat, x, y);
				if (color == -1)y = 480;
				if (color == 0) putpixel(character[c], x, y, transparent);
			}
	}

	sprintf(msg, "creating bitmap masks for car and truck");
	textout(screen, font, msg, 0, 380, makecol(255, 255, 255));
	/*These these two are for the car and the trunk*/
	for (x = 0; x < 640; x++)
		for (y = 0; y < 480; y++)
		{
			color = getpixel(datafile[23].dat, x, y);
			if (color == -1)y = 480;
			if (color == 0) putpixel(datafile[23].dat, x, y, transparent);
		}
	for (x = 0; x < 640; x++)
		for (y = 0; y < 480; y++)
		{
			color = getpixel(datafile[21].dat, x, y);
			if (color == -1)y = 480;
			if (color == 0) putpixel(datafile[21].dat, x, y, transparent);
		}

	clear_to_color(dialog, transparent);   /*clears the bitmap
											that is used for showing
											text when character talks
											to the screen*/

											/*This sections draws to every other pixel
											on the text box grey or darker grey so that
											text box looks translucent without having
											to draw the bitmap transparent*/
	sprintf(msg, "creating message bitmap");
	textout(screen, font, msg, 0, 400, makecol(255, 255, 255));
	for (x = 0; x < 599; x++)
		for (y = 0; y < 599; y++)
		{

			if ((x & 1) == 1)
			{
				if ((y & 1) == 1)
					putpixel(dialog, x, y, makecol(196, 164, 164));
			}
			else if ((y & 1) == 0)
				putpixel(dialog, x, y, makecol(164, 164, 164));
		}

	sprintf(msg, "initialize objects");
	textout(screen, font, msg, 0, 420, makecol(255, 255, 255));
	init_objects();    /*function called to initialize
					   some of the object properties
					   such as if the character can pick the the object
					   or not*/
	sprintf(msg, "opening binary files for read only");
	textout(screen, font, msg, 0, 440, makecol(255, 255, 255));
	f2 = fopen("data/level1/movie1", "rb");
	f3 = fopen("data/level1/pace", "rb");
	f4 = fopen("data/level2/follow", "rb");
	f5 = fopen("data/level2/pace2", "rb");
	/*The following clears the bitmaps*/
	clear(buffer);
	clear(screen);
	clear_to_color(redlight, makecol(255, 0, 255));

	/* install a sound driver  */
	sprintf(msg, "installing sound");
	textout(screen, font, msg, 0, 460, makecol(255, 255, 255));
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, argv[0]) != 0) {
		printf("Error initialising sound system\n%s\n", allegro_error);
		return 11;
	}
	/*sets the digital and midi volumes for the game*/
	set_volume(255, 255);

	/* start title music */
	play_midi(music[0].dat, 1);

	/*sets voice used for footsteps and the speed for the footsteps*/
	footstep = allocate_voice(music[1].dat);
	voice_set_frequency(footstep, 8000);
	/*sets the color and solidness of the transparencies*/
	set_trans_blender(0, 0, 0, 196);

	titlescreen();            /*loads the title screen*/

	while (!(key[KEY_ESC]))
	{
		stop_midi();          /*once game starts the midi music is turned off
							  and if the level is different from the last time
							  the program looped through the music will change
							  to match the level*/
		if (old_level != level)
		{
			old_level = level;
			stop_sample(music[3].dat);
			stop_sample(music[4].dat);
			stop_sample(music[5].dat);

			if (level == 1)play_sample(music[4].dat, 255, 128, 1000, TRUE);
			if (level == 2)play_sample(music[5].dat, 255, 128, 1000, TRUE);
			if (level == 3 || level == 4)play_sample(music[3].dat, 255, 128, 1000, TRUE);

		}
		/*checks what level the game is in and either quits loads a function
		or goes through the main game loop*/
		if (level == 3)tankgame();
		if (level == 4)tankgame2();
		if (level == QUIT)break;
		if (level == GAMEOVER)break;
		if (level == 5)break;
		/*this while statement keeps the game at a constant speed
		and is the main game loop*/
		while (speed_counter > 0)
		{
			updatescreen = 1;   /*screen can only be updated when
							  this integer is 1*/

			if (movies == 0)
			{
				if ((mouse_b & 1) && (mousepressed == 1))mousepressed = 2;   /*the left mouse has been pressed
																	   and is still pressed*/
				if (!(mouse_b & 1))mousepressed = 0;                        /*the left mouse is not pressed*/
				if ((mouse_b & 1) && (mousepressed == 0))mousepressed = 1;   /*the mousepressed integer is only
																	   equal to one directly after it is pressed*/
				if ((mouse_b & 2) && (mousepressedright == 1))mousepressedright = 2;   /*the left mouse has been pressed
																	   and is still pressed*/
				if (!(mouse_b & 2))mousepressedright = 0;                        /*the left mouse is not pressed*/
				if ((mouse_b & 2) && (mousepressedright == 0))mousepressedright = 1;   /*the mousepressed integer is only
																		equal to one directly after it is pressed*/

				if (talkback == 0)/*if talkback is 0 the player is
								 not talking to the screen and can
								 move around*/
				{

					if (showplayskip > 2)/*The player animation is only updated
										 after the screen has updated*/
					{
						showplayskip = 0;
						show_player();
					}

					input();          /*function used for user input*/


					/*I used this section when I was writing binary files
					for character movement during movies*/

					/*    if ( (key[KEY_J]) || (key[KEY_K]) || (key[KEY_I]) || (key[KEY_L]) )
						  badguy();
						if(seekslow==1)            Was used to check values in the binary
													 file and watch how character moved

						  {
						  if (key_shifts & KB_SHIFT_FLAG)
						  {
						  if (key[KEY_A]) follow++;
						  if (key[KEY_S]) follow--;
						  }
						  else
						  {
						  if (key[KEY_A]) follow+=20;
						  if (key[KEY_S]) follow-=20;
						  }
						  }
						  seekslow%=5;
						  seekslow++;

									   */



					collide = collision();  /*calls collision function and returns
										  a 0,1,2 or a 3
										  if it returns a 0 the player can not move
										  if it returns a 1 the player can move left or right
										  if it returns a 2 the player can move up or down
										  if it returns a 3 player can move any direction*/



					if (collide != 0)
					{
						if (collide == 1) moveplayerx();
						if (collide == 2) moveplayery();
						if (collide == 3) { moveplayerx(); moveplayery(); }
					}

				}
				if ((mousepressed == 1))clear_keybuf();  /*if the mouse is pressed
													   the player can not move with
													   the arrow keys*/

													   /*if statement used to stop the player movement*/
				if ((collide == 0) || (playermovex == 0 && collide == 1) || (playermovey == 0 && collide == 2) || (keypressed()) || talkback == 1)
					stopplayer();
				/*used to check when to switch the screen and some miscellaneous
				things*/
				screenswitch();
				/*used to check what object the mouse is on by checking
				the same bitmaps that are used for collision*/
				on_screen();
			}
			else
				moviesinput(movies);


			/*The speed counter is subtracted every scan that the
	   speed counter is greater than zero and added by the
	   bps function so that when the speed is subtracted
	   more than the bps function adds the while statement
	   will end and display the graphics
	   (if you look at the allegro faq document it explains it
	   also)*/
			speed_counter--;

		}


		/*The screen is only updated once every
		25fps*/
		if (updatescreen == 1)
		{
			frame_count++;    /*increments how many times the display
							  is called for the update*/

							  /*If the game is in the game play section movies is
							  equal to 0*/
			if (movies == 0)
			{
				/*after sample is done it can be called again*/
				if (playermove > 0 && voice_get_position(footstep) < 0)voice_start(footstep);
				/*updates the display and also if there is
				fire or snow effects it updates that too.*/
				display();
				/*used for changing collision bitmaps
				if a object is taken off the screen or if
				a wall is removed*/
				cdisplay();
			}
			else
			{

				moviedisplay1(movies);

			}

			/*clears lighting or makes screen a different color*/
			if (showscreen != 6 && showscreen != 7 && showscreen != 8 && showscreen != 10)
				clear_to_color(redlight, makecol(255, 0, 255));
			else
			{
				if (showscreen == 6 || showscreen == 7)clear_to_color(redlight, makecol(redlightcolor*1.2, 53 - redlightcolor / 4, 75 - redlightcolor / 2));
			}

			if (movies == 100)
			{
				if (introcount < 398 && introcount <= 250) clear_to_color(redlight, makecol(0, 0, 0));
				if (introcount > 250 && introcount < 398)clear_to_color(redlight, makecol(introcount - 185, introcount - 185 + introcount / 10, introcount - 185));
			}
			show_mouse(buffer);   /*shows the mouse after all drawing
								  to the buffer is done*/

								  /*If a movie is being shown the menu will disappear
								  and the display will appear larger*/
			if (movies == 0)
			{
				stretch_blit(buffer, screen,
					0, 0, 640, 600,
					0, 0, SCREEN_W, SCREEN_H);
			}
			else
			{
				stretch_blit(buffer, screen,
					0, 0, 640, 480,
					0, 0, SCREEN_W, SCREEN_H);
			}
			show_mouse(NULL);
			/*clears the buffer*/
			clear(buffer);
		}
		updatescreen = 0;      /*is set back to one once
							 the input is updated then the
							 screen can be drawn again*/


	}
	/*once the game is over the music stops and the midi music is played
	if the game is over a quit message will show up. If the player made it
	to the level I haven't started a congratulations message will show up.*/
	stop_sample(music[2].dat);
	stop_sample(music[3].dat);
	stop_sample(music[4].dat);
	play_midi(music[0].dat, 1);
	if (level == GAMEOVER)
	{
		clear_to_color(screen, makecol(128, 128, 128));
		sprintf(msg, "GAME OVER");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 30, 0);
		sprintf(msg, "PRESS ESC TO QUIT");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 60, 0);
		for (;;)
		{
			if (key[KEY_ESC])break;
		}
	}

	if (level == 5)
	{
		clear_to_color(screen, makecol(128, 128, 128));
		sprintf(msg, "GOOD JOB");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 30, 0);
		sprintf(msg, "HOPE YOU ENJOYED MY GAME");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 60, 0);
		sprintf(msg, "THE GAME IS IN PROGRESS");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 90, 0);
		sprintf(msg, "FEEL FREE TO SEND COMMENTS");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 120, 0);
		sprintf(msg, "AND QUESTIONS ON THE GAME");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 150, 0);
		sprintf(msg, "OR SOURCE TO RESTIVO@CONESTOGA.NET");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 180, 0);
		sprintf(msg, "I LIKE TO HERE OF IDEAS FOR THE GAME");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 210, 0);
		sprintf(msg, "OR ANY OTHER HELP YOU CAN GIVE");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 240, 0);
		sprintf(msg, "THANKS AGAIN AND BEST REGARDS, TINO");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 270, 0);
		sprintf(msg, "NEXT PASSWORD AFTER UPDATE IS:");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 300, 0);
		sprintf(msg, "LEVEL5");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 330, 0);
		sprintf(msg, "PRESS ESC TO QUIT");
		textout_centre(screen, datafile[22].dat, msg, SCREEN_W / 2, 360, 0);

		for (;;)
		{
			if (key[KEY_ESC])break;
		}
	}
	/* unloads all the memory used for the program*/
	deallocate_voice(footstep);
	for (c = 0; c < 3; c++)
	{
		destroy_bitmap(intro[c]);
	}
	for (c = 0; c < 9; c++)
	{
		destroy_bitmap(level1[c]);
	}
	for (c = 0; c < 9; c++)
	{
		destroy_bitmap(level2[c]);
	}
	for (c = 0; c < 30; c++)
	{
		destroy_bitmap(character[c]);
	}
	destroy_bitmap(buffer);
	destroy_bitmap(bufferc);
	destroy_bitmap(dialog);
	destroy_bitmap(redlight);

	unload_datafile(intro1);
	unload_datafile(chara);
	unload_datafile(level1dat);
	unload_datafile(level2dat);
	unload_datafile(datafile);
	unload_datafile(music);
	unload_datafile(level1cdat);
	unload_datafile(level2cdat);

	return 0;
}
END_OF_MAIN();


