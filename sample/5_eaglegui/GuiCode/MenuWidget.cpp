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


#include "Eagle/GuiCode/MenuWidget.hpp"

using std::vector;
using std::string;
using std::endl;
using std::ostream;



const unsigned int TOPIC_MENU_ITEM = NextFreeTopicId();
const unsigned int TOPIC_MENU = NextFreeTopicId();

const char* MenuItem::arrow_text = ">>";



int DefaultMenuItemPadding() {
   static const int DEFAULT_PADDING = 4;
   return DEFAULT_PADDING;
}



void MenuItem::CalcDimensions() {
   icon_cw = 2*padding + icon.W();
   label_cw = 2*padding + gui_strlen(label.c_str());
   key_text_cw = 2*padding + text_length(text_font , key_text.c_str());
   arrow_text_cw = 2*padding + text_length(text_font , arrow_text);
   int icon_h = 2*padding + icon.H();
   int text_h = 2*padding + text_height(text_font);
   item_rh = (icon_h>text_h)?icon_h:text_h;
   SetDrawDimensions(icon_cw + label_cw + key_text_cw + arrow_text_cw, item_rh);
}


/*
MenuItem::MenuItem(string name) :
      WidgetBase(name),
      icon(),
      icon_disabled(),
      input_key(input_key_press(KEY_NONE)),
      label(""),
      key_text(""),
      sub_menu(0),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      show_icon(false)
{
   SetMenu(0);
}
*/


MenuItem::MenuItem(string name , InputGroup key_input , FONT* textfont , string item_label , string info_text , Menu* menu,
                   const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      icon(),
      icon_disabled(),
      input_key(key_input),
      text_font(0),
      label(item_label),
      key_text(info_text),
      sub_menu(menu),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      show_icon(false),
      toggle_icon_on_click(false)
{
   SetFont(textfont);
   SetPadding(DefaultMenuItemPadding());
   SetMenu(menu);
}




MenuItem::MenuItem(string name , BITMAP* icon_pic , BITMAP* disabled_icon_pic , BMP_DRAW_TYPE bmp_draw_type , bool toggle_icon,
         InputGroup key_input , FONT* textfont , string item_label , string info_text ,
         Menu* menu , int padding_size , bool display_icon,
         const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      icon(icon_pic , MEMORY , bmp_draw_type , true , 0),
      icon_disabled(disabled_icon_pic , MEMORY , bmp_draw_type , true , 0),
      input_key(key_input),
      text_font(0),
      label(item_label),
      key_text(info_text),
      sub_menu(menu),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      show_icon(display_icon),
      toggle_icon_on_click(toggle_icon)
{
   SetFont(textfont);
   SetPadding(padding_size);
   SetMenu(menu);
}



MenuItem::MenuItem(string name , const char* icon_file , const char* disabled_icon_file , BMP_DRAW_TYPE bmp_draw_type , bool toggle_icon,
                  InputGroup key_input , FONT* textfont , string item_label , string info_text ,
                  Menu* menu , int padding_size , bool display_icon,
                  const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      icon(icon_file , NULL , MEMORY , bmp_draw_type , 0),
      icon_disabled(disabled_icon_file , NULL , MEMORY , bmp_draw_type , 0),
      input_key(key_input),
      text_font(0),
      label(item_label),
      key_text(info_text),
      sub_menu(menu),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      show_icon(display_icon),
      toggle_icon_on_click(toggle_icon)
{   
   SetFont(textfont);
   SetPadding(padding_size);
   SetMenu(menu);
}



WidgetMsg MenuItem::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg MenuItem::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (input_key || (input_mouse_press(LMB) && area.Contains(msx,msy))) {
      QueueUserMessage(this , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
      retmsg |= DIALOG_INPUT_USED;
      if (toggle_icon_on_click) {
         show_icon = !show_icon;
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void MenuItem::DisplayOn(BITMAP* bmp , int x , int y) {
   FormattedDisplayOn(bmp , x , y , icon_cw , label_cw , key_text_cw , item_rh);
}



void MenuItem::FormattedDisplayOn(BITMAP* bmp , int x , int y ,
                                    int icon_colw , int label_colw , int key_text_colw , int item_rowh) {
   if (flags & VISIBLE) {
      int tlx = area.X() + x;
      int icon_x = tlx;
      int label_x = icon_x + icon_colw;
      int key_text_x = label_x + label_colw;
      int arrow_text_x = key_text_x + key_text_colw;
      icon_x += (icon_colw - icon.W())/2;
      label_x += padding;
      key_text_x += padding;
      arrow_text_x += padding;
      int tly = area.Y() + y;
      int icon_y = tly + (item_rowh - icon.H())/2;
      int label_y = tly + (item_rowh - text_height(text_font))/2;
      int key_text_y = label_y;
      int arrow_text_y = label_y;
      
      Rectangle r(tlx , tly , icon_colw + label_colw + key_text_colw + arrow_text_cw , item_rowh);
      Rectangle text_area(r.X() + icon_colw , r.Y() , r.W() - icon_colw , r.H());
      Rectangle old_clip = GetClippingRectangle(bmp);

      r.SetClipRect(bmp);
      r.Fill(bmp , (*wcols)[FGCOL]);
      FONT* old_font = font;
      font = text_font;
      if (WidgetBase::Flags() & ENABLED) {
         if (show_icon) {
            icon.DrawTo(bmp , icon_x , icon_y);
         }
         if (WidgetBase::Flags() & HOVER) {
            text_area.Fill(bmp , (*wcols)[MGCOL]);
         }
         gui_textout_ex(bmp , label.c_str() , label_x , label_y , (*wcols)[TXTCOL] , -1 , 0);
         textout_ex(bmp , text_font , key_text.c_str() , key_text_x , key_text_y , (*wcols)[TXTCOL] , -1);
         if (sub_menu) {
            textout_ex(bmp , text_font , arrow_text , arrow_text_x , arrow_text_y , (*wcols)[TXTCOL] , -1);
            if (sub_menu->MenuOpen()) {
               sub_menu->DisplayOn(bmp , x , y);
            }
         }
      } else {
         if (show_icon) {
            icon_disabled.DrawTo(bmp , icon_x , icon_y);
         }
         gui_textout_ex(bmp , label.c_str() , label_x , label_y , (*wcols)[MGCOL] , -1 , 0);
         textout_ex(bmp , text_font , key_text.c_str() , key_text_x , key_text_y , (*wcols)[MGCOL] , -1);
         if (sub_menu) {
            textout_ex(bmp , text_font , arrow_text , arrow_text_x , arrow_text_y , (*wcols)[MGCOL] , -1);
         }
      }
      font = old_font;
      old_clip.SetClipRect(bmp);
   }
}



void MenuItem::SetFormatting(int icon_colw , int label_colw , int key_text_colw , int arrow_text_colw , int item_rowh) {
   if (icon_colw < 0) {icon_colw = 0;}
   if (label_colw < 0) {label_colw = 0;}
   if (key_text_colw < 0) {key_text_colw = 0;}
   if (item_rowh < 0) {item_rowh = 0;}
   icon_cw = icon_colw;
   label_cw = label_colw;
   key_text_cw = key_text_colw;
   arrow_text_cw = arrow_text_colw;
   item_rh = item_rowh;
   WidgetBase::SetDrawDimensions(icon_cw + label_cw + key_text_cw + arrow_text_cw, item_rh);
   
   padding = 0;
}



void MenuItem::ShrinkWrap() {
   padding = 1;
   CalcDimensions();
}



void MenuItem::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      wcols = colorset;
      if (sub_menu) {
         sub_menu->UseWidgetColorset(colorset);
      }
   }
}



void MenuItem::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   if (sub_menu) {
      sub_menu->SetDrawPos(area.X() + area.W() , area.Y());
   }
}



void MenuItem::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width , height);
   if (sub_menu) {
      sub_menu->SetDrawPos(area.X() + area.W() , area.Y());
   }
}



void MenuItem::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   if (sub_menu) {
      sub_menu->SetDrawPos(area.X() + area.W() , area.Y());
   }
}



