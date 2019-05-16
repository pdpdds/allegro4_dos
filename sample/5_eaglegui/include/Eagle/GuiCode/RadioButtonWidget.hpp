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


#ifndef RadioButtonWidget_H
#define RadioButtonWidget_H


#include <vector>
#include <map>
#include <string>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/GuiCode/IconButton.hpp"
#include "Eagle/Error.hpp"



extern const unsigned int TOPIC_RADIO_BUTTON;



enum RADIO_MSGS {
   RADIO_BTN_CLICKED = 0
};



enum RADIO_TYPE {
   RADIO_HORIZONTAL = 0,
   RADIO_VERTICAL = 1
};

const char* ToString(RADIO_TYPE type);

/// ------------------------      Radio Button Item Base class     ---------------------------------------



class RadioButtonItemBase {
protected :
   std::string text;
   
public :
   RadioButtonItemBase(std::string text_str);
//   RadioButtonItemBase(std::string text_str , std::string name);
//   RadioButtonItemBase(std::string text_str , std::string name , Rectangle position , UINT wflags = DEFAULT_FLAGS);

   virtual ~RadioButtonItemBase() {}


   virtual std::string Text()=0;
   virtual void SetButtonUpState(bool button_up)=0;
   virtual bool ButtonUpState()=0;
   
};



/// ------------------------      IconRadioButtonItem class     --------------------------------------



class IconRadioButtonItem : public RadioButtonItemBase , public IconButton {
   
public :
   // Uses ICON_DISPLAY_CENTER , RECTANGLE_BTN , TOGGLE_BTN , input_key_press(KEY_NONE)
   IconRadioButtonItem(std::string text_label , std::string name);

   // RECTANGLE_BTN , TOGGLE_BTN , input_key_press(KEY_NONE)
   IconRadioButtonItem(std::string text_label , std::string name , ICON_DISPLAY_TYPE dtype , Rectangle position , UINT wflags = DEFAULT_FLAGS);

   IconRadioButtonItem(std::string text_label , std::string name , ICON_DISPLAY_TYPE dtype , BUTTON_SHAPE shape ,
                       InputGroup input , Rectangle position , UINT wflags = DEFAULT_FLAGS);

   /// From RadioButtonItemBase
   virtual std::string Text();
   virtual void SetButtonUpState(bool button_up);
   virtual bool ButtonUpState();

};



/// ------------------------      Radio Button Item class     ---------------------------------------



class RadioButtonItem : public RadioButtonItemBase , public WidgetBase {

private :
   void ResetPositions();

protected :
   Button* button;
   FONT* text_font;
   int hpad;
   int tx;
   int ty;
   int tw;
   int th;
   

public :

   RadioButtonItem(std::string name , BUTTON_CLASS btnclass , BUTTON_SHAPE shape , FONT* textfont , std::string text_label ,
                   int btnw , int btnh , int padding , Rectangle position , UINT wflags = DEFAULT_FLAGS);

   virtual ~RadioButtonItem();


   /// From WidgetBase
   WidgetMsg Update (double tsec);
   WidgetMsg CheckInputs(int msx , int msy);
   void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);
   
   
   void SetWidgetColors(const WidgetColorset& colors);
   void UseWidgetColorset(WidgetColorset* colorset);
   void UseIndividualColorset();
   
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);
   
   virtual bool SetColorDepth(int color_depth);

   void SetFont(FONT* textfont);

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// From RadioButtonItemBase
   virtual std::string Text();
   virtual void SetButtonUpState(bool button_up);
   virtual bool ButtonUpState();
   

   
};



/// ------------------------      GenericRadioButtonItem class     --------------------------------------



class GenericRadioButton : public WidgetBase {
protected :
   std::vector<Button*> buttons;
   std::map<Button* , bool> delete_map;
   RADIO_TYPE radio_type;
   int padding;
   UINT active_index;
   
   Button* active_item;
   Button* hover_item;
   
   bool delete_added_widgets;
   
   FONT* text_font;

   void Reposition();
   void Resize();
   void ResetMinDimensions();
   void AdjustItemSizes();

   UINT ItemIndex(Button* item);
   
   bool HasButton(Button* item);


public :


   GenericRadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int pad = 0 , UINT wflags = DEFAULT_FLAGS);

   virtual ~GenericRadioButton() {ClearItems();}

   /// From WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   /// When a RadioButton is resized, it resizes the items within it and preserves the padding that is set
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// Class methods
   // all items will have the same dimensions, that of the largest item's AbsMinWidth and AbsMinHeight, or larger
   virtual void AddItem(Button* btn , bool delete_when_done);
   virtual GenericRadioButton& operator<<(Button* rbtn);// shortcut to AddItem(rbtn , DeleteWhenDone())
   virtual GenericRadioButton& operator<<(MEMORY_MANAGEMENT_TYPE type);
   virtual void RemoveItem(std::string item_text);
   virtual void RemoveItem(Button* btn);
   virtual void ClearItems();
   
   virtual void SetActiveItem(Button* btn);
   void SetActiveItem(std::string item_text);
   void SetActiveItem(UINT index);
   
   void SetFont(FONT* textfont);
   
   // call this if you set the font of a child RadioButtonItemBase*, or you will have problems
   void RenewItemSizes();
   
   Button* ActiveItemPointer();
   Button* ItemIndexToButton(UINT index);
   std::string ActiveItem();
   UINT ActiveItemIndex();
   UINT Size();

   virtual void PrintTo(std::ostream& os , Indenter& indent) const;
};


/// --------------------------      RadioButton     ----------------------------------------



