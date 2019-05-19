#ifndef PARTICLE_H
#define PARTICLE_H

#define CIRCLE 0
#define SQUARE 1
#define PIXEL 2
#define RANDOM 3

#define EXPLODE 4

#include "tools.h"
#include <allegro.h>

class particle{
  public:
    particle();
    particle(int newX, int newY, int newColor, float newXMin, float newXMax, float newYMin, float newYMax, int newSize, int newType, int newLife, int newBehavior);
    ~particle();

    void logic();
    void draw(BITMAP* tempBitmap);

    bool getDead();
  private:
    int particleColor;
    int particleRed;
    int particleGreen;
    int particleBlue;

    int particleType;
    int particleSize;
    int particleLife;
    int particleBehaviour;

    bool dead;

    float x;
    float y;
    float x_velocity;
    float y_velocity;
};

#endif

