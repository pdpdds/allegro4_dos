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


#include "Eagle/FileWork.hpp"
#include "Eagle/MouseHandler.hpp"


using std::string;

/* To map the public mousecodes directly to the flags in mbtns_pressed, etc... */
const int mbtn_flags[NUM_MBTNS] = {
   1,  // LMB
   2,  // RMB
   4,  // MMB
   8,  // EMB1
   16, // EMB2
   32, // EMB3
   64, // EMB4
   128 // EMB5
};

/* Track mouse position changes */
volatile int mpos     = 0;
volatile int mx      = 0;
volatile int my      = 0;
volatile int prev_mpos = 0;
volatile int prev_mx = 0;
volatile int prev_my = 0;

/* Track scroll wheel changes */
volatile int mz = 0;
volatile int prev_mz = 0;
volatile int mw = 0;
volatile int prev_mw = 0;

/* Track positional differences since last update for x,y location and z wheel location */
volatile int ms_xdiff = 0;
volatile int ms_ydiff = 0;
volatile int ms_zdiff = 0;
volatile int ms_wdiff = 0;

/* Track mouse button states */
volatile int mb         = 0;
volatile int prev_mb    = 0;
volatile int mb_up      = 0;
volatile int prev_mb_up = 0;

/* Track mouse button state changes */
volatile int held_mbtns      = 0;
volatile int pressed_mbtns   = 0;
volatile int released_mbtns  = 0;
volatile int open_mbtns      = 0;
volatile int dbl_click_mbtns = 0;

/* Track duration that each mouse button is held down for */
volatile double held_mbtns_duration[NUM_MBTNS];

/* Track how long since the last click */
volatile double since_last_click_mbtns[NUM_MBTNS];
double double_click_duration_mbtn = 0.5;


/* Maps the flag values to c strings */
const char* mousecode_to_name(int mbtn) {
   static const char* mbtn_name[NUM_MBTNS] = {
      "Left Mouse Button",
      "Right Mouse Button",
      "Middle Mouse Button",
      "Extra Mouse Button 1",
      "Extra Mouse Button 2",
      "Extra Mouse Button 3",
      "Extra Mouse Button 4",
      "Extra Mouse Button 5"
   };
   return mbtn_name[mbtn];
}




/// Pointer code

int mouse_owner = 0;

BITMAP* mouse_pointer = 0;
int mouse_offset_x = 0;
int mouse_offset_y = 0;
MOUSE_POINTER current_pointer_type = POINTER_NORMAL;

bool using_allegro_pointer = true;

POINTER_INFO pointer_info[NUM_POINTERS];



/* Just initializes the held_mbtns_duration array to 0.0 and the time since last click to 
   twice the double click duration to prevent false positives. */
void SetupMouseHandler() {
   int i = 0;
   POINTER_INFO* pi = 0;
   
   for (i = 0 ; i < NUM_MBTNS ; ++i) {
      held_mbtns_duration[i] = 0.0;
      since_last_click_mbtns[i] = 2*double_click_duration_mbtn;
   }
   
   for (i = 0 ; i < NUM_POINTERS ; ++i) {
      pi = &pointer_info[i];
      pi->bmp = 0;
      pi->x = 0;
      pi->y = 0;
   }
}



/* Call once for each logic update and pass the amount
   of time passed since the last update. Pass 0.0 if
   you don't care about the durations that the mouse
   buttons were held. */
