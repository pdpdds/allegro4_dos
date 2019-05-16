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


#ifndef ListBox_H
#define ListBox_H


#include <map>
#include <list>
#include <string>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ScrollBarWidget.hpp"
#include "Eagle/Error.hpp"



extern const unsigned int TOPIC_LISTBOX;

enum LISTBOX_MSGS {
   LISTBOX_SELECTION_CHANGED = 0,
   LISTBOX_DOUBLE_CLICK_SELECT = 1
};



class ListItem {
public :
   std::string str;
   bool selected;
   
   ListItem(std::string s , bool is_selected);
};


typedef std::list<ListItem>::iterator LIIT;

class ListBox : public WidgetBase {
protected :
   std::list<ListItem> items;
   
   ScrollBar vscroller;// lines
   ScrollBar hscroller;// pixels
   int vscroll;// lines
   int hscroll;// pixels

   FONT* text_font;
   int padding;
   int lineheight;
   
   Rectangle list_area;

   int item_anchor;
   bool item_anchored;
   
   bool multiselect;
   

   void SetupPositions();
   int MaxItemLength();
   void ResetScrollBars();

   bool ItemInView(UINT uindex);

public :
   
   /// Minimum height and width of 40 + 16 + 2 = 58
   ListBox(std::string name , FONT* textfont , bool multiple_selections_allowed , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   virtual ~ListBox() {}
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();
   
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   void SetFont(FONT* textfont);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// Setters
   void ClearSelections();// all items are deselected
   void SelectItem(UINT index);// all other items are deselected, will scroll view if necessary
   void SelectItem(std::string s);// all other items are deselected, will scroll view if necessary
   void SelectItems(UINT start_index , UINT end_index , bool selected);// end_index is inclusive, no other items are (de)selected
   void ToggleItem(UINT index);
   void ToggleItem(std::string s);
   void SetItemSelection(UINT index , bool selected = true);
   void SetItemSelection(std::string s , bool selected = true);

   void AllowMultipleSelections(bool allow);

   virtual void AddListItem(std::string s , bool selected = false);
   virtual ListBox& operator<<(std::string s);
   
   virtual void ClearList();
   virtual void Remove(std::string s);
   virtual void SetList(std::list<std::string> ls);
   virtual void CopyFromFileList();// Only for use with the File Selection Dialog.
   
   /// Getters
   std::list<std::string> GetSelections();
   std::string GetSelections(const char* quote , const char* delimiter);
   std::string GetSelection();// single select only - returns first selected item or empty string if none selected. Check ItemsSelected() first.
   bool ItemsSelected();
   bool OnList(std::string s);

};



/// ------------------------------------------- TypedListBox class ----------------------------------------------


/// The class Type must have a member function 'std::string Type::ToString()' and a default constructor.
/// This means TypedListBox<class Type> won't work for built in types. Write a wrapper for the built in type
/// that has the member function ToString and a default constructor. It should be copy safe as well.
template <class Type>
class TypedListBox : public ListBox {
protected :
   
