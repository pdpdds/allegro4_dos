#include "Colors.h"

Colors::Colors()
{
	this->red = makecol(255, 0, 0);
	this->green = makecol(0, 255, 0);
	this->blue = makecol(0, 0, 255);
	this->white = makecol(255, 255, 255);
	this->yellow = makecol(255, 255, 0);
	this->black = makecol(0, 0, 0);
	this->gray = makecol(192, 192, 192);
	this->pink = makecol(255, 128, 255);
	this->purple = makecol(128, 0, 255);
	this->orange = makecol(255, 128, 0);
	this->brown = makecol(64, 0, 0);
	this->lime = makecol(128, 255, 128);
	this->turquoise = makecol(0, 255, 255);
	this->azure = makecol(0, 128, 255);
}

int Colors::make_color(int r, int g, int b)
{
	return makecol(r, g, b);
}