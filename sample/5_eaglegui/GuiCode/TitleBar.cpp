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


#include "Eagle/GuiCode/TitleBar.hpp"
#include "Eagle/GuiCode/WidgetHandler.hpp"

using std::string;



const unsigned int TOPIC_MINIMIZE_BUTTON = NextFreeTopicId();
const unsigned int TOPIC_CLOSE_BUTTON = NextFreeTopicId();
const unsigned int TOPIC_TITLE_BAR = NextFreeTopicId();



/// -------------------------------- Minimize Button ------------------------------------



MinimizeButton::MinimizeButton(string name , bool is_minimized , const Rectangle& position , UINT wflags) :
      Button(name , RECTANGLE_BTN , TOGGLE_BTN , font , "" , input_key_press(KEY_NONE) , position , wflags),
      minimized(is_minimized)
{
   
}



WidgetMsg MinimizeButton::CheckInputs(int msx , int msy) {
   WidgetMsg retmsg(this , TOPIC_DIALOG , DIALOG_OKAY);
   if (Flags() & ENABLED) {
      if (input_mouse_press(LMB)) {
         if (Area().Contains(msx,msy)) {
            ToggleMinimizeState();
         }
      }
   }
   return retmsg;
}



void MinimizeButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (Flags() & VISIBLE) {
      Rectangle r = Area();
      r.MoveBy(x,y);
      int col = (*wcols)[FGCOL];
      if (parent && (parent->Flags() & HASFOCUS)) {
         col = (*wcols)[BGCOL];
      }
      r.Draw(bmp , col);
      int lx = r.X() + (2*r.W())/10;
      int rx = r.X() + (8*r.W())/10;
      int ty = r.Y() + (2*r.H())/10;
      int by = r.Y() + (8*r.H())/10;
      if (minimized) {
         // show maximize button
         rect(bmp , lx , ty , rx , by , col);
         rect(bmp , lx + 1 , ty + 1 , rx - 1 , by - 1 , col);
      } else {
         // show minimize button
         hline(bmp , lx , by , rx , col);
         hline(bmp , lx , by - 1 , rx , col);
      }
   }
   ClearRedrawFlag();
}



void MinimizeButton::Minimize() {
   minimized = true;
   WidgetBase::QueueUserMessage(this , TOPIC_MINIMIZE_BUTTON , MINIMIZED);
   SetBgRedrawFlag();
}



void MinimizeButton::Maximize() {
   minimized = false;
   WidgetBase::QueueUserMessage(this , TOPIC_MINIMIZE_BUTTON , MAXIMIZED);
   SetBgRedrawFlag();
}



void MinimizeButton::SetMinimizeState(bool is_minimized) {
   if (is_minimized) {
      Minimize();
   } else {
      Maximize();
   }
}



void MinimizeButton::ToggleMinimizeState() {
   SetMinimizeState(!minimized);
}



/// -------------------------------- Close Button ---------------------------------------



CloseButton::CloseButton(string name , const Rectangle& position , UINT wflags) :
      Button(name , RECTANGLE_BTN , SPRING_BTN , font , "" , input_key_press(KEY_NONE) , position , wflags | ALLOW_CLOSE)
{}