   std::map<std::string , Type> smap;
   
public :
   TypedListBox(std::string name , FONT* textfont , bool multiple_selections_allowed ,
                  const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   /// Overloads from ListBox. Some base class functions are not valid in the derived class and they are noted.
   
   void AddListItem(std::string s , bool selected = false) throw (EagleError);// Do not call this function.
   ListBox& operator<<(std::string s) throw (EagleError);// Do not call this function.
   
   void ClearList();
   void Remove(std::string s);
   
   void SetList(std::list<std::string> ls) throw (EagleError);// Do not call this function.
   void CopyFromFileList() throw (EagleError);// Do not call this function.

   /// Member functions
   
   void TypedSelectItem(Type t);
   void TypedSetItemSelection(Type t , bool selected = true);
   
   void AddListItem(Type t , bool selected = false);
   TypedListBox<Type>& operator<<(Type t);// Same as AddListItem(t , false).
   
   void Remove(Type t);
   void SetList(std::list<Type> lt);

   /// Additional Getters
   std::list<Type> GetTypedSelections();
   Type GetTypedSelection();// single select only - returns first selected item or Type() if none selected. Check ItemsSelected() first.
   bool OnList(Type t);

};



template <class Type>
TypedListBox<Type>::TypedListBox(std::string name , FONT* textfont , bool multiple_selections_allowed ,
                                 const Rectangle& position , UINT wflags) :
      ListBox(name , textfont , multiple_selections_allowed , position , wflags),
      smap()
{}



template <class Type>
void TypedListBox<Type>::AddListItem(std::string s , bool selected) throw (EagleError) {
   const char* error = "Error. Can't use TypedListBox::AddListItem(std::string s , bool selected). Use TypedListBox::AddListItem(Type t , bool selected) instead.";
   throw EagleError(error);
   ASSERT(0);//Use TypedListBox::AddListItem(Type t , bool selected) instead
}



template <class Type>
ListBox& TypedListBox<Type>::operator<<(std::string s) throw (EagleError) {
   const char* error = "Error. Can't use TypedListBox::operator<<(std::string s). Use TypedListBox::operator<<(Type t) instead";
   throw EagleError(error);
   ASSERT(0);// Use TypedListBox::operator<<(Type t) instead
   return ListBox::operator<<(s);// never reached
}



template <class Type>
void TypedListBox<Type>::ClearList() {
   ListBox::ClearList();
   smap.clear();
}



template <class Type>
void TypedListBox<Type>::Remove(std::string s) {
   ListBox::Remove(s);
   typename std::map<std::string,Type>::iterator it = smap.find(s);
   if (it != smap.end()) {
      smap.erase(it);
   }
}



template <class Type>
void TypedListBox<Type>::SetList(std::list<std::string> ls) throw (EagleError) {
   const char* error = "Error. Can't use TypedListBox::SetList(std::list<std::string> ls). Use TypedListBox::SetList(std::list<Type> lt) instead.";
   throw EagleError(error);
   ASSERT(0);// Use TypedListBox::SetList(std::list<Type> lt) instead
}



template <class Type>
void TypedListBox<Type>::CopyFromFileList() throw (EagleError) {
   const char* error = "Error. Can't use TypedListBox::CopyFromFileList(). Not for public use anyway.";
   throw EagleError(error);
   ASSERT(0);// Base class function only used by the file selection dialog
}



template <class Type>
void TypedListBox<Type>::TypedSelectItem(Type t) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   ListBox::SelectItem(s);
}



template <class Type>
void TypedListBox<Type>::TypedSetItemSelection(Type t , bool selected) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   ListBox::SetItemSelection(s,selected);
}



template <class Type>
void TypedListBox<Type>::AddListItem(Type t , bool selected) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   typename std::map<std::string , Type>::iterator it = smap.find(s);
   if (it == smap.end()) {
      smap[s] = t;
      ListBox::AddListItem(s , selected);
   }
}



template <class Type>
TypedListBox<Type>& TypedListBox<Type>::operator<<(Type t) {
   AddListItem(t,false);
   return *this;
}



template <class Type>
void TypedListBox<Type>::Remove(Type t) {
   std::string s = t.ToString();// std::string Type::ToString() must be defined
   typename std::map<std::string , Type>::iterator it = smap.find(s);
   if (it != smap.end()) {
      smap.erase(it);
      ListBox::Remove(s);
   }
}



template <class Type>
void TypedListBox<Type>::SetList(std::list<Type> lt) {
   ClearList();
   for (typename std::list<Type>::iterator it = lt.begin() ; it != lt.end() ; ++it) {
      AddListItem(*it);
   }
}



template <class Type>
std::list<Type> TypedListBox<Type>::GetTypedSelections() {
   std::list<Type> lt;
   for (LIIT it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.selected) {
         typename std::map<std::string , Type>::iterator mit = smap.find(li.str);
         ASSERT(mit != smap.end());// Should always be in the map
         lt.push_back(mit->second);
      }
   }
   return lt;
}



template <class Type>
Type TypedListBox<Type>::GetTypedSelection() {
   for (std::list<ListItem>::iterator it = items.begin() ; it != items.end() ; ++it) {
      ListItem& li = *it;
      if (li.selected) {
         typename std::map<std::string , Type>::iterator mit = smap.find(li.str);
         ASSERT(mit != smap.end());// Should always be in map
         return mit->second;
      }
   }
   return Type();
}



template <class Type>
bool TypedListBox<Type>::OnList(Type t) {
   return ListBox::OnList(t.ToString());
}




#endif // ListBox_H








