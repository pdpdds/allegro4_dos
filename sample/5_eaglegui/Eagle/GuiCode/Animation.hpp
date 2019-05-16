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


#ifndef Animation_H
#define Animation_H


#include <allegro.h>
#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/BitmapHandler.hpp"
#include "Eagle/AllegroUtilities.hpp"

#include <map>
#include <list>
#include <string>


/// Widget Messages from the TextAnimation and BitmapAnimation classes come in the form :
/// WidgetMsg(this , TOPIC_ANIMATION , new_loop_number) and are sent when the animations
/// loop number changes. This means you can check for the new_loop_number to equal -1 which
/// means the animation has stopped playing.

/// WidgetMessages from the StateAnimation class will be in the form :
/// WidgetMsg(this , TOPIC_STATE_ANIMATION , -1) to signal when the queue of animations has
/// finished playing.

extern const unsigned int TOPIC_ANIMATION;

extern const unsigned int TOPIC_STATE_ANIMATION;


enum ANIMATION_TYPE {
   FORWARD_ONCE            = 0,
   BACKWARD_ONCE           = 1,
   FORWARD_AND_BACK_ONCE   = 2,
   FORWARD_REPEAT          = 3,
   BACKWARD_REPEAT         = 4,
   FORWARD_AND_BACK_REPEAT = 5
};



class AnimationBase {
protected :
   BITMAP** frames;
   bool* free_frame;
   int* xposition;
   int* yposition;
   double* rotation;// degrees clockwise from zero pointing right
   double* scale;
   int* color1;
   int* color2;
   
   int base_x;
   int base_y;
   
   ANIMATION_TYPE anim_type;
   double duration;
   double frames_per_sec;
   double frametime;
   double time_factor;
   int num_frames;
   int frame_num;
   int loop_num;
   int loop_stop_num;
   
   int num_trails;
   int frames_per_trail;
   bool paused;
   
   void SetColorHSV(int* color_array , double hue , double huespeed , double sat , double satspeed,
                     double val , double valspeed , int frame_start , int frame_stop);
   void SetColorRGB(int* color_array , int start_color , int finish_color , int frame_start , int frame_stop);
   
   AnimationBase(const AnimationBase& ab) {}
   
public :
   
   AnimationBase(ANIMATION_TYPE atype);
   virtual ~AnimationBase() {}
   
   virtual void Reallocate(int number_of_frames , double total_play_time) {}
   virtual void Free();
   
   virtual void CalcSize()=0;// must be in the derived class

   virtual void DrawOn(BITMAP* bmp , int x , int y)=0;// must be in the derived class

   /// AdvanceFrameTime, SetFrameTime, and SetFrameNumber all return the now current loop number, -1 if no longer playing
   // If frame_time is negative , the animation will stop playing
   int AdvanceFrameTime(double delta_time);
   virtual int SetFrameTime(double frame_time);
   virtual int SetFrameNumber(int frame_number);

   /// CurrentFrame and CurrentLoop may return -1 to indicate the animation is not currently playing
   int CurrentFrame() {return frame_num;}
   int CurrentLoop() {return loop_num;}
   
   double CurrentFrameTime() {return frametime;}
   double FramesPerSecond() {return frames_per_sec;}
   double SecondsPerFrame() {ASSERT(frames_per_sec != 0.0);return 1.0/frames_per_sec;}
   double TimeBetweenFrames(int frame_start , int frame_stop);// this frame_stop is inclusive

   void SetTimeFactor(double factor);// multiplier for delta time passed to AdvanceFrameTime
   void SetLoopStop(int loop_stop);// -1 infinite repeat, loop_stop is loop_num to complete before stopping (zero based)

   void Pause() {paused = true;}
   void Resume() {paused = false;}
   void TogglePause() {paused = !paused;}

   /// These functions retrieve the currently stored data
   BITMAP* GetBitmap  (int frame_number);
   int     GetXpos    (int frame_number);
   int     GetYpos    (int frame_number);
   float   GetRotation(int frame_number);
   float   GetScale   (int frame_number);
   int     GetColor1  (int frame_number);
   int     GetColor2  (int frame_number);
   int     GetBaseX() {return base_x;}
   int     GetBaseY() {return base_y;}
   int     GetNumTrails() {return num_trails;}
   int     GetFramesPerTrail() {return frames_per_trail;}


   virtual void SetNumTrails(int numtrails , int num_frames_per_trail);

   /// These functions set all the data
   void SetBitmap  (BITMAP* bmp , bool free_bmp);
   void SetXpos    (int x);
   void SetYpos    (int y);
   void SetPos     (int x , int y);
   void SetRotation(double Rotation);
   void SetScale   (double Scale);
   void SetColor1  (int Color);
   void SetColor2  (int Color);

