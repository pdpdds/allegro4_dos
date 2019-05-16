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


#ifndef Word_H
#define Word_H

#include <string>

class Word {
private :
   std::string word;
   int pixwidth;
   int x,y;
   int len;
   ALLEGRO_FONT* f;
   bool is_whitespace_word;
   bool is_newline_word;
public :
   Word(string& s , ALLEGRO_FONT* al_font , int xpos , int ypos) :
      word(s),
      pixwidth(0),
      x(xpos),
      y(ypos),
      len(strlen(s.c_str());
      f(al_font),
      is_whitespace_word(true),
      is_newline_word(true),
      is_tab_word(true)
   {
      pixwidth = al_get_text_width(f , word.c_str());
      for (int i = 0 ; i < strlen.size() ; ++i) {
         char c = word[i];
         if (c != '\n') {is_newline_word = false;}
         if ((c != ' ') {is_whitespace_word = false;}
         if (c != '\t')) {is_tab_word = false;}
      }
   }
   void Draw
};

const Word EmptyWord("" , 0);

inline Word GetNextWord(char** strpos , ALLEGRO_FONT* al_font) {
   if (!strpos) {return EmptyWord;}
   char* str = *strpos;
   
}


#endif // Word_H




