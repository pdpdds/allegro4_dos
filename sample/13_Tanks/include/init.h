/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
**/
#ifndef INIT_H
#define INIT_H

#include "state.h"

class init : public state
{
  public:
    init();
    ~init(){};

    void update(){};
    void draw(){}

  protected:

  private:
};

#endif // INIT_H
