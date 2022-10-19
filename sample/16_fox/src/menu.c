#include "MENU.H"
#include "fox.h"
#include "game.h"

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

char menu_text[10][256] = {
	"INTRO",
	"OPENING",
	"START",
	"EXIT",
	0 };


extern volatile int update_count;
void timer_proc();

extern OPTION option;

static long menu_elapsed_time = 0;

static DATAFILE *sound_data = NULL;
static DATAFILE *opening_data = NULL;

/* timer callback for measuring stage elapsed time */
void menu_elapsed_time_proc(void)
{
	menu_elapsed_time+=2;
}
END_OF_FUNCTION(menu_elapsed_time_proc)

static void load_callback(DATAFILE *dat)
{
	static const char indicator[] = "-\\|/-";
	static int current = 0;

	textprintf_centre_ex(screen, font, LOGICAL_SCREEN_W /2, LOGICAL_SCREEN_H/2,
			makecol(255, 255, 255), 0,
			"Loading %c", indicator[current]);
	current++;
	if (!indicator[current])
		current = 0;
}

int menu(void)
{
	int choice = MENU_STATE_INTRO;
	int x, y;

	BITMAP *buffer = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	clear(buffer);
	
	BITMAP *bg = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	clear(bg);

	BITMAP *intro = 0;

#ifdef USE_DATAFILE
	opening_data = load_datafile_callback("OPENING.DAT", load_callback);
	if (opening_data) {
		intro = opening_data[DAT_OPENING_BMP].dat;
		set_palette(opening_data[DAT_OPENING_PAL_BMP].dat);

	} else {
		err("Couldn't load sprite resources!");
	}
#else
	PALETTE pal;
	intro = load_bitmap("RES/OPENING.BMP", pal);
	set_palette(pal);
#endif

	BITMAP *title_fox = create_sub_bitmap(intro, 40, 144, 109, 111);
	BITMAP *title_ranger = create_sub_bitmap(intro, 9, 256, 96, 27);
	BITMAP *fox = create_sub_bitmap(intro, 1, 1, 178, 143);
	
	install_int_ex(timer_proc, BPS_TO_TIMER(option.fps));
	update_count = 0;

	int exit_loop = FALSE;
	
	init_effect_starfield(1000);
	//init_effect_starfield_horizontal(1000);
	
	SAMPLE *sound_move = 0, *sound_select = 0;
	if ( !strcasecmp(option.sound, "yes") ) {
#ifdef USE_DATAFILE
		sound_data = load_datafile_callback("SOUND.DAT", load_callback);
		if (sound_data) {
			sound_move = sound_data[DAT_MENU_WAV].dat;
			sound_select = sound_data[DAT_MENU2_WAV].dat;
		} else {
			err("Couldn't load sound resources!");
		}
#else
		sound_move = load_sample("RES/SOUND/MENU.WAV");
		sound_select = load_sample("RES/SOUND/MENU2.WAV");
#endif
	}

	// -------------------------------------------
	List *menu_list = NULL;
	list_new(&menu_list);	

	int i=0;
	while(1) {
		if (i==4) break;
		char *text = menu_text[i];
		if ( text == NULL ) break;

		char *kssm_str = malloc(strlen(text)+1);
		han_conv(KS2TG, text, kssm_str);
		list_add(menu_list, kssm_str);
		i++;
	}

	int pady = 70;
	int menu_height = list_size(menu_list)*16;
	long selected_time = 0;
	
	menu_elapsed_time = 0;
	LOCK_FUNCTION(menu_elapsed_time_proc);
	LOCK_VARIABLE(menu_elapsed_time);

	install_int_ex(menu_elapsed_time_proc, BPS_TO_TIMER(1000/2));

	// --------------------------------------------
	while (1) {
		int update_flag = FALSE;

		// yield_timeslice();
		rest(10);

		while (update_count > 0) {
			acquire_bitmap(buffer);
			
			blit(bg, buffer, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);

			if(option.debug) {
				show_available_memory(buffer, 0, 0);
			}

			update_starfield(buffer, 2);
			//update_starfield_horizontal(buffer);

			x = (LOGICAL_SCREEN_W /2) - (fox->w/2);
			y = (LOGICAL_SCREEN_H /2) - (fox->h/2);
			//draw_sprite(bg, fox, x, y);
					
			x = 70;
			y = 20;
			draw_sprite(buffer, title_fox, x, y);

			x = 150;
			y = 60;
			draw_sprite(buffer, title_ranger, x, y);

			for (i=0; i<(int)list_size(menu_list); i++) {
				void *v;
				list_get_at(menu_list, i, (void*) &v);
				char *text = (char*)v;

				int x = LOGICAL_SCREEN_W /2;
				int y = (LOGICAL_SCREEN_H /2)-(menu_height/2) + (i*16) + pady;
				int text_color;

				if ( i == choice ) {
					int w = strlen(text)*8;
					int h = 16;

					int bx = x-(w/2);
					int bx2 = bx+w;
					int by = y-(h/2);
					int by2 = by+h;

					//rect(buffer, bx-10, by, bx2+10, by2, makecol(255,255,0));

					al_han_textout_centre_ex(buffer, text, x, y, makecol(255,0,0));
					//al_han_textout_centre_ex(buffer, text, x+1, y+1, makecol(255,255,255));

				} else {
					al_han_textout_centre_ex(buffer, text, x, y, makecol(255,255,255));
				}

			}

			if ( keypressed() ) {
				// 이동
				if (key[KEY_DOWN]) {
					choice++;
					if ( !strcasecmp(option.sound, "yes") ) {
						play_sample(sound_move, 255, 128, 1000, 0);
					}
				}
				else if (key[KEY_UP]) {
					choice--;
					if ( !strcasecmp(option.sound, "yes") ) {
						play_sample(sound_move, 255, 128, 1000, 0);
					}
				}
				else if (key[KEY_ENTER]) {
					exit_loop = TRUE;
					selected_time = menu_elapsed_time;
					if ( !strcasecmp(option.sound, "yes") ) {
						play_sample(sound_select, 255, 128, 1000, 0);
					}
				}

				if (choice < 0) choice = 3;
				if (choice > 3) choice = 0;

				clear_keybuf();
			}

			update_count--;

			release_bitmap(buffer);

			update_flag = TRUE;
		}

		if(update_flag == TRUE) {
			// 사운드 출력할 시간을 줘야 하기 때문에 1초후에 나감
			if (exit_loop) {
				int diff = menu_elapsed_time - selected_time;
				if ( diff >= 1000 ) {
					break;
				}
			}

			blit(buffer, screen, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
		}

	}

	rest(200);


	clear_keybuf();

	destroy_effect_starfield();
	//destroy_effect_starfield_horizontal();

	list_remove_all_cb(menu_list, free);
	list_destroy(menu_list);

	remove_int(menu_elapsed_time_proc);
	remove_int(timer_proc);

	clear(screen);

	destroy_bitmap(title_fox);
	destroy_bitmap(title_ranger);
	destroy_bitmap(fox);

#ifdef USE_DATAFILE
	if (opening_data != NULL) {
		unload_datafile(opening_data);
		opening_data = NULL;
	}
#else
	destroy_bitmap(intro);
#endif

	destroy_bitmap(buffer);
	destroy_bitmap(bg);

	if ( !strcasecmp(option.sound, "yes") ) {
#ifdef USE_DATAFILE
		if (sound_data != NULL) {
			unload_datafile(sound_data);
			sound_data = NULL;
		}
#else
		destroy_sample(sound_move);
		destroy_sample(sound_select);
#endif
	}

	return choice;
}

