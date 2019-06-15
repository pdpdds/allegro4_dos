#include "OPENING.H"
#include "OPTION.H"
#include "DAT.H"

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

extern volatile int update_count;
extern void timer_proc();

extern OPTION option;

static DATAFILE *opening_data = NULL;
static DATAFILE *music_data = NULL;

// ---------------------------------------------------
static void load_callback(DATAFILE *dat)
{
	static const char indicator[] = "-\\|/-";
	static int current = 0;

	textprintf_centre_ex(screen, font, SCREEN_W/2, SCREEN_H/2, 
			makecol(255, 255, 255), makecol(0, 0, 0), 
			"Loading %c", indicator[current]);
	current++;
	if (!indicator[current])
		current = 0;
}

int opening(void)
{
	clear_keybuf();

	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear(buffer);
		
	BITMAP *bg = 0;
	BITMAP *b = 0;
		
#ifdef USE_DATAFILE
	opening_data = load_datafile_callback("OPENING.DAT", load_callback);
	if (opening_data) {
		bg = opening_data[DAT_EARTH_BMP].dat;
		
		b = opening_data[DAT_OPENING_BMP].dat;
		set_palette(opening_data[DAT_OPENING_PAL_BMP].dat);

	} else {
		err("Couldn't load sprite resources!");
	}
#else
	PALETTE pal;
	bg = load_bitmap("RES/EARTH.BMP", pal);
	set_palette(pal);

	b = load_bitmap("RES/OPENING.BMP", NULL);
#endif

	BITMAP *title_fox = copy_sub_bitmap(b, 40, 144, 109, 111, 1.0);
	BITMAP *title_ranger = copy_sub_bitmap(b, 9, 256, 96, 27, 1.0);
	//BITMAP *title_www = copy_sub_bitmap(b, 7, 292, 155, 15, 1.0);
	BITMAP *fox = copy_sub_bitmap(b, 1, 1, 178, 143, 1.0);
	
	MIDI *bg_midi = NULL;

#ifdef USE_DATAFILE
	if ( !strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib") ) {
		music_data = load_datafile_callback("SC55.DAT", load_callback);
	} else {
		music_data = load_datafile_callback("MT32.DAT", load_callback);
	}
	if ( music_data == NULL ) {
		err("Couldn't load music resources!");
	}

	bg_midi = music_data[DAT_05_MID].dat;

#else
	if ( !strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib") ) {
		bg_midi = load_midi("RES/MUSIC/SC-55/05.MID");
	} else if ( !strcasecmp(option.music, "mt32") ) {
		bg_midi = load_midi("RES/MUSIC/MT-32/05.MID");
	}
#endif

	play_midi( bg_midi, 1 );

	float scale = 0.1;
	BITMAP *fox_resized = resize_bitmap(fox, scale);
	int x = (SCREEN_W/2) - (fox_resized->w/2);
	int y = (SCREEN_H/2) - (fox_resized->h/2);
	int loop = TRUE;
	
	int title_x = SCREEN_W;
	float title_accel = 0.0;
	int title_loop = FALSE;
	
	int title_x2 = title_ranger->w * -1;
	float title_accel2 = 0.0;
	int title_loop2 = FALSE;
	
	int title_x3 = SCREEN_W;
	float title_accel3 = 0.0;
	int title_loop3 = FALSE;
	
	install_int_ex(timer_proc, BPS_TO_TIMER(option.fps));
	update_count = 0;

	do {
		int update_flag = FALSE;

		rest(1);

		while (update_count > 0) {
			acquire_bitmap(buffer);

			blit(bg, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 

			//show_available_memory(buffer);

			draw_sprite(buffer, fox_resized, x, y);
			draw_sprite(buffer, title_fox, title_x, 20);
			draw_sprite(buffer, title_ranger, title_x2, 60);
			//draw_sprite(buffer, title_www, title_x3, 170);
			
			// 폭스 레인저 확대 처리
			if ( loop == TRUE ) {
				if ( scale >= 1.0 ) {
					scale = 1.0;
				}

				destroy_bitmap(fox_resized);
				fox_resized = resize_bitmap(fox, scale);

				x = (SCREEN_W/2) - (fox_resized->w/2);
				y = (SCREEN_H/2) - (fox_resized->h/2);
				
				if ( scale >= 1.0 ) {
					loop = FALSE;
					title_loop = TRUE;

				} else {
					scale+=0.01;
				}
			}

			if ( title_loop == TRUE ) {
				if ( 70 >= title_x ) {
					title_loop = FALSE;
					title_loop2 = TRUE;

					title_x = 70;
				} else {
					title_x-=(int)title_accel;
					title_accel+=0.1;
				}
			}

			if ( title_loop2 == TRUE ) {
				if ( 150 <= title_x2 ) {
					title_loop2 = FALSE;
					title_loop3 = TRUE;
					title_x2 = 150;
				} else {
					title_x2+=(int)title_accel2;
					title_accel2+=0.1;
				}
			}

#if 0
			if ( title_loop3 == TRUE ) {
				if ( (SCREEN_W-title_www->w)/2 >= title_x3 ) {
					title_x3 = (SCREEN_W-title_www->w)/2;
					title_loop3 = FALSE;
				} else {
					title_x3-=(int)title_accel3;
					title_accel3+=0.1;
				}
			}
#endif
	
			release_bitmap(buffer);

			update_count--;

			update_flag = TRUE;
		}
			
		// -------------------------------------------------------
		if(update_flag == TRUE) {
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 
			release_screen();
		}

	} while ((!keypressed()));
	

	if ( bg_midi != NULL ) {
		stop_midi();
	}

#ifdef USE_DATAFILE
	unload_datafile(music_data);
	music_data = NULL;
#else
	destroy_midi(bg_midi);
#endif

	fade_out(5);

	clear_keybuf();
	clear(screen);

	remove_int(timer_proc);
	
	destroy_bitmap(title_fox);
	destroy_bitmap(title_ranger);
	//destroy_bitmap(title_www);
	destroy_bitmap(fox);
	destroy_bitmap(fox_resized);

#ifdef USE_DATAFILE
	unload_datafile(opening_data);
	opening_data = NULL;
#else
	destroy_bitmap(b);
	destroy_bitmap(bg);
#endif

	destroy_bitmap(buffer);

	return TRUE;
}
