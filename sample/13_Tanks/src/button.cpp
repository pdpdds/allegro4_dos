#include "button.h"


// Defaults
button::button(){
  this -> x = 0;
  this -> y = 0;
  this -> text = "";
  this -> image = 0;
  this -> button_font = 0;
  this -> hovering = false;

  padding_x = 10;
  padding_y = 10;
}

// Constructor
button::button( int x, int y, std::string text, FONT *button_font, int width, int height, int padding_x, int padding_y){
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = 0;

  if( !setFont( button_font)){
    this -> width = width;
    this -> height = height;
  }

  this -> visible = true;
  this -> hovering = false;

  this -> padding_x = padding_x;
  this -> padding_y = padding_y;
}

// Destruct
button::~button(){
  if( image != 0)
    destroy_bitmap( image);
}

// Sets an image
void button::setImage( BITMAP *image){
  this -> image = image;
  this -> width = this -> image -> w;
  this -> height = this -> image -> h;
}

// Set new font
bool button::setFont( FONT *font){
  this -> button_font = font;
  if( button_font != 0){
    this -> width = text_length( button_font, text.c_str());
    this -> height = text_height( button_font);
    return true;
  }
  return false;
}

// Update
void button::update(){
  hovering = mouse_x > x && mouse_x < x + getWidth() &&
             mouse_y > y && mouse_y < y + getHeight();
}

// True if hovering
bool button::hover(){
  return hovering;
}

// True if clicked
bool button::clicked(){
  return hovering && mouseListener::mouse_pressed & 1;
}

// Draw
void button::draw( BITMAP *tempBitmap){
  if(visible){
    // Backdrop
    rectfill( tempBitmap, x, y, x + width + padding_x * 2, y + height + padding_y * 2, makecol( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering));
    rect( tempBitmap, x, y, x + width + padding_x * 2, y + height + padding_y * 2, makecol( 0, 0, 0));

    // Text
    if( button_font != 0)
      textprintf_ex( tempBitmap, button_font, x + padding_x, y + padding_y, makecol( 0, 0, 0), -1, text.c_str());

    // Image if avail
    if( image != 0)
      draw_sprite( tempBitmap, image, x + padding_x, y + padding_y);
  }
}
