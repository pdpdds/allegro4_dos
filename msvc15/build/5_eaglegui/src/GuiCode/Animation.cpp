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


#include "Eagle/Area.hpp"
#include "Eagle/GuiCode/Animation.hpp"
#include "Eagle/Blitting.hpp"

#include <vector>
using std::vector;

#include <cmath>

using std::map;
using std::list;
using std::string;
using std::endl;


typedef map<string , AnimationBase*> SABMAP;
typedef SABMAP::iterator SABMIT;



const unsigned int TOPIC_ANIMATION = NextFreeTopicId();

const unsigned int TOPIC_STATE_ANIMATION = NextFreeTopicId();



void AnimationBase::SetColorHSV(int* color_array , double hue , double huespeed , double sat , double satspeed,
                  double val , double valspeed , int frame_start , int frame_stop) {
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop >= 1) && (frame_stop <= num_frames));
   ASSERT(frames_per_sec != 0.0);
   double t = 0.0;
   double spf = 1.0/frames_per_sec;// seconds per frame
   double h,s,v;
   int r,g,b;
   int col;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      h = hue + huespeed*t;
      while(h >= 360.0) {h -= 360.0;}
      while(h < 0.0) {h += 360.0;}
      s = sat + satspeed*t;
      if (s > 1.0) {s = 1.0;}
      if (s < 0.0) {s = 0.0;}
      v = val + valspeed*t;
      if (v > 1.0) {v = 1.0;}
      if (v < 0.0) {v = 0.0;}
      hsv_to_rgb(h,s,v,&r,&g,&b);
      if ((r == 255) && (g == 0) && (b == 255)) {b = 254;}// fudge to prevent use of the mask color
      col = makecol(r,g,b);
      color_array[i] = col;
      t += spf;
   }
}



void AnimationBase::SetColorRGB(int* color_array , int start_color , int finish_color , int frame_start , int frame_stop) {
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop >= 1) && (frame_stop <= num_frames));
   int sr = getr(start_color);
   int sg = getg(start_color);
   int sb = getb(start_color);
   int fr = getr(finish_color);
   int fg = getg(finish_color);
   int fb = getb(finish_color);
   // Next two lines are a fudge to prevent use of the mask color
   if ((sr == 255) && (sg == 0) && (sb == 255)) {sb = 254;}
   if ((fr == 255) && (fg == 0) && (fb == 255)) {fb = 254;}
   double nframes = frame_stop - frame_start - 1;
   if (nframes == 0.0) {nframes = 1.0;}
   double dr = ((double)(fr - sr))/nframes;
   double dg = ((double)(fg - sg))/nframes;
   double db = ((double)(fb - sb))/nframes;
   double r = sr;
   double g = sg;
   double b = sb;
   int col;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      col = makecol((int)r , (int)g , (int)b);
      color_array[i] = col;
      r += dr;
      g += dg;
      b += db;
   }
}



AnimationBase::AnimationBase(ANIMATION_TYPE atype) :
      frames(0),
      free_frame(0),
      xposition(0),
      yposition(0),
      rotation(0),
      scale(0),
      color1(0),
      color2(0),
      base_x(0),
      base_y(0),
      anim_type(atype),
      duration(0.0),
      frames_per_sec(0.0),
      frametime(0.0),
      time_factor(1.0),
      num_frames(0),
      frame_num(-1),
      loop_num(-1),
      loop_stop_num(-1),
      num_trails(0),
      frames_per_trail(1),
      paused(false)
{}



void AnimationBase::Free() {
   vector<BITMAP*> freed_list;
   if (frames) {
      if (free_frame) {
         for (int i = 0 ; i < num_frames ; ++i) {
            if (frames[i] && free_frame[i]) {
               bool freed = false;
               for (unsigned int j = 0 ; j < freed_list.size() ; ++j) {
                  if (freed_list[j] == frames[i]) {
                     freed = true;
                  }
               }
               if (!freed) {
                  destroy_bitmap(frames[i]);
                  freed_list.push_back(frames[i]);
               }
               frames[i] = 0;
            }
         }
      }
      delete [] frames;
      frames = 0;
   }
   if (free_frame) {
      delete [] free_frame;
      free_frame = 0;
   }
   if (xposition) {
      delete [] xposition;
      xposition = 0;
   }
   if (yposition) {
      delete [] yposition;
      yposition = 0;
   }
   if (rotation) {
      delete [] rotation;
      rotation = 0;
   }
   if (scale) {
      delete [] scale;
      scale = 0;
   }
   if (color1) {
      delete [] color1;
      color1 = 0;
   }
   if (color2) {
      delete [] color2;
      color2 = 0;
   }
   num_frames = 0;
   frames_per_sec = 0.0;
   frame_num = 0;
   loop_num = 0;
}



int AnimationBase::AdvanceFrameTime(double delta_time) {
   if (paused) {return loop_num;}
   return SetFrameTime(frametime + time_factor*delta_time);
}



int AnimationBase::SetFrameTime(double frame_time) {
   frametime = frame_time;
   double short_frame_time = frametime;
   double num_loops = 0.0;
   switch (anim_type) {
      case FORWARD_ONCE :
         if ((frametime < 0.0) || (frametime >= duration)) {
            frame_num = -1;
            loop_num = -1;
         } else {
            frame_num = (int)(frametime*frames_per_sec);
            loop_num = 0;
         }
         break;
      case BACKWARD_ONCE :
         if ((frametime < 0.0) || (frametime >= duration)) {
            frame_num = -1;
            loop_num = -1;
         } else {
            frame_num = (num_frames - 1) - (int)(frametime*frames_per_sec);
            loop_num = 0;
         }
         break;
      case FORWARD_AND_BACK_ONCE :
         if ((frametime < 0.0) || (frametime >= 2.0*duration)) {
            frame_num = -1;
            loop_num = -1;
         } else {
            if (frametime < duration) {
               frame_num = (int)(frametime*frames_per_sec);
            } else {
               frame_num = (num_frames - 1) - (int)((frametime-duration)*frames_per_sec);
            }
            loop_num = 0;
         }
         break;
      case FORWARD_REPEAT :
         loop_num = 0;
         if (frametime < 0.0) {
            frame_num = -1;
            loop_num = -1;
         } else {
            if (frametime >= duration) {
               num_loops = frametime/duration;
               loop_num = (int)num_loops;
               short_frame_time = frametime - duration*(double)(loop_num);
            }
            frame_num = (int)(short_frame_time*frames_per_sec);
         }
         break;
      case BACKWARD_REPEAT :
         loop_num = 0;
         if (frametime < 0.0) {
            frame_num = -1;
            loop_num = -1;
         } else {
            if (frametime >= duration) {
               num_loops = frametime/duration;
               loop_num = (int)num_loops;
               short_frame_time = frametime - duration*(double)(loop_num);
            }
            frame_num = (num_frames - 1) - (int)(short_frame_time*frames_per_sec);
         }
         break;
      case FORWARD_AND_BACK_REPEAT :
         loop_num = 0;
         if (frametime < 0.0) {
            frame_num = -1;
            loop_num = -1;
         } else {
            if (frametime >= 2.0*duration) {
               num_loops = frametime/(2.0*duration);
               loop_num = (int)num_loops;
               short_frame_time = frametime - (2.0*duration)*(double)loop_num;
            }
            if (short_frame_time < duration) {
               frame_num = (int)(short_frame_time*frames_per_sec);
            } else {
               frame_num = (num_frames - 1) - (int)((short_frame_time-duration)*frames_per_sec);
            }
         }
         break;
   }
   if (loop_stop_num != -1) {
      if (loop_num > loop_stop_num) {
         frame_num = -1;
         loop_num = -1;
      }
   }
   return loop_num;
}



int AnimationBase::SetFrameNumber(int frame_number) {
   ASSERT(frames_per_sec != 0.0);
   frametime = (double)frame_number/frames_per_sec;
   frame_num = frame_number;
   loop_num = 0;
   if ((frame_num < 0) || (frame_num >= num_frames)) {
      frame_num = -1;
      loop_num = -1;
   }
   return loop_num;
}



