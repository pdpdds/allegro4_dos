/*
SAMPLE BRICKBREAKER GAME
Developed by Chris DeLeon
August 2009
For part of the Free Newsletter Vol. 5 of
www.GameDevLessons.com

Instructions: Click to reset the ball, move mouse to position paddle

This code is written with clarity and understandability to a beginner in mind.
It is not intended to represent the most optimized or most organized way
that the code could possibly be prepared.

Allegro is required to compile this application. Find instructions here:
http://gamedevlessons.com/lessons/letter5.html
(All software needed to compile this, for Mac/PC/Linux, is free!)

Files that are part of this project should be:
  core.cpp
  core.h
  main.cpp

Feel free to make changes, experiment, and distribute. Please leave this
comment at the top of the file, since it points back to my instructional
resources. My mission is to help people - your help in this is appreciated!
*/

#include"core.h"

BITMAP *screenBuffer;

void techInit()
{

	allegro_init(); // all Allegro programs need this

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else
					/* set a graphics mode sized 320x200 */
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return;
		}
	}

	//set_color_depth(32);
	//set_gfx_mode(GFX_AUTODETECT, 1024, 768, 0, 0);

	screenBuffer = create_bitmap(screen->w, screen->h); //Set up double buffer
	if (!screenBuffer) {
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}

	/* set the color palette */
	set_palette(desktop_palette);

	install_keyboard();
	install_mouse();
	/* add other initializations here*/
}

void updateScreen() {
	// draw the game's action to the screen
	blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}
