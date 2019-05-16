//Menu Management
#include <cstdio>

#include "boatrage.h"
#include "wrappers.h"
#include "menu.h"
#include "player.h"
#include "sprite.h"
#include "control.h"
#include "objects.h"
#include "mapedit.h"
#include "strings.h"
#include "view.h"
#include "menumagm.h"
#include "tilemap.h"

//define the menus
//*****************
coolmenuitem m_mainmenu[] = {
	//NAMN     X  Y
	{S_NEWGAME,  M_CHOICE,40, 64,0,0,0,0},
	{S_OPTIONS,  M_CHOICE,40, 84,0,0,0,0},
	{S_EDITOR,   M_CHOICE,40,104,0,0,0,0},
	{S_CREDITS,  M_CHOICE,40,124,0,0,0,0},
	{S_QUIT,     M_CHOICE,40,148,0,0,0,0}
};

coolmenuitem m_newgamemenu[] = {
{S_SINGLER,  M_CHOICE,40, 64,0,0,0,0},
{S_SEASON,   M_CHOICE,40, 84,0,0,0,0},
{S_OWNLEVEL, M_CHOICE,40,104,0,0,0,0},
{S_OWNSEASON,M_CHOICE,40,124,0,0,0,0},
{S_BACK,     M_CHOICE,40,148,0,0,0,0}
};

coolmenuitem m_quitmenu[] = {
{S_EXITYES,  M_CHOICE,40,64,0,0,0,0},
{S_EXITNO,   M_CHOICE,40,84,0,0,0,0},
};

