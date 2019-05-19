#ifndef AMBULANCIA_H
#define AMBULANCIA_H

#include "sprite.h"

class Ambulancia : public Sprite
{
	public:
		Ambulancia(int Speed);
		~Ambulancia();
		int Avanza();
};

#endif
