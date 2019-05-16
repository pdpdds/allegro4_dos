//PLAYER.CC
#include "boatrage.h"
#include "wrappers.h"
#include "player.h"
#include <string.h>

player::player(char *pname, int _boat) {
 strncpy(name,pname,sizeof(name));
 name[sizeof(name)-1] = '\0';
 boat=_boat;
 engine=0;
 rudder=0;
// armor =0;
 money =100000;
 lastposition=0;
}

void player::drawinfo(BITMAP *bmp) {
 textprintf(bmp,(FONT *)d(FONT_NORMAL),1,1,-1,name);
 textprintf(bmp,(FONT *)d(FONT_SMALL),60,3,makecol(255,200,30),"$%i",money);
}
