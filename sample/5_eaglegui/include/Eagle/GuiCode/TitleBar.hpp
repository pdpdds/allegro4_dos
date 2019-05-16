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


#ifndef TitleBar_H
#define TitleBar_H


#include <string>

#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/GuiCode/TextWidget.hpp"



extern const unsigned int TOPIC_MINIMIZE_BUTTON;

enum MINIMIZE_BTN_MSGS {
   MINIMIZED = 0,
   MAXIMIZED = 1
};

extern const unsigned int TOPIC_CLOSE_BUTTON;

enum CLOSE_BTN_MSGS {
   SIGNAL_CLOSE = 0
};

extern const unsigned int TOPIC_TITLE_BAR;

enum TITLE_BAR_MSGS {
   TITLE_MINIMIZED = 0,
   TITLE_MAXIMIZED = 1,
   TITLE_SIGNAL_CLOSE = 2
};



/// Minimize button is a private class for use by the titlebar class only since 
/// its drawing output depends on the HASFOCUS flag of its parent.
class MinimizeButton : public Button {
private :
   bool minimized;
   
public :
   MinimizeButton(std::string name , bool is_minimized , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   
   void Minimize();
   void Maximize();
   void SetMinimizeState(bool is_minimized);
   void ToggleMinimizeState();

   bool Minimized() {return minimized;}
   bool Maximized() {return !minimized;}
   
};



/// Close button is a private class for use by the titlebar class only since 
/// its drawing output depends on the HASFOCUS flag of its parent.
class CloseButton : public Button {

public :
   CloseButton(std::string name , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

};



class WidgetHandler;

class TitleBar : public WidgetBase {
private :
   TextWidget title;
   Rectangle textarea;
   MinimizeButton min_btn;
   CloseButton close_btn;
   
   WidgetHandler* parent_gui;
   
   bool allow_move,start_move;
   Rectangle gui_start;
   int start_x , start_y;

public :
   TitleBar(std::string name , FONT* text_font , std::string title_text , bool is_minimized , bool allow_movement);
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual bool AcceptsFocus() {return false;}
   
   void ResetPositions(bool minimized);// If minimized is false the position is requested from the parent gui.
                                       // This is only public for the widget handler class.
   void SetParentGui(WidgetHandler* gui);
   void SetTitle(std::string title);
   void SetFont(FONT* textfont);

   void Minimize();
   void Maximize();
   void SetMinimizeState(bool is_minimized);
   void ToggleMinimizeState();

   const FONT* GetFont() const {return title.Font();}
   bool Minimized() {return min_btn.Minimized();}

};




#endif // TitleBar_H

