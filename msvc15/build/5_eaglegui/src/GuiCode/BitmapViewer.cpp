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


#include "Eagle/GuiCode/BitmapViewer.hpp"

using std::string;



const unsigned int TOPIC_BITMAP_VIEWER = NextFreeTopicId();



void BitmapViewer::ResetScrollbars() {
   int hsx,hsy,hsw,hsh,vsx,vsy,vsw,vsh;
   /// Drawn locally to buffer
   hsx = 0;
   hsy = area.H() - sbar_width;
   hsw = area.W() - sbar_width;
   hsh = sbar_width;
   vsx = area.W() - sbar_width;
   vsy = 0;
   vsw = sbar_width;
   vsh = area.H() - sbar_width;
   
   hscroll.SetArea(hsx,hsy,hsw,hsh);
   vscroll.SetArea(vsx,vsy,vsw,vsh);
   
   
   ASSERT(view);
   BITMAP* bmp = (BITMAP*)(*view);
   ASSERT(bmp);
   int w = bmp->w;
   int h = bmp->h;
   int hview = area.W() - sbar_width;
   int vview = area.H() - sbar_width;
   bool wide = (w > hview);
   bool tall = (h > vview);
   hscroll.SetEnabledState(wide);
   hscroll.SetVisibilityState(wide);
   vscroll.SetEnabledState(tall);
   vscroll.SetVisibilityState(tall);

///   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);
   hscroll.SetParameters(w , 0 , 1 , hview);
   vscroll.SetParameters(h , 0 , 1 , vview);
   
   SetRedrawFlag();
}



BitmapViewer::BitmapViewer(string name , BitmapHandler* bitmap , int scroll_bar_width , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      hscroll(name + string("-hscroll") , 1 , 0 , 1 , 1 , false , Rectangle(0,0,50,50)),
      vscroll(name + string("-vscroll") , 1 , 0 , 1 , 1 , true , Rectangle(0,0,50,50)),
      sbar_width(0),
      view(bitmap),
      scroll_rate(10),
      mxstart(0),
      mystart(0),
      hscrstart(0),
      vscrstart(0),
      drag(false)
{
   if (scroll_bar_width < vscroll.AbsMinWidth()) {scroll_bar_width = vscroll.AbsMinWidth();}
   sbar_width = scroll_bar_width;
   hscroll.SetParent(this);
   vscroll.SetParent(this);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   ResetScrollbars();
}



WidgetMsg BitmapViewer::Update(double tsec) {
   hscroll.Update(tsec);
   vscroll.Update(tsec);
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg BitmapViewer::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      WidgetMsg msg;
      
      if (area.Contains(msx,msy)) {
         if (input_mouse_press(LMB)) {
            retmsg |= DIALOG_TAKE_FOCUS;
         }
         if (input_mouse_press(MMB)) {
            drag = true;
            mxstart = mx;
            mystart = my;
            hscrstart = hscroll.GetScroll();
            vscrstart = vscroll.GetScroll();
            retmsg |= DIALOG_INPUT_USED;
         }
         if (ms_wdiff) {
            hscroll.ScrollBy(scroll_rate*ms_wdiff);
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
            SetRedrawFlag();
         }
         if (ms_zdiff) {
            vscroll.ScrollBy(scroll_rate*(-ms_zdiff));
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
            SetRedrawFlag();
         }
      }
      
      if (drag && input_mouse_held(MMB)) {
         int dx = mx - mxstart;
         int dy = my - mystart;
         hscroll.SetScroll(hscrstart - dx);
         vscroll.SetScroll(vscrstart - dy);
         retmsg |= DIALOG_REDRAW_ME;
         retmsg |= DIALOG_INPUT_USED;
      }
      
      if (input_mouse_release(MMB)) {
         drag = false;
      }
      
      if (flags & HASFOCUS) {
         double hold_duration = 0.5;
         if (input_key_press(KEY_LEFT) || key_held_for(KEY_LEFT , hold_duration)) {
            hscroll.ScrollBy(-scroll_rate);
            retmsg |= DIALOG_INPUT_USED;
            retmsg |= DIALOG_REDRAW_ME;
            SetRedrawFlag();
         }
         if (input_key_press(KEY_RIGHT) || key_held_for(KEY_RIGHT , hold_duration)) {
            hscroll.ScrollBy(scroll_rate);
            retmsg |= DIALOG_INPUT_USED;
            retmsg |= DIALOG_REDRAW_ME;
            SetRedrawFlag();
         }
         if (input_key_press(KEY_UP) || key_held_for(KEY_UP , hold_duration)) {
            vscroll.ScrollBy(-scroll_rate);
            retmsg |= DIALOG_INPUT_USED;
            retmsg |= DIALOG_REDRAW_ME;
            SetRedrawFlag();
         }
         if (input_key_press(KEY_DOWN) || key_held_for(KEY_DOWN , hold_duration)) {
            vscroll.ScrollBy(scroll_rate);
            retmsg |= DIALOG_INPUT_USED;
            retmsg |= DIALOG_REDRAW_ME;
            SetRedrawFlag();
         }
      }
      
      int nmsx = msx - area.X();
      int nmsy = msy - area.Y();
      
      msg = hscroll.CheckInputs(nmsx,nmsy);
      retmsg |= msg.Messages();
      msg = vscroll.CheckInputs(nmsx,nmsy);
      retmsg |= msg.Messages();
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void BitmapViewer::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   
   if (WidgetBase::flags & VISIBLE) {
      
      BITMAP* target = bmp;
      if (view && *view) {
         BITMAP* src = (BITMAP*)(*view);
         int hscr = hscroll.GetScroll();
         int vscr = vscroll.GetScroll();
         int hview = area.W() - sbar_width;
         int vview = area.H() - sbar_width;
         bool wide = (src->w > hview);
         bool tall = (src->h > vview);
         bool small = !wide && !tall;
         Rectangle old_clip = GetClippingRectangle(bmp);
         Rectangle clip(x + area.X() , y + area.Y() , hview , vview);
         clip.SetClipRect(bmp);
         if (!small) {
            switch(view->DrawType()) {
               case SOLID :
                  blit(src , target , hscr , vscr , x + area.X() , y + area.Y() , hview , vview);
                  break;
               case TRANSPARENT :
                  masked_blit(src , target , hscr , vscr , x + area.X() , y + area.Y() , hview , vview);
                  break;
               case TRANSLUCENT :
                  set_alpha_blender();
                  draw_trans_sprite(target , src , x + area.X() - hscr , y + area.Y() - vscr);
                  break;
            }
         } else {
            /// if src is not wide enough and not tall enough, center the small image
            int tlx = x + area.X() + (hview - src->w)/2;
            int tly = y + area.Y() + (vview - src->h)/2;
            switch(view->DrawType()) {
               case SOLID :
                  blit(src , target , 0 , 0 , tlx , tly , src->w , src->h);
                  break;
               case TRANSPARENT :
                  masked_blit(src , target , 0 , 0 , tlx , tly , src->w , src->h);
                  break;
               case TRANSLUCENT :
                  set_alpha_blender();
                  draw_trans_sprite(target , src , tlx , tly);
                  break;
            }
         }
         old_clip.SetClipRect(bmp);
         hscroll.DisplayOn(bmp , x + area.X() , y + area.Y());
         vscroll.DisplayOn(bmp , x + area.X() , y + area.Y());
         Rectangle r(x + area.X() + hview , y + area.Y() + vview , sbar_width , sbar_width);
         r.Fill(bmp , (*wcols)[FGCOL]);
         r.Draw(bmp , (*wcols)[MGCOL]);
      }
   }
   ClearRedrawFlag();
}



bool BitmapViewer::Init() {
   return true;
}



bool BitmapViewer::Ready() {
   return true;
}



bool BitmapViewer::Shutdown() {
   return true;
}



void BitmapViewer::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg hmsg(&hscroll , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   const WidgetMsg vmsg(&vscroll , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   
   if ((wmsg == hmsg) || (wmsg == vmsg)) {
      WidgetBase::QueueUserMessage(this , TOPIC_BITMAP_VIEWER , BITMAP_VIEW_CHANGED);
      SetRedrawFlag();
   }
}



void BitmapViewer::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      WidgetBase::UseWidgetColorset(colorset);
      hscroll.UseWidgetColorset(colorset);
      vscroll.UseWidgetColorset(colorset);
   }
}



