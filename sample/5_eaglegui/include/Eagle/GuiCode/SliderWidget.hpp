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


#ifndef SliderWidget_H
#define SliderWidget_H


#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/InputHandler.hpp"

#include <typeinfo>


extern const unsigned int TOPIC_SLIDER;

enum SLIDER_MSGS {
   SLIDER_VALUE_CHANGED = 0
};

/// Slider values go from left to right for horizontal sliders and from bottom to top for vertical ones
/// If you want the opposite behavior, use the I functions SetIValue and GetIValue. (I stands for Inverted).

/// Valid Types are char , int , float , and double
template <class Type>
class Slider : public WidgetBase {

protected :
   double percent;
   Type min,range,max;
   Type val,increment;

   Rectangle slider_area;
   Rectangle handle;

   bool vertical;// Alignment

   bool active;// For tracking when the handle is held onto.

   void SetDimensions();
   void SetPosition(double percentage);

public :

   Slider(std::string name , Type minimum , Type value , Type maximum , Type key_increment , bool is_vertical ,
            const Rectangle& position , UINT wflags = DEFAULT_FLAGS);


   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   void SetIncrement(Type key_increment);

   void SetValue(Type value);
   void SetIValue(Type value);

   void SetParameters(Type minimum , Type value , Type maximum);
   void SetIParameters(Type minimum , Type value , Type maximum);


   /// Getters
   Type GetValue();
   Type GetIValue();

};




template <class Type>
void Slider<Type>::SetDimensions() {
   if (vertical) {
///      slider_area.SetDimensions(area.W() , (9*area.H())/10);
      slider_area.SetDimensions(area.W() , area.H());
      handle.SetDimensions(area.W() - 2 , area.H()/10);
   } else {
///      slider_area.SetDimensions((9*area.W())/10 , area.H());
      slider_area.SetDimensions(area.W() , area.H());
      handle.SetDimensions(area.W()/10 , area.H() - 2);
   }
}



/// Percentage from left to right for horizontal scroll bars and from bottom to top for vertical ones
template <class Type>
void Slider<Type>::SetPosition(double percentage) {
   if (percentage < 0.0) {percentage = 0.0;}
   if (percentage > 1.0) {percentage = 1.0;}

   percent = percentage;

///   val = min + (Type)((double)range*percent);

   const double length = vertical?area.H():area.W();
//      const double width = vertical?area.W():area.H();

   slider_area.SetPos(area.X() , area.Y());
   if (vertical) {
      handle.SetPos(area.X() + 1 , int(double(area.Y()) + double(area.H())*0.9*(1.0 - percent)));
   } else {
      handle.SetPos(area.X() + int(0.9*percent*length) , area.Y() + 1);
//      handle.SetPos(int((0.9*percentage + 0.05)*length) , 1);
   }
   SetRedrawFlag();
}



