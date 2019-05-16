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


#include "Eagle/GuiCode/DropDownList.hpp"

using std::string;



const unsigned int TOPIC_DDL = NextFreeTopicId();



void DropDownList::SetupPositions() {
   SetupButton();
   SetupListArea();
   SetupScroller();
}



void DropDownList::SetupButton() {
/*
//   bx = area.X() + 1 + (9*(area.W() - 2))/10;
   int bx = area.X() + 1 + (area.W() - 3) - (area.W() - 2)/10;
   int by = area.Y() + 1;
   int bw = (area.W() - 2)/10;
   int bh = area.H() - 2;
*/
   int bw = 16;
   int bh = area.H() - 2;
   int bx = (area.BRX() - 1) - bw;
   int by = area.Y() + 1;
   list_btn.SetArea(bx,by,bw,bh);
}



void DropDownList::SetupListArea() {
   int lx = area.X() + 1;
   int ly = 0;
   int lw = area.W() - 2;
   int lh = 2 + line_height*nitems_shown;
   switch(list_direction) {
      case DDL_OPEN_ABOVE :
         ly = area.Y() - lh;
         break;
      case DDL_OPEN_BELOW :
         ly = area.BRY() + 1;
         break;
   }
   list_area.SetArea(lx,ly,lw,lh);
}



void DropDownList::SetupScroller() {
   int sw = 16;
   int sh = list_area.H() - 2;
   int sx = (list_area.BRX() - 1) - 16;
   int sy = list_area.Y() + 1;
   list_scroller.SetArea(sx,sy,sw,sh);
   bool show_scroller = (int)items.size() > nitems_shown;
   list_scroller.SetVisibilityState(show_scroller);
   list_scroller.SetEnabledState(show_scroller);
   if (show_scroller) {
//   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);
      list_scroller.SetParameters((int)items.size() , 0 , 1 , nitems_shown);
   }
}


///   Scroller(std::string name , int maximum_scroll , int set_scroll , int set_scroll_rate ,
///            int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

