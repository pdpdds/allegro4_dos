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


#include "Eagle/GuiCode/ScrollBarWidget.hpp"
#include "Eagle/InputHandler.hpp"

using std::string;



const unsigned int TOPIC_SCROLLER = NextFreeTopicId();

const unsigned int TOPIC_SCROLLBAR = NextFreeTopicId();


void Scroller::ResetHandle() {
   ASSERT(scroll_max >= 1);
   if (view_length < 1) {view_length = 1;}
   if (view_length > scroll_max) {view_length = scroll_max;}
   if (vertical) {
      ASSERT(area.H() > 2);
      view_scale = (float)scroll_max / (float)(area.H() - 2);
      int sx = area.X() + 1;
      int sy = area.Y() + 1 + (int)(((float)scroll/(float)scroll_max)*((float)(area.H() - 2)));
      int sw = area.W() - 2;
      int sh = (int)((float)view_length / view_scale);
      scroll_handle.SetArea(sx,sy,sw,sh);
   } else {
      ASSERT(area.W() > 2);
      view_scale = (float)scroll_max / (float)(area.W() - 2);
      int sx = area.X() + 1 + (int)(((float)scroll/(float)scroll_max)*((float)(area.W() - 2)));
      int sy = area.Y() + 1;
      int sw = (int)((float)view_length / view_scale);
      int sh = area.H() - 2;
      scroll_handle.SetArea(sx,sy,sw,sh);
   }
}



Scroller::Scroller(string name , int maximum_scroll , int set_scroll , int set_scroll_rate ,
                  int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      scroll_handle(),
      scroll_max(maximum_scroll),
      scroll(set_scroll),
      scroll_rate(set_scroll_rate),
      view_length(set_view_length),
      view_scale(0.0),
      vertical(set_vertical),
      active(false),
      msx_start(0),msy_start(0),scroll_start(0),
      hstyle(SCROLLER_HANDLE_ROUNDED)
{
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   ResetHandle();
}



WidgetMsg Scroller::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}




WidgetMsg Scroller::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      
      Rectangle scroll_area(area.X() + 1 , area.Y() + 1 , area.W() - 2 , area.H() - 2);
      
      if (input_mouse_press(LMB)) {
         if (scroll_area.Contains(msx,msy)) {
            if (scroll_handle.Contains(msx,msy)) {
               // Check for click drags on scroll handle
               active = true;
               msx_start = msx;
               msy_start = msy;
               scroll_start = scroll;
            } else {
               // Check for clicks outside scroll handle and inside scroll area and scroll by view length
               int cx = scroll_handle.X() + scroll_handle.W()/2;
               int cy = scroll_handle.Y() + scroll_handle.H()/2;
               int dx = msx - cx;
               int dy = msy - cy;
               if (vertical) {
                  if (dy > 0) {
                     ScrollMore();
                  } else {
                     ScrollLess();
                  }
               } else {
                  if (dx > 0) {
                     ScrollMore();
                  } else {
                     ScrollLess();
                  }
               }
               retmsg |= DIALOG_REDRAW_ME;
            }
         }
      }
      
      if (active) {
         if (input_mouse_held(LMB)) {
            // find on screen distance from start point
            float sx = msx - msx_start;
            float sy = msy - msy_start;
            // find actual scroll distance from start point
//            float ratio = (float)scroll_max / (float)scroll_area.H();
            int dx = (int)(sx*view_scale);
            int dy = (int)(sy*view_scale);
            // scroll to scroll start plus scroll distance
            if (vertical) {
               SetScroll(scroll_start + dy);
            } else {
               SetScroll(scroll_start + dx);
            }
            retmsg |= DIALOG_REDRAW_ME;
         }
         if (input_mouse_release(LMB)) {
            active = false;
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void Scroller::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[MGCOL]);
      r.Draw(bmp , (*wcols)[FGCOL]);
      
      Rectangle s = scroll_handle;
      s.MoveBy(x,y);
      Rectangle s2 = s;
      s2.Shrink(1);
      
      switch (hstyle) {
         case SCROLLER_HANDLE_RECTANGLE :
            s2.Fill(bmp , (*wcols)[FGCOL]);
            s2.Draw(bmp , (*wcols)[SDCOL]);
            break;
         case SCROLLER_HANDLE_ROUNDED :
            {
               int corner_radius = 0;
               if (vertical) {
                  corner_radius = ((s2.W()<s2.H())?s2.W():s2.H())/4;
               } else {
                  corner_radius = ((s2.W()<s2.H())?s2.W():s2.H())/4;
               }
               s2.RoundedFill(bmp , corner_radius , (*wcols)[FGCOL]);
               s2.RoundedOutline(bmp , corner_radius , (*wcols)[SDCOL]);
               break;
            }
      }
///      s.RoundedFill(bmp , 5 , (*wcols)[FGCOL]);// corner radius too large for small handles
      if (vertical) {
         // draw three horizontal stripes
         int cy = s.Y() + s.H()/2;
//         int lx = s.X() + 2;
//         int rx = s.X() + s.W() - 3;
         int lx = s.X() + s.W()/4;
         int rx = s.X() + (3*s.W())/4;
         int color = (*wcols)[MGCOL];
         hline(bmp , lx , cy - 3 , rx , color);
         hline(bmp , lx , cy     , rx , color);
         hline(bmp , lx , cy + 3 , rx , color);
      } else {
         // draw three vertical stripes
         int cx = s.X() + s.W()/2;
//         int ty = s.Y() + 2;
//         int by = s.Y() + s.H() - 3;
         int ty = s.Y() + s.H()/4;
         int by = s.Y() + (3*s.H())/4;
         int color = (*wcols)[MGCOL];
         vline(bmp , cx - 3 , ty , by , color);
         vline(bmp , cx     , ty , by , color);
         vline(bmp , cx + 3 , ty , by , color);
      }
   }
   ClearRedrawFlag();
}



