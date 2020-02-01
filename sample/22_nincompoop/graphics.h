// Allegro basic routines
void init_allegro() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(16);
#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 400, 300, 0, 0))
#else
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0))
#endif
	{
		allegro_message("Couldn\'nt set 320x200 resolution.'");
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
	
//	install_mouse();
	/* add other initializations here */
}

void deinit_allegro() {
	clear_keybuf();
	/* add other deinitializations here */
}

short int sprite_collide(int x1,int y1,int x2,int y2, int x3,int y3,int x4,int y4){
	//, sprite_ptr object2) {

    if (y2 < y3) return(0);
    if (y1 > y4) return(0);

    if (x2 < x3) return(0);
    if (x1 > x4) return(0);

    return(1);
};