   /// These functions set the data for a given frame
   // frame_number is zero based - x frames gives frame numbers in the range [0,x-1]
   void SetBitmap(BITMAP* bmp , bool free_bmp , int frame_number);
   void SetXpos(int x , int frame_number);
   void SetYpos(int y , int frame_number);
   void SetPos(int x , int y , int frame_number);
   void SetRotation(double Rotation , int frame_number);
   void SetScale(double Scale , int frame_number);
   void SetColor1(int Color , int frame_number);
   void SetColor2(int Color , int frame_number);
   
   /// These functions set the data for a range of frames
   // frame_stop is exclusive so data[frame_stop] won't be set
   // frame_number is zero based - x frames gives frame numbers in the range [0,x-1]
   void SetBitmap(BITMAP* bmp , bool free_bmp , int frame_start , int frame_stop);
   void SetXpos(int x , int frame_start , int frame_stop);
   void SetYpos(int y , int frame_start , int frame_stop);
   void SetPos(int x , int y , int frame_start , int frame_stop);
   void SetRotation(double Rotation , int frame_start , int frame_stop);
   void SetScale(double Scale , int frame_start , int frame_stop);
   void SetColor1(int Color , int frame_start , int frame_stop);
   void SetColor2(int Color , int frame_start , int frame_stop);

   /// Data setting functions
   virtual void SetBasePos(int xpos , int ypos);
   
   void SetXPos(double xstart , double xspeed , double xaccel , int frame_start , int frame_stop);
   void SetYPos(double ystart , double yspeed , double yaccel , int frame_start , int frame_stop);
   // positive degrees rotate clockwise from zero pointing right
   void SetRotation(double rdeg , double rspeed , double raccel , int frame_start , int frame_stop);
   void SetColor1HSV(double hue , double huespeed , double sat , double satspeed ,
                     double val , double valspeed , int frame_start , int frame_stop);
   void SetColor2HSV(double hue , double huespeed , double sat , double satspeed ,
                     double val , double valspeed , int frame_start , int frame_stop);
   void SetColor1RGB(int start_color , int finish_color , int frame_start , int frame_stop);
   void SetColor2RGB(int start_color , int finish_color , int frame_start , int frame_stop);

};




enum TEXT_GRADIENT_TYPE {
   TEXT_GRADIENT_NONE = 0,
   TEXT_GRADIENT_LR   = 1,
   TEXT_GRADIENT_TB   = 2,
   TEXT_GRADIENT_OM   = 3
};

enum DIAGONAL_TYPE {
   DIAGONAL_TL = 0,
   DIAGONAL_TR = 1,
   DIAGONAL_BR = 2,
   DIAGONAL_BL = 3
};

/// The Text Animation class does not use the frames or free_frame arrays from the Animation Base class
/// so calling AnimationBase::SetBitmap of any kind will produce an error. Allegro must be in a graphics
/// mode before creating a TextAnimation object. No global TextAnimation objects are allowed. For text gradients,
/// color1 will be used as the left/top/outer color and color2 will be used as the right/bottom/middle color.
/// When there is no gradient, the text will be colored using color1's current color. xpos and ypos indicate where
/// the center of the text will be.
class TextAnimation : public AnimationBase , public WidgetBase {
protected :
   BitmapHandler text_bh;
   std::string text_str;
   FONT* text_font;
   int text_w;
   int text_h;
   TEXT_GRADIENT_TYPE gradient_type;
   bool using_color_font;
   
   
public :
   TextAnimation(const TextAnimation& ta);
   TextAnimation(std::string name , std::string text , FONT* textfont , bool use_color_font , 
                  TEXT_GRADIENT_TYPE gtype , ANIMATION_TYPE atype , int frame_count , double play_time , int loop_stop = -1);
   ~TextAnimation() {Free();}
   
   TextAnimation& operator=(const TextAnimation& ta);

   /// Override functions for AnimationBase
   virtual void Reallocate(int number_of_frames , double total_play_time);
   virtual void Free();
   
   virtual void CalcSize();

   virtual void DrawOn(BITMAP* bmp , int x , int y);
   virtual void LoggedDrawOn(BITMAP* bmp , int x , int y);

   virtual int SetFrameTime(double frame_time);
   virtual int SetFrameNumber(int frame_number);

   virtual void SetNumTrails(int numtrails , int num_frames_per_trail);// number of frames between trails

   virtual void SetBasePos(int xpos , int ypos);