void UpdateMouseHandler(double dt_sec) {
   int i = 0, flag = 0;
   
   /// Update old states
   prev_mpos = mpos;
   prev_mx = mx;
   prev_my = my;
   prev_mz = mz;
   prev_mw = mw;
   
   prev_mb = mb;
   prev_mb_up = mb_up;

   /// Update current states
   mpos = mouse_pos;
   mx = mpos >> 16;
   my = mpos & 0x0000ffff;
   mz = mouse_z;
   mw = mouse_w;
   mb = mouse_b;
   mb_up = ~mb;

   /// Update state differences
   ms_xdiff = mx - prev_mx;
   ms_ydiff = my - prev_my;
   ms_zdiff = mz - prev_mz;
   ms_wdiff = mw - prev_mw;

   /// Update button state changes
   held_mbtns = prev_mb & mb;
   pressed_mbtns  = prev_mb_up & mb;
   released_mbtns = prev_mb & mb_up;
   open_mbtns = prev_mb_up & mb_up;
   dbl_click_mbtns = 0;
   
   /// Update button held durations and time since last click
   for (i = 0 ; i < NUM_MBTNS ; ++i) {
      since_last_click_mbtns[i] += dt_sec;
      flag = mbtn_flags[i];
      if (held_mbtns & flag) {
         held_mbtns_duration[i] += dt_sec;
      }
      if (pressed_mbtns & flag) {
         if (since_last_click_mbtns[i] <= double_click_duration_mbtn) {
            dbl_click_mbtns |= flag;
         }
         since_last_click_mbtns[i] = 0.0;
      }
      if (released_mbtns & flag) {
         held_mbtns_duration[i] = 0.0;
      }
   }
   return;
}


/// Pointer code


const char* pointer_names[NUM_POINTERS] = {
   "NORMAL",
   "BUSY",
   "QUESTION",
   "EDIT",
   "GRAB",
   "GRABBED",
   "NS",
   "NESW",
   "EW",
   "SENW",
   "CUSTOM"
};


void PrivateSetMousePointer(BITMAP* bmp , int mouse_x_offset , int mouse_y_offset) {
   if ((mouse_pointer != bmp) ||
       (mouse_offset_x != mouse_x_offset) ||
       (mouse_offset_y != mouse_y_offset)) {
      mouse_pointer = bmp;
      mouse_offset_x = mouse_x_offset;
      mouse_offset_y = mouse_y_offset;
      if (using_allegro_pointer) {
         set_mouse_sprite(bmp);
         set_mouse_sprite_focus(mouse_x_offset , mouse_y_offset);
         /// Workaround - need to call show_mouse to update the focus hotspot on the mouse
         /// when using hardware cursors
         show_mouse(screen);
      }
   }
}



int AcquireMouseOwnerId() {
   static int i = 1;
   return i++;
}



int GrabMousePointer(int owner_id) {
   if (owner_id < 1) {return 0;}
   if (mouse_owner) {
      if (mouse_owner != owner_id) {
         return 0;
      }
      return 1;
   }
   mouse_owner = owner_id;
   return 1;
}



int UseMousePointer(MOUSE_POINTER pointer_type , int owner_id) {
   POINTER_INFO* pi = 0;
   
   ASSERT(pointer_type != NUM_POINTERS);
   
   if ((mouse_owner < 1) || (owner_id < 1) || (mouse_owner != owner_id)) {return 0;}
   pi = &pointer_info[pointer_type];
   ASSERT(pi->bmp);
   PrivateSetMousePointer(pi->bmp , pi->x , pi->y);
   current_pointer_type = pointer_type;
   return 1;
}



int ReleaseMousePointer(int owner_id) {
   POINTER_INFO* pi = 0;
   
   if (owner_id < 1) {return 0;}
   if (mouse_owner == 0) {return 1;}
   if (mouse_owner == owner_id) {
      mouse_owner = 0;
      /// Revert to normal mouse pointer here...
      if (using_allegro_pointer) {
         PrivateSetMousePointer(0 , 0 , 0);
      } else {
         pi = &pointer_info[POINTER_NORMAL];
         PrivateSetMousePointer(pi->bmp , pi->x , pi->y);
         current_pointer_type = POINTER_NORMAL;
      }
      return 1;
   }
   return 0;
}



void SetPointerAndPosition(MOUSE_POINTER pointer_type , BITMAP* pointer_sprite , int pointer_x , int pointer_y) {
   POINTER_INFO* pi = 0;
   
   ASSERT(pointer_sprite);
   ASSERT(pointer_type != NUM_POINTERS);
   
   pi = &pointer_info[pointer_type];
   pi->bmp = pointer_sprite;
   pi->x = pointer_x;
   pi->y = pointer_y;
   
   if (current_pointer_type == pointer_type) {
      if (using_allegro_pointer) {
         if (current_pointer_type != POINTER_NORMAL) {
            PrivateSetMousePointer(pointer_sprite , pointer_x , pointer_y);
         }
      } else {
         PrivateSetMousePointer(pointer_sprite , pointer_x , pointer_y);
      }
   }
}



