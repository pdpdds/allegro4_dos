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


#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/WidgetHandler.hpp"

using std::string;



const unsigned int TOPIC_DIALOG = NextFreeTopicId();

const UINT BASIC_FLAGS = ENABLED | VISIBLE;
const UINT DEFAULT_FLAGS = ENABLED | VISIBLE | MOVEABLE | RESIZEABLE;
const UINT DEFAULT_OVERLAP_FLAGS = DEFAULT_FLAGS | ALLOW_OVERLAP;


/** -------------   Constructors   -------------*/

WidgetBase::WidgetBase(string name) :
      wname(name),
      parent(0),
      messagehandler(0),
      wc(DefaultWidgetColors()),
      wcols(&wc),
      dest(0),
      area(0,0,1,1),
      minw(0),
      minh(0),
      flags(DEFAULT_FLAGS),
      display_priority(MID_DISPLAY_PRIORITY)
   {}



WidgetBase::WidgetBase(string name , const Rectangle& position, UINT wflags) :
      wname(name),
      parent(0),
      messagehandler(0),
      wc(DefaultWidgetColors()),
      wcols(&wc),
      dest(0),
      area(position),
      minw(0),
      minh(0),
      flags(wflags),
      display_priority(MID_DISPLAY_PRIORITY)
   {}



/// TODO : Ehhh... This constructor should probably be removed, most widgets don't use it anyway
WidgetBase::WidgetBase(string name , BITMAP* surface , const Rectangle& position, UINT wflags) :
      wname(name),
      parent(0),
      messagehandler(0),
      wc(DefaultWidgetColors()),
      wcols(&wc),
      dest(surface),
      area(position),
      minw(0),
      minh(0),
      flags(wflags),
      display_priority(MID_DISPLAY_PRIORITY)
   {}


/** Calling a virtual function from a constructor may cause problems in the derived class.
      If a derived class constructor uses a base class constructor that calls a virtual function, 
      then the wrong method may be called because the object is not fully constructed. I don't see
      how that is causing the problem here though, because the virtual method being called is on an object
      that has already been fully constructed??? Not the problem , see below.
      Try moving the AddWidget call outside of the constructors and see if that fixes this error :
      "Invalid parameter passed to C runtime function." The call stack is strange at the source of the error -
      it starts in an iostream function and then mentions something about a cxa pure virtual frame, and then
      the pure virtual method that it is failing in, since it fails in all of the pure virtual methods for some reason.
      
      The reason you can't use AddWidget on this is because AddWidget calls virtual methods on the object *this and
      *this hasn't been fully constructed yet. It sucks but it can't be helped.
      
      The finally derived classes could use dialog->AddWidget but it isn't that hard for the user to do it manually.
      
WidgetBase::WidgetBase(WidgetBase* dialog , BITMAP* surface , const Rectangle& position, UINT wflags) :
               parent(0) , wc(DefaultWidgetColors()) , wcols(&wc) , dest(surface) , area(position) ,flags(wflags) {
   if (dialog) {dialog->AddWidget(this);}
}
*/



bool WidgetBase::Init() {return true;}
bool WidgetBase::Ready() {return true;}
bool WidgetBase::Shutdown() {return true;}



void WidgetBase::QueueUserMessage(const WidgetMsg& wmsg) {
   if (messagehandler) {messagehandler->QueueUserMessage(wmsg);}
   else if (parent) {parent->QueueUserMessage(wmsg);}
}



void WidgetBase::SetWidgetColors(const WidgetColorset& colors) {
   wc = colors;
   wcols = &wc;
   SetBgRedrawFlag();
}



void WidgetBase::UseWidgetColorset(WidgetColorset* colorset) {
   ASSERT(colorset);
   if (colorset) {
      wcols = colorset;
      SetBgRedrawFlag();
   }
}



void WidgetBase::UseIndividualColorset() {
   wcols = &wc;
   SetBgRedrawFlag();
}



void WidgetBase::SetDrawTarget(BITMAP* surface) {
   dest = surface;
}



