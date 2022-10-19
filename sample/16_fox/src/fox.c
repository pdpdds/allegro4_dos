#include "FOX.H"
#include "MOVE.H"
#include "OPENING.H"
#include "TOOL.H"
#include "MENU.H"
#include "DAT.H"
#include "game.h"

#ifdef _WIN32
#include <synchapi.h>
extern void usleep(unsigned int usec);
extern int opening(void);
#endif

extern void LaunchGameSystem();

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define DAT_TEXT  		DAT_ID('T','X','T',' ')

OPTION option;

#ifndef _WIN32
// ---------------------------------------
void print_dpmi_information(void)
{
	// https://sourceware.org/ml/gdb-patches/2001-07/msg00185.html
	int dpmi_flags;
	char dpmi_vendor_info[129];
	int dpmi_vendor_available = __dpmi_get_capabilities (&dpmi_flags, dpmi_vendor_info);
	if (dpmi_vendor_available == 0)
	{
		/* The DPMI spec says the vendor string should be ASCIIZ, but
		   I don't trust the vendors to follow that...  */
		if (!memchr (&dpmi_vendor_info[2], 0, 126))
			dpmi_vendor_info[128] = '\0';
		printf ("DPMI Host...... %s v%d.%d (capabilities: %#x)\n",
				&dpmi_vendor_info[2],
				(unsigned)dpmi_vendor_info[0],
				(unsigned)dpmi_vendor_info[1],
				((unsigned)dpmi_flags & 0x7f));
		usleep(10000);
	}

	__dpmi_version_ret dpmi_version_data;
	__dpmi_get_version (&dpmi_version_data);
	printf("DPMI Version... %d.%02d\n", 
			dpmi_version_data.major, dpmi_version_data.minor);
	usleep(10000);
	printf ("DPMI Info...... %s-bit DPMI, with%s Virtual Memory support\n",
		(dpmi_version_data.flags & 1) ? "32" : "16",
		(dpmi_version_data.flags & 4) ? "" : "out");
	usleep(10000);
	printf ("                Interrupts reflected to %s mode\n",
		(dpmi_version_data.flags & 2) ? "V86" : "Real");
	usleep(10000);
	printf ("                Processor type: i%d86\n",
		dpmi_version_data.cpu);
	usleep(10000);
	printf ("                PIC base interrupt: Master: %#x  Slave: %#x\n",
		dpmi_version_data.master_pic, dpmi_version_data.slave_pic);
}
#endif

// -----------------------------------------------------
// 사용자 정의 DATA 포맷 (ascii 파일)
void *load_text_data(PACKFILE *f, long size)
{
	char *buf;
	int i, c;

	buf = malloc(size+1);
	memset(buf, '\0', size);

	int pos=0;
	for (i=0; i<size; i++) {
		if ((c = pack_getc(f)) == EOF)
			break;

		if ( c != '\r' ) {
			buf[pos] = c;
			pos++;
		}
	}

	buf[pos] = '\0';

	return buf;
}

void destroy_text_data(void *data)
{
	if (data)
		free(data);
}

// -----------------------------------------------------
int setup(void)
{
#ifndef _WIN32
	print_dpmi_information();
#endif

	printf("Engine Initialize....... ");
	usleep(10000);
	if ( allegro_init() < 0 ) {
		err("Could not initialize allegro.\n");
	} else {
		printf("OK.\n");
	}

	printf("Load settings file...... ");
	usleep(10000);
	set_config_file("FOX.CFG");
	sprintf(option.music, "%s", get_config_string("option", "music", "no"));
	sprintf(option.sound, "%s", get_config_string("option", "sound", "no"));
	option.fps = get_config_int("option", "fps", 50);
	if ( !strcasecmp(get_config_string("option", "snow_flakes", "yes"), "yes" ) ) {
		option.snow_flakes = TRUE;
	} else {
		option.snow_flakes = FALSE;
	}
	if ( !strcasecmp(get_config_string("option", "logo", "yes"), "yes" ) ) {
		option.logo = TRUE;
	} else {
		option.logo = FALSE;
	}
	if ( !strcasecmp(get_config_string("option", "debug", "no"), "yes" ) ) {
		option.debug = TRUE;
	} else {
		option.debug = FALSE;
	}
	printf("OK.\n");


#if 0
	printf("DUMB Initialize......... ");
	usleep(10000);
	dumb_register_packfiles();
	dumb_register_dat_it(DUMB_DAT_IT);
	dumb_resampling_quality = DUMB_RQ_LINEAR;
	dumb_it_max_to_mix = 128;
	printf("OK.\n");
#endif

	printf("Keyboard Initialize..... ");
	usleep(10000);
	if ( install_keyboard() < 0 ) {
		err("Could not initialize keyboard.\n");
	} else {
		printf("OK.\n");
	}
	clear_keybuf(); 

	printf("Timer Initialize........ ");
	usleep(10000);
	if ( install_timer() < 0 ) {
		err("Could not initialize timer.\n");
	} else {
		printf("OK.\n");
	}

	printf("Sound Initialize........ ");
	usleep(10000);

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) >= 0) {
		// digi, midi
		//set_volume(255, 255);
		printf("OK.\n");
	} else {
		err("Could not initialize sound.\n");
	}

