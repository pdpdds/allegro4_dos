#include "../include/barrier.h"

barrier::barrier( world *newWorld, int newX, int newY, BITMAP* newImage, int newHealth){
  this -> x = newX;
  this -> y = newY;

  image = newImage;

  this -> worldPointer = newWorld;

  this -> width = image -> w;
  this -> height = image -> h;

  this -> health = newHealth;

  this -> indestructable = false;
  this -> exploded = false;

  sample_explode = load_sample_ex( "sfx/explode.wav");
}

barrier::~barrier(){
  //destroy_sample( sample_explode);
}

// Update
void barrier::update( std::vector<bullet>* newBullets){
  if( health > 0 || indestructable){
    for( unsigned int i = 0; i < newBullets -> size(); i++){
      if( collisionAny( x, x + width, newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5, y, y + height, newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5)){
        if( collisionBottom( newBullets -> at(i).getY() + newBullets -> at(i).getYVelocity(), newBullets -> at(i).getY() + 5, y, y + height)){
          newBullets -> at(i).reverseDirection("y");
          newBullets -> at(i).bounce( BOTTOM);
        }
        if( collisionTop( newBullets -> at(i).getY(), newBullets -> at(i).getY() + 5 + newBullets -> at(i).getYVelocity(), y, y + height)){
          newBullets -> at(i).reverseDirection("y");
          newBullets -> at(i).bounce( TOP);
        }
        if( collisionLeft( newBullets -> at(i).getX(), newBullets -> at(i).getX() + 5 + newBullets -> at(i).getXVelocity(), x, x + width)){
          newBullets -> at(i).reverseDirection("x");
          newBullets -> at(i).bounce( LEFT);
        }
        if( collisionRight( newBullets -> at(i).getX() + newBullets -> at(i).getXVelocity(), newBullets -> at(i).getX() + 5, x, x + width)){
          newBullets -> at(i).reverseDirection("x");
          newBullets -> at(i).bounce( RIGHT);
        }
        if( !indestructable)
          health -= 1;
      }
    }
  }
  else if( !exploded){
    explode( x + width/2, y + height/2, 6, 100, 30);
    exploded = true;
  }
}

// Draw image
void barrier::draw( BITMAP* tempImage){
  if( (health > 0 || indestructable) && visible)
    draw_sprite( tempImage, image, this -> x, this -> y);
}

// Get x
int barrier::getX(){
  return x;
}

// Get y
int barrier::getY(){
  return y;
}

// Get width
int barrier::getWidth(){
  return width;
}

// Get height
int barrier::getHeight(){
  return height;
}

// Check if needs cleanup
bool barrier::getDead(){
  if( !indestructable && health <= 0){
    return true;
  }
  return false;
}

// Explode
void barrier::explode( int newX, int newY, int newVelocity, int newAmount, int newLife){
  // Explode
  play_sample( sample_explode, 255, 127, 1000, 0);

  for( int i = 0; i < newAmount; i ++){
    int new_colour = 0;

    // Make sure not transparent ( they show as white)
    do{
      // position of colour
      int random_y = random(0, height);
      int random_x = random(0, width);

      // New colour
      new_colour = getpixel( image, random_y, random_x);
    }while( getr(new_colour) == 255 && getg(new_colour) == 255 && getb(new_colour) == 255);

    // Make particle
    particle *newParticle = new particle(newX, newY, new_colour, -newVelocity, newVelocity, -newVelocity, newVelocity, 1, CIRCLE, newLife, EXPLODE);

    worldPointer -> addParticle(newParticle);
  }
}
