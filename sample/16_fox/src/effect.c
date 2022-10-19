#include "EFFECT.H"
#include "sintbl.h"
#include "tool.h"
#include "game.h"

// https://www.allegro.cc/forums/thread/598750/787856#target

#ifdef _WIN32

int random()
{
	return rand();
}

#endif

/* 스파크 효과 */
static List *effect_spark;

typedef struct {
	double x;
	double y;
	double speed;
	double angle;
	int life;
	int size;
	int color;
} SPARK;

void init_effect_spark(void)
{
	list_new(&effect_spark);
}

void destroy_effect_spark(void)
{
	list_remove_all_cb(effect_spark, free);
	list_destroy_cb(effect_spark, free);
}

void create_effect_spark_small(int x, int y, int num, int size, int color)
{
	int i;
	for(i=0; i<num; i++) {
		SPARK *e = malloc(sizeof(SPARK));
		e->x = x;
		e->y = y;
		e->speed = rand() % 6 + 2;
		e->angle = rand() % 360 + 1;
		e->angle = e->angle * 3.14159 / 180;
		e->life = 5;
		e->size = size;
		e->color = color;
		list_add(effect_spark, e);
	}
}

void create_effect_spark(int x, int y, int num)
{
	int i;
	for(i=0; i<num; i++) {
		SPARK *e = malloc(sizeof(SPARK));
		e->x = x;
		e->y = y;
		e->speed = rand() % 6 + 2;
		e->angle = rand() % 360 + 1;
		e->angle = e->angle * 3.14159 / 180;
		e->life = rand() % 10 + 10;
		e->size = rand() % 5;
		//e->color = rand() % 3;
		e->color = makecol(255, 255, 255);
		list_add(effect_spark, e);
	}
}

void process_spark(BITMAP *bmp)
{
	ListIter iter;
	void *v;
	list_iter_init(&iter, effect_spark);
	while (list_iter_next(&iter, (void*) &v) != CC_ITER_END) 
	{
		SPARK *e = (SPARK*)v;

		if ( e->life) {
#if 0
			if ( e->color == 0 ) {
				color = makecol(248, 248, 216);	// white
			} else if ( e->color == 1 ) {
				color = makecol(248, 24, 0);	// red
			} else if ( e->color == 2 ) {
				color = makecol(252, 236, 0);	// yellow
			}
#else
			//int color = makecol(255, 255, 255);
#endif
			rectfill(bmp, e->x, e->y, e->x + e->size, e->y + e->size, e->color);

			e->x += e->speed * LookUpCos(e->angle);
			e->y += e->speed * LookUpSin(e->angle);
			e->life--;

		} else {
			list_iter_remove(&iter, NULL);
			free(e);
		}
	}
}

//--------------------------------------------------------------------
//
struct snow_flake *malloc_snow_flake(struct snow_flake *parent)
{
	struct snow_flake *tmp;
	tmp = (struct snow_flake *)malloc(sizeof(struct snow_flake));
	tmp->next = parent;
	return tmp;
}

struct snow_flake *create_snow_flake(struct snow_flake *parent)
{
	struct snow_flake *tmp;
	double r,direction;
	int c;

	tmp = malloc_snow_flake (parent);   // once again start by creating the snow

	// place the snow in a random place on the screen

	tmp->x = (float)(rand()%(LOGICAL_SCREEN_W +20))-20;
	tmp->y = (float)(rand()%(LOGICAL_SCREEN_H +20))-20;

	// decide speed and direction of this snow of snow
	r = (float)(rand()%15)/15+1;  // notice the snow is much slower than the rain was
	direction=rand()%64+64;

	// set the speed & direction

	tmp->dx = r*LookUpSin(direction);
	tmp->dy = -r*LookUpCos(direction);

	// snow is very light so it tends to be blown about, so we add some randomness
	// to its movement.

	tmp->rx = 1;
	tmp->ry = 1;

	// don't forget to add life to the snow.

	tmp->life = rand()%30+60;

	// snow should be all white.. but I made it a random shade of grey to make it look
	// more... interesting.

	c = rand()%128+128;
	tmp->color=makecol(c,c,c);

	// and finally set the drawing type
	// randomly set it to either of the two flake types.

	//tmp->type = rand()%2+SNOW_PARTICLE1;
	tmp->type = SNOW_PARTICLE1;
	return tmp;
}

struct snow_flake *clean_snow_flake(struct snow_flake *t, int all) 
{
	struct snow_flake *tmp;
	struct snow_flake *top;

	top=t;
	tmp=NULL;

