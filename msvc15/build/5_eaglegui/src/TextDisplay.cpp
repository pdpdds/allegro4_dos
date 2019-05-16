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


#include <string>

#include "Eagle/TextDisplay.hpp"

using std::string;



int textout_ex_gui(BITMAP* bmp , FONT* text_font , const char* s , int x , int y , int color , int bg , int center) {
   ASSERT(bmp);
   ASSERT(text_font);
   int voffset = 2;
   if (s) {
      string text("");
      string underline("");
      const char* scan = s;
      while(scan[0]) {
         int chars_left = strlen(scan);
         if (scan[0] == '&') {
            if (chars_left > 1) {
               if (scan[1] == '&') {
                  text.push_back('&');
                  underline.push_back(' ');
               } else {
                  text.push_back(scan[1]);
                  underline.push_back('_');
               }
            }
            ++scan;
         } else {
            text.push_back(scan[0]);
            underline.push_back(' ');
         }
         if (scan[0]) {++scan;}
      }
      if (center) {
         textout_centre_ex(bmp , text_font , text.c_str() , x , y , color , bg);
         textout_centre_ex(bmp , text_font , underline.c_str() , x , y + voffset , color , bg);
      } else {
         textout_ex(bmp , text_font , text.c_str() , x , y , color , bg);
         textout_ex(bmp , text_font , underline.c_str() , x , y + voffset , color , bg);
      }
      return text_length(text_font , text.c_str());
   }
   return 0;
}



void vtextout_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(str);
   int vspace = text_height(f);
   char buf[2] = {'\0' , '\0'};
   while(buf[0] = str[0]) {// this is not a typo!
      textout_ex(bmp , f , buf , x , y , color , bg);
      y += vspace;
      ++str;
   }
}



void vtextout_centre_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(str);
   int vspace = text_height(f);
   int height = vspace*strlen(str);
   y = y - height/2;
   char buf[2] = {'\0' , '\0'};
   while(buf[0] = str[0]) {// this is not a typo!
      textout_ex(bmp , f , buf , x , y , color , bg);
      y += vspace;
      ++str;
   }
}



void vtextout_bottom_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(str);
   int vspace = text_height(f);
   int height = vspace*strlen(str);
   y = y - height;
   char buf[2] = {'\0' , '\0'};
   while(buf[0] = str[0]) {// this is not a typo!
      textout_ex(bmp , f , buf , x , y , color , bg);
      y += vspace;
      ++str;
   }
}



void vtextprintf_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(format);
   char buf[1024];
   va_list args;
   
   va_start(args , format);
   uvszprintf(buf , 1023 , format , args);
   va_end(args);
   
   vtextout_ex(bmp , f , buf , x , y , color , bg);
}



void vtextprintf_centre_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(format);
   char buf[1024];
   va_list args;
   
   va_start(args , format);
   uvszprintf(buf , 1023 , format , args);
   va_end(args);
   
   vtextout_centre_ex(bmp , f , buf , x , y , color , bg);
}



void vtextprintf_bottom_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...) {
   ASSERT(bmp);
   ASSERT(f);
   ASSERT(format);
   char buf[1024];
   va_list args;
   
   va_start(args , format);
   uvszprintf(buf , 1023 , format , args);
   va_end(args);
   
   vtextout_bottom_ex(bmp , f , buf , x , y , color , bg);
}