DropDownList::DropDownList(string name , FONT* textfont , int num_items_shown , 
                           const Rectangle& position , DROP_DOWN_LIST_DIRECTION direction , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      list_btn(name + string("-list_btn") , BTN_DOWN , Rectangle(0,0,0,0) , wflags),
      list_scroller(name + string("-list_scroller") , 1 , 0 , 1 , 1 , true , Rectangle(0,0,1,1)),
      scroll(0),
      items(),
      item_index(0),
      text_font(textfont),
      list_open(false),
      item_highlight(0),
      nitems_shown(0),
      line_height(0),
      list_direction(direction),
      list_area()
{
   ASSERT(textfont);
   ASSERT(num_items_shown > 0);
   if (num_items_shown < 1) {num_items_shown = 1;}
   nitems_shown = num_items_shown;
   list_btn.SetParent(this);
   list_scroller.SetParent(this);
   list_btn.UseWidgetColorset(&wc);
   list_scroller.UseWidgetColorset(&wc);
   line_height = text_height(text_font) + 2*(text_height(text_font)/2);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   SetupPositions();
   if (list_direction == DDL_OPEN_ABOVE) {
      list_btn.SetDirection(BTN_UP);
   }
}




WidgetMsg DropDownList::Update(double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg msg = list_btn.Update(tsec);
   retmsg |= msg.Messages();
   msg = list_scroller.Update(tsec);
   retmsg |= msg.Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg DropDownList::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      /// Button press is intercepted in QueueUserMessage
      WidgetMsg msg = list_btn.CheckInputs(msx,msy);
      retmsg |= msg.Messages();
      if (list_open) {
         msg = list_scroller.CheckInputs(msx,msy);
         retmsg |= msg.Messages();
      }
      
      if (retmsg & DIALOG_INPUT_USED) {
         return WidgetMsg(this , TOPIC_DIALOG , retmsg);
      }
      
      if (input_mouse_press(LMB) && area.Contains(msx,msy)) {
         retmsg |= DIALOG_TAKE_FOCUS;
      }
      
      int highlight = item_highlight;
      bool mouse_move = (ms_xdiff) || (ms_ydiff);
      
      if (!list_open) {
         if (flags & HASFOCUS) {
            if (input_key_press(KEY_SPACE) || input_key_press(KEY_ENTER)) {
               list_open = true;
               highlight = 0;
               scroll = 0;
               list_scroller.SetScroll(0);
               switch (list_direction) {
                  case DDL_OPEN_ABOVE :
                     list_btn.SetDirection(BTN_DOWN);
                     break;
                  case DDL_OPEN_BELOW :
                     list_btn.SetDirection(BTN_UP);
                     break;
               }
               SetRedrawFlag();
            }
         }
      } else {
         
         if (flags & HASFOCUS) {
            if (input_key_press(KEY_UP)) {
               --highlight;
               if (highlight < 0) {highlight = 0;}
               if (highlight < scroll) {
                  list_scroller.ScrollBy(-1);
                  scroll = list_scroller.GetScroll();
               }
               retmsg |= DIALOG_INPUT_USED;
               SetRedrawFlag();
            }
            if (input_key_press(KEY_DOWN)) {
               if (items.size()) {
                  ++highlight;
                  if (highlight >= (int)items.size()) {highlight = (int)items.size() - 1;}
                  if (highlight >= (scroll + nitems_shown)) {
                     list_scroller.ScrollBy(1);
                     scroll = list_scroller.GetScroll();
                  }
                  retmsg |= DIALOG_INPUT_USED;
                  SetRedrawFlag();
               }
            }
            if (input_key_press(KEY_SPACE) || input_key_press(KEY_ENTER)) {
               SetItem(highlight);
               list_open = false;
               switch (list_direction) {
                  case DDL_OPEN_ABOVE :
                     list_btn.SetDirection(BTN_UP);
                     break;
                  case DDL_OPEN_BELOW :
                     list_btn.SetDirection(BTN_DOWN);
                     break;
               }
               retmsg |= DIALOG_REDRAW_ALL;
               retmsg |= DIALOG_INPUT_USED;
               SetRedrawFlag();
               QueueUserMessage(WidgetMsg(this , TOPIC_DDL , DDL_SELECTION_CHANGED));
            }
            if (ms_zdiff) {
               mouse_move = true;
               list_scroller.ScrollBy(-ms_zdiff);
               retmsg |= DIALOG_INPUT_USED;
            }
         }
         
         Rectangle l = list_area;
         l.Shrink(1);
         if (l.Contains(msx,msy)) {
            int sw = 0;
            if (list_scroller.Flags() & VISIBLE) {sw = list_scroller.Area().W();}
            Rectangle il(l.X() , l.Y() , l.W() - sw , l.H());// inner list area (not including scroller)
            if (il.Contains(msx,msy)) {
               int lmsy = msy - l.Y();
               if (mouse_move) {
                  highlight = (UINT)(lmsy / line_height) + (UINT)scroll;
               }
               if (input_mouse_press(LMB)) {
                  SetItem(highlight);
                  list_open = false;
                  list_btn.SetDirection(BTN_DOWN);
                  retmsg |= DIALOG_INPUT_USED;
                  retmsg |= DIALOG_REDRAW_ALL;
                  SetRedrawFlag();
                  QueueUserMessage(WidgetMsg(this , TOPIC_DDL , DDL_SELECTION_CHANGED));
               }
            }
         } else {
            if (mouse_move) {
               highlight = items.size();
            }
            if (input_mouse_press(LMB)) {
               if (!(list_btn.Area().Contains(msx,msy))) {
                  list_open = false;
                  switch (list_direction) {
                     case DDL_OPEN_ABOVE :
                        list_btn.SetDirection(BTN_UP);
                        break;
                     case DDL_OPEN_BELOW :
                        list_btn.SetDirection(BTN_DOWN);
                        break;
                  }
                  retmsg |= DIALOG_INPUT_USED;
                  retmsg |= DIALOG_REDRAW_ALL;
                  SetRedrawFlag();
               }
            }
         }
      }
      if (highlight != (int)item_highlight) {
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      }
      item_highlight = highlight;
   }
   
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void DropDownList::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[MGCOL]);
      r.Draw(bmp , (*wcols)[FGCOL]);
      int tx = r.X() + 1 + text_length(text_font , " ");
      int ty = r.Y() + (r.H() - text_height(text_font))/2;
      if (items.size()) {
         const char* text = items[item_index].c_str();
         textout_ex(bmp , text_font , text , tx , ty , (*wcols)[TXTCOL] , -1);
      }
      list_btn.DisplayOn(bmp , x , y);
      if (list_open) {
         Rectangle l = list_area;
         l.Fill(bmp , (*wcols)[MGCOL]);
         l.Draw(bmp , (*wcols)[FGCOL]);
         l.Shrink(1);
         if (item_highlight != items.size()) {
            if (((int)item_highlight >= scroll) && ((int)item_highlight < (scroll + nitems_shown))) {
               int sw = 0;
               if (list_scroller.Flags() & VISIBLE) {sw = list_scroller.Area().W();}
               Rectangle h(l.X() , l.Y() + (item_highlight - scroll)*line_height, l.W() - sw , line_height);
               h.Fill(bmp , (*wcols)[FGCOL]);
            }
         }
         
         tx = l.X() + text_length(text_font , " ");
         ty = l.Y() + text_height(text_font)/2;
         int stop = scroll + nitems_shown;
         if (stop > (int)items.size()) {
            stop = (int)items.size();
         }
         for (int i = scroll ; i < stop ; ++i) {
            textout_ex(bmp , text_font , items[i].c_str() , tx , ty , (*wcols)[TXTCOL] , -1);
            ty += line_height;
         }
         list_scroller.DisplayOn(bmp , x , y);
      }
   }
   ClearRedrawFlag();
}