void Scroller::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetHandle();
}



void Scroller::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetHandle();
}



void Scroller::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetHandle();
}



int Scroller::AbsMinWidth() {
   int amw = 10;
   if (!vertical) {amw = 20;}
   return amw;
}



int Scroller::AbsMinHeight() {
   int amh = 10;
   if (vertical) {amh = 20;}
   return amh;
}



void Scroller::SetScroll(int new_scroll) {
   if (new_scroll < 0) {new_scroll = 0;}
   int view_max = scroll_max - view_length;
   if (new_scroll > view_max) {new_scroll = view_max;}
   scroll = new_scroll;
   ResetHandle();
   QueueUserMessage(this , TOPIC_SCROLLER , SCROLLER_VALUE_CHANGED);
   SetRedrawFlag();
}



void Scroller::ScrollMore() {
   SetScroll(scroll + view_length);
}



void Scroller::ScrollLess() {
   SetScroll(scroll - view_length);
}



void Scroller::ScrollBy(int scroll_by) {
   SetScroll(scroll + scroll_by);
}



void Scroller::RatedScrollBy(int scroll_by) {
   SetScroll(scroll + scroll_by*scroll_rate);
}



void Scroller::SetScrollRate(int new_scroll_rate) {
   if (new_scroll_rate < 1) {new_scroll_rate = 1;}
   scroll_rate = new_scroll_rate;
}



void Scroller::SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length) {
   scroll_max = maximum_scroll;
   scroll = set_scroll;
   scroll_rate = set_scroll_rate;
   view_length = set_view_length;
   ResetHandle();
}



/// ------------------------------  Scroll button class  ---------------------------------------

ScrollButton::ScrollButton(string name , BUTTON_DIRECTION direction , const Rectangle& position , UINT wflags) :
      Button(name , RECTANGLE_BTN , SPRING_BTN , font , "" , input_key_press(KEY_NONE) , position , wflags),
      bdir(direction)
{
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
}



void ScrollButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      if (Button::up) {
         switch (bdir) {
            /// FILL color , OUTLINE color , TFILL color
            case BTN_UP :
//               r.DrawGuiTriangleFaceUp(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[BGCOL]);
               r.DrawGuiTriangleFaceUp(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL] , (*wcols)[MGCOL]);
               break;
            case BTN_RIGHT :
//               r.DrawGuiTriangleFaceRight(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[BGCOL]);
               r.DrawGuiTriangleFaceRight(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL] , (*wcols)[MGCOL]);
               break;
            case BTN_DOWN :
