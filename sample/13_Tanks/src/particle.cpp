#include "../include/particle.h"

// Default Constructor
particle::particle(){
  x = 0;
  y = 0;

  particleColor = makecol( 255, 255, 255);
  particleRed = getr(particleColor);
  particleGreen = getg(particleColor);
  particleBlue = getb(particleColor);

  particleSize = 1;
  particleType = 0;
  particleBehaviour = 0;

  particleLife = 0;

  x_velocity = 0;
  y_velocity = 0;

  dead = false;
}

// Constructor
particle::particle(int newX, int newY, int newColor, float newXMin, float newXMax, float newYMin, float newYMax, int newSize, int newType, int newLife, int newBehaviour){
  x = newX;
  y = newY;

  particleColor = newColor;
  particleRed = getr(particleColor);
  particleGreen = getg(particleColor);
  particleBlue = getb(particleColor);

  particleSize = newSize;
  particleType = newType;
  particleBehaviour = newBehaviour;

  particleLife = newLife;

  x_velocity = randomf( newXMin, newXMax);
  y_velocity = randomf( newYMin, newXMax);

  // No unmoving
  if( x_velocity == 0)
    x_velocity = 0.1;
  if( y_velocity == 0)
    y_velocity = 0.1;

  dead = false;
}

// Deconstructor
particle::~particle(){

}

// Logic
void particle::logic(){
  // Behaviour
  if( particleBehaviour == EXPLODE){
    x += x_velocity;
    y += y_velocity;
    x_velocity -= x_velocity/10;
    y_velocity -= y_velocity/10;
  }
  else{
    x += randomf( -x_velocity, x_velocity);
    y += randomf( -y_velocity, y_velocity);
  }

  // Die
  if( random( 0, particleLife) == 0)
    dead = true;
}

// Check death
bool particle::getDead(){
  return dead;
}

// Draw
void particle::draw( BITMAP* tempBitmap){
  if(particleType == PIXEL){
    putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == SQUARE){
    rectfill( tempBitmap, x, y, x + particleSize, y + particleSize, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == CIRCLE){
    circlefill( tempBitmap, x, y, particleSize, makecol( particleRed, particleGreen, particleBlue));
  }
  else if(particleType == RANDOM){
    switch(random(0,3)){
      case 0:
        putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 1:
        circlefill( tempBitmap, x, y, particleSize, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 2:
        rectfill( tempBitmap, x, y, x + particleSize, y + particleSize, makecol( particleRed, particleGreen, particleBlue));
        break;
    }
  }
}