void WidgetBase::SetDrawPos(int xpos , int ypos) {
   if (flags & MOVEABLE) {
/// Uncomment to prevent widget overlap
///      Rectangle r(xpos,ypos,Area().W(),Area().H());
///      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
///      if (!pwh || (pwh && pwh->AreaFree(r , this))) {
         if (flags & VISIBLE) {
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
            if (pwh) {pwh->MakeAreaDirty(area);}
         }
         area.SetPos(xpos,ypos);
         if (flags & VISIBLE) {
            SetBgRedrawFlag();
         }
///      }
   }
}



void WidgetBase::SetDrawDimensions(int width , int height) {
   if (flags & RESIZEABLE) {
/// Uncomment to prevent widget overlap
///      Rectangle r(Area().X() , Area().Y() , width , height);
///      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
///      if (!pwh || (pwh && pwh->AreaFree(r , this))) {
         if (width < minw) {width = minw;}
         if (height < minh) {height = minh;}
         if (flags & VISIBLE) {
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
            if (pwh) {pwh->MakeAreaDirty(area);}
         }
         area.SetDimensions(width , height);
         if (flags & VISIBLE) {SetBgRedrawFlag();}
//      SetRedrawAllFlag();
///      }
   }
}



void WidgetBase::SetArea(const Rectangle& r) {
   int w = r.W();
   if (w < minw) {w = minw;}
   int h = r.H();
   if (h < minh) {h = minh;}
   Rectangle r2(r.X() , r.Y() , w , h);
   if ((flags & MOVEABLE) && !(flags & RESIZEABLE)) {
      WidgetBase::SetDrawPos(r2.X() , r2.Y());
      return;
   }
   if (!(flags & MOVEABLE) && (flags & RESIZEABLE)) {
      WidgetBase::SetDrawDimensions(r2.W() , r2.H());
      return;
   }
   if ((flags & MOVEABLE) && (flags & RESIZEABLE)) {
/// Uncomment to prevent widget overlap
///      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
///      if (!pwh || (pwh && pwh->AreaFree(r , this))) {
         if (flags & VISIBLE) {
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
            if (pwh) {pwh->MakeAreaDirty(area);}
         }
         area = r2;
         if (flags & VISIBLE) {SetBgRedrawFlag();}
//      SetRedrawAllFlag();
///      }
   }
}



void WidgetBase::SetArea(int xpos , int ypos , int width , int height) {
   if (width < minw) {width = minw;}
   if (height < minh) {height = minh;}
   
   if ((flags & MOVEABLE) && !(flags & RESIZEABLE)) {
      WidgetBase::SetDrawPos(xpos , ypos);
      return;
   }
   if (!(flags & MOVEABLE) && (flags & RESIZEABLE)) {
      WidgetBase::SetDrawDimensions(width , height);
      return;
   }
   if ((flags & MOVEABLE) && (flags & RESIZEABLE)) {
/// Uncomment to prevent widget overlap
///      Rectangle r(xpos,ypos,width,height);
///      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
///      if (!pwh || (pwh && pwh->AreaFree(r , this))) {
         if (flags & VISIBLE) {
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
            if (pwh) {pwh->MakeAreaDirty(area);}
         }
         area.SetArea(xpos , ypos , width , height);
         if (flags & VISIBLE) {SetBgRedrawFlag();}
//      SetRedrawAllFlag();
///      }
   }
}



void WidgetBase::SetFlagStates(UINT FLAGS , bool state) {
   
   /** FLAGS contains the flags that you wish to change state, and state is whether they will be
   set to true or false.
   */
   
   UINT newflags = (
      state?
      // Set these flags to true
      (flags | FLAGS):
      // Set these flags to false
      (flags & ~FLAGS)
   );
   
   // Which flags changed state?
   UINT changed = flags^newflags;
   // Which state did they change to?
   if (changed) {
      if (changed & ENABLED)       {SetEnabledState     (newflags & ENABLED      );}
      if (changed & VISIBLE)       {SetVisibilityState  (newflags & VISIBLE      );}
      if (changed & HOVER)         {SetHoverState       (newflags & HOVER        );}
      if (changed & HASFOCUS)      {SetFocusState       (newflags & HASFOCUS     );}
      if (changed & MOVEABLE)      {SetMoveableState    (newflags & MOVEABLE     );}
      if (changed & RESIZEABLE)    {SetResizeableState  (newflags & RESIZEABLE   );}
      if (changed & NEEDS_REDRAW)  {SetNeedsRedrawState (newflags & NEEDS_REDRAW );}
      if (changed & ALLOW_CLOSE)   {SetAllowCloseState  (newflags & ALLOW_CLOSE  );}
      if (changed & ALLOW_OVERLAP) {SetAllowOverlapState(newflags & ALLOW_OVERLAP);}
   }
}



