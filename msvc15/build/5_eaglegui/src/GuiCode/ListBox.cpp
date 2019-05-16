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


#include "Eagle/GuiCode/ListBox.hpp"
#include "Eagle/GuiCode/FileSelectorDialog.hpp"

using std::map;
using std::list;
using std::string;



const unsigned int TOPIC_LISTBOX = NextFreeTopicId();



ListItem::ListItem(string s , bool is_selected) :
      str(s),
      selected(is_selected)
{
}



void ListBox::SetupPositions() {
   /// 1px outline , 16px wide scrollers
   int scrw = 16;
   ASSERT(area.W() >= (40 + scrw + 2*1));
   ASSERT(area.H() >= (40 + scrw + 2*1));
   list_area.SetArea(area.X() + 1 , area.Y() + 1 , area.W() - (2 + scrw) , area.H() - (2 + scrw));
   vscroller.SetArea(list_area.BRX() + 1 , area.Y() + 1 , scrw , area.H() - (2 + scrw));
   hscroller.SetArea(area.X() + 1 , list_area.BRY() + 1 , area.W() - (2 + scrw) , scrw);
}



int ListBox::MaxItemLength() {
   int maxl = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      int l = text_length(text_font , li.str.c_str());
      if (l > maxl) {maxl = l;}
   }
   return maxl;
}



void ListBox::ResetScrollBars() {
   int maxl = MaxItemLength() + 2*padding;
///   int maxlines = list_area.H() / lineheight;
///   int extralines = (int)items.size() - maxlines;
   int extrapixels = (int)items.size()*lineheight - list_area.H();
//   void SetParameters(int maximum_scroll , int set_scroll , int set_scroll_rate , int set_view_length);
   if (maxl > list_area.W()) {
      hscroller.ShowWidget();
///      hscroller.SetParameters(maxl , 0 , text_length(text_font , " ") , list_area.W());/// Scroll by letter
      hscroller.SetParameters(maxl , 0 , 1 , list_area.W());/// Scroll by pixel
      hscroll = 0;
   } else {
      hscroller.HideWidget();
      hscroller.SetParameters(1,0,1,1);
   }
///   if (extralines > 0) {
   if (extrapixels > 0) {
      vscroller.ShowWidget();
///      vscroller.SetParameters(items.size() , 0 , 1 , maxlines);/// Scroll by item
      vscroller.SetParameters(items.size()*lineheight , 0 , 1 , list_area.H());/// Scroll by pixel
      vscroll = 0;
   } else {
      vscroller.HideWidget();
      vscroller.SetParameters(1,0,1,1);
   }
   SetBgRedrawFlag();
}



bool ListBox::ItemInView(UINT uindex) {
   int index = (int)uindex;
   int maxlines = list_area.H()/lineheight;
   int start = vscroller.GetScroll();
   int end = start + maxlines - 1;
   if (index < start) {return false;}
   if (index > end) {return false;}
   return true;
}