double AnimationBase::TimeBetweenFrames(int frame_start , int frame_stop) {
   double nframes = (double)(frame_stop - frame_start);
   return nframes*SecondsPerFrame();
}



void AnimationBase::SetTimeFactor(double factor) {
   time_factor = factor;
}



void AnimationBase::SetLoopStop(int loop_stop) {
   if (loop_stop < -1) {loop_stop = -1;}
   loop_stop_num = loop_stop;
   SetFrameTime(frametime);
}



BITMAP* AnimationBase::GetBitmap(int frame_number) {
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return frames[frame_number];
}



int AnimationBase::GetXpos(int frame_number) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return xposition[frame_number];
}



int AnimationBase::GetYpos(int frame_number) {
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return yposition[frame_number];
}



float AnimationBase::GetRotation(int frame_number) {
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return rotation[frame_number];
}



float AnimationBase::GetScale(int frame_number) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return scale[frame_number];
}



int AnimationBase::GetColor1(int frame_number) {
   ASSERT(color1);// make sure the color array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return color1[frame_number];
}



int AnimationBase::GetColor2(int frame_number) {
   ASSERT(color2);// make sure the color array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   return color2[frame_number];
}



void AnimationBase::SetNumTrails(int numtrails , int num_frames_per_trail) {
   if (numtrails < 0) {numtrails = 0;}
   if (num_frames_per_trail < 1) {num_frames_per_trail = 1;}
   num_trails = numtrails;
   frames_per_trail = num_frames_per_trail;
}



void AnimationBase::SetBitmap(BITMAP* bmp , bool free_bmp) {
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT(free_frame);// make sure the free_frame array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = bmp;
      free_frame[i] = free_bmp;
   }
}



void AnimationBase::SetXpos(int x) {
   ASSERT(xposition);// make sure the xposition array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = x;
   }
}



void AnimationBase::SetYpos(int y) {
   ASSERT(yposition);// make sure the yposition array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      yposition[i] = y;
   }
}



void AnimationBase::SetPos(int x , int y) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = x;
      yposition[i] = y;
   }
}



void AnimationBase::SetRotation(double Rotation) {
   ASSERT(rotation);// make sure the rotation array is allocated
   double extra = Rotation/360.0;
   Rotation -= 360.0*(double)(int)extra;
   if (Rotation < 0.0) {Rotation += 360.0;}
   for (int i = 0 ; i < num_frames ; ++i) {
      rotation[i] = Rotation;
   }
}



void AnimationBase::SetScale(double Scale) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(Scale > 0.0);
   for (int i = 0 ; i < num_frames ; ++i) {
      scale[i] = Scale;
   }
}



void AnimationBase::SetColor1(int Color) {
   ASSERT(color1);// make sure the color array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      color1[i] = Color;
   }
}



void AnimationBase::SetColor2(int Color) {
   ASSERT(color2);// make sure the color array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      color2[i] = Color;
   }
}



void AnimationBase::SetBitmap(BITMAP* bmp , bool free_bmp , int frame_number) {
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT(free_frame);// make sure the free_frame array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   frames[frame_number] = bmp;
   free_frame[frame_number] = free_bmp;
}



void AnimationBase::SetXpos(int x , int frame_number) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   xposition[frame_number] = x;
}



void AnimationBase::SetYpos(int y , int frame_number) {
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   yposition[frame_number] = y;
}



void AnimationBase::SetPos(int x , int y , int frame_number) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   xposition[frame_number] = x;
   yposition[frame_number] = y;
}



void AnimationBase::SetRotation(double Rotation , int frame_number) {
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   double extra = Rotation/360.0;
   Rotation -= 360.0*(double)(int)extra;
   if (Rotation < 0.0) {Rotation += 360.0;}
   rotation[frame_number] = Rotation;
}



void AnimationBase::SetScale(double Scale , int frame_number) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(Scale > 0.0);
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   scale[frame_number] = Scale;
}



void AnimationBase::SetColor1(int Color , int frame_number) {
   ASSERT(color1);// make sure the color array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   color1[frame_number] = Color;
}



void AnimationBase::SetColor2(int Color , int frame_number) {
   ASSERT(color2);// make sure the color array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   color2[frame_number] = Color;
}



void AnimationBase::SetBitmap(BITMAP* bmp , bool free_bmp , int frame_start , int frame_stop) {
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT(free_frame);// make sure the free_frame array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      frames[i] = bmp;
      free_frame[i] = free_bmp;
   }
}



void AnimationBase::SetXpos(int x , int frame_start , int frame_stop) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      xposition[i] = x;
   }
}



void AnimationBase::SetYpos(int y , int frame_start , int frame_stop) {
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      yposition[i] = y;
   }
}



void AnimationBase::SetPos(int x , int y , int frame_start , int frame_stop) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      xposition[i] = x;
      yposition[i] = y;
   }
}



void AnimationBase::SetRotation(double Rotation , int frame_start , int frame_stop) {
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   double extra = Rotation/360.0;
   Rotation -= 360.0*(double)(int)extra;
   if (Rotation < 0.0) {Rotation += 360.0;}
   for (int i = frame_start ; i < frame_stop ; ++i) {
      rotation[i] = Rotation;
   }
}



void AnimationBase::SetScale(double Scale , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(Scale > 0.0);
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      scale[i] = Scale;
   }
}



void AnimationBase::SetColor1(int Color , int frame_start , int frame_stop) {
   ASSERT(color1);// make sure the color array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      color1[i] = Color;
   }
}



void AnimationBase::SetColor2(int Color , int frame_start , int frame_stop) {
   ASSERT(color2);// make sure the color array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   for (int i = frame_start ; i < frame_stop ; ++i) {
      color2[i] = Color;
   }
}



void AnimationBase::SetBasePos(int xpos , int ypos) {
   base_x = xpos;
   base_y = ypos;
}



void AnimationBase::SetXPos(double xstart , double xspeed , double xaccel , int frame_start , int frame_stop) {
   // x = xstart + xspeed*t + (xaccel/2)*t^2
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop >= 1) && (frame_stop <= num_frames));
   ASSERT(frames_per_sec != 0.0);
   ASSERT(xposition);// make sure the xposition array is allocated
   double t = 0.0;
   double spf = 1.0/frames_per_sec;// seconds per frame
   double x;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      x = xstart + xspeed*t + (xaccel/2.0)*t*t;
      xposition[i] = (int)x;
      t += spf;
   }
}



void AnimationBase::SetYPos(double ystart , double yspeed , double yaccel , int frame_start , int frame_stop) {
   // y = ystart + yspeed*t + (yaccel/2)*t^2
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop >= 1) && (frame_stop <= num_frames));
   ASSERT(frames_per_sec != 0.0);
   ASSERT(yposition);// make sure the yposition array is allocated
   double t = 0.0;
   double spf = 1.0/frames_per_sec;// seconds per frame
   double y;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      y = ystart + yspeed*t + (yaccel/2.0)*t*t;
      yposition[i] = (int)y;
      t += spf;
   }
}



void AnimationBase::SetRotation(double rdeg , double rspeed , double raccel , int frame_start , int frame_stop) {
   // r = rdeg + rspeed*t + (raccel/2)*t^2
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop >= 1) && (frame_stop <= num_frames));
   ASSERT(frames_per_sec != 0.0);
   ASSERT(rotation);// make sure the rotation array is allocated
   double t = 0.0;
   double spf = 1.0/frames_per_sec;// seconds per frame
   double r;
   double extra;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      r = rdeg + rspeed*t + (raccel/2.0)*t*t;
      // r = 0 + 900*4.0 + -225*16 = 3600 - 2250 - 1200 - 150 = 0
      // rv = 900 + -450*4 = -1800
      extra = r/360.0;
      r -= 360.0*(double)(int)extra;
      if (r < 0.0) {r += 360.0;}
      rotation[i] = (int)r;
      t += spf;
   }
}



