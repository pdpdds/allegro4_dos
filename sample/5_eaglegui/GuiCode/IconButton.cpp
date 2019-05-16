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


#include "Eagle/GuiCode/IconButton.hpp"

using std::string;
using std::ostream;


bool IconButton::RefreshStretchedImages() {
   bool success = true;
   if (!RefreshStretchedImage(UP_PIC)) {success = false;}
   if (!RefreshStretchedImage(DOWN_PIC)) {success = false;}
   if (!RefreshStretchedImage(HOVER_PIC)) {success = false;}
   if (!RefreshStretchedImage(DISABLED_PIC)) {success = false;}
   return success;
}



bool IconButton::RefreshStretchedImage(ICON_PIC_TYPE type) {
   BitmapHandler& pic = pics[type];
   if (pic && (icon_display_type == ICON_DISPLAY_STRETCH)) {
      return stretch_pics[type].Stretch(pic , pic.CD() , area.W() , area.H() , pic.MemType() , pic.DrawType() , stype);
   }
   return true;
}



IconButton::IconButton(string name) :
      Button(name),
      icon_display_type(ICON_DISPLAY_CENTER),
      pics(),
      stretch_pics(),
      clickarea(),
      stype(USE_ALLEGRO_STRETCH)
{}


//   Button(std::string name , BUTTON_TYPE type , BTN_ACTION_TYPE atype , FONT* textfont , 
//         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

IconButton::IconButton(std::string name , ICON_DISPLAY_TYPE dtype , Rectangle position , UINT wflags) :
      Button(name , RECTANGLE_BTN , TOGGLE_BTN , font , "" , input_key_press(KEY_NONE) , position , wflags),
      icon_display_type(dtype),
      pics(),
      stretch_pics(),
      clickarea(),
      stype(USE_ALLEGRO_STRETCH)
{}



IconButton::IconButton(string name , ICON_DISPLAY_TYPE dtype , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype ,
                       const InputGroup& input , const Rectangle& position , UINT wflags) :
      Button(name , shape , atype , font , "" , input , position , wflags),
      icon_display_type(dtype),
      pics(),
      stretch_pics(),
      clickarea(),
      stype(USE_ALLEGRO_STRETCH)
{}



WidgetMsg IconButton::CheckInputs(int msx , int msy) {
   
   if (!(flags & ENABLED)) {
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
   } else if ((flags & HASFOCUS) && (input_key_press(KEY_SPACE) || input_key_press(KEY_ENTER))) {
      activated = true;
      if (action_type == SPRING_BTN) {focuskey_activated = true;}
   } else if (input_mouse_press(LMB) && area.Contains(msx,msy)) {
      if (clickarea.size()) {
         if (clickarea.Contains(msx - area.X() , msy - area.Y())) {
             activated = true;
             if (action_type == SPRING_BTN) {pointer_activated = true;}
         }
      } else {
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
               if (flags & ALLOW_CLOSE) {
                  retmsg |= DIALOG_CLOSE;
               }
            }
            break;
         case TOGGLE_BTN :
            up = !up;
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
            if (flags & ALLOW_CLOSE) {
               retmsg |= DIALOG_CLOSE;
            }
            break;
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void IconButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      const Rectangle oldclip = GetClippingRectangle(bmp);
      r.SetClipRect(bmp);
      switch (icon_display_type) {
         case ICON_DISPLAY_CENTER :
            if (flags & ENABLED) {
               if (flags & HOVER) {
                  if (Button::up) {
                     if (pics[HOVER_PIC]) {
                        pics[HOVER_PIC].CenteredDrawTo(bmp , r);
                     } else {
                        pics[UP_PIC].CenteredDrawTo(bmp , r);
                     }
                  } else {
                     pics[DOWN_PIC].CenteredDrawTo(bmp , r);
                  }
               } else {
                  if (Button::up) {
                     pics[UP_PIC].CenteredDrawTo(bmp , r);
                  } else {
                     pics[DOWN_PIC].CenteredDrawTo(bmp , r);
                  }
               }
            } else {
               if (pics[DISABLED_PIC]) {
                  pics[DISABLED_PIC].CenteredDrawTo(bmp , r);
               } else {
                  if (Button::up) {
                     pics[UP_PIC].CenteredDrawTo(bmp , r);
                  } else {
                     pics[DOWN_PIC].CenteredDrawTo(bmp , r);
                  }
               }
            }
            break;
         case ICON_DISPLAY_STRETCH :
            if (flags & ENABLED) {
               if (flags & HOVER) {
                  if (Button::up) {
                     if (stretch_pics[HOVER_PIC]) {
                        stretch_pics[HOVER_PIC].DrawTo(bmp , r.X() , r.Y());
                     } else {
                        stretch_pics[UP_PIC].DrawTo(bmp , r.X() , r.Y());
                     }
                  } else {
                     stretch_pics[DOWN_PIC].DrawTo(bmp , r.X() , r.Y());
                  }
               } else {
                  if (Button::up) {
                     stretch_pics[UP_PIC].DrawTo(bmp , r.X() , r.Y());
                  } else {
                     stretch_pics[DOWN_PIC].DrawTo(bmp , r.X() , r.Y());
                  }
               }
            } else {
               if (stretch_pics[DISABLED_PIC]) {
                  stretch_pics[DISABLED_PIC].DrawTo(bmp , r.X() , r.Y());
               } else {
                  if (Button::up) {
                     stretch_pics[UP_PIC].DrawTo(bmp , r.X() , r.Y());
                  } else {
                     stretch_pics[DOWN_PIC].DrawTo(bmp , r.X() , r.Y());
                  }
               }
            }
            break;
      }
      oldclip.SetClipRect(bmp);
   }
   ClearRedrawFlag();
}