DIALOG settings[] = {
	//Typ          X   Y   W   H   FG     BG     Key    Flags    d1   d2   dp     dp2    dp3
	{d_clear_proc, 0,  0,  0,  0,  255,   0,     0,     0,       0,   0},
	{d_text_proc , 10, 80, 0,  0,  255,   0,     0,     0,       0,   0, S_OPTIONS},
	{d_text_proc , 10, 88, 0,  0,  255,   0,     0,     0,       0,   0, S_CHECKREADME},
	{d_check_proc, 20, 100,100,8,  255,   0,     0,     0,       0,   0, S_VSYNC},
	{d_button_proc,255,90, 60, 12, 255,   0,     0, D_EXIT,      0,   0, S_OK},
	{NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
};


//To vsync or not to vsync...That is the question!!! :-)
int synch = 0;

//FPS it tries to keep
int LOCKFPS = 60;

//The countdown!
volatile int countdown = 0;

//timing
volatile int fps, fpsc, fpsx, fpsxc;
volatile int target_cycle = 0;
int actual_cycle = 0;

int position = 0; //placering n„sta b†t kommer p?int quitseason=0; //vill spelaren avsluta s„songen?
int wannaquitthis = 0; //vill spelaren g?tillbaka?
int inseason = 0;   //„r 1 om man spelar en s„song

//h†lla hastigheten p?spelet
void speeder() {
	target_cycle++;
}
END_OF_FUNCTION(speeder);

//r„kna frames per second
void countfps() {
	fps = fpsc;
	fpsc = 0;
	fpsx = fpsxc;
	fpsxc = 0;
}
END_OF_FUNCTION(countfps);

//r„kna ned
void thecountdown() {
	countdown--;
}
END_OF_FUNCTION(thecountdown);

//K™R ETT RACE!!!!!!!!!!!!
void race(player &p1, player &p2, char *level) {
	int paused = 0;
	int q = 0; //quit?
	int quitseason = 0;
	text_mode(-1);

	letplayerupgrade(p1);
	if (wannaquitthis) return;
	letplayerupgrade(p2);
	if (wannaquitthis) return;

	BITMAP *p[2];   //the two screenhalves: subbitmaps of Display
	BITMAP *p_i[2]; //scoreboards: also subbitmaps of Display

	//create the subbitmaps
	p[0] = display.subbitmap(0, 40, display.w() / 2, display.h() - 40);
	p[1] = display.subbitmap(display.w() / 2, 40, display.w() / 2, display.h() - 40);
	p_i[0] = display.subbitmap(0, 0, display.w() / 2, 40);
	p_i[1] = display.subbitmap(display.w() / 2, 0, display.w() / 2, 40);

	//create the two views...
	view *v1 = new view(p[0]);
	view *v2 = new view(p[1]);

	//declare the boats...
	boat *b1;
	boat *b2;

	//controls
	ctl_keyboard Keyboard1(KEY_T, KEY_F, KEY_H);
	ctl_keyboard Keyboard2(KEY_UP, KEY_LEFT, KEY_RIGHT);

	//...and instanciate them.
	b1 = new boat(p1, Keyboard1);
	b2 = new boat(p2, Keyboard2);

	//boat coords (needed for loading positions from maps)
	boatcoord bc1, bc2;
	map->loadmap(level, bc1, bc2);
	b1->setcoords(bc1);
	b2->setcoords(bc2);

	//Make a bitmap to draw the countdown on
	BITMAP *btmp = create_bitmap(6, text_height((FONT *)d(FONT_SMALL)));

	//Start the three necessary timers
	install_int_ex(countfps, BPS_TO_TIMER(1));
	install_int_ex(speeder, BPS_TO_TIMER(LOCKFPS));
	install_int_ex(thecountdown, BPS_TO_TIMER(2));

	//reset the cycle counters...
	actual_cycle = 0;
	target_cycle = 0;
	//and the position and countdown.
	position = 0;
	countdown = 5;

	//set the correct palette...
	display.setstdpal();

	//and start the music!
	Music.playsong(map->getsong());

	//NOW LET'S LOOP! :-)
	do {
		//Pause?
		if (key[KEY_P] || key[KEY_PAUSE]) { if (paused) paused = 0; else paused = 1; key[KEY_P] = 0; key[KEY_PAUSE] = 0; }
		//Increase the FPS counter (press F10 to show)
		fpsc++;
		//let the views follow the boats
		v1->followboat(b1);
		v2->followboat(b2);
		//tell both views to draw what they see
		v1->draw();
		v2->draw();
		//draw a vertical line between the views
		vline(display, SCREEN_W / 2 - 1, 0, SCREEN_H, makecol(0, 0, 255));
		vline(display, SCREEN_W / 2 - 0, 0, SCREEN_H, makecol(0, 0, 128));
		//show scoreboard
		b1->drawinfo(p_i[0]);
		b2->drawinfo(p_i[1]);
		//finished?
		if (b1->isfinished()) {
			textprintf(p[0], (FONT *)d(FONT_SLICKER), 10, 100, -1, S_POSITION, p1.lastposition);
		}
		if (b2->isfinished()) {
			textprintf(p[1], (FONT *)d(FONT_SLICKER), 10, 100, -1, S_POSITION, p2.lastposition);
		}
		//if counting down...
		if (countdown >= 0) {
			//let's update the counter bitmap and stretch it to the screen
			clear(btmp);
			textprintf(btmp, (FONT *)d(FONT_SMALL), 0, 0, 31, "%i", countdown);
			stretch_sprite(display, btmp, SCREEN_W / 2 - 50, SCREEN_H / 2 - 50, 100, 100);
		}
		//finished counting? If so, stop the timer.
		if (countdown == -1) {
			remove_int(thecountdown);
		}
		//Show FPS with F10
		if (key[KEY_F10]) textprintf_centre(display, (FONT *)d(FONT_SLICKER), SCREEN_W / 2, 100, -1, "FPS: %i", fps);
		//Sync to display refresh
		if (synch) vsync();

		//We're finished! Send to the screen!
		display.flip();

		//Let the game progress
		while (target_cycle > actual_cycle) {
			if (!paused) sprite::moveall();
			actual_cycle++;
			fpsxc++;
		}

		//Quit?
		if (key[KEY_ESC]) {
			if (inseason) {
				if (b1->isfinished() && b2->isfinished()) {
					q = 1;
				}
				else {
					if (alert(S_QUITSEASON1, S_QUITSEASON2, NULL, S_YES, S_NO, S_Y, S_N) == 1) {
						quitseason = 1;
						q = 1;
					}
				}
			}
			else {
				q = 1;
			}
		}
	} while (q == 0);

	//stop the boat noises
	b1->goquiet();
	b2->goquiet();

	//close the map
	map->closemap(1);

	//stop timers
	remove_int(countfps);
	remove_int(speeder);
	remove_int(thecountdown);

	//crush the camera lenses against a rock :-)
	delete v1;
	delete v2;

	//destroy all temporary stuff
	destroy_bitmap(p[0]);
	destroy_bitmap(p[1]);
	destroy_bitmap(p_i[0]);
	destroy_bitmap(p_i[1]);
	destroy_bitmap(btmp);

	//and fade out!
	display.fadeout();
}

//shows the credits
void credits() {
	text_mode(-1);
	int x = SCR_X + 20, y = SCR_Y;
	clear(display);
	clear(screen);
	display.setstdpal();
	FONT *rf = (FONT *)d(FONT_RAVE);
	do {
		clear(display);
		y = SCR_Y;
		textprintf(display, rf, x, y += 14, -1, S_GAMENAME);
		textprintf(display, rf, x, y += 14, -1, S_CODING);
		textprintf(display, rf, x + 40, y += 14, -1, S_AUTHOR);
		textprintf(display, rf, x, y += 14, -1, S_GRAPHICS);
		textprintf(display, rf, x + 40, y += 14, -1, S_AUTHOR);
		textprintf(display, rf, x, y += 14, -1, S_SOUNDFX);
		textprintf(display, rf, x + 40, y += 14, -1, S_AUTHOR);
		textprintf(display, rf, x, y += 14, -1, S_MUSIC);
		textprintf(display, rf, x + 40, y += 14, -1, S_AUTHORANDNET);
		textprintf(display, rf, x, y += 14, -1, S_LEVELS);
		textprintf(display, rf, x + 40, y += 14, -1, S_AUTHOR);
		textprintf(display, rf, x + 40, y += 14, -1, S_COAUTHOR);
		vsync();
		blit(display, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	} while (!key[KEY_ESC]);
	display.fadeout(4);
}

//prepares for single race
void singlerace() {
	char file[16];
	inseason = 0;
	wannaquitthis = 0;
	player player1(S_PLAYER1, 0), player2(S_PLAYER2, 1);
	player1.money = 20000;
	player2.money = 20000;
	sprintf(file, "l%i.boa", selectlevel());
	if (wannaquitthis) return;
	race(player1, player2, file);
}

//prepares for custom race
void customrace() {
	char file[255];
	int ret;
	wannaquitthis = 0;
	player player1(S_PLAYER1, 0), player2(S_PLAYER2, 1);
	player1.money = 20000;
	player2.money = 20000;
	clear(display);
	clear(screen);
	display.setpal(desktop_palette);
	ret = file_select(S_SELECTLEVEL, file, "BOA");
	if (ret == 0) return;
	if (!exists(file)) {
		alert(S_FILEDONTEXIST, NULL, NULL, S_OK, NULL, S_O, NULL);
		return;
	}
	showlevelinfo(file);
	race(player1, player2, file);
}

//prepares for season
void season() {
	int i, num;
	char file[12];
	int quitseason = 0;
	wannaquitthis = 0;
	inseason = 1;
	player player1(S_PLAYER1, 0), player2(S_PLAYER1, 1);
	player1.money = 300;
	player2.money = 300;

	for (i = 1; ; i++) {
		sprintf(file, "l%i.boa", i);
		if (!exists(file)) break;
	}
	num = i;
	for (i = 1; i < num; i++) {
		selectlevel(i, 1);
		if (wannaquitthis) break;
		sprintf(file, "l%i.boa", i);
		race(player1, player2, file);
		if (wannaquitthis) break;
		if (quitseason) { inseason = 0; return; }
		clear(display);
		afterrace(player1, player2, i);
	}
	inseason = 0;
}

//prepares for custom season
void customseason() {
	int i, num, ret;
	char file[255];
	int quitseason = 0;
	wannaquitthis = 0;
	inseason = 1;
	player player1(S_PLAYER1, 0), player2(S_PLAYER2, 1);
	player1.money = 300;
	player2.money = 300;

	for (i = 1; ; i++) {
		sprintf(file, "l%i.boa", i);
		if (!exists(file)) break;
	}

	num = i;
	for (i = 1; i < num; i++) {
		clear(display);
		clear(screen);
	selectthelevel:
		set_palette(desktop_palette);
		ret = file_select(S_SELECTLEVEL, file, "BOA");
		if (ret == 0) return;
		if (!exists(file)) {
			alert(S_FILEDONTEXIST, NULL, NULL, S_OK, NULL, S_O, NULL);
			goto selectthelevel;
		}
		showlevelinfo(file);
		if (wannaquitthis) break;
		race(player1, player2, file);
		if (wannaquitthis) break;
		if (quitseason) { inseason = 0; return; }
		clear(display);
		afterrace(player1, player2, i);
	}
	inseason = 0;
}

//shows the new game menu
void newgamemenu() {
	int quit = false, a = 0;
	do {
		a = coolmenu((RLE_SPRITE *)d(BOATRAGE), m_newgamemenu, 5, a);
		switch (a) {
		case 0:
			singlerace();
			quit = true;
			break;
		case 1:
			season();
			quit = true;
			break;
		case 2:
			customrace();
			quit = true;
			break;
		case 3:
			customseason();
			quit = true;
			break;
		case -1:
		case 4:
			quit = true;
			break;
		}
	} while (!quit);
}

//shows the Sure you wanna quit? menu
int quitmenu() {
	int a;
	a = coolmenu((RLE_SPRITE *)d(BOATRAGE), m_quitmenu, 2);
	if (a == 0) return 1;
	return 0;
}

//shows the main menu and init some stuff
void mainmenu() {
	int quit = false, a = 0;
	LOCK_FUNCTION(countfps);
	LOCK_FUNCTION(speeder);
	LOCK_FUNCTION(thecountdown);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(fpsc);
	LOCK_VARIABLE(fpsx);
	LOCK_VARIABLE(fpsxc);
	LOCK_VARIABLE(target_cycle);
	LOCK_VARIABLE(countdown);
	do {
		a = coolmenu((RLE_SPRITE *)d(BOATRAGE), m_mainmenu, 5, a);
		switch (a) {
		case 4: case -1:
			if (quitmenu()) quit = true;
			break;
		case 1:
			settings[3].flags = synch ? D_SELECTED : 0;
			set_palette(desktop_palette);
			do_dialog(settings, -1);
			display.setstdpal();
			synch = (settings[3].flags & D_SELECTED) ? 1 : 0;
			display.fadeout(4);
			break;
		case 2:
			if (!nomouse) {
				mapedit();
			}
			else {
				display.setstdpal();
				alert(S_NEEDMOUSEEDIT1, S_NEEDMOUSEEDIT2, NULL, S_OK, NULL, S_O, NULL);
				clear(screen);
			}
			break;
		case 0:
			newgamemenu();
			break;
		case 3:
			credits();
			break;
		}
	} while (!quit);
}

//let the user select a level
int selectlevel(int std, int still) {
	int l = std,
		changed = 1,
		tmp,
		x, y,
		first = 1;
	char file[12];
	boatcoord tmp1, tmp2;
	BITMAP *btmp1, *btmp2;

	text_mode(-1);
	Music.playsong(14);
	display.goblack();
	btmp1 = NULL;
	btmp2 = create_bitmap(128, 128);
	do {
		clear(display);
		draw_rle_sprite(display, (RLE_SPRITE *)d(CLOUDS), SCR_X, SCR_Y);
		if (changed) {
			Sound.play((SAMPLE *)d(SND_CHOOSE));
			changed = 0;
			sprintf(file, "l%i.boa", l);
			if (exists(file)) {
				map->closemap();
				map->loadmap(file, tmp1, tmp2);
				if (btmp1) destroy_bitmap(btmp1);
				btmp1 = create_bitmap(map->getwidth(), map->getheight());
				for (y = 0; y < map->getheight(); y++) {
					for (x = 0; x < map->getwidth(); x++) {
						tmp = map->get(x, y, 0);
						if (tmp < 0 || tmp>255) tmp = 0; //f”r att vara s„ker
						putpixel(btmp1, x, y, tiletypes[tmp].color);
					}
				}
				stretch_blit(btmp1, btmp2, 0, 0, btmp1->w, btmp1->h, 0, 0, btmp2->w, btmp2->h);
			}
			else {
				l--;
			}
		}
		textprintf(display, (FONT *)d(FONT_RAVE), SCR_X + 10, SCR_Y + 12, -1, S_LEVELNUMBERNAME, l, map->name);
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 27, 31, S_LEVELBY, map->creator);
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 37, 31, S_LEVELSIZE, map->getwidth(), map->getheight());
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 47, 31, S_LEVELLAPS, map->getlaps());
		if (still == 0) {
			textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 5, SCR_Y + 1, 63, S_SELECTLEVEL);
		}
		else {
			textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 5, SCR_Y + 1, 63, S_NEXTLEVEL);
		}
		//rita en liten karta
		rect(display, SCR_X + 10, SCR_Y + 60, SCR_X + btmp2->w + 1 + 10, SCR_Y + btmp2->h + 1 + 60, makecol(0, 0, 0));
		blit(btmp2, display, 0, 0, SCR_X + 11, SCR_Y + 61, btmp2->w, btmp2->h);
		vsync();
		blit(display, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		if (first) { first = 0; display.fadein(3); }
		if (still == 0) {
			if (key[KEY_DOWN]) { clear_keybuf(); l++; changed = 1; }
			if (key[KEY_UP] && l > 1) { clear_keybuf(); l--; changed = 1; }
		}
		if (key[KEY_ENTER]) {
			Sound.play((SAMPLE *)d(SND_SELECT));
			clear_keybuf();
			break;
		}
		if (key[KEY_ESC]) {
			wannaquitthis = 1;
			break;
		}
	} while (1);
	map->closemap();
	destroy_bitmap(btmp1);
	destroy_bitmap(btmp2);
	display.fadeout(3);
	return l;
}

