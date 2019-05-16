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


#ifndef TabGroup_H
#define TabGroup_H


#include "Eagle/GuiCode/WidgetHandler.hpp"

#include <string>
#include <vector>
#include <list>



/// Tabs are not meant to be used as stand alone widgets. To function properly they must be added to a TabGroup.
class Tab : public WidgetBase {
   
private :
   FONT* text_font;
   std::string label;
   bool active;
   
   WidgetHandler* gui;
   
   void ResetTabDimensions();
   
public :


   Tab(std::string name , FONT* textfont , std::string text , WidgetHandler& tab_gui , UINT wflags = DEFAULT_FLAGS);

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void UseWidgetColorset(WidgetColorset* colorset);

   virtual void SetDrawTarget(BITMAP* surface);
   
   /// Don't call these 4 functions. Tab placement is handled by the parent TabGroup.
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual bool AcceptsFocus() {return false;}

   void ActivateTab(bool tab_active);
   
   void SetLabel(std::string new_label);
   void SetFont(FONT* textfont);
   void SetGui(WidgetHandler& tab_gui);
   
   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   virtual bool HasGui() {return true;}
   virtual WidgetHandler* GetGui();
};




/// --------------------------------     TabGroup class     -------------------------------------------




class TabGroup : public WidgetBase {

typedef std::vector<Tab*> TAB_ROW;
typedef TAB_ROW::iterator TRIT;

typedef std::list<TAB_ROW> TAB_ROWS;
typedef TAB_ROWS::iterator    TRSIT;

private :
   TAB_ROWS tab_rows;
   
   Tab* active_tab;
   
   int guiw;
   int guih;


   void ResetTabPositions();

   int NumTabs();
   bool HasTab(Tab* t);

public :
   
//   void        FocusStart();
//   WidgetBase* NextFocus();
//   WidgetBase* PreviousFocus();

   bool HasWidget(WidgetBase* widget);

   TabGroup(std::string name , int gui_width , int gui_height , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   virtual WidgetMsg Update (double tsec);

   virtual WidgetMsg CheckInputs(int msx , int msy);

   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawTarget(BITMAP* surface);

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);// height is fixed based on gui size
   virtual void SetArea(int xpos , int ypos , int width , int height);// height is fixed based on gui size
   virtual void SetArea(const Rectangle& r);// height is fixed based on gui size

   virtual bool AcceptsFocus() {return false;}
   virtual void SetFocusDrawType(FOCUS_DRAW_TYPE type);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   virtual bool HasGui();
   virtual WidgetHandler* GetGui();

   

   void AddTab(Tab* tab , unsigned int row = 0);
   void AddTab(Tab& tab , unsigned int row = 0);

   // operator << has the same effect as AddTab(tab,row=0)
   TabGroup& operator<<(Tab* tab);
   TabGroup& operator<<(Tab& tab);


   void SetActiveTab(Tab* t);
   void SetFont(FONT* textfont);
   void SetGuiFocusDrawType(FOCUS_DRAW_TYPE draw_type , bool for_all_guis);
   
   Tab* ActiveTab() {return active_tab;}

};





#endif // TabGroup_H






