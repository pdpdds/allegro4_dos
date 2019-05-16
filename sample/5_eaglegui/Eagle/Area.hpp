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


#ifndef Area_H
#define Area_H



#include <allegro.h>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <cmath>


/// -----------------------------     AreaBase class     --------------------------------



class Rectangle;

class AreaBase {

protected :

public :
   AreaBase() {}
   virtual ~AreaBase() {}
   
   virtual bool Contains(int xpos , int ypos) const =0;
   virtual void MoveBy(int dx , int dy)=0;
   virtual void Draw(BITMAP* bmp , int color) const =0;
   virtual void Fill(BITMAP* bmp , int color) const =0;
   // CreateBoundingRectangles creates a vector of NEW rectangles, destroy with DestroyBoundingRectangles
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const =0;
   virtual AreaBase* Clone() const =0;// returns a newed copy of itself
   
};



void DestroyBoundingRectangles(std::vector<Rectangle*>& vrect);



/// ----------------------------     AreaGroup class      --------------------------------



class AreaGroup : public AreaBase {
   
protected :
   std::vector<AreaBase*> areas;
   std::map<AreaBase* , bool> del_map;
   
   
public :
   AreaGroup();
   ~AreaGroup();
   
   /// Overloads from AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual void MoveBy(int dx , int dy);
   virtual void Draw(BITMAP* bmp , int color) const;
   virtual void Fill(BITMAP* bmp , int color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;


   /// Member functions
   void AddArea(AreaBase* a , bool delete_area = true);
   void RemoveArea(AreaBase* a);
   void ClearAreas();
   void CloneAreaGroup(const AreaGroup& ag);
   void AdoptAreaGroup(AreaGroup& ag);// takes ownership of ag's areas and clears ag of data
   
   int size() {return areas.size();}
};



/// -------------------------------------     Pos2d class     ------------------------------------------



class Pos2d {
protected :
   int x;
   int y;
   
public :
   Pos2d() : x(0) , y(0) {}
   Pos2d(int xpos , int ypos) : x(xpos) , y(ypos) {}
   
   inline bool operator==(const Pos2d& p) const ;
   
   inline void SetPos(int xpos , int ypos);
   inline void MoveBy(int dx , int dy);

   inline void Draw(BITMAP* bmp , int color) const;
   inline void DrawLineTo(BITMAP* bmp , const Pos2d& p , int color) const;

   inline double AngleToPoint(const Pos2d& p) const;
   inline double AngleToPoint(int x , int y) const;

   inline double DistanceToPoint(const Pos2d& p) const;
   inline double DistanceToPoint(int xpos , int ypos) const;

   int X() const {return x;}
   int Y() const {return y;}
};



inline bool Pos2d::operator==(const Pos2d& p) const {
   return ((x == p.x) && (y == p.y));
}

inline void Pos2d::SetPos(int xpos , int ypos) {
   x = xpos;
   y = ypos;
}

inline void Pos2d::MoveBy(int dx , int dy) {
   SetPos(x + dx , y + dy);
}

inline void Pos2d::Draw(BITMAP* bmp , int color) const {
   ASSERT(bmp);
   bmp->vtable->putpixel(bmp , x , y , color);
}

inline void Pos2d::DrawLineTo(BITMAP* bmp , const Pos2d& p , int color) const {
   ASSERT(bmp);
   line(bmp , x , y , p.x , p.y , color);
}

inline double Pos2d::AngleToPoint(const Pos2d& p) const {
   if (*this == p) {return 0.0;}// same point, no angle
   return atan2(p.y - y , p.x - x);
}

inline double Pos2d::AngleToPoint(int xpos , int ypos) const {
   return atan2(ypos - y , xpos - x);
}

inline double Pos2d::DistanceToPoint(const Pos2d& p) const {
   int dx = p.x - x;
   int dy = p.y - y;
   return sqrt((double)(dx*dx + dy*dy));
}

inline double Pos2d::DistanceToPoint(int xpos , int ypos) const {
   int dx = xpos - x;
   int dy = ypos - y;
   return sqrt((double)(dx*dx + dy*dy));
}

inline double AngleToP2(const Pos2d& p1 , const Pos2d& p2) {return p1.AngleToPoint(p2);}
   



/// -------------------------------------     Line class       -----------------------------------------



class Line : public AreaBase {
protected :
   Pos2d p1;
   Pos2d p2;
   
public :
   /// TODO : Working here...
};



/// -------------------------------------     Triangle class     ----------------------------------------



class Triangle : public AreaBase {

   
protected :
   Pos2d p1;
   Pos2d p2;
   Pos2d p3;
   double angle_ab;
   double angle_bc;
   double angle_ca;
   bool inside_is_positive_angle;
   bool points_unique;
   bool points_make_triangle;

