#ifndef __OPTION__
#define __OPTION__
#endif // ! __OPTION__


typedef struct OPTION
{
	char music[20];
	char sound[20];
	int fps;
	int snow_flakes;
	int logo;
	int debug;
} OPTION;

#define MIDI_MUSIC	1
#define MOD_MUSIC	2
#define NO_MUSIC	3