   /// Override functions for WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();
   
   /// Setting the drawing position or area has no effect as the real area is calculated
   /// based on the position, scale, and rotation data.
   virtual void SetDrawPos(int xpos , int ypos) {}
   virtual void SetDrawDimensions(int width , int height) {}
   virtual void SetArea(int xpos , int ypos , int width , int height) {}
   virtual void SetArea(const Rectangle& r) {}

   /// Getters
   int GetWidth(int frame_number);
   int GetHeight(int frame_number);
   int TextWidth() {return text_w;}
   int TextHeight() {return text_h;}


   /// Setters
   void SetWidth(int width);
   void SetHeight(int height);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , double unused);

   void SetWidth(int width , int frame_number);
   void SetHeight(int height , int frame_number);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_number);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_number , double unused);

   void SetWidth(int width , int frame_start , int frame_stop);
   void SetHeight(int height , int frame_start , int frame_stop);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_start , int frame_stop);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_start , int frame_stop , double unused);

   
   void SetWidth(double width , double wspeed , double waccel , int frame_start , int frame_stop);
   void SetHeight(double height , double hspeed , double haccel , int frame_start , int frame_stop);


   // SetText resizes an internal bitmap, call Ready() afterwards
   void SetText(std::string text , FONT* textfont , bool use_color_font);
};



// From BitmapHandler.hpp :
//enum BMP_MEM_TYPE {
//   MEMORY = 0,
//   SYSTEM = 1,
//   VIDEO = 2
//};
//enum BMP_DRAW_TYPE {
//   SOLID       = 0,
//   TRANSPARENT = 1,
//   TRANSLUCENT = 2
//};

/// SimpleBitmapAnimation uses all the data from AnimationBase except for rotation and scale data, so
/// trying to call SetRotation or SetScale will cause an assertion failure in debug mode and will segfault
/// in non debug mode. Check the flags in gfx_capabilities to see whether you can use hardware acceleration
/// to draw the bitmaps. Color1 is used as the color to tint the bitmap to, and color2 is a number from 0 to
/// 255 that determines how fully tinted the bitmap will be. 0 is no tint, and 255 is full tint. Tinting has no
/// effect on draw type TRANSLUCENT since there are no functions to draw alpha bitmaps with a tint. The tint
/// drawing functions have no hardware acceleration, but you can use the tint_bitmap function to pre tint the bitmaps.
/// The x and y positions are used as the position of the top left of the bitmap.
class SimpleBitmapAnimation : public AnimationBase , public WidgetBase {
protected :
   BMP_DRAW_TYPE draw_type;
   
public :
   SimpleBitmapAnimation(const SimpleBitmapAnimation& sba);
   SimpleBitmapAnimation(std::string name , Rectangle position , ANIMATION_TYPE atype , BMP_DRAW_TYPE bmp_draw_type , int frame_count , double play_time , int loop_stop = -1);
   ~SimpleBitmapAnimation() {Free();}
   
   SimpleBitmapAnimation& operator=(const SimpleBitmapAnimation& sba);
   
   /// Override functions from AnimationBase
   void Reallocate(int number_of_frames , double total_play_time);
   void Free();
   
   void CalcSize();
   
   void DrawOn(BITMAP* bmp , int x , int y);
   
   int SetFrameTime(double frame_time);
   int SetFrameNumber(int frame_number);


   /// Override functions from WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   // These functions do nothing since the widget area is determined by CalcSize(), which is 
   // called during Init().
   virtual void SetDrawPos(int xpos , int ypos) {}
   virtual void SetDrawDimensions(int width , int height) {}
   virtual void SetArea(int xpos , int ypos , int width , int height) {}
   virtual void SetArea(const Rectangle& r) {}
};




/// FullBitmapAnimation uses all the data from AnimationBase except for color1 and color2, so
/// trying to call SetColor* will cause an assertion failure in debug mode and will segfault
/// in non debug mode. Draw type SOLID only uses position and scale data, draw type TRANSPARENT
/// uses position, rotation, and scale data, and draw type TRANSLUCENT only uses position data. This is due
/// to the availability of drawing functions for the different types in Allegro. Draw type solid
/// must use memory bitmaps as the source only due to restrictions on stretch_blit.
/// Use SetBitmap* before you use SetWidth or SetHeight or the scale data will be set incorrectly.
/// Use SetBitmap* before you use SetDiagonal or the scale and rotation data will be set incorrectly.
class FullBitmapAnimation : public AnimationBase , public WidgetBase {
protected :
   BMP_DRAW_TYPE draw_type;
   
public :
   FullBitmapAnimation(const FullBitmapAnimation& fba);
   FullBitmapAnimation(std::string name , Rectangle position , ANIMATION_TYPE atype , BMP_DRAW_TYPE bmp_draw_type , int frame_count , double play_time , int loop_stop = -1);
   ~FullBitmapAnimation() {Free();}
   