   void CheckPoints();
   void RenewAngles();

public :
   
   Triangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);
   
   virtual bool Contains(int xpos , int ypos) const;
   virtual void MoveBy(int dx , int dy);
   virtual void Draw(BITMAP* bmp , int color) const;
   virtual void Fill(BITMAP* bmp , int color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;
   
   
   void SetPos(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);
};


/// -----------------------------------      Circle clsss         -----------------------------------------



class Circle : public AreaBase {
protected :
   int x;
   int y;
   int r;
   
public :
   
   Circle();
   Circle(int xpos , int ypos , int radius);
   ~Circle() {}
   
   /// Overloads from AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual void MoveBy(int dx , int dy);
   virtual void Draw(BITMAP* bmp , int color) const;
   virtual void Fill(BITMAP* bmp , int color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;

   /// Member functions
   void SetCenter(int xpos , int ypos);
   void SetRadius(int radius);
   


};


/// -----------------------------------      Rectangle class      ------------------------------------------


enum RECT_CORNER_POS {
   RECT_CORNER_TL = 0,
   RECT_CORNER_TR = 1,
   RECT_CORNER_BR = 2,
   RECT_CORNER_BL = 3
};


class Rectangle : public AreaBase {
private :
   int x,y,w,h,brx,bry;
   
   inline void RenewCornerPos();
   inline void RenewDimensions();

   
public :
   
   Rectangle() : AreaBase() , x(0) , y(0) , w(0) , h(0) , brx(0) , bry(0) {}
   
   Rectangle(int xpos , int ypos , int width , int height) : AreaBase() , x(xpos) , y(ypos) , w(width) , h(height) {RenewCornerPos();}

   Rectangle(BITMAP* bmp) : x(0) , y(0) , w(0) , h(0) , brx(0) , bry(0) {
      ASSERT(bmp);
      w = bmp->w;
      h = bmp->h;
      RenewCornerPos();
   }

   Rectangle(const Rectangle& r) : AreaBase() , x(r.x) , y(r.y) , w(r.w) , h(r.h) , brx(r.brx) , bry(r.bry) {}

   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;// returns a newed copy of itself

   inline bool DimensionsEqual(const Rectangle& r) const {
      return ((w == r.w) && (h == r.h));
   }
   
   inline bool AreaEqual(const Rectangle& r) const {
      return (Area() == r.Area());
   }

   int X()   const {return x;}
   int Y()   const {return y;}
   int W()   const {return w;}
   int H()   const {return h;}
   int BRX() const {return brx;}
   int BRY() const {return bry;}
   int CX()  const {return (x + w/2);}
   int CY()  const {return (y + h/2);}
   int Area() const {return w*h;}


   void SetPos       (int xpos  , int ypos);
   void SetCenterPos (int cxpos , int cypos);
   void SetCorners   (int x1 , int y1 , int x2 , int y2);
   void SetCornerPos (RECT_CORNER_POS rcpos , int xpos , int ypos);
   void SetDimensions(int width , int height);
   void SetArea      (int xpos  , int ypos , int width , int height);

