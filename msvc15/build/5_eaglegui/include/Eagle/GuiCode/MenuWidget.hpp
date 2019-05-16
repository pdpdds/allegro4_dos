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


#ifndef MenuWidget_H
#define MenuWidget_H



#include <string>
#include <iostream>
#include <vector>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/BitmapHandler.hpp"
#include "Eagle/InputHandler.hpp"



extern const unsigned int TOPIC_MENU_ITEM;
extern const unsigned int TOPIC_MENU;

enum MENU_ITEM_MSGS {
   MENU_ITEM_CLICKED = 0
};

enum MENU_MSGS {
   MENU_OPENED = 0
};


int DefaultMenuItemPadding();




/** ----------------------  MenuItem class  ----------------------------*/





class Menu;


/// MenuItems are not global constructor safe
class MenuItem : public WidgetBase {

private :

   void CalcDimensions();

protected :
   
   BitmapHandler icon;
   BitmapHandler icon_disabled;
   
   InputGroup input_key;
   FONT* text_font;
   std::string label;
   std::string key_text;

   Menu* sub_menu;

   int icon_cw;
   int label_cw;
   int key_text_cw;
   int arrow_text_cw;
   int item_rh;
   int padding;

   bool show_icon;
   bool toggle_icon_on_click;

   static const char* arrow_text;

public :
   
//   MenuItem();
   
