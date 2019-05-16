/****************************************************************************
*BoatRage!          file: BOATRAGE.CC                                       *
*****************************************************************************
*Emailware by Zenon (Henrik Rydg†rd)                                        *
*This code and this program may be distributed for free, but please email me*
*first.                                                                     *
*henrik_83_@hotmail.com (don't forget any of the underscores...).           *
*                                                                           *
*Zenon                                                                      *
****************************************************************************/
#include <iostream> //you should know what this is
#include "boatrage.h" //Some global stuff + allegro.h


#include "wrappers.h" //the classes display, Sound, and Music.
#include "sprite.h"   //the Sprite class
#include "player.h"   //the Player class, which keeps track of if you won and stuff like that
#include "control.h"
#include "objects.h"  //The boat and light classes, which keeps tracks of boats and lighting FX
#include "tilemap.h"  //The Tilemap-class which contains the current map

//We only need 8-bit color
//BEGIN_COLOR_DEPTH_LIST
// COLOR_DEPTH_8
//END_COLOR_DEPTH_LIST

//This is the big fat datafile
DATAFILE *data;

//all kinds of tiles
tiletype tiletypes[255];

//if you are running in hires, these keep track of the small screen
int SCR_X, SCR_Y, SCR_H, SCR_W;

//the number of different tiles + 1
int MAXTILES = 71;

//a couple of flags
int nosound = 0;
int nomouse = 0;

//these are not really used yet
boattype boattypes[] = {
 {"Urbans HB","PowerRacer",BOAT1, 2 << 12, 2 << 16, ftofix(1.05),ftofix(1.0)},
 {"Shanghai","Badkaret",   BOAT2, 2 << 12, 2 << 16, ftofix(1.05),ftofix(1.0)}
};

//Ž™†„”=ÅÄÖåäö

//I intended to have pictures of the different stuff, but I didn't get around
//to draw them. But if I do, I'll remove these constants so that code using
//them points to the new datafile objects (pictures) instead.
#define ENG01 0
#define ENG02 0
#define ENG03 0
#define ENG04 0
#define ENG05 0

#define RUD01 0
#define RUD02 0
#define RUD03 0
#define RUD04 0
#define RUD05 0

/**********
* Engines *
**********/
upgrade engines[] = {
	//NAME      DESCRIPTION #1 & #2 PIC PRICE ACC TURN unused
	{S_ENG1NAME,S_ENG1D1,S_ENG1D2,ENG01,   0,1.0,1.0,1.0,1.0},
	{S_ENG2NAME,S_ENG2D1,S_ENG2D2,ENG02, 300,1.2,1.0,1.0,1.0},
	{S_ENG3NAME,S_ENG3D1,S_ENG3D2,ENG03,1000,1.5,1.0,1.0,1.0},
	{S_ENG4NAME,S_ENG4D1,S_ENG4D2,ENG04,2700,1.9,1.0,1.0,1.0},
	{S_ENG5NAME,S_ENG5D1,S_ENG5D2,ENG05,4500,2.5,1.0,1.0,1.0},
};

/**********
*  RUDDER *
**********/
upgrade rudders[] = {
	//NAME      DESCRIPTION #1 & #2 PIC PRICE ACC TURN unused
	{S_RUD1NAME,S_RUD1D1,S_RUD1D2,RUD01,   0,1.0,1.0,1.0,1.0},
	{S_RUD2NAME,S_RUD2D1,S_RUD2D2,RUD02, 300,1.0,1.3,1.0,1.0},
	{S_RUD3NAME,S_RUD3D1,S_RUD3D2,RUD03,1000,1.0,1.6,1.0,1.0},
	{S_RUD4NAME,S_RUD4D1,S_RUD4D2,RUD04,2700,1.0,1.9,1.0,1.0},
	{S_RUD5NAME,S_RUD5D1,S_RUD5D2,RUD05,4500,1.0,2.7,1.0,1.0},
};
#include "MENU.H"

coolmenuitem m_resolution[] = {
	//NAME                     X   Y
	{"320x200 VGA",   M_CHOICE,40, 24,0,0,0,0},
	{"320x240 ModeX", M_CHOICE,40, 42,0,0,0,0},
	{"320x240 VESA recom.",M_CHOICE,40,60,0,0,0,0},
	{"400x300 VESA",  M_CHOICE,40, 78,0,0,0,0},
	{"512x384 VESA",  M_CHOICE,40, 96,0,0,0,0},
	{"640x480 VESA",  M_CHOICE,40, 114,0,0,0,0},
	{"800x600 VESA",  M_CHOICE,40, 132,0,0,0,0},
};


//The Allegro 3.9 version of my color addition function.
//As this is only run when the program is started (creating a lookuptable),
//there is no point optimizing it.

//NOTE: If you use an earlier version of allegro, you'll need to replace this
//function with the commented out one which comes after this one.l
void return_added_color(const PALETTE pal, int x, int y, RGB *rgb) {
	int r, g, b;
	//add the colors together
	r = pal[x].r + pal[y].r;
	g = pal[x].g + pal[y].g;
	b = pal[x].b + pal[y].b;
	//bounds clip
	if (r > 63) r = 63;
	if (g > 63) g = 63;
	if (b > 63) b = 63;
	//and assign to rgb
	rgb->r = r;
	rgb->g = g;
	rgb->b = b;
}

/*
RGB return_added_color(PALETTE pal, int x, int y) {
 RGB rgb;
 int r,g,b;
 r=pal[x].r+pal[y].r;
 g=pal[x].g+pal[y].g;
 b=pal[x].b+pal[y].b;
 if (r>63) r=63;
 if (g>63) g=63;
 if (b>63) b=63;
 rgb.r=r;
 rgb.g=g;
 rgb.b=b;
 return rgb;
}
*/