void WidgetBase::SetEnabledState(bool state) {
   const UINT FLAG = ENABLED;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      SetBgRedrawFlag();
   }
}



void WidgetBase::SetHoverState(bool state) {
   const UINT FLAG = HOVER;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
/*
      SetRedrawFlag();
      if (!state) {
         SetBgRedrawFlag();
      }
//*/
   }
}



void WidgetBase::SetVisibilityState(bool state) {
   const UINT FLAG = VISIBLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (!state) {
         WidgetHandler* wh = NearestParentGui();
         if (wh) {wh->MakeAreaDirty(area);}
      } else {
         SetRedrawFlag();
      }
//      SetBgRedrawFlag();
   }
}



void WidgetBase::SetFocusState(bool state) {
   const UINT FLAG = HASFOCUS;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      SetRedrawFlag();
      if (!state) {
         SetBgRedrawFlag();
      }
   }
}



void WidgetBase::SetMoveableState(bool state) {
   const UINT FLAG = MOVEABLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetResizeableState(bool state) {
   const UINT FLAG = RESIZEABLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetNeedsRedrawState(bool state) {
   const UINT FLAG = NEEDS_REDRAW;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (state) {
         if (parent) {parent->SetRedrawFlag();}
      }
   }
}



void WidgetBase::SetNeedsBgRedrawState(bool state) {
   const UINT FLAG = NEEDS_BG_REDRAW;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (state) {
         WidgetBase::SetNeedsRedrawState(true);
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(parent);
         if (parent) {
            if (pwh) {
               pwh->MakeAreaDirty(area);
            } else {
               parent->SetNeedsBgRedrawState(true);
            }
         }
      }
   }
}


