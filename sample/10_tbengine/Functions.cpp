#include "Functions.h"
#include "Primitives.h"

Axis::Axis() {}

void Axis::draw(BITMAP* bmp) {
	if (key[KEY_O]) {

		line(bmp, (SCREEN_W / 2), 0, SCREEN_W / 2, SCREEN_H, makecol(222, 222, 222));
		line(bmp, 0, SCREEN_H / 2, SCREEN_W, SCREEN_H / 2, makecol(222, 222, 222));

		line(bmp, (SCREEN_W / 2), 0, SCREEN_W / 2 + 20, 20, makecol(222, 222, 222));
		line(bmp, (SCREEN_W / 2), 0, SCREEN_W / 2 - 20, 20, makecol(222, 222, 222));

		line(bmp, SCREEN_W - 20, SCREEN_H / 2 - 20, SCREEN_W, SCREEN_H / 2, makecol(222, 222, 222));
		line(bmp, SCREEN_W - 20, SCREEN_H / 2 + 20, SCREEN_W, SCREEN_H / 2, makecol(222, 222, 222));

		for (int i = 100; i < SCREEN_H; i += 100) {
			line(bmp, (SCREEN_W / 2) - 10, i, SCREEN_W / 2 + 10, i, makecol(222, 222, 222));
		}
		for (int i = 0; i < SCREEN_W; i += 100) {
			line(bmp, i, SCREEN_H / 2 - 10, i, SCREEN_H / 2 + 10, makecol(222, 222, 222));
		}
	}
}

Square_function::Square_function(int a, int b, int c, int start, int end, int color) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->start = start;
	this->end = end;
	this->color = color;
}


void Square_function::draw(BITMAP* bmp)
{
	for (int x = start; x != end; x++) {
		line(bmp, (SCREEN_W / 2) + x, (SCREEN_H / 2) - ((a*x*x) + (b*x) + c), (SCREEN_W / 2) + x + 1, (SCREEN_H / 2) - ((a*(x + 1)*(x + 1)) + (b*(x + 1)) + c), color);
	}
}