//               r.DrawGuiTriangleFaceDown(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[BGCOL]);
               r.DrawGuiTriangleFaceDown(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL] , (*wcols)[MGCOL]);
               break;
            case BTN_LEFT :
//               r.DrawGuiTriangleFaceLeft(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[BGCOL]);
               r.DrawGuiTriangleFaceLeft(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL] , (*wcols)[MGCOL]);
               break;
         }
      } else {
         switch (bdir) {
            /// FILL color , OUTLINE color , TFILL color
            case BTN_UP :
//               r.DrawGuiTriangleFaceUp(bmp , (*wcols)[FGCOL] , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               r.DrawGuiTriangleFaceUp(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
               break;
            case BTN_RIGHT :
//               r.DrawGuiTriangleFaceRight(bmp , (*wcols)[FGCOL] , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               r.DrawGuiTriangleFaceRight(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
               break;
            case BTN_DOWN :
//               r.DrawGuiTriangleFaceDown(bmp , (*wcols)[FGCOL] , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               r.DrawGuiTriangleFaceDown(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
               break;
            case BTN_LEFT :
//               r.DrawGuiTriangleFaceLeft(bmp , (*wcols)[FGCOL] , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               r.DrawGuiTriangleFaceLeft(bmp , (*wcols)[FGCOL] , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
               break;
         }
      }
   }
   ClearRedrawFlag();
}



int ScrollButton::AbsMinWidth() {
   return 10;
}



int ScrollButton::AbsMinHeight() {
   return 10;
}



void ScrollButton::SetDirection(BUTTON_DIRECTION direction) {
   bdir = direction;
   SetRedrawFlag();
}



/// ------------------------------------------ ScrollBar class  ------------------------------------------



void ScrollBar::SetupPositions() {
   // Buttons take up 1/10 of the length each, minimum of 10 long , scroller is minimum 20 long
   int length = vertical?area.H():area.W();
   
   ASSERT(length >= 40);
   
   int btn_length = length/10;
   if (btn_length < 10) {btn_length = 10;}
   int scroller_length = length - 2*btn_length;
   
   ASSERT(scroller_length >= 20);
   
   int bw,bh,lx,ly,sx,sy,sw,sh,mrx,mry;
   if (vertical) {
      bw = area.W();
      bh = btn_length;
      lx = area.X();
      ly = area.Y();
      sx = area.X();
      sy = ly + bh;
      sw = area.W();
      sh = scroller_length;
      mrx = area.X();
      mry = sy + sh;
   } else {
      bw = btn_length;
      bh = area.H();
      lx = area.X();
      ly = area.Y();
      sx = lx + bw;
      sy = area.Y();
      sw = scroller_length;
      sh = area.H();
      mrx = sx + sw;
      mry = area.Y();
   }
   less_btn.SetArea(lx,ly,bw,bh);
   more_btn.SetArea(mrx,mry,bw,bh);
   scroller.SetArea(sx,sy,sw,sh);
}



ScrollBar::ScrollBar(string name , int maximum_scroll , int set_scroll , int set_scroll_rate ,
                     int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      scroller(name + string("-scroller") , maximum_scroll , set_scroll , set_scroll_rate , set_view_length , set_vertical , Rectangle(0,0,10,10)),
      more_btn(name + string("-more_btn") , (set_vertical?BTN_DOWN:BTN_RIGHT), Rectangle(0,0,0,0)),
      less_btn(name + string("-less_btn") , (set_vertical?BTN_UP:BTN_LEFT), Rectangle(0,0,0,0)),
      vertical(set_vertical)
{
   scroller.SetParent(this);
   more_btn.SetParent(this);
   less_btn.SetParent(this);
   more_btn.SetSpringDuration(0.05);
   less_btn.SetSpringDuration(0.05);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   SetupPositions();
}




WidgetMsg ScrollBar::Update(double tsec) {
//      scroller.Update(tsec);
   more_btn.Update(tsec);
   less_btn.Update(tsec);
   
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}

WidgetMsg ScrollBar::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      WidgetMsg msg;
      msg = scroller.CheckInputs(msx,msy);
      retmsg |= msg.Messages();
      msg = more_btn.CheckInputs(msx,msy);
      retmsg |= msg.Messages();
      msg = less_btn.CheckInputs(msx,msy);
      retmsg |= msg.Messages();
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}

