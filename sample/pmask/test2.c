/*

This program is written as a demonstration of PMASK and COLLIST

*/
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "pmask.h"
#include "collist.h"
#include "get_time.h"

//#define COORDINATES_WRAP


#ifdef _DEBUG
#	define res_type  GFX_AUTODETECT_WINDOWED
#else
#	define res_type  GFX_AUTODETECT
//#	define res_type  GFX_AUTODETECT_WINDOWED
#endif

enum {

	res_x = 640
,	res_y = 480
,	res_z = 16

,	physics_x = 7000
,	physics_y = 7000
,	starting_asteroids = 8000
,	max_asteroids      = 50000

};

#define HUD_SCALE (1.0 / 40)
#define BUMP_DIST 0.25
#define HUD_ASTSIZE 0

int display_hud = 1;

#define file_name1 "a.dat"

double atime4;

typedef struct ASTDATA {
	float x, y;
	float vx, vy;
	float angle, spin;
	float mass;
} ASTDATA;

float view_x = 0;
float view_y = 0;
int sprite_w, sprite_h;
int num_asteroids = 0;
//named test2_drawing_mode because of a namespace collision with Allegro
int test2_drawing_mode = 0;//0 = sprite drawing, 1 = mask drawing
int collision_mode = 1;
enum { num_collision_modes = 3
};
ASTDATA asteroid[max_asteroids];
BITMAP *vscreen = NULL;//screen buffer
BITMAP *hud = NULL;
BITMAP *sprite[64];
PMASK *mask[64];

COLLIST_LIST *collist = NULL;

#ifndef PI
#	define PI 3.14159265358979
#endif

static float MIN_DIFF (float a, float b, float c) {
	a -= b;
	a += c / 2;
	a = fmod(a, c);
	if (a < 0) a += c;
	a -= c / 2;
	return a;
}

void remove_asteroid() {
	if (num_asteroids == 0) return;
	num_asteroids -= 1;
//	remove_from_collist (collist, &asteroid[num_asteroids]);
}
void collide_asteroids ( ASTDATA *a1, ASTDATA *a2 ) {
	//figuring out some quantities we'll need
#	ifdef COORDINATES_WRAP
		float dx = MIN_DIFF(a1->x, a2->x, physics_x);
		float dy = MIN_DIFF(a1->y, a2->y, physics_y);
#	else
		float dx = a1->x - a2->x;
		float dy = a1->y - a2->y;
#	endif
	float dvx = a1->vx - a2->vx;
	float dvy = a1->vy - a2->vy;
	float distance = sqrt(dx * dx + dy * dy);
	float impact = -2.0 / distance * (dx * dvx + dy * dvy);
	int frame1 = 63 & (int)(64 + a1->angle * 64 / PI / 2);
	int frame2 = 63 & (int)(64 + a2->angle * 64 / PI / 2);
	if (!check_pmask_collision(mask[frame1], mask[frame2], (int)a1->x, (int)a1->y, (int)a2->x, (int) a2->y)) {
		return;//not really a collision, just a bounding-box-overlap, so ignore it
	}

	if (distance == 0) return;//we can't handle this...

	dx /= distance;
	dy /= distance;
	//move each one BUMP_DIST pixels further away, just because
	a1->x += dx * BUMP_DIST;
	a1->y += dy * BUMP_DIST;
	a2->x -= dx * BUMP_DIST;
	a2->y -= dy * BUMP_DIST;
	//adjust velocities for newtonian physics
	if (impact < 0) return;//weird collision (should only happen in unusual circumstances)
	impact = impact / (a1->mass + a2->mass);
	a1->vx += impact * a2->mass * dx;
	a1->vy += impact * a2->mass * dy;
	a2->vx -= impact * a1->mass * dx;
	a2->vy -= impact * a1->mass * dy;
	return;
}
void get_asteroid_callback(COLLIST_ITEM *coldata) {
	ASTDATA *a = (ASTDATA*) coldata->userdata;
	int index;
	if (a >= &asteroid[num_asteroids]) {
		remove_from_collist_during_callback(coldata);
		return;
	}
	index = 63 & (int)(64 + a->angle * 64 / PI / 2);
	coldata->x = a->x;
	coldata->y = a->y;
	coldata->w = mask[index]->w;
	coldata->h = mask[index]->h;
//	coldata->userdata = a;
}
int collisions_callback(int num_collisions, void **data) {
	int i;
	ASTDATA **collision_list = (ASTDATA **) data;
	for (i = 0; i < num_collisions; i += 1) {
		collide_asteroids(
			collision_list[i * 2 + 1], 
			collision_list[i * 2 + 2]
		);
	}
	return 0;
}
void add_asteroid() {
	int n = num_asteroids;
	if (n == max_asteroids) return;
	num_asteroids += 1;
#	ifdef COORDINATES_WRAP
		asteroid[n].x = physics_x * rand() / (float)RAND_MAX;
		asteroid[n].y = physics_y * rand() / (float)RAND_MAX;
#	else
		asteroid[n].x = (physics_x-mask[0]->w) * rand() / (float)RAND_MAX;
		asteroid[n].y = (physics_y-mask[0]->h) * rand() / (float)RAND_MAX;
#	endif
	asteroid[n].vx = 1.0 * (2.0 * (rand() / (float)RAND_MAX - 0.5));
	asteroid[n].vy = 1.0 * (2.0 * (rand() / (float)RAND_MAX - 0.5));
	asteroid[n].angle = PI * 2 * (rand() / (float)RAND_MAX);
	while (asteroid[n].angle >= PI * 2) asteroid[n].angle -= PI * 2;
	asteroid[n].spin = 1.0 / 64 * PI * 2 * sqrt(rand() / (float)RAND_MAX);
	if (rand() & 1) asteroid[n].spin *= -1;
	asteroid[n].mass = 1.0 + 10.0 * (rand() / (float)RAND_MAX);
	add_to_collist(collist, &asteroid[n]);
}

