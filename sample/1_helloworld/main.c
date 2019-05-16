#include <allegro.h>

#ifdef _WIN32
#include <synchapi.h>
#include <conio.h>
#endif

int main(void)
{
    /* you should always do this at the start of Allegro programs */
    if (allegro_init() != 0)
        return 1;

	BITMAP *screen2; //Double buffer

    /* set up the keyboard handler */
    install_keyboard();

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else
	/* set a graphics mode sized 320x200 */
    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
        if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
            return 1;
        }
    }

	screen2 = create_bitmap(screen->w, screen->h); //Set up double buffer
	if (!screen2) {
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}

    /* set the color palette */
    set_palette(desktop_palette);

    /* clear the screen to white */
    clear_to_color(screen, makecol(255, 255, 255));    

	while (!key[KEY_ESC]) {

		clear_bitmap(screen2);
		textout_centre_ex(screen2, font, "Hello, world!", SCREEN_W / 2, SCREEN_H / 2, makecol(0, 0, 0), -1);

		blit(screen2, screen, 0, 0, 0, 0, screen->w, screen->h);

		rest(1);
	}	
	
    return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif
