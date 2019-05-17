#include "Library.h"

class Axis
{
	bool d;
public:
	Axis();
	void draw(BITMAP * bmp);
};

class Square_function
{
	int a;
	int b;
	int c;
	int start;
	int end;
	int color;
public:
	Square_function(int a, int b, int c, int start, int end, int color);
	void draw(BITMAP* bmp);
};