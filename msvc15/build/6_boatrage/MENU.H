//MENU.H

//dessa har ingen betydelse „n...
#define M_LABEL 0
#define M_CHOICE 1
#define M_SLIDER 2
#define M_NUMBER 3
#define M_CHECK 4


struct coolmenuitem {
 char *text;  //texten s†klart
 int type;    //meningsl”s „n s?l„nge
 int x,y;     //var valet ska vara
 int min,max; //meningsl”s
 int val;     //samma h„r
 char *strval;//och h„r
};

//starta en meny
int coolmenu(RLE_SPRITE *background, coolmenuitem *items, int count, int sel=0);

