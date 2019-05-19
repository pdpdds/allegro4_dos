#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "tools.h"
#include "bullet.h"

class barrier{
  public:
    barrier( world *newWorld, int newX, int newY, BITMAP* newImage, int newHealth);
    ~barrier();

    void update( std::vector<bullet>* newBullets);
    void draw( BITMAP* tempImage);

    int getX();
    int getY();

    int getWidth();
    int getHeight();

    void setHealth( int newHealth){ health = newHealth; }

    void setIndestructable( bool newIndestructable){ indestructable = newIndestructable; }

    bool getDead();

    bool visible;
  private:
    int x;
    int y;

    int width;
    int height;

    int health;

    bool indestructable;
    bool exploded;

    world *worldPointer;

    void explode( int newX, int newY, int newVelocity, int newAmount, int newLife);

    BITMAP* image;
    SAMPLE *sample_explode;
};

#endif // BARRIER_H
