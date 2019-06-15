#include <allegro.h>

#define ATTR_NORMAL		0
#define ATTR_REVERSE	1
#define ATTR_DIM		2
#define ATTR_SHADOW		3
#define ATTR_THREED		4
#define ATTR_OUTLINE	5
#define ATTR_BOLD		6
#define ATTR_UNDER		7

void al_load_hanfont(char *file);
void al_load_engfont(char *file);
void al_load_buff_hanfont(unsigned char *buf);
void al_load_buff_engfont(unsigned char *buf);
void al_han_puts(BITMAP *bmp, int x, int y, int color, int attr, char *str);
void al_han_textout_centre_ex(BITMAP *b, char *msg, int x, int y, int color);
void al_han_textout(BITMAP *b, char *msg, int x, int y, int color);
int al_han_printf(BITMAP *bmp, int x, int y, int color, int attr, char *fmt, ...);

