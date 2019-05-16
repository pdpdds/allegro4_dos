

#include "Graphing.hpp"

#include <cmath>



FunctionBase* FuncAdapter::f = 0;



/// -----------------------------------      Plotter class     -------------------------------------------



void Plotter::ResetScales() {
   double viewxrange = (double)area.W();
   double viewyrange = (double)area.H();
/* Should be taken care of already
   if (viewxrange == 0.0) {throw EagleError("Plotter::ResetScales - view x range is 0.0!\n");}
   if (viewyrange == 0.0) {throw EagleError("Plotter::ResetScales - view y range is 0.0!\n");}
   if (xrange == 0.0) {throw EagleError("Plotter::ResetScales - x range is 0.0!\n");}
   if (yrange == 0.0) {throw EagleError("Plotter::ResetScales - y range is 0.0!\n");}
*/
   viewx_to_x_scale = xrange/viewxrange;
   viewy_to_y_scale = yrange/viewyrange;
   x_to_viewx_scale = viewxrange/xrange;
   y_to_viewy_scale = viewyrange/yrange;
}



bool Plotter::CheckReady() {
   if (eval && (area.W() > 0) && (area.H() > 0) && (xrange > 0.0) && (yrange > 0.0)) {
      return true;
   }
   return false;
}



Plotter::Plotter(std::string name , Rectangle position) :
   WidgetBase(name , position),
   xmin(0.0) , xmax(0.0) , xrange(0.0),
   ymin(0.0) , ymax(0.0) , yrange(0.0),
   centerx(0.0) , centery(0.0),
   viewx_to_x_scale(0.0),
   viewy_to_y_scale(0.0),
   x_to_viewx_scale(0.0),
   y_to_viewy_scale(0.0),
   f(0),
   adapter(),
   use_adapter(false),
   eval(0),
   valuemap(),
   var(),
   show_axes(true),
   ready(false),
   drag(false),
   vxstart(0),
   vystart(0)
{
   int w = area.W();
   int h = area.H();
   if ((w < 1) || (h < 1)) {
      if (w < 1) {w = 1;}
      if (h < 1) {h = 1;}
      WidgetBase::SetDrawDimensions(w,h);
   }
}



void Plotter::SetView(double x_min , double x_max , double y_min , double y_max) {
   double temp = 0.0;
   if (x_max < x_min) {
      temp = x_min;
      x_min = x_max;
      x_max = temp;
   }
   if (y_max < y_min) {
      temp = y_min;
      y_min = y_max;
      y_max = temp;
   }
   if (x_min == x_max) {throw EagleError("Plotter::SetView - x range is 0.0!\n");}
   if (y_min == y_max) {throw EagleError("Plotter::SetView - y range is 0.0!\n");}
   xmin = x_min;
   xmax = x_max;
   ymin = y_min;
   ymax = y_max;
   xrange = xmax - xmin;
   yrange = ymax - ymin;
   centerx = (xmin + xmax)/2.0;
   centery = (ymin + ymax)/2.0;
   ResetScales();
   ready = CheckReady();
   SetRedrawFlag();
}



void Plotter::SetFunction(EVALFUNC function) {
   eval = function;
   use_adapter = false;
   ready = CheckReady();
}


void Plotter::SetFunction(FunctionBase* function) {
   f = function;
   use_adapter = true;
   eval = &FuncAdapter::Evaluate;
   ready = CheckReady();
}



double Plotter::x_to_viewx(double x) {
   return ((x - xmin)*x_to_viewx_scale) + (double)area.X();
   // viewx = (x - xmin)*x_to_viewx_scale + viewmin;
   // (viewx - viewmin)/x_to_viewx_scale + xmin = x;
}



double Plotter::y_to_viewy(double y) {
   return (double)(area.Y() + area.H()) - (y - ymin)*y_to_viewy_scale;
   // viewy = (viewymin + viewyheight) - (y - ymin)*y_to_viewy_scale
   // (viewy - (viewymin + viewyheight))/-y_to_viewy_scale + ymin = y
   // (viewymin + viewyheight - viewy)*viewy_to_y_scale + ymin = y
}



double Plotter::viewx_to_x(double viewx) {
   return (viewx - (double)area.X())*viewx_to_x_scale + xmin;
}



double Plotter::viewy_to_y(double viewy) {
   return ((double)(area.Y() + area.H()) - viewy + (double)area.Y())*viewy_to_y_scale + ymin;
}



