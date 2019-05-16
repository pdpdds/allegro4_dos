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


#include "Eagle/GuiCode/TabGroup.hpp"

using std::string;
using std::list;
using std::vector;



void Tab::ResetTabDimensions() {
   WidgetBase::SetDrawDimensions(AbsMinWidth() , AbsMinHeight());
}



Tab::Tab(string name , FONT* textfont , string text , WidgetHandler& tab_gui , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text_font(0),
      label(text),
      active(false),
      gui(0)
{
   SetFont(textfont);// calls ResetTabDimensions()
   SetGui(tab_gui);
//   ResetTabDimensions();
   gui->SetParent(this);
   gui->UseWidgetColorset(wcols);
   gui->SetRedrawAllFlag();
}



WidgetMsg Tab::Update (double tsec) {
   return gui->Update(tsec);
}




WidgetMsg Tab::CheckInputs(int msx , int msy) {
   WidgetMsg ret = gui->CheckInputs(msx,msy);
   if (gui->Flags() & NEEDS_BG_REDRAW) {
      SetBgRedrawFlag();
   }
   return ret;
}



void Tab::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      if (active) {
         int color = (*wcols)[FGCOL];
         r.Fill(bmp , (*wcols)[MGCOL]);
         hline(bmp , r.X() , r.Y() , r.BRX() , color);
         vline(bmp , r.X() , r.Y() , r.BRY() , color);
         vline(bmp , r.BRX() , r.Y() , r.BRY() , color);
         textout_centre_ex(bmp , text_font , label.c_str() ,
                           (r.X() + r.BRX())/2 , (r.Y() + r.BRY() - text_height(text_font))/2 , (*wcols)[TXTCOL] , -1);
         gui->DisplayOn(bmp,x,y);
         
      } else {
         int color = (*wcols)[MGCOL];
         int ty = r.Y() + (3*r.H())/10;
         rectfill(bmp , r.X() , ty , r.BRX() , r.BRY() , (*wcols)[SDCOL]);
         hline(bmp , r.X() , ty , r.BRX() , color);
         vline(bmp , r.X() , ty , r.BRY() , color);
         vline(bmp , r.BRX() , ty , r.BRY() , color);
         textout_centre_ex(bmp , text_font , label.c_str() ,
                           (r.X() + r.BRX())/2 , (ty + r.BRY() - text_height(text_font))/2 , (*wcols)[TXTCOL] , -1);
      }
   }
   ClearRedrawFlag();
}



void Tab::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   if (gui) {
      gui->UseWidgetColorset(colorset);
   }
}



void Tab::SetDrawTarget(BITMAP* surface) {
   WidgetBase::SetDrawTarget(surface);
   if (gui) {
      gui->SetDrawTarget(surface);
   }
}



void Tab::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   gui->SetRedrawAllFlag();
}



void Tab::SetDrawDimensions(int width , int height) {
   // Does nothing , width and height are fixed based on the font
}



void Tab::SetArea(int xpos , int ypos , int width , int height) {
   SetDrawPos(xpos,ypos);
   // width and height are fixed based on the font
}



void Tab::SetArea(const Rectangle& r) {
   SetDrawPos(r.X() , r.Y());
   // width and height are fixed based on the font
}



void Tab::ActivateTab(bool tab_active) {
   if (active != tab_active) {
      active = tab_active;
      if (gui) {
         gui->SetRedrawAllFlag();
      }
      SetBgRedrawFlag();
   }
}



void Tab::SetLabel(string new_label) {
   label = new_label;
   ResetTabDimensions();
}



void Tab::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   ResetTabDimensions();
}



void Tab::SetGui(WidgetHandler& tab_gui) {
   if (gui) {
      gui->SetParent(NULL);
   }
   
   gui = &tab_gui;
   gui->SetParent(this);
   gui->UseWidgetColorset(wcols);
   gui->SetRedrawAllFlag();
   SetBgRedrawFlag();
}



int Tab::AbsMinWidth() {
   return (10 + text_length(text_font , label.c_str()));
}



int Tab::AbsMinHeight() {
   return (2 + 2*text_height(text_font));
}



WidgetHandler* Tab::GetGui() {
   ASSERT(gui);// Should always be true since the constructor takes a WidgetHandler reference
   return gui;
}





/// -----------------------------------------   TabGroup class     ------------------------------------


typedef vector<Tab*> TAB_ROW;
typedef TAB_ROW::iterator TRIT;

