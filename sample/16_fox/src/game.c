#include "game.h"
#include "fox.h"

extern OPTION option;
extern DATAFILE *stage_data;
COLOR_MAP global_trans_table;

int current_stage = 1;

extern void load_data_callback(DATAFILE *dat);

extern int load_bitmaps(int stage);
extern void load_fox_weapon(int stage);
extern void load_sound(void);
extern int load_music(int stageNum);
extern int load_stage_resource(int stage);
extern void load_enemy_advent(int stage);
extern int stage_start(int stage);
extern int stage_destroy();
extern void free_enemy_advent();
extern int free_stage_resource(void);
extern void free_sound(void);
extern void free_music(void);
extern void free_fox_weapon(void);
extern int free_bitmaps(void);


int StartStage(int stageNum)
{
	// -------------------------------------------------------
	stage_bg_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
	clear(stage_bg_bitmap);

	stage_buf_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
	clear(stage_buf_bitmap);

	stage_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
	clear(stage_bitmap);
	// ---------------------------------
#ifdef USE_DATAFILE
	char file_name[1024];
	sprintf(file_name, "STAGE%d.DAT", stageNum);

	stage_data = load_datafile_callback(file_name, load_data_callback);
	if (stage_data == NULL) {
		err("Couldn't load sprite resources!");
	}
#endif

	// ----------------------------------------------------
	// transparent color table
#ifdef USE_DATAFILE
	set_palette(stage_data[DAT_STAGE_PAL_BMP].dat);

	create_trans_table(&global_trans_table,
		stage_data[DAT_STAGE_PAL_BMP].dat, 128, 128, 128, NULL);
#else
	char tmp[1024];
	sprintf(tmp, "RES/STAGE%d/STAGE.BMP", stageNum);
	PALETTE pal;
	BITMAP *b = load_bitmap(tmp, pal);
	set_palette(pal);
	destroy_bitmap(b);

	create_trans_table(&global_trans_table, pal, 128, 128, 128, NULL);
#endif

	if (get_color_depth() == 8)
		color_map = &global_trans_table;
	else
		set_trans_blender(128, 128, 128, 128);

	// ----------------------------------------------------

	load_bitmaps(stageNum);

	load_fox_weapon(stageNum);

	if (!strcasecmp(option.sound, "yes")) {
		load_sound();
	}

	if (strcasecmp(option.music, "no")) {
		load_music(stageNum);
	}

	load_stage_resource(stageNum);

	// -----------------
	fox_damage_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
	rectfill(fox_damage_bitmap, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 0));

	load_enemy_advent(stageNum);

	// -------------------------------------------------------
	//load_orbit_xml("ORBIT.XML");

#ifdef USE_DATAFILE
	unload_datafile(stage_data);
	stage_data = NULL;
#endif
	// ---------------------------------

	rest(200);

	// ---------------------------------------------------------------
	stageNum = stage_start(stageNum);
	// ---------------------------------------------------------------

	stage_destroy();

	clear(screen);

	free_enemy_advent();

	destroy_bitmap(fox_damage_bitmap);

	free_stage_resource();

	if (!strcasecmp(option.sound, "yes")) {
		free_sound();
	}
	if (strcasecmp(option.music, "no")) {
		free_music();
	}

	free_fox_weapon();

	// ---------------------------------
	free_bitmaps();

	destroy_bitmap(stage_bg_bitmap);
	destroy_bitmap(stage_buf_bitmap);
	destroy_bitmap(stage_bitmap);

	if (stage_popup_bitmap != NULL) {
		destroy_bitmap(stage_popup_bitmap);
		stage_popup_bitmap = NULL;
	}

	return stageNum;
}

void LaunchGameSystem()
{
	while (1)
	{
		int nextStatge = StartStage(current_stage);
		if (nextStatge != current_stage)
			current_stage = nextStatge;
		else
			break;
	}
	
}

