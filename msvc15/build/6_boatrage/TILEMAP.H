//TILEMAP.H

//dessa konstanter „r f”r tiletype.action.
#define A_NONE    0 //tomt
#define A_BLOCK   1 //blockera (standard)
#define A_EDGE    2 //kanter & palmer & s†nt
#define A_CP1     3 //checkpoints
#define A_CP2     4
#define A_CP3     5
#define A_GOAL    6 //m†l
#define A_SPLEFT  7 //speed ups
#define A_SPUP    8
#define A_SPRIGHT 9
#define A_SPDOWN  10

extern int permit[11][2];

//en sorts tile
class tiletype {
 public:
  RLE_SPRITE *bmp;
  int action;
  int loaded;
  int color;
};

//listan med alla sorterna
extern tiletype tiletypes[];

class tilemap {
 public:
  tilemap() {_map=0;}
  ~tilemap();
  void newmap(int w, int h);
  void loadmap(char *filename,boatcoord &bc1,boatcoord &bc2);
  void loadmap(char *filename,boat* b1,boat *b2,player &p1,player &p2);
  void savemap(char *filename,boatcoord &bc1,boatcoord &bc2);
  void closemap(int killsprites=1);
  void fixedges();
  void drawminimap(BITMAP *bmp);
  int get(int x, int y, int l)
  {
   if ((x>=0) && (y>=0) && (x<width) && (y<height))
    return _map[y*width+x+l*width*height];
   else
    return 0;
  }
  int set(int x, int y, int l, int newtile)
  {
   if ((x>=0) && (y>=0) && (x<width) && (y<height))
    return _map[y*width+x+l*width*height]=newtile;
   else
    return 0;
  }
  int _get(int i) {return _map[i];}
  int _set(int i, int n) {return _map[i]=n;}
  void draw_using_brush(int x,int y,int layer,int tile,int brush);
  int getwidth() {return width;}
  int getheight() {return height;}
  int getscrwidth() {return width<<4;}
  int getscrheight() {return height<<4;}
  fixed getfixwidth() {return width<<16;}
  fixed getfixheight() {return height<<16;}
  void clear();
  int getlaps() {return laps;}
  int setlaps(int nlaps) {return laps=nlaps;}
  int getsong() {return song;}
  int setsong(int nsong) {return song=nsong;}
  char name[16];
  char creator[16];
 protected:
  int *_map;
  int width, height;
  int laps;
  int song;
};

//kartan
extern tilemap *map;



