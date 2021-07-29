#pragma once
#include <allegro.h>

BITMAP* grabframe(BITMAP* source,
	int width, int height,
	int startx, int starty,
	int columns, int frame);

int collided(int x, int y);