	while(t) {    // basically we are looping through all the particles
		// in the list.
		if(t->life<0||all) { // is the snow is dead or if we are deleting all particles
			if(tmp) {          // then delete it!
				tmp->next=t->next;
				free(t);
				t=tmp->next;
			}
			else {
				top=t->next;
				free(t);
				t=top;
			}
		}
		else {
			tmp=t;
			t=t->next;
		}
	}
	return top; // return the list back, possibly empty :)
}

void draw_snow_flake(BITMAP *bmp, struct snow_flake *t) 
{
	switch(t->type) {
		case SNOW_PARTICLE1: // just draw a single pixel for a small flake of snow
			putpixel(bmp, (int)t->x, (int)t->y, t->color); 
			break;

		case SNOW_PARTICLE2:
			putpixel(bmp, (int)t->x, (int)t->y, t->color); 
			putpixel(bmp, (int)t->x-1, (int)t->y, t->color); 
			putpixel(bmp, (int)t->x+1, (int)t->y, t->color);
			putpixel(bmp, (int)t->x, (int)t->y-1, t->color);
			putpixel(bmp, (int)t->x, (int)t->y+1, t->color);
			break;
	}
}

void draw_snow_flakes(BITMAP *bmp, struct snow_flake *t) 
{
	while(t) {    // once again we are just looping through all the particles
		draw_snow_flake(bmp,t);
		t=t->next;
	}
}

void update_snow_flake(struct snow_flake *t)
{
	// first store the old positon as the end of the trail

	t->tx = t->x;  // first things first! store the current position of the snow
	t->ty = t->y;

	// then update the position using its speed & direction

	t->x += t->dx; 
	t->y += t->dy; 

	// now add any randomness

	if(t->rx>0) t->x += (rand() % (int)(t->rx*2)) - t->rx; 
	if(t->ry>0) t->y += (rand() % (int)(t->ry*2)) - t->ry;

	// subtract one from the particles life.

	t->life--;
}

void update_snow_flakes(struct snow_flake *t) 
{
	while (t) {    // and once again we are just looping through all the particles
		update_snow_flake(t);
		t=t->next;
	}
}

//--------------------------------------------------------------------
static List *effect_starfield;
//
void init_effect_starfield(int num)
{
	list_new(&effect_starfield);

	int i;
	for(i=0; i<num; i++) {
		STAR *star = malloc(sizeof(STAR));
		star->xpos = (random ()%(LOGICAL_SCREEN_W *2))-(LOGICAL_SCREEN_W);
		star->ypos = (random ()%(LOGICAL_SCREEN_H *2))-(LOGICAL_SCREEN_H);
		star->zpos = ((random ()%(STARDEPTH-2))+2);
		//star->color = STAR_COLOR_END-(rand()%STAR_COLOR_NUM);
		int color = rand_number(0, 2);
		if (color == 0)
			star->color = makecol(63,63,63);
		else if (color == 1)
			star->color = makecol(16,16,16);
		else if (color == 2)
			star->color = makecol(32,32,32);
		list_add(effect_starfield, star);
	}
}

void destroy_effect_starfield(void)
{
	list_remove_all_cb(effect_starfield, free);
	list_destroy_cb(effect_starfield, free);
}

void update_starfield(BITMAP *b, int timegap)
{
	int j, x, y;

    ListIter iter;
	void *v;
    list_iter_init(&iter, effect_starfield);
    while (list_iter_next(&iter, (void*) &v) != CC_ITER_END) 
	{
		STAR *star = (STAR*)v;

		//putpixel(b, star->oldx2, star->oldy2, 0);
		j = timegap;
		while (j)
		{
			star->zpos -= STARSPEED;
			if (star->zpos<=0) star->zpos += STARDEPTH;
			j--;
		}

		x = LOGICAL_SCREEN_W /2 + (STARDIST*star->xpos/star->zpos);
		y = LOGICAL_SCREEN_H /2 + (STARDIST*star->ypos/star->zpos);
		//putpixel(b, x, y, 32-((int)(star->zpos/64.0))+star->color);
		putpixel(b, x, y, star->color);
		star->oldx2 = star->oldx;
		star->oldy2 = star->oldy;
		star->oldx = x;
		star->oldy = y;
	}
}

// ------------------------------------------
/* 불꽃이 터지는 효과 */
static List *effect_firework;

typedef struct {
    float x, y;
    float dx, dy;
    int color;
    int size;
    unsigned int alive;
} FIREWORK;

void init_effect_firework(void)
{
	list_new(&effect_firework);
}

void destroy_effect_firework(void)
{
	list_remove_all_cb(effect_firework, free);
	list_destroy_cb(effect_firework, free);
}