void MenuItem::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   if (sub_menu) {
      sub_menu->SetDrawPos(area.X() + area.W() , area.Y());
   }
}



void MenuItem::SetRedrawFlag() {
   WidgetBase::SetRedrawFlag();
   Menu* m = dynamic_cast<Menu*>(parent);
   if (m) {
      m->SetRedrawFlag();
   }
}



void MenuItem::SetPadding(int pad) {
   if (pad < 0) {pad = 0;}
   padding = pad;
   CalcDimensions();
}



void MenuItem::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   CalcDimensions();
   Menu* pm = dynamic_cast<Menu*>(parent);
   if (parent && pm) {
      pm->MenuItemsChanged();
   }
}



void MenuItem::SetIconVisibility(bool visible) {
   show_icon = visible;
   SetRedrawFlag();
}



void MenuItem::SetIconToggleOnClick(bool toggle_icon) {
   toggle_icon_on_click = toggle_icon;
}



void MenuItem::SetMenu(Menu* m) {
   if (sub_menu) {
      sub_menu->SetParent(NULL);
      sub_menu->SetDrawTarget(NULL);
   }
   sub_menu = m;
   if (m) {
      m->SetParent(this);
      m->SetDrawTarget(WidgetBase::dest);
      m->UseWidgetColorset(WidgetBase::wcols);
      m->SetDrawPos(area.X() + area.W() , area.Y());
   }
}



