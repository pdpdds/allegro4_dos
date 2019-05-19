#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "particle.h"


class world{
  public:
    world();
    virtual ~world();

    void update();
    void draw( BITMAP *tempImage);

    void addParticle( particle *newParticle);
  protected:

  private:
    std::vector<particle*> particles;
};

#endif // WORLD_H
