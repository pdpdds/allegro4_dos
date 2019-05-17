#pragma once
#include "Library.h"
#include "Characters.h"

class Sky {
	int color;

public:
	BITMAP *BMP;
	Sky(int color);
	void draw(BITMAP *buffor, int x, int w);
};

class Moon {
	int x;
	int y;
	BITMAP* BMP;
public:
	Moon();
	void draw(BITMAP *buffor);
	void move(int xx, int timer, bool anim);
};

class Stars {
	int x;
	int y;
	int color;
public:
	Stars();
	void draw(BITMAP *buffor, int x, int w);
};

class Ground {
	int x;
	int y;
	int l;
	int yp;
	bool moving;
	bool resume;
	BITMAP *BMP;
public:

	static int gravity_blocks;
	Ground(int x, int y, int l, bool moving);
	Ground(int x, int y, int l, bool moving, bool ice);
	Ground(int x, int y, int l, bool moving, bool ice, bool fire);
	void gravity(Hero &h);
	void draw(BITMAP *buffor);
	void move(Hero h);
	void wind(Hero &h);
};

class Telebim {
	int x;
	int y;
public:
	Telebim(int x, int y);
	void draw(BITMAP * buffor, Hero h, int timer);
};

class Tree {
	int x;
	int y;
	BITMAP* BMP;
public:
	Tree(int x, int y);
	Tree(int x, int y, bool ice);
	Tree(int x, int y, bool ice, bool fire);
	void draw(BITMAP * buffor);
};

class Bush {
	int x;
	int y;
	BITMAP* BMP;
public:
	Bush(int x, int y);
	Bush(int x, int y, bool ice);
	Bush(int x, int y, bool ice, bool fire);
	void draw(BITMAP * buffor);
};

class Water_clif {
	int x;
	int y;
	int delta;
	bool ice;
	bool fire;
public:
	Water_clif(int x, int y);
	Water_clif(int x, int y, bool ice);
	Water_clif(int x, int y, bool ice, bool fire);
	void draw(BITMAP* buffor);
};

class Block
{
	int x;
	int y;
	int l;
	int ymax;
	int ymin;
	bool direction;
	bool dir;
	BITMAP *BMP;
public:
	Block(int x, int y, int l, int ymin, int ymax, bool dir);
	Block(bool fire, int x, int y, int l, int ymin, int ymax, bool dir);
	void draw(BITMAP *buffor);
	void move(Hero &h);
	void gravity(Hero &h);
};

class Footbridge
{
	int x;
	int y;
	BITMAP *BMP;
public:
	Footbridge(int x, int y);
	void draw(BITMAP *buffor);
};

class Bridge
{
	int x;
	int y;
	int n;
	BITMAP *BMP;
public:
	Bridge(int x, int y, int n);
	void draw(BITMAP *buffor);
	void gravity(Hero &h);
};