void create_effect_firework(float x, float y, int num, BITMAP *bmp)
{
	int i;
	for(i=0; i<num; i++) {
		FIREWORK *p = malloc(sizeof(FIREWORK));
		p->x = x;
		p->y = y;
		p->dx = ((float)((rand() % 101) - 50)) / 10;
		p->dy = ((float)((rand() % 101) - 50)) / 10;
		p->alive = 255;
		p->size = rand_number(1, 2);
		if ( bmp == NULL ) {
			p->color = makecol(255,255,255);
		} else {
			while (1) {
				int xx = rand_number(2, bmp->w-2);
				int yy = rand_number(2, bmp->h-2);
				
				int color = getpixel(bmp, xx, yy);

				int r = getr(color);
				int g = getg(color);
				int b = getb(color);

				if (r == 0 && g == 0 && b == 0) continue;
				if (r == 255 && g == 0 && b == 255) continue;

				p->color = color;
				break;
			}
		}

		list_add(effect_firework, p);
	}
}

void process_firework(BITMAP *bmp)
{
	ListIter iter;
	void *v;
	list_iter_init(&iter, effect_firework);
	while (list_iter_next(&iter, (void*) &v) != CC_ITER_END) 
	{
		FIREWORK *p = (FIREWORK*)v;

		if (p->alive > 0) {
			p->dy += .1;
			p->x += p->dx;
			p->y += p->dy;
			p->alive--;

			//putpixel(bmp, p->x, p->y, makecol(p->alive, p->alive, p->alive));
			//putpixel(bmp, p->x, p->y, makecol(255,255,255));
			//drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
			//set_trans_blender(255, 255, 255, 128);
			rectfill(bmp, (int)p->x, (int)p->y, (int)p->x + p->size, (int)p->y + p->size, p->color);
			//solid_mode();

		} else {
			list_iter_remove(&iter, NULL);
			free(p);
		}
	}
}

// ------------------------------------------
/* 떨어지는 효과 */
static List *effect_sand;

typedef struct {
	float x, y;
    float x1, y1;
    float vel, velx;
	int life;
	int size;
	int color;
} SAND;

void init_effect_sand(void)
{
	list_new(&effect_sand);
}

void destroy_effect_sand(void)
{
	list_destroy_cb(effect_sand, free);
}

void create_effect_sand(int x, int y, int num, int size, int color)
{
	int i;
	for(i=0; i<num; i++) {
		SAND *e = malloc(sizeof(SAND));
		e->x = x;
		e->y = y;
		e->x1 = x;
		e->y1 = y;
		e->vel = (rand()%3)+1;
		e->life = rand() % 10 + 10;
		e->size = 1;
		e->color = makecol(255, 255, 255);
		list_add(effect_sand, e);
	}
}

void process_sand(BITMAP *bmp)
{
	List *remove_sand;
	list_new(&remove_sand);

	int i;
    for(i=0; i < (int)list_size(effect_sand); i++) {
        void *v;
        list_get_at(effect_sand, i, &v);
		SAND *e = (SAND*)v;

		if ( e->life) {
			e->y += e->vel;
			e->x += e->velx;
			e->velx = (rand()%5+0.25) * ((rand()%2 == 1?-1:1));
			e->vel = (rand() % 8)+3;
			rectfill(bmp, e->x, e->y, e->x + e->size, e->y + e->size, e->color);
			e->life--;

		} else {
			list_add(remove_sand, e);
		}
	}

	list_destroy_cb(remove_sand, free);
}

// ------------------------------------------------------------
static List *effect_starfield_horizontal;

void init_effect_starfield_horizontal(int num)
{
	list_new(&effect_starfield_horizontal);

	int i;
	for (i=0;i<num;i++)
	{
		STAR_HORIZONTAL *star = malloc(sizeof(STAR_HORIZONTAL));
		star->x = rand()% LOGICAL_SCREEN_W;
		star->y = rand()% LOGICAL_SCREEN_H;
		star->z = rand()%3+1;
		int color = rand_number(0, 2);
		if (color == 0)
			star->color = makecol(63,63,63);
		else if (color == 1)
			star->color = makecol(16,16,16);
		else if (color == 2)
			star->color = makecol(32,32,32);

		list_add(effect_starfield_horizontal, star);
	}
}

void destroy_effect_starfield_horizontal(void)
{
	list_remove_all_cb(effect_starfield_horizontal, free);
	list_destroy_cb(effect_starfield_horizontal, free);
}

void update_starfield_horizontal(BITMAP *b)
{
    ListIter iter;
	void *v;
    list_iter_init(&iter, effect_starfield_horizontal);
    while (list_iter_next(&iter, (void*) &v) != CC_ITER_END) 
	{
		STAR_HORIZONTAL *star = (STAR_HORIZONTAL*)v;

		int x = star->x;
		int y = star->y;
		
		putpixel(b, x, y, star->color);
		star->x += star->z;
		if(x > LOGICAL_SCREEN_W -1)
			star->x = 0;
	}
}
