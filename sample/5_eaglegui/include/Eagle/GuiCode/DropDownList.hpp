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


#ifndef DropDownList_H
#define DropDownList_H


#include <vector>
#include <string>
#include <map>
#include <exception>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ScrollBarWidget.hpp" // for ScrollButton class
#include "Eagle/Error.hpp"



extern const unsigned int TOPIC_DDL;


enum DROP_DOWN_LIST_MSGS {
   DDL_SELECTION_CHANGED = 0
};

enum DROP_DOWN_LIST_DIRECTION {
   DDL_OPEN_ABOVE = 0,
   DDL_OPEN_BELOW = 1
};

class DropDownList : public WidgetBase {

private :
   
   ScrollButton list_btn;
   Scroller list_scroller;
   int scroll;
   
   std::vector<std::string> items;
   UINT item_index;
   
   FONT* text_font;
   
   bool list_open;
   UINT item_highlight;
   
   int nitems_shown;
   int line_height;
   DROP_DOWN_LIST_DIRECTION list_direction;
   Rectangle list_area;
   
   
   void SetupPositions();
   void SetupButton();
   void SetupListArea();
   void SetupScroller();
   
public :
   
   DropDownList(std::string name , FONT* textfont , int num_items_shown , const Rectangle& position ,
                DROP_DOWN_LIST_DIRECTION direction = DDL_OPEN_BELOW , UINT wflags = DEFAULT_OVERLAP_FLAGS);
   virtual ~DropDownList() {}

   /// From WidgetBase
   virtual WidgetMsg Update (double tsec); // logic handling with animation support

   virtual WidgetMsg CheckInputs(int msx , int msy);// Pass it the mouse position relative to it's drawing target.
                                                      // Hover is taken care of by WidgetHandlers, if you need it to
                                                      // do something, override the virtual SetHoverState function.

   virtual void DisplayOn(BITMAP* bmp , int x , int y);/// DisplayOn should always draw, and always 
                                                         /// clear the NEEDS_REDRAW flag by calling ClearRedrawFlag().

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual bool IsMouseOver(int x , int y) const ;

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   
   /// Member functions
   virtual void AddItem(std::string str);
   virtual DropDownList& operator<<(std::string str);// Same effect as using AddItem(str);
   virtual void ClearItems();

   /// Setters
   void SetItem(UINT index);
   void SetItem(std::string str);

   void SetFont(FONT* new_font);

   /// Getters
   std::string GetSelection();
   UINT NumItems() {return items.size();}
   UINT GetSelectionIndex();// will fail an ASSERT in debug mode if there are no items

};



/// TypedDropDownList requires that you use a type that provides a string Type::ToString() function.
template <class Type>
class TypedDropDownList : public DropDownList {
protected :
   std::map<std::string , Type> smap;
   
public :
   TypedDropDownList(std::string name , const Rectangle& position , UINT wflags = DEFAULT_OVERLAP_FLAGS);

   ~TypedDropDownList() {}
   
   /// Overloads from DropDownList
   
   void AddItem(std::string str) throw (EagleError);// Do not call this function
   DropDownList& operator<<(std::string str) throw (EagleError);// Do not call this function
   
   /// Member functions
   
   void AddItem(Type t);
   TypedDropDownList& operator<<(Type t);
   
   void SetItem(Type t);
   
   void TypedGetSelection();
};



template <class Type>
TypedDropDownList<Type>::TypedDropDownList(std::string name , const Rectangle& position , UINT wflags) :
      DropDownList(name , position , wflags),
      smap()
{}



template <class Type>
void TypedDropDownList<Type>::AddItem(std::string str) throw (EagleError){
   const char* error = "Error. Can't use TypedDropDownList::AddItem(std::string str). Use TypedDropDownList::AddItem(Type t) instead.";
   throw EagleError(error);
   ASSERT(0);// Use TypedDropDownList::AddItem(Type t) instead.
}



template <class Type>
DropDownList& TypedDropDownList<Type>::operator<<(std::string str) throw (EagleError){
   const char* error = "Error. Can't use TypedDropDownList::operator<<(std::string str). Use TypedDropDownList::operator<<(Type t) instead.";
   throw EagleError(error);
   ASSERT(0);// Use TypedDropDownList::operator<<(Type t) instead.
   return DropDownList::operator<<(str);// never reached
}



template <class Type>
void TypedDropDownList<Type>::AddItem(Type t) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   typename std::map<std::string , Type>::iterator it = smap.find(s);
   if (it == smap.end()) {
      smap[s] = t;
      DropDownList::AddItem(s);
   }
}



template <class Type>
TypedDropDownList<Type>& TypedDropDownList<Type>::operator<<(Type t) {
   AddItem(t);
   return *this;
}



template <class Type>
void TypedDropDownList<Type>::SetItem(Type t) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   DropDownList::SetItem(s);
}



template <class Type>
void TypedDropDownList<Type>::TypedGetSelection() {
   return smap[DropDownList::GetSelection()];
}




#endif // DropDownList_H




