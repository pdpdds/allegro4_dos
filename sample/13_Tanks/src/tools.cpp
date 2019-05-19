#include "../include/tools.h"

//Collision
bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2){
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}
bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2){
  if( yMax1 > yMax2 && yMin1 < yMax2){
    return true;
  }
  return false;
}
bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2){
  if( yMax1 > yMin2 && yMin1 < yMin2){
    return true;
  }
  return false;
}
bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2){
  if( xMax1 > xMin2 && xMin1 < xMin2){
    return true;
  }
  return false;
}
bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2){
  if( xMax1 > xMax2 && xMin1 < xMax2){
    return true;
  }
  return false;
}

//Checks if file exists
bool fexists(const char *filename){
  std::ifstream ifile(filename);
  return ifile.good();
}

// Random number generator. Use int random(lowest,highest);
int random(int newLowest, int newHighest){
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

// Random float
float randomf(float newLowest, float newHighest){
  float lowest = newLowest, highest = newHighest;
  float range = (highest - lowest) + 1;
  float randomNumber = lowest + float( range * rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

// Convert int to string
std::string convertIntToString(int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

//Convert double to string
std::string convertDoubleToString(double number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

//Convert bool to string
std::string convertBoolToString(bool boolean){
  std::stringstream ss;
  ss << boolean;
  return ss.str();
}

//Finds angle of point 2 relative to point 1
float find_angle( float x_1, float y_1, float x_2, float y_2){
  return atan2(y_1 - y_2, x_1 - x_2);
}

//Finds distance between 2 points
float find_distance( float x_1, float y_1, float x_2, float y_2){
  return hypot(x_1 - x_2, y_1 - y_2);
}

// Fade in
void highcolor_fade_in(BITMAP* bmp_orig, int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP* str_orig = create_bitmap( SCREEN_W, SCREEN_H);
  stretch_sprite( str_orig, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);

  if ( speed<=0)
    speed=16;

  for(int a=0; a<256; a+=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, str_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  stretch_sprite( screen, str_orig, 0, 0,  SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out(int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP *bmp_orig = create_bitmap( SCREEN_W, SCREEN_H);
  blit(screen,bmp_orig,0,0,0,0,SCREEN_W,SCREEN_H);

  if ( speed<=0)
    speed=16;

  for( int a=255 - speed; a>0; a-=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  destroy_bitmap( bmp_orig);
  rectfill( screen, 0, 0,  SCREEN_W, SCREEN_H, makecol(0,0,0));
}

// ERROR REPORTING
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}


// Load bitmap with error checked_array_iterator
BITMAP *load_bitmap_ex( const char *path){
  BITMAP *temp_loader;
  if (!( temp_loader = load_bitmap(path, NULL)))
    abort_on_error(("Cannot find image " + std::string(path) + "\nPlease check your files and try again").c_str());

  return temp_loader;
}

// Load and error check sounds
SAMPLE *load_sample_ex( const char *path){
  SAMPLE *temp_loader;
  if (!( temp_loader = load_sample(path)))
    abort_on_error(("Cannot find sound " + std::string(path) + "\nPlease check your files and try again").c_str());

  return temp_loader;
}

// Load and error check fonts
FONT *load_font_ex( const char *path){
  FONT *temp_loader;
  if (!( temp_loader = load_font(path, NULL, NULL)))
    abort_on_error(("Cannot find font " + std::string(path) + "\nPlease check your files and try again").c_str());

  return extract_font_range(temp_loader, ' ', 'z');
}


// Returns distance 2D
int distanceTo2D(int x1, int y1, int x2, int y2){
  return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
                           float p2_x, float p2_y, float p3_x, float p3_y,
                           float *i_x, float *i_y){
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1){
        // Collision detected
        if (i_x != NULL)
          *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
          *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}