void DropDownList::QueueUserMessage(const WidgetMsg& wmsg) {
   if (wmsg.IsAbout(&list_btn , TOPIC_BUTTON_WIDGET)) {
      if (wmsg.Messages() == BUTTON_RELEASED) {
         if (list_open) {
            list_open = false;
            switch (list_direction) {
               case DDL_OPEN_ABOVE :
                  list_btn.SetDirection(BTN_UP);
                  break;
               case DDL_OPEN_BELOW :
                  list_btn.SetDirection(BTN_DOWN);
                  break;
            }
            SetRedrawAllFlag();
            /// TODO : Set the area under the list_area as dirty instead of redrawing all?
         } else {
            list_open = true;
            switch (list_direction) {
               case DDL_OPEN_ABOVE :
                  list_btn.SetDirection(BTN_DOWN);
                  break;
               case DDL_OPEN_BELOW :
                  list_btn.SetDirection(BTN_UP);
                  break;
            }
//            item_highlight = items.size();
            item_highlight = 0;
            scroll = 0;
            list_scroller.SetScroll(0);
            SetRedrawFlag();
         }
      }
   } else if (wmsg == WidgetMsg(&list_scroller , TOPIC_SCROLLER , SCROLLER_VALUE_CHANGED)) {
      scroll = list_scroller.GetScroll();
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void DropDownList::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   list_btn.UseWidgetColorset(&wc);
   list_scroller.UseWidgetColorset(&wc);
}



void DropDownList::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      WidgetBase::UseWidgetColorset(colorset);
      list_btn.UseWidgetColorset(colorset);
      list_scroller.UseWidgetColorset(colorset);
   }
}



void DropDownList::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   list_btn.UseWidgetColorset(&wc);
   list_scroller.UseWidgetColorset(&wc);
}



void DropDownList::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   SetupButton();
}



void DropDownList::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   SetupButton();
}



void DropDownList::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   SetupButton();
}



void DropDownList::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   SetupButton();
}



bool DropDownList::IsMouseOver(int x , int y) const {
   if (area.Contains(x,y)) {return true;}
   if (list_open && list_area.Contains(x,y)) {return true;}
   return false;
}


   
int DropDownList::AbsMinWidth() {
   int mintw = 0;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      int tw = text_length(text_font , items[i].c_str());
      if (tw > mintw) {mintw = tw;}
   }
   mintw += 2*text_length(text_font , " ");
   int sw = 0;
   if ((int)items.size() > nitems_shown) {sw = 16;}
   return (2 + mintw + sw);
}



int DropDownList::AbsMinHeight() {
   return 2 + line_height;
}



void DropDownList::AddItem(string str) {
   bool on_list = false;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      if (items[i] == str) {
         on_list = true;
      }
   }
   if (!on_list) {
      items.push_back(str);
      if (items.size() == 1) {
         SetItem(str);
      }
      SetMinDimensions(AbsMinWidth() , AbsMinHeight());
      SetupPositions();
      SetRedrawFlag();
   }
}



DropDownList& DropDownList::operator<<(string str) {
   AddItem(str);
   return *this;
}



void DropDownList::ClearItems() {
   items.clear();
   item_highlight = 0;
   item_index = 0;
   SetupPositions();
   SetRedrawFlag();
}



void DropDownList::SetItem(UINT index) {
   if (index < items.size()) {
      item_index = index;
      SetRedrawFlag();
   }
}



void DropDownList::SetItem(string str) {
   for (UINT i = 0 ; i < items.size() ; ++i) {
      if (items[i] == str) {
         SetItem(i);
      }
   }
}



void DropDownList::SetFont(FONT* new_font) {
   if (new_font && (new_font != text_font)) {
      text_font = new_font;
      line_height = text_height(text_font) + 2*(text_height(text_font)/2);
      SetMinDimensions(AbsMinWidth() , AbsMinHeight());
      SetupPositions();
      SetRedrawFlag();
   }
}



string DropDownList::GetSelection() {
   ASSERT(items.size());// no items
   if (items.size()) {
      return items[item_index];
   }
   return string("");
}



UINT DropDownList::GetSelectionIndex() {
   ASSERT(items.size());// no items!
   if (items.size()) {
      return item_index;
   }
   return items.size();
}







