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
#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/Drawing.hpp"

#include <cmath>

using std::list;
using std::vector;
using std::map;

#ifdef _WIN32
#define M_PI       3.14159265358979323846   // pi
#endif



void DestroyBoundingRectangles(vector<Rectangle*>& vrect) {
   for (unsigned int i = 0 ; i < vrect.size() ; ++i) {
      delete vrect[i];
   }
   vrect.clear();
}



/// ----------------------------------     Area Group class     -----------------------------------



AreaGroup::AreaGroup() :
      areas(),
      del_map()
{}



AreaGroup::~AreaGroup() {
   ClearAreas();
}



bool AreaGroup::Contains(int xpos , int ypos) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      if (a->Contains(xpos,ypos)) {return true;}
   }
   return false;
}



void AreaGroup::MoveBy(int dx , int dy) {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->MoveBy(dx,dy);
   }
}
   


void AreaGroup::Draw(BITMAP* bmp , int color) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->Draw(bmp , color);
   }
}



void AreaGroup::Fill(BITMAP* bmp , int color) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->Fill(bmp , color);
   }
}



vector<Rectangle*> AreaGroup::CreateBoundingRectangles() const {
   vector<Rectangle*> vtotal;
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      vector<Rectangle*> vrect = a->CreateBoundingRectangles();
      for (unsigned int j = 0 ; j < vrect.size() ; ++j) {
         Rectangle* r = vrect[j];
         vtotal.push_back(r);
      }
   }
   return vtotal;
}



AreaBase* AreaGroup::Clone() const {
   AreaGroup* a = new AreaGroup();
   a->CloneAreaGroup(*this);
   return a;
}



void AreaGroup::AddArea(AreaBase* a , bool delete_area) {
   if (a) {
      if (del_map.find(a) == del_map.end()) {
         areas.push_back(a);
      }
      del_map[a] = delete_area;
   }
}



void AreaGroup::RemoveArea(AreaBase* a) {
   if (!a) {return;}
   map<AreaBase* , bool>::iterator it = del_map.find(a);
   if (it != del_map.end()) {
      bool del = it->second;
      if (del) {delete a;}
      del_map.erase(it);
      for (vector<AreaBase*>::iterator vit = areas.begin() ; vit != areas.end() ; ++vit) {
         AreaBase* area = *vit;
         if (a == area) {
            areas.erase(vit);
            break;
         }
      }
   }
}



void AreaGroup::ClearAreas() {
   for (map<AreaBase* , bool>::iterator it = del_map.begin() ; it != del_map.end() ; ++it) {
      AreaBase* a = it->first;
      bool del = it->second;
      if (del) {delete a;}
   }
   del_map.clear();
   areas.clear();
}



void AreaGroup::CloneAreaGroup(const AreaGroup& ag) {
   ClearAreas();
   const vector<AreaBase*>& varea = ag.areas;
   for (unsigned int i = 0 ; i < varea.size() ; ++i) {
      AreaBase* a = varea[i];
      AddArea(a->Clone() , true);
   }
}



void AreaGroup::AdoptAreaGroup(AreaGroup& ag) {
   ClearAreas();
   areas = ag.areas;
   del_map = ag.del_map;
   ag.areas.clear();
   ag.del_map.clear();
}



/// -----------------------------------      Pos2d class       ---------------------------------------------



/// -------------------------------------     Triangle class     ----------------------------------------



void Triangle::CheckPoints() {
   points_unique = true;
   if (p1 == p2) {points_unique = false;}
   if (p1 == p3) {points_unique = false;}
   if (p2 == p3) {points_unique = false;}
   points_make_triangle = points_unique;
}



void Triangle::RenewAngles() {
   CheckPoints();
   if (points_unique) {
      angle_ab = AngleToP2(p1,p2);// -PI to PI
      angle_bc = AngleToP2(p2,p3);// -PI to PI
      angle_ca = AngleToP2(p3,p1);// -PI to PI
      double turn_angle = angle_bc - angle_ab;//[-PI - PI , PI - -PI] = [-2PI , 2PI]
      // Normalize turn angle to (-M_PI , M_PI];
      if (turn_angle <= -M_PI) {turn_angle += 2.0*M_PI;}
      if (turn_angle > M_PI) {turn_angle -= 2.0*M_PI;};
      if ((turn_angle == 0.0) || (turn_angle == M_PI)) {
         // collinear points
         points_make_triangle = false;
      }
      inside_is_positive_angle = (turn_angle > 0.0);
   }
}



Triangle::Triangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3) :
      p1(x1,y1),
      p2(x2,y2),
      p3(x3,y3),
      angle_ab(0.0),
      angle_bc(0.0),
      angle_ca(0.0),
      inside_is_positive_angle(false),
      points_unique(false),
      points_make_triangle(false)
{
   RenewAngles();
}



bool Triangle::Contains(int xpos , int ypos) const {
   if (!points_make_triangle) {return false;}
   Pos2d p4(xpos,ypos);
   const Pos2d* pts[3] = {&p1 , &p2 , &p3};
   double angles[3] = {angle_ab , angle_bc , angle_ca};
   double turn_angle = 0.0;
   double angle_to_p4 = 0.0;
   for (int i = 0 ; i < 3 ; ++i) {
      if (*pts[i] == p4) {return true;}
      angle_to_p4 = AngleToP2(*pts[i] , p4);// -PI to PI
      turn_angle = angle_to_p4 - angles[i];// -2PI to 2PI
      // Normalize turn angle to (-M_PI , M_PI];
      if (turn_angle <= -M_PI) {turn_angle += 2.0*M_PI;}
      if (turn_angle > M_PI) {turn_angle -= 2.0*M_PI;};
      if (inside_is_positive_angle && (turn_angle < 0.0)) {return false;}
      if (!inside_is_positive_angle && (turn_angle > 0.0)) {return false;}
   }
   return true;
}



