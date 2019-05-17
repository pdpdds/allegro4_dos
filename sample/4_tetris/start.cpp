#include "start.h"
#include <allegro.h>

extern float virtual_screen_x;
extern float virtual_screen_y;

extern float screen_ratio_x;
extern float screen_ratio_y;

start::start()
{
	BITMAP * tlo = NULL;
	mx = 0;
	my = 0;
	mb = 0;
}

int start::startuj()
{
	allegro_on();
	////////////////
	install_mouse();
	show_mouse(tlo);
	unscare_mouse();
	while (!key[KEY_ESC])
	{
		blit(tlo, screen, 0, 0, 0, 0, virtual_screen_x, virtual_screen_y);
		myszka();
		if (mx > 320*screen_ratio_x && mx*screen_ratio_x < 480 * screen_ratio_x 
			&& my>175 * screen_ratio_y && my*screen_ratio_y < 260 && mb != 0) { return 1; }          // przycisk graj
		if (mx > 300 * screen_ratio_x && mx < 480 * screen_ratio_x
			&& my>320 * screen_ratio_y && my < 380 * screen_ratio_y && mb != 0) { return 0; }          // przycisk wyjdz
	}
	//////////////
	allegro_off();
}

void start::myszka()
{
	if (mx != mouse_x || my != mouse_y || mb != mouse_b)
	{
		mx = mouse_x;
		my = mouse_y;
		mb = mouse_b;
	}
};
void start::allegro_on()
{
	allegro_init();
	install_keyboard();
	
	set_color_depth(16);
#ifdef _WIN32	
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, virtual_screen_x, virtual_screen_y, 0, 0);
#else	
	set_gfx_mode(GFX_AUTODETECT, virtual_screen_x, virtual_screen_y, 0, 0);
	virtual_screen_x = screen->w;
	virtual_screen_y = screen->h;
#endif
		
	/* set the color palette */
	set_palette(desktop_palette);
	clear_to_color(screen, makecol(128, 128, 128));
	
#ifdef _WIN32
	tlo = load_bmp("start.bmp", default_palette);
#else
	tlo = load_bmp("_start.bmp", default_palette);
#endif
	if (!tlo)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("nie moge zaladowac tla!");
	}

}

void start::allegro_off()
{

	remove_mouse();
	destroy_bitmap(tlo);
	allegro_exit();
}