int count_collisions1=0, count_collisions2=0;

void do_collisions ( int collision_mode ) {
	int are_we_counting;
	int c = -1;
	if (key[KEY_C]) are_we_counting = 1;
	else are_we_counting = 0;

	if (are_we_counting) {
		count_collisions2 = _do_collist_collisions(collist, NULL, NULL, SORT_BUBBLE, 1);
		//count_collisions2 = do_collist_collisions(collist, NULL, NULL);
	}

	switch (collision_mode) {
		case 0: {
			//c = do_collist_collisions(collist, collisions_callback, NULL);
			c = _do_collist_collisions(collist, collisions_callback, NULL, SORT_BUBBLE, 1);
		}
		break;
		case 1: {
			//c = _do_collist_collisions_wrap(collist, collisions_callback, NULL, physics_x, physics_y, SORT_BUBBLE, 1);
			c = _do_collist_collisions(collist, collisions_callback, NULL, SORT_BUBBLE, 0);
//			c = _find_collisions_2d2_indirect(collist, 1, collisions_callback, NULL);

//			c = do_collist_collisions3(collist, collisions_callback, NULL);
		}
		break;
		case 2: {
			c = _do_collist_collisions(collist, collisions_callback, NULL, SORT_BUBBLE, 0);
//			_update_collist(collist, SORT_BUBBLE);
//			c = _find_collisions_2d2_indirect(collist, 8, collisions_callback, NULL);
//			c = _find_collisions_2d2_indirect(collist, 32, NULL, NULL);
//			c = do_collist_collisions3(collist, collisions_callback, NULL);
/*			int i;
			COLLIST_ITEM *collidable = NULL;
			collidable = malloc(sizeof(COLLIST_ITEM) * num_asteroids);
			for (i = 0; i < num_asteroids; i += 1) {
				collidable[i].userdata = &asteroid[i];
				get_asteroid_callback(&collidable[i]);
			}
			c = find_collisions3 ( &collidable[0], num_asteroids, collisions_callback, NULL);
			free(collidable);*/
		}
		break;
		default: {
		}
		break;
	}
	if (are_we_counting) count_collisions1 = c;
	return;
}
void do_physics(float dt) {
	int i;
	for (i = 0; i < num_asteroids; i += 1) {
		float d;
		float tspin;
		asteroid[i].x += asteroid[i].vx * dt;
		asteroid[i].y += asteroid[i].vy * dt;
#		ifdef COORDINATES_WRAP
			while (asteroid[i].x < 0) 
				asteroid[i].x += physics_x;
			while (asteroid[i].x >= physics_x) 
				asteroid[i].x -= physics_x;
			while (asteroid[i].y < 0) 
				asteroid[i].y += physics_y;
			while (asteroid[i].y >= physics_y) 
				asteroid[i].y -= physics_y;
#		else
			d = asteroid[i].x;
			if (d < 0) {
				asteroid[i].x = -d;
				asteroid[i].vx = fabs(asteroid[i].vx);
			}
			d = asteroid[i].x + sprite_w - physics_x;
			if (d > 0) {
				asteroid[i].x -= 2 * d;
				asteroid[i].vx = -1 * fabs(asteroid[i].vx);
			}
//			if ((asteroid[i].x < 0) || (asteroid[i].x > physics_x)) _asm {int 3};
			d = asteroid[i].y;
			if (d < 0) {
				asteroid[i].y = -d;
				asteroid[i].vy = fabs(asteroid[i].vy);
			}
			d = asteroid[i].y + sprite_h - physics_y;
			if (d > 0) {
				asteroid[i].y -= 2 * d;
				asteroid[i].vy = -1 * fabs(asteroid[i].vy);
			}
//			if ((asteroid[i].y < 0) || (asteroid[i].y > physics_y)) _asm {int 3};
#		endif
		//asteroid[i].angle += asteroid[i].spin * dt;
		tspin = sqrt(asteroid[i].vx * asteroid[i].vx + asteroid[i].vy * asteroid[i].vy);
		tspin += 0.4;
		asteroid[i].angle += asteroid[i].spin * dt * tspin / 3;
		while (asteroid[i].angle < 0) asteroid[i].angle += PI * 2;
		while (asteroid[i].angle >= PI * 2) asteroid[i].angle -= PI * 2;
	}
	do_collisions(collision_mode);
	return;
}
void do_graphics() {
	int i;
	acquire_bitmap(vscreen);
	for (i = 0; i < num_asteroids; i += 1) {
		float x = asteroid[i].x - view_x;
		float y = asteroid[i].y - view_y;
		int index = 63 & (int)(64 + asteroid[i].angle * 64 / PI / 2);
		//if (index >= 64) index = 63;//exit(1);
		if (index < 0) {
//			__asm int 3
			index = 0;
		}
		if (index >= 64) {
//			__asm int 3
			index = 63;
		}
#		ifdef COORDINATES_WRAP
			if (x < -mask[index]->w) x += physics_x;
			if (x > physics_x-mask[index]->w) x -= physics_x;
			if (y < -mask[index]->h) y += physics_y;
			if (y > physics_y-mask[index]->h) y -= physics_y;
#		endif
		switch (test2_drawing_mode) {
		case 0:
			draw_sprite(vscreen, sprite[index], (int)x, (int)y);
			break;
		case 1:
			draw_allegro_pmask(mask[index], vscreen, (int)x, (int)y, makecol(0,192,64));
			break;
		}
	}
	if (display_hud) {
		int background_color = makecol(0,0,0);
		//int background_color = bitmap_mask_color(hud);
		int border_color = makecol(64,64,64);
		int asteroid_color = makecol(128,110,36);
		int rect_color = makecol(0,255,255);
		clear_to_color(hud, background_color);
		if (HUD_ASTSIZE == 0) {
			for (i = 0; i < num_asteroids; i += 1) {
				putpixel(hud, asteroid[i].x * HUD_SCALE, asteroid[i].y * HUD_SCALE, asteroid_color);
			}
		}
		if (HUD_ASTSIZE == 1) {
			for (i = 0; i < num_asteroids; i += 1) {
				int x = (asteroid[i].x * HUD_SCALE);
				int y = (asteroid[i].y * HUD_SCALE);
				rect(hud, x, y, x+1, y+1, asteroid_color);
			}
		}
		else {
			for (i = 0; i < num_asteroids; i += 1) {
				circlefill(hud, asteroid[i].x * HUD_SCALE, asteroid[i].y * HUD_SCALE, HUD_ASTSIZE, asteroid_color);
			}
		}
		rect(hud, view_x * HUD_SCALE, view_y * HUD_SCALE, (view_x+vscreen->w) * HUD_SCALE-1, (view_y+vscreen->h) * HUD_SCALE-1, rect_color);
		rect(vscreen, vscreen->w - hud->w - 2, vscreen->h - hud->h - 2, vscreen->w-1, vscreen->h-1, border_color);
		blit(hud, vscreen, 0, 0, vscreen->w - hud->w -1, vscreen->h - hud->h -1, hud->w, hud->h);
		//masked_blit(hud, vscreen, 0, 0, vscreen->w - hud->w -1, vscreen->h - hud->h -1, hud->w, hud->h);
	}
	release_bitmap(vscreen);
	return;
}
void do_flip() {
//	vsync();
	acquire_bitmap(screen);
	blit(vscreen, screen, 0, 0, 0, 0, vscreen->w, vscreen->h);
	release_bitmap(screen);
	acquire_bitmap(vscreen);
	clear_to_color(vscreen, 0);
	release_bitmap(vscreen);
	return;
}