ListBox::ListBox(string name , FONT* textfont , bool multiple_selections_allowed , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      items(),
//   ScrollBar(std::string name , int maximum_scroll , int set_scroll , int set_scroll_rate , 
//            int set_view_length , bool set_vertical , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
      vscroller(name + string("-vscroller") , 10 , 0 , 1 , 1 , true , Rectangle(0,0,0,40)),
      hscroller(name + string("-hscroller") , 10 , 0 , 1 , 1 , false , Rectangle(0,0,40,0)),
      vscroll(0),
      hscroll(0),
      text_font(0),
      padding(0),
      lineheight(0),
      list_area(0,0,0,0),
      item_anchor(0),
      item_anchored(false),
      multiselect(multiple_selections_allowed)
{
   vscroller.SetParent(this);
   hscroller.SetParent(this);
   vscroller.UseWidgetColorset(wcols);
   hscroller.UseWidgetColorset(wcols);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   SetupPositions();
   SetFont(textfont);
}



WidgetMsg ListBox::Update (double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg wmsg1 = vscroller.Update(tsec);
   WidgetMsg wmsg2 = hscroller.Update(tsec);
   retmsg |= wmsg1.Messages();
   retmsg |= wmsg2.Messages();
   
   return (WidgetMsg(this , TOPIC_DIALOG , retmsg));
}



WidgetMsg ListBox::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (flags & ENABLED) {
      WidgetMsg wmsg1 = vscroller.CheckInputs(msx,msy);
      WidgetMsg wmsg2 = hscroller.CheckInputs(msx,msy);
      retmsg |= wmsg1.Messages();
      retmsg |= wmsg2.Messages();
      /// Check for no mod left clicks on list items , as well as shift left clicks and ctrl left clicks
      if (input_mouse_press(LMB) && list_area.Contains(msx,msy)) {
         retmsg |= DIALOG_TAKE_FOCUS;
//            int tx = list_area.X() - hscroll;
///         int ty = list_area.Y() - lineheight*vscroll;
         int ty = list_area.Y() - vscroll;
//            int dx = msx - tx;
         int dy = msy - ty;
         int item_num = dy / lineheight;
         if (input_key_held(KEY_NO_MOD) || input_mouse_double_click(LMB)) {
            ClearSelections();
            SelectItems(item_num , item_num , true);
            item_anchor = item_num;
            item_anchored = true;
            if (input_mouse_double_click(LMB)) {
               WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_DOUBLE_CLICK_SELECT);
               if (Flags() & ALLOW_CLOSE) {
                  retmsg |= DIALOG_REDRAW_ME;
                  retmsg |= DIALOG_INPUT_USED;
                  retmsg |= DIALOG_CLOSE;
                  SetRedrawFlag();
                  return WidgetMsg(this , TOPIC_DIALOG , retmsg);
               }
            }
         }
         if (input_key_held(KEY_ONLY_SHIFT) && item_anchored && multiselect) {
            ClearSelections();
            int item_start = (item_num < item_anchor)?item_num:item_anchor;
            int item_end = (item_num > item_anchor)?item_num:item_anchor;
            SelectItems(item_start , item_end , true);
         }
         if (input_key_held(KEY_ONLY_CTRL) && multiselect) {
            ToggleItem(item_num);
            item_anchored = false;
         }
         retmsg |= DIALOG_REDRAW_ME;
         retmsg |= DIALOG_INPUT_USED;
         SetRedrawFlag();
      }
      if (list_area.Contains(msx,msy)) {
         if (ms_wdiff) {
            int length = text_length(text_font , " ");
            hscroller.ScrollBy(length*ms_wdiff);
            SetRedrawFlag();
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
         }
         if (ms_zdiff) {
            vscroller.ScrollBy(-ms_zdiff*lineheight);
            SetRedrawFlag();
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void ListBox::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      Rectangle l = list_area;
      l.MoveBy(x,y);
//      r.Fill(bmp , (*wcols)[BGCOL]);
      r.Fill(bmp , (*wcols)[SDCOL]);
      r.Draw(bmp , (*wcols)[FGCOL]);
      Rectangle vscr = vscroller.Area();
      Rectangle corner(vscr.X() , vscr.BRY() + 1 , vscr.W() , vscr.W());
      corner.Fill(bmp , (*wcols)[FGCOL]);
      
      vscroller.DisplayOn(bmp , x, y);
      hscroller.DisplayOn(bmp , x, y);
      Rectangle old_clip = GetClippingRectangle(bmp);
      l.SetClipRect(bmp);
      int tx = l.X() - hscroll;
///      int ty = l.Y() - lineheight*vscroll;
      int ty = l.Y() - vscroll;
      int tw = MaxItemLength() + 2*padding;
      int th = lineheight;
      for (LIIT it = items.begin() ; it != items.end() ; ++it) {
         ListItem& li = *it;
         if (li.selected) {
            Rectangle t(tx,ty,tw,th);
            t.Fill(bmp , (*wcols)[MGCOL]);
         }
         textout_ex(bmp , text_font , li.str.c_str() , tx + padding , ty + padding , (*wcols)[TXTCOL] , -1);
         ty += lineheight;
      }
      old_clip.SetClipRect(bmp);
   }
}



void ListBox::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg vsmsg(&vscroller , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   const WidgetMsg hsmsg(&hscroller , TOPIC_SCROLLBAR , SCROLLBAR_VALUE_CHANGED);
   if ((wmsg == vsmsg) || (wmsg == hsmsg)) {
      hscroll = hscroller.GetScroll();
      vscroll = vscroller.GetScroll();
      SetRedrawFlag();
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void ListBox::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   vscroller.UseWidgetColorset(wcols);
   hscroller.UseWidgetColorset(wcols);
}



void ListBox::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   vscroller.UseWidgetColorset(wcols);
   hscroller.UseWidgetColorset(wcols);
}



void ListBox::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   vscroller.UseWidgetColorset(wcols);
   hscroller.UseWidgetColorset(wcols);
}



void ListBox::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   SetupPositions();
}



void ListBox::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   SetupPositions();
   ResetScrollBars();
}



void ListBox::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   SetupPositions();
   ResetScrollBars();
}



void ListBox::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   SetupPositions();
   ResetScrollBars();
}



void ListBox::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   padding = text_height(text_font)/4;
   lineheight = text_height(text_font) + 2*padding;
   ResetScrollBars();
//   vscroller.SetScrollRate(lineheight);
   SetRedrawFlag();
}






int ListBox::AbsMinWidth() {
   return (2 + 16 + hscroller.AbsMinWidth());
}



int ListBox::AbsMinHeight() {
   return (2 + 16 + vscroller.AbsMinHeight());
}



void ListBox::ClearSelections() {
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      li.selected = false;
   }
   WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   SetRedrawFlag();
}



void ListBox::SelectItem(UINT index) {
   UINT i = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (i == index) {
         li.selected = true;
         if (!ItemInView(index)) {
            vscroller.SetScroll(index);
         }
      } else {
         li.selected = false;
      }
      ++i;
   }
   WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   SetRedrawFlag();
}



