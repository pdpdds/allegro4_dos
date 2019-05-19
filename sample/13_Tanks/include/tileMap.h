#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>

#include "barrier.h"
#include "coordinate.h"

class tileMap{
  public:
    tileMap();
    virtual ~tileMap();

    void update();
    void draw( BITMAP *buffer, int x, int y);

    void generate_map( unsigned char width, unsigned char height);
    void load_map( std::string fileName);

  protected:
    unsigned char width;
    unsigned char height;

    std::vector<barrier> barriers;
    std::vector<coordinate2> startLocations;

    int map_temp[255][255];
  private:

};

#endif // TILEMAP_H
