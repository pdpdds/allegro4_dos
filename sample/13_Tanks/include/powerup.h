#ifndef POWERUP_H
#define POWERUP_H

#include <allegro.h>

class powerup{
  public:
    powerup();
    powerup( int x, int y, int type, BITMAP *image);
    virtual ~powerup();

    int getX(){ return x;}
    int getY(){ return y;}
    int getWidth(){ return image -> w;}
    int getHeight(){ return image -> h;}
    int getType(){ return type;}

    bool getDead(){ return dead;}

    void pickup(){ dead = true;}

    void draw( BITMAP *tempBitmap);
  protected:

  private:
    BITMAP *image;

    int x;
    int y;
    int type;

    bool dead;
};

#endif // POWERUP_H
