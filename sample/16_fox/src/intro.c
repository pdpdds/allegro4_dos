// https://liballeg.org/stabledocs/en/help.html
#include "LIST.H"
#include "SXMLC.H"
#include "INTRO.H"
#include "HANCONV.H"
#include "HANOUT.H"
#include "TOOL.H"
#include "OPTION.H"
#include "DAT.H"
#include "game.h"

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

extern volatile int update_count;
void timer_proc();

extern OPTION option;

static DATAFILE *intro_data = NULL;
static DATAFILE *music_data = NULL;
static DATAFILE *sound_data = NULL;

List *scene_list = NULL;

typedef struct {
	char *bitmap;
	char *music;
	char *sample;
	List *text_list;
} SCENE;

static long intro_elapsed_time = 0;

// -------------------------------------------------------
void intro_elapsed_time_proc(void)
{
	intro_elapsed_time++;
}
END_OF_FUNCTION(intro_elapsed_time_proc)

// ---------------------------------------------------
static void load_callback(DATAFILE *dat)
{
	static const char indicator[] = "-\\|/-";
	static int current = 0;

	textprintf_centre_ex(screen, font, LOGICAL_SCREEN_W /2, LOGICAL_SCREEN_H /2,
			makecol(255, 255, 255), 0,
			"Loading %c", indicator[current]);
	current++;
	if (!indicator[current])
		current = 0;
}

#ifdef USE_DATAFILE
int get_bitmap_data(char *file_name)
{
	if (!strcasecmp(file_name, "sky.bmp")) {
		return DAT_SKY_BMP;
	} else if (!strcasecmp(file_name, "planet.bmp")) {
		return DAT_PLANET_BMP;
	} else if (!strcasecmp(file_name, "talk1.bmp")) {
		return DAT_TALK1_BMP;
	} else if (!strcasecmp(file_name, "talk2.bmp")) {
		return DAT_TALK2_BMP;
	} else if (!strcasecmp(file_name, "ship.bmp")) {
		return DAT_SHIP_BMP;
	} else if (!strcasecmp(file_name, "city.bmp")) {
		return DAT_CITY_BMP;
	} else if (!strcasecmp(file_name, "ready.bmp")) {
		return DAT_READY_BMP;
	}

	return -1;
}

int get_palette_data(char *file_name)
{
	if (!strcasecmp(file_name, "sky.bmp")) {
		return DAT_SKY_PAL_BMP;
	} else if (!strcasecmp(file_name, "planet.bmp")) {
		return DAT_PLANET_PAL_BMP;
	} else if (!strcasecmp(file_name, "talk1.bmp")) {
		return DAT_TALK1_PAL_BMP;
	} else if (!strcasecmp(file_name, "talk2.bmp")) {
		return DAT_TALK2_PAL_BMP;
	} else if (!strcasecmp(file_name, "ship.bmp")) {
		return DAT_SHIP_PAL_BMP;
	} else if (!strcasecmp(file_name, "city.bmp")) {
		return DAT_CITY_PAL_BMP;
	} else if (!strcasecmp(file_name, "ready.bmp")) {
		return DAT_READY_PAL_BMP;
	}

	return -1;
}

int get_music_data(char *file_name)
{
	if (!strcasecmp(file_name, "14")) {
		return DAT_14_MID;
	} else if (!strcasecmp(file_name, "15")) {
		return DAT_15_MID;
	} else if (!strcasecmp(file_name, "16")) {
		return DAT_16_MID;
	} else if (!strcasecmp(file_name, "17")) {
		return DAT_17_MID;
	}

	return -1;
}
#endif

// ---------------------------------------------------
void load_scene_list (char *xml) 
{
	list_new(&scene_list);

	XMLDoc doc;
	XMLDoc_init(&doc);

	//if (!XMLDoc_parse_file_DOM(filename, &doc))
	if (!XMLDoc_parse_buffer_DOM(xml, NULL, &doc)) {
		err("Couldn't load xml resources!");
	}

	XMLNode* root = XMLDoc_root(&doc);
		
	XMLNode* node;
	node = XMLNode_get_child(root, 0);
	while (1) {
		if ( node == NULL ) break;

		SCENE *scene = malloc(sizeof(SCENE));
		scene->bitmap = NULL;
		scene->music = NULL;
		scene->sample = NULL;
		scene->text_list = NULL;

		XMLNode* node2;
		node2 = XMLNode_get_child(node, 0);

		while (1) {
			if ( node2 == NULL ) break;

			if ( !strcasecmp(node2->tag, "BITMAP") ) {
				if ( node2->text != NULL ) {
					scene->bitmap = strdup(node2->text);
				}

			} else if ( !strcasecmp(node2->tag, "MUSIC") ) {
				if ( node2->text != NULL ) {
					scene->music = strdup(node2->text);
				}

			} else if ( !strcasecmp(node2->tag, "SOUND") ) {
				if ( node2->text != NULL ) {
					scene->sample = strdup(node2->text);
				}

			} else if ( !strcasecmp(node2->tag, "TEXT") ) {
				if ( node2->text != NULL ) {
					list_new(&scene->text_list);

					char *p;
					p = strtok (node2->text, "\n");
					while (p != NULL)
					{
						if ( strlen(p) > 0 ) {
							char *kssm_str;

							kssm_str = malloc(strlen(p)+1);
							han_conv(KS2TG, p, kssm_str);
							kssm_str[strlen(p)] = '\0';

							list_add(scene->text_list, kssm_str);
						}
						p = strtok (NULL, "\n");
					}
				}
			}

			node2 = XMLNode_next_sibling(node2);
		}

		if ( scene->text_list == NULL ) {
			free(scene);
		} else {
			list_add(scene_list, scene);
		}

		node = XMLNode_next_sibling(node);
	}

	XMLDoc_free(&doc);
}

