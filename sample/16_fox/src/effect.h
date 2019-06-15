#ifndef EFFECT_H
#define EFFECT_H

#include <math.h>
#include "list.h"
#include <allegro.h>

//------------------------------------------------------------------------
void init_effect_spark(void);
void destroy_effect_spark(void);
void create_effect_spark(int x, int y, int num);
void create_effect_spark_small(int x, int y, int num, int size, int color);
void process_spark(BITMAP *bmp);

//------------------------------------------------------------------------
typedef struct snow_flake
{
	double x,y,             // location of the particle on the screen
		   dx,dy,           // speed and direction of movement
		   rx,ry,           // the amount of randomness in the movement
		   tx,ty;           // the location this particle was last at.
	int color,              // the particle's color
		type,               // the drawing type of the particle
		life;               // This is a counter!
	// When the counter hits zero we remove the particle.
	struct snow_flake *next;  // a link to the next particle.
} snow_flake;

#define SNOW_PARTICLE1 1
#define SNOW_PARTICLE2 2

void update_snow_flakes(struct snow_flake *t);
void draw_snow_flakes(BITMAP *bmp, struct snow_flake *t);
struct snow_flake *create_snow_flake(struct snow_flake *parent);
struct snow_flake *clean_snow_flake(struct snow_flake *t, int all);

// ---------------------
#define STARDEPTH 2048
#define STARDIST 512
#define STARINC 10
#define STARSPEED 16
//#define STAR_COLOR_END           191
#define STAR_COLOR_END           makecol(152,152,152)
#define STAR_COLOR_NUM           1

typedef struct
{
	float xpos, ypos, zpos;
	int oldx, oldy, oldx2, oldy2;
	int color;
} STAR;

void init_effect_starfield(int num);
void destroy_effect_starfield(void);
void update_starfield(BITMAP *b, int timegap);

// ---------------------

typedef struct {
	float x,y,velX,velY; //obvious what these do
	short life; //life of the fire 'particle'
	int visible; //weather or not it's visible
} FIRE;

//extern List *fire_list;

extern int MAX_FIRE;
extern int fire_counter;

// ----------------------------
void init_effect_firework(void);
void destroy_effect_firework(void);
void create_effect_firework(float x, float y, int num, BITMAP *bmp);
void process_firework(BITMAP *bmp);

// ------------------------------------------

typedef struct {
	float x, y;
	int z;
	unsigned int color;
} STAR_HORIZONTAL;

void init_effect_starfield_horizontal(int num);
void destroy_effect_starfield_horizontal(void);
void update_starfield_horizontal(BITMAP *b);

#endif  // for EFFECT_H