//Show info 'bout a level
void showlevelinfo(char *file) {
	text_mode(-1);
	clear_keybuf();
	Music.playsong(14);
	int first = 1;
	display.goblack();
	boatcoord tmp1, tmp2;
	BITMAP *btmp1, *btmp2;
	btmp1 = NULL;
	btmp2 = create_bitmap(128, 128);
	map->closemap();
	map->loadmap(file, tmp1, tmp2);
	btmp1 = create_bitmap(map->getwidth(), map->getheight());
	map->drawminimap(btmp1);
	stretch_blit(btmp1, btmp2, 0, 0, btmp1->w, btmp1->h, 0, 0, btmp2->w, btmp2->h);

	do {
		clear(display);
		draw_rle_sprite(display, (RLE_SPRITE *)d(CLOUDS), SCR_X, SCR_Y);
		textprintf(display, (FONT *)d(FONT_RAVE), SCR_X + 10, SCR_Y + 12, -1, "%s", map->name);
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 27, 31, S_LEVELBY, map->creator);
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 37, 31, S_LEVELSIZE, map->getwidth(), map->getheight());
		textprintf(display, (FONT *)d(FONT_SMALL), SCR_X + 10, SCR_Y + 47, 31, S_LEVELLAPS, map->getlaps());

		rect(display, SCR_X + 10, SCR_Y + 60, SCR_X + btmp2->w + 1 + 10, SCR_Y + btmp2->h + 1 + 60, makecol(0, 0, 0));
		blit(btmp2, display, 0, 0, SCR_X + 11, SCR_Y + 61, btmp2->w, btmp2->h);
		vsync();
		blit(display, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		if (first) { first = 0; display.fadein(3); }
		if (key[KEY_ENTER]) {
			Sound.play((SAMPLE *)d(SND_SELECT));
			clear_keybuf();
			break;
		}
		if (key[KEY_ESC]) {
			wannaquitthis = 1;
			break;
		}
	} while (1);
	map->closemap();
	destroy_bitmap(btmp1);
	destroy_bitmap(btmp2);
	display.fadeout(3);
}