void Triangle::MoveBy(int dx , int dy) {
   p1.MoveBy(dx,dy);
   p2.MoveBy(dx,dy);
   p3.MoveBy(dx,dy);
}



void Triangle::Draw(BITMAP* bmp , int color) const {
   ASSERT(bmp);
   p1.DrawLineTo(bmp , p2 , color);
   p2.DrawLineTo(bmp , p3 , color);
   p3.DrawLineTo(bmp , p1 , color);
}



void Triangle::Fill(BITMAP* bmp , int color) const {
   ASSERT(bmp);
   if (points_make_triangle) {
      triangle(bmp , p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y() , color);
   }
}



vector<Rectangle*> Triangle::CreateBoundingRectangles() const {
   int lx = p1.X();
   int rx = p1.X();
   int ty = p1.Y();
   int by = p1.Y();
   if (p2.X() < lx) {lx = p2.X();}
   if (p3.X() < lx) {lx = p3.X();}
   if (p2.X() > rx) {rx = p2.X();}
   if (p3.X() > rx) {rx = p3.X();}
   if (p2.Y() < ty) {ty = p2.Y();}
   if (p3.Y() < ty) {ty = p3.Y();}
   if (p2.Y() > by) {by = p2.Y();}
   if (p3.Y() > by) {by = p3.Y();}
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(lx , ty , rx - lx + 1 , by - ty + 1));
   return vrect;
}



AreaBase* Triangle::Clone() const {
   return new Triangle(p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y());
}



void Triangle::SetPos(int x1 , int y1 , int x2 , int y2 , int x3 , int y3) {
   p1.SetPos(x1,y1);
   p2.SetPos(x2,y2);
   p3.SetPos(x3,y3);
   RenewAngles();
}




/// -----------------------------------      Circle clsss         -----------------------------------------



Circle::Circle() : x(0) , y(0) , r(0) {}



Circle::Circle(int xpos , int ypos , int radius) : x(xpos) , y(ypos) , r(radius) {}



bool Circle::Contains(int xpos , int ypos) const {
   float dx = (float)(xpos - x);
   float dy = (float)(ypos - y);
   float dist = sqrt(dx*dx + dy*dy);
   if (dist <= (float)r) {return true;}
   return false;
}



void Circle::MoveBy(int dx , int dy) {
   SetCenter(x + dx , y + dy);
}



void Circle::Draw(BITMAP* bmp , int color) const {
   circle(bmp , x , y , r , color);
}



void Circle::Fill(BITMAP* bmp , int color) const {
   circlefill(bmp , x , y , r , color);
}



vector<Rectangle*> Circle::CreateBoundingRectangles() const {
   int lx = x - r;
   int rx = x + r;
   int ty = y - r;
   int by = y + r;
   int w = rx - lx + 1;
   int h = by - ty + 1;
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(lx , ty , w , h));
   return vrect;
}



AreaBase* Circle::Clone() const {
   return new Circle(x,y,r);
}



void Circle::SetCenter(int xpos , int ypos) {
   x = xpos;
   y = ypos;
}



void Circle::SetRadius(int radius) {
   if (radius < 0) {radius = 0;}
   r = radius;
}



/// --------------------------------      Rectangle class      ----------------------------------------


using std::ostream;
using std::endl;


vector<Rectangle*> Rectangle::CreateBoundingRectangles() const {
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(x,y,w,h));
   return vrect;
}



AreaBase* Rectangle::Clone() const {
   return new Rectangle(x,y,w,h);
}



void Rectangle::SetPos(int xpos , int ypos) {
   x = xpos;
   y = ypos;
   RenewCornerPos();
}



void Rectangle::SetCenterPos (int cxpos , int cypos) {
   SetPos(cxpos - w/2 , cypos - h/2);
}



void Rectangle::SetCorners   (int x1 , int y1 , int x2 , int y2) {
   x = (x1 < x2)?x1:x2;
   y = (y1 < y2)?y1:y2;
   brx = (x1 > x2)?x1:x2;
   bry = (y1 > y2)?y1:y2;
   RenewDimensions();
}



void Rectangle::SetCornerPos (RECT_CORNER_POS rcpos , int xpos , int ypos) {
   int x1 = x;
   int x2 = brx;
   int y1 = y;
   int y2 = bry;
   switch (rcpos) {
      case RECT_CORNER_TL :
         x1 = xpos;
         y1 = ypos;
         break;
      case RECT_CORNER_TR :
         x2 = xpos;
         y1 = ypos;
         break;
      case RECT_CORNER_BR :
         x2 = xpos;
         y2 = ypos;
         break;
      case RECT_CORNER_BL :
         x1 = xpos;
         y2 = ypos;
         break;
   }
   SetCorners(x1,y1,x2,y2);
}



void Rectangle::SetDimensions(int width , int height) {
   w = width;
   h = height;
   RenewCornerPos();
}



void Rectangle::SetArea(int xpos , int ypos , int width , int height) {
   x = xpos;
   y = ypos;
   w = width;
   h = height;
   RenewCornerPos();
}



void Rectangle::SetClipRect(BITMAP* bmp) const {
   ASSERT(bmp);
   set_clip_rect(bmp , x , y , brx , bry);
}



void Rectangle::GetClipRect(BITMAP* bmp) {
   ASSERT(bmp);
   get_clip_rect(bmp , &x , &y , &brx , &bry);
   w = brx - x + 1;
   h = bry - y + 1;
}