void AnimationBase::SetColor1HSV(double hue , double huespeed , double sat , double satspeed ,
                  double val , double valspeed , int frame_start , int frame_stop) {
   ASSERT(color1);// make sure the color1 array is allocated
   SetColorHSV(color1 , hue , huespeed , sat , satspeed , val , valspeed , frame_start , frame_stop);
}



void AnimationBase::SetColor2HSV(double hue , double huespeed , double sat , double satspeed ,
                  double val , double valspeed , int frame_start , int frame_stop) {
   ASSERT(color2);// make sure the color2 array is allocated
   SetColorHSV(color2 , hue , huespeed , sat , satspeed , val , valspeed , frame_start , frame_stop);
}



void AnimationBase::SetColor1RGB(int start_color , int finish_color , int frame_start , int frame_stop) {
   ASSERT(color1);
   SetColorRGB(color1 , start_color , finish_color , frame_start , frame_stop);
}



void AnimationBase::SetColor2RGB(int start_color , int finish_color , int frame_start , int frame_stop) {
   ASSERT(color2);
   SetColorRGB(color2 , start_color , finish_color , frame_start , frame_stop);
}



/// ----------------------------- Text Animation class ---------------------------------------



TextAnimation::TextAnimation(const TextAnimation& ta) :
      AnimationBase(ta.anim_type),
      WidgetBase(ta.Name() , NULL , ta.Area()),
      text_bh(),
      text_str(),
      text_font(0),
      text_w(0),
      text_h(0),
      gradient_type(ta.gradient_type),
      using_color_font(ta.using_color_font)
{
   Reallocate(ta.num_frames , ta.duration);
   SetText(ta.text_str , ta.text_font , ta.using_color_font);
   SetLoopStop(ta.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = ta.xposition[i];
      yposition[i] = ta.yposition[i];
      rotation[i] = ta.rotation[i];
      scale[i] = ta.scale[i];
      color1[i] = ta.color1[i];
      color2[i] = ta.color2[i];
   }
}



TextAnimation::TextAnimation(string name , string text , FONT* textfont , bool use_color_font ,
                              TEXT_GRADIENT_TYPE gtype , ANIMATION_TYPE atype , int frame_count , double play_time , int loop_stop) :
      AnimationBase(atype),
      WidgetBase(name),
      text_bh(),
      text_str(),
      text_font(0),
      text_w(0),
      text_h(0),
      gradient_type(gtype),
      using_color_font(use_color_font)
{
   Reallocate(frame_count , play_time);
   SetText(text , textfont , use_color_font);
   SetLoopStop(loop_stop);
}



TextAnimation& TextAnimation::operator=(const TextAnimation& ta) {
   /// TODO , IMPORTANT : Figure out how to assign the base class parts like WidgetBase
   Reallocate(ta.num_frames , ta.duration);
   SetText(ta.text_str , ta.text_font , ta.using_color_font);
   SetLoopStop(ta.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = ta.xposition[i];
      yposition[i] = ta.yposition[i];
      rotation[i] = ta.rotation[i];
      scale[i] = ta.scale[i];
      color1[i] = ta.color1[i];
      color2[i] = ta.color2[i];
   }
   return *this;
}



void TextAnimation::Reallocate(int number_of_frames , double total_play_time) {
   ASSERT(total_play_time > 0.0);
   ASSERT(number_of_frames > 0);
   AnimationBase::Free();
   num_frames = number_of_frames;
   duration = total_play_time;
   frames_per_sec = (double)num_frames / duration;
   SetFrameTime(0.0);
   
//   frames = new BITMAP*[num_frames];
//   free_frame = new bool[num_frames];
   xposition = new int[num_frames];
   yposition = new int[num_frames];
   rotation = new double[num_frames];
   scale = new double[num_frames];
   color1 = new int[num_frames];
   color2 = new int[num_frames];
   
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = 0;
      yposition[i] = 0;
      rotation[i] = 0.0;
      scale[i] = 1.0;
      color1[i] = 0;
      color2[i] = 0;
   }
}



void TextAnimation::Free() {
   AnimationBase::Free();
   text_bh.Free();
}



void TextAnimation::CalcSize() {
   int lxmax = 100000;
   int rxmax = -100000;
   int tymax = 100000;
   int bymax = -100000;
   int wmax,hmax;
   double dw = (text_w + 1)/2;
   double dh = (text_h + 1)/2;
   double dlen;
   const double sa[4] = {
      atan2(dh,dw),
      atan2(dh,-dw),
      atan2(-dh,dw),
      atan2(-dh,-dw)
   };
   double a[4];
   double radangle;
   double cx,cy;
   double dx,dy;
   int x,y;
   for (int i = 0 ; i < num_frames ; ++i) {
      cx = (double)(xposition[i] + base_x);
      cy = (double)(yposition[i] + base_y);
      dw = scale[i]*(double)((text_w + 1)/2);
      dh = scale[i]*(double)((text_h + 1)/2);
      dlen = sqrt(dw*dw + dh*dh);
      radangle = rotation[i]*(M_PI/180.0);
      for (int j = 0 ; j < 4 ; ++j) {
         a[j] = radangle + sa[j];
         dx = cx + dlen*cos(a[j]);
         dy = cy + dlen*sin(a[j]);
         x = (int)dx;
         y = (int)dy;
         if (x - 1 < lxmax) {lxmax = x - 1;}
         if (x + 1 > rxmax) {rxmax = x + 1;}
         if (y - 1 < tymax) {tymax = y - 1;}
         if (y + 1 > bymax) {bymax = y + 1;}
      }
   }
   wmax = rxmax - lxmax + 1;
   hmax = bymax - tymax + 1;
   WidgetBase::SetArea(lxmax,tymax,wmax,hmax);
}



void TextAnimation::DrawOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      if ((num_frames > 0) && (BITMAP*)text_bh && (frame_num != -1)) {
         if (using_color_font) {
            textout_ex(text_bh , text_font , text_str.c_str() , 0 , 0 , -1 , -1);
         } else {
            textout_ex(text_bh , text_font , text_str.c_str() , 0 , 0 , color1[frame_num] , -1);
         }
         Rectangle r(0,0,text_w,text_h);
         switch (gradient_type) {
            case TEXT_GRADIENT_NONE : break;
            case TEXT_GRADIENT_LR :
               r.DrawMaskedGradientLtoR(text_bh , color1[frame_num] , color2[frame_num]);
               break;
            case TEXT_GRADIENT_TB :
               r.DrawMaskedGradientTtoB(text_bh , color1[frame_num] , color2[frame_num]);
               break;
            case TEXT_GRADIENT_OM :
               r.DrawMaskedGradientOtoM(text_bh , color1[frame_num] , color2[frame_num]);
               break;
         }
         for (int i = frame_num - num_trails*frames_per_trail ; i <= frame_num ; i += frames_per_trail) {
            if (i >= 0) {
               fixed angle = ftofix(rotation[i]*(256.0/360.0));
      //void pivot_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, fixed angle, fixed scale);
               pivot_scaled_sprite(bmp , text_bh , x + base_x + xposition[i] , y + base_y + yposition[i] ,
                                    text_w/2 , text_h/2 , angle , ftofix(scale[i]));
            }
         }
      }
   }
}