template <class Type>
Slider<Type>::Slider(std::string name , Type minimum , Type value , Type maximum , Type key_increment , bool is_vertical ,
               const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      percent(0.0),
      min(minimum) , range(maximum - minimum) , max(maximum),
      val(0),increment(0),
      slider_area(),
      handle(),
      vertical(is_vertical),
      active(false)
{
   SetDimensions();
   SetIncrement(key_increment);
   SetValue(value);
}



template <class Type>
WidgetMsg Slider<Type>::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



template <class Type>
WidgetMsg Slider<Type>::CheckInputs(int msx , int msy) {
   // Accept key input when the widget has focus
   if (WidgetBase::Flags() & HASFOCUS) {
      const double duration = 0.5;
//      const double length = vertical?area.H():area.W();
//      const double increment = 1.0/(0.9*length);// move one pixel at a time
      if (vertical) {
         if (input_key_press(KEY_UP) || (key_held_duration[KEY_UP] > duration)) {
//            SetPosition(percent + increment);
            SetValue(val + increment);
            QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
         }
         if (input_key_press(KEY_DOWN) || key_held_duration[KEY_DOWN] > duration) {
//            SetPosition(percent - increment);
            SetValue(val - increment);
            QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
         }
      } else {
         if (input_key_press(KEY_RIGHT) || key_held_duration[KEY_RIGHT] > duration) {
//            SetPosition(percent + increment);
            SetValue(val + increment);
            QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
         }
         if (input_key_press(KEY_LEFT) || key_held_duration[KEY_LEFT] > duration) {
//            SetPosition(percent - increment);
            SetValue(val - increment);
            QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
         }
      }
   }

   // Watch for handle drags by the mouse
   if (active) {
      if (input_mouse_held(LMB)) {

         // Find slider position
         if (vertical) {
            int ymin = area.Y() + (19*area.H())/20 - 1;
            int ymax = area.Y() +  area.H()/20;
            double percentage = double(msy - ymin)/double(ymax - ymin);
            if (percentage < 0.0) {percentage = 0.0;}
            if (percentage > 1.0) {percentage = 1.0;}
            val = min + (Type)((double)range*percentage);
            SetPosition(percentage);
         } else {
            int xmin = area.X() + area.W()/20;
            int xmax = area.X() + (19*area.W())/20 - 1;
            double percentage = double(msx - xmin)/double(xmax - xmin);
            if (percentage < 0.0) {percentage = 0.0;}
            if (percentage > 1.0) {percentage = 1.0;}
            val = min + (Type)((double)range*percentage);
            SetPosition(percentage);
         }
         QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
      } else if (input_mouse_release(LMB)) {
         active = false;
      }
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
   }

   int retmsg = DIALOG_OKAY;
   // Check the mouse pointer input, look for slider area clicks and handle drags
   if (input_mouse_press(LMB) && area.Contains(msx,msy)) {
      retmsg |= DIALOG_TAKE_FOCUS;
      if (handle.Contains(msx,msy)) {
         active = true;
      } else {// Click on slider area

//         const double increment = 0.1;
         int cx = handle.X() + handle.W()/2;
         int cy = handle.Y() + handle.H()/2;
         int dx = msx - cx;
         int dy = cy - msy;// Inverted so lower is less
         if (vertical) {
            if (dy > 0) {
//               SetPosition(percent + increment);
               SetValue(val + increment);
            }
            if (dy < 0) {
//               SetPosition(percent - increment);
               SetValue(val - increment);
            }
         } else {
            if (dx > 0) {
//               SetPosition(percent + increment);
               SetValue(val + increment);
            }
            if (dx < 0) {
//               SetPosition(percent - increment);
               SetValue(val - increment);
            }
         }
         QueueUserMessage(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);

      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);

}



template <class Type>
void Slider<Type>::DisplayOn(BITMAP* bmp , int x , int y) {
   Rectangle draw_slider_area = slider_area;
   Rectangle draw_handle = handle;
   draw_slider_area.MoveBy(x,y);
   draw_handle.MoveBy(x + 1,y + 1);

   draw_slider_area.Fill(bmp , (*wcols)[MGCOL]);
   draw_slider_area.Draw(bmp , (*wcols)[FGCOL]);
   draw_handle.Draw(bmp , (*wcols)[SDCOL]);
   draw_handle.MoveBy(-1,-1);
   draw_handle.Fill(bmp , (*wcols)[FGCOL]);
}



template <class Type>
void Slider<Type>::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   SetValue(val);
}



template <class Type>
void Slider<Type>::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   SetDimensions();
   SetValue(val);
}



template <class Type>
void Slider<Type>::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   SetDimensions();
   SetValue(val);
}



template <class Type>
void Slider<Type>::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   SetDimensions();
   SetValue(val);
}



template <class Type>
void Slider<Type>::SetIncrement(Type key_increment) {
   if (key_increment < (Type)0) {key_increment = -key_increment;}
   if (key_increment == (Type)0) {
      key_increment = (Type)1;
      if ((typeid(Type) == typeid(float)) || (typeid(Type) == typeid(double))) {
         const double length = vertical?area.H():area.W();
         const double inc_percent = 1.0/(0.9*length);// move one pixel at a time
         const double inc_value = inc_percent*range;
         key_increment = (Type)inc_value;
      }
   }
   increment = key_increment;
}



template <class Type>
void Slider<Type>::SetValue(Type value) {
   if (value < min) {value = min;}
   if (value > max) {value = max;}
   val = value;

   SetPosition(double(val - min) / double(range));
}



template <class Type>
void Slider<Type>::SetIValue(Type value) {
   SetValue(min + max - value);
}



template <class Type>
void Slider<Type>::SetParameters(Type minimum , Type value , Type maximum) {
   if (maximum < minimum) {maximum = minimum;}
   min = minimum;
   max = maximum;
   range = max - min;
   SetValue(value);
}



template <class Type>
void Slider<Type>::SetIParameters(Type minimum , Type value , Type maximum) {
   if (maximum < minimum) {maximum = minimum;}
   min = minimum;
   max = maximum;
   range = max - min;
   SetIValue(value);
}



template <class Type>
Type Slider<Type>::GetValue() {
   return val;
//   return min + Type(percent*double(range));
}



template <class Type>
Type Slider<Type>::GetIValue() {
   return (min + max - val);
//   return min + Type((1.0 - percent)*double(range));
}






#endif // SliderWidget_H

