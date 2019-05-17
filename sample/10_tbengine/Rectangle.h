#include "Library.h"
#pragma once

class Rectangle
{
	int x;
	int y;
	int w;
	int h;
public:
	Rectangle(int x, int y, int w, int h);
	void draw(BITMAP *buffor);
	void move();
};