void ScrollBar::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
      scroller.DisplayOn(bmp,x,y);
      more_btn.DisplayOn(bmp,x,y);
      less_btn.DisplayOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



void ScrollBar::QueueUserMessage(const WidgetMsg& wmsg) {
   if (wmsg == WidgetMsg(&scroller , TOPIC_SCROLLER , SCROLLER_VALUE_CHANGED)) {
      SetRedrawFlag();
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED));
      return;
   }
   double mouse_btn_duration = 0.5;
   if (wmsg.IsAbout(&more_btn , TOPIC_BUTTON_WIDGET)) {
      switch (wmsg.Messages()) {
         case BUTTON_CLICKED :
            RatedScrollBy(1);
            SetRedrawFlag();
            WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED));
            break;
         case BUTTON_HELD :
            if (mb_held_for(LMB , mouse_btn_duration)) {
               RatedScrollBy(1);
               SetRedrawFlag();
               WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED));
            }
            break;
         case BUTTON_RELEASED : break;
         case BUTTON_TOGGLED  : break;
      }
      return;
   }
   if (wmsg.IsAbout(&less_btn , TOPIC_BUTTON_WIDGET)) {
      switch (wmsg.Messages()) {
         case BUTTON_CLICKED :
            RatedScrollBy(-1);
            SetRedrawFlag();
            WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED));
            break;
         case BUTTON_HELD :
            if (mb_held_for(LMB , mouse_btn_duration)) {
               RatedScrollBy(-1);
               SetRedrawFlag();
               WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED));
            }
            break;
         case BUTTON_RELEASED : break;
         case BUTTON_TOGGLED  : break;
      }
      return;
   }
}




void ScrollBar::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      scroller.UseWidgetColorset(colorset);
      more_btn.UseWidgetColorset(colorset);
      less_btn.UseWidgetColorset(colorset);
   }
}



void ScrollBar::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   scroller.UseWidgetColorset(&wc);
   more_btn.UseWidgetColorset(&wc);
   less_btn.UseWidgetColorset(&wc);
}



void ScrollBar::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   SetupPositions();
}



void ScrollBar::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width , height);
   SetupPositions();
}



void ScrollBar::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   SetupPositions();
}



void ScrollBar::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   SetupPositions();
}



int ScrollBar::AbsMinWidth() {
   int scramw = scroller.AbsMinWidth();
   int btnamw = more_btn.AbsMinWidth();
   int amw = (scramw > btnamw)?scramw:btnamw;
   if (!vertical) {amw = scramw + 2*btnamw;}
   return amw;
}



int ScrollBar::AbsMinHeight() {
   int scramh = scroller.AbsMinHeight();
   int btnamh = more_btn.AbsMinHeight();
   int amh = (scramh > btnamh)?scramh:btnamh;
   if (vertical) {amh = scramh + 2*btnamh;}
   return amh;
}



void ScrollBar::SetScroll(int new_scroll) {
   scroller.SetScroll(new_scroll);
}



void ScrollBar::ScrollMore() {
   scroller.ScrollMore();
}



void ScrollBar::ScrollLess() {
   scroller.ScrollLess();
}



void ScrollBar::ScrollBy(int scroll_by) {
   scroller.ScrollBy(scroll_by);
}



void ScrollBar::RatedScrollBy(int scroll_by) {
   scroller.RatedScrollBy(scroll_by);
}



void ScrollBar::SetScrollRate(int new_scroll_rate) {
   scroller.SetScrollRate(new_scroll_rate);
}



void ScrollBar::SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length) {
   scroller.SetParameters(maximum_scroll , set_scroll , set_scroll_rate , set_view_length);
}








