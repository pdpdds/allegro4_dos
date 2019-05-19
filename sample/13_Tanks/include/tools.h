#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <allegro.h>

//Collision
extern bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2);
extern bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2);

//Finds angle of point 2 relative to point 1
extern float find_angle( float x_1, float y_1, float x_2, float y_2);
extern float find_distance( float x_1, float y_1, float x_2, float y_2);

// Mouse position including resolution difference
extern int mouseX();
extern int mouseY();

//Checks if file exists
extern bool fexists(const char *filename);

//Random number generator
extern int random(int newLowest, int newHighest);
extern float randomf(float newLowest, float newHighest);

//Convert int to string
extern std::string convertIntToString(int number);

//Convert double to string
extern std::string convertDoubleToString(double number);

//Convert bool to string
extern std::string convertBoolToString(bool boolean);

// Fade in and out
extern void highcolor_fade_in(BITMAP* bmp_orig, int speed);
extern void highcolor_fade_out(int speed);

// Next state
extern void set_next_state( int newState );

// Error reporting
extern void abort_on_error(const char *message);

// Load and error check images
extern BITMAP *load_bitmap_ex( const char *path);

// Load and error check sounds
extern SAMPLE *load_sample_ex( const char *path);

// Load and error check fonts
extern FONT *load_font_ex( const char *path);


int distanceTo2D(int x1, int y1, int x2, int y2);

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
                           float p2_x, float p2_y, float p3_x, float p3_y,
                           float *i_x, float *i_y);

#endif // TOOLS_H
