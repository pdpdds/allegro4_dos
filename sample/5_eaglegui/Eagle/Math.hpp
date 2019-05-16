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


#ifndef Math_H
#define Math_H

#include <allegro.h>



extern const double TWO_PI;
extern const double PI_OVER_TWO;


double DistanceBetween(double x1 , double y1 , double x2 , double y2);



template<class ValueT>
inline bool RangeOverlaps(const ValueT& min1, const ValueT& max1, const ValueT& min2 , const ValueT& max2) {
  return (!((min1 > max2) || (min2 > max1)));
}



enum SOLVER_RESULT {
   RESULT_VALID     = 0,
   RESULT_UNDEFINED = 1,
   RESULT_ERROR     = 2
};



class GeneralLineEquation {
protected :
   double x1;
   double y1;
   double x2;
   double y2;
   
   double A;
   double B;
   double C;
   
   double theta;
   
   bool is_line_segment;
   
   void Normalize();
   
public :
   GeneralLineEquation();// basic line y = 0
   GeneralLineEquation(double xpos1 , double ypos1 , double xpos2 , double ypos2 , int* result);// result will hold RESULT_ERROR if pos1 and pos2 are the same
   GeneralLineEquation(double xpos1 , double ypos1 , double angle);// always works
   GeneralLineEquation(double A1 , double B1 , double C1 , int* result);// result will hold RESULT_ERROR if A and B are equal to zero

   SOLVER_RESULT SetPosition(double xpos1 , double ypos1 , double xpos2 , double ypos2);// pos1 and pos2 must be unique or you get RESULT_ERROR
   void SetPositionAndAngle(double xpos1 , double ypos1 , double angle);
   SOLVER_RESULT SetABC(double A1 , double B1 , double C1);// A and B can't both be 0 or you get RESULT_ERROR
   
   inline GeneralLineEquation PerpendicularThrough(double xpos , double ypos) const ;
   inline GeneralLineEquation ParallelThrough(double xpos , double ypos) const ;

   SOLVER_RESULT IntersectionWith(const GeneralLineEquation& gle , double* storex , double* storey) const ;// returns RESULT_UNDEFINED if lines are equal or parallel

   double DistanceTo(double xpos , double ypos) const ;// works for line segments too
   double DistanceTo(const GeneralLineEquation& gle) const ;// 0.0 if they intersect, otherwise the distance between the parallel lines

   
   SOLVER_RESULT XatY(double ypos , double* storex) const ;// returns RESULT_UNDEFINED if there is no valid x for the given ypos
   SOLVER_RESULT YatX(double xpos , double* storey) const ;// returns RESULT_UNDEFINED if there is no valid y for the given xpos

};



inline GeneralLineEquation GeneralLineEquation::PerpendicularThrough(double xpos , double ypos) const {
   return GeneralLineEquation(xpos,ypos,theta + PI_OVER_TWO);
}

inline GeneralLineEquation GeneralLineEquation::ParallelThrough(double xpos , double ypos) const {
   return GeneralLineEquation(xpos , ypos , theta);
}


#endif // Math_H