   // This constructor is global object safe
   MenuItem(std::string name , InputGroup key_input , FONT* textfont , std::string item_label , std::string info_text , Menu* menu,
            const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   // This constructor clones the bitmaps so you can free them after the constructor finishes and Ready() is true
   MenuItem(std::string name , BITMAP* icon_pic , BITMAP* disabled_icon_pic , BMP_DRAW_TYPE bmp_draw_type , bool toggle_icon,
            InputGroup key_input , FONT* textfont , std::string item_label , std::string info_text ,
            Menu* menu , int padding_size , bool display_icon,
            const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   // This constructor clones the bitmaps so you can free them after the constructor finishes and Ready() is true
   MenuItem(std::string name , const char* icon_file , const char* disabled_icon_file , BMP_DRAW_TYPE bmp_draw_type , bool toggle_icon,
            InputGroup key_input , FONT* textfont , std::string item_label , std::string info_text ,
            Menu* menu , int padding_size , bool display_icon,
            const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);


   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   void FormattedDisplayOn(BITMAP* bmp , int x , int y ,
                           int icon_colw , int label_colw , int key_text_colw , int item_rowh);

   void SetFormatting(int icon_colw , int label_colw , int key_text_colw , int arrow_text_colw , int item_rowh);
   void ShrinkWrap();

   virtual void UseWidgetColorset(WidgetColorset* colorset);
   

   virtual bool AcceptsFocus() {return false;}

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetRedrawFlag();

   void SetPadding(int pad);// Only for stand alone menu items not added to a menu
   void SetFont(FONT* textfont);// Set the font before adding to a menu
   void SetIconVisibility(bool visible);
   void SetIconToggleOnClick(bool toggle_icon);

   void SetMenu(Menu* m);
   
   /// For Load*Icon, passing 0 for the file will clear that icon's picture
   bool LoadIcons(const char* normal_icon_file , const char* disabled_icon_file , BMP_DRAW_TYPE bmp_draw_type);
   bool LoadNormalIcon(const char* icon_file , BMP_DRAW_TYPE bmp_draw_type);
   bool LoadDisabledIcon(const char* icon_file , BMP_DRAW_TYPE bmp_draw_type);

   /// For Set*Icon, passing 0 for the bitmap will clear that icon's picture
   bool SetIcons(BITMAP* normal_icon , BITMAP* disabled_icon , BMP_DRAW_TYPE bmp_draw_type);
   bool SetNormalIcon(BITMAP* icon_bmp , BMP_DRAW_TYPE bmp_draw_type);
   bool SetDisabledIcon(BITMAP* icon_bmp , BMP_DRAW_TYPE bmp_draw_type);

   void ClearIcons();
   
   bool ReloadIcons();
   

   /// Getters
   InputGroup InputKey()   {return input_key;}
   std::string     Label()      {return label;}
   std::string     InfoText()   {return key_text;}
   bool       ShowIcon()   {return show_icon;}
   bool       ItemActive() {return show_icon;}
   bool       ToggleIconOnClick() {return toggle_icon_on_click;}

   int IconColW()      {return icon_cw;}
   int LabelColW()     {return label_cw;}
   int KeyTextColW()   {return key_text_cw;}
   int ArrowTextColW() {return arrow_text_cw;}
   int ItemRowH()      {return item_rh;}
   int Padding()       {return padding;}

   Menu* SubMenu()     {return sub_menu;}

   bool HasMenu() {return (bool)sub_menu;}
   
   void CascadeCloseMenu();
   
   
   friend std::ostream& operator<<(std::ostream& os , const MenuItem& m);
   
};





/** ----------------------  Menu class  ----------------------------*/






class Menu : public WidgetBase {

private :
   void ReadjustSize();
   void ResetItemPositions();

   MenuItem* GetItemPointer(int num);
   int GetItemNum(MenuItem* m);

   int CountNumItems();

protected :
   std::vector< std::vector<MenuItem*> > item_groups;

   int icon_cw;
   int label_cw;
   int key_text_cw;
   int arrow_text_cw;
   int item_rh;
   int padding;

   int item_highlight;
   int num_items;
   bool menu_open;
   bool menu_just_opened;
   
   Menu* open_sub_menu;
   MenuItem* item_hover;
   int old_msx;
   int old_msy;

   static const int separator_height;
   
   
public :
   
   Menu(std::string name);
   Menu(std::string name , const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   /// Redefined virtual functions from WidgetBase

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   virtual void UseWidgetColorset(WidgetColorset* colorset);

   virtual void SetDrawPos(int xpos , int ypos);
//   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

//   virtual void SetRedrawFlag();

   void SetFont(FONT* textfont);// Sets the font for this menu and all of its items but not for any sub menus.

   virtual bool IsMouseOver(int msx , int msy) const ;

   /// Member functions

   void OpenMenu();
   void CloseMenu();
   void CascadeCloseMenu();
   void CloseSubMenu();
   
   void HighlightItem(int item_number);
   void HighlightItem(MenuItem* item);

   void AddItem(MenuItem* item , UINT group = 0);
   Menu& operator<<(MenuItem* item);// Same effect as using AddItem(item,0);
   
   void RemoveItem(MenuItem* item);
   void ClearAllItems();

   void SetPadding(int pad);
   
   void MenuItemsChanged();

   bool Contains(int x , int y) const ;

   bool MenuOpen() {return menu_open;}

   friend std::ostream& operator<<(std::ostream& os , const Menu& m);

};





/// ---------------------------------    MenuBarItem class    --------------------------------------





class MenuBarItem : public WidgetBase {
   
private :
   Menu* item_menu;
   Button button;
   
   bool menu_open;
   
   FONT* text_font;

   void SetUpMenu();
   
protected :

public :

//   MenuBarItem();
   MenuBarItem(std::string name , FONT* textfont , std::string label_text , InputGroup input_key , Menu* menu,
               const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   /// Virtual function redefinitions from WidgetBase
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   
   virtual void UseWidgetColorset(WidgetColorset* colorset);

   virtual void SetDrawPos(int xpos , int ypos);
//   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

//   virtual void SetRedrawFlag();

   virtual bool IsMouseOver(int msx , int msy) const ;

   /// Member functions

   void SetButtonUpState(bool button_up);
   void SetMenuState(bool menu_is_open);

   void MenuWasClosed();

   void SetButtonUpStateOnly(bool button_up);

   bool Contains(int x , int y) const ;

   void SetFont(FONT* textfont);

   /// Getters
   bool MenuOpen() {return menu_open;}
   
   InputGroup InputKey() {return button.InputKey();}

   Menu* SubMenu() {return item_menu;}

};





/// ----------------------------------    MenuBar class    -------------------------------------------------




class MenuBar : public WidgetBase {

private :
   std::vector<MenuBarItem*> items;
   
   bool active;
   bool open;
   MenuBarItem* active_item;
   int active_index;
   
   

   void ResetPositions();
   bool HasItem(MenuBarItem* mbi);

   void CloseActiveItem();
   void ActivateItem(MenuBarItem* mbi , bool item_open);


   MenuBarItem* ItemHover(int msx , int msy);
   
   int ItemIndex(MenuBarItem* mbi);
   
protected :
   

public :

   MenuBar(std::string name);
   MenuBar(std::string name , const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   /// Virtual function redefinitions from WidgetBase
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   void UseWidgetColorset(WidgetColorset* colorset);
   
   void SetDrawPos(int xpos , int ypos);

   virtual bool IsMouseOver(int msx , int msy) const ;

   /// Member functions
   
   void AddItem(MenuBarItem* mbi);
   MenuBar& operator<<(MenuBarItem* mbi);// Same effect as AddItem(mbi);

   void MenuWasClosed();
   
   bool Contains(int x , int y) const ;

   friend std::ostream& operator<<(std::ostream& os , const MenuBar& mbar);

};







#endif // MenuWidget_H