void IconButton::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   RefreshStretchedImages();
}



void IconButton::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   RefreshStretchedImages();
}



void IconButton::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   RefreshStretchedImages();
}



void IconButton::SetHoverState(bool state) {
   switch (icon_display_type) {
      case ICON_DISPLAY_CENTER :
         if (pics[HOVER_PIC]) {
            SetRedrawFlag();
         }
         break;
      case ICON_DISPLAY_STRETCH :
         if (stretch_pics[HOVER_PIC]) {
            SetRedrawFlag();
         }
         break;
   }
   WidgetBase::SetHoverState(state);
}



bool IconButton::LoadImages(const char* up_pic_file , const char* down_pic_file ,
                            const char* hover_pic_file , const char* disabled_pic_file ,
                            BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (up_pic_file) {
      if (!pics[UP_PIC].LoadImage(up_pic_file , NULL , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[UP_PIC].Free();
      stretch_pics[UP_PIC].Free();
   }
   if (down_pic_file) {
      if (!pics[DOWN_PIC].LoadImage(down_pic_file , NULL , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[DOWN_PIC].Free();
      stretch_pics[DOWN_PIC].Free();
   }
   if (hover_pic_file) {
      if (!pics[HOVER_PIC].LoadImage(hover_pic_file , NULL , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[HOVER_PIC].Free();
      stretch_pics[HOVER_PIC].Free();
   }
   if (disabled_pic_file) {
      if (!pics[DISABLED_PIC].LoadImage(disabled_pic_file , NULL , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[DISABLED_PIC].Free();
      stretch_pics[DISABLED_PIC].Free();
   }
   RefreshStretchedImages();
   SetBgRedrawFlag();
   return success;
}



bool IconButton::LoadImage(ICON_PIC_TYPE icon_pic_type , const char* pic_file , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (pic_file) {
      if (!pics[icon_pic_type].LoadImage(pic_file , NULL , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[icon_pic_type].Free();
      stretch_pics[icon_pic_type].Free();
   }
   RefreshStretchedImage(icon_pic_type);
   SetBgRedrawFlag();
   return success;
}



bool IconButton::SetImages(BITMAP* up_pic , BITMAP* down_pic , BITMAP* hover_pic , BITMAP* disabled_pic ,
                           BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (up_pic) {
      if (!pics[UP_PIC].Clone(up_pic , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[UP_PIC].Free();
      stretch_pics[UP_PIC].Free();
   }
   if (down_pic) {
      if (!pics[DOWN_PIC].Clone(down_pic , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[DOWN_PIC].Free();
      stretch_pics[DOWN_PIC].Free();
   }
   if (hover_pic) {
      if (!pics[HOVER_PIC].Clone(hover_pic , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[HOVER_PIC].Free();
      stretch_pics[HOVER_PIC].Free();
   }
   if (disabled_pic) {
      if (!pics[DISABLED_PIC].Clone(disabled_pic , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[DISABLED_PIC].Free();
      stretch_pics[DISABLED_PIC].Free();
   }
   RefreshStretchedImages();
   SetBgRedrawFlag();
   return success;
}



bool IconButton::SetImage(ICON_PIC_TYPE icon_pic_type , BITMAP* bmp , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   bool success = true;
   if (bmp) {
      if (!pics[icon_pic_type].Clone(bmp , bmp_mem_type , bmp_draw_type)) {success = false;}
   } else {
      pics[icon_pic_type].Free();
      stretch_pics[icon_pic_type].Free();
   }
   RefreshStretchedImage(icon_pic_type);
   SetBgRedrawFlag();
   return success;
}



bool IconButton::SetImage(ICON_PIC_TYPE icon_pic_type , const BitmapHandler& bh) {
   bool success = pics[icon_pic_type].Clone(bh);
   RefreshStretchedImage(icon_pic_type);
   SetBgRedrawFlag();
   return success;
}



void IconButton::FreeImages() {
   pics[UP_PIC].Free();
   pics[DOWN_PIC].Free();
   pics[HOVER_PIC].Free();
   pics[DISABLED_PIC].Free();
   stretch_pics[UP_PIC].Free();
   stretch_pics[DOWN_PIC].Free();
   stretch_pics[HOVER_PIC].Free();
   stretch_pics[DISABLED_PIC].Free();
}



void IconButton::SetBlenderType(ALPHA_BLENDER_TYPE type) {
   pics[UP_PIC].SetBlenderType(type);
   pics[DOWN_PIC].SetBlenderType(type);
   pics[HOVER_PIC].SetBlenderType(type);
   pics[DISABLED_PIC].SetBlenderType(type);
   stretch_pics[UP_PIC].SetBlenderType(type);
   stretch_pics[DOWN_PIC].SetBlenderType(type);
   stretch_pics[HOVER_PIC].SetBlenderType(type);
   stretch_pics[DISABLED_PIC].SetBlenderType(type);
   SetBgRedrawFlag();
}



void IconButton::SetStretchType(STRETCH_TYPE type) {
   if (stype != type) {
      RefreshStretchedImages();
      stype = type;
   }
}