using std::cout;
using std::endl;

int init() {
	int i;

	//tell the user which game he/she is starting :-)
	cout << S_STARTING1 << endl;
	cout << S_STARTING2 << endl << endl;

	//initialize allegro
	if (allegro_init() != 0) {
		cout << S_ERRALLEG << endl;
		return 0;
	}
	else {
		cout << S_OKALLEG << endl;
	}

	//load the palette
	//transparent text drawing
	text_mode(-1);
	//set 320x200
#ifdef _WIN32
	display.set_graphics_mode(GFX_AUTODETECT_WINDOWED, 8, 320, 200);
#else
	/* set a graphics mode sized 320x200 */
	display.set_graphics_mode(GFX_AUTODETECT, 8, 320, 200);
#endif
	

	//tell the user that we're loading the datafile
	cout << S_LOADING << " boatrage.dat" << endl;

	//and load it!
	data = load_datafile("boatrage.dat");
	if (data == NULL) {
		cout << S_ERRLOADING1 << "boatrage.dat" << S_ERRLOADING2 << endl;
		return 0;
	}

	PALETTE tmppal;
	memcpy(tmppal, d(GAMEPAL), sizeof(PALETTE));
	display.setthestdpal(tmppal);
	//fill lookup-tables
	display.creatergbtable();
	display.createcolortable(return_added_color);

	//load the mouse driver
	if ((i = install_mouse()) == 0) {
		cout << S_ERRMOUSE << endl;
		nomouse = 1;
	}
	else {
		cout << i << S_OKMOUSE << endl;
	}
	//load a sound driver
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
		cout << S_ERRSND << endl;
		nosound = 1;
	}
	else {
		cout << S_OKSND << endl;
	}
	//load the keyboard driver
	if (install_keyboard() != 0) {
		cout << S_ERRKEYB << endl;
		return 0;
	}
	else {
		cout << S_OKKEYB << endl;
	}
	//load the timer driver
	if (install_timer() != 0) {
		cout << S_ERRTIMER << endl;
		return 0;
	}
	else {
		cout << S_OKTIMER << endl;
	}


	//tell the user that we're loading graphics (though we're actually just setting
	//up variables and filling look-up tables)
	cout << S_LOADINGGFX << endl;

	//set default values
	SCR_X = 0; SCR_Y = 0;
	SCR_W = 320; SCR_H = 200;
	//set map to be a new tilemap instance
	map = new tilemap();
	//show the resolution menu
	int a = coolmenu(NULL, m_resolution, 7, 2);
#ifdef _WIN32
	int sw, sh, mode = GFX_AUTODETECT_WINDOWED;
#else
	int sw, sh, mode = GFX_AUTODETECT;
#endif
	switch (a) {
	case 0:
		goto initproceed; //Yuck! A goto! :-(
	case 1:
		sw = 320; sh = 240; break;
	case 2:
		sw = 320; sh = 240; break;
	case 3:
		sw = 400; sh = 300; break;
	case 4:
		sw = 512; sh = 384; break;
	case 5:
		sw = 640; sh = 480; break;
	case 6:
		sw = 800; sh = 600; break;
	default:
		sw = 320; sh = 200; break;
	}
#ifdef _WIN32	
	//Now set the REAL mode we're gonna use!
	if (!display.set_graphics_mode(mode, 8, sw, sh)) {
		alert(S_GFXFAIL1, S_GFXFAIL2, 0, S_OK, 0, S_O, 0);
	}
#endif

	//position the menu screen
	SCR_X = (SCREEN_W - SCR_W) / 2;
	SCR_Y = (SCREEN_H - SCR_H) / 2;

initproceed:
	//fill tiletype list
	for (i = 0; i < 256; i++) {
		tiletypes[i].bmp = NULL;
		tiletypes[i].loaded = false;
	}
	for (i = 0; i < MAXTILES; i++) {
		tiletypes[i].bmp = (RLE_SPRITE *)d(TILE000 + i);
		tiletypes[i].loaded = true;
		tiletypes[i].action = A_EDGE;
	}
	tiletypes[0].action = A_NONE;
	tiletypes[0].color = makecol(0, 0, 128);
	tiletypes[1].action = A_NONE;
	tiletypes[1].color = makecol(0, 0, 96);
	tiletypes[2].action = A_BLOCK;
	tiletypes[2].color = makecol(0, 192, 0);
	tiletypes[31].action = A_BLOCK;
	tiletypes[31].color = makecol(206, 121, 48);
	tiletypes[63].action = A_GOAL;
	tiletypes[63].color = makecol(255, 255, 255);
	for (i = 64; i < 67; i++) {
		tiletypes[i].action = A_CP1 + i - 64;
		tiletypes[i].color = makecol(128, 192, 255);
	}
	for (i = 67; i < 71; i++)
		tiletypes[i].action = A_SPLEFT + i - 67;
	//we're finished!
	return 1;
}

//#undef main
#include "menumagm.h"
int main(int argc, const char *argv[]) {
	set_volume(255, 225);
	//try to init; if it didn't succed, bail out.
	if (!init()) return -1;
	//start da muzak!
	Music.playsong(menumusic);
	//darken the screen...
	display.goblack();
	//...draw the Zenon presents screen....
	draw_rle_sprite(screen, (RLE_SPRITE *)d(ZENONPRESENTERAR), SCR_X, SCR_Y);
	//...fade in...
	display.fadein(3);
	//...wait...
	// Sleep(1500); //delay(1500);
	//...and fade out!
	display.fadeout(3);
	//Now show the main menu!
	mainmenu();
	return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif
