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


#include <cmath>

#include "Eagle/Logging.hpp"
#include "Eagle/EagleCredits.hpp"
#include "Eagle/GuiCode/Animation.hpp"
#include "Eagle/GuiCode/WidgetHandler.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/timer_handler.h"
#include "Eagle/GuiCode/TextWidget.hpp"
#include "Eagle/AllegroSetup.hpp"

#ifdef _WIN32
#define M_PI       3.14159265358979323846   // pi
#endif

using std::endl;

void RunEagleCredits1() {
   ASSERT(screen);
   
   OutputLog() << "Running Eagle credits #1" << endl;
   
   WidgetHandler gui("gui" , screen , Rectangle(0,0,SCREEN_W,SCREEN_H));
   gui.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID);
   
   const int bg_color = makecol(0,32,64);
   WidgetColorset wc = DefaultWidgetColors();
   wc.SetColor(BGCOL , bg_color);
   
   gui.UseWidgetColorset(&wc);
   
   // 32X720 = 21600 + 1440 = 23040 Bytes
   TextAnimation textanime("textanime" , "EAGLE" , font , false , TEXT_GRADIENT_TB , FORWARD_REPEAT , 540 , 9.0 , 0);
   
   gui << &textanime;
   
   const int top_start_color = makecol(255,255,255);
//   const int top_finish_color = bg_color;
   const int bot_start_color = makecol(0,0,255);
   
   textanime.SetBasePos(SCREEN_W/2 , SCREEN_H/2);
   textanime.SetWidth(3*SCREEN_W/4);

//* Spins outward from center growing in size and slowing to stop, then fades bottom and then fades top
   textanime.SetWidth(1.0 , (double)((3*SCREEN_W)/4)/4.0 , 0.0 , 0 , 240);

   textanime.SetColor2(bot_start_color);
   textanime.SetColor2RGB(bot_start_color,bg_color,240,360);
   textanime.SetColor2(bg_color,360,540);

   textanime.SetColor1(top_start_color);
   textanime.SetColor1RGB(top_start_color , bg_color,360,480);
   textanime.SetColor1(bg_color,480,540);
   double numrotations = 5.0;
   double deg = numrotations*360.0;//1800
   double rtime = 4.0;
//   void SetRotation(double rdeg , double rspeed , double raccel , int frame_start , int frame_stop);
   textanime.SetRotation(0.0 , 2.0*deg/rtime , -2.0*deg/(rtime*rtime) , 0 , (int)(rtime*(double)60));
///   textanime.SetRotation(0.0 , 900.0 , -225.0 , 0 , 240);
///   textanime.SetRotation(0.0 , 2.0*deg/4.0 , -2.0*deg/16.0 , 0 , 240);
   // r = rdeg + rspeed*t + (raccel/2.0)*t*t;
   // r = 0 + 900*4.0 + -225*16 = 3600 - 2250 - 1200 - 150 = 0
   // rv = 900 + -450*4 = -1800
   
   textanime.SetNumTrails(5 , 3);
//*/

/* Cascading rainbow
   textanime.SetColor1HSV(0.0 , 720.0/12.0 , 1.0 , 0.0 , 1.0 , 0.0 , 0 , 720);
   textanime.SetColor2HSV(30.0 , 720.0/12.0 , 1.0 , 0.0 , 1.0 , 0.0 , 0 , 720);
*/   
/** TODO : Diagnostic , remove
   TextWidget frame_display(0 , Rectangle(0,0,50,20));
   gui << &frame_display;
//*/
   const WidgetMsg textstop(&textanime , TOPIC_ANIMATION , -1);
   
   if (!gui.Init()) {
      OutputLog() << "Gui Init() failed during RunEagleCredits1()." << endl;
      return;
   }
   if (!gui.Ready()) {
      OutputLog() << "Gui Ready() failed during RunEagleCredits1()." << endl;
      return;
   }

   Input input_quit(input_key_press(KEY_ESC));

   double dt = 0.0;
   const int old_timer_rate = GetTimerRate();
   const int timer_running = IsTimerRunning();
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {
      OutputLog() << "Could not setup and start the timer during RunEagleCredits1()." << endl;
      return;
   }

///   int frame_num = 0;/// TODO : Diagnostic , remove
   bool animation_over = false;
   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);

      gui.Update(dt);
      