void WidgetBase::SetAllowCloseState(bool state) {
   const UINT FLAG = ALLOW_CLOSE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetAllowOverlapState(bool state) {
   const UINT FLAG = ALLOW_OVERLAP;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetRedrawFlag() {
   SetNeedsRedrawState(true);
}



void WidgetBase::SetBgRedrawFlag() {
   SetNeedsBgRedrawState(true);
}



void WidgetBase::ClearRedrawFlag() {
   SetNeedsRedrawState(false);
   SetNeedsBgRedrawState(false);
}



void WidgetBase::SetRedrawAllFlag() {
   if (parent) {parent->SetRedrawAllFlag();}
}



void WidgetBase::ShowWidget() {
   SetEnabledState(true);
   SetVisibilityState(true);
}



void WidgetBase::HideWidget() {
   SetEnabledState(false);
   SetVisibilityState(false);
}



void WidgetBase::ToggleWidgetVisibility() {
   SetEnabledState(!(flags & ENABLED));
   SetVisibilityState(!(flags & VISIBLE));
}



bool WidgetBase::IsMouseOver(int msx , int msy) const {
   return area.Contains(msx,msy);
}



bool WidgetBase::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
   if (parent && notify_parent) {
      return parent->GiveWidgetFocus(widget);
   }
   return false;
}



bool WidgetBase::SetColorDepth(int color_depth) {
   wc.SetColorDepth(color_depth);
   wcols->SetColorDepth(color_depth);
   SetBgRedrawFlag();
   return true;
}



void WidgetBase::SetMinWidth(int w) {
   const int amw = AbsMinWidth();
   if (w < amw) {w = amw;}
   minw = w;
   if (area.W() < minw) {SetDrawDimensions(minw , area.H());}
}



void WidgetBase::SetMinHeight(int h) {
   const int amh = AbsMinHeight();
   if (h < amh) {h = amh;}
   minh = h;
   if (area.H() < minh) {SetDrawDimensions(area.W() , minh);}
}



void WidgetBase::SetMinDimensions(int w , int h) {
   const int amw = AbsMinWidth();
   const int amh = AbsMinHeight();
   if (w < amw) {w = amw;}
   if (h < amh) {h = amh;}
   minw = w;
   minh = h;
   if ((area.W() < minw) || (area.H() < minh)) {
      SetDrawDimensions((area.W() > minw)?area.W():minw , (area.H() > minh)?area.H():minh);
   }
}



void WidgetBase::SetDisplayPriority(int priority) {
   if (priority < 0) {priority = 0;}
   if (priority > 255) {priority = 255;}
   display_priority = priority;
   SetBgRedrawFlag();
}



WidgetHandler* WidgetBase::NearestParentGui() {
   WidgetBase* p = parent;
   while (p) {
      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(p);
      if (pwh) {return pwh;}
      p = p->parent;
   }
   return 0;
}



WidgetBase* WidgetBase::Root() {
	if (!parent) {return this;}
   WidgetBase* p = parent;
   while (p->parent) {
      p = p->parent;
   }
   return p;
}



WidgetHandler* WidgetBase::RootGui() {
   return dynamic_cast<WidgetHandler*>(Root());
}



const Rectangle WidgetBase::Area() const {return area;}



void WidgetBase::PrintBaseTo(std::ostream& os , Indenter& indent) const {
   using std::endl;
   os << indent << "WidgetBase (" << wname.c_str() << ") (" << this << ") {" << endl;
   ++indent;
   os << indent << "parent widget : " << parent << " , dest bitmap : " << dest << endl;
   os << indent << "&wc = " << &wc << " , wcols = " << wcols << endl;
//   os << indent << "dest = " << dest << endl;
   os << indent << "area = Size( " << area.W() << " X " << area.H() << " ) , At( " << area.X() << " , " << area.Y() << " )" << endl;
   os << indent << "Minimum dimensions = " << minw << " X " << minh << endl;
   os << indent << "Flags {" << endl;
   ++indent;
   os << indent;
   os << "ENABLED (" << ((flags & ENABLED)?("Y"):("N")) << ") , ";
   os << "VISIBLE (" << ((flags & VISIBLE)?("Y"):("N")) << ") , ";
   os << "HOVER (" << ((flags & HOVER)?("Y"):("N")) << ") , ";
   os << "HASFOCUS (" << ((flags & HASFOCUS)?("Y"):("N")) << ") , ";
   os << "MOVEABLE (" << ((flags & MOVEABLE)?("Y"):("N")) << ")" << endl;
   os << indent;
   os << "RESIZEABLE (" << ((flags & RESIZEABLE)?("Y"):("N")) << ") , ";
   os << "NEEDS_REDRAW (" << ((flags & NEEDS_REDRAW)?("Y"):("N")) << ") , ";
   os << "NEEDS_BG_REDRAW (" << ((flags & NEEDS_BG_REDRAW)?("Y"):("N")) << ") , ";
   os << "ALLOW_CLOSE (" << ((flags & ALLOW_CLOSE)?("Y"):("N")) << ") , ";
   os << "ALLOW_OVERLAP (" << ((flags & ALLOW_OVERLAP)?("Y"):("N")) << ")" << endl;
   --indent;
   os << indent << "}" << endl;
   --indent;
   os << indent << "}" << endl;
}



void WidgetBase::PrintTo(std::ostream& os) const {
   Indenter i(0,3);
   PrintTo(os , i);
}



void WidgetBase::PrintTo(std::ostream& os , Indenter& indent) const {
   PrintBaseTo(os , indent);
}



std::ostream& operator<<(std::ostream& os , const WidgetBase& wb) {
   wb.PrintTo(os);
   return os;
}



/// ---------------------    Global functions    -----------------------------



bool DrawPriorityIsLess(WidgetBase* lhs , WidgetBase* rhs) {
   ASSERT(lhs);
   ASSERT(rhs);
   return (lhs->DisplayPriority() < rhs->DisplayPriority());
}