void SetupPointers(POINTER_INFO* info_array) {
   POINTER_INFO* pi = &pointer_info[current_pointer_type];
   
   ASSERT(info_array);
   
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      pointer_info[i] = info_array[i];
   }
   
   if (using_allegro_pointer) {
      if (current_pointer_type != POINTER_NORMAL) {
         PrivateSetMousePointer(pi->bmp , pi->x , pi->y);
      }
   } else {
      PrivateSetMousePointer(pi->bmp , pi->x , pi->y);
   }
}



void SetAllegroPointerUse(bool use_allegro_pointer) {
   using_allegro_pointer = use_allegro_pointer;
   PrivateSetMousePointer(mouse_pointer , mouse_offset_x , mouse_offset_y);
}



void DrawPointer(BITMAP* bmp , int x , int y) {
   x -= mouse_offset_x;
   y -= mouse_offset_y;
   if (!using_allegro_pointer) {
      draw_sprite(bmp , mouse_pointer , x , y);
   }
}



/// -----------------------------------     MousePointerSetup class    --------------------------------------------




MousePointerSetup::MousePointerSetup() : info() {
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      info[i].bmp = 0;
      info[i].x = 0;
      info[i].y = 0;
   }
}
MousePointerSetup::~MousePointerSetup() {
   Free();
}
void MousePointerSetup::Free() {
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      BITMAP* bmp = info[i].bmp;
      if (bmp) {destroy_bitmap(bmp);}
      info[i].bmp = 0;
   }
}

bool MousePointerSetup::LoadPointersFromScript(const char* script_file) {
   /// Sample script
   // TODO : UPDATE TO USE AN ALLEGRO CONFIG FILE INSTEAD OF THIS CUSTOM FORMAT SHITE
   
   /// SPRITESHEET
   /// data/images/MousePointers.bmp
   /// SIZE
   /// 20 20
   /// NUMBER
   /// 10
   /// OFFSETS
   /// 0 0
   /// 10 10
   /// ...
   FILE* f = fopen(script_file , "r");
   if (!f) {return false;}
   
   string s;
   BITMAP* bmp = 0;
   int w = 0;
   int h = 0;
   int n = 0;
   int i = 0;
   
   GetLinePN(f , s);
   if (strcmp(s.c_str() , "SPRITESHEET") != 0) {goto fail;}
   
   GetLinePN(f , s);
   bmp = load_bitmap(s.c_str() , NULL);
   if (!bmp) {goto fail;}
   
   GetLinePN(f , s);
   if (strcmp(s.c_str() , "SIZE") != 0) {goto fail;}
   
   if (!ReadTextInteger(f , &w)) {goto fail;}
   SkipWhiteSpace(f);
   if (!ReadTextInteger(f , &h)) {goto fail;}
   SkipWhiteSpace(f);
   
   GetLinePN(f , s);
   if (strcmp(s.c_str() , "NUMBER") != 0) {goto fail;}
   
   if (!ReadTextInteger(f , &n)) {goto fail;}
   SkipWhiteSpace(f);
   
   if (n != NUM_POINTERS) {goto fail;}
   
   GetLinePN(f , s);
   if (strcmp(s.c_str() , "OFFSETS") != 0) {goto fail;}
   
   for (i = 0 ; i < NUM_POINTERS ; ++i) {
      if (!ReadTextInteger(f , &(info[i].x))) {goto fail;}
      SkipWhiteSpace(f);
      if (!ReadTextInteger(f , &(info[i].y))) {goto fail;}
      SkipWhiteSpace(f);
   }
   
   if (bmp->w < w*n) {goto fail;}
   
   Free();
   
   for (i = 0 ; i < NUM_POINTERS ; ++i) {
      info[i].bmp = create_bitmap(w,h);
      if (!info[i].bmp) {goto fail;}
      int x = i*w;
      blit(bmp , info[i].bmp , x , 0 , 0 , 0 , w , h);
   }
   
   SetupPointers(info);
   destroy_bitmap(bmp);
   fclose(f);
   return true;
   
   fail:
   if (bmp) {destroy_bitmap(bmp);}
   fclose(f);
   return false;
}