void Rectangle::DrawInnerFrame(BITMAP* bmp , unsigned int width , int color) const {
   ASSERT(bmp);
   const int len = width - 1;
   rectfill(bmp , x         , y         , brx     , y + len     , color);
   rectfill(bmp , x         , y + width , x + len , bry - width , color);
   rectfill(bmp , brx - len , y + width , brx     , bry - width , color);
   rectfill(bmp , x         , bry - len , brx     , bry         , color);

/** Old
   ASSERT(bmp);
   for (unsigned int i = 0 ; i < width ; ++i) {
      rect(bmp , x + i , y + i , brx - i , bry - i , color);
   }
//*/
}



void Rectangle::RoundedFill(BITMAP* bmp , int corner_radius , int color) const {
   ASSERT(bmp);
   int mid_tlx = x + corner_radius;
   int mid_tly = y + corner_radius;
   int mid_brx = brx - corner_radius;
   int mid_bry = bry - corner_radius;

   circlefill(bmp , mid_tlx , mid_tly , corner_radius , color);
   circlefill(bmp , mid_brx , mid_tly , corner_radius , color);
   circlefill(bmp , mid_tlx , mid_bry , corner_radius , color);
   circlefill(bmp , mid_brx , mid_bry , corner_radius , color);
   
   rectfill(bmp , x     , mid_tly , mid_tlx , mid_bry , color);
   rectfill(bmp , brx     , mid_tly , mid_brx , mid_bry , color);
   rectfill(bmp , mid_tlx , y     , mid_brx , bry     , color);
}



void Rectangle::RoundedOutline(BITMAP* bmp , int corner_radius  , int color) const {
   ASSERT(bmp);
   int mid_tlx = x + corner_radius;
   int mid_tly = y + corner_radius;
   int mid_brx = brx - corner_radius;
   int mid_bry = bry - corner_radius;
   
//void arc(BITMAP *bmp, int x, y, fixed ang1, ang2, int r, int color);   
   arc(bmp , mid_brx , mid_tly , itofix(0)   , itofix(64)  , corner_radius , color);
   arc(bmp , mid_tlx , mid_tly , itofix(64)  , itofix(128) , corner_radius , color);
   arc(bmp , mid_tlx , mid_bry , itofix(128) , itofix(192) , corner_radius , color);
   arc(bmp , mid_brx , mid_bry , itofix(192) , itofix(256) , corner_radius , color);
   
   hline(bmp , mid_tlx , y       , mid_brx , color);
   hline(bmp , mid_tlx , bry     , mid_brx , color);
   
   vline(bmp , x       , mid_tly , mid_bry , color);
   vline(bmp , brx     , mid_tly , mid_bry , color);
}



void Rectangle::DottedOutline(BITMAP* bmp , int color , bool even) const {
   ASSERT(bmp);
   const int start = (even?0:1);
   for (int xpos = x + start ; xpos < x + w ; xpos += 2) {
      putpixel(bmp , xpos , y , color);
      putpixel(bmp , xpos , y + h - 1 , color);
   }
   for (int ypos = y + start ; ypos < y + h ; ypos += 2) {
      putpixel(bmp , x , ypos , color);
      putpixel(bmp , x + w - 1 , ypos , color);
   }
}



void Rectangle::DrawGuiRectUp(BITMAP* bmp , int fg_color , int sd_color) const {
   ASSERT(bmp);
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(bmp , sd_color);
   upper.Fill(bmp , fg_color);
}



void Rectangle::DrawGuiRectDown(BITMAP* bmp , int fg_color , int sd_color) const {
   ASSERT(bmp);
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(bmp , sd_color);
   upper.Fill(bmp , fg_color);
}



void Rectangle::DrawGuiCircleUp(BITMAP* bmp , int radius , int fg_color , int sd_color) const {
   ASSERT(bmp);
   int cx = x + w/2;
   int cy = y + h/2;
   circlefill(bmp , cx     , cy     , radius     , sd_color);
   circlefill(bmp , cx + 1 , cy + 1 , radius - 1 , sd_color);
   circlefill(bmp , cx - 1 , cy - 1 , radius - 2 , fg_color);
}



void Rectangle::DrawGuiCircleDown(BITMAP* bmp , int radius , int fg_color , int sd_color) const {
   ASSERT(bmp);
   int cx = x + w/2;
   int cy = y + h/2;
   circlefill(bmp , cx , cy , radius     , sd_color);
   circlefill(bmp , cx , cy , radius - 2 , fg_color);
}



void Rectangle::DrawGuiRoundedUp(BITMAP* bmp , int radius , int fg_color , int sd_color) const {
   ASSERT(bmp);
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(bmp , radius , sd_color);
   upper.RoundedFill(bmp , radius , fg_color);
}



void Rectangle::DrawGuiRoundedDown(BITMAP* bmp , int radius , int fg_color , int sd_color) const {
   ASSERT(bmp);
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(bmp , radius , sd_color);
   upper.RoundedFill(bmp , radius , fg_color);
}



void Rectangle::DrawGuiEllipseUp(BITMAP* bmp , int fg_color , int sd_color) const {
   ASSERT(bmp);
   int cx = x + w/2;
   int cy = y + h/2;
   ellipsefill(bmp , cx + 1 , cy + 1 , w/2 - 1 , h/2 - 1 , sd_color);
   ellipsefill(bmp , cx - 1 , cy - 1 , w/2 - 2 , h/2 - 2 , fg_color);
}



void Rectangle::DrawGuiEllipseDown(BITMAP* bmp , int fg_color , int sd_color) const {
   ASSERT(bmp);
   int cx = x + w/2;
   int cy = y + h/2;
   ellipsefill(bmp , cx , cy , w/2 - 1 , h/2 - 1 , sd_color);
   ellipsefill(bmp , cx , cy , w/2 - 2 , h/2 - 2 , fg_color);
}



