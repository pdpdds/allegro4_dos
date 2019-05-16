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

#ifndef TextWidget_H
#define TextWidget_H

#include <allegro.h>

#include "Eagle/GuiCode/WidgetBase.hpp"

#include <string>


enum TEXT_HPOS {
   TEXT_LEFT   = 0,
   TEXT_CENTER = 1,
   TEXT_RIGHT  = 2
};

enum TEXT_VPOS {
   TEXT_TOP     = 0,
   TEXT_VCENTER = 1,
   TEXT_BOTTOM  = 2
};



class TextWidget : public WidgetBase {

private :
   std::string text;
   FONT* text_font;
   TEXT_HPOS texthpos;
   TEXT_VPOS textvpos;
   int text_x , text_y;

   void ResetTextPos();

public :

   /// These 4 TextWidget constructors all use TEXT_LEFT and TEXT_TOP as their positions and the default font
   TextWidget(std::string name , std::string text_str , int tx , int ty , UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , int text_val , int tx , int ty , UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , double text_val , int tx , int ty , UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , double text_val , unsigned int min_width , unsigned int num_decimals ,
              int tx , int ty , UINT wflags = DEFAULT_FLAGS);

   TextWidget(std::string name , std::string text_str , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
              int tx , int ty ,UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , int text_val , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
              int tx , int ty ,UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , double text_val , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
              int tx , int ty ,UINT wflags = DEFAULT_FLAGS);
   TextWidget(std::string name , double text_val , unsigned int min_width , unsigned int num_decimals ,
              FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
              int tx , int ty , UINT wflags = DEFAULT_FLAGS);



   /// Redefined virtual functions from WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);/// does nothing
   virtual void SetArea(int xpos , int ypos , int width , int height);/// only sets position
   virtual void SetArea(const Rectangle& r);/// only sets position

   inline virtual bool AcceptsFocus() {return false;}
   
   void SetFont(FONT* textfont);

   /// Getters
   const std::string    Text()     const {return text;}
   const FONT*     Font()     const {return text_font;}
   TEXT_HPOS TextHPos() const {return texthpos;}
   TEXT_VPOS TextVPos() const {return textvpos;}
   int       TextX()    const {return text_x;}
   int       TextY()    const {return text_y;}

   /// Setters
   void SetText(std::string textstr);
   void SetText(const char* textstr);
   void SetText(int value);
   void SetText(double value);
   void SetText(double value , unsigned int min_width , unsigned int num_decimals);

   void SetTextPos(int tx , int ty);
   void SetTextPosType(TEXT_HPOS hpos , TEXT_VPOS vpos);
   void SetTextPosEx(int tx , int ty , TEXT_HPOS hpos , TEXT_VPOS vpos);

};

#endif // TextWidget_H