void TextAnimation::LoggedDrawOn(BITMAP* bmp , int x , int y) {
   OutputLog() << "LoggedDrawOn(bmp = " << bmp << " , x = " << x << " , y = " << y << ")" << endl;
   Indenter indent(1,3);
   OutputLog() << indent << "VISIBLE is " << ((flags & VISIBLE)?"true":"false") << endl;
   if (flags & VISIBLE) {
      OutputLog() << indent << "numframes = " << num_frames << " , (BITMAP*)text_bh = " << (BITMAP*)text_bh;
      OutputLog() << " , frame_num = " << frame_num << endl;
      if ((num_frames > 0) && (BITMAP*)text_bh && (frame_num != -1)) {
//         OutputLog() << indent << "using_color_font is " << (using_color_font?"true":"false") << endl;
         if (using_color_font) {
            OutputLog() << indent << "Using color font , text_font = " << text_font << " , text_str.c_str() = " << text_str.c_str() << endl;
            textout_ex(text_bh , text_font , text_str.c_str() , 0 , 0 , -1 , -1);
         } else {
            OutputLog() << indent << "NOT Using color font , text_font = " << text_font << " , text_str.c_str() = " << text_str.c_str();
            OutputLog() << " , frame_num = " << frame_num << " , color1[frame_num] = " << color1[frame_num] << endl;
            Color c1(color1[frame_num]);
            OutputLog() << indent << c1 << endl;
            textout_ex(text_bh , text_font , text_str.c_str() , 0 , 0 , color1[frame_num] , -1);
         }
         Rectangle r(0,0,text_w,text_h);
         OutputLog() << indent << "text_w = " << text_w << " , text_h = " << text_h << " , Rectangle r = " << r << endl;
         switch (gradient_type) {
            OutputLog() << indent << "Gradient Type is ";
            case TEXT_GRADIENT_NONE :
               OutputLog() << "TEXT_GRADIENT_NONE" << endl;
               break;
            case TEXT_GRADIENT_LR :
               OutputLog() << "TEXT_GRADIENT_LR" << endl;
               r.DrawMaskedGradientLtoR(text_bh , color1[frame_num] , color2[frame_num]);
               break;
            case TEXT_GRADIENT_TB :
               OutputLog() << "TEXT_GRADIENT_TB" << endl;
               r.DrawMaskedGradientTtoB(text_bh , color1[frame_num] , color2[frame_num]);
               break;
            case TEXT_GRADIENT_OM :
               OutputLog() << "TEXT_GRADIENT_OM" << endl;
               r.DrawMaskedGradientOtoM(text_bh , color1[frame_num] , color2[frame_num]);
               break;
         }
         OutputLog() << indent << "Starting i = " << frame_num - num_trails*frames_per_trail;
         OutputLog() << " , Ending i = " << frame_num << " , num_trails = " << num_trails;
         OutputLog() << " , frames_per_trail = " << frames_per_trail << endl;
         Indenter indent2(2,3);
         for (int i = frame_num - num_trails*frames_per_trail ; i <= frame_num ; i += frames_per_trail) {
            OutputLog() << indent << "i = " << i << endl;
            if (i >= 0) {
               OutputLog() << indent2 << "xposition[" << i << "] = " << xposition[i] << endl;
               OutputLog() << indent2 << "yposition[" << i << "] = " << yposition[i] << endl;
               OutputLog() << indent2 << "rotation[" << i << "] = " << rotation[i] << " degrees" << endl;
               OutputLog() << indent2 << "scale[" << i << "] = " << scale[i] << endl;
               Color col1(color1[i]);
               Color col2(color2[i]);
               OutputLog() << indent2 << "color1[" << i << "] = " << color1[i] << " , " << col1 << endl;
               OutputLog() << indent2 << "color2[" << i << "] = " << color2[i] << " , " << col2 << endl;
               fixed angle = ftofix(rotation[i]*(256.0/360.0));
      //void pivot_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, fixed angle, fixed scale);
               pivot_scaled_sprite(bmp , text_bh , x + base_x + xposition[i] , y + base_y + yposition[i] ,
                                    text_w/2 , text_h/2 , angle , ftofix(scale[i]));
            }
         }
      }
   }
}



int TextAnimation::SetFrameTime(double frame_time) {
   int cf = frame_num;
   int ln = loop_num;
   int ret = AnimationBase::SetFrameTime(frame_time);
   if (cf != frame_num) {
      SetBgRedrawFlag();
   }
   if (ln != ret) {
      QueueUserMessage(this , TOPIC_ANIMATION , ret);
   }
   return ret;
}



int TextAnimation::SetFrameNumber(int frame_number) {
   int fn = frame_num;
   int ret = AnimationBase::SetFrameNumber(frame_number);
   if (frame_num != fn) {
      SetBgRedrawFlag();
   }
   return ret;
}



void TextAnimation::SetNumTrails(int numtrails , int num_frames_per_trail) {
   AnimationBase::SetNumTrails(numtrails , num_frames_per_trail);
   SetBgRedrawFlag();
}



void TextAnimation::SetBasePos(int xpos , int ypos) {
   Rectangle r = Area();
   r.MoveBy(xpos - base_x , ypos - base_y);
   WidgetBase::SetArea(r);
   AnimationBase::SetBasePos(xpos,ypos);
}



WidgetMsg TextAnimation::Update (double tsec) {
   if (flags & ENABLED) {
      AdvanceFrameTime(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg TextAnimation::CheckInputs(int msx , int msy) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void TextAnimation::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      DrawOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



bool TextAnimation::Init() {
   CalcSize();
   return true;
}



bool TextAnimation::Ready() {
   if (!text_bh) {return false;}
   if (!xposition) {return false;}
   if (!yposition) {return false;}
   if (!rotation) {return false;}
   if (!scale) {return false;}
   if (!color1) {return false;}
   if (!color2) {return false;}
   return true;
}



bool TextAnimation::Shutdown() {
   return true;
}



int TextAnimation::GetWidth(int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   return (int)(scale[frame_number]*(double)text_w);
}



int TextAnimation::GetHeight(int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   return (int)(scale[frame_number]*(double)text_h);
}



void TextAnimation::SetWidth(int width) {
   ASSERT(scale);// make sure the scale array is allocated
   double Scale = (double)width/(double)text_w;
   for (int i = 0 ; i < num_frames ; ++i) {
      scale[i] = Scale;
   }
}



void TextAnimation::SetHeight(int height) {
   ASSERT(scale);// make sure the scale array is allocated
   double Scale = (double)height/(double)text_h;
   for (int i = 0 ; i < num_frames ; ++i) {
      scale[i] = Scale;
   }
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   const double tw = (double)text_w;
   const double th = (double)text_h;
   const double dw = (double)((text_w + 1)/2);
   const double dh = (double)((text_h + 1)/2);
   const double dlen = sqrt(dw*dw + dh*dh);
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   double ra = 0.0;
   if (rlen != 0.0) {ra = atan2(rh,rw);}
   const double sa[4] = {
      atan2(-th,-tw),// top left
      atan2(-th,tw),// top right
      atan2(th,tw),// bottom right
      atan2(th,-tw)// bottom left
   };
   const double arad = ra - sa[dtype];
   double adeg = arad*(180.0/M_PI);
   if (adeg < 0.0) {adeg += 360.0;}
   const double Scale = rlen/dlen;
   
   for (int i = 0 ; i < num_frames ; ++i) {
      xposition[i] = centerx;
      yposition[i] = centery;
      rotation[i] = adeg;
      scale[i] = Scale;
   }
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2);
}



void TextAnimation::SetWidth(int width , int frame_number) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   double Scale = (double)width/(double)text_w;
   scale[frame_number] = Scale;
}



void TextAnimation::SetHeight(int height , int frame_number) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   double Scale = (double)height/(double)text_h;
   scale[frame_number] = Scale;
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_number) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   const double tw = (double)text_w;
   const double th = (double)text_h;
   const double dw = (double)((text_w + 1)/2);
   const double dh = (double)((text_h + 1)/2);
   const double dlen = sqrt(dw*dw + dh*dh);
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   double ra = 0.0;
   if (rlen != 0.0) {ra = atan2(rh,rw);}
   const double sa[4] = {
      atan2(-th,-tw),// top left
      atan2(-th,tw),// top right
      atan2(th,tw),// bottom right
      atan2(th,-tw)// bottom left
   };
   const double arad = ra - sa[dtype];
   double adeg = arad*(180.0/M_PI);
   if (adeg < 0.0) {adeg += 360.0;}
   const double Scale = rlen/dlen;
   
   xposition[frame_number] = centerx;
   yposition[frame_number] = centery;
   rotation[frame_number] = adeg;
   scale[frame_number] = Scale;
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_number , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2 , frame_number);
}



