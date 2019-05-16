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


#include "Eagle/Math.hpp"

#include <cmath>

#ifdef _WIN32
#define M_PI       3.14159265358979323846   // pi
#endif

extern const double TWO_PI = 2.0*M_PI;
extern const double PI_OVER_TWO = M_PI/2.0;



double DistanceBetween(double x1 , double y1 , double x2 , double y2) {
   const double dx = x2 - x1;
   const double dy = y2 - y1;
   return sqrt(dx*dx + dy*dy);
}



/// --------------------------------- GeneralLineEquation class  -----------------------------------------


void GeneralLineEquation::Normalize() {
   if (A != 0.0) {
      double one_over_a = 1.0/A;
      A = 1.0;
      B *= one_over_a;
      C *= one_over_a;
   } else if (B != 0.0) {
      double one_over_b = 1.0/B;
      A *= one_over_b;
      B = 1.0;//b*(1/b) = 1
      C *= one_over_b;
   }
}



GeneralLineEquation::GeneralLineEquation() :
      x1(0),
      y1(0),
      x2(0),
      y2(0),
      A(0.0),
      B(1.0),
      C(0.0),
      theta(0.0),
      is_line_segment(false)
{}



GeneralLineEquation::GeneralLineEquation(double xpos1 , double ypos1 , double xpos2 , double ypos2 , int* result) :
      x1(0),
      y1(0),
      x2(0),
      y2(0),
      A(0.0),
      B(1.0),
      C(0.0),
      theta(0.0),
      is_line_segment(false)
{
   int r = SetPosition(xpos1 , ypos1 , xpos2 , ypos2);
   if (result) {
      *result = r;
   }
}



GeneralLineEquation::GeneralLineEquation(double xpos1 , double ypos1 , double angle) :
      x1(0),
      y1(0),
      x2(0),
      y2(0),
      A(0.0),
      B(1.0),
      C(0.0),
      theta(0.0),
      is_line_segment(false)
{
   SetPositionAndAngle(xpos1 , ypos1 , angle);
}



GeneralLineEquation::GeneralLineEquation(double A1 , double B1 , double C1 , int* result) :
      x1(0),
      y1(0),
      x2(0),
      y2(0),
      A(0.0),
      B(1.0),
      C(0.0),
      theta(0.0),
      is_line_segment(false)
{
   int r = SetABC(A1 , B1 , C1);
   if (result) {
      *result = r;
   }
}



SOLVER_RESULT GeneralLineEquation::SetPosition(double xpos1 , double ypos1 , double xpos2 , double ypos2) {
   if ((xpos1 == xpos2) && (ypos1 == ypos2)) {
      return RESULT_ERROR;
   }
   x1 = xpos1;
   y1 = ypos1;
   x2 = xpos2;
   y2 = ypos2;
   A = y1 - y2;
   B = x2 - x1;
   C = x1*y2 - x2*y1;
   theta = atan2(y2-y1 , x2-x1);
   if (theta <= 0.0) {theta += TWO_PI;}
   is_line_segment = true;
   Normalize();
   return RESULT_VALID;
}



void GeneralLineEquation::SetPositionAndAngle(double xpos1 , double ypos1 , double angle) {
   if (angle > TWO_PI) {
      angle = angle - TWO_PI*((double)(int)(angle/TWO_PI));
   }
   if (angle < TWO_PI) {
      angle = angle + TWO_PI*((double)((int)(-angle/TWO_PI) + 1));
   }
   x1 = xpos1;
   y1 = ypos1;
   theta = angle;
   A = -sin(theta);
   B = cos(theta);
   C = -A*x1 - B*y1;
   is_line_segment = false;
   Normalize();
}



SOLVER_RESULT GeneralLineEquation::SetABC(double A1 , double B1 , double C1) {
   if ((A1 == 0.0) && (B1 == 0.0)) {return RESULT_ERROR;}
   A = A1;
   B = B1;
   C = C1;
   Normalize();
   theta = atan2(-A,B);
   is_line_segment = false;
   return RESULT_VALID;
}



SOLVER_RESULT GeneralLineEquation::IntersectionWith(const GeneralLineEquation& gle , double* storex , double* storey) const {
   double D = gle.A;
   double E = gle.B;
   double F = gle.C;
   double ae_minus_bd = A*E - B*D;
   if (ae_minus_bd == 0.0) {return RESULT_UNDEFINED;}
   if (storex) {
      *storex = (B*F - C*E)/ae_minus_bd;
   }
   if (storey) {
      *storey = (A*F - C*D)/-ae_minus_bd;
   }
   return RESULT_VALID;
}



double GeneralLineEquation::DistanceTo(double xpos , double ypos) const {
   GeneralLineEquation perp = PerpendicularThrough(xpos,ypos);
   double ix = 0.0;
   double iy = 0.0;
   int result = IntersectionWith(perp , &ix , &iy);
   ASSERT(result == RESULT_VALID);// this should always be true
   if (result != RESULT_VALID) {return 0.0;}
   
   double dist_from_line = DistanceBetween(xpos , ypos , ix , iy);
   if (!is_line_segment) {// is line
      return dist_from_line;
   }
   // is line segment
   double lx = (x1 < x2)?x1:x2;
   double rx = (x1 > x2)?x1:x2;
   double ty = (y1 < y2)?y1:y2;
   double by = (y1 > y2)?y1:y2;
   if ((ix >= lx) && (ix <= rx) && (iy >= ty) && (iy <= by)) {// closest point is on line segment
      return dist_from_line;
   }
   // closest point is off line segment, return minimum distance from endpoints
   double dist_p1 = DistanceBetween(xpos , ypos , x1 , y1);
   double dist_p2 = DistanceBetween(xpos , ypos , x2 , y2);
   return (dist_p1 <= dist_p2)?dist_p1:dist_p2;
}



double GeneralLineEquation::DistanceTo(const GeneralLineEquation& gle) const {
   if (IntersectionWith(gle , 0 , 0) == RESULT_VALID) {
      return 0.0;
   }
   // lines are parallel
   double X1 = 0.0;
   double Y1 = 0.0;
   if (YatX(0.0 , &Y1) == RESULT_UNDEFINED) {// find y intercept
      ASSERT(XatY(0.0 , &X1) != RESULT_UNDEFINED);// find x intercept
      // Every line has to cross one of the axes somewhere
   }
   return gle.DistanceTo(X1,Y1);
}



SOLVER_RESULT GeneralLineEquation::XatY(double ypos , double* storex) const {
   if (A == 0.0) {return RESULT_UNDEFINED;}
   if (storex) {
      *storex = (-C - B*ypos)/A;
   }
   return RESULT_VALID;
}



SOLVER_RESULT GeneralLineEquation::YatX(double xpos , double* storey) const {
   if (B == 0.0) {return RESULT_UNDEFINED;}
   if (storey) {
      *storey = (-C - A*xpos)/B;
   }
   return RESULT_VALID;
}





