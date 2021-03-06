#ifndef __GAME__
#define __GAME__

#include<allegro.h>
#include<stdio.h>
#include"colly.h"

/* tiles */
#define TILE_W   16
#define TILE_H   16

#define TL_EMPTY    0
#define TL_WALL     1
#define TL_ITEM     2
#define TL_TRAP     4
#define TL_KEY      8
#define TL_KEYHOLE 16
#define TL_BOOTS   32

/* images */
#define IMG_TOT 26
#define IMG_EMPTY   0
#define IMG_WALL    1
#define IMG_ART1    2
#define IMG_DIAMOND 3
#define IMG_KEY     4
#define IMG_KEYHOLE 5
#define IMG_BOOTS   6
#define IMG_PINS    7
#define IMG_NULL    8
#define IMG_GAMEOVER 9
#define IMG_PAUSED  10

/* pointer to initial player frame Images array */
#define PL_FBASE 11

#define PL_FNUM 15

/* level */
#define VIS_X   320
#define VIS_Y   176
#define MAX_LEVEL_W 400
#define MAX_LEVEL_H 220

/* objects */
#define OBJ_MAX     6
#define OBJ_PLAYER  0
#define OBJ_KEY     1
#define OBJ_KEYHOLE 2
#define OBJ_BOOTS   3
#define OBJ_LRBLOCK 4
#define OBJ_UDBLOCK 5

/* states */
#define STATE_MAX 5
#define STANDING 0
#define JUMPING  1
#define FALLING  2
#define DYING    3
#define DEAD     4

/* directions */
#define DIR_MAX 4
#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3

/* player */

/* inventory size */
#define INV_SIZE 5

#define PL_LIVES 2

/* walk frames */
#define PL_WFRAMES 5

/*
  frames are stored in this order:
  front,
  right1, right2, right3, right4,
  left1, left2, left3, left4,
  jump_right, jump_left,
  dead1, dead2, dead3, dead4
*/

/* jump length */
#define JLEN 16

/* speed limit while falling */
#define JLIM 64
#define DEAD_TIMER 60

/* boots */
#define BOOT_JLEN   32
#define BOOT_TIMER 500

/* score */
#define SCORE_DIAMOND  50
#define SCORE_KEY     100
#define SCORE_KEYHOLE  70
#define SCORE_BOOTS   200

typedef struct {
    int x, y;
} TPoint;

typedef struct {
    short int img;
    short int flags;
    class TObject *obj;
} TTile;

typedef struct {
    TPoint pos;
    int wdir;
} THome;

extern BITMAP *Images[IMG_TOT];        /* pointers to all images */
extern int Obj_ID[OBJ_MAX];
extern int delta_x[DIR_MAX];
extern int delta_y[DIR_MAX];

class TLevel {
private:
    void drawtile(BITMAP *b, int row, int col);

public:
    TPoint size, eye;
    THome home;
    int no;
    TTile map[MAX_LEVEL_H+2][MAX_LEVEL_W+2];  /* 1..LEVEL_H/W */
    TColly *objects, *delQ;
    class TPlayer *player;

    TLevel(int ano, int asx, int asy, int ox, int oy, int idir);
    ~TLevel();
    int  visible(class TObject *p);
    void loadmap(FILE *f);
    void move_camera(int x, int y);
    void add_object(class TObject *p);
    void remove_object(class TObject *p);
    void draw(BITMAP *b);
    void run();
};

class TObject {
private:
    short int get_close1(int dir, TPoint t, TPoint e);
    short int get_close2(int dir, TPoint t, TPoint e);
public:
    int type, id;
    TPoint pos, size;
    TLevel *level;
    class TPlayer *owner;
    int tileflags, free;

    TObject(TLevel *alevel, int atype);
    virtual ~TObject();
    int aside(TObject *p, int dir, int thresx, int thresy);
    void calc_pos(TPoint *t, TPoint *e);
    short int get_close(int dir, TPoint t, TPoint e);
    virtual void draw(BITMAP *b, int x, int y);
    virtual void run();
    virtual void handle_visit(class TObject *s);
    virtual int useit(class TObject *s);
    virtual void drop(class TObject *s);
};

class TPlayer: public TObject {
private:
    int frames[STATE_MAX][DIR_MAX][PL_WFRAMES];
    int wf;
    int cX, cD, carry;
    int carrx;

    void control_state(TPoint t, TPoint e);
public:
    int score, lives, jlen, state, wdir, jc;
    class TUDBlock *elev;
    TColly *inv, *delQ;

    TPlayer(TLevel *alevel);
    ~TPlayer();
    void fixpos(TPoint t, int flags);
    void add_object(TObject *p);
    int remove_object(TObject *p);
    void home();
    void die();
    void draw(BITMAP *b, int x, int y);
    void show_status(BITMAP *b);
    void walk(int odir, int adir, int anim, int speed, TPoint t, TPoint e);
    void run();
};

class TLRBlock: public TObject {
private:
    int img, wdir, cD;
    int tx1, tx2;
    int visitlock;

public:
    TLRBlock(TLevel *alevel, int ty, int atx1, int atx2);
    void draw(BITMAP *b, int x, int y);
    void run();
    void handle_visit(class TObject *s);
};

class TUDBlock: public TObject {
private:
    int img, cD;
    int ty1, ty2;
    int visitlock;

public:
    int wdir;
    TUDBlock(TLevel *alevel, int tx, int aty1, int aty2);
    void draw(BITMAP *b, int x, int y);
    void run();
    void handle_visit(class TObject *s);
};

class TKey: public TObject {
private:
    int img;
    TPoint tpos;
    class TKeyhole *keyhole;

public:

    TKey(TLevel *alevel, class TKeyhole *keyhole, int row, int col);
    void draw(BITMAP *b, int x, int y);
    void handle_visit(class TObject *s);
};

class TKeyhole: public TObject {
private:
    int img;
    TPoint tpos;

public:
    class TKey *pkey;

    TKeyhole(TLevel *alevel, int row, int col);
    void draw(BITMAP *b, int x, int y);
    void handle_visit(class TObject *s);
};

class TBoots: public TObject {
private:
    int img, cc;
    TPoint tpos;
public:
    int jval;

    TBoots(TLevel *alevel, int row, int col);
    void draw(BITMAP *b, int x, int y);
    void handle_visit(class TObject *s);
    int useit(class TObject *s);
    void drop(class TObject *s);
};

int intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void purge(TColly *d, TColly *s, int force);

#endif
