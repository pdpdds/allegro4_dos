#include <allegro.h>
#include <stdio.h>
#include "pmask.h"

//#define res_type  GFX_AUTODETECT_FULLSCREEN
#define res_type  GFX_AUTODETECT
#define res_x     640
#define res_y     480
#define res_z     32

#define file_name1 "a.bmp"
#define file_name2 "b.bmp"

#define delay_time 25

#define move_speed 3


void draw_text() {
	textprintf(screen, font, 0, 10, palette_color[15], 
		" This is a test program");
	textprintf(screen, font, 0, 22, palette_color[15], 
		"  for the Pixel MASK library.");
	textprintf(screen, font, 0, 34, palette_color[15], 
		" Use the arrow keys to move");
	textprintf(screen, font, 0, 46, palette_color[15], 
		"  the currently selected image.");
	textprintf(screen, font, 0, 58, palette_color[15], 
		" Use TAB to switch images.");
	textprintf(screen, font, 0, 70, palette_color[15], 
		" Use SPACE to toggle drawing method.");
	textprintf(screen, font, 0, 82, palette_color[15], 
		" Press ESCAPE to quit.");
}

int main (int argc, char **argv) {
	int i, j, x[2], y[2], mode[2];
	int draw_text_counter;
	PMASK pd;
	PMASK *pi;
	BITMAP *bmp[2];
	allegro_init();
	install_keyboard();
	set_color_depth(res_z);
	if (set_gfx_mode(res_type, res_x, res_y, 0, 0) < 0) {
		//char buffy[1024];
		allegro_message(allegro_error);
		exit(0);
	}

	bmp[0] = load_bitmap(file_name1, NULL);
	bmp[1] = load_bitmap(file_name2, NULL);

	if (!bmp[0] || !bmp[1]) {
		textprintf(screen, font, 0, 100, palette_color[15], "An error occured loading the image files");
		while (!keypressed()) ;
		exit(0);
	}

	install_pmask();
	init_allegro_pmask(&pd, bmp[0]);
	pi = create_allegro_pmask(bmp[1]);
	//these two PMASKs are generated in different ways from different images

	x[0] = res_x * 2 / 5 - bmp[0]->w/2;
	y[0] = res_y / 2 - bmp[0]->h/2;
	mode[0] = 0;

	x[1] = res_x * 3 / 5 - bmp[0]->w/2;
	y[1] = res_y / 2 - bmp[0]->h/2;
	mode[1] = 0;

	j = 0;

	text_mode(-1);



//	while (!keypressed()) ;
//	readkey();

	draw_text();
	draw_text_counter = 0;

	while (!key[KEY_ESC]) {
		int colliding;
		static int draw_text_counter = 0;
		if (++draw_text_counter >= 20) {
			draw_text();
			draw_text_counter = 0;
		}
		colliding = check_pmask_collision (&pd, pi, x[0], y[0], x[1], y[1]);

		for (i = 0; i < 2; i += 1) {
			rectfill(screen, x[i], y[i], x[i] + bmp[i]->w - 1, y[i] + bmp[i]->h - 1, 0);
		}
		if (key[KEY_RIGHT] || key[KEY_6_PAD]) {
			x[j] += move_speed;
			if (x[j] > res_x) x[j] = res_x;
		}
		if (key[KEY_LEFT] || key[KEY_4_PAD]) {
			x[j] -= move_speed;
			if (x[j] < -bmp[j]->w) x[j] = -bmp[j]->w;
		}
		if (key[KEY_DOWN] || key[KEY_2_PAD]) {
			y[j] += move_speed;
			if (y[j] > res_y) y[j] = res_y;
		}
		if (key[KEY_UP] || key[KEY_8_PAD]) {
			y[j] -= move_speed;
			if (y[j] < -bmp[j]->h) y[j] = -bmp[j]->h;
		}
		for (i = 0; i < 2; i += 1) {
			if (mode[i] == 0) 
				masked_blit(bmp[i], screen, 0, 0, x[i], y[i], bmp[i]->w, bmp[i]->h);
			else {
				PMASK *pntr;
				if (i == 0) pntr = &pd;
				else pntr = pi;
			draw_allegro_pmask_stretch(pntr, screen, x[i], y[i], bmp[i]->w, bmp[i]->h, makecol(0,0,144));
			}
		}
		rectfill(screen, 100, 100, 200, 132, 0);
		if (colliding) {
			textprintf(screen, font, 100, 100, palette_color[12], "Colliding!");
		}
		if (keypressed()) {
			int i = readkey();
			if ((i >> 8) == KEY_TAB) j ^= 1;
			if ((i >> 8) == KEY_SPACE) mode[j] ^= 1;
		}
		rest(delay_time);
	}

	destroy_bitmap(bmp[0]);
	destroy_bitmap(bmp[1]);

	deinit_pmask(&pd);
	destroy_pmask ( pi );

	return 0;
}
END_OF_MAIN()