#if 0
	if (install_sound(DIGI_AUTODETECT, MIDI_MPU, NULL) < 0) {
		err("Could not initialize sound.\n");
	} else {
		// digi, midi
		set_volume(255, 255);
		printf("OK.\n");
	}
#endif

	register_datafile_object(DAT_TEXT, load_text_data, destroy_text_data);


	printf("Ready...");
#ifdef _WIN32
	Sleep(1);
#else
	sleep(1);
#endif

#if 0
	printf("Hit any key to start...");
	while(!kbhit()) {
      sleep(1); 
	}
	//if ( getch() == 27 ) exit(0);
#endif

	fade_out(5);
	
	set_color_depth(8);
#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
#else
    if (set_gfx_mode(GFX_VGA, 320, 200, 0, 0) != 0) {
#endif
		printf("\tCould not set video mode to 320x200.\n");
		printf("\tAllegro error: %s\n", allegro_error);
		allegro_exit();
		return FALSE;
    }

	text_mode(-1);

	return TRUE;
}

void load_data_callback(DATAFILE *dat)
{
	static const char indicator[] = "-\\|/-";
	static int current = 0;

	textprintf_centre_ex(screen, font, LOGICAL_SCREEN_W / 2, LOGICAL_SCREEN_H / 2, makecol(255, 255, 255), 0, "Loading %c", indicator[current]);
	current++;
	if (!indicator[current])
		current = 0;
}


void show_logo(void)
{
#ifdef USE_DATAFILE
	DATAFILE *opening_data = NULL;
	opening_data = load_datafile_callback("OPENING.DAT", load_data_callback);
	if (opening_data) {
		PALETTE pal;
		BITMAP *b = opening_data[DAT_ALLEG_BMP].dat;
		memcpy(pal, opening_data[DAT_ALLEG_PAL_BMP].dat, sizeof(PALETTE));
		show_bitmap(b, pal);
		
		b = opening_data[DAT_DJGPP_BMP].dat;
		memcpy(pal, opening_data[DAT_DJGPP_PAL_BMP].dat, sizeof(PALETTE));
		show_bitmap(b, pal);

		unload_datafile(opening_data);
		opening_data = NULL;

	} else {
		err("Couldn't load sprite resources!");
	}
#else
	PALETTE pal;
	BITMAP *b = load_bitmap("RES/ALLEG.BMP", pal);
	show_bitmap(b, pal);
	destroy_bitmap(b);

	b = load_bitmap("RES/DJGPP.BMP", pal);
	show_bitmap(b, pal);
	destroy_bitmap(b);
#endif
}

void load_kssm_font()
{
#ifdef USE_DATAFILE
	DATAFILE *font_data = NULL;
	font_data = load_datafile_callback("FONT.DAT", load_data_callback);
	if (font_data) {
		al_load_buff_hanfont(font_data[DAT_HAN_FNT].dat);
		al_load_buff_engfont(font_data[DAT_ENG_FNT].dat);
	}
	else {
		err("Couldn't load font resources!");
	}

	unload_datafile(font_data);
	font_data = NULL;

#else
	// --------------------------------------
	if (exists("RES/HAN.FNT") == FALSE)
		err("Couldn't load font resources!");

	if (exists("RES/ENG.FNT") == FALSE)
		err("Couldn't load font resources!");

	al_load_hanfont("RES/HAN.FNT");
	al_load_engfont("RES/ENG.FNT");
#endif
}

// --------------------------------------------------
int main(int argc, char **argv)
{
	setup();
	
#ifdef _DEBUG
#else
	if(option.logo) {
		show_logo();
	}
#endif

	load_kssm_font();

#ifdef _DEBUG
#else
	opening();
#endif

	while (1) 
	{
		switch (menu())
		{
		case MENU_STATE_INTRO:
			intro();
			opening();
			break;
		case MENU_STATE_OPENING:
			opening();
			break;
		case MENU_STATE_START:
			LaunchGameSystem();			
			break;
		case MENU_STATE_EXIT:
			goto GAMEEND;
			break;
		}				
	}

GAMEEND:	
	remove_sound();

	remove_keyboard();
	remove_mouse();
	remove_timer();

#if 0
	dumb_exit();
#endif

	allegro_exit();

	printf("Created by In-Hak Min\n");
	printf("E-MAIL: h2h1995@naver.com\n");
	printf("NaverCafe - DOS MUSEUM\n");
	printf("http://cafe.naver.com/olddos\n\n");

	exit(0);
}

#ifdef _WIN32
END_OF_MAIN()
#endif