void Rectangle::DrawGuiTriangleFaceUp   (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const {
   ASSERT(bmp);
   Fill(bmp , fg_color);
   int p1x = x + w/2;
   int p1y = y + h/5;
   int p2x = x + w/5;
   int p2y = y + (4*h)/5;
   int p3x = x + (4*w)/5;
   int p3y = p2y;
   triangle(bmp , p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   triangle_outline(bmp , p1x , p1y , p2x , p2y , p3x , p3y , bg_color);
}



void Rectangle::DrawGuiTriangleFaceRight(BITMAP* bmp , int fg_color , int bg_color , int sd_color) const {
   ASSERT(bmp);
   Fill(bmp , fg_color);
   int p1x = x + (4*w)/5;
   int p1y = y + h/2;
   int p2x = x + w/5;
   int p2y = y + h/5;
   int p3x = p2x;
   int p3y = y + (4*h)/5;
   triangle(bmp , p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   triangle_outline(bmp , p1x , p1y , p2x , p2y , p3x , p3y , bg_color);
}



void Rectangle::DrawGuiTriangleFaceDown (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const {
   ASSERT(bmp);
   Fill(bmp , fg_color);
   int p1x = x + w/2;
   int p1y = y + (4*h)/5;
   int p2x = x + w/5;
   int p2y = y + h/5;
   int p3x = x + (4*w)/5;
   int p3y = p2y;
   triangle(bmp , p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   triangle_outline(bmp , p1x , p1y , p2x , p2y , p3x , p3y , bg_color);
}



void Rectangle::DrawGuiTriangleFaceLeft (BITMAP* bmp , int fg_color , int bg_color , int sd_color) const {
   ASSERT(bmp);
   Fill(bmp , fg_color);
   int p1x = x + w/5;
   int p1y = y + h/2;
   int p2x = x + (4*w)/5;
   int p2y = y + h/5;
   int p3x = p2x;
   int p3y = y + (4*h)/5;
   triangle(bmp , p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   triangle_outline(bmp , p1x , p1y , p2x , p2y , p3x , p3y , bg_color);
}



void Rectangle::DrawGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) const {
   ASSERT(bmp);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   int lr = getr_functions[cdi](lcolor);
   int lg = getg_functions[cdi](lcolor);
   int lb = getb_functions[cdi](lcolor);
   int rr = getr_functions[cdi](rcolor);
   int rg = getg_functions[cdi](rcolor);
   int rb = getb_functions[cdi](rcolor);
   int dr = rr - lr;
   int dg = rg - lg;
   int db = rb - lb;
   double dw = w - 1;
   if (dw == 0) {
      vline(bmp , x , y , bry , lcolor);
      return;
   }
   double rdiff = (double)dr/dw;
   double gdiff = (double)dg/dw;
   double bdiff = (double)db/dw;
   double r = lr;
   double g = lg;
   double b = lb;
   MKCOL_FUNC mkcol = makecol_functions[cdi];
   for (int _x = x ; _x <= brx ; ++_x) {
      int col = mkcol((int)r , (int)g , (int)b);
      vline(bmp , _x , y , bry , col);
      r += rdiff;
      g += gdiff;
      b += bdiff;
   }
}



void Rectangle::DrawGradientRtoL(BITMAP* bmp , int rcolor , int lcolor) const {
   DrawGradientLtoR(bmp , lcolor , rcolor);
}



void Rectangle::DrawGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) const {
   ASSERT(bmp);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   int tr = getr_functions[cdi](tcolor);
   int tg = getg_functions[cdi](tcolor);
   int tb = getb_functions[cdi](tcolor);
   int br = getr_functions[cdi](bcolor);
   int bg = getg_functions[cdi](bcolor);
   int bb = getb_functions[cdi](bcolor);
   int dr = br - tr;
   int dg = bg - tg;
   int db = bb - tb;
   double dh = h - 1;
   if (dh == 0) {
      hline(bmp , x , y , brx , tcolor);
      return;
   }
   double rdiff = (double)dr/dh;
   double gdiff = (double)dg/dh;
   double bdiff = (double)db/dh;
   double r = tr;
   double g = tg;
   double b = tb;
   MKCOL_FUNC mkcol = makecol_functions[cdi];
   for (int _y = y ; _y <= bry ; ++_y) {
      int col = mkcol((int)r , (int)g , (int)b);
      hline(bmp , x , _y , brx , col);
      r += rdiff;
      g += gdiff;
      b += bdiff;
   }
   
}



void Rectangle::DrawGradientBtoT(BITMAP* bmp , int bcolor , int tcolor) const {
   DrawGradientTtoB(bmp , tcolor , bcolor);
}



void Rectangle::DrawGradientOtoM(BITMAP* bmp , int ocolor , int mcolor) const  {
   ASSERT(bmp);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   int ored = getr_functions[cdi](ocolor);
   int ogreen = getg_functions[cdi](ocolor);
   int oblue = getb_functions[cdi](ocolor);
   int mred = getr_functions[cdi](mcolor);
   int mgreen = getg_functions[cdi](mcolor);
   int mblue = getb_functions[cdi](mcolor);
   int dr = mred - ored;
   int dg = mgreen - ogreen;
   int db = mblue - oblue;
   int min = (w < h)?w:h;
   ASSERT(min >= 1);// Minimum dimensions of 1X1 required
   int num = (min + 1)/2;// number of rectangles
   double dmin = (double)(num - 1);
   if (num == 1) {
      rect(bmp , x , y , brx , bry , ocolor);
      return;
   }
   double rdiff = (double)dr/dmin;
   double gdiff = (double)dg/dmin;
   double bdiff = (double)db/dmin;
   double r = ored;
   double g = ogreen;
   double b = oblue;
   int _x = x , _y = y , _brx = brx , _bry = bry;
   MKCOL_FUNC mkcol = makecol_functions[cdi];
   for (int i = 0 ; i < num ; ++i) {
      int col = mkcol((int)r , (int)g , (int)b);
      rect(bmp , _x , _y , _brx , _bry , col);
      ++_x;
      ++_y;
      --_brx;
      --_bry;
      r += rdiff;
      g += gdiff;
      b += bdiff;
   }
}



void Rectangle::DrawGradientMtoO(BITMAP* bmp , int mcolor , int ocolor) const {
   DrawGradientOtoM(bmp , ocolor , mcolor);
}



void Rectangle::DrawGradientFrame(BITMAP* bmp , int num_frames , int start_color , int finish_color) const  {
   ASSERT(bmp);
   if (num_frames == 0) {return;}
   if (num_frames < 0) {
      num_frames = -num_frames;
      Rectangle r(*this);
      /// r(0,0,10,10) num_frames -1 , rg(0,0,10,10) num_frames 1
      /// r(0,0,10,10) num_frames -2 , rg(-1,-1,12,12) num_frames 2
      r.Grow(num_frames - 1);
      r.DrawGradientFrame(bmp , num_frames , finish_color , start_color);
      return;
   }
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   int sr = getr_functions[cdi](start_color);
   int sg = getg_functions[cdi](start_color);
   int sb = getb_functions[cdi](start_color);
   int fr = getr_functions[cdi](finish_color);
   int fg = getg_functions[cdi](finish_color);
   int fb = getb_functions[cdi](finish_color);
   int dr = fr - sr;
   int dg = fg - sg;
   int db = fb - sb;
   if (num_frames == 1) {
      rect(bmp , x , y , brx , bry , start_color);
      return;
   }
   double dnum = num_frames - 1;
   double rdiff = dr/dnum;
   double gdiff = dg/dnum;
   double bdiff = db/dnum;
   double r = sr;
   double g = sg;
   double b = sb;
   MKCOL_FUNC mkcol = makecol_functions[cdi];
   int _x = x , _y = y , _brx = brx , _bry = bry;
   for (int i = 0 ; i < num_frames ; ++i) {
      int col = mkcol((int)r , (int)g , (int)b);
      rect(bmp , _x , _y , _brx , _bry , col);
      ++_x;
      ++_y;
      --_brx;
      --_bry;
      r += rdiff;
      g += gdiff;
      b += bdiff;
   }
}



void Rectangle::DrawAlphaGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) const {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   
   int lr = getr32(lcolor);
   int lg = getg32(lcolor);
   int lb = getb32(lcolor);
   int la = geta32(lcolor);
   int rr = getr32(rcolor);
   int rg = getg32(rcolor);
   int rb = getb32(rcolor);
   int ra = geta32(rcolor);
   int dr = rr - lr;
   int dg = rg - lg;
   int db = rb - lb;
   int da = ra - la;
   double dw = w - 1;
   if (dw == 0) {
      vline(bmp , x , y , bry , lcolor);
      return;
   }
   double rdiff = (double)dr/dw;
   double gdiff = (double)dg/dw;
   double bdiff = (double)db/dw;
   double adiff = (double)da/dw;
   double r = lr;
   double g = lg;
   double b = lb;
   double a = la;
   for (int _x = x ; _x <= brx ; ++_x) {
      int col = makeacol((int)r , (int)g , (int)b , (int)a);
      vline(bmp , _x , y , bry , col);
      r += rdiff;
      g += gdiff;
      b += bdiff;
      a += adiff;
   }
}



void Rectangle::DrawAlphaGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) const {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   
   int tr = getr32(tcolor);
   int tg = getg32(tcolor);
   int tb = getb32(tcolor);
   int ta = geta32(tcolor);
   int br = getr32(bcolor);
   int bg = getg32(bcolor);
   int bb = getb32(bcolor);
   int ba = geta32(bcolor);
   int dr = br - tr;
   int dg = bg - tg;
   int db = bb - tb;
   int da = ba - ta;
   double dh = h - 1;
   if (dh == 0) {
      hline(bmp , x , y , brx , tcolor);
      return;
   }
   double rdiff = (double)dr/dh;
   double gdiff = (double)dg/dh;
   double bdiff = (double)db/dh;
   double adiff = (double)da/dh;
   double r = tr;
   double g = tg;
   double b = tb;
   double a = ta;
   for (int _y = y ; _y <= bry ; ++_y) {
      int col = makeacol((int)r , (int)g , (int)b , (int)a);
      hline(bmp , x , _y , brx , col);
      r += rdiff;
      g += gdiff;
      b += bdiff;
      a += adiff;
   }
}



void Rectangle::DrawAlphaGradientOtoM(BITMAP* bmp , int ocolor , int mcolor) const {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   
   int ored = getr32(ocolor);
   int ogreen = getg32(ocolor);
   int oblue = getb32(ocolor);
   int oalpha = geta32(ocolor);
   int mred = getr32(mcolor);
   int mgreen = getg32(mcolor);
   int mblue = getb32(mcolor);
   int malpha = geta32(mcolor);
   int dr = mred - ored;
   int dg = mgreen - ogreen;
   int db = mblue - oblue;
   int da = malpha - oalpha;
   int min = (w < h)?w:h;
   ASSERT(min >= 1);// Minimum dimensions of 1X1 required
   int num = (min + 1)/2;// number of rectangles
   double dmin = (double)(num - 1);
   if (num == 1) {
      rect(bmp , x , y , brx , bry , ocolor);
      return;
   }
   double rdiff = (double)dr/dmin;
   double gdiff = (double)dg/dmin;
   double bdiff = (double)db/dmin;
   double adiff = (double)da/dmin;
   double r = ored;
   double g = ogreen;
   double b = oblue;
   double a = oalpha;
   int _x = x , _y = y , _brx = brx , _bry = bry;
   for (int i = 0 ; i < num ; ++i) {
      int col = makeacol((int)r , (int)g , (int)b , (int)a);
      rect(bmp , _x , _y , _brx , _bry , col);
      ++_x;
      ++_y;
      --_brx;
      --_bry;
      r += rdiff;
      g += gdiff;
      b += bdiff;
      a += adiff;
   }
}



void Rectangle::DrawMaskedGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) {
   ASSERT(bmp);
   const int maskcolor = bitmap_mask_color(bmp);
   const GPX_FUNC gpx_func = bmp->vtable->getpixel;
   const PPX_FUNC ppx_func = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func = makecol_functions[cdi];
   const GETR_FUNC getr_func = getr_functions[cdi];
   const GETR_FUNC getg_func = getg_functions[cdi];
   const GETR_FUNC getb_func = getb_functions[cdi];
   
   const int lr = getr_func(lcolor);
   const int lg = getg_func(lcolor);
   const int lb = getb_func(lcolor);
   const int rr = getr_func(rcolor);
   const int rg = getg_func(rcolor);
   const int rb = getb_func(rcolor);
   double numsteps = (double)(w - 1);
   if (w == 1) {numsteps = 1.0;}// to prevent divide by zero
   const double dr = (double)(rr - lr)/numsteps;
   const double dg = (double)(rg - lg)/numsteps;
   const double db = (double)(rb - lb)/numsteps;
   double r = (double)lr;
   double g = (double)lg;
   double b = (double)lb;
   int oldcolor = 0 , newcolor = 0;
   acquire_bitmap(bmp);
   for (int _y = y ; _y <= bry ; ++_y) {
      r = (double)lr;
      g = (double)lg;
      b = (double)lb;
      for (int _x = x ; _x <= brx ; ++_x) {
         newcolor = mkcol_func((int)r , (int)g , (int)b);
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
         r += dr;
         g += dg;
         b += db;
      }
   }
   release_bitmap(bmp);
}



void Rectangle::DrawMaskedGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) {
   ASSERT(bmp);
   const int maskcolor = bitmap_mask_color(bmp);
   const GPX_FUNC gpx_func = bmp->vtable->getpixel;
   const PPX_FUNC ppx_func = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func = makecol_functions[cdi];
   const GETR_FUNC getr_func = getr_functions[cdi];
   const GETR_FUNC getg_func = getg_functions[cdi];
   const GETR_FUNC getb_func = getb_functions[cdi];
   
   const int tr = getr_func(tcolor);
   const int tg = getg_func(tcolor);
   const int tb = getb_func(tcolor);
   const int br = getr_func(bcolor);
   const int bg = getg_func(bcolor);
   const int bb = getb_func(bcolor);
   double numsteps = (double)(h - 1);
   if (h <= 1) {numsteps = 1.0;}// to prevent divide by zero
   const double dr = (double)(br - tr)/numsteps;
   const double dg = (double)(bg - tg)/numsteps;
   const double db = (double)(bb - tb)/numsteps;
   double r = (double)tr;
   double g = (double)tg;
   double b = (double)tb;
   int oldcolor = 0 , newcolor = 0;
   acquire_bitmap(bmp);
   for (int _y = y ; _y <= bry ; ++_y) {
      newcolor = mkcol_func((int)r , (int)g , (int)b);
      for (int _x = x ; _x <= brx ; ++_x) {
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
      }
      r += dr;
      g += dg;
      b += db;
   }
   release_bitmap(bmp);
}