void TextAnimation::SetWidth(int width , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   double Scale = (double)width/(double)text_w;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      scale[i] = Scale;
   }
}



void TextAnimation::SetHeight(int height , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   double Scale = (double)height/(double)text_h;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      scale[i] = Scale;
   }
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_start , int frame_stop) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   const double tw = (double)text_w;
   const double th = (double)text_h;
   const double dw = (double)((text_w + 1)/2);
   const double dh = (double)((text_h + 1)/2);
   const double dlen = sqrt(dw*dw + dh*dh);
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   double ra = 0.0;
   if (rlen != 0.0) {ra = atan2(rh,rw);}
   const double sa[4] = {
      atan2(-th,-tw),// top left
      atan2(-th,tw),// top right
      atan2(th,tw),// bottom right
      atan2(th,-tw)// bottom left
   };
   const double arad = ra - sa[dtype];
   double adeg = arad*(180.0/M_PI);
   if (adeg < 0.0) {adeg += 360.0;}
   const double Scale = rlen/dlen;
   
   for (int i = frame_start ; i < frame_stop ; ++i) {
      xposition[i] = centerx;
      yposition[i] = centery;
      rotation[i] = adeg;
      scale[i] = Scale;
   }
}



void TextAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_start , int frame_stop , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2 , frame_start , frame_stop);
}



void TextAnimation::SetWidth(double width , double wspeed , double waccel , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   ASSERT(text_w);
   // w = width + wspeed*t + (waccel/2.0)*t^2
   // wv = wspeed + waccel*t
   double t = 0.0;
   double spf = SecondsPerFrame();
   double w,Scale;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      w = width + wspeed*t + (waccel/2.0)*t*t;
      Scale = w/(double)text_w;
      if (Scale < 0.0) {Scale = 0.0;}
      scale[i] = Scale;
      t += spf;
   }
}



void TextAnimation::SetHeight(double height , double hspeed , double haccel , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   ASSERT(text_h);
   // h = height + hspeed*t + (haccel/2.0)*t^2
   // hv = hspeed + haccel*t
   double t = 0.0;
   double spf = SecondsPerFrame();
   double h,Scale;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      h = height + hspeed*t + (haccel/2.0)*t*t;
      Scale = h/(double)text_h;
      if (Scale < 0.0) {Scale = 0.0;}
      scale[i] = Scale;
      t += spf;
   }
}



void TextAnimation::SetText(string text , FONT* textfont , bool use_color_font) {
   ASSERT(text.size() != 0);
   ASSERT(textfont);
   ASSERT(screen);// allegro must be in a graphics mode
   text_str = text;
   text_font = textfont;
   using_color_font = use_color_font;
   text_w = text_length(text_font , text_str.c_str());
   text_h = text_height(text_font);
   text_bh.SetClearColor(Color(makecol(255,0,255)));
//   bool ClearResize(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   text_bh.ClearResize(bitmap_color_depth(screen) , text_w , text_h , MEMORY , TRANSPARENT);
   CalcSize();
}



/// ---------------------------------- SimpleBitmapAnimation class -----------------------------------------------



SimpleBitmapAnimation::SimpleBitmapAnimation(const SimpleBitmapAnimation& sba) :
      AnimationBase(sba.anim_type),
      WidgetBase(sba.Name() , NULL , sba.Area()),
      draw_type(sba.draw_type)
{
   Reallocate(sba.num_frames , sba.duration);
   SetLoopStop(sba.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = sba.frames[i];
      free_frame[i] = false;
      xposition[i] = sba.xposition[i];
      yposition[i] = sba.yposition[i];
      color1[i] = sba.color1[i];
      color2[i] = sba.color2[i];
   }
}



SimpleBitmapAnimation::SimpleBitmapAnimation(string name , Rectangle position , ANIMATION_TYPE atype ,
                     BMP_DRAW_TYPE bmp_draw_type , int frame_count , double play_time , int loop_stop) :
      AnimationBase(atype),
      WidgetBase(name , NULL , position),
      draw_type(bmp_draw_type)
{
   Reallocate(frame_count , play_time);
   SetLoopStop(loop_stop);
}



SimpleBitmapAnimation& SimpleBitmapAnimation::operator=(const SimpleBitmapAnimation& sba) {
   /// TODO , IMPORTANT - Figure out how to assign the base class parts like WidgetBase
   Reallocate(sba.num_frames , sba.duration);
   SetLoopStop(sba.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = sba.frames[i];
      free_frame[i] = false;
      xposition[i] = sba.xposition[i];
      yposition[i] = sba.yposition[i];
      color1[i] = sba.color1[i];
      color2[i] = sba.color2[i];
   }
   return *this;
}



void SimpleBitmapAnimation::Reallocate(int number_of_frames , double total_play_time) {
   ASSERT(number_of_frames > 0);
   ASSERT(total_play_time > 0.0);
   AnimationBase::Free();
   num_frames = number_of_frames;
   duration = total_play_time;
   frames_per_sec = (double)num_frames / duration;
   SetFrameTime(0.0);
   
   frames = new BITMAP*[num_frames];
   free_frame = new bool[num_frames];
   xposition = new int[num_frames];
   yposition = new int[num_frames];
   // rotation = new double[num_frames];// not used
   // scale = new double[num_frames];// not used
   color1 = new int[num_frames];
   color2 = new int[num_frames];
   
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = 0;
      free_frame[i] = false;
      xposition[i] = 0;
      yposition[i] = 0;
      color1[i] = 0;
      color2[i] = 0;
   }
}



void SimpleBitmapAnimation::Free() {
   AnimationBase::Free();
}



void SimpleBitmapAnimation::CalcSize() {
   int xmin = 100000;
   int xmax = -100000;
   int ymin = 100000;
   int ymax = -100000;
   
   int tlx = 0;
   int tly = 0;
   int brx = 0;
   int bry = 0;
   
   if (!frames) {
      WidgetBase::SetArea(0,0,0,0);
      return;
   }
   for (int i = 0 ; i < num_frames ; ++i) {
      BITMAP* bmp = frames[i];
      tlx = base_x + xposition[i];
      tly = base_y + yposition[i];
      brx = tlx + bmp->w - 1;
      bry = tly + bmp->h - 1;
      if (tlx < xmin) {xmin = tlx;}
      if (tly < ymin) {ymin = tly;}
      if (brx > xmax) {xmax = brx;}
      if (bry > ymax) {ymax = bry;}
   }
   
   WidgetBase::SetArea(tlx,tly,(brx-tlx) + 1 , (bry - tly) + 1);
}



void SimpleBitmapAnimation::DrawOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   ASSERT(frames);
   ASSERT(xposition);
   ASSERT(yposition);
   ASSERT(color1);
   ASSERT(color2);
   
   BITMAP* src = frames[frame_num];
   if (src && (frame_num != -1)) {
      int tlx = x + base_x + xposition[frame_num];
      int tly = y + base_y + yposition[frame_num];
      int tint = color2[frame_num];
      if (tint) {
         switch (draw_type) {
            case SOLID :
//void tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n);
               tint_blit(src , bmp , 0 , 0 , tlx , tly , src->w , src->h , color1[frame_num] , tint);
               break;
            case TRANSPARENT :
//void masked_tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n);
               masked_tint_blit(src , bmp , 0 , 0 , tlx , tly , src->w , src->h , color1[frame_num] , tint);
               break;
            case TRANSLUCENT :
               set_alpha_blender();
               draw_trans_sprite(bmp , src , tlx , tly);
               break;
         }
      } else {
         switch (draw_type) {
            case SOLID :
               blit(src , bmp , 0 , 0 , tlx , tly , src->w , src->h);
               break;
            case TRANSPARENT :
               masked_blit(src , bmp , 0 , 0 , tlx , tly , src->w , src->h);
               break;
            case TRANSLUCENT :
               set_alpha_blender();
               draw_trans_sprite(bmp , src , tlx , tly);
               break;
         }
      }
   }
}