typedef list<TAB_ROW> TAB_ROWS;
typedef TAB_ROWS::iterator    TRSIT;



void TabGroup::ResetTabPositions() {
   
   // find tallest tab and use that height for spacing
   int maxheight = 0;
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         int height = t->Area().H();
         if (height > maxheight) {maxheight = height;}
      }
   }
   
   // position all tabs next to each other with 5 pixel horizontal spacing
   int xpos = area.X() , ypos = area.Y() , hspace = 5;
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      xpos = area.X();
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         int th = t->Area().H();
         t->SetDrawPos(xpos , ypos + (maxheight - th));
         xpos += t->Area().W();
         xpos += hspace;
      }
      ypos += maxheight;
   }


   // position all GUIs underneath the last row of tabs
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         WidgetHandler& wh = *(t->GetGui());
         wh.SetArea(area.X() , ypos , guiw , guih);
         wh.SetRedrawAllFlag();
      }
   }
   
   int totalheight = guih + tab_rows.size()*maxheight;
   WidgetBase::SetDrawDimensions(area.W() , totalheight);
   
   SetBgRedrawFlag();
}



int TabGroup::NumTabs() {
   int size = 0;
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      size += v.size();
   }
   return size;
}



bool TabGroup::HasTab(Tab* t) {
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         if (v[i] == t) {return true;}
      }
   }
   return false;
}


/* TODO : OLD, no longer necessary due to HasGui() and GetGui()
void TabGroup::FocusStart() {
   if (active_tab) {
      WidgetHandler& whl = *(active_tab->GetGui());
      whl.FocusStart();
   }
}



WidgetBase* TabGroup::NextFocus() {
   if (active_tab) {
      return active_tab->GetGui()->NextFocus();
   }
   return 0;
}



WidgetBase* TabGroup::PreviousFocus() {
   if (active_tab) {
      return active_tab->GetGui()->PreviousFocus();
   }
   return 0;
}
*/


bool TabGroup::HasWidget(WidgetBase* widget) {
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         if (t == widget) {return true;}
         WidgetHandler& wh = *(t->GetGui());
         if (wh.HasWidget(widget)) {return true;}
      }
   }
   return false;
}



TabGroup::TabGroup(string name , int gui_width , int gui_height , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      tab_rows(),
      active_tab(0),
      guiw(gui_width),
      guih(gui_height)
{}



WidgetMsg TabGroup::Update (double tsec) {
   if (active_tab) {
      return active_tab->Update(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg TabGroup::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (flags & ENABLED) {
      // check each tab for LMB click to activate tab
      if (input_mouse_press(LMB)) {
         for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
            vector<Tab*>& v = *it;
            for (UINT i = 0 ; i < v.size() ; ++i) {
               Tab* t = v[i];
               Rectangle r = t->Area();
               if (r.Contains(msx,msy)) {
                  SetActiveTab(t);
                  retmsg |= DIALOG_REDRAW_ME | DIALOG_INPUT_USED;
                  return WidgetMsg(this , TOPIC_DIALOG , retmsg);
               }
            }
         }
      }
      
      if (active_tab) {
         return active_tab->CheckInputs(msx,msy);
/*
         WidgetMsg ret = active_tab->CheckInputs(msx,msy);
         if (active_tab->Flags() & NEEDS_BG_REDRAW) {
            SetBgRedrawFlag();
         }
         return ret;
*/
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void TabGroup::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
         vector<Tab*>& v = *it;
         for (UINT i = 0 ; i < v.size() ; ++i) {
            Tab* t = v[i];
            t->DisplayOn(bmp,x,y);
         }
      }
      // fgcol
      if (active_tab) {
         int color = (*wcols)[FGCOL];
         Rectangle r = area;
         r.MoveBy(x,y);
         Rectangle tab = active_tab->Area();
         tab.MoveBy(x,y);
         // outline tab horizontally
         hline(bmp , r.X()     , tab.BRY() , tab.X()   , color);
         hline(bmp , tab.BRX() , tab.BRY() , r.BRX()   , color);
         hline(bmp , tab.X()   , tab.Y()   , tab.BRX() , color);
         vline(bmp , tab.X()   , tab.Y()   , tab.BRY() , color);
         vline(bmp , tab.BRX() , tab.Y()   , tab.BRY() , color);
         // outline bottom of area
         vline(bmp , r.X()     , tab.BRY() , r.BRY()   , color);
         vline(bmp , r.BRX()   , tab.BRY() , r.BRY()   , color);
         hline(bmp , r.X()     , r.BRY()   , r.BRX()   , color);
      }
   }
   ClearRedrawFlag();
}



void TabGroup::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         t->SetWidgetColors(colors);
      }
   }
}