void Rectangle::DrawMaskedGradientOtoM(BITMAP* bmp , int ocolor , int mcolor) {
   ASSERT(bmp);
   const int maskcolor = bitmap_mask_color(bmp);
   const GPX_FUNC gpx_func = bmp->vtable->getpixel;
   const PPX_FUNC ppx_func = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func = makecol_functions[cdi];
   const GETR_FUNC getr_func = getr_functions[cdi];
   const GETR_FUNC getg_func = getg_functions[cdi];
   const GETR_FUNC getb_func = getb_functions[cdi];
   
   const int ored   = getr_func(ocolor);
   const int ogreen = getg_func(ocolor);
   const int oblue  = getb_func(ocolor);
   const int mred   = getr_func(mcolor);
   const int mgreen = getg_func(mcolor);
   const int mblue  = getb_func(mcolor);
   int minlength = (w<h)?(w):(h);
   double numframes = (double)((minlength+1)/2);
   double numsteps = numframes - 1.0;
   if (numsteps < 1.0) {numsteps = 1.0;}// to prevent divide by zero
   const double dr = (double)(mred - ored)/numsteps;
   const double dg = (double)(mgreen - ogreen)/numsteps;
   const double db = (double)(mblue - oblue)/numsteps;
   double r = (double)ored;
   double g = (double)ogreen;
   double b = (double)oblue;
   int oldcolor = 0 , newcolor = 0;
   Rectangle R = *this;
   int _x , _y;
   acquire_bitmap(bmp);
   for (int i = 0 ; i < numframes ; ++i) {
      newcolor = mkcol_func((int)r , (int)g , (int)b);
      _y = R.Y();
      for (_x = R.X() ; _x <= R.BRX() ; ++_x) {
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
      }
      _y = R.BRY();
      for (_x = R.X() ; _x <= R.BRX() ; ++_x) {
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
      }
      _x = R.X();
      for (_y = R.Y() ; _y <= R.BRY() ; ++_y) {
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
      }
      _x = R.BRX();
      for (_y = R.Y() ; _y <= R.BRY() ; ++_y) {
         oldcolor = gpx_func(bmp , _x , _y);
         if (oldcolor != maskcolor) {
            ppx_func(bmp , _x , _y , newcolor);
         }
      }
      R.Shrink(1);
      r += dr;
      g += dg;
      b += db;
   }
   release_bitmap(bmp);
}