/** TODO : Diagnostic , remove
//      if (textanime.CurrentFrame() == -1) {break;}
      if (input_key_press(KEY_P)) {
         textanime.TogglePause();
      }
      if (input_key_press(KEY_F)) {
         frame_display.ToggleWidgetVisibility();
      }
      
      if (input_key_press(KEY_LEFT) || key_held_for(KEY_LEFT , 0.5)) {
         frame_num = textanime.CurrentFrame();
         --frame_num;
         if (frame_num < 0) {frame_num = 0;}
         textanime.SetFrameNumber(frame_num);
      }
      if (input_key_press(KEY_RIGHT) || key_held_for(KEY_RIGHT , 0.5)) {
         frame_num = textanime.CurrentFrame();
         ++frame_num;
         if (frame_num > 719) {frame_num = 719;}
         textanime.SetFrameNumber(frame_num);
      }
      frame_display.SetText(textanime.CurrentFrame());
      
      if (input_key_press(KEY_L)) {
         int cf = textanime.CurrentFrame();
         Color col1(textanime.GetColor1(cf));
         Color col2(textanime.GetColor2(cf));
         OutputLog() << "Logging animation : Loop #" << textanime.CurrentLoop() << " , Frame #" << cf << endl;
         OutputLog() << "   Color1 : " << col1 << endl;
         OutputLog() << "   Color2 : " << col2 << endl;
         OutputLog() << "   Rotation : " << textanime.GetRotation(cf) << " degrees" << endl;
         OutputLog() << "   Scale : " << textanime.GetScale(cf) << endl;
         OutputLog() << "   BasePos : " << textanime.GetBaseX() << " , " << textanime.GetBaseY() << endl;
         OutputLog() << "   Position : " << textanime.GetXpos(cf) << " , " << textanime.GetYpos(cf) << endl;
         OutputLog() << "   NumTrails : " << textanime.GetNumTrails() << endl;
         OutputLog() << "   FramesPerTrail : " << textanime.GetFramesPerTrail() << endl;
         OutputLog() << "   WidgetBase :" << endl;
         Indenter indent(2,3);
         textanime.PrintBaseTo(OutputLog() , indent);
      }
      
      if (input_key_press(KEY_D)) {
         int oldframe = textanime.CurrentFrame();
         textanime.SetFrameNumber(600);
         clear(screen);
         textanime.LoggedDrawOn(screen , 0 , 0);
         clear_keybuf();
         readkey();
         textanime.SetFrameNumber(oldframe);
      }
//*/
      
      gui.CheckInputs(mx,my);
      
      while (gui.HasMessages()) {
         WidgetMsg msg = gui.TakeNextMessage();
         
         if (msg == textstop) {animation_over = true;}
      }
      
      gui.Display();
      if (animation_over) {break;}
   }
   
   gui.Shutdown();
   UpdateInputHandler(0.0);
   SetTimerRate(old_timer_rate);
   if (!timer_running) {
      StopTimer();
   }
   SetTickCount(0);
}



