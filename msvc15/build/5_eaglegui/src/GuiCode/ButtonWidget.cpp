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


#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/Area.hpp"

#include <cmath>

using std::string;


const unsigned int TOPIC_BUTTON_WIDGET = NextFreeTopicId();
double SPRING_BTN_DURATION = 0.25;
double BTN_ROUNDNESS = 0.4;



void Button::ResetRadii() {
   // Uses the largest size shape that will fit into the rectangle
   switch (btn_type) {
      case RECTANGLE_BTN : 
         break;
      case CIRCLE_BTN : 
         rad_a = (area.W()<area.H())?(area.W()/2):(area.H()/2);
         break;
      case ROUNDED_BTN :
         {
            int min = (area.W()<area.H())?(area.W()):(area.H());
            rad_a = (2*min)/5;
         }
         break;
      case ELLIPSE_BTN : 
         rad_a = area.W()/2;
         rad_b = area.H()/2;
         break;
   }
}



void Button::DrawButtonRectangle(BITMAP* bmp , int x , int y) {
   Rectangle r = area;
   r.MoveBy(x,y);
   switch (btn_type) {
      case RECTANGLE_BTN : 
         up?
         r.DrawGuiRectUp(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL]):
         r.DrawGuiRectDown(bmp , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
         break;
      case CIRCLE_BTN : 
         up?
         r.DrawGuiCircleUp(bmp , rad_a , (*wcols)[FGCOL] , (*wcols)[SDCOL]):
         r.DrawGuiCircleDown(bmp , rad_a , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
         break;
      case ROUNDED_BTN :
         up?
         r.DrawGuiRoundedUp(bmp , rad_a , (*wcols)[FGCOL] , (*wcols)[SDCOL]):
         r.DrawGuiRoundedDown(bmp , rad_a , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
         break;
      case ELLIPSE_BTN : 
         up?
         r.DrawGuiEllipseUp(bmp , (*wcols)[FGCOL] , (*wcols)[SDCOL]):
         r.DrawGuiEllipseDown(bmp , (*wcols)[MGCOL] , (*wcols)[SDCOL]);
         break;
   }
}



void Button::DrawButtonText(BITMAP* bmp , int x , int y) {
   int tx = area.X() + x + area.W()/2;
   int ty = area.Y() + y + (area.H() - text_height(font))/2 ;
   FONT* old_font = font;
   font = text_font;
   gui_textout_ex(bmp , text.c_str() , tx , ty , (*wcols)[TXTCOL] , -1 , 1);
   font = old_font;
}



void Button::FreeClickArea() {
   if (click_area && delete_area_ptr) {
      delete click_area;
   }
   click_area = 0;
}



Button::Button(string name) :
      WidgetBase(name),
      input_group(input_key_press(KEY_NONE)),
      pointer_input(false),
      btn_type(RECTANGLE_BTN),
      action_type(TOGGLE_BTN),
      up(true),
      dn_time_left(SPRING_BTN_DURATION),
      text_font(0),
      text(""),
      spring_duration(SPRING_BTN_DURATION),
      roundness(BTN_ROUNDNESS),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      click_area(0),
      delete_area_ptr(false)
{
   SetButtonType(RECTANGLE_BTN , SPRING_BTN);
   SetFont(font);
}



Button::Button(string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont ,
               string label , const InputGroup& input , const Rectangle& position , UINT wflags) :
      WidgetBase(name , position , wflags),
      input_group(input),
      pointer_input(input.PointerInput()),
      btn_type(RECTANGLE_BTN),
      action_type(atype),
      up(true),
      dn_time_left(SPRING_BTN_DURATION),
      text_font(0),
      text(label),
      spring_duration(SPRING_BTN_DURATION),
      roundness(BTN_ROUNDNESS),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      click_area(0),
      delete_area_ptr(false)
{
   SetButtonType(shape , atype);
   SetFont(textfont);
}



WidgetMsg Button::Update (double tsec) // logic handling with animation support
{
   UINT retmsg = DIALOG_OKAY;
   if (!(WidgetBase::Flags() & ENABLED)) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_DISABLED);
   }
   
   if (action_type == SPRING_BTN) {
      if (!up && !user_activated && !focuskey_activated && !pointer_activated) {
         dn_time_left -= tsec;
         if (dn_time_left <= 0.0) {
            dn_time_left = spring_duration;
            up = true;
            retmsg |= DIALOG_REDRAW_ME;
            SetBgRedrawFlag();
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg Button::CheckInputs(int msx , int msy)// Pass it the mouse position relative to it's drawing target,
                                                // except for WidgetHandlers
                                                // Hover is taken care of by WidgetHandlers, if you need it to
                                                // do something, override the virtual SetHoverState function.
{
   
   if (!(WidgetBase::Flags() & ENABLED)) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_DISABLED);
   }
   
   UINT retmsg = DIALOG_OKAY;
   bool activated = false;
   bool released = false;

   /// Track whether the button is still being held since the last activation
   if (user_activated || focuskey_activated || pointer_activated) {
      /// Look for input releases
      if (user_activated) {
         Input click = input_group.FindClickInput();
         if (click != input_key_press(KEY_NONE)) {
            Input release(click.Source() , RELEASE , click.Value());
            if (release) {
               released = true;
            }
         }
      }
      if (focuskey_activated) {
         if (input_key_release(KEY_SPACE) || input_key_release(KEY_ENTER)) {released = true;}
      }
      if (pointer_activated) {
         if (input_mouse_release(LMB)) {released = true;}
      }
      if (released) {
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
         user_activated = false;
         focuskey_activated = false;
         pointer_activated = false;
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      } else {
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD);
      }
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
   }


   if (input_group) {
      activated = true;
      if (action_type == SPRING_BTN) {user_activated = true;}
   } else if ((WidgetBase::Flags() & HASFOCUS) && (input_key_press(KEY_SPACE) || input_key_press(KEY_ENTER))) {
      activated = true;
      if (action_type == SPRING_BTN) {focuskey_activated = true;}
   } else if (input_mouse_press(LMB) && area.Contains(msx,msy)) {
      if (click_area) {
         if (click_area->Contains(msx - area.X() , msy - area.Y())) {
            activated = true;
            if (action_type == SPRING_BTN) {pointer_activated = true;}
         }
      }
      else {
         switch (btn_type) {
            case RECTANGLE_BTN :
               activated = true;
               if (action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case CIRCLE_BTN :
               {
                  Circle c(area.X() + area.W()/2 , area.Y() + area.H()/2 , rad_a);
                  if (c.Contains(msx,msy)) {
                     activated = true;
                     if (action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
            case ROUNDED_BTN :
               // Lazy hit detection since a rounded rectangle is so close to a rectangle anyway.
               activated = true;
               if (action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case ELLIPSE_BTN :
               {
                  int xd = msx - (area.X() + area.W()/2);
                  int yd = msy - (area.Y() + area.H()/2);
                  double dist_sq = xd*xd + yd*yd;
                  double angle = atan2(yd,xd);
                  double exd = (double)rad_a*cos(angle);
                  double eyd = (double)rad_b*sin(angle);
                  double ellp_dist = exd*exd + eyd*eyd;
                  if (dist_sq <= ellp_dist) {
                     activated = true;
                     if (action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
         }
      }
   }
   
   if (activated) {// click or key press has activated the button action
      retmsg |= DIALOG_TAKE_FOCUS;
      retmsg |= DIALOG_REDRAW_ME;
      retmsg |= DIALOG_INPUT_USED;
      SetBgRedrawFlag();
      switch (action_type) {
         case SPRING_BTN :
            if (up) {
               up = false;
               QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
               if (WidgetBase::Flags() & ALLOW_CLOSE) {
                  retmsg |= DIALOG_CLOSE;
               }
            }
            break;
         case TOGGLE_BTN :
            up = !up;
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
            if (WidgetBase::Flags() & ALLOW_CLOSE) {
               retmsg |= DIALOG_CLOSE;
            }
            break;
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void Button::DisplayOn(BITMAP* bmp , int x , int y)/// DisplayOn should always draw, and always 
                                                   /// clear the NEEDS_REDRAW flag.
{
   DrawButtonRectangle(bmp , x , y);
   DrawButtonText(bmp , x , y);
   ClearRedrawFlag();
}



void Button::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetRadii();
}



void Button::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetRadii();
}



void Button::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetRadii();
}



void Button::SetInputGroup(InputGroup ig) {
   input_group = ig;
   pointer_input = input_group.PointerInput();
}



void Button::SetClickArea(AreaBase* btn_click_area , bool delete_when_done) {
   FreeClickArea();
   click_area = btn_click_area;
   delete_area_ptr = delete_when_done;
}



void Button::SetButtonType(BUTTON_SHAPE shape , BTN_ACTION_TYPE atype) {
   btn_type = shape;
   action_type = atype;
   ResetRadii();
}



void Button::SetSpringDuration(double duration) {
   if (duration < 0.0) {duration = 0.0;}
   spring_duration = duration;
}



void Button::SetButtonUpState(bool button_up) {
   if (up != button_up) {
      up = button_up;
      SetBgRedrawFlag();
   }
}



void Button::ToggleButton() {
   up = !up;
   SetBgRedrawFlag();
}



void Button::SetLabel(string label_text) {
   text = label_text;
   SetBgRedrawFlag();
}



void Button::SetRoundingPercent(double percent) {
   if (percent < 0.0) {percent = 0.0;}
   if (percent > 1.0) {percent = 1.0;}
   roundness = percent;
   int min = (area.W() < area.H())?area.W():area.H();
   min = min/2;
   if (min) {
      min -= 1;
   }
   if (btn_type == ROUNDED_BTN) {
      rad_a = (int)(roundness*(double)min);
   }
   SetBgRedrawFlag();
}



void Button::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   SetBgRedrawFlag();
}



/// --------------------------------     HoverButton     --------------------------------



HoverButton::HoverButton(std::string name) :
      Button(name)
{}



HoverButton::HoverButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
                         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags) :
      Button(name , shape , atype , textfont , label , input , position , wflags)
{}



void HoverButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & HOVER) {
      if (up) {
         Rectangle r = area;
         r.MoveBy(x,y);
         switch (btn_type) {
            case RECTANGLE_BTN : 
               r.DrawGuiRectUp(bmp , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               break;
            case CIRCLE_BTN : 
               r.DrawGuiCircleUp(bmp , rad_a , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               break;
            case ROUNDED_BTN :
               r.DrawGuiRoundedUp(bmp , rad_a , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               break;
            case ELLIPSE_BTN : 
               r.DrawGuiEllipseUp(bmp , (*wcols)[BGCOL] , (*wcols)[SDCOL]);
               break;
         }
      }
      else {
         Button::DisplayOn(bmp , x , y);
      }
   }
   else {
      Button::DisplayOn(bmp , x , y);
   }
}


   
void HoverButton::SetHoverState (bool state) {
   WidgetBase::SetHoverState(state);
   SetRedrawFlag();
}
  