void TabGroup::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         t->UseWidgetColorset(colorset);
      }
   }
}



void TabGroup::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         t->SetWidgetColors(wc);
      }
   }
}



void TabGroup::SetDrawTarget(BITMAP* surface) {
   WidgetBase::SetDrawTarget(surface);
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         t->SetDrawTarget(surface);
      }
   }
}



void TabGroup::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
}



void TabGroup::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width , area.H());
}



void TabGroup::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetDrawPos(xpos,ypos);
   WidgetBase::SetDrawDimensions(width , area.H());
}



void TabGroup::SetArea(const Rectangle& r) {
   WidgetBase::SetDrawPos(r.X() , r.Y());
   WidgetBase::SetDrawDimensions(r.W() , area.H());
}



void TabGroup::SetFocusDrawType(FOCUS_DRAW_TYPE type) {
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (UINT i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         WidgetHandler* wh = t->GetGui();
         wh->SetFocusDrawType(type , true);
      }
   }
}



int TabGroup::AbsMinWidth() {
   int maxwidth = 0;
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& vtab = *it;
      int width = 0;
      for (unsigned int i = 0 ; i < vtab.size() ; ++i) {
         Tab* t = vtab[i];
         width += t->Area().W();
      }
      if (width > maxwidth) {maxwidth = width;}
   }
   return maxwidth;
}



int TabGroup::AbsMinHeight() {
   int totalrowheight = 0;
   int maxheight = 0;
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& vtab = *it;
      int height = 0;
      maxheight = 0;
      for (unsigned int i = 0 ; i < vtab.size() ; ++i) {
         Tab* t = vtab[i];
         height = t->Area().H();
         if (height > maxheight) {maxheight = height;}
      }
      totalrowheight += maxheight;
   }
   return totalrowheight;
}



bool TabGroup::HasGui() {
   if (active_tab) {
      return true;
   }
   return false;
}



WidgetHandler* TabGroup::GetGui() {
   if (active_tab) {
      return active_tab->GetGui();
   }
   return 0;
}



void TabGroup::AddTab(Tab* tab , unsigned int row) {
   if (tab && !HasTab(tab)) {
      if (row > tab_rows.size()) {row = tab_rows.size();}
      if (row == tab_rows.size()) {
         vector<Tab*> v;
         v.push_back(tab);
         tab_rows.push_back(v);
      } else {
         UINT index = 0;
         for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
            if (index == row) {
               vector<Tab*>& v = *it;
               v.push_back(tab);
            }
            ++index;
         }
      }
      ResetTabPositions();
      tab->SetParent(this);
      tab->UseWidgetColorset(wcols);
      if (NumTabs() == 1) {
         SetActiveTab(tab);
      } else {
         tab->ActivateTab(false);
      }
   }
}



void TabGroup::AddTab(Tab& tab , unsigned int row) {
   AddTab(&tab , row);
}



TabGroup& TabGroup::operator<<(Tab* tab) {
   *this << *tab;
   return *this;
}



TabGroup& TabGroup::operator<<(Tab& tab) {
   AddTab(tab);
   return *this;
}



void TabGroup::SetActiveTab(Tab* t) {
   if (t && HasTab(t) && (t != active_tab)) {
      if (active_tab) {
         active_tab->ActivateTab(false);
      }
      active_tab = t;
      t->ActivateTab(true);
      WidgetHandler& wh = *(t->GetGui());
      GiveWidgetFocus(wh.CurrentFocus());
      SetBgRedrawFlag();
   }
}



void TabGroup::SetFont(FONT* textfont) {
   ASSERT(textfont);
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& v = *it;
      for (unsigned int i = 0 ; i < v.size() ; ++i) {
         Tab* t = v[i];
         t->SetFont(textfont);
      }
   }
   ResetTabPositions();
}



void TabGroup::SetGuiFocusDrawType(FOCUS_DRAW_TYPE draw_type , bool for_all_guis) {
   for (TRSIT it = tab_rows.begin() ; it != tab_rows.end() ; ++it) {
      vector<Tab*>& tabvec = *it;
      for (unsigned int i = 0 ; i < tabvec.size() ; ++i) {
         Tab* t = tabvec[i];
         WidgetHandler& whref = *(t->GetGui());
         whref.SetFocusDrawType(draw_type , for_all_guis);
      }
   }
}












