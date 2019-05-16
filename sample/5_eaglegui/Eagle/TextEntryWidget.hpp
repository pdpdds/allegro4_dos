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


#ifndef allegro_console_in_H
#define allegro_console_in_H



#error "Not to be included directly, but only by copy/paste/modify."


/** Allegro Console In - Boilerplate Code */


/** readkey()

The low byte of the return value contains the ASCII code of the key, and the high byte the scancode.

The scancode remains the same whatever the state of the shift, ctrl and alt keys,
while the ASCII code is affected by shift and ctrl in the normal way 
(shift changes case, ctrl+letter gives the position of that letter in the alphabet,
 eg. ctrl+A = 1, ctrl+B = 2, etc).

Pressing alt+key returns only the scancode, with a zero ASCII code in the low byte. For example: 


//*/

[][][scancode][ascii]

[code]
//
#include <string>
using std::string;
/*
typedef string::iterator         StrIter;

#if 0
StrIter IterFromIndex(string& str , unsigned int index) {
   if ((index + 1) >= str.size()) {return str.end();}
   
   unsigned int  search_index = 0;
   StrIter       search       = str.begin();
   const StrIter stop         = str.end();
   while(search != stop) {
      if (search_index == index) {return search;}
      ++search_index;
      ++start;
   }
   return str.end();
}
#endif
*/

class FontHandler {
private :
   FONT* f;
   int fheight;
public :
   FontHandler()          : f(0) , fheight(0) {}
   FontHandler(FONT* fnt) : f(0) : fheight(0) {
      SetFont(fnt);
   }
   void SetFont(FONT* fnt) {
      f = fnt;
      fheight = (f?text_height(f):0);
   }
   FONT* GetFont() {return f;}
   
   int W(const string& str) {return text_length(f , str.c_str());}
   int W(const char* cstr)  {return text_length(f , cstr);}
   int W(const char& c) {
      char cstr[2] = {c , '\0'};
      return text_length(f , cstr);
   }
   // fixed height fonts
   int H(const string& str ) {return fheight;}
   int H(const char*   cstr) {return fheight;}
   int H(const char&   c   ) {return fheight;}
};



class ScrollBar {
private :
   Rect   area_total;// fixed
   Button scr_up_btn;// & left
   Button scr_dn_btn;// & right
   Rect   area_handle;
   
   bool valigned;// vertically aligned or horizontally
   
   BITMAP* bmp;
public :
   
};

class TextInputHandler {
private :
   typedef unsigned int Index;

   string fullstr;
   vector<string*> lines;
   
   FontHandler font_handler;
   
   Index caret_lpos;// linear position of caret in the full string
   Index caret_ypos;// line counter of caret in parsed string lines
   Index caret_xpos;// column counter of caret in parsed string lines
   
   ScrollBar vscroll;
   ScrollBar hscroll;
   
   Rect area_total;// dynamic width and height based on scroll bars and parsing
   Rect area_view;// fixed (0,0,w=W,h=H)
   Rect area_vscrollbar;// dynamic scroll bar position and dimensions
   Rect area_hscrollbar;// dynamic scroll bar position and dimensions
   Rect sub_area_text;

   BITMAP* bmp_total;
   BITMAP* bmp_vscrollbar;
   BITMAP* bmp_hscrollbar;
   BITMAP* sub_bmp_text;

   bool vsb_on_left;
   bool hsb_on_top;
   
   bool insert_mode;
   
public :
   
};

string str;
str.reserve(128);
unsigned int caret_pos = 0;
bool insert_mode = true;

// ...In logic processing
while (keypressed()) {
   const int keypress = readkey();// scancode + ascii
   const int keyscancode = keypress & 0xff;
   const int keyascii = keypress >> 8;
   bool commandkey = false;
   
   if (keyascii == 0) {// Alt + key was pressed
      commandkey = true;// to prevent adding this character to the buffer
      /* Process alt key commands */
      
   } else if ((keyascii > 0) && (keyascii < 27)) {// Ctrl + letter
      commandkey = true;
      /* Process ctrl letter commands */
      
   } else {// Either shift or no mod key
      commandkey = true;// maybe, but assume it is until we know otherwise
      switch (keyscancode) {
         case KEY_INSERT :
            insert_mode = !insert_mode;
            break;
         case KEY_TAB :
            
            break;
         case KEY_ENTER :
            
            break;
         case KEY_DELETE :
            if (caret_pos < str.size()) {
               str.erase(caret_pos , 1);
            }
            break;
         case KEY_BACKSPACE :
            if ((caret_pos > 0) && (str.size() >= 1)) {
               --caret_pos;
               str.erase(caret_pos , 1);
            }
            break;
         case KEY_HOME :
            caret_pos = 0;
            break;
         case KEY_END :
            caret_pos = str.size();
            break;
         case KEY_UP :
            
            break;
         case KEY_DOWN :
            
            break;
         case KEY_LEFT :
            if (caret_pos > 0) {--caret_pos;}
            break;
         case KEY_RIGHT :
            if (caret_pos < str.size()) {++caret_pos;}
            break;
         case (/* KEY_F1 thru KEY_F12 */ :
            
            break;
         default :
            commandkey = false;// We've already handled all the other command key combinations
            break;
      }
      if (!commandkey) {
         if (insert_mode) {
            str.insert(caret_pos , 1 , keyascii);
         } else {
            if (caret_pos < str.size()) {
               str[caret_pos] = keyascii;
            } else {
               str += keyascii;
            }
         }
         ++caret_pos;
      }
   }

[/code]

#endif // allegro_console_in_H