int main (int argc, char **argv) {
	int i, seed;
	double average_graphics_time = 0.010, average_physics_time = 0.001;
	double last_physics, last_graphics, otime;
	DATAFILE *data;
	RLE_SPRITE *tmprle;
	BITMAP *tmpbmp;
	srand(seed = time(NULL));
	allegro_init();
	install_keyboard();
	install_timer();
	init_time();
	set_color_depth(res_z);
	if (set_gfx_mode(res_type, res_x, res_y, 0, 0) < 0) {
		allegro_message(allegro_error);
		exit(0);
	}
	vscreen = create_video_bitmap(res_x, res_y);
	vscreen = create_video_bitmap(res_x, res_y);
	if (!vscreen) vscreen = create_bitmap(res_x, res_y);
	if (!vscreen) {
		textprintf(screen, font, 0, 100, palette_color[15], "An error occured creating a bitmap");
		while (!keypressed()) ;
		exit(0);
	}
	hud = create_bitmap ( physics_x * HUD_SCALE, physics_y * HUD_SCALE );
	if (!hud) {
		textprintf(screen, font, 0, 100, palette_color[15], "An error occured creating a memory bitmap");
		while (!keypressed()) ;
		exit(0);
	}

	data = load_datafile(file_name1);
	if (!data) {
		textprintf(screen, font, 0, 100, palette_color[15], "An error occured loading the file %s", file_name1);
		while (!keypressed()) ;
		exit(0);
	}
	tmprle = (RLE_SPRITE*) data[0].dat;
	tmpbmp = create_bitmap_ex(tmprle->color_depth, tmprle->w, tmprle->h);
	if (!tmpbmp) {
		textprintf(screen, font, 0, 100, palette_color[15], "An error occured creating a bitmap");
		while (!keypressed()) ;
		exit(0);
	}
	clear_to_color(tmpbmp, bitmap_mask_color(tmpbmp));
	draw_rle_sprite(tmpbmp, tmprle, 0, 0);
	unload_datafile(data);
	tmprle = NULL;

	if (is_video_bitmap(vscreen)) {
		BITMAP *vtmp = NULL;
		//vtmp = create_video_bitmap(tmpbmp->w, tmpbmp->h);
		if (vtmp) {
			blit(tmpbmp, vtmp, 0, 0, 0, 0, tmpbmp->w, tmpbmp->h);
			destroy_bitmap(tmpbmp);
			tmpbmp = vtmp;
		}
	}

	install_pmask();
	for (i = 0; i < 64; i += 1) {
		sprite[i] = create_sub_bitmap(tmpbmp, (i%8) * tmpbmp->w / 8, (i/8) * tmpbmp->h / 8, tmpbmp->w / 8, tmpbmp->h / 8);
		mask[i] = create_allegro_pmask(sprite[i]);
		if (!tmpbmp) {
			textprintf(screen, font, 0, 100, palette_color[15], "An error occured");
			while (!keypressed()) ;
			exit(0);
		}
	}

	sprite_w = tmpbmp->w / 8;
	sprite_h = tmpbmp->h / 8;

	while (keypressed()) readkey();

	collist = create_collist(get_asteroid_callback);
	for (i = 0; i < starting_asteroids; i += 1) add_asteroid();
	_update_collist(collist, SORT_QSORT);
	last_physics = get_time_s();
	last_graphics = get_time_s();
	otime = get_time_s();
	while (!key[KEY_ESC]) {
		float view_speed = 600;
		double ctime = get_time_s();
		double dt = ctime - otime;
		otime = ctime;
		if (dt > 40) dt = 40;
		if (key[KEY_RIGHT] || key[KEY_6_PAD]) {
			view_x += dt * view_speed;
			if (view_x > physics_x - res_x) {
				view_x = physics_x - res_x;
				if (view_x < 0) view_x = 0;
			}
		}
		if (key[KEY_LEFT] || key[KEY_4_PAD]) {
			view_x -= dt * view_speed;
			if (view_x < 0) {
				view_x = 0;
			}
		}
		if (key[KEY_UP] || key[KEY_8_PAD]) {
			view_y -= dt * view_speed;
			if (view_y < 0) view_y = 0;
		}
		if (key[KEY_DOWN] || key[KEY_2_PAD]) {
			view_y += dt * view_speed;
			if (view_y > physics_y - res_y) {
				view_y = physics_y - res_y;
				if (view_y < 0) view_y = 0;
			}
		}
		if (ctime >= last_graphics + average_graphics_time * 2 + 0.010) {
			int ty = -15;
			double graphics_time = get_time_s();
			last_graphics = ctime;
			acquire_bitmap(vscreen);
			do_graphics();
			text_mode(-1);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"This is a demonstration program for Pixel MASK collision detection library");
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"With %d asteroids bouncing around in real-time", num_asteroids);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"In a %d by %d pixel region", (int)physics_x, (int)physics_y);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"You're only seeing the upper left %d by %d pixels", (int) res_x, (int) res_y);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"Press Page Up / Page Down to increase / decrease the # of asteroids.");
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"Use the arrow keys to move your view-point");
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"Press SPACE to fast-foward.  Press ESCAPE to quit.");
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"Press ENTER to change drawing mode.");
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"physics: %.1f milliseconds per tick", average_physics_time * 1000);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"graphics: %.1f milliseconds per tick", average_graphics_time * 1000);
//			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
//				"x: %.1f    y: %.1f", (double)view_x, (double)view_y);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"collision mode %d (TAB to change)", collision_mode);
//			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
//				"t1: %.2f t2: %.2f t3: %.2f t4: %.3f", atime1, atime2, atime3, atime4);
//			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
//				"%d %d", count_collisions1, count_collisions1 - count_collisions2);
//			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
//				"seed = %d", seed);//*/
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"ALLEGRO (.h) version   = Allegro %s, %s", 
				ALLEGRO_VERSION_STR, ALLEGRO_PLATFORM_STR);
			textprintf(vscreen, font, 0, ty+=15, palette_color[15], 
				"ALLEGRO (.dll) version = %s", allegro_id);//*/
			release_bitmap(vscreen);
			do_flip();
			graphics_time = get_time_s() - graphics_time;
			average_graphics_time = (graphics_time + average_graphics_time * 15.0) / 16.0;
		}
		if ((ctime >= last_physics + 0.015) || key[KEY_SPACE]) {
			double physics_time = get_time_s();
			last_physics = ctime;
			if (key[KEY_SPACE]) do_physics(3.0);
			else do_physics(1.0);
			physics_time = get_time_s() - physics_time;
			average_physics_time = (physics_time + average_physics_time * 15.0) / 16.0;
		}
		else rest(5);
		while (keypressed()) switch (readkey() >> 8) {
			case KEY_ESC:
				return 0;
				break;
			case KEY_TAB:
				if (!(key[KEY_LSHIFT] || key[KEY_RSHIFT])) {
					collision_mode = (collision_mode + 1) % num_collision_modes;
				}
				else {
					collision_mode = (collision_mode - 1 + num_collision_modes) % num_collision_modes;
				}
				break;
			case KEY_SPACE:
				break;
			case KEY_ENTER:
				test2_drawing_mode += 1;
				test2_drawing_mode %= 2;
				break;
			case KEY_9_PAD:
			case KEY_PGUP:
				for (i = 0; i < 250; i += 1) add_asteroid();
				_update_collist(collist, SORT_QSORT);
				break;
			case KEY_3_PAD:
			case KEY_PGDN:
				for (i = 0; i < 250; i += 1) remove_asteroid();
				_update_collist(collist, SORT_QSORT);
				break;
		}
	}

	for (i = 0; i < 64; i += 1) {
		destroy_bitmap(sprite[i]);
		destroy_pmask(mask[i]);
	}

	return 0;
}
END_OF_MAIN()
//*/