#include "world.h"

world::world(){

}

world::~world(){

}

// Adds particle to global particle handler
void world::addParticle( particle *newParticle){
  particles.push_back( newParticle);
}

// Updates world
void world::update(){
  // Update particles
  for( unsigned int i = 0; i < particles.size(); i++){
    particles.at(i) -> logic();

    //Check death of particles
    if( particles.at(i) -> getDead())
      particles.erase( particles.begin() + i);
  }
}

// Draw world
void world::draw( BITMAP *tempImage){
  // Draw particles
  for( unsigned int i = 0; i < particles.size(); i++){
    particles.at(i) -> draw(tempImage);
  }
}