void BitmapViewer::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   hscroll.UseWidgetColorset(&wc);
   vscroll.UseWidgetColorset(&wc);
}



void BitmapViewer::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetScrollbars();
}



void BitmapViewer::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetScrollbars();
}



void BitmapViewer::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetScrollbars();
}



void BitmapViewer::SetNeedsRedrawState  (bool state) {
   WidgetBase::SetNeedsRedrawState(state);
   if (state) {
      if (view && ((view->DrawType() == TRANSPARENT) || (view->DrawType() == TRANSLUCENT))) {
         WidgetBase::SetNeedsBgRedrawState(true);
      }
   }
}



void BitmapViewer::SetRedrawFlag() {
   if (view && ((view->DrawType() == TRANSPARENT) || (view->DrawType() == TRANSLUCENT))) {
      SetBgRedrawFlag();
   } else {
      WidgetBase::SetRedrawFlag();
   }
}



int BitmapViewer::AbsMinWidth() {
   return (sbar_width + hscroll.AbsMinWidth());
//   return (hscroll.AbsMinWidth() + vscroll.AbsMinWidth());
}



int BitmapViewer::AbsMinHeight() {
   return (sbar_width + vscroll.AbsMinHeight());
//   return (hscroll.AbsMinHeight() + vscroll.AbsMinHeight());
}



void BitmapViewer::RefreshImage() {
   ResetScrollbars();
}



void BitmapViewer::SetScrollRate(int rate) {
   scroll_rate = rate;
}



void BitmapViewer::HScrollTo(int hscr) {
   hscroll.SetScroll(hscr);
   SetRedrawFlag();
}



void BitmapViewer::VScrollTo(int vscr) {
   vscroll.SetScroll(vscr);
   SetRedrawFlag();
}



void BitmapViewer::HScrollBy(int hscr) {
   hscroll.ScrollBy(hscr);
   SetRedrawFlag();
}



void BitmapViewer::VScrollBy(int vscr) {
   vscroll.ScrollBy(vscr);
   SetRedrawFlag();
}



void BitmapViewer::ScrollTo(int hscr , int vscr) {
   HScrollTo(hscr);
   VScrollTo(vscr);
}



void BitmapViewer::ScrollBy(int hscr , int vscr) {
   HScrollBy(hscr);
   VScrollBy(vscr);
}



void BitmapViewer::CenterViewOn(int cx , int cy) {
   int hview = area.W() - sbar_width;
   int vview = area.H() - sbar_width;
   Rectangle r(0,0,hview,vview);
   r.SetCenterPos(cx,cy);
   hscroll.SetScroll(r.X());
   vscroll.SetScroll(r.Y());
   SetRedrawFlag();
}



int BitmapViewer::GetHScroll() {
   return hscroll.GetScroll();
}



int BitmapViewer::GetVScroll() {
   return vscroll.GetScroll();
}