bool MenuItem::LoadIcons(const char* normal_icon_file , const char* disabled_icon_file , BMP_DRAW_TYPE bmp_draw_type) {
//   bool LoadImage(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   bool success = true;
   if (!normal_icon_file) {
      icon.ClearData();
   } else if (!icon.LoadImage(normal_icon_file , NULL , MEMORY , bmp_draw_type)) {
      success = false;
   }
   if (!disabled_icon_file) {
      icon_disabled.ClearData();
   } else if (!icon_disabled.LoadImage(disabled_icon_file , NULL , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



bool MenuItem::LoadNormalIcon(const char* icon_file , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (!icon_file) {
      icon.ClearData();
   } else if (!icon.LoadImage(icon_file , NULL , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



bool MenuItem::LoadDisabledIcon(const char* icon_file , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (!icon_file) {
      icon_disabled.ClearData();
   } else if (!icon_disabled.LoadImage(icon_file , NULL , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



bool MenuItem::SetIcons(BITMAP* normal_icon , BITMAP* disabled_icon , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (!normal_icon) {
      icon.ClearData();
   } else if (!icon.Clone(normal_icon , MEMORY , bmp_draw_type)) {
      success = false;
   }
   if (!disabled_icon) {
      icon_disabled.ClearData();
   } else if (!icon_disabled.Clone(disabled_icon , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



bool MenuItem::SetNormalIcon(BITMAP* icon_bmp , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (!icon_bmp) {
      icon.ClearData();
   } else if (!icon.Clone(icon_bmp , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



bool MenuItem::SetDisabledIcon(BITMAP* icon_bmp , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (!icon_bmp) {
      icon_disabled.ClearData();
   } else if (!icon_disabled.Clone(icon_bmp , MEMORY , bmp_draw_type)) {
      success = false;
   }
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



void MenuItem::ClearIcons() {
   icon.ClearData();
   icon_disabled.ClearData();
}



bool MenuItem::ReloadIcons() {
   bool success = true;
   if (!icon.Reload()) {success = false;}
   if (!icon_disabled.Reload()) {success = false;}
   CalcDimensions();
   SetRedrawFlag();
   Menu* pmenu = dynamic_cast<Menu*>(parent);
   if (parent && pmenu) {
      pmenu->MenuItemsChanged();
   }
   return success;
}



void MenuItem::CascadeCloseMenu() {
   Menu* m = dynamic_cast<Menu*>(WidgetBase::parent);
   if (m) {
      m->CascadeCloseMenu();
   }
}



ostream& operator<<(ostream& os , const MenuItem& m) {
   os << "MenuItem (" << &m << ")" << endl;
   os << "   Label      = " << m.label.c_str() << endl;
   os << "   Key Text   = " << m.key_text.c_str() << endl;
   os << "   Arrow Text = " << m.arrow_text << endl;
   os << "   Sub Menu Ptr (" << m.sub_menu << ")";
   if (m.sub_menu) {
      os << "   {" << endl;
      os << *(m.sub_menu);
      os << "   }" << endl;
   } else {
      os << endl;
   }
   os << "   Icon Column Width     = " << m.icon_cw << endl;
   os << "   Label Column Width    = " << m.label_cw << endl;
   os << "   Key Text Column Width = " << m.key_text_cw << endl;
   os << "   Item Row Height       = " << m.item_rh << endl;
   os << "   Padding               = " << m.padding << endl;
   os << "   Show Icon             = (" << m.show_icon << ")" << endl;
   return os;
}






/** ----------------------  Menu class  ----------------------------*/





const int Menu::separator_height = 10;


void Menu::ReadjustSize() {
   int max_icon_cw = 0;
   int max_label_cw = 0;
   int max_key_text_cw = 0;
   int max_arrow_text_cw = 0;
   int max_item_rh = 0;

   for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
      for (unsigned int j = 0 ; j < item_groups[i].size() ; ++j) {
         MenuItem* item = item_groups[i][j];
         item->SetPadding(padding);
         if (item->IconColW()      > max_icon_cw      ) {max_icon_cw       = item->IconColW();}
         if (item->LabelColW()     > max_label_cw     ) {max_label_cw      = item->LabelColW();}
         if (item->KeyTextColW()   > max_key_text_cw  ) {max_key_text_cw   = item->KeyTextColW();}
         if (item->ArrowTextColW() > max_arrow_text_cw) {max_arrow_text_cw = item->ArrowTextColW();}
         if (item->ItemRowH()      > max_item_rh      ) {max_item_rh       = item->ItemRowH();}
      }
   }
   
   icon_cw = max_icon_cw;
   label_cw = max_label_cw;
   key_text_cw = max_key_text_cw;
   arrow_text_cw = max_arrow_text_cw;
   item_rh = max_item_rh;
   for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
      for (unsigned int j = 0 ; j < item_groups[i].size() ; ++j) {
         MenuItem* item = item_groups[i][j];
         item->SetFormatting(icon_cw , label_cw , key_text_cw , arrow_text_cw , item_rh);
      }
   }
   int num_separators = (int)item_groups.size() - 1;
   if (num_separators < 0) {num_separators = 0;}
   SetDrawDimensions(icon_cw + label_cw + key_text_cw + arrow_text_cw + 2,
                     num_items*item_rh + num_separators*separator_height + 2);
}



void Menu::ResetItemPositions() {
   int item_x = area.X() + 1;
   int item_y = area.Y() + 1;
   for (UINT i = 0 ; i < item_groups.size() ; ++i) {
      for (UINT j = 0 ; j < item_groups[i].size() ; ++j) {
         MenuItem* m = item_groups[i][j];
         m->SetDrawPos(item_x , item_y);
         item_y += item_rh;
      }
      item_y += separator_height;
   }
}



MenuItem* Menu::GetItemPointer(int num) {
   if (num < 0) {return 0;}
   for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
      if (num < (int)item_groups[i].size()) {
         return item_groups[i][num];
      }
      num -= item_groups[i].size();
   }
   return 0;
}



/// 0 based item number , -1 indicates not found
int Menu::GetItemNum(MenuItem* m) {
   int num = 0;
   for (UINT i = 0 ; i < item_groups.size() ; ++i) {
      for (UINT j = 0 ; j < item_groups[i].size() ; ++j) {
         if (item_groups[i][j] == m) {
            return num;
         }
         ++num;
      }
   }
///   return num_items;
   return -1;// So that keyboard navigation will start from the top of the menu instead of the bottom
}



int Menu::CountNumItems() {
   int count = 0;
   for (UINT i = 0 ; i < item_groups.size() ; ++i) {
      count += item_groups[i].size();
   }
   return count;
}



Menu::Menu(string name) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , DEFAULT_OVERLAP_FLAGS),
      item_groups(),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      item_highlight(-1),
      num_items(0),
      menu_open(false),
      menu_just_opened(false),
      open_sub_menu(0),
      item_hover(0),
      old_msx(0),
      old_msy(0)
{}



Menu::Menu(string name , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      item_groups(),
      icon_cw(0),
      label_cw(0),
      key_text_cw(0),
      arrow_text_cw(0),
      item_rh(0),
      padding(0),
      item_highlight(-1),
      num_items(0),
      menu_open(false),
      menu_just_opened(false),
      open_sub_menu(0),
      item_hover(0),
      old_msx(0),
      old_msy(0)
{}




WidgetMsg Menu::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg Menu::CheckInputs(int msx , int msy) {

   /// Redraw when hover item changes or when menu loses or gains hover
   /// Redraw all when menu closes
   /// Menu should have focus when open, how to ask the parent to give the widget focus?
   
   /// What about having Menu::CheckInputs call MenuItem::CheckInputs on each menu item widget?
   /// What about checking each menu item's area for containing msx and msy instead?

   int retmsg = DIALOG_OKAY;
   bool input_used = false;
   MenuItem* new_hover = 0;
   const int d_msx = msx - old_msx;
   const int d_msy = msy - old_msy;
   const bool mouse_move = (d_msx || d_msy);
   

/// TODO : Working here, disallow disabled menu items from being activated

   if (WidgetBase::Flags() & ENABLED) {
      if (menu_open) {
         /// Check keyboard based shortcuts to activate menu items
         if (!open_sub_menu) {// only if this is the bottom menu
            for (UINT i = 0 ; i < item_groups.size() ; ++i) {
               for (UINT j = 0 ; j < item_groups[i].size() ; ++j) {
                  MenuItem* mitem = item_groups[i][j];
                  bool enabled = mitem->Flags() & ENABLED;
                  if (mitem->InputKey() && enabled) {
                     input_used = true;
                     if (mitem->HasMenu()) {
                        /// Keyboard shortcut for a menu item with a menu
                        Menu* sub_menu = mitem->SubMenu();
                        sub_menu->OpenMenu();
                        open_sub_menu = sub_menu;
                        new_hover = 0;
                        item_highlight = -1;
                        retmsg |= DIALOG_REDRAW_ME;
                        SetRedrawFlag();
                     } else {
                        /// Keyboard shortcut for a menu item without a menu
                        CloseSubMenu();
                        retmsg |= DIALOG_REDRAW_ALL;
                        SetRedrawFlag();
                        QueueUserMessage(mitem , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
                        if (mitem->ToggleIconOnClick()) {
                           mitem->SetIconVisibility(!mitem->ShowIcon());
                        }
                        CascadeCloseMenu();
                     }
                  }
               }
            }
         }
         /// Find mouse hovered item
         /// Check mouse click on hovered item or background
         /// If the mouse moves & menu not just opened, check for new mouse hover
         /// else check for :
         /// keyboard navigation hover
         
         
         /// Find whether a widget is being hovered over and which item_num it is.
         bool widget_hover = false;
         int item_num = 0;
         if (area.Contains(msx,msy)) {
            for (UINT i = 0 ; i < item_groups.size() ; ++i) {
               for (UINT j = 0 ; j < item_groups[i].size() ; ++j) {
                  Rectangle item_area = item_groups[i][j]->Area();
                  if (item_area.Contains(msx,msy)) {
                     widget_hover = true;
                     i = item_groups.size();
                     break;
                  }
                  ++item_num;
               }
            }
         }
         
         MenuItem* m = 0;
         if (widget_hover) {
            m = GetItemPointer(item_num);
         }
         
         if (input_mouse_press(LMB)) {
            if (area.Contains(msx,msy)) {
               if (m) {
                  input_used = true;
                  /// Mouse click on menu item
                  bool enabled = m->Flags() & ENABLED;
                  if (!(m->HasMenu()) && enabled) {
                     CloseSubMenu();
                     retmsg |= DIALOG_REDRAW_ALL;
                     SetRedrawFlag();
                     QueueUserMessage(m , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
                     if (m->ToggleIconOnClick()) {
                        m->SetIconVisibility(!m->ShowIcon());
                     }
                     CascadeCloseMenu();
                  }
               }
            } else {/// Mouse click not over menu area
               /// Close if there is no open sub menu and not part of a menu bar or menu item
               if (!open_sub_menu) {
                  MenuBarItem* mbi = dynamic_cast<MenuBarItem*>(parent);
                  MenuBar* menubar = 0;
                  if (mbi) {
                     menubar = dynamic_cast<MenuBar*>(mbi->Parent());
                  }
                  MenuItem* menuitem = dynamic_cast<MenuItem*>(parent);
                  if (!(menubar || menuitem)) {
                     CloseMenu();
                     retmsg |= (DIALOG_REDRAW_ALL | DIALOG_INPUT_USED);
                  }
               }
            }
         }

         if (mouse_move && !menu_just_opened) {
            /// Mouse based menu navigation active
            if (area.Contains(msx,msy)) {
               
               /// Handle hover states of menu items
               if (widget_hover) {
                  /// set new hover for menu item
                  new_hover = m;
                  
                  bool enabled = m->Flags() & ENABLED;
                  Menu* new_menu = (enabled?(m->SubMenu()):0);
                  /// Exchange open menus if different
                  if (m && (open_sub_menu != new_menu)) {
                     CloseSubMenu();
                     retmsg |= DIALOG_REDRAW_ALL;
                     SetRedrawFlag();
                     open_sub_menu = new_menu;
                     if (open_sub_menu) {
                        open_sub_menu->OpenMenu();
                     }
                  }
               }
            }
         } else {
            /// Keyboard based menu navigation active
            if (!open_sub_menu) {
               if (input_key_press(KEY_UP)) {
                  if (item_highlight > 0) {
                     --item_highlight;
                     input_used = true;
                     retmsg |= DIALOG_REDRAW_ME;
                  }
               }
               if (input_key_press(KEY_DOWN)) {
                  if (item_highlight < num_items - 1) {
                     ++item_highlight;
                     input_used = true;
                     retmsg |= DIALOG_REDRAW_ME;
                  }
               }
            }
            new_hover = GetItemPointer(item_highlight);
            m = new_hover;
            bool enabled = false;
            if (m) {enabled = m->Flags() & ENABLED;}
            if (input_key_press(KEY_ENTER) && enabled) {
               if (m && (!m->HasMenu())) {
                  QueueUserMessage(m , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
                  input_used = true;
                  retmsg |= DIALOG_REDRAW_ALL;
                  CascadeCloseMenu();
                  open_sub_menu = 0;
               }
            }
            if (m && m->HasMenu()) {
               if (input_key_press(KEY_RIGHT) && enabled) {
                  Menu* sub_menu = m->SubMenu();
                  sub_menu->OpenMenu();
                  open_sub_menu = sub_menu;
                  new_hover = 0;
                  input_used = true;
                  retmsg |= DIALOG_REDRAW_ME;
               }
            }
            if (input_key_press(KEY_LEFT)) {
               MenuItem* super_item = dynamic_cast<MenuItem*>(WidgetBase::parent);
               if (super_item) {
                  CloseMenu();
                  Menu* super_menu = dynamic_cast<Menu*>(super_item->Parent());
                  if (super_menu) {
                     super_menu->HighlightItem(super_item);
                     super_menu->CloseSubMenu();
                  }
                  input_used = true;
                  retmsg |= DIALOG_REDRAW_ALL;
               }
            }
         }

         if (new_hover != item_hover) {
            if (item_hover) {
               item_hover->SetHoverState(false);
            }
            if (new_hover) {
               new_hover->SetHoverState(true);
            }
            retmsg |= DIALOG_REDRAW_ME;
         }
         item_hover = new_hover;
         item_highlight = GetItemNum(item_hover);

         if (open_sub_menu) {
            WidgetMsg subret = open_sub_menu->CheckInputs(msx,msy);
            retmsg |= subret.Messages();
         }
      }
      
      old_msx = msx;
      old_msy = msy;
      menu_just_opened = false;
   }
   if (input_used) {
      retmsg |= DIALOG_INPUT_USED;
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}




void Menu::DisplayOn(BITMAP* bmp , int x , int y) {
   if (menu_open && WidgetBase::Flags() & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[FGCOL]);
      int item_x = r.X() + 1;
      int item_y = r.Y() + 1;
      for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
         for (unsigned int j = 0 ; j < item_groups[i].size() ; ++j) {
            MenuItem* mitem = item_groups[i][j];
            mitem->DisplayOn(bmp , x , y);
            item_y += item_rh;
         }
         /** TODO : Improve group spacing and separator drawing */
         if (i != item_groups.size() - 1) {
            int separator_y = item_y + separator_height/2 - 1;
            hline(bmp , item_x , separator_y , item_x + r.W() - 2 , (*wcols)[MGCOL]);
            hline(bmp , item_x , separator_y + 1, item_x + r.W() - 2 , (*wcols)[MGCOL]);
            item_y += separator_height;
         }
      }
      r.Draw(bmp , (*wcols)[HLCOL]);
///      if (open_sub_menu) {
///         open_sub_menu->DisplayOn(bmp , x , y);
///      }
   }
}



void Menu::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      for (UINT i = 0 ; i < item_groups.size() ; ++i) {
         for (UINT j = 0 ; j < item_groups[i].size() ; ++j) {
            MenuItem* m = item_groups[i][j];
            m->UseWidgetColorset(colorset);
         }
      }
      wcols = colorset;
   }
}



void Menu::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetItemPositions();
}



//void SetDrawDimensions(int width , int height) {}



void Menu::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetItemPositions();
}



void Menu::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetItemPositions();
}


/* Hmmm... WidgetBase::SetRedrawFlag already does this...
void Menu::SetRedrawFlag() {
   WidgetBase::SetRedrawFlag();
   MenuBarItem* mbi = dynamic_cast<MenuBarItem*>(parent);
   if (mbi) {
      mbi->SetRedrawFlag();
   }
   MenuItem* mi = dynamic_cast<MenuItem*>(parent);
   if (mi) {
      mi->SetRedrawFlag();
   }
}
*/


void Menu::SetFont(FONT* textfont) {
   ASSERT(textfont);
   for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
      for (unsigned int j = 0 ; j < item_groups[i].size() ; ++i) {
         MenuItem* mitem = item_groups[i][j];
         mitem->SetFont(textfont);
      }
   }
   ReadjustSize();
   ResetItemPositions();
}



bool Menu::IsMouseOver(int msx , int msy) const {
   return Contains(msx,msy);
}



void Menu::OpenMenu() {
   if (parent) {
      parent->GiveWidgetFocus(this);
   }
   WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_MENU , MENU_OPENED));
   menu_open = true;
   menu_just_opened = true;
   HighlightItem(int(0));
}



void Menu::CloseMenu() {
   CloseSubMenu();
   MenuBarItem* mbitem = dynamic_cast<MenuBarItem*>(WidgetBase::parent);
   if (mbitem) {
      mbitem->MenuWasClosed();
   }
   menu_open = false;
}



void Menu::CascadeCloseMenu() {
   CloseMenu();
   MenuItem* mitem = dynamic_cast<MenuItem*>(WidgetBase::parent);
   if (mitem) {
      mitem->CascadeCloseMenu();
   }
}



void Menu::CloseSubMenu() {
   if (open_sub_menu) {
      open_sub_menu->CloseMenu();
      open_sub_menu = 0;
   }
}



void Menu::HighlightItem(int item_number) {
//   if (item_number >= num_items) {item_number = num_items;}
//   if (item_number < 0) {item_number = 0;}
   MenuItem* m = GetItemPointer(item_number);
   if (item_hover != m) {
      if (item_hover) {
         item_hover->SetHoverState(false);
      }
      if (m) {
         m->SetHoverState(true);
      }
      item_hover = m;
      item_highlight = GetItemNum(item_hover);
   }
}



void Menu::HighlightItem(MenuItem* item) {
   UINT item_num = GetItemNum(item);
   MenuItem* m = GetItemPointer(item_num);
   if (item_hover != m) {
      if (item_hover) {
         item_hover->SetHoverState(false);
      }
      if (m) {
         m->SetHoverState(true);
      }
      item_hover = m;
      item_highlight = GetItemNum(item_hover);
   }
}



void Menu::AddItem(MenuItem* item , UINT group) {
   if (item) {
      
      if (group >= item_groups.size()) {
         group = item_groups.size();
         vector<MenuItem*> new_v;
         item_groups.push_back(new_v);
      }
      
      item->SetParent(this);
      item->SetDrawTarget(WidgetBase::dest);
      item_groups[group].push_back(item);
      ++num_items;
      ReadjustSize();
      ResetItemPositions();
      item->UseWidgetColorset(WidgetBase::wcols);
      
      SetRedrawFlag();
   }
}



Menu& Menu::operator<<(MenuItem* item) {
   AddItem(item);
   return *this;
}



void Menu::RemoveItem(MenuItem* item) {
   if (item) {
      typedef vector<MenuItem*>::iterator MIT;
      for (unsigned int i = 0 ; i < item_groups.size() ; ++i) {
         for (MIT it = item_groups[i].begin() ; it != item_groups[i].end() ; ++it) {
            if (*it == item) {
               item_groups[i].erase(it);
               --num_items;
               ReadjustSize();
               ResetItemPositions();
               i = item_groups.size();
               break;
            }
         }
      }
   }
}



void Menu::ClearAllItems() {
   item_groups.clear();
   num_items = 0;
   ReadjustSize();
   ResetItemPositions();
}



void Menu::SetPadding(int pad) {
   if (pad < 0) {pad = 0;}
   padding = pad;
   ReadjustSize();
   ResetItemPositions();
}



void Menu::MenuItemsChanged() {
   ReadjustSize();
   ResetItemPositions();
}



bool Menu::Contains(int x , int y) const {
   if (area.Contains(x,y)) {return true;}
   if (open_sub_menu) {
      if (open_sub_menu->Contains(x,y)) {return true;}
   }
   return false;
}



ostream& operator<<(ostream& os , const Menu& m) {
   using std::endl;
   os << "Printing Menu contents... (" << &m << ")" << endl;
   for (UINT i = 0 ; i < m.item_groups.size() ; ++i) {
      os << "   Group #" << i << "   ";
      for (UINT j = 0 ; j < m.item_groups[i].size() ; ++j) {
         os << " - " << m.item_groups[i][j];
      }
      os << endl;
   }
   os << "   Icon column width     : " << m.icon_cw << endl;
   os << "   Label column width    : " << m.label_cw << endl;
   os << "   Key Text column width : " << m.key_text_cw << endl;
   os << "   Item row height       : " << m.item_rh << endl;
   os << "   Padding size          : " << m.padding << endl;
   os << "   Item highlight        : " << m.item_highlight << endl;
   os << "   Number of items       : " << m.num_items << endl;
   os << "   Menu open             : (" << m.menu_open << ")" << endl;
   os << "   Open Sub Menu         : (" << m.open_sub_menu << ")" << endl;
   os << "   Item hover            : " << m.item_hover << endl;
   
   int n = 0;
   for (UINT i = 0 ; i < m.item_groups.size() ; ++i) {
      for (UINT j = 0 ; j < m.item_groups[i].size() ; ++j) {
         os << "Menu Item # " << n << endl;
         os << *(m.item_groups[i][j]) << endl;
         ++n;
      }
   }
   os << endl;
   return os;
}





/// ------------------------------    MenuBarItem class     -----------------------------------




void MenuBarItem::SetUpMenu() {
   if (item_menu) {
      item_menu->SetDrawPos(area.X() , area.Y() + area.H());
      item_menu->SetParent(this);
      item_menu->UseWidgetColorset(wcols);
   }
   
}


/*
MenuBarItem::MenuBarItem() :
      WidgetBase(),
      item_menu(0),
      button(),
      menu_open(false),
      text_font(0)
{
   SetFont(font);
   SetUpMenu();
}
*/


MenuBarItem::MenuBarItem(string name , FONT* textfont , string label_text , InputGroup input_key , Menu* menu,
                         const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      item_menu(menu),
      button(name + string("-button") , RECTANGLE_BTN , TOGGLE_BTN , font , label_text , input_key , position , wflags),
      menu_open(false),
      text_font(0)
{
   SetFont(textfont);
   SetUpMenu();
}



WidgetMsg MenuBarItem::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg MenuBarItem::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::Flags() & ENABLED) {
      if (menu_open) {
         WidgetMsg msg = item_menu->CheckInputs(msx,msy);
         retmsg |= msg.Messages();
      }
      if (button.InputKey() || (input_mouse_press(LMB) && area.Contains(msx,msy))) {
         menu_open = !menu_open;
         button.SetButtonUpState(!menu_open);
         if (menu_open) {
//            button.SetButtonUpState(false);
            item_menu->OpenMenu();
            retmsg |= DIALOG_REDRAW_ME;
         } else {
//            button.SetButtonUpState(true);
            item_menu->CloseMenu();
            retmsg |= DIALOG_REDRAW_ALL;
         }
         retmsg |= (DIALOG_INPUT_USED | DIALOG_TAKE_FOCUS);
         SetRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void MenuBarItem::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::Flags() & VISIBLE) {
      Rectangle r = button.Area();
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[SDCOL]);
      button.DisplayOn(bmp,x,y);
      if (menu_open) {
         item_menu->DisplayOn(bmp,x,y);
      }
   }
   ClearRedrawFlag();
}



void MenuBarItem::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      wcols = colorset;
      button.UseWidgetColorset(colorset);
      if (item_menu) {
         item_menu->UseWidgetColorset(wcols);
      }
   }
}



void MenuBarItem::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   button.SetDrawPos(xpos,ypos);
   if (item_menu) {
      item_menu->SetDrawPos(area.X() , area.Y() + area.H());
   }
}



void MenuBarItem::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   button.SetDrawPos(xpos,ypos);
   if (item_menu) {
      item_menu->SetDrawPos(area.X() , area.Y() + area.H());
   }
}



void MenuBarItem::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   button.SetDrawPos(r.X() , r.Y());
   if (item_menu) {
      item_menu->SetDrawPos(area.X() , area.Y() + area.H());
   }
}


/* Hmmm? WidgetBase::SetRedrawFlag already does this... No need to overload it.
void MenuBarItem::SetRedrawFlag() {
   WidgetBase::SetRedrawFlag();
   MenuBar* mbar = dynamic_cast<MenuBar*>(parent);
   if (mbar) {
      mbar->SetRedrawFlag();
   }
}
*/



bool MenuBarItem::IsMouseOver(int msx , int msy) const {
   return Contains(msx,msy);
}



void MenuBarItem::SetButtonUpState(bool button_up) {
   if (button_up != button.ButtonUpState()) {
      button.SetButtonUpState(button_up);
      if (button_up) {
         item_menu->CloseMenu();
      } else {
         item_menu->OpenMenu();
      }
      menu_open = !button_up;
      SetRedrawFlag();
   }
}



void MenuBarItem::SetMenuState(bool menu_is_open) {
   SetButtonUpState(!menu_is_open);
}



void MenuBarItem::MenuWasClosed() {
   button.SetButtonUpState(true);
   menu_open = false;
   SetRedrawFlag();
   MenuBar* menubar = dynamic_cast<MenuBar*>(parent);
   if (menubar) {
      menubar->MenuWasClosed();
   }
}



void MenuBarItem::SetButtonUpStateOnly(bool button_up) {
   button.SetButtonUpState(button_up);
///   item_menu->CloseMenu();
   menu_open = false;
   SetRedrawFlag();
}



bool MenuBarItem::Contains(int x , int y) const {
   if (area.Contains(x,y)) {return true;}
   if (menu_open) {
      if (item_menu->Contains(x,y)) {return true;}
   }
   return false;
}



void MenuBarItem::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   button.SetFont(textfont);
}





