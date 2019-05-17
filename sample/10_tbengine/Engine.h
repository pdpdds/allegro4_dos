#include "Library.h"

class TBEngine
{
	int w;
	int h;
	bool full;
public:
	TBEngine(int w, int h);
	void fullscreen();
};


class Profile
{
	int id;

public:
	const char* fileName;
	FILE *file;
	Profile(int id);
	void load(const char* fileName, int &lvl);
	void save(int lvl);
};




























