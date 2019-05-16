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


#ifndef BitmapViewer_H
#define BitmapViewer_H


#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ScrollBarWidget.hpp"
#include "Eagle/BitmapHandler.hpp"


extern const unsigned int TOPIC_BITMAP_VIEWER;

enum BITMAP_VIEWER_MSGS {
   BITMAP_VIEW_CHANGED = 0
};


class BitmapViewer : public WidgetBase {
   

protected :
   ScrollBar hscroll;
   ScrollBar vscroll;
   int sbar_width;
   BitmapHandler* view;
   int scroll_rate;

   int mxstart;
   int mystart;
   int hscrstart;
   int vscrstart;
   bool drag;
   
   void ResetScrollbars();

   

public :

   /// scroll_bar_width has a minimum value of 10, and position width and height have a minimum of 40 + scroll_bar_width
   /// use memory or system type bitmap handlers as the source bitmap or it will be slow to draw on non
   /// video bitmaps. Video BitmapHandlers drawing on video gui may be okay. Check allegro's gfx_capabilites first.
   BitmapViewer(std::string name , BitmapHandler* bitmap , int scroll_bar_width , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

//   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetNeedsRedrawState  (bool state);
   virtual void SetRedrawFlag();
   
   virtual int AbsMinWidth();
   virtual int AbsMinHeight();
   
   
   void RefreshImage();// Call this whenever the source BitmapHandler image changes size.
                       // For image changes only, call BitmapViewer::SetRedrawFlag();
   
   /// Setters
   void SetScrollRate(int rate);// defaults to 10

   void HScrollTo(int hscr);
   void VScrollTo(int vscr);

   void HScrollBy(int hscr);
   void VScrollBy(int vscr);

   void ScrollTo(int hscr , int vscr);
   void ScrollBy(int hscr , int vscr);

   void CenterViewOn(int cx , int cy);
   
   /// Getters
   int GetHScroll();
   int GetVScroll();
};




#endif // BitmapViewer_H