/// ----------------------------------    MenuBar class    -------------------------------------------------





void MenuBar::ResetPositions() {
   /// Horizontal menu bar with menu bar items from left to right in order
   int xpos = area.X();
   int ypos = area.Y();
   int width = 0;
   int maxheight = 0;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      MenuBarItem* mbi = items[i];
      mbi->SetDrawPos(xpos,ypos);
      const Rectangle r = mbi->Area();
      if (r.H() > maxheight) {maxheight = r.H();}
      int w = r.W();
      xpos += w;
      width += w;
   }
   WidgetBase::SetDrawDimensions(width , maxheight);
}




bool MenuBar::HasItem(MenuBarItem* mbi) {
   for (UINT i = 0 ; i < items.size() ; ++i) {
      if (items[i] == mbi) {return true;}
   }
   return false;
}



void MenuBar::CloseActiveItem() {
   if (active_item) {
      active_item->SetMenuState(false);
      active_item = 0;
      open = false;
      active = false;
   }
}



void MenuBar::ActivateItem(MenuBarItem* mbi , bool item_open) {
   if (mbi) {
      if (item_open) {
         mbi->SetMenuState(true);
      } else {
         mbi->SetButtonUpStateOnly(false);
      }
      open = item_open;
      active = true;
      active_item = mbi;
      active_index = ItemIndex(mbi);
      SetRedrawFlag();
   }
}



