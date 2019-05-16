struct boattype {
 char name[12];
 char info[100];
 int picid;
 fixed acc;
 fixed turnspeed;
 fixed skid;
 fixed armor;
};

extern boattype boattypes[];

struct upgrade {
 char name[12];
 char info1[40];
 char info2[40];
 int picid;
 int price;
 float accmul;
 float turnmul;
 float skidmul;
 float armmul;
};

extern upgrade engines[];
extern upgrade rudders[];
//extern upgrade armors[];

class boat : public sprite {
 public:
  boat(player &p, boatcontroller &c);
  virtual ~boat();
  void draw(BITMAP *bmp,fixed sx,fixed sy);
  void drawinfo(BITMAP *bmp);
  void move();
  void writetofile(PACKFILE *f) {f=f;}  //do nothing
  void readfromfile(PACKFILE *f) {f=f;} //do nothing
  void setcoords(boatcoord &bc);
  void goquiet() {stop_sample(engine);}
  int isfinished() {return finished;}
  fixed angle;
  fixed dx,dy;
 protected:
  player *pl;
  boatcontroller *controller;
  BITMAP *pic;
  SAMPLE *engine;
  boat *enemy;
  fixed acc;
  fixed turnspeed;
  fixed skid;
  int lap;
  int checkpoint;
  int finished;
};

#define LIGHT_WHITE   0
#define LIGHT_RED     1
#define LIGHT_GREEN   2
#define LIGHT_BLUE    3
#define LIGHT_YELLOW  4
#define LIGHT_MAGENTA 5
#define LIGHT_CYAN    6
#define LIGHT_RANDOM  rnd(6)

#define LIGHT_SMALL   0
#define LIGHT_BIG     1
#define LIGHT_LTHOUSE 2

class light : public sprite {
 public:
  light(fixed x, fixed y, int _color, int _type, int _p1, int _p2);
  light(PACKFILE *f);
  void draw(BITMAP *bmp, fixed sx, fixed sy);
  void move() {if (ltype==LIGHT_LTHOUSE) p1+=2;}
  void writetofile(PACKFILE *f);
  void readfromfile(PACKFILE *f);
 protected:
  int color;
  int ltype;
  int p1,p2; //olika extraegenskaper f”r ljusk„llor som
             //roterande fyrar och annat
};


class editcursor: public sprite {
 public:
  editcursor(fixed x, fixed y);
  void draw(BITMAP *bmp, fixed sx, fixed sy);
  void move();
  void writetofile(PACKFILE *f) {f=f;}
  void readfromfile(PACKFILE *f) {f=f;}
 protected:
  int curtile;
  RLE_SPRITE *pic;
};