WidgetMsg CloseButton::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (Flags() & ENABLED) {
      if (input_mouse_press(LMB)) {
         if (Area().Contains(msx,msy)) {
            QueueUserMessage(this , TOPIC_CLOSE_BUTTON , SIGNAL_CLOSE);
            retmsg |= DIALOG_CLOSE;
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void CloseButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (Flags() & VISIBLE) {
      Rectangle r = Area();
      r.MoveBy(x,y);
      int col = (*wcols)[FGCOL];
      if (parent && (parent->Flags() & HASFOCUS)) {
         col = (*wcols)[BGCOL];
      }
      r.Draw(bmp , col);
      int lx = r.X() + (2*r.W())/10;
      int rx = r.X() + (8*r.W())/10;
      int ty = r.Y() + (2*r.H())/10;
      int by = r.Y() + (8*r.H())/10;
      line(bmp , lx , ty , rx , by , col);
      line(bmp , lx + 1, ty , rx + 1, by , col);
      line(bmp , lx , by , rx , ty , col);
      line(bmp , lx + 1 , by , rx + 1, ty , col);
   }
   ClearRedrawFlag();
}




/// ------------------------------------ Title Bar --------------------------------------------



TitleBar::TitleBar(string name , FONT* text_font , string title_text , bool is_minimized , bool allow_movement) :
      WidgetBase(name),
      title(name + string("-title") , title_text , text_font , TEXT_LEFT , TEXT_VCENTER , 0 , 0),
      textarea(0,0,0,0),
      min_btn(name + string("-min_btn") , is_minimized , Rectangle(0,0,0,0)),
      close_btn(name + string("-close_btn") , Rectangle(0,0,0,0)),
      parent_gui(0),
      allow_move(allow_movement),start_move(false),
      gui_start(),
      start_x(0),start_y(0)
{
   ASSERT(text_font);
   title.SetParent(this);
   min_btn.SetParent(this);
   close_btn.SetParent(this);
   title.UseWidgetColorset(&wc);
   min_btn.UseWidgetColorset(&wc);
   close_btn.UseWidgetColorset(&wc);
}



WidgetMsg TitleBar::Update(double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg msg1 = title.Update(tsec);
   WidgetMsg msg2 = min_btn.Update(tsec);
   WidgetMsg msg3 = close_btn.Update(tsec);
   retmsg |= msg1.Messages();
   retmsg |= msg2.Messages();
   retmsg |= msg3.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg TitleBar::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      WidgetMsg msg1 = title.CheckInputs(msx,msy);
      WidgetMsg msg2 = min_btn.CheckInputs(msx,msy);
      WidgetMsg msg3 = close_btn.CheckInputs(msx,msy);
      retmsg |= msg1.Messages();
      retmsg |= msg2.Messages();
      retmsg |= msg3.Messages();
      
      if (allow_move && start_move && input_mouse_held(LMB)) {
         int dx = mouse_x - start_x;
         int dy = mouse_y - start_y;
         int gx = gui_start.X();
         int gy = gui_start.Y();
         parent_gui->SetDrawPos(gx + dx , gy + dy);
         retmsg |= DIALOG_INPUT_USED;
      }
      
      if (input_mouse_release(LMB)) {start_move = false;}
      
      if (input_mouse_press(LMB)) {
         if (area.Contains(msx,msy)) {
            Rectangle r1 = min_btn.Area();
            Rectangle r2 = close_btn.Area();
            if (!r1.Contains(msx,msy) && !r2.Contains(msx,msy)) {
               if (parent_gui) {
                  start_move = true;
                  gui_start = parent_gui->Area();
                  start_x = mouse_x;
                  start_y = mouse_y;
                  retmsg |= DIALOG_TAKE_FOCUS;
                  retmsg |= DIALOG_INPUT_USED;
               }
            }
         }
      }
      
      
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void TitleBar::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   
   if (WidgetBase::flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      if (wcols->ColorDepth() == 32) {
         if (flags & HASFOCUS) {
//            r.DrawAlphaGradientLtoR(bmp , (*wcols)[BGCOL] , (*wcols)[FGCOL]);
            r.DrawAlphaGradientLtoR(bmp , (*wcols)[MGCOL] , (*wcols)[HLCOL]);
            r.Draw(bmp , (*wcols)[FGCOL]);
         } else {
//            r.DrawAlphaGradientLtoR(bmp , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
            r.DrawAlphaGradientLtoR(bmp , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
            r.Draw(bmp , (*wcols)[SDCOL]);
         }
      } else {
         if (flags & HASFOCUS) {
//            r.DrawGradientLtoR(bmp , (*wcols)[BGCOL] , (*wcols)[FGCOL]);
            r.DrawGradientLtoR(bmp , (*wcols)[MGCOL] , (*wcols)[HLCOL]);
            r.Draw(bmp , (*wcols)[FGCOL]);
         } else {
//            r.DrawGradientLtoR(bmp , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
            r.DrawGradientLtoR(bmp , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
            r.Draw(bmp , (*wcols)[SDCOL]);
         }
      }
      Rectangle t = textarea;
      t.MoveBy(x,y);
      Rectangle old_clip = GetClippingRectangle(bmp);
      t.SetClipRect(bmp);
      title.DisplayOn(bmp,x,y);
      old_clip.SetClipRect(bmp);
      min_btn.DisplayOn(bmp,x,y);
      close_btn.DisplayOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



void TitleBar::QueueUserMessage(const WidgetMsg& wmsg) {
   
   const WidgetMsg minmsg(&min_btn , TOPIC_MINIMIZE_BUTTON , MINIMIZED);
   const WidgetMsg maxmsg(&min_btn , TOPIC_MINIMIZE_BUTTON , MAXIMIZED);
   const WidgetMsg closemsg(&close_btn , TOPIC_CLOSE_BUTTON , SIGNAL_CLOSE);
   
   if (wmsg == minmsg) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_TITLE_BAR , TITLE_MINIMIZED));
   } else if (wmsg == maxmsg) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_TITLE_BAR , TITLE_MAXIMIZED));
   } else if (wmsg == closemsg) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_TITLE_BAR , TITLE_SIGNAL_CLOSE));
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void TitleBar::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   title.UseWidgetColorset(&wc);
   min_btn.UseWidgetColorset(&wc);
   close_btn.UseWidgetColorset(&wc);
}