ostream& operator<<(ostream& os , const Rectangle& r) {
   os << "(x,y,brx,bry,w,h) = " << "(" << r.x << "," << r.y << "," << r.brx << "," << r.bry << "," << r.w << "," << r.h << ")";
   return os;
}



RECT_CONNECT_TYPE DoRectanglesConnect(const Rectangle& r1 , const Rectangle& r2) {
   if ((r1.X() == r2.X()) && (r1.BRX() == r2.BRX())) {
      if (r2.BRY() + 1 == r1.Y()) {return RECT_CONNECT_R2_ON_TOP;}
      if (r1.BRY() + 1 == r2.Y()) {return RECT_CONNECT_R2_ON_BOTTOM;}
   }
   if ((r1.Y() == r2.Y()) && (r1.BRY() == r2.BRY())) {
      if (r1.BRX() + 1 == r2.X()) {return RECT_CONNECT_R2_ON_RIGHT;}
      if (r2.BRX() + 1 == r1.X()) {return RECT_CONNECT_R2_ON_LEFT;}
   }
   return RECT_CONNECT_NONE;
}



RECT_CONTAINS_TYPE DoRectanglesContain(const Rectangle& r1 , const Rectangle& r2) {
   if (r1.Contains(r2)) {return R1_CONTAINS_R2;}
   if (r2.Contains(r1)) {return R2_CONTAINS_R1;}
   return CONTAINS_NONE;
}



