#include "powerup.h"

powerup::powerup() : powerup( 0, 0, 0, 0){

}

powerup::powerup( int x, int y, int type, BITMAP *image){
  this -> x = x;
  this -> y = y;
  this -> type = type;
  this -> image = image;
  this -> dead = false;
}

powerup::~powerup(){

}

void powerup::draw( BITMAP *tempBitmap){
  draw_sprite( tempBitmap, image, x, y);
}
