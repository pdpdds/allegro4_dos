#pragma once
#include <allegro.h>

#ifdef _WIN32
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#else
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#endif

typedef struct SPRITE
{
    int dir, alive;
    int x, y;
    int width, height;
    int xspeed, yspeed;
    int xdelay, ydelay;
    int xcount, ycount;
    int curframe, maxframe, animdir;
    int framecount, framedelay;
}SPRITE;

extern SPRITE* player;

void InitPlayer(char* szName);
void RenderPlayer(BITMAP* buffer, int mapxoff, int mapyoff);
void UpdatePlayer();

