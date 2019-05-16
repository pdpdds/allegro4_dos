#include <allegro.h>
#include "data.h"
#include "strings.h"


extern DATAFILE *data;

const int menumusic=10;

extern int MAXTILES;
extern int SCR_W,SCR_H,SCR_X,SCR_Y;
extern BITMAP *buffer;
extern int actual_cycle;
extern volatile int target_cycle;
extern void speeder();
extern volatile int countdown;
extern int position;
extern int numplayers;
extern int nosound;
extern int nomouse;
