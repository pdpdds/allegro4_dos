#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}
void Rectangle::draw(BITMAP* buffor)
{
	rect(buffor, x, y, x + w, y + w, makecol(0, 0, 0));
	rectfill(buffor, 0, 0, SCREEN_W, y, makecol(0, 0, 0));
	rectfill(buffor, 0, y + h, SCREEN_W, SCREEN_H, makecol(0, 0, 0));
	rectfill(buffor, 0, 0, x, SCREEN_H, makecol(0, 0, 0));
	rectfill(buffor, x + w, 0, SCREEN_W, SCREEN_H, makecol(0, 0, 0));
}

void Rectangle::move()
{
	if (key[KEY_LEFT])
		x--;
	if (key[KEY_RIGHT])
		x++;
	if (key[KEY_UP])
		y--;
	if (key[KEY_DOWN])
		y++;

}
