


#include "Eagle/GuiCode/UtilityWidgets.hpp"

#include "Eagle/GuiCode/WidgetHandler.hpp"
#include "Eagle/InputHandler.hpp"

using std::string;

WIDGET_GRAB_AREA GetGrabbedArea(WidgetBase* w , Rectangle area , int x , int y) {
   ASSERT(w);
   
   if (!area.Contains(x,y)) {return GRAB_NONE;}
   Rectangle test;
   int size = 3;
   UINT flags = w->Flags();
   bool moveable = flags & MOVEABLE;
   bool resizeable = flags & RESIZEABLE;
   
   if (resizeable) {
      // SE
      test.SetCorners(area.BRX() , area.BRY() , area.BRX() - size , area.BRY() - size);
      if (test.Contains(x,y)) {return GRAB_SE;}
      
      // NE
      test.SetCorners(area.BRX() , area.Y() , area.BRX() - size , area.Y() + size);
      if (test.Contains(x,y)) {return GRAB_NE;}
      
      // SW
      test.SetCorners(area.X() , area.BRY() , area.X() + size , area.BRY() - size);
      if (test.Contains(x,y)) {return GRAB_SW;}
      
      // NW
      test.SetCorners(area.X() , area.Y() , area.X() + size , area.Y() + size);
      if (test.Contains(x,y)) {return GRAB_NW;}
      
      // E
      test.SetCorners(area.BRX() , area.Y() , area.BRX() - size , area.BRY());
      if (test.Contains(x,y)) {return GRAB_E;}
      
      // W
      test.SetCorners(area.X() , area.Y() , area.X() + size , area.BRY());
      if (test.Contains(x,y)) {return GRAB_W;}
      
      // N
      test.SetCorners(area.X() , area.Y() , area.BRX() , area.Y() + size);
      if (test.Contains(x,y)) {return GRAB_N;}
      
      // S
      test.SetCorners(area.X() , area.BRY() , area.BRX() , area.BRY() - size);
      if (test.Contains(x,y)) {return GRAB_S;}
   }
   if (moveable) {
      return GRAB_MOVE;
   }
   return GRAB_NONE;
}



/// ---------------------------------    WidgetAdjuster class    ---------------------------------------




WidgetAdjuster::WidgetAdjuster(string name) :
      WidgetBase(name , Rectangle(0,0,1,1) , FLAGS_NONE),
      grabbed(0),
      mouse_owner_id(AcquireMouseOwnerId()),
      is_move(false),
      x1(0),
      x2(0),
      y1(0),
      y2(0),
      sx(0),
      sy(0),
      smx(0),
      smy(0),
      px(0),
      py(0)
{}