void ListBox::SelectItem(string s) {
   UINT index = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.str == s) {
         SelectItem(index);
         break;
      }
      ++index;
   }
}



void ListBox::SelectItems(UINT start_index , UINT end_index , bool selected) {
   if (!multiselect) {
      end_index = start_index;
      ClearSelections();
   }
   UINT i = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      if ((i >= start_index) && (i <= end_index)) {
         ListItem& li = *it;
         li.selected = selected;
      }
      ++i;
   }
   WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   SetRedrawFlag();
}



void ListBox::ToggleItem(UINT index) {
   
   ListItem* li = 0;
   if (index < items.size()) {
      UINT i = 0;
      for (LIIT it = items.begin() ; it != items.end() ; ++it) {
         if (i == index) {
            li = &(*it);
         }
         ++i;
      }
      if (multiselect) {
         li->selected = !(li->selected);
      } else {
         if (li->selected) {
            li->selected = false;
         } else {
            ClearSelections();
            li->selected = true;
         }
      }
      WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
      SetRedrawFlag();
   }
}



void ListBox::ToggleItem(string s) {

   ListItem* li = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      if (it->str == s) {
         li = &(*it);
      }
   }
   if (li) {
      if (multiselect) {
         li->selected = !(li->selected);
      } else {
         if (li->selected) {
            li->selected = false;
         } else {
            ClearSelections();
            li->selected = true;
         }
      }
      WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
      SetRedrawFlag();
   }
}



void ListBox::SetItemSelection(UINT index , bool selected) {

   ListItem* li = 0;
   if (index < items.size()) {
      UINT i = 0;
      for (LIIT it = items.begin() ; it != items.end() ; ++it) {
         if (i == index) {
            li = &(*it);
         }
         ++i;
      }
      if (selected) {
         if (!multiselect) {
            ClearSelections();
         }
         li->selected = true;
      } else {
         li->selected = false;
      }
      WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
      SetRedrawFlag();
   }
}



void ListBox::SetItemSelection(string s , bool selected) {

   ListItem* li = 0;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      if (it->str == s) {
         li = &(*it);
      }
   }
   if (li) {
      if (selected) {
         if (!multiselect) {
            ClearSelections();
         }
         li->selected = true;
      } else {
         li->selected = false;
      }
      WidgetBase::QueueUserMessage(this , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
      SetRedrawFlag();
   }
}



void ListBox::AllowMultipleSelections(bool allow) {
   multiselect = allow;
   if (!allow) {
      list<string> selections = GetSelections();
      ClearSelections();
      if (selections.size()) {
         string s = *(selections.begin());
         SetItemSelection(s);
      }
   }
}



void ListBox::AddListItem(string s , bool selected) {
   if (!OnList(s)) {
      items.push_back(ListItem(s,false));
      if (selected) {
         SetItemSelection(s,true);
      }
      ResetScrollBars();
   }
}



ListBox& ListBox::operator<<(string s) {
   AddListItem(s);
   return *this;
}



void ListBox::ClearList() {
   items.clear();
   ResetScrollBars();
}



void ListBox::Remove(string s) {
   LIIT it = items.end();
   for (it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.str == s) {break;}
   }
   if (it != items.end()) {
      items.erase(it);
      ResetScrollBars();
   }
}



void ListBox::SetList(list<string> ls) {
   ClearList();
   for (list<string>::iterator it = ls.begin() ; it != ls.end() ; ++it) {
      items.push_back(ListItem(*it , false));
   }
}



void ListBox::CopyFromFileList() {
   ClearList();
   list<File>& files = GetFileList();
   for (list<File>::iterator it = files.begin() ; it != files.end() ; ++it) {
      File& f = *it;
      const string& s = f.file_path;
      string short_name = GetShortFileName(s); 
      items.push_back(ListItem(short_name , false));
   }
   ResetScrollBars();
}



list<string> ListBox::GetSelections() {
   list<string> sel;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.selected) {
         sel.push_back(li.str);
      }
   }
   return sel;
}



string ListBox::GetSelections(const char* quote , const char* delimiter) {
   const char* nullstr = "";
   if (!quote) {quote = nullstr;}
   if (!delimiter) {delimiter = nullstr;}
   list<string> sel = GetSelections();
   string s("");
   unsigned int i = 0;
   for (list<string>::iterator it = sel.begin() ; it != sel.end() ; ++it) {
      s.append(quote);
      s.append(*it);
      s.append(quote);
      if (sel.size() && (i < (sel.size() - 1))) {
         s.append(delimiter);
      }
      ++i;
   }
   return s;
}



string ListBox::GetSelection() {
   ASSERT(!multiselect);
   const string empty;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.selected) {
         return li.str;
      }
   }
   return empty;
}



bool ListBox::ItemsSelected() {
   for (list<ListItem>::iterator it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.selected) {return true;}
   }
   return false;
}



bool ListBox::OnList(string s) {
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.str == s) {
         return true;
      }
   }
   return false;
}