MenuBarItem* MenuBar::ItemHover(int msx , int msy) {
   if (area.Contains(msx,msy)) {
      for (UINT i = 0 ; i < items.size() ; ++i) {
         MenuBarItem* mbi = items[i];
         if (mbi->Area().Contains(msx,msy)) {return mbi;}
      }
   }
   return (MenuBarItem*)0;
}



int MenuBar::ItemIndex(MenuBarItem* mbi) {
   if (mbi) {
      for (UINT i = 0 ; i < items.size() ; ++i) {
         if (items[i] == mbi) {return (int)i;}
      }
   }
   return items.size();
}



MenuBar::MenuBar(string name) :
      WidgetBase(name),
      items(),
      active(false),
      open(false),
      active_item(0),
      active_index(0)
{}



MenuBar::MenuBar(string name , const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      items(),
      active(false),
      open(false),
      active_item(0),
      active_index(0)
{}



WidgetMsg MenuBar::Update(double tsec) {
   if (open) {
      UINT retmsg = DIALOG_OKAY;
      for (UINT i = 0 ; i < items.size() ; ++i) {
         MenuBarItem* mbi = items[i];
         retmsg |= (mbi->Update(tsec)).Messages();
      }
      return WidgetMsg(this , TOPIC_DIALOG , retmsg);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg MenuBar::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      /// Three states - inactive , active closed, and active open
      
      /// Alt key click toggles activity and opens/closes open menu bar item
      if (input_key_press(KEY_ANY_ALT)) {
         retmsg |= DIALOG_TAKE_FOCUS;
         retmsg |= DIALOG_INPUT_USED;
         if (active) {
            CloseActiveItem();
            retmsg |= DIALOG_REDRAW_ALL;
         } else {
            if (items.size()) {
               ActivateItem(items[0] , false);// show button press without menu open
            }
         }
         SetRedrawFlag();
      }
      
      /// Left / Right arrow keys move along menu , Down arrow key moves into menu
      /// Shortcut keys open the selected menu
      if (active && !open) {
         if (input_key_press(KEY_LEFT)) {
            CloseActiveItem();
            --active_index;
            if (active_index < 0) {active_index += items.size();}
            if (active_index < 0) {active_index = 0;}// For when there are no menu bar items
            if (items.size()) {
               ActivateItem(items[active_index] , false);
            }
            retmsg |= DIALOG_INPUT_USED;
//            retmsg |= DIALOG_REDRAW_ALL;
            SetRedrawFlag();
         }
         if (input_key_press(KEY_RIGHT)) {
            CloseActiveItem();
            ++active_index;
            if (active_index >= (int)items.size()) {active_index -= items.size();}
            if (active_index >= (int)items.size()) {active_index = 0;}// For when there are no menu bar items
            if (items.size()) {
               ActivateItem(items[active_index] , false);
            }
            retmsg |= DIALOG_INPUT_USED;
//            retmsg |= DIALOG_REDRAW_ALL;
            SetRedrawFlag();
         }
         if (input_key_press(KEY_DOWN)) {
            CloseActiveItem();
            ActivateItem(items[active_index] , true);
            retmsg |= DIALOG_INPUT_USED;
            SetRedrawFlag();
            /// TODO : The menu is now open, perhaps inputs shouldn't be checked after this
         }
         for (UINT i = 0 ; i < items.size() ; ++i) {
            if (items[i]->InputKey()) {
               CloseActiveItem();
               ActivateItem(items[i] , true);
               retmsg |= DIALOG_INPUT_USED;
               SetRedrawFlag();
            }
         }
      }
      
      
      /// Inactive - mouse clicks on menu bar items open their menu
      
      /// Active closed - mouse clicks on menu bar items open their menu and
      ///    hover activity changes the unopened active item
      
      /// Active open - hover activity changes the opened active item 
      
      
      MenuBarItem* hover = ItemHover(msx,msy);
      
      if (!active || (active && !open)) {
         if (hover && input_mouse_press(LMB)) {
            CloseActiveItem();
            ActivateItem(hover , true);
            retmsg |= DIALOG_INPUT_USED;
            retmsg |= DIALOG_TAKE_FOCUS;
            SetRedrawFlag();
         }
      }
      
      if (active) {
         if (hover && (hover != active_item)) {
            bool was_open = open;
            CloseActiveItem();
            ActivateItem(hover , was_open);
            if (open) {retmsg |= DIALOG_REDRAW_ALL;}
            SetRedrawFlag();
         }
         if (input_mouse_press(LMB)) {
            if (!Contains(msx,msy)) {
               CloseActiveItem();
               retmsg |= DIALOG_INPUT_USED;
               retmsg |= DIALOG_REDRAW_ALL;
               SetRedrawFlag();
            }
         }
         if (open) {
            Menu* sub_menu = active_item->SubMenu();
            if (sub_menu) {
               if (!(retmsg & DIALOG_INPUT_USED)) {
                  /// Check inputs for the sub menu
                  WidgetMsg msg = sub_menu->CheckInputs(msx,msy);
                  retmsg |= msg.Messages();
                  
                  /// Key left and right menu changes for when key left and right
                  /// wouldn't do anything else (input not used by sub menu)
                  if (!(retmsg & DIALOG_INPUT_USED)) {
                     if (input_key_press(KEY_LEFT)) {
                        CloseActiveItem();
                        --active_index;
                        if (active_index < 0) {active_index += items.size();}
                        if (active_index < 0) {active_index = 0;}// For when there are no menu bar items
                        if (items.size()) {
                           ActivateItem(items[active_index] , true);
                        }
                        retmsg |= DIALOG_INPUT_USED;
                        retmsg |= DIALOG_REDRAW_ALL;
                        SetRedrawFlag();
                     }
                     if (input_key_press(KEY_RIGHT)) {
                        CloseActiveItem();
                        ++active_index;
                        if (active_index >= (int)items.size()) {active_index -= items.size();}
                        if (active_index >= (int)items.size()) {active_index = 0;}// For when there are no menu bar items
                        if (items.size()) {
                           ActivateItem(items[active_index] , true);
                        }
                        retmsg |= DIALOG_INPUT_USED;
                        retmsg |= DIALOG_REDRAW_ALL;
                        SetRedrawFlag();
                     }
                  }
               }
            }
         }
      }
      
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void MenuBar::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
/*
      Rectangle r = area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[BGCOL]);
      r.Draw(bmp , (*wcols)[FGCOL]);
*/
      for (UINT i = 0 ; i < items.size() ; ++i) {
         MenuBarItem* mbi = items[i];
         mbi->DisplayOn(bmp , x , y);
      }
   }
   ClearRedrawFlag();
}



void MenuBar::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      wcols = colorset;
      for (UINT i = 0 ; i < items.size() ; ++i) {
         MenuBarItem* mbi = items[i];
         mbi->UseWidgetColorset(colorset);
      }
   }
}



