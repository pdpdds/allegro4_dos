#ifndef BULLET_H
#define BULLET_H
#include <vector>
#include "tools.h"
#include "particle.h"
#include "world.h"

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define TANK 4

class bullet
{
  public:
    bullet( world *newWorld, float newX, float newY, float newAngle, float newSpeed, int newOwnerID, int newHealth, SAMPLE* newSound);
    ~bullet();

    bool getErase();
    void update();
    void draw( BITMAP* tempImage);

    float getX();
    float getY();

    float getYVelocity();
    float getXVelocity();

    void bounce( int newDirection);
    void destroy();

    void reverseDirection( std::string newDirection);
  protected:
  private:
    int health;
    int incidenceDirection;

    float x;
    float y;
    float vector_x;
    float vector_y;

    world *worldPointer;

    bool owner;
    bool pendingErase;

    SAMPLE* shotSound;
};

#endif // BULLET_H