   inline void SetArea(const Rectangle& r);

   inline void MoveBy(int dx , int dy);

   inline void Shrink(int delta);
   inline void Grow(int delta);

   inline bool Overlaps(const Rectangle& r) const;

   inline bool Contains(int xpos , int ypos) const;
   inline bool Contains(const Rectangle& r) const;
   
   void SetClipRect(BITMAP* bmp) const ;
   void GetClipRect(BITMAP* bmp);

   inline void Draw(BITMAP* bmp , int color) const;

   inline void OffsetDraw(BITMAP* bmp , int ox , int oy , int color) const;

   inline void Fill(BITMAP* bmp , int color) const;

   void DrawInnerFrame(BITMAP* bmp , unsigned int width , int color) const ;
   void RoundedFill   (BITMAP* bmp , int corner_radius  , int color) const ;
   void RoundedOutline(BITMAP* bmp , int corner_radius  , int color) const ;
   void DottedOutline(BITMAP* bmp , int color , bool even = true) const;

   /// DrawGui* functions that draw the shape with a shadow underneath to the right
   void DrawGuiRectUp     (BITMAP* bmp , int fg_color , int sd_color) const ;
   void DrawGuiRectDown   (BITMAP* bmp , int fg_color , int sd_color) const ;
   void DrawGuiCircleUp   (BITMAP* bmp , int radius   , int fg_color , int sd_color) const ;
   void DrawGuiCircleDown (BITMAP* bmp , int radius   , int fg_color , int sd_color) const ;
   void DrawGuiRoundedUp  (BITMAP* bmp , int radius   , int fg_color , int sd_color) const ;
   void DrawGuiRoundedDown(BITMAP* bmp , int radius   , int fg_color , int sd_color) const ;
   void DrawGuiEllipseUp  (BITMAP* bmp , int fg_color , int sd_color) const ;
   void DrawGuiEllipseDown(BITMAP* bmp , int fg_color , int sd_color) const ;

   /// Draws outlined triangular buttons
   void DrawGuiTriangleFaceUp   (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const ;
   void DrawGuiTriangleFaceRight(BITMAP* bmp , int fg_color , int bg_color , int sd_color) const ;
   void DrawGuiTriangleFaceDown (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const ;
   void DrawGuiTriangleFaceLeft (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const ;

   /// Gradient drawing for solid colors
   void DrawGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) const ;
   void DrawGradientRtoL(BITMAP* bmp , int rcolor , int lcolor) const ;
   void DrawGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) const ;
   void DrawGradientBtoT(BITMAP* bmp , int bcolor , int tcolor) const ;
   void DrawGradientOtoM(BITMAP* bmp , int ocolor , int mcolor) const ;
   void DrawGradientMtoO(BITMAP* bmp , int mcolor , int ocolor) const ;

   void DrawGradientFrame(BITMAP* bmp , int num_frames , int start_color , int finish_color) const ;// num_frames may be negative

   /// Gradient drawing for alpha colors
   void DrawAlphaGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) const ;
   void DrawAlphaGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) const;
   void DrawAlphaGradientOtoM(BITMAP* bmp , int ocolor , int mcolor) const;

   /// Gradient drawing for masked bitmaps, good for coloring text, skips source pixels that are the mask color.
   void DrawMaskedGradientLtoR(BITMAP* bmp , int lcolor , int rcolor);
   void DrawMaskedGradientTtoB(BITMAP* bmp , int tcolor , int bcolor);
   void DrawMaskedGradientOtoM(BITMAP* bmp , int ocolor , int mcolor);