//let a player upgrade his boat
void letplayerupgrade(player &p) {
	text_mode(-1);
	int first = 1;
	int prop[2], what = 0, pr = 0, sn = 0;
	int x[2] = { 0 + SCR_X,160 + SCR_X };//,0+SCR_X};
	int y[2] = { 10 + SCR_Y,10 + SCR_Y };//,100+SCR_Y};
	FONT *rfont = (FONT *)d(FONT_RAVE),
		*sfont = (FONT *)d(FONT_SMALL),
		*nfont = (FONT *)d(FONT_NORMAL);

	prop[0] = p.engine;
	prop[1] = p.rudder;
	// prop[2]=p.armor;
	display.goblack();
	Music.playsong(14);
	do {
		clear(display);
		draw_rle_sprite(display, (RLE_SPRITE *)d(SKY), SCR_X, SCR_Y);
		if (key[KEY_UP] && prop[what] < 4) { clear_keybuf(); prop[what]++; sn = 1; }
		if (key[KEY_DOWN] && prop[what] > 0) { clear_keybuf(); prop[what]--; sn = 1; }
		if (key[KEY_LEFT] && what > 0) { clear_keybuf(); what--; sn = 1; }
		if (key[KEY_RIGHT] && what < 1) { clear_keybuf(); what++; sn = 1; }
		if (sn) {
			Sound.play((SAMPLE *)d(SND_CHOOSE)); sn = 0;
		}
		if (key[KEY_SPACE]) {
			clear_keybuf();
			switch (what) {
			case 0: pr = engines[prop[what]].price; break;
			case 1: pr = rudders[prop[what]].price; break;
				//  case 2: pr= armors[prop[what]].price; break;
			}

			if (p.money >= pr) {
				if (prop[what] < 5) {
					switch (what) {
					case 0:
						if (prop[what] > p.engine) {
							Sound.play((SAMPLE *)d(SND_SELECT));
							p.engine = prop[what];
							p.money -= pr;
						}
						break;
					case 1:
						if (prop[what] > p.rudder) {
							Sound.play((SAMPLE *)d(SND_SELECT));
							p.rudder = prop[what];
							p.money -= pr;
						}
						break;
						/*    case 2:
							   if (prop[what]>p.armor) {
								p.armor=prop[what];
								p.money-=pr;
							   }
							   break;*/
					}
				}
			}
			else {
				Sound.play((SAMPLE *)d(SND_BUZZER));
			}
		}
		textprintf(display, rfont, 3 + SCR_X, 3 + SCR_Y, -1, S_UPGRADEBOAT, p.name);
		textprintf(display, sfont, 3 + SCR_X, 14 + SCR_Y, makecol(255, 128, 0), S_YOURMONEY, p.money);
		textprintf(display, nfont, x[0] + 3, y[0] + 23, -1, engines[prop[0]].name);
		textprintf(display, nfont, x[1] + 3, y[1] + 23, -1, rudders[prop[1]].name);
		//  textprintf(display,(FONT *)d(FONT_NORMAL),x[2]+3,y[2]+3,-1,  armors[prop[2]].name);
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		switch (what) {
		case 0:
			if (prop[what] == p.engine)
				textprintf(display, sfont, x[0] + 3, y[0] + 30, 47, S_YOUHAVE);
			break;
		case 1:
			if (prop[what] == p.rudder)
				textprintf(display, sfont, x[1] + 3, y[1] + 30, 47, S_YOUHAVE);
			break;
		}
		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		textprintf(display, sfont, x[0] + 3, y[0] + 71, 47, S_COSTS, engines[prop[0]].price);
		textprintf(display, sfont, x[1] + 3, y[1] + 71, 47, S_COSTS, rudders[prop[1]].price);
		//textprintf(display,(FONT *)d(FONT_SMALL) ,x[2]+3,y[2]+71,47,"Kostar $%i", armors[prop[2]].price);
		textprintf(display, nfont, x[0] + 3, y[0] + 80, -1, engines[prop[0]].info1);
		textprintf(display, nfont, x[1] + 3, y[1] + 80, -1, rudders[prop[1]].info1);
		//textprintf(display,(FONT *)d(FONT_NORMAL),x[2]+3,y[2]+80,-1, armors[prop[2]].info1);
		textprintf(display, nfont, x[0] + 3, y[0] + 88, -1, engines[prop[0]].info2);
		textprintf(display, nfont, x[1] + 3, y[1] + 88, -1, rudders[prop[1]].info2);
		//textprintf(display,(FONT *)d(FONT_NORMAL),x[2]+3,y[2]+88,-1, armors[prop[2]].info2);
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		rectfill(display, x[what] + 3, y[what] + 22, x[what] + 157, y[what] + 70, makecol(0, 64, 128));
		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		textprintf(display, nfont, SCR_X + 10, SCR_Y + 130, -1, S_BUYUPGRADES1);
		textprintf(display, nfont, SCR_X + 10, SCR_Y + 140, -1, S_BUYUPGRADES2);

		display.flip();
		if (first) { first = 0; display.fadein(3); }
		if (key[KEY_ENTER]) break;
		if (key[KEY_ESC]) { wannaquitthis = 1; break; }
	} while (1);
	display.fadeout(4);
	clear_keybuf();
}

