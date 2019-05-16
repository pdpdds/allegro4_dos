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


#ifndef ScrollerWidget_H
#define ScrollerWidget_H


#include "Eagle/GuiCode/WidgetBase.hpp"



extern const unsigned int TOPIC_SCROLLER;

enum SCROLLER_MSGS {
   SCROLLER_VALUE_CHANGED = 0
};


extern const unsigned int TOPIC_SCROLLBAR;

enum SCROLLBAR_MSGS {
   SCROLLBAR_VALUE_CHANGED = 0
};


enum SCROLLER_HANDLE_STYLE {
   SCROLLER_HANDLE_RECTANGLE = 0,
   SCROLLER_HANDLE_ROUNDED = 1
};

class Scroller : public WidgetBase {
   

private :
   Rectangle scroll_handle;
   
   int scroll_max;
   int scroll;
   int scroll_rate;
   
   int view_length;
   float view_scale;// 0.0 to 1.0
   
   bool vertical;// true for vertical scroll bars , false for horizontal ones
   bool active;
   
   int msx_start , msy_start , scroll_start;
   
   SCROLLER_HANDLE_STYLE hstyle;

   void ResetHandle();

public :

   /// Maximum scroll must be >= 1 , vertical scrollers must be at least 10 wide and 20 tall,
   /// and horizontal scrollers must be at least 20 wide and 10 tall
   Scroller(std::string name , int maximum_scroll , int set_scroll , int set_scroll_rate ,
            int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual int AbsMinWidth();// 10 if vertical , 20 if horizontal
   virtual int AbsMinHeight();// 20 if vertical , 10 if horizontal

   /// Setters
   void SetScroll(int new_scroll);
   void ScrollMore();// scrolls by the view length forward
   void ScrollLess();// scrolls by the view length backward
   void ScrollBy(int scroll_by);
   void RatedScrollBy(int scroll_by);

   void SetScrollRate(int new_scroll_rate);

   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);

   /// Getters
   int GetScroll() {return scroll;}
   int GetViewLength() {return view_length;}
   int GetMaxScroll() {return scroll_max;}
};




/// --------------------------------------  Scroll button class  -------------------------------------------------




#include "ButtonWidget.hpp"

enum BUTTON_DIRECTION {
   BTN_UP = 0,
   BTN_RIGHT = 1,
   BTN_DOWN = 2,
   BTN_LEFT = 3
};


/** ScrollButton uses the messages from ButtonBaseWidget.hpp */

class ScrollButton : public Button {
private :
   BUTTON_DIRECTION bdir;
   
public :
   /// Must be at least 10 x 10
   ScrollButton(std::string name , BUTTON_DIRECTION direction , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual void DisplayOn(BITMAP* bmp , int x , int y);/// DisplayOn should always draw, and always 
                                                       /// clear the NEEDS_REDRAW flag.

   virtual int AbsMinWidth();// 10
   virtual int AbsMinHeight();// 10

   void SetDirection(BUTTON_DIRECTION direction);

};



/// -----------------------------------   ScrollBar class   ---------------------------------------------



//*


class ScrollBar : public WidgetBase {

private :
   Scroller scroller;
   ScrollButton more_btn;
   ScrollButton less_btn;

   bool vertical;

   void SetupPositions();

public :


   /// A ScrollBar needs to have a length >= 40 pixels, 10 minimum for each button and 20 for the scroller
   ScrollBar(std::string name , int maximum_scroll , int set_scroll , int set_scroll_rate , 
            int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   
   /// For reference
///   WidgetBase();
///   WidgetBase(const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
///   WidgetBase(BITMAP* surface , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   /// Warning - All widget messages passed back to a WidgetHandler by the Update 
   ///           and CheckInputs functions that are not related to a dialog will be ignored.
   ///    Use QueueUserMessage for user notifications instead.

   /// Pure virtual methods, these must have a derived class counterpart
   virtual WidgetMsg Update (double tsec); // logic handling with animation support

   virtual WidgetMsg CheckInputs(int msx , int msy);// Pass it the mouse position relative to it's drawing target.
                                                      // Hover is taken care of by WidgetHandlers, if you need it to
                                                      // do something, override the virtual SetHoverState function.

   virtual void DisplayOn(BITMAP* bmp , int x , int y);/// DisplayOn should always draw, and always 
                                                         /// clear the NEEDS_REDRAW flag by calling ClearRedrawFlag().

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();



   /// Setters
   void SetScroll(int new_scroll);
   void ScrollMore();// scrolls by the view length forward
   void ScrollLess();// scrolls by the view length backward
   void ScrollBy(int scroll_by);
   void RatedScrollBy(int scroll_by);

   void SetScrollRate(int new_scroll_rate);

   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);

   /// Getters
   int GetScroll() {return scroller.GetScroll();}
   int GetViewLength() {return scroller.GetViewLength();}
   int GetMaxScroll() {return scroller.GetMaxScroll();}
   
         // Scroll bar only
         // PgUp and PgDn scroll by view length
         // Up and Down / Left and Right scroll by 1%? by 1?
};

//*/   



#endif // ScrollerWidget_H



