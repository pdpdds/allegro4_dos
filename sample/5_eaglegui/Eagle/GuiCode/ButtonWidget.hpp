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


#ifndef ButtonWidget_H
#define ButtonWidget_H


#include <string>

#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/InputHandler.hpp"



extern const unsigned int TOPIC_BUTTON_WIDGET;

enum BUTTON_MSGS {
   BUTTON_CLICKED  = 0, // Sent whenever a spring button is pushed down.
   BUTTON_HELD     = 1, // Sent whenever a spring button is held down.
   BUTTON_RELEASED = 2, // Sent whenever a spring button is released.
   BUTTON_TOGGLED  = 3  // Sent whenever a toggle button is toggled, poll for the current state.
};

enum BUTTON_SHAPE {
   RECTANGLE_BTN = 0,
   CIRCLE_BTN    = 1,
   ROUNDED_BTN   = 2,
   ELLIPSE_BTN   = 3,
};

enum BTN_ACTION_TYPE {
   SPRING_BTN = 0,
   TOGGLE_BTN = 1
};

extern double SPRING_BTN_DURATION;// Set before instantiating a new Button or use Button::SetSpringDuration
//extern double BTN_ROUNDNESS = 0.4;

enum BUTTON_CLASS {
   PLAIN_BUTTON = 0,
   HOVER_BUTTON = 1
};


/** Basic variety of simple spring or toggle buttons - rectangular, circular, rounded rectangular, and elliptical.
      Support for displaying text on the button included, will probably not look right on the circular buttons due to size.
*/

class AreaBase;

class Button : public WidgetBase {

protected :

   InputGroup input_group;// For holding a key / mouse / jstk button combo to activate the button
   bool pointer_input;

   int btn_type;
   int action_type;// SPRING_BTN or TOGGLE_BTN
   bool up;
   double dn_time_left;
   FONT* text_font;
   std::string text;// gui_textout_ex, gui_strlen from allegro

   /** Button type determines hit detection properties as well */
   // Rectangle button uses dimensions passed in constructor call
   int rad_a;// Primary radius for circles and the horizontal radius for ellipse type buttons,
               // as well as the corner radius for rounded rectangular buttons
   int rad_b;// Secondary vertical radius, only for ellipses

   double spring_duration;
   double roundness;// for ROUNDED_BTN type buttons
   
   /// For tracking state while the button is held
   bool user_activated;
   bool focuskey_activated;
   bool pointer_activated;
   
   AreaBase* click_area;
   bool delete_area_ptr;


   void ResetRadii();
   
   void DrawButtonRectangle(BITMAP* bmp , int x , int y);
   void DrawButtonText(BITMAP* bmp , int x , int y);

   void FreeClickArea();

public :
   /// Button position is determined from the button type, and the radii are set according to type
   Button(std::string name);

   Button(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual ~Button() {FreeClickArea();}

   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
//   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   /// Setters
   void SetInputGroup(InputGroup ig);
   void SetClickArea(AreaBase* btn_click_area , bool delete_when_done);// Relative to widget, NOT absolute
   void SetButtonType(BUTTON_SHAPE shape , BTN_ACTION_TYPE atype);
   void SetSpringDuration(double duration);
   void SetButtonUpState(bool button_up);
   void ToggleButton();
   
   virtual void SetLabel(std::string label_text);
   
   void SetRoundingPercent(double percent);// for ROUNDED_BTN type buttons

   virtual void SetFont(FONT* textfont);

   /// Getters
   InputGroup InputKey()      {return input_group;}
   bool       ButtonUpState() {return up;}
   std::string Text() {return text;}
   
};



class HoverButton : public Button {
private :

public :
   HoverButton(std::string name);

   HoverButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetHoverState (bool state);
   
};


#endif // ButtonWidget_H