class RadioButton : public WidgetBase {
protected :
   int pad;
   RADIO_TYPE radio_type;
   FONT* text_font;

   std::vector<RadioButtonItemBase*> items;
   std::map<RadioButtonItemBase* , bool> delete_map;
   
   RadioButtonItemBase* active_item;
   RadioButtonItemBase* hover_item;


   void Reposition();
   void Resize();
   void ResetMinDimensions();
   void AdjustItemSizes();

   UINT ItemIndex(RadioButtonItemBase* item);
   
   bool HasButton(RadioButtonItemBase* item);

public :

   RadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int padding = 0 , UINT wflags = DEFAULT_FLAGS);

   virtual ~RadioButton() {ClearItems();}

   /// From WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();

   virtual void SetDrawPos(int xpos , int ypos);
   /// When a RadioButton is resized, it resizes the items within it and preserves the padding that is set
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// Class methods
   // all items will have the same dimensions, that of the largest item's AbsMinWidth and AbsMinHeight, or larger
   virtual void AddItem(RadioButtonItemBase* rbtn , bool delete_when_done);
   virtual RadioButton& operator<<(RadioButtonItemBase* rbtn);// shortcut to AddItem(rbtn , false)
   virtual void RemoveItem(std::string item_text);
   virtual void RemoveItem(RadioButtonItemBase* rbtn);
   virtual void ClearItems();
   
   virtual void SetActiveItem(RadioButtonItemBase* item);
   void SetActiveItem(std::string item_text);
   void SetActiveItem(UINT index);
   
   void SetFont(FONT* textfont);
   
   // call this if you set the font of a child RadioButtonItemBase*, or you will have problems
   void RenewItemSizes();
   
   RadioButtonItemBase* ActiveItemPointer();
   std::string ActiveItem();
   UINT ActiveItemIndex();
   UINT Size();
};



/// ---------------------------    TypedRadioButton class   ---------------------------------------


template <class Type>
class TypedRadioButton : public RadioButton {
protected :
   std::map<RadioButtonItemBase* , Type> rbimap;
   
public :
   TypedRadioButton(std::string name , RADIO_TYPE rtype , int xpos , int ypos , int padding = 0 , UINT wflags = DEFAULT_FLAGS) :
         RadioButton(name , rtype , xpos , ypos , padding , wflags),
         rbimap()
   {}
   ~TypedRadioButton() {}
   
   /// From RadioButton
   
   void AddItem(std::string text) throw (EagleError);// do not call this function
   RadioButton& operator<<(std::string text) throw (EagleError);// do not call this function
   
   void RemoveItem(RadioButtonItemBase* rbtn);
   void ClearItems();
   
   /// Member functions
   
   // For the next two functions, it is recommended that Type only stores an indirect pointer to a RadioButtonItemBase object
   // Ie... The class shouldn't new and delete it's IconRadioButton/RadioButtonItem object
   // Either it should be a member or it should just be referenced from the Type object
   void AddItem(const Type& t);// Type must have a RadioButtonItemBase* GetRadioButton() const function
   TypedRadioButton& operator<<(const Type& t);// Type must have a RadioButtonItemBase* GetRadioButton() const function
   
   void AddItem(const Type& t , RadioButtonItemBase* rbtn);
   
   /// Getter
   
   Type TypedActiveItem();
};



template <class Type>
void TypedRadioButton<Type>::AddItem(std::string text) throw (EagleError) {
   const char* error = "Error. Can't use TypedRadioButton::AddItem(std::string text). Use TypedRadioButton::AddItem(Type t) instead.";
   throw EagleError(error);
   ASSERT(0);// Use TypedRadioButton::AddItem(Type t) instead.
}



template <class Type>
RadioButton& TypedRadioButton<Type>::operator<<(std::string text) throw (EagleError) {
   const char* error = "Error. Can't use TypedRadioButton::operator<<(std::string text). Use TypedRadioButton::operator<<(Type t) instead.";
   throw EagleError(error);   // Use TypedRadioButton::operator<<(Type t) or AddItem(RadioButtonItemBase* , Type t) instead.
   ASSERT(0);
   return *this;///RadioButton::operator<<(text);// never reached
}



template <class Type>
void TypedRadioButton<Type>::RemoveItem(RadioButtonItemBase* rbtn) {
   RadioButton::RemoveItem(rbtn);
   typename std::map<RadioButtonItemBase* , Type>::iterator it = rbimap.find(rbtn);
   if (it != rbimap.end()) {
      rbimap.erase(it);
   }
}



template <class Type>
void TypedRadioButton<Type>::ClearItems() {
   RadioButton::ClearItems();
   rbimap.clear();
}



template <class Type>
void TypedRadioButton<Type>::AddItem(const Type& t) {
   RadioButtonItemBase* rbtn = t.GetRadioButton();
   AddItem(t , rbtn);
}



template <class Type>
TypedRadioButton<Type>& TypedRadioButton<Type>::operator<<(const Type& t) {
   AddItem(t);
   return *this;
}



template <class Type>
void TypedRadioButton<Type>::AddItem(const Type& t , RadioButtonItemBase* rbtn) {
   if (!rbtn) {return;}
   typename std::map<RadioButtonItemBase* , Type>::iterator it = rbimap.find(rbtn);
   if (it == rbimap.end()) {
      rbimap[rbtn] = t;
      AddItem(rbtn , false);
   }
}



template <class Type>
Type TypedRadioButton<Type>::TypedActiveItem() {
   ASSERT(items.size());// no items!
   return rbimap[RadioButton::ActiveItemPointer()];
}




#endif // RadioButtonWidget_H