list<Rectangle> CombineRectangles(Rectangle r1 , Rectangle r2) {
   list<Rectangle> rlist;

   // Contained rectangles
   RECT_CONTAINS_TYPE contype = DoRectanglesContain(r1,r2);
   if (contype) {
      if (contype == R1_CONTAINS_R2) {rlist.push_back(r1);}
      if (contype == R2_CONTAINS_R1) {rlist.push_back(r2);}
      return rlist;
   }

   // Connecting rectangles
   RECT_CONNECT_TYPE rctype = DoRectanglesConnect(r1,r2);
   if (rctype) {
      if (rctype == RECT_CONNECT_R2_ON_TOP) {
         rlist.push_back(Rectangle(r2.X() , r2.Y() , r2.W() , r1.H() + r2.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_BOTTOM) {
         rlist.push_back(Rectangle(r1.X() , r1.Y() , r1.W() , r1.H() + r2.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_RIGHT) {
         rlist.push_back(Rectangle(r1.X() , r1.Y() , r1.W() + r2.W() , r1.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_LEFT) {
         rlist.push_back(Rectangle(r2.X() , r2.Y() , r1.W() + r2.W() , r2.H()));
      }
      return rlist;
   }
   
   // Overlapping Rectangles
   /// w = brx - x + 1
   /// h = bry - y + 1
   if (r1.Overlaps(r2)) {
      // Overlapping aligned rectangles combine to produce one rectangle
      // Vertical alignment
      if ((r1.X() == r2.X()) && (r1.BRX() == r2.BRX())) {
         const Rectangle* top = (r1.Y() < r2.Y())?&r1:&r2;
         const Rectangle* bot = (r1.BRY() > r2.BRY())?&r1:&r2;
         rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , bot->BRY() - top->Y() + 1));
         return rlist;
      }
      // Horizontal alignment
      if ((r1.Y() == r2.Y()) && (r1.BRY() == r2.BRY())) {
         const Rectangle* left = (r1.X() < r2.X())?&r1:&r2;
         const Rectangle* right = (r1.BRX() > r2.BRX())?&r1:&r2;
         rlist.push_back(Rectangle(left->X() , left->Y() , right->BRX() - left->X() + 1 , left->H()));
         return rlist;
      }
      // Overlapping offset rectangles
      const Rectangle* temin = (r1.Y() < r2.Y())?&r1:&r2;// top edge min y value
      const Rectangle* temax = (r1.Y() > r2.Y())?&r1:&r2;// top edge max y value
      const Rectangle* lemin = (r1.X() < r2.X())?&r1:&r2;// left edge min x value
      const Rectangle* lemax = (r1.X() > r2.X())?&r1:&r2;// left edge max x value
      const Rectangle* bemin = (r1.BRY() < r2.BRY())?&r1:&r2;// bottom edge min y value
      const Rectangle* bemax = (r1.BRY() > r2.BRY())?&r1:&r2;// bottom edge max y value
      const Rectangle* remin = (r1.BRX() < r2.BRX())?&r1:&r2;// right edge min x value
      const Rectangle* remax = (r1.BRX() > r2.BRX())?&r1:&r2;// right edge max x value
      if (r1.Y() == r2.Y()) {
         // When the tops are even, only two unique rectangles are formed
         // new top rectangle formed by min left edge , max right edge , either top edge , and the min bottom edge
         rlist.push_back(Rectangle(lemin->X() , r1.Y() , remax->BRX() - lemin->X() + 1 , bemin->BRY() - r1.Y() + 1));
         // new bot rectangle formed by bottom rectangles left and right edges , top.BRY() + 1 , and bot.BRY()
         rlist.push_back(Rectangle(bemax->X() , bemin->BRY() + 1 , bemax->BRX() - bemax->X() + 1 , bemax->BRY() - (bemin->BRY() + 1) + 1));
      } else if (r1.BRY() == r2.BRY()) {
         // When the bottoms are even, only two unique rectangles are formed
         // new top rectangle formed by top rectangles left and right edges , top.Y() and bot.Y() - 1
         rlist.push_back(Rectangle(temin->X() , temin->Y() , temin->BRX() - temin->X() + 1 , (temax->Y() - 1) - temin->Y() + 1));
         // new bot rectangle formed by min left edge , max right edge , bot.Y() and either bottom edge
         rlist.push_back(Rectangle(lemin->X() , temax->Y() , remax->BRX() - lemin->X() + 1 , r1.BRY() - temax->Y() + 1));
      } else if (r1.X() == r2.X()) {
         // When the left edges are even, two vertical rectangles are formed
         rlist.push_back(Rectangle(r1.X() , temin->Y() , remin->BRX() - r1.X() + 1 , bemax->BRY() - temin->Y() + 1));
         rlist.push_back(Rectangle(remin->BRX() + 1 , remax->Y() , remax->BRX() - (remin->BRX() + 1) + 1 , remax->H()));
      } else if (r1.BRX() == r2.BRX()) {
         // When the right edges are even, two vertical rectangles are formed
         rlist.push_back(Rectangle(lemin->X() , lemin->Y() , (lemax->X() - 1) - lemin->X() + 1 , lemin->H()));
         rlist.push_back(Rectangle(lemax->X() , temin->Y() , r1.BRX() - lemax->X() + 1 , bemax->BRY() - temin->Y() + 1));
      } else {
         // One rectangle is on top and one rectangle is on left
         const Rectangle* top = temin;
         const Rectangle* bot = temax;
         const Rectangle* left = lemin;
         const Rectangle* right = lemax;
         if (((top->Y() <= bot->Y()) && (top->BRY() >= bot->BRY())) && ((left->X() <= right->X()) && (left->BRX() >= right->BRX()))) {
            // The two rectangles cross in the middle, forming three rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , bot->Y() , bot->W() , bot->H()));
            rlist.push_back(Rectangle(top->X() , bot->BRY() + 1 , top->W() , top->BRY() - (bot->BRY() + 1) + 1));
         } else if ((top->X() <= bot->X()) && (top->BRX() >= bot->BRX())) {
            // Upper rectangle horizontally surrounds lower rectangle, forming two rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , top->H()));
            rlist.push_back(Rectangle(bot->X() , top->BRY() + 1 , bot->W() , bot->BRY() - (top->BRY() + 1) + 1));
         } else if ((bot->X() <= top->X()) && (bot->BRX() >= top->BRX())) {
            // Lower rectangle horizontally surrounds upper rectangle, forming two rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , bot->Y() , bot->W() , bot->H()));
         } else if ((left->Y() <= right->Y()) && (left->BRY() >= right->BRY())) {
            // Left rectangle vertically surrounds right rectangle, forming two rectangles
            rlist.push_back(Rectangle(left->X() , left->Y() , left->W() , left->H()));
            rlist.push_back(Rectangle((left->BRX() + 1) , right->Y() , right->BRX() - (left->BRX() + 1) + 1 , right->H()));
         } else if ((right->Y() <= left->Y()) && (right->BRY() >= left->BRY())) {
            // Right rectangle vertically surrounds left rectangle, forming two rectangles
            rlist.push_back(Rectangle(left->X() , left->Y() , (right->X() - 1) - left->X() + 1 , left->H()));
            rlist.push_back(Rectangle(right->X() , right->Y() , right->W() , right->H()));
         } else {
            // only one corner of each overlaps the other, forming three rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(lemin->X() , bot->Y() , remax->BRX() - lemin->X() + 1 , top->BRY() - bot->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , top->BRY() + 1 , bot->W() , bot->BRY() - (top->BRY() + 1) + 1));
         }
      }
      return rlist;
   }

   /// If it gets this far, the two rectangles can not be combined, so return an empty list to signal this
   ///rlist.push_back(r1);
   ///rlist.push_back(r2);
   return rlist;
}



/** Fallback version of ConsolidateRectangles, Keep for now
list<Rectangle> ConsolidateRectangles(list<Rectangle> rectlist) {
   if (rectlist.size() <= 1) {return rectlist;}
   
   list<Rectangle>::iterator it1 , it2 , add;
   for (it1 = rectlist.begin() ; it1 != rectlist.end() ; ++it1) {
      while (true) {
         ++it2;
         if (it2 == rectlist.end()) {break;}
         list<Rectangle> clist = CombineRectangles(*it1 , *it2);
         if (!clist.empty()) {
            rectlist.erase(it1);
            rectlist.erase(it2);
            for (add = clist.begin() ; add != clist.end() ; ++add) {
               rectlist.push_back(*add);
            }
            it1 = rectlist.begin();
            it2 = it1;
         }
      }
   }
   return rectlist;
}
//*/



list<Rectangle> ConsolidateRectangles(list<Rectangle> rectlist) {
   if (rectlist.size() <= 1) {return rectlist;}
   
   list<Rectangle>::iterator it1 , it2 , next;
   list<Rectangle> addlist;
   it1 = rectlist.begin();
   while (it1 != rectlist.end()) {
      it2 = it1;
      while (true) {
         ++it2;
         if (it2 == rectlist.end()) {break;}
         list<Rectangle> clist = CombineRectangles(*it1 , *it2);
         if (!clist.empty()) {
            next = it1;
            ++next;
            if (next == it2) {++next;}
            rectlist.erase(it1);
            rectlist.erase(it2);
            addlist.splice(addlist.end() , clist);
            it1 = next;
            it2 = it1;
            if (next == rectlist.end()) {break;}
         }
      }
      next = it1;
      if (it1 != rectlist.end()) {
         ++it1;
         ++next;
      }
      if (next == rectlist.end()) {
         if (!addlist.empty()) {
            rectlist.splice(rectlist.end() , addlist);
            it1 = rectlist.begin();
            it2 = it1;
         }
      }
   }
   
   return rectlist;
}