int SimpleBitmapAnimation::SetFrameTime(double frame_time) {
   int cf = frame_num;
   int ln = loop_num;
   int ret = AnimationBase::SetFrameTime(frame_time);
   if (cf != frame_num) {
      SetBgRedrawFlag();
   }
   if (ln != ret) {
      QueueUserMessage(this , TOPIC_ANIMATION , ret);
   }
   return ret;
}



int SimpleBitmapAnimation::SetFrameNumber(int frame_number) {
   int fn = frame_num;
   int ret = AnimationBase::SetFrameNumber(frame_number);
   if (frame_num != fn) {
      SetBgRedrawFlag();
   }
   return ret;
}



WidgetMsg SimpleBitmapAnimation::Update (double tsec) {
   if (flags & ENABLED) {
      AdvanceFrameTime(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg SimpleBitmapAnimation::CheckInputs(int msx , int msy) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void SimpleBitmapAnimation::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      DrawOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



bool SimpleBitmapAnimation::Init() {
   CalcSize();
   return true;
}



bool SimpleBitmapAnimation::Ready() {
   if (!frames) {return false;}
   if (!free_frame) {return false;}
   if (!xposition) {return false;}
   if (!yposition) {return false;}
   if (!color1) {return false;}
   if (!color2) {return false;}
   return true;
}



bool SimpleBitmapAnimation::Shutdown() {
   return true;
}




/// ---------------------------------- FullBitmapAnimation class -----------------------------------------------



FullBitmapAnimation::FullBitmapAnimation(const FullBitmapAnimation& fba) :
      AnimationBase(fba.anim_type),
      WidgetBase(fba.Name() , NULL , fba.Area()),
      draw_type(fba.draw_type)
{
   Reallocate(fba.num_frames , fba.duration);
   SetLoopStop(fba.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = fba.frames[i];
      free_frame[i] = false;
      xposition[i] = fba.xposition[i];
      yposition[i] = fba.yposition[i];
      rotation[i] = fba.rotation[i];
      scale[i] = fba.scale[i];
   }
}



FullBitmapAnimation::FullBitmapAnimation(string name , Rectangle position , ANIMATION_TYPE atype ,
                  BMP_DRAW_TYPE bmp_draw_type , int frame_count , double play_time , int loop_stop) :
      AnimationBase(atype),
      WidgetBase(name , NULL , position),
      draw_type(bmp_draw_type)
{
   Reallocate(frame_count , play_time);
   SetLoopStop(loop_stop);
}



FullBitmapAnimation& FullBitmapAnimation::operator=(const FullBitmapAnimation& fba) {
   /// TODO , IMPORTANT - Figure out how to assign the base class parts like WidgetBase
   Reallocate(fba.num_frames , fba.duration);
   SetLoopStop(fba.loop_stop_num);
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = fba.frames[i];
      free_frame[i] = false;
      xposition[i] = fba.xposition[i];
      yposition[i] = fba.yposition[i];
      rotation[i] = fba.rotation[i];
      scale[i] = fba.scale[i];
   }
   return *this;
}



void FullBitmapAnimation::Reallocate(int number_of_frames , double total_play_time) {
   ASSERT(total_play_time > 0.0);
   ASSERT(number_of_frames > 0);
   AnimationBase::Free();
   num_frames = number_of_frames;
   duration = total_play_time;
   frames_per_sec = (double)num_frames / duration;
   SetFrameTime(0.0);
   
   frames = new BITMAP*[num_frames];
   free_frame = new bool[num_frames];
   xposition = new int[num_frames];
   yposition = new int[num_frames];
   rotation = new double[num_frames];
   scale = new double[num_frames];
//   color1 = new int[num_frames];
//   color2 = new int[num_frames];
   
   for (int i = 0 ; i < num_frames ; ++i) {
      frames[i] = 0;
      free_frame[i] = false;
      xposition[i] = 0;
      yposition[i] = 0;
      rotation[i] = 0.0;
      scale[i] = 1.0;
   }
}



void FullBitmapAnimation::Free() {
   AnimationBase::Free();
}



void FullBitmapAnimation::CalcSize() {
   int lxmax = 100000;
   int rxmax = -100000;
   int tymax = 100000;
   int bymax = -100000;
   int wmax,hmax;
   double a[4];
   double radangle;
   double cx,cy;
   double dx,dy;
   int x,y;
   for (int i = 0 ; i < num_frames ; ++i) {
      BITMAP* bmp = frames[i];
      double dw = 0.0;
      double dh = 0.0;
      double dlen = 0.0;
      double sa[4] = {0.0 , 0.0 , 0.0 , 0.0};
      if (bmp && bmp->w && bmp->h) {
         dw = scale[i]*(double)((bmp->w + 1)/2);
         dh = scale[i]*(double)((bmp->h + 1)/2);
         dlen = sqrt(dw*dw + dh*dh);
         sa[0] = atan2(dh,dw);
         sa[1] = atan2(dh,-dw);
         sa[2] = atan2(-dh,dw);
         sa[3] = atan2(-dh,-dw);
      }
      cx = (double)(xposition[i] + base_x);
      cy = (double)(yposition[i] + base_y);
      radangle = rotation[i]*(M_PI/180.0);
      for (int j = 0 ; j < 4 ; ++j) {
         a[j] = radangle + sa[j];
         dx = cx + dlen*cos(a[j]);
         dy = cy + dlen*sin(a[j]);
         x = (int)dx;
         y = (int)dy;
         if (x - 1 < lxmax) {lxmax = x - 1;}
         if (x + 1 > rxmax) {rxmax = x + 1;}
         if (y - 1 < tymax) {tymax = y - 1;}
         if (y + 1 > bymax) {bymax = y + 1;}
      }
   }
   wmax = rxmax - lxmax + 1;
   hmax = bymax - tymax + 1;
   WidgetBase::SetArea(lxmax,tymax,wmax,hmax);
   
}



void FullBitmapAnimation::DrawOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      for (int i = frame_num - num_trails*frames_per_trail ; i <= frame_num ; i += frames_per_trail) {
         if (i >= 0) {
            fixed angle;
            BITMAP* src = frames[i];
            if (src) {
               int dw = (int)(scale[i]*(double)src->w);
               int dh = (int)(scale[i]*(double)src->h);
               int dx = x + base_x + xposition[i] - dw/2;
               int dy = y + base_y + yposition[i] - dh/2;
               switch (draw_type) {
                  case SOLID :
                     ASSERT(is_memory_bitmap(src));// due to restrictions on stretch_blit only being able to use memory bitmaps
                     stretch_blit(src , bmp , 0 , 0 , src->w , src->h , dx , dy , dw , dh);
                     break;
                  case TRANSPARENT :
                     angle = ftofix(rotation[i]*(256.0/360.0));
                     //void pivot_scaled_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, fixed angle, fixed scale);
                     pivot_scaled_sprite(bmp , src , x + base_x + xposition[i] , y + base_y + yposition[i] ,
                                          src->w/2 , src->h/2 , angle , ftofix(scale[i]));
                     break;
                  case TRANSLUCENT :
                     set_alpha_blender();
                     draw_trans_sprite(bmp , src , x + base_x + xposition[i] - src->w/2 , y + base_y + yposition[i] - src->h/2);
                     break;
               }
            }
         }
      }
   }
}



int FullBitmapAnimation::SetFrameTime(double frame_time) {
   int cf = frame_num;
   int ln = loop_num;
   int ret = AnimationBase::SetFrameTime(frame_time);
   if (cf != frame_num) {
      SetBgRedrawFlag();
   }
   if (ln != ret) {
      QueueUserMessage(this , TOPIC_ANIMATION , ret);
   }
   return ret;
}



int FullBitmapAnimation::SetFrameNumber(int frame_number) {
   int fn = frame_num;
   int ret = AnimationBase::SetFrameNumber(frame_number);
   if (frame_num != fn) {
      SetBgRedrawFlag();
   }
   return ret;
}



