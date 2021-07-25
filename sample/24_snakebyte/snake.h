#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#define MAP_HEIGHT 48
#define MAP_WIDTH 64
#else
#define MAP_HEIGHT 20
#define MAP_WIDTH 32
#endif

#define TILE_SIZE 10
//32x24 tiles, 20 px in size

typedef enum
{
	left = 0, right, up, down
}direction;

typedef enum
{
	nothing = 0, apple, snake
}object;

typedef struct snake_node
{
	int dir;
	int x;
	int y;
	struct snake_node *next;
}snake_node;

 
void game_over();
void move(snake_node* player);
snake_node* move_body(snake_node* player, int tempx, int tempy);
void generate_new_apple();
void timer1(void);
void rest1(void);
void draw();
int main();


int objMap[MAP_HEIGHT][MAP_WIDTH];//This will contain all the objects. will use it to keep track of collisions
BITMAP *buffer;//This will be our temporary bitmap for double buffering 
int score;//score for the game
int gamespeed; //this is a value between 1 and 3

//timer variables
volatile int counter;
volatile int ticks;
volatile int framerate;
volatile int resting, rested;