void TitleBar::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   title.UseWidgetColorset(wcols);
   min_btn.UseWidgetColorset(wcols);
   close_btn.UseWidgetColorset(wcols);
}



void TitleBar::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   title.UseWidgetColorset(&wc);
   min_btn.UseWidgetColorset(&wc);
   close_btn.UseWidgetColorset(&wc);
}



void TitleBar::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetPositions(min_btn.Minimized());
}



void TitleBar::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetPositions(min_btn.Minimized());
}



void TitleBar::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetPositions(min_btn.Minimized());
}



void TitleBar::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetPositions(min_btn.Minimized());
}



void TitleBar::ResetPositions(bool minimized) {
   if (!minimized) {
      if (parent_gui) {
         WidgetBase::SetArea(parent_gui->GetTitleBarArea());
///         area = parent_gui->GetTitleBarArea();
      }
   }
   int x = area.X() , y = area.Y() , w = area.W() , h = area.H();
   int cbx = x + w + 1 - h , cby = y + 1, cbw = h - 2 , cbh = h - 2;
   int mbx = cbx - h + 1 , mby = y + 1, mbw = h - 2, mbh = h - 2;
   int tx = x + h + 1, ty = y + 1 , tw = w - 3*h , th = h - 2;
   int textx = tx , texty = ty + th/2;
   textarea.SetArea(tx,ty,tw,th);
///   title.SetArea(tx,ty,tw,th);
   title.SetTextPos(textx,texty);
   min_btn.SetArea(mbx,mby,mbw,mbh);
   close_btn.SetArea(cbx,cby,cbw,cbh);
///   SetRedrawAllFlag();
}



void TitleBar::SetParentGui(WidgetHandler* gui) {
   ASSERT(gui);
   if (gui) {
      parent_gui = gui;
      ResetPositions(false);
   }
}



void TitleBar::SetTitle(string title_text) {
   title.SetText(title_text);
}



void TitleBar::SetFont(FONT* textfont) {
   ASSERT(textfont);
   title.SetFont(textfont);
}



void TitleBar::Minimize() {
   min_btn.Minimize();
   SetRedrawFlag();
}



void TitleBar::Maximize() {
   min_btn.Maximize();
   SetRedrawFlag();
}



void TitleBar::SetMinimizeState(bool is_minimized) {
   if (is_minimized) {
      Minimize();
   } else {
      Maximize();
   }
}



void TitleBar::ToggleMinimizeState() {
   min_btn.ToggleMinimizeState();
}