WidgetMsg WidgetAdjuster::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg WidgetAdjuster::CheckInputs(int msx , int msy) {
   bool input_used = false;
   int retmsg = DIALOG_OKAY;

   if (flags & ENABLED) {
      if (grabbed) {
         if (input_mouse_release(LMB)) {
            grabbed = 0;
         }
         else {
            if (is_move) {
               grabbed->SetDrawPos(sx + (mx - smx) , sy + (my - smy));
            }
            else {
               if (px) {
                  *px = sx + (mx - smx);
               }
               if (py) {
                  *py = sy + (my - smy);
               }
               Rectangle r;
               r.SetCorners(x1,y1,x2,y2);
               grabbed->SetArea(r);
            }
         }
         return WidgetMsg(this , TOPIC_DIALOG , DIALOG_INPUT_USED);
      }
      else {
         // if not grabbed
         // find widget under mouse
         // check hover areas for left mouse click to grab
         WidgetHandler* gui = NearestParentGui();
         if (gui) {
            WidgetBase* w = gui->GetWidgetAt(msx,msy);
            WidgetBase* w2 = w;
            if (w && w != gui) {
               Rectangle a = w->Area();
               // make msx and msy relative to the space the widget is in
               while (1) {
                  w2 = w2->Parent();
                  ASSERT(w2);
                  if (w2 != gui) {
                     msx -= w2->Area().X();
                     msy -= w2->Area().Y();
//                     a.MoveBy(w2->Area().X() , w2->Area().Y());
                  } else {
                     break;
                  }
               }
               WIDGET_GRAB_AREA grab_area = GetGrabbedArea(w , a , msx , msy);
               if (grab_area != GRAB_NONE) {
                  GrabMousePointer(mouse_owner_id);
                  if (input_mouse_press(LMB)) {
                     grabbed = w;
                     x1 = a.X();
                     y1 = a.Y();
                     x2 = a.BRX();
                     y2 = a.BRY();
                     smx = mx;
                     smy = my; 
                     px = 0;
                     py = 0;
                     is_move = false;
                     input_used = true;
                  }
               } else {
                  ReleaseMousePointer(mouse_owner_id);
               }
               switch (grab_area) {
                  case GRAB_NW :
                     UseMousePointer(POINTER_SENW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x1;
                        sy = y1;
                        px = &x1;
                        py = &y1;
                     }
                     break;
                  case GRAB_N :
                     UseMousePointer(POINTER_NS , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = 0;
                        sy = y1;
                        px = 0;
                        py = &y1;
                     }
                     break;
                  case GRAB_NE :
                     UseMousePointer(POINTER_NESW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x2;
                        sy = y1;
                        px = &x2;
                        py = &y1;
                     }
                     break;
                  case GRAB_E :
                     UseMousePointer(POINTER_EW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x2;
                        sy = 0;
                        px = &x2;
                        py = 0;
                     }
                     break;
                  case GRAB_SE :
                     UseMousePointer(POINTER_SENW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x2;
                        sy = y2;
                        px = &x2;
                        py = &y2;
                     }
                     break;
                  case GRAB_S :
                     UseMousePointer(POINTER_NS , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = 0;
                        sy = y2;
                        px = 0;
                        py = &y2;
                     }
                     break;
                  case GRAB_SW :
                     UseMousePointer(POINTER_NESW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x1;
                        sy = y2;
                        px = &x1;
                        py = &y2;
                     }
                     break;
                  case GRAB_W :
                     UseMousePointer(POINTER_EW , mouse_owner_id);
                     if (input_mouse_press(LMB)) {
                        sx = x1;
                        sy = 0;
                        px = &x1;
                        py = 0;
                     }
                     break;
                  case GRAB_MOVE :
                     if (input_mouse_press(LMB)) {
                        UseMousePointer(POINTER_GRABBED , mouse_owner_id);
                        sx = x1;
                        sy = y1;
                        is_move = true;
                     }
                     else {
                        UseMousePointer(POINTER_GRAB , mouse_owner_id);
                     }
                     break;
                  case GRAB_NONE :
                     // do nothing
                     break;
               }
            } else {
               ReleaseMousePointer(mouse_owner_id);
            }
         }
         
      }
      if (input_used) {retmsg |= DIALOG_INPUT_USED;}
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void WidgetAdjuster::DisplayOn(BITMAP* bmp , int x , int y) {
   // Not visible
}



void WidgetAdjuster::SetEnabledState(bool state) {
   if (!state) {
      grabbed = 0;
      ReleaseMousePointer(mouse_owner_id);
   }
   WidgetBase::SetEnabledState(state);
}



void DefaultSplitterDraw(BITMAP* bmp , Rectangle r , SPLITTER_TYPE split_type , const WidgetColorset& wc) {
   ASSERT(bmp);
   ASSERT(r.W() > 0);
   ASSERT(r.H() > 0);
   
   int lx = r.X();
   int midx = r.CX();
   int rx = r.BRX();
   int ty = r.Y();
   int midy = r.CY();
   int by = r.BRY();
   int lx2 = 0;
   int ty2 = 0;
   
   int hlcol = wc[HLCOL];
   int sdcol = wc[SDCOL];
   
   switch (split_type) {
      case SPLIT_LEFTRIGHT :
         if (r.W() < 2) {
            vline(bmp , lx , ty , by , hlcol);
         }
         else {
            // cx = x + w/2
            if ((r.W() % 2) == 0) {
               // 0 1 2 3 (0 + 4/2 = 2)
               lx2 = midx - 1;
            }
            else {
               // 0 1 2 (0 + 2/1 = 1)
               lx2 = midx;
            }
            Rectangle l;
            Rectangle r2;
            l.SetCorners(lx,ty,lx2,by);
            r2.SetCorners(midx,ty,rx,by);
//   void DrawGradientLtoR(BITMAP* bmp , int lcolor , int rcolor) const ;
            l.DrawGradientLtoR(bmp , sdcol , hlcol);
            r2.DrawGradientLtoR(bmp , hlcol , sdcol);
         }
         break;
      case SPLIT_TOPBOTTOM :
         if (r.H() < 2) {
            hline(bmp , lx , ty , rx , hlcol);
         }
         else {
            // cy = y + h/2
            if ((r.H() % 2) == 0) {
               // 0 1 2 3 (0 + 4/2 = 2)
               ty2 = midy - 1;
            }
            else {
               // 0 1 2 (0 + 3/2 = 1)
               ty2 = midy;
            }
            Rectangle t;
            Rectangle b;
            t.SetCorners(lx , ty , rx , ty2);
            b.SetCorners(lx , midy , rx , by);
//   void DrawGradientTtoB(BITMAP* bmp , int tcolor , int bcolor) const ;
            t.DrawGradientTtoB(bmp , sdcol , hlcol);
            b.DrawGradientTtoB(bmp , hlcol , sdcol);
         }
         break;
   }
}



/// -----------------------------------     Splitter class     ---------------------------------------



void Splitter::SetupArea() {
   int ulw = wupleft->Area().W();
   int ulh = wupleft->Area().H();
   int drw = wdownright->Area().W();
   int drh = wdownright->Area().H();
   int maxh = (ulh>drh)?ulh:drh;
   int maxw = (ulw>drw)?ulw:drw;
   int tw = ulw + drw;
   int th = ulh + drh;
   int totalw = 0;
   int totalh = 0;
   switch (splitter_type) {
      case SPLIT_LEFTRIGHT :
         totalw = tw + gapsz;
         totalh = maxh;
         WidgetBase::SetDrawDimensions(totalw , totalh);
         wupleft->SetDrawPos(0,0);
         wdownright->SetDrawPos(wupleft->Area().W() + gapsz , 0);
         if (wupleft->Area().H() < maxh) {
            wupleft->SetDrawDimensions(wupleft->Area().W() , maxh);
         }
         if (wdownright->Area().H() < maxh) {
            wdownright->SetDrawDimensions(wdownright->Area().W() , maxh);
         }
         split_percent = (double)tw/(double)ulw;
         gap.SetArea(ulw , 0 , gapsz , totalh);
         break;
      case SPLIT_TOPBOTTOM :
         totalw = maxw;
         totalh = th + gapsz;
         WidgetBase::SetDrawDimensions(totalw , totalh);
         wupleft->SetDrawPos(0,0);
         wdownright->SetDrawPos(0 , wupleft->Area().H() + gapsz);
         if (wupleft->Area().W() < maxw) {
            wupleft->SetDrawDimensions(maxw , wupleft->Area().H());
         }
         if (wdownright->Area().W() < maxw) {
            wdownright->SetDrawDimensions(maxw , wdownright->Area().H());
         }
         split_percent = (double)th/(double)ulh;
         gap.SetArea(0 , ulh , totalw , gapsz);
         break;
   }
}



Splitter::Splitter(string name , SPLITTER_TYPE type , WidgetBase* upleft , WidgetBase* downright , WidgetBase* gets_input_first ,
                   int gapsize , int xpos , int ypos , SPLITTER_DRAWING_CALLBACK callback , UINT wflags) :
      WidgetBase(name , Rectangle(xpos,ypos,1,1) , wflags),
      mouse_owner_id(AcquireMouseOwnerId()),
      wupleft(0),
      wdownright(0),
      wztop(0),
      wzbottom(0),
      splitter_type(type),
      gap(),
      gapsz(0),
      split_percent(0.0),
      drawing_callback(callback)
{
   if (gapsize < 1) {gapsize = 1;}
   gapsz = gapsize;
   
   SetWidgets(upleft , downright , gets_input_first);
}



WidgetMsg Splitter::Update(double tsec) {
   int retmsg = DIALOG_OKAY;
   retmsg |= wztop->Update(tsec).Messages();
   retmsg |= wzbottom->Update(tsec).Messages();
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg Splitter::CheckInputs(int msx , int msy) {
   int retmsg = DIALOG_OKAY;
   retmsg |= (wztop->CheckInputs(msx - area.X() , msy - area.Y())).Messages();
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= (wzbottom->CheckInputs(msx - area.X() , msy - area.Y())).Messages();
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void Splitter::DisplayOn(BITMAP* bmp , int x , int y) {
   wzbottom->DisplayOn(bmp , x + area.X() , y + area.Y());
   wztop->DisplayOn(bmp , x + area.X() , y + area.Y());
   drawing_callback(bmp , gap , splitter_type , *wcols);
}



bool Splitter::Init() {
   bool i1 = wztop->Init();
   bool i2 = wzbottom->Init();
   return (i1 && i2);
}



bool Splitter::Ready() {
   bool i1 = wztop->Ready();
   bool i2 = wzbottom->Ready();
   return (i1 && i2);
}



bool Splitter::Shutdown() {
   bool i1 = wztop->Shutdown();
   bool i2 = wzbottom->Shutdown();
   return (i1 && i2);
}



void Splitter::SetWidgets(WidgetBase* upleft , WidgetBase* downright , WidgetBase* gets_input_first) {
   ASSERT(upleft);
   ASSERT(downright);
   
   wupleft = upleft;
   wdownright = downright;
   if (gets_input_first && ((gets_input_first == wupleft) || (gets_input_first == wdownright))) {
      wztop = gets_input_first;
      (wztop == wupleft)?
      wzbottom = wdownright:
      wzbottom = wupleft;
   }
   else {
      wztop = wupleft;
      wzbottom = wdownright;
   }
   SetupArea();
}