//shows after-race results
void afterrace(player &p1, player &p2, int moneyamount) {
	int first = 1, x = SCR_X, y = SCR_Y;
	int winmoney = (moneyamount * 200 + 500);
	int losemoney = (moneyamount * 100 + 300);
	player &w = (p1.lastposition == 1 ? p1 : p2), &l = (p1.lastposition == 1 ? p2 : p1); //winner,loser
	w.money += winmoney;
	l.money += losemoney;
	display.goblack();
	do {
		clear(display);
		draw_rle_sprite(display, (RLE_SPRITE *)d(ENDINGSEA), SCR_X, SCR_Y);
		textprintf(display, (FONT *)d(FONT_RAVE), x + 70, y + 38, -1, "%s", w.name);
		textprintf(display, (FONT *)d(FONT_RAVE), x + 70, y + 83, -1, "%s", l.name);
		textprintf(display, (FONT *)d(FONT_NORMAL), x + 230, y + 26, -1, "$%i", winmoney);
		textprintf(display, (FONT *)d(FONT_NORMAL), x + 230, y + 42, -1, "$%i", w.money);
		textprintf(display, (FONT *)d(FONT_NORMAL), x + 230, y + 68, -1, "$%i", losemoney);
		textprintf(display, (FONT *)d(FONT_NORMAL), x + 230, y + 84, -1, "$%i", l.money);
		vsync();
		display.flip();
		if (first) { first = 0; display.fadein(3); }
	} while (!key[KEY_ESC] && !key[KEY_ENTER]);
	Sound.play((SAMPLE *)d(SND_SELECT));
	display.fadeout(4);
	clear_keybuf();
}