void RunEagleCredits2() {
   ASSERT(screen);
   
   OutputLog() << "Running Eagle credits #2" << endl;
   
   WidgetHandler gui("gui" , screen , Rectangle(0,0,SCREEN_W,SCREEN_H));
   gui.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID);
   
   const int bg_color = makecol(0,32,64);
   WidgetColorset wc = DefaultWidgetColors();
   wc.SetColor(BGCOL , bg_color);
   
   gui.UseWidgetColorset(&wc);
   
   const int NUMFRAMES = 600;
   const double duration = 10.0;
   TextAnimation animl("animl" , "EAGLE" , font , false , TEXT_GRADIENT_LR , FORWARD_REPEAT , NUMFRAMES , duration , 0);
   TextAnimation animr("animr" , "EAGLE" , font , false , TEXT_GRADIENT_LR , FORWARD_REPEAT , NUMFRAMES , duration , 0);
   
   gui << &animl << &animr;
   const WidgetMsg textstop(&animl , TOPIC_ANIMATION , -1);
   
   const int tw = animl.TextWidth();
   const int th = animl.TextHeight();
   const double radiusmax = (double)(SCREEN_W/2)/(cos(atan2(th,tw)));
   const double lstheta = atan2(th,-tw);
   const double rstheta = atan2(-th,tw);
   double ltheta , rtheta;
   const double spintime = 4.0;
   const double thetamax = 5.5*2.0*M_PI;
   int ldx = SCREEN_W/4;
   int ldy = SCREEN_H/2;
   int rdx = (3*SCREEN_W)/4;
   int rdy = SCREEN_H/2;
   int lodx,lody,rodx,rody;
   double radius = 0.0;
   const double spf = animl.SecondsPerFrame();
   double t = 0.0;
   const double rv = (2.0*thetamax)/spintime;
   const double ra = (-2.0*thetamax)/(spintime*spintime);
   for (int i = 0 ; i < 240 ; ++i) {
      radius = (t/spintime)*radiusmax;
      double thetamove = rv*t + (ra/2.0)*t*t;
      ltheta = lstheta + thetamove;
      rtheta = rstheta + thetamove;
      lodx = ldx + (int)(radius*cos(ltheta));
      lody = ldy + (int)(radius*sin(ltheta));
      rodx = rdx + (int)(radius*cos(rtheta));
      rody = rdy + (int)(radius*sin(rtheta));
      animl.SetDiagonal(DIAGONAL_BL , ldx , ldy , lodx , lody , i , 0.0);
      animr.SetDiagonal(DIAGONAL_TR , rdx , rdy , rodx , rody , i , 0.0);
      t += spf;
   }
   
   t = 0.0;
   int sw = SCREEN_W/2;
   animl.SetWidth(sw , 240);
   animr.SetWidth(sw , 240);
   const int stopw = (3*SCREEN_W)/4 - 10;
   animl.SetWidth(stopw , 360 , NUMFRAMES);
   animr.SetWidth(stopw , 360 , NUMFRAMES);
   const int stoph = animl.GetHeight(360);
   double dw = (double)(stopw - sw);
   
   // (lbly - stoph = ltly) (rbry - (ltly + 10)/2 = moveheight
   int ltly = ldy - stoph;
   double mh = (double)(((rdy - ltly) - 10)/2);
   int movey;
   double movew = (double)(SCREEN_W/8 - ldx);
   int movex;
   const double movetime = 2.0;
   
   animl.SetWidth((double)sw , 2.0*dw/movetime , -2.0*dw/(movetime*movetime) , 240 , 360);
   animr.SetWidth((double)sw , 2.0*dw/movetime , -2.0*dw/(movetime*movetime) , 240 , 360);

   int lcx,lcy,rcx,rcy;
   int cw,ch;
   for (int i = 240 ; i < 360 ; ++i) {
      cw = animl.GetWidth(i);
      ch = animl.GetHeight(i);
      movex = (int)(2.0*movew*t/movetime + (-2.0*movew*t*t/(2.0*movetime*movetime)));
      movey = (int)(2.0*mh*t/movetime + (-2.0*mh*t*t/(2.0*movetime*movetime)));
      lcx = ldx + movex + cw/2;
      lcy = ldy + movey - ch/2;
      rcx = (rdx - cw/2) - movex;
      rcy = (rdy + ch/2) - movey;
      animl.SetPos(lcx,lcy,i);
      animr.SetPos(rcx,rcy,i);
      t += spf;
   }
   animl.SetPos(lcx,lcy,360,NUMFRAMES);
   animr.SetPos(rcx,rcy,360,NUMFRAMES);
   
   
   int lcol = makecol(0,0,255);
   int rcol = makecol(0,255,0);
   int lcol2 = makecol(0,0,64);
   int rcol2 = makecol(0,64,0);
   animl.SetColor1(lcol);
   animl.SetColor2(rcol);
   animr.SetColor1(rcol);
   animr.SetColor2(lcol);
   animr.SetColor1RGB(rcol,lcol2,240,300);
   animr.SetColor2RGB(lcol,rcol2,240,300);
   animr.SetColor1(lcol2,300,NUMFRAMES);
   animr.SetColor2(rcol2,300,NUMFRAMES);
   
   
   animl.SetNumTrails(5,3);
   animr.SetNumTrails(5,3);

//** TODO : Diagnostic , remove
   TextWidget frame_display("frame_display" , 0 , 0 , 0);
   gui << &frame_display;
   TextWidget rotation_displayl("rotation_displayl" , 0 , 0 , 12);
   gui << &rotation_displayl;
   TextWidget rotation_displayr("rotation_displayr" , 0 , 0 , 24);
   gui << &rotation_displayr;
   char strbuf[256];