void FullBitmapAnimation::SetNumTrails(int numtrails , int num_frames_per_trail) {
   AnimationBase::SetNumTrails(numtrails , num_frames_per_trail);
   SetBgRedrawFlag();
}



void FullBitmapAnimation::SetBasePos(int xpos , int ypos) {
   Rectangle r = Area();
   r.MoveBy(xpos - base_x , ypos - base_y);
   WidgetBase::SetArea(r);
   AnimationBase::SetBasePos(xpos,ypos);
}



WidgetMsg FullBitmapAnimation::Update (double tsec) {
   if (flags & ENABLED) {
      AdvanceFrameTime(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg FullBitmapAnimation::CheckInputs(int msx , int msy) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void FullBitmapAnimation::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      DrawOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



bool FullBitmapAnimation::Init() {
   CalcSize();
   return true;
}



bool FullBitmapAnimation::Ready() {
   if (!frames) {return false;}
   if (!free_frame) {return false;}
   if (!xposition) {return false;}
   if (!yposition) {return false;}
   if (!rotation) {return false;}
   if (!scale) {return false;}
   return true;
}



bool FullBitmapAnimation::Shutdown() {
   return true;
}



int FullBitmapAnimation::GetWidth(int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   if (!frames[frame_number]) {return 0;}
   return (int)(scale[frame_number]*(double)(frames[frame_number]->w));
}



int FullBitmapAnimation::GetHeight(int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   if (!frames[frame_number]) {return 0;}
   return (int)(scale[frame_number]*(double)(frames[frame_number]->h));
}



bool FullBitmapAnimation::AllBitmapsReady() {
   bool ready = true;
   for (int i = 0 ; i < num_frames ; ++i) {
      if (!frames[i]) {ready = false;}
   }
   return ready;
}



void FullBitmapAnimation::SetWidth(int width) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      BITMAP* bmp = frames[i];
      if (bmp && bmp->w) {
         double bw = (double)(bmp->w);
         double Scale = ((double)width)/bw;
         scale[i] = Scale;
      }
   }
}



void FullBitmapAnimation::SetHeight(int height) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   for (int i = 0 ; i < num_frames ; ++i) {
      BITMAP* bmp = frames[i];
      if (bmp && bmp->h) {
         double bh = (double)(bmp->h);
         double Scale = ((double)height)/bh;
         scale[i] = Scale;
      }
   }
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery) {
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   ASSERT(rlen != 0.0);// center coordinates and diagonal end point coordinates cannot be the same
   const double ra = atan2(rh,rw);
   
   for (int i = 0 ; i < num_frames ; ++i) {
      double adeg = 0.0;
      double Scale = 1.0;
      BITMAP* bmp = frames[i];
      if (bmp && bmp->w && bmp->h) {
         const double dw = (double)((bmp->w + 1)/2);
         const double dh = (double)((bmp->h + 1)/2);
         const double dlen = sqrt(dw*dw + dh*dh);
         const double sa[4] = {
            atan2(-dh,-dw),// top left
            atan2(-dh,dw),// top right
            atan2(dh,dw),// bottom right
            atan2(dh,-dw)// bottom left
         };
         const double arad = ra - sa[dtype];
         adeg = arad*(180.0/M_PI);
         if (adeg < 0.0) {adeg += 360.0;}
         Scale = rlen/dlen;
      }
      xposition[i] = centerx;
      yposition[i] = centery;
      rotation[i] = adeg;
      scale[i] = Scale;
   }
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2);
}



void FullBitmapAnimation::SetWidth(int width , int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   BITMAP* bmp = frames[frame_number];
   if (bmp && bmp->w) {
      double bw = (double)(bmp->w);
      double Scale = ((double)width)/bw;
      scale[frame_number] = Scale;
   }
}



void FullBitmapAnimation::SetHeight(int height , int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   BITMAP* bmp = frames[frame_number];
   if (bmp && bmp->h) {
      double bh = (double)(bmp->h);
      double Scale = ((double)height)/bh;
      scale[frame_number] = Scale;
   }
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_number) {
   ASSERT((frame_number >= 0) && (frame_number < num_frames));
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   ASSERT(rlen != 0.0);// center coordinates and diagonal end point coordinates cannot be the same
   const double ra = atan2(rh,rw);
   
   double adeg = 0.0;
   double Scale = 1.0;
   BITMAP* bmp = frames[frame_number];
   if (bmp && bmp->w && bmp->h) {
      const double dw = (double)((bmp->w + 1)/2);
      const double dh = (double)((bmp->h + 1)/2);
      const double dlen = sqrt(dw*dw + dh*dh);
      const double sa[4] = {
         atan2(-dh,-dw),// top left
         atan2(-dh,dw),// top right
         atan2(dh,dw),// bottom right
         atan2(dh,-dw)// bottom left
      };
      const double arad = ra - sa[dtype];
      adeg = arad*(180.0/M_PI);
      if (adeg < 0.0) {adeg += 360.0;}
      Scale = rlen/dlen;
   }
   xposition[frame_number] = centerx;
   yposition[frame_number] = centery;
   rotation[frame_number] = adeg;
   scale[frame_number] = Scale;
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_number , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2 , frame_number);
}



void FullBitmapAnimation::SetWidth(int width , int frame_start , int frame_stop) {
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   for (int i = frame_start ; i < frame_stop ; ++i) {
      BITMAP* bmp = frames[i];
      if (bmp && bmp->w) {
         double bw = (double)(bmp->w);
         double Scale = ((double)width)/bw;
         scale[i] = Scale;
      }
   }
}



void FullBitmapAnimation::SetHeight(int height , int frame_start , int frame_stop) {
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   for (int i = frame_start ; i < frame_stop ; ++i) {
      BITMAP* bmp = frames[i];
      if (bmp && bmp->h) {
         double bh = (double)(bmp->h);
         double Scale = ((double)height)/bh;
         scale[i] = Scale;
      }
   }
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_start , int frame_stop) {
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   ASSERT(xposition);// make sure the xposition array is allocated
   ASSERT(yposition);// make sure the yposition array is allocated
   ASSERT(rotation);// make sure the rotation array is allocated
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   const double rw = (double)(diagx - centerx);
   const double rh = (double)(diagy - centery);
   const double rlen = sqrt(rw*rw + rh*rh);
   ASSERT(rlen != 0.0);// center coordinates and diagonal end point coordinates cannot be the same
   const double ra = atan2(rh,rw);
   
   for (int i = frame_start ; i < frame_stop ; ++i) {
      double adeg = 0.0;
      double Scale = 1.0;
      BITMAP* bmp = frames[i];
      if (bmp && bmp->w && bmp->h) {
         const double dw = (double)((bmp->w + 1)/2);
         const double dh = (double)((bmp->h + 1)/2);
         const double dlen = sqrt(dw*dw + dh*dh);
         const double sa[4] = {
            atan2(-dh,-dw),// top left
            atan2(-dh,dw),// top right
            atan2(dh,dw),// bottom right
            atan2(dh,-dw)// bottom left
         };
         const double arad = ra - sa[dtype];
         adeg = arad*(180.0/M_PI);
         if (adeg < 0.0) {adeg += 360.0;}
         Scale = rlen/dlen;
      }
      xposition[i] = centerx;
      yposition[i] = centery;
      rotation[i] = adeg;
      scale[i] = Scale;
   }
}



void FullBitmapAnimation::SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_start , int frame_stop , double unused) {
   SetDiagonal(dtype , diagx , diagy , (diagx + odiagx)/2 , (diagy + odiagy)/2 , frame_start , frame_stop);
}



void FullBitmapAnimation::SetWidth(double width , double wspeed , double waccel , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   // w = width + wspeed*t + (waccel/2.0)*t^2
   // wv = wspeed + waccel*t
   double t = 0.0;
   double spf = SecondsPerFrame();
   double w;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      w = width + wspeed*t + (waccel/2.0)*t*t;
      double Scale = 1.0;
      BITMAP* bmp = frames[i];
      if (bmp && bmp->w) {
         Scale = w/(double)bmp->w;
      }
      if (Scale < 0.0) {Scale = 0.0;}
      scale[i] = Scale;
      t += spf;
   }
}



