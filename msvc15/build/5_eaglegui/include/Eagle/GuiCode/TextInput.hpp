/*
 *
 *     _______       ___       ____      __       _______ 
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\ 
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_ 
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\ 
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_ 
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Allegro Gui Library and Extensions
 *
 *    Copyright 2009-2011 by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef TextInput_H
#define TextInput_H


#include "Eagle/GuiCode/WidgetBase.hpp"

#include <string>

//#include <ctype.h>
#include <cctype>


extern const unsigned int TOPIC_TEXT_INPUT;

enum TEXT_INPUT_MSGS {
   TEXT_VALUE_CHANGED = 0,// sent when the text changes
   TEXT_VALUE_ENTERED = 1 // sent when the user presses enter
};

enum TEXT_INPUT_TYPE {
   TEXT_INPUT           = 0,
   INTEGER_INPUT        = 1,
   FLOATING_POINT_INPUT = 2
};
//#define NUM_INPUT_TYPES 3

const char* ToString(TEXT_INPUT_TYPE type);



class TextInput : public WidgetBase {
private :
   TEXT_INPUT_TYPE input_type;
   std::string str;
   FONT* text_font;
   int max_char , chars_wide , char_scroll , padding;
   
   bool insert_mode , selection_made , caret_on , begin_selection;
   bool scroll_left , scroll_right;
   UINT caretp , begin_select , end_select;
   double time_count , caret_period , scroll , scroll_rate;
   
   int int_min , int_value , int_max;
   double float_min , float_value , float_max;

   bool round_floats;
   float num_decimal_digits;


   bool EditParser(int key_read);
   bool TextParser(int key_read);
   bool IntegerParser(int key_read);
   bool FloatingPointParser(int key_read);

   void ResetDimensions();

   void RefreshIntValue();
   void RefreshFloatValue();
   
   void ResetCaret();

   void RoundFloatValue();

public :
   
   /// Use fixed width fonts only.
   /// TextInput size will be 2*(text_height(textfont)/4 + 1) + text_length(textfont , " ")*num_chars_wide) wide
   /// by 2*(text_height(textfont)/4 + 1) + text_height(textfont) tall
   /// For the default 8x8 font the size is (6 + 8*num_chars_wide) by 14
   TextInput(std::string name , TEXT_INPUT_TYPE type , FONT* textfont , int num_chars_wide , int max_num_chars ,
               int xpos , int ypos , UINT wflags = DEFAULT_FLAGS);

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

//   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);
   
   void SetFont(FONT* textfont);
   
   void SetNumCharsWide(int num);
   
   void SetCaretPeriod(double time_on = 0.5);
   void SetScrollRate(double chars_per_second = 35.0);
   
   void SetTextValue(std::string text);
   void SetIntegerValue(int value);
   void SetFloatValue(double value);

   void SetIntegerRange(int min , int value , int max);
   void SetFloatRange(double min , double value , double max);

   void RoundFloats(bool use_rounding);
   void SetNumDecimalDigits(UINT num_digits);

   /// Getters
   std::string GetTextValue();
   int GetIntMin();
   int GetIntValue();
   int GetIntMax();
   double GetFloatMin();
   double GetFloatValue();
   double GetFloatMax();

};




#endif // TextInput_H