void destroy_scene_list(void)
{
	void *v;
	ListIter iter;
	list_iter_init(&iter, scene_list);
	while (list_iter_next(&iter, (void*) &v) != CC_ITER_END) {
		SCENE *scene = (SCENE*)v;

		if (scene->bitmap != NULL) free(scene->bitmap);
		if (scene->music != NULL) free(scene->music);
		if (scene->sample != NULL) free(scene->sample);
		if (scene->text_list != NULL) {
			list_remove_all_cb(scene->text_list, free);
			list_destroy(scene->text_list);
		}
	}

	list_remove_all_cb(scene_list, free);
	list_destroy(scene_list);
}

int intro(void)
{
	clear_keybuf();

	BITMAP *buffer = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	clear(buffer);

	long show_text_time = get_time_ms();
	
	BITMAP *bg = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	clear(bg);

#ifdef USE_DATAFILE
	intro_data = load_datafile_callback("INTRO.DAT", load_callback);
	if (!intro_data) {
		err("Couldn't load intro resources!");
	}

	if ( !strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib") ) {
		music_data = load_datafile_callback("SC55.DAT", load_callback);
	} else {
		music_data = load_datafile_callback("MT32.DAT", load_callback);
	}

	if (music_data == NULL) {
		err("Couldn't load music resources!");
	}

	sound_data = load_datafile_callback("SOUND.DAT", load_callback);
	if (sound_data == NULL) {
		err("Couldn't load sound resources!");
	}
#endif
	
#ifdef USE_DATAFILE
	load_scene_list(intro_data[DAT_INTRO_XML].dat);
#else
	char *xml;
   	read_text_file("RES/INTRO/INTRO.XML", &xml);
	load_scene_list(xml);
	free(xml);
#endif

	List *text_list = NULL;
	list_new(&text_list);

	int max_row = 0;
	int row = 0;
	int col = 0;
	int cnt = 0;

	char *display_text = malloc(2048);
	memset(display_text, '\0', 2048);

	PALETTE pal;
	get_palette(pal);

	int exit_loop = FALSE;

	ListIter iter;
	list_iter_init(&iter, scene_list);

	MIDI *bg_midi = NULL;

	SAMPLE *bg_sample = NULL;
	BITMAP *bg_bitmap = NULL;

	install_int_ex(timer_proc, BPS_TO_TIMER(20));
	update_count = 0;

	LOCK_VARIABLE(intro_elapsed_time);
	intro_elapsed_time = 0;
	install_int_ex(intro_elapsed_time_proc, BPS_TO_TIMER(1000));

	int bg_changed = FALSE;

	do {
		int update_flag = FALSE;

		if ( keypressed() ) {
			fade_out(3);
			break;
		}

		// yield_timeslice();
		//rest(1);

		int text_changed = FALSE;

		while (update_count > 0) {
			acquire_bitmap(buffer);

			blit(bg, buffer, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);

			if (list_size(text_list) == 0) {
				void *v;
				if ( list_iter_next(&iter, (void*) &v) == CC_ITER_END ) {
					exit_loop = TRUE;
					break;
				}

				SCENE *scene = (SCENE*)v;
						
				ListIter iter2;
				void *v2;
				list_iter_init(&iter2, scene->text_list);

				while (list_iter_next(&iter2, (void*) &v2) != CC_ITER_END) {
					char *line = (char*)v2;

					char *text = strdup(line);
					list_add(text_list, text);
				}

				clear(bg);
				clear(screen);

				if ( scene->bitmap != NULL ) {
#ifndef USE_DATAFILE
					if ( bg_bitmap != NULL ) {
						destroy_bitmap(bg_bitmap);
					}
#endif

					bg_changed = TRUE;

#ifdef USE_DATAFILE
					int data = get_bitmap_data(scene->bitmap);
					bg_bitmap = intro_data[data].dat;

					data = get_palette_data(scene->bitmap);
					memcpy(pal, intro_data[data].dat, sizeof(PALETTE));
#else
					char tmp[1024];
					sprintf(tmp, "RES/INTRO/%s", scene->bitmap);
					bg_bitmap = load_bitmap(tmp, pal);
#endif
					blit(bg_bitmap, bg, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);

				} else {
					bg_changed = FALSE;
				}

				set_palette(pal);

				if ( scene->music != NULL ) {
					if ( !strcasecmp(option.music, "sc55") || 
							!strcasecmp(option.music, "mt32") || 
							!strcasecmp(option.music, "adlib")) {
						if ( bg_midi != NULL ) {
							stop_midi();
#ifndef USE_DATAFILE
							destroy_midi(bg_midi);
#endif
						}


#ifdef USE_DATAFILE
						int data = get_music_data(scene->music);
						bg_midi = music_data[data].dat;
#else
						if ( !strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib") ) {
							char tmp[1024];
							sprintf(tmp, "RES/MUSIC/SC-55/%s.MID", scene->music);
							bg_midi = load_midi(tmp);
						} else if ( !strcasecmp(option.music, "mt32") ) {
							char tmp[1024];
							sprintf(tmp, "RES/MUSIC/MT-32/%s.MID", scene->music);
							bg_midi = load_midi(tmp);
						}
#endif
						play_midi( bg_midi, 1 );
					} 
				}

				if ( scene->sample != NULL ) {
					if ( bg_sample != NULL ) {
						stop_sample(bg_sample);
						destroy_sample(bg_sample);
					}

					bg_sample = load_sample(scene->sample);
					play_sample(bg_sample, 255, 127, 1000, 0);
				}

				max_row = list_size(text_list);
				row = 0;
				col = 0;
				cnt = 0;
				memset(display_text, '\0', 2048);

				show_text_time = intro_elapsed_time;

			} else {
				text_changed = FALSE;

				int diff = intro_elapsed_time - show_text_time;
				if ( diff >= 110 ) {
					void *v3;
					list_get_at(text_list, row, &v3);
					char *text = (char*)v3;

					if ( col >= (int)strlen(text) ) {
						*(display_text+cnt) = '\n';
						cnt+=1;
						row+=1;
						col=0;

					} else {
						unsigned char data1, data2;
						data1 = *(text+col);
						if (data1 > 127) {
							data2 = *(text+col+1);
							col+=2;

							*(display_text+cnt) = data1;
							*(display_text+cnt+1) = data2;
							cnt+=2;

						} else {
							*(display_text+cnt) = data1;
							col+=1;
							cnt+=1;
						}
					}

					if ( row >= max_row ) {
						list_remove_all_cb(text_list, free);

						row=0;
						col=0;
						cnt=0;

						fade_out(3);

					}

					show_text_time = intro_elapsed_time;
					text_changed = TRUE;
				}
			}

			if ( display_text[0] != '\0' ) {
				if ( strlen(display_text) > 0 ) {
					char *tmp = malloc(2048);
					memset(tmp, '\0', 2048);
					memcpy(tmp, display_text, 2048);

					int pady = LOGICAL_SCREEN_H -(max_row*16);

					int j=0;
					char *p;
					p = strtok (tmp, "\n");
					while (p != NULL)
					{
						al_han_textout_centre_ex(buffer, p, 
							LOGICAL_SCREEN_W /2, pady+(j*16), makecol(255,255,255));
						p = strtok (NULL, "\n");
						j++;
					}

					free(tmp);
				}
			}

			//show_available_memory(buffer);

			release_bitmap(buffer);

			update_count--;

			update_flag = TRUE;
		}


		// -------------------------------------------------------
		if(update_flag == TRUE) {
			if ( bg_changed == TRUE )
				set_pallete(black_pallete);

			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
			release_screen();

			if ( bg_changed == TRUE )
				fade_in(pal, 3);

			bg_changed = FALSE;
		}

		if (exit_loop == TRUE) {
			break;
		}

	} while (1);

	remove_int(timer_proc);
	remove_int(intro_elapsed_time_proc);

	// ------------------------------------
	if ( bg_midi != NULL ) {
		stop_midi();

#ifndef USE_DATAFILE
		destroy_midi(bg_midi);
		bg_midi = NULL;
#endif
	}

	if ( bg_sample != NULL ) {
		stop_sample(bg_sample);
		destroy_sample(bg_sample);
		bg_sample = NULL;
	}

#ifndef USE_DATAFILE
	if ( bg_bitmap != NULL ) {
		destroy_bitmap(bg_bitmap);
		bg_bitmap = NULL;
	}
#endif

	// ------------------------------------
	list_remove_all_cb(text_list, free);
	list_destroy(text_list);

	free(display_text);
	
	// ------------------------------------
	destroy_scene_list();

	// ------------------------------------------------------------
#define MAX_SPRITE	5*6

	// 마지막 출격
	if ( !key[KEY_ESC] ) {
		clear(buffer);
		clear(screen);

		clear_keybuf();

#ifdef USE_DATAFILE
		bg_sample = sound_data[DAT_GO_WAV].dat;
#else
		bg_sample = load_sample("RES/SOUND/GO.WAV");
#endif
		BITMAP *launch_bitmaps[MAX_SPRITE];

#ifdef USE_DATAFILE
		PALETTE pal2;
		BITMAP *b1 = intro_data[DAT_LAUNCH_BMP].dat;
		memcpy(pal2, intro_data[DAT_LAUNCH_PAL_BMP].dat, sizeof(PALETTE));
		set_palette(pal2);
		load_tile_bitmaps(launch_bitmaps, b1, 5, 6, 1.0);
		
		BITMAP *b2 = intro_data[DAT_GO_FOX_BMP].dat;
		BITMAP *b3 = resize_bitmap(b2, 0.8);
#else
		PALETTE pal2;
		BITMAP *b1 = load_bitmap("RES/INTRO/LAUNCH.BMP", pal2);
		set_palette(pal2);
		load_tile_bitmaps(launch_bitmaps, b1, 5, 6, 1.0);
		
		BITMAP *b2 = load_bitmap("RES/INTRO/GO_FOX.BMP", NULL);
		BITMAP *b3 = resize_bitmap(b2, 0.8);
#endif

		float scale = 1.0;
		BITMAP* fox_new_bitmaps[30] = { 0, };
		int fox_count = 1;
		while(1) {
			if(scale<0.1) break;
			fox_new_bitmaps[fox_count-1] = resize_bitmap(b3, scale);
			scale-=0.05;
			fox_count++;
		}
		
		float fox_x2;
		float fox_y2;
		float fox_padx;

		exit_loop = FALSE;
		int loop_count = 0;

		int sprite=0;
		//long launch_time = get_time_ms();
				
		int fox_i = 0;

		do {
			if ( keypressed() ) break;

			// yield_timeslice();
			rest(50);

			acquire_bitmap(buffer);

			if (sprite == 0) {
				fox_padx = 0.0;
				fox_y2 = LOGICAL_SCREEN_H - 95;
			}

			BITMAP *spr = launch_bitmaps[sprite];

			int x = (LOGICAL_SCREEN_W /2) - (spr->w/2);
			int y = (LOGICAL_SCREEN_H /2) - (spr->h/2);

			draw_sprite(buffer, spr, x, y);

			if ( loop_count == 1 ) {
				if(fox_count-1 > fox_i) {
					BITMAP *resized_fox = fox_new_bitmaps[fox_i];
					fox_x2 = (LOGICAL_SCREEN_W /2) - ((float)resized_fox->w/2);
					fox_x2+=fox_padx;
					draw_sprite(buffer, resized_fox, (int)fox_x2, (int)fox_y2);

					fox_padx+=0.5;
					fox_y2-=0.1;
					fox_i++;
				}
			}

			sprite++;
			if ( sprite > MAX_SPRITE-1 ) {
				sprite = 0;

				loop_count++;

				if ( loop_count == 1 ) {
					play_sample(bg_sample, 255, 127, 1000, 0);
				}

				if ( loop_count == 2 ) {
					exit_loop = TRUE;
					//break;
				}
			}

			//launch_time = get_time_ms();

			//show_available_memory(buffer);

			release_bitmap(buffer);

			// -------------------------------------------------------
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
			release_screen();

			if (exit_loop == TRUE) break;
		} while (1);

		// ------------------	
		int i;
		for(i=0; i<fox_count; i++) 
		{
			if(fox_new_bitmaps[i])
				destroy_bitmap(fox_new_bitmaps[i]);
		}
		for(i=0; i<MAX_SPRITE; i++) {
			destroy_bitmap(launch_bitmaps[i]);
		}

		// ------------------	
		destroy_bitmap(b3);

#ifndef USE_DATAFILE
		destroy_bitmap(b2);
		destroy_bitmap(b1);
#endif

		// ------------------	
		stop_sample(bg_sample);

#ifndef USE_DATAFILE
		destroy_sample(bg_sample);
#endif
	}

	fade_out(3);
	
	stop_midi();

	clear(screen);
	clear_keybuf();
	
	destroy_bitmap(bg);
	destroy_bitmap(buffer);

#ifdef USE_DATAFILE
	unload_datafile(intro_data);
	intro_data = NULL;

	unload_datafile(music_data);
	music_data = NULL;

	unload_datafile(sound_data);
	sound_data = NULL;
#endif

	return TRUE;
}