   FullBitmapAnimation& operator=(const FullBitmapAnimation& fba);
   
   /// Override functions from AnimationBase
   virtual void Reallocate(int number_of_frames , double total_play_time);
   virtual void Free();
   
   virtual void CalcSize();
   
   virtual void DrawOn(BITMAP* bmp , int x , int y);

   virtual int SetFrameTime(double frame_time);
   virtual int SetFrameNumber(int frame_number);

   virtual void SetNumTrails(int numtrails , int num_frames_per_trail);

   virtual void SetBasePos(int xpos , int ypos);


   /// Override functions for WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);
   
   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();
   
   // These functions do nothing since the widget area is determined by CalcSize(), which is 
   // called during Init().
   virtual void SetDrawPos(int xpos , int ypos) {}
   virtual void SetDrawDimensions(int width , int height) {}
   virtual void SetArea(int xpos , int ypos , int width , int height) {}
   virtual void SetArea(const Rectangle& r) {}

   /// Getters
   int GetWidth(int frame_number);
   int GetHeight(int frame_number);
   
   bool AllBitmapsReady();// checks to make sure all the bitmaps are valid, will still work if they aren't though

   /// Setters
   /// Don't call SetWidth, SetHeight, or SetDiagonal until you have set up the frames array using
   /// SetBitmap* because these functions need to know the bitmap width and height to set the scale
   /// and rotation data correctly.
   void SetWidth(int width);
   void SetHeight(int height);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , double unused);

   void SetWidth(int width , int frame_number);
   void SetHeight(int height , int frame_number);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_number);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_number , double unused);

   void SetWidth(int width , int frame_start , int frame_stop);
   void SetHeight(int height , int frame_start , int frame_stop);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int centerx , int centery , int frame_start , int frame_stop);
   void SetDiagonal(DIAGONAL_TYPE dtype , int diagx , int diagy , int odiagx , int odiagy , int frame_start , int frame_stop , double unused);

   
   void SetWidth(double width , double wspeed , double waccel , int frame_start , int frame_stop);
   void SetHeight(double height , double hspeed , double haccel , int frame_start , int frame_stop);

   /// Functions to load frames from a sprite sheet
   void GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , BMP_MEM_TYPE mem_type);
   void GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h);

   void GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , int frame_number , BMP_MEM_TYPE mem_type);
   void GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h , int frame_number);

   void GrabFrameAsCopy(BITMAP* bmp , int x , int y , int w , int h , int frame_start , int frame_stop , BMP_MEM_TYPE mem_type);
   void GrabFrameAsSubBitmap(BITMAP* bmp , int x , int y , int w , int h , int frame_start , int frame_stop);


};



class StateAnimation : public WidgetBase {
protected :
   std::map<std::string , AnimationBase*> amap;
   AnimationBase* active;
   std::list<std::string> nextanim;
   
public :
   
   StateAnimation(std::string name);
   
   /// Override functions for WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   // The position and area setting functions do nothing since the area is determined by the active animation
   virtual void SetDrawPos(int xpos , int ypos) {}
   virtual void SetDrawDimensions(int width , int height) {}
   virtual void SetArea(int xpos , int ypos , int width , int height) {}
   virtual void SetArea(const Rectangle& r) {}

   /// Functions to mimic AnimationBase, these all call the function of the same name for the active animation
   void FreeAll();// calls Free() for every animation in the map
   
   void DrawOn(BITMAP* bmp , int x , int y);

   int AdvanceFrameTime(double delta_time);
   int SetFrameTime(double frame_time);
   int SetFrameNumber(int frame_number);

   // CurrentFrame and CurrentLoop may return -1 to indicate the animation is not currently playing
   int CurrentFrame();
   int CurrentLoop();
   
   double CurrentFrameTime();
   double FramesPerSecond();
   double SecondsPerFrame();
   double TimeBetweenFrames(int frame_start , int frame_stop);// this frame_stop is inclusive

   void SetTimeFactor(double factor);

   void Pause();
   void Resume();
   void TogglePause();

   /// Functions for adding Animations to the map and for setting the current animation
   void SetAnimationState(std::string state);
   void AddState(std::string state , AnimationBase* ab);
   
   void QueueAnimationState(std::string state);
   
   void ClearQueuedAnimations();
   
};



#endif // Animation_H