void FullBitmapAnimation::SetHeight(double height , double hspeed , double haccel , int frame_start , int frame_stop) {
   ASSERT(scale);// make sure the scale array is allocated
   ASSERT(frames);// make sure the frames array is allocated
   ASSERT((frame_start >= 0) && (frame_start < num_frames));
   ASSERT(frame_stop > frame_start);
   ASSERT((frame_stop > 0) && (frame_stop <= num_frames));
   // h = height + hspeed*t + (haccel/2.0)*t^2
   // hv = hspeed + haccel*t
   double t = 0.0;
   double spf = SecondsPerFrame();
   double h;
   for (int i = frame_start ; i < frame_stop ; ++i) {
      h = height + hspeed*t + (haccel/2.0)*t*t;
      double Scale = 1.0;
      BITMAP* bmp = frames[i];
      if (bmp && bmp->h) {
         Scale = h/(double)bmp->h;
      }
      if (Scale < 0.0) {Scale = 0.0;}
      scale[i] = Scale;
      t += spf;
   }
}



void FullBitmapAnimation::GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , BMP_MEM_TYPE mem_type) {
   BITMAP* bitmap = CreateBitmap(AllegroColorDepth() , w , h , mem_type);
   if (bitmap) {
      blit(bmp , bitmap , x , y , 0 , 0 , w , h);
   }
   SetBitmap(bitmap , true);
}



void FullBitmapAnimation::GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h) {
   BITMAP* sub = create_sub_bitmap(bmp , x , y  , w , h);
   SetBitmap(sub , true);
}



void FullBitmapAnimation::GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , int frame_number , BMP_MEM_TYPE mem_type) {
   BITMAP* bitmap = CreateBitmap(AllegroColorDepth() , w , h , mem_type);
   if (bitmap) {
      blit(bmp , bitmap , x , y , 0 , 0 , w , h);
   }
   SetBitmap(bitmap , true , frame_number);
}



void FullBitmapAnimation::GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h , int frame_number) {
   BITMAP* sub = create_sub_bitmap(bmp , x , y  , w , h);
   SetBitmap(sub , true , frame_number);
}



void FullBitmapAnimation::GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , int frame_start , int frame_stop , BMP_MEM_TYPE mem_type) {
   BITMAP* bitmap = CreateBitmap(AllegroColorDepth() , w , h , mem_type);
   if (bitmap) {
      blit(bmp , bitmap , x , y , 0 , 0 , w , h);
   }
   SetBitmap(bitmap , true , frame_start , frame_stop);
}



void FullBitmapAnimation::GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h , int frame_start , int frame_stop) {
   BITMAP* sub = create_sub_bitmap(bmp , x , y  , w , h);
   SetBitmap(sub , true , frame_start , frame_stop);
}



/// ---------------------------------------- StateAnimation class --------------------------------------------



StateAnimation::StateAnimation(string name) :
      WidgetBase(name),
      amap(),
      active(0),
      nextanim()
{
}



WidgetMsg StateAnimation::Update (double tsec) {
   if (active) {
      active->AdvanceFrameTime(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg StateAnimation::CheckInputs(int msx , int msy) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void StateAnimation::DisplayOn(BITMAP* bmp , int x , int y) {
   if (active && (flags & VISIBLE)) {
      active->DrawOn(bmp,x,y);
   }
   ClearRedrawFlag();
}



bool StateAnimation::Init() {
   bool ret = true;
   for (SABMIT it = amap.begin() ; it != amap.end() ; ++it) {
      AnimationBase* ab = it->second;
      WidgetBase* wb = dynamic_cast<WidgetBase*>(ab);
      if (wb && !wb->Init()) {ret = false;}
   }
   return ret;
}



bool StateAnimation::Ready() {
   bool ret = true;
   for (SABMIT it = amap.begin() ; it != amap.end() ; ++it) {
      AnimationBase* ab = it->second;
      WidgetBase* wb = dynamic_cast<WidgetBase*>(ab);
      if (wb && !wb->Ready()) {ret = false;}
   }
   return ret;
}



bool StateAnimation::Shutdown() {
   bool ret = true;
   for (SABMIT it = amap.begin() ; it != amap.end() ; ++it) {
      AnimationBase* ab = it->second;
      WidgetBase* wb = dynamic_cast<WidgetBase*>(ab);
      if (wb && !wb->Shutdown()) {ret = false;}
   }
   return ret;
}



void StateAnimation::QueueUserMessage(const WidgetMsg& wmsg) {
   WidgetMsg activestop(dynamic_cast<WidgetBase*>(active) , TOPIC_ANIMATION , -1);
   if (wmsg == activestop) {
      if (nextanim.size()) {
         SetAnimationState(*nextanim.begin());
         nextanim.pop_front();
      } else {
         WidgetBase::QueueUserMessage(this , TOPIC_STATE_ANIMATION , -1);
      }
   }
   WidgetBase::QueueUserMessage(wmsg);
}



void StateAnimation::FreeAll() {
   for (SABMIT it = amap.begin() ; it != amap.end() ; ++it) {
      AnimationBase* ab = it->second;
      ab->Free();
   }
}



void StateAnimation::DrawOn(BITMAP* bmp , int x , int y) {
   if (active) {active->DrawOn(bmp,x,y);}
}



int StateAnimation::AdvanceFrameTime(double delta_time) {
   ASSERT(active);
   return active->AdvanceFrameTime(delta_time);
}



int StateAnimation::SetFrameTime(double frame_time) {
   ASSERT(active);
   return active->SetFrameTime(frame_time);
}



int StateAnimation::SetFrameNumber(int frame_number) {
   ASSERT(active);
   return active->SetFrameNumber(frame_number);
}



int StateAnimation::CurrentFrame() {
   ASSERT(active);
   return active->CurrentFrame();
}



int StateAnimation::CurrentLoop() {
   ASSERT(active);
   return active->CurrentLoop();
}



double StateAnimation::CurrentFrameTime() {
   ASSERT(active);
   return active->CurrentFrameTime();
}



double StateAnimation::FramesPerSecond() {
   ASSERT(active);
   return active->FramesPerSecond();
}



double StateAnimation::SecondsPerFrame() {
   ASSERT(active);
   return active->SecondsPerFrame();
}



double StateAnimation::TimeBetweenFrames(int frame_start , int frame_stop) {
   ASSERT(active);
   return active->TimeBetweenFrames(frame_start , frame_stop);
}



void StateAnimation::SetTimeFactor(double factor) {
   ASSERT(active);
   active->SetTimeFactor(factor);
}



void StateAnimation::Pause() {
   ASSERT(active);
   active->Pause();
}



void StateAnimation::Resume() {
   ASSERT(active);
   active->Resume();
}



void StateAnimation::TogglePause() {
   ASSERT(active);
   active->TogglePause();
}



void StateAnimation::SetAnimationState(string state) {
   SABMIT it = amap.find(state);
   ASSERT(it != amap.end());// make sure state is in map
   active = it->second;
   active->SetFrameTime(0.0);
   WidgetBase* wb = dynamic_cast<WidgetBase*>(active);
   if (wb) {
      WidgetBase::SetArea(wb->Area());
   } else {
      WidgetBase::SetArea(Rectangle(0,0,1,1));
   }
}



void StateAnimation::AddState(string state , AnimationBase* ab) {
   ASSERT(ab);
   amap[state] = ab;
   WidgetBase* wb = dynamic_cast<WidgetBase*>(ab);
   if (wb) {
      wb->SetParent(this);
   }
}



void StateAnimation::QueueAnimationState(string state) {
   SABMIT it = amap.find(state);
   if (it != amap.end()) {
      nextanim.push_back(state);
   }
}



void StateAnimation::ClearQueuedAnimations() {
   nextanim.clear();
}