void Plotter::Plot(BITMAP* bmp , int x , int y) {
   double prevxpos = 0.0;
   double prevypos = 0.0;
   SOLVER_RESULT prevsr;
   int prevx = 0;
   int prevy = 0;
   // background - BGCOL
   // axes - HLCOL
   // plot - FGCOL
   // undefined - MGCOL
   
   adapter.SetFunction(f);
   
   area.Fill(bmp , (*wcols)[BGCOL]);
   
   if (!ready) {
      textout_centre_ex(bmp , font , "Not Ready!" , area.CX() + x , area.CY() - text_height(font)/2 + y , (*wcols)[TXTCOL] , -1);
      textprintf_centre_ex(bmp , font , area.CX() + x , area.CY() + text_height(font)/2 + y + 4 , (*wcols)[TXTCOL] , -1 ,
                           "%s %s" , (eval?"":"No function.") , (((xrange == 0.0) || (yrange == 0.0))?"Range invalid.":""));
      ClearRedrawFlag();
      return;
   }
   
   Rectangle oldclip = GetClippingRectangle(bmp);
   Rectangle r = area;
   r.MoveBy(x,y);
   r.SetClipRect(bmp);
   
   if (show_axes) {
      int zerox = (int)x_to_viewx(0.0);
      int zeroy = (int)y_to_viewy(0.0);
      vline(bmp , zerox , area.Y() + y , area.BRY() + y , (*wcols)[HLCOL]);
      hline(bmp , area.X() + x , zeroy , area.BRX() + x , (*wcols)[HLCOL]);
   }
   
   prevx = area.X();
   prevxpos = (double)area.X() + 0.5;
   valuemap[var] = prevxpos;
   prevsr = eval(valuemap , &prevypos);
   if (prevsr == RESULT_VALID) {
      double viewy = y_to_viewy(prevypos);
      prevy = (int)viewy;
      if ((double)prevy == viewy) {
         --prevy;// This makes it so that pixels are displayed upwards if they are at the bottom edge instead of downward
      }
      putpixel(bmp , prevx + x , prevy + y, (*wcols)[FGCOL]);
   }
   else if (prevsr == RESULT_UNDEFINED) {
      vline(bmp , prevx + x , area.Y() + y , area.BRY() + y , (*wcols)[MGCOL]);
   }
   for (int viewx = area.X() + 1; viewx <= area.BRX() ; ++viewx) {
      double xpos = viewx_to_x((double)viewx + 0.5);
      double ypos = 0.0;
      int viewy = 0;
      valuemap[var] = xpos;
      SOLVER_RESULT sr = eval(valuemap , &ypos);
      if (sr == RESULT_VALID) {
         // latest result valid
         double vy = y_to_viewy(ypos);
         viewy = (int)vy;
         if ((double)viewy == vy) {
            --viewy;// This makes it so that pixels are displayed upwards if they are at the bottom edge instead of downward
         }
         if (prevsr == RESULT_VALID) {
            line(bmp , prevx + x , prevy + y, viewx + x, viewy + y, (*wcols)[FGCOL]);
         } else if (prevsr == RESULT_UNDEFINED) {
            putpixel(bmp , viewx + x , viewy + y, (*wcols)[FGCOL]);
         }
      } else if (sr == RESULT_UNDEFINED) {
         // latest result undefined
         vline(bmp , viewx + x , area.Y() , area.BRY() , (*wcols)[MGCOL]);
      }
      prevx = viewx;
      prevy = viewy;
      prevsr = sr;
   }
   
   oldclip.SetClipRect(bmp);
   ClearRedrawFlag();
}



WidgetMsg Plotter::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg Plotter::CheckInputs(int msx , int msy) {
   
   if (area.Contains(msx,msy)) {
      // check for LMB drag
      if (input_mouse_press(LMB)) {
         drag = true;
         vxstart = mouse_x;
         vystart = mouse_y;
      }
      // check for mouse wheel
      if (ms_zdiff) {
         double factor = pow(1.5 , -ms_zdiff);
         double newxrange = xrange*factor;
         double newyrange = yrange*factor;
         SetView(centerx - newxrange/2.0 , centerx + newxrange/2.0 , centery - newyrange/2.0 , centery + newyrange/2.0);
      }
   }
   
   if (drag && input_mouse_held(LMB)) {
      /// To drag, we need to know how many pixels the pointer has moved,
      /// multiply the dx and dy by the scale to get the actual dx and dy
      /// then move the top left corner by that much
      /// after that move the mouse pointer back to the starting position
      /// so we can keep moving indefinitely while the LMB is held down
      double dx = ((double)(vxstart - mouse_x))*viewx_to_x_scale;
      double dy = ((double)(mouse_y - vystart))*viewy_to_y_scale;
      if ((mouse_x - vxstart) || (vystart - mouse_y)) {
         SetView(xmin + dx , xmax + dx , ymin + dy , ymax + dy);
         vxstart = mouse_x;
         vystart = mouse_y;
//         position_mouse(vxstart , vystart);
      }
   }
   if (drag && input_mouse_release(LMB)) {
      drag = false;
   }
   
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void Plotter::DisplayOn(BITMAP* bmp , int x , int y) {
   Plot(bmp , x , y);
}