//*/

   if (!gui.Init()) {
      OutputLog() << "Gui Init() failed during RunEagleCredits2()." << endl;
      return;
   }
   if (!gui.Ready()) {
      OutputLog() << "Gui Ready() failed during RunEagleCredits2()." << endl;
      return;
   }

   Input input_quit(input_key_press(KEY_ESC));

   double dt = 0.0;
   const int old_timer_rate = GetTimerRate();
   const int timer_running = IsTimerRunning();
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {
      OutputLog() << "Could not setup and start the timer during RunEagleCredits2()." << endl;
      return;
   }

   int frame_num = 0;/// TODO : Diagnostic , remove
   bool animation_over = false;
   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);

      gui.Update(dt);

//** TODO : Diagnostic , remove
//      if (textanime.CurrentFrame() == -1) {break;}
      if (input_key_press(KEY_P)) {
         animl.TogglePause();
         animr.TogglePause();
      }
      if (input_key_press(KEY_F)) {
         frame_display.ToggleWidgetVisibility();
         rotation_displayl.ToggleWidgetVisibility();
         rotation_displayr.ToggleWidgetVisibility();
      }
      
      if (input_key_press(KEY_LEFT) || key_held_for(KEY_LEFT , 0.5)) {
         frame_num = animl.CurrentFrame();
         --frame_num;
         if (frame_num < 0) {frame_num = 0;}
         animl.SetFrameNumber(frame_num);
         animr.SetFrameNumber(frame_num);
      }
      if (input_key_press(KEY_RIGHT) || key_held_for(KEY_RIGHT , 0.5)) {
         frame_num = animl.CurrentFrame();
         ++frame_num;
         if (frame_num > (NUMFRAMES - 1)) {frame_num = NUMFRAMES - 1;}
         animl.SetFrameNumber(frame_num);
         animr.SetFrameNumber(frame_num);
      }
      int cf = animl.CurrentFrame();
      frame_display.SetText(cf);
      if (cf != -1) {
         sprintf(strbuf , "Rotation L : %lf" , animl.GetRotation(cf));
         rotation_displayl.SetText(strbuf);
         sprintf(strbuf , "Rotation R : %lf" , animr.GetRotation(cf));
         rotation_displayr.SetText(strbuf);
      }
//*/
      gui.CheckInputs(mx,my);
      
      while (gui.HasMessages()) {
         WidgetMsg msg = gui.TakeNextMessage();
         
         if (msg == textstop) {animation_over = true;}
      }
      
      gui.Display();
      if (animation_over) {break;}
   }
   
   gui.Shutdown();
   UpdateInputHandler(0.0);
   SetTimerRate(old_timer_rate);
   if (!timer_running) {
      StopTimer();
   }
   SetTickCount(0);
}



void RunEagleCredits3() {
   ASSERT(screen);
   
   OutputLog() << "Running Eagle credits #3" << endl;
   
   WidgetHandler gui("gui" , screen , Rectangle(0,0,SCREEN_W,SCREEN_H));
   gui.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID);
   
   const int bg_color = makecol(0,32,64);
   WidgetColorset wc = DefaultWidgetColors();
   wc.SetColor(BGCOL , bg_color);
   
   gui.UseWidgetColorset(&wc);
   
   // 32X720 = 21000 + 1400 + 640 = 23040 Bytes
   TextAnimation textanime("textanime" , "EAGLE" , font , false , TEXT_GRADIENT_TB , FORWARD_REPEAT , 540 , 9.0 , 1);
   
   gui << &textanime;
   const WidgetMsg textstop(&textanime , TOPIC_ANIMATION , -1);
   
   if (!gui.Init()) {
      OutputLog() << "Gui Init() failed during RunEagleCredits3()." << endl;
      return;
   }
   if (!gui.Ready()) {
      OutputLog() << "Gui Ready() failed during RunEagleCredits3()." << endl;
      return;
   }

   Input input_quit(input_key_press(KEY_ESC));

   double dt = 0.0;
   const int old_timer_rate = GetTimerRate();
   const int timer_running = IsTimerRunning();
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {
      OutputLog() << "Could not setup and start the timer during RunEagleCredits3()." << endl;
      return;
   }

///   int frame_num = 0;/// TODO : Diagnostic , remove
   bool animation_over = true;
   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);

      gui.Update(dt);
      gui.CheckInputs(mx,my);
      
      while (gui.HasMessages()) {
         WidgetMsg msg = gui.TakeNextMessage();
         
         if (msg == textstop) {animation_over = true;}
      }
      
      gui.Display();
      if (animation_over) {break;}
   }
   
   gui.Shutdown();
   UpdateInputHandler(0.0);
   SetTimerRate(old_timer_rate);
   if (!timer_running) {
      StopTimer();
   }
   SetTickCount(0);
}