   friend std::ostream& operator<<(std::ostream& os , const Rectangle& r);
};



/// Inline Rectangle:: class methods

inline void Rectangle::RenewCornerPos() {
   brx = x + w - 1;// w = brx - x + 1
   bry = y + h - 1;// h = bry - y + 1
}

inline void Rectangle::RenewDimensions() {
   w = brx - x + 1;
   h = bry - y + 1;
}

inline void Rectangle::SetArea(const Rectangle& r) {
   *this = r;
}

inline void Rectangle::MoveBy(int dx    , int dy) {
   SetPos(x + dx , y + dy);
}

inline void Rectangle::Shrink(int delta) {
   x += delta;
   y += delta;
   w -= 2*delta;
   h -= 2*delta;
   RenewCornerPos();
}

inline void Rectangle::Grow(int delta) {
   x -= delta;
   y -= delta;
   w += 2*delta;
   h += 2*delta;
   RenewCornerPos();
}

inline bool Rectangle::Overlaps(const Rectangle& r) const {
   return !(((x > r.brx) || (brx < r.x)) || ((y > r.bry) || (bry < r.y)));
   // if true, (x <= r.brx) && (brx >= r.x) && (y <= r.bry) && (bry >= r.y)
}

inline bool Rectangle::Contains(int xpos , int ypos) const {
   return !(((xpos < x) || (xpos > brx)) || ((ypos < y) || (ypos > bry)));
}

inline bool Rectangle::Contains(const Rectangle& r) const {
//   return (Contains(r.x,r.y) && Contains(r.brx,r.y) && Contains(r.x,r.bry) && Contains(r.brx,r.bry));
   return !((r.x < x) || (r.y < y) || (r.brx > brx) || (r.bry > bry));
}

inline void Rectangle::Draw(BITMAP* bmp , int color) const {
   rect(bmp , x , y , brx , bry , color);
}

inline void Rectangle::OffsetDraw(BITMAP* bmp , int ox , int oy , int color) const {
   rect(bmp , x + ox , y + oy , brx + ox , bry + oy , color);
}

inline void Rectangle::Fill(BITMAP* bmp , int color) const {
   rectfill(bmp , x , y , brx , bry , color);
}

/// Global methods 
inline bool DimensionsEqual(const Rectangle& r1 , const Rectangle& r2) {
   return r1.DimensionsEqual(r2);
}

inline bool Overlaps(const Rectangle& r1 , const Rectangle& r2) {
   return r1.Overlaps(r2);
}

inline bool Overlaps(int min1 , int max1 , int min2 , int max2) {
   return !((min2 > max1) || (max2 < min1));
}

inline bool Contains(int min1 , int max1 , int value) {
   return !((value > max1) || (value < min1));
}

inline bool Contains(const Rectangle& r , int xpos , int ypos) {
   return r.Contains(xpos,ypos);
}

inline Rectangle OffsetRectangle(const Rectangle& r , int dx , int dy) {
   Rectangle R(r);
   R.MoveBy(dx,dy);
   return R;
}

inline Rectangle GetClippingRectangle(BITMAP* bmp) {
   Rectangle r;
   r.GetClipRect(bmp);
   return r;
}

enum RECT_CONNECT_TYPE {
   RECT_CONNECT_NONE         = 0,
   RECT_CONNECT_R2_ON_TOP    = 1,
   RECT_CONNECT_R2_ON_BOTTOM = 2,
   RECT_CONNECT_R2_ON_RIGHT  = 3,
   RECT_CONNECT_R2_ON_LEFT   = 4
};

enum RECT_CONTAINS_TYPE {
   CONTAINS_NONE  = 0,
   R1_CONTAINS_R2 = 1,
   R2_CONTAINS_R1 = 2
};

// returns true if the rectangles are adjacent and aligned
RECT_CONNECT_TYPE DoRectanglesConnect(const Rectangle& r1 , const Rectangle& r2);

RECT_CONTAINS_TYPE DoRectanglesContain(const Rectangle& r1 , const Rectangle& r2);

std::list<Rectangle> CombineRectangles(Rectangle r1 , Rectangle r2);

std::list<Rectangle> ConsolidateRectangles(std::list<Rectangle> rectlist);




#endif // Area_H