void MenuBar::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   ResetPositions();
}



bool MenuBar::IsMouseOver(int msx , int msy) const {
   return Contains(msx,msy);
}



void MenuBar::AddItem(MenuBarItem* mbi) {
   if (mbi && !HasItem(mbi)) {
      mbi->SetParent(this);
      mbi->SetDrawTarget(WidgetBase::dest);
      mbi->UseWidgetColorset(WidgetBase::wcols);
      items.push_back(mbi);
      ResetPositions();
      SetRedrawFlag();
   }
}



MenuBar& MenuBar::operator<<(MenuBarItem* mbi) {
   AddItem(mbi);
   return *this;
}



void MenuBar::MenuWasClosed() {
   CloseActiveItem();
   SetRedrawFlag();
}



bool MenuBar::Contains(int x , int y) const {
   if (area.Contains(x,y)) {return true;}
   if (open) {
      if (active_item->Contains(x,y)) {return true;}
   }
   return false;
}



ostream& operator<<(ostream& os , const MenuBar& mbar) {
   os << "Menu bar " << &mbar << " has " << mbar.items.size() << " items and" << endl;
   os << "   " << (mbar.active?"Is":"Is not") << " active." << endl;
   os << "   " << (mbar.open?"Is":"Is not") << " open." << endl;
   os << "   " << "Active item is " << mbar.active_item << endl;
   os << "   " << "Active index is " << mbar.active_index << endl;
   return os;
}







