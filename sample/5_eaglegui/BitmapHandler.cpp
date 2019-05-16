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

#include "Eagle/Blending.hpp"
#include "Eagle/BitmapHandler.hpp"
#include "Eagle/AllegroUtilities.hpp"

#include <cstring>

using std::string;

void BitmapHandler::CarbonCopy(const BitmapHandler& bh) {
   bmp = bh.bmp;
   cd = bh.cd;
   w = bh.w;
   h = bh.h;
   clear_color = bh.clear_color;
   memtype = bh.memtype;
   drawtype = bh.drawtype;
   image_path = bh.image_path;
   store_palette = bh.store_palette;
   // registry = bh.registry // Don't copy this
}



/**
    Sets a default clear_color based on drawtype :
    (SOLID = black , TRANSPARENT = magic pink, and TRANSLUCENT = clear black)
*/
void BitmapHandler::SetDefaultClearColor() {
   switch (drawtype) {
      case SOLID :
         clear_color.SetRGBAex(0,0,0,255,cd);
         break;
      case TRANSPARENT :
         if (bmp) {
            clear_color.SetColorDepth(cd);
            clear_color.SetColor(bitmap_mask_color(bmp));
         } else {
            if (cd == 8) {
               clear_color.SetColorDepth(8);
               clear_color.SetColor(0);
            } else {
               clear_color.SetRGBex(255,0,255,cd);
            }
         }
         break;
      case TRANSLUCENT :
         clear_color.SetRGBAex(0,0,0,0,cd);
         break;
   }
}



/** Empty bitmap handler */
BitmapHandler::BitmapHandler() : 
         bmp(0) , 
         cd(32) , 
         w(0) , 
         h(0) , 
         clear_color(0,0,0,255,32) , 
         memtype(MEMORY) , 
         drawtype(SOLID),
         image_path(),
         store_palette(0),
         registry(0),
         blender_type(USE_ALPHA_BLENDER)
{}



/** Empty bitmap handler */
BitmapHandler::BitmapHandler(ResourceRegistry* reg) : 
         bmp(0) , 
         cd(32) , 
         w(0) , 
         h(0) , 
         clear_color(0,0,0,255,32) , 
         memtype(MEMORY) , 
         drawtype(SOLID),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{}




/**
   Sets a default clear_color based on drawtype.
*/
BitmapHandler::BitmapHandler(BITMAP* bitmap , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg) :
         bmp(0) , 
         cd(32) , 
         w(0) , 
         h(0) , 
         clear_color(0,0,0,255,32) , 
         memtype(MEMORY) , 
         drawtype(SOLID),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   if (registry) {
      registry->RegisterBH(this);
   }
   if (bitmap) {
      bmp = bitmap;
      cd = bitmap_color_depth(bmp);
      w = bitmap->w;
      h = bitmap->h;
      /** 
          Should the TRANSLUCENT setting be retained even though it wouldn't work in non 32 bit color depths?
          Probably not, as it will try to use draw_trans_sprite on a non-32 bit bitmap.
      */
      if ((bmp_draw_type == TRANSLUCENT) && (cd != 32)) {bmp_draw_type = TRANSPARENT;}
      memtype = GetMemType(bmp);
      drawtype = bmp_draw_type;
      SetDefaultClearColor();
   }
}

BitmapHandler::BitmapHandler(BITMAP* bitmap , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , bool clone , ResourceRegistry* reg) :
         bmp(0),
         cd(32),
         w(0),
         h(0),
         clear_color(0,0,0,255,32),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   if (registry) {
      registry->RegisterBH(this);
   }
   if (bitmap) {
      if (clone) {
         Clone(bitmap , clear_color , bmp_mem_type , bmp_draw_type);
      } else {
         bmp = bitmap;
         cd = bitmap_color_depth(bmp);
         w = bitmap->w;
         h = bitmap->h;
         if ((bmp_draw_type == TRANSLUCENT) && (cd != 32)) {bmp_draw_type = TRANSPARENT;}
         memtype = GetMemType(bmp);
         drawtype = bmp_draw_type;
         SetDefaultClearColor();
      }
   }
}



BitmapHandler::BitmapHandler(BITMAP* bitmap , int x , int y , int width , int height , int color_depth ,
               BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg) :
         bmp(0),
         cd(AllegroColorDepth()),
         w(width),
         h(height),
         clear_color(0,0,0,255,AllegroColorDepth()),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   Realloc(color_depth , width , height , bmp_mem_type , bmp_draw_type);
   SetDefaultClearColor();
   if (bmp) {
      blit(bitmap , bmp , x , y , 0 , 0 , width , height);
   }
}


BitmapHandler::BitmapHandler(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg) :
         bmp(0),
         cd(AllegroColorDepth()),
         w(width),
         h(height),
         clear_color(0,0,0,255,AllegroColorDepth()),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   int color_depth = AllegroColorDepth();
   Realloc(color_depth , width , height , bmp_mem_type , bmp_draw_type);
   SetDefaultClearColor();
}



BitmapHandler::BitmapHandler(int color_depth,
                             int width,
                             int height,
                             Color clear_to_color,
                             BMP_MEM_TYPE bmp_mem_type,
                             BMP_DRAW_TYPE bmp_draw_type,
                             ResourceRegistry* reg) :
         bmp(0),
         cd(color_depth),
         w(width),
         h(height),
         clear_color(clear_to_color),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(),
         store_palette(0),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   ASSERT(ColorDepthIndex(color_depth) != NUMCOLORDEPTHS);
   clear_to_color.SetColorDepth(color_depth);
   clear_color = clear_to_color;
   Realloc(color_depth , width , height , bmp_mem_type , bmp_draw_type);
   Clear();
}



BitmapHandler::BitmapHandler(const char* filepath , RGB* pal , int color_depth ,
                              BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , bool load_now , ResourceRegistry* reg) :
         bmp(0),
         cd(32),
         w(0),
         h(0),
         clear_color(0,0,0,255,32),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(filepath),
         store_palette(pal),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   if (load_now) {
      LoadImage(filepath , pal , color_depth , bmp_mem_type , bmp_draw_type);
   }
}


   
BitmapHandler::BitmapHandler(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type ,
                              bool load_now , ResourceRegistry* reg) :
         bmp(0),
         cd(32),
         w(0),
         h(0),
         clear_color(0,0,0,255,32),
         memtype(bmp_mem_type),
         drawtype(bmp_draw_type),
         image_path(filepath),
         store_palette(pal),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   if (load_now) {
      LoadImage(filepath , pal , bmp_mem_type , bmp_draw_type);
   }
}



BitmapHandler::BitmapHandler(const BitmapHandler& bh , ResourceRegistry* reg) :
         bmp(0),
         cd(bh.cd),
         w(bh.w),
         h(bh.h),
         clear_color(bh.clear_color),
         memtype(bh.memtype),
         drawtype(bh.drawtype),
         image_path(bh.image_path),
         store_palette(bh.store_palette),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   Clone(bh);
}



BitmapHandler::BitmapHandler(const BitmapHandler& bh , int color_depth , BMP_MEM_TYPE bmp_mem_type , ResourceRegistry* reg) :
         bmp(0) , 
         cd(bh.cd),
         w(bh.w),
         h(bh.h),
         clear_color(bh.clear_color),
         memtype(bmp_mem_type),
         drawtype(bh.drawtype),
         image_path(bh.image_path),
         store_palette(bh.store_palette),
         registry(reg),
         blender_type(USE_ALPHA_BLENDER)
{
   Clone(bh , color_depth , bmp_mem_type);
}



BitmapHandler::~BitmapHandler() {
   if (registry) {
      registry->UnRegisterBH(this);
   }
   Free();
}


void BitmapHandler::Free() {
   if (bmp) {
      destroy_bitmap(bmp);
   }
   bmp = (BITMAP*)0;
}



void BitmapHandler::ClearData() {
   Free();
   w = 0;
   h = 0;
}



/** Realloc will retain the old bitmap if it fails to reallocate a new one.
    It will also handle freeing the old one when it succeeds.
*/
bool BitmapHandler::Realloc(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {

   if (registry) {
      registry->RegisterBH(this);
   }

   ASSERT((width > 0) && (height > 0));
   ASSERT(ColorDepthIndex(color_depth) != NUMCOLORDEPTHS);

   if (bmp_mem_type != MEMORY) {
      ASSERT(screen);
      color_depth = bitmap_color_depth(screen);
   }

   // So we don't reallocate if we don't need to
   if (bmp) {
      if ((cd == color_depth) &&
          (w == width) &&
          (h == height) &&
          (memtype == bmp_mem_type)) {
         return true;
      }
   }

   BITMAP* bitmap = CreateBitmap(color_depth , width , height , bmp_mem_type);
   if (bitmap) {
      Free();
      bmp = bitmap;
      cd = color_depth;
      w = width;
      h = height;
      clear_color.SetColorDepth(cd);
      memtype = bmp_mem_type;
      drawtype = bmp_draw_type;
      
      return true;
   }
   return false;
}



bool BitmapHandler::Realloc() {
   if (!bmp) {
      return Realloc(cd,w,h,memtype,drawtype);
   }
   return true;
}



bool BitmapHandler::LoadImage(const char* filepath , RGB* pal , int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(filepath);
   ASSERT(strlen(filepath));
   ASSERT(ColorDepthIndex(color_depth) != NUMCOLORDEPTHS);

   if (registry) {
      registry->RegisterBH(this);
   }

   if ((filepath) && (ColorDepthIndex(color_depth) != NUMCOLORDEPTHS)) {
      const int oldcc = get_color_conversion();
      set_color_conversion(COLORCONV_NONE);
      
      BITMAP* img = load_bitmap(filepath , pal);
      set_color_conversion(oldcc);
      
      BITMAP* src = NULL;
      if (img) {
         src = CreateBitmap(color_depth , img->w , img->h , bmp_mem_type);
         if (src) {
            blit(img , src , 0 , 0 , 0 , 0 , img->w , img->h);
         }
         destroy_bitmap(img);
      }
      if (src) {
         Free();
         bmp = src;
         cd = bitmap_color_depth(src);
         w = src->w;
         h = src->h;
         memtype = bmp_mem_type;
         drawtype = bmp_draw_type;
         SetDefaultClearColor();
         return true;
      }
   }
   return false;
}



/* Uses the current color depth that allegro is in */
bool BitmapHandler::LoadImage(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   return LoadImage(filepath , pal , AllegroColorDepth() , bmp_mem_type , bmp_draw_type);
}



bool BitmapHandler::Reload() {
   return LoadImage(image_path.c_str() , store_palette , cd , memtype , drawtype);
}



/* Generic saving routine */
bool BitmapHandler::SaveImage(const char* filepath , RGB* pal) {
   ASSERT(filepath);
   ASSERT(bmp);

   return (save_bitmap(filepath , bmp , pal) == 0);
}



bool BitmapHandler::Clone(BITMAP* src , Color clear_to_color , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(src);
   if (Realloc(bitmap_color_depth(src) , src->w , src->h , bmp_mem_type , bmp_draw_type)) {
      blit(src , bmp , 0 , 0 , 0 , 0 , src->w , src->h);
      clear_color = clear_to_color;
      clear_color.SetColorDepth(cd);
      return true;
   }
   return false;
}



bool BitmapHandler::Clone(BITMAP* src , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(src);
   
   if (Realloc(bitmap_color_depth(src) , src->w , src->h , bmp_mem_type , bmp_draw_type)) {
      blit(src , bmp , 0 , 0 , 0 , 0 , src->w , src->h);
      SetDefaultClearColor();
      return true;
   }
   return false;
}



bool BitmapHandler::Clone(BITMAP* src , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(src);

   if (Realloc(bitmap_color_depth(src) , src->w , src->h , GetMemType(src) , bmp_draw_type)) {
      blit(src , bmp , 0 , 0 , 0 , 0 , src->w , src->h);
      SetDefaultClearColor();
      return true;
   }
   return false;
}



bool BitmapHandler::Clone(const BitmapHandler& bh , int color_depth , BMP_MEM_TYPE bmp_mem_type) {
   BITMAP* src = bh.bmp;
   ASSERT(src);
   if (Realloc(color_depth , src->w , src->h , bmp_mem_type , bh.drawtype)) {
      clear_color = bh.clear_color;
      clear_color.SetColorDepth(color_depth);
      blit(src , bmp , 0 , 0 , 0 , 0 , src->w , src->h);
      return true;
   }
   return false;
}



bool BitmapHandler::Clone(const BitmapHandler& bh , BMP_MEM_TYPE bmp_mem_type) {
   return Clone(bh , bh.cd , bmp_mem_type);
}



bool BitmapHandler::Clone(const BitmapHandler& bh) {
   return Clone(bh , bh.cd , bh.memtype);
}



void BitmapHandler::Adopt(BITMAP* src , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(src);
   
   if (registry) {
      registry->RegisterBH(this);
   }

   if (src) {
      Free();
      bmp = src;
      cd = bitmap_color_depth(src);
      w = src->w;
      h = src->h;
      memtype = GetMemType(src);
      drawtype = bmp_draw_type;
   }
}



bool BitmapHandler::PreserveResize(int color_depth , int width , int height ,
                                    BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type) {
   BitmapHandler temp(color_depth , width , height , clear_color , bmp_mem_type , bmp_draw_type);
   if (temp.bmp) {
      switch(stretch_type) {
         case USE_ALLEGRO_STRETCH :
            stretch_blit(bmp , temp.bmp , 0 , 0 , w , h , 0 , 0 , width , height);
            break;
         case USE_SMOOTH_STRETCH :
            stretch_blit_smooth(bmp , temp.bmp , 0 , 0 , w , h , 0 , 0 , width , height);
            break;
      }
      BITMAP* old = bmp;
      CarbonCopy(temp);
      temp.bmp = old;
      return true;
   }
   return false;
}




bool BitmapHandler::Resize(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   return Realloc(color_depth , width , height , bmp_mem_type , bmp_draw_type);
}



bool BitmapHandler::ClearResize(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   if (Realloc(color_depth , width , height , bmp_mem_type , bmp_draw_type)) {
      Clear();
      return true;
   }
   return false;
}



bool BitmapHandler::Stretch(BITMAP* src , int color_depth , int width , int height ,
                            BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type) {
   ASSERT(src);
   bool success = Resize(color_depth , width , height , bmp_mem_type , bmp_draw_type);
   if (src && success) {
      switch (stretch_type) {
         case USE_ALLEGRO_STRETCH :
            stretch_blit(src , bmp , 0 , 0 , src->w , src->h , 0 , 0 , bmp->w , bmp->h);
            break;
         case USE_SMOOTH_STRETCH :
            stretch_blit_smooth(src , bmp , 0 , 0 , src->w , src->h , 0 , 0 , bmp->w , bmp->h);
            break;
      }
   }
   return success;
}



void BitmapHandler::TypedDrawTo(BITMAP* dest , int x , int y , BMP_DRAW_TYPE bmp_draw_type) const {
   ASSERT(dest);
   if (bmp) {
      switch (bmp_draw_type) {
         case SOLID :
            blit(bmp , dest , 0 , 0 , x , y , w , h);
            break;
         case TRANSPARENT :
            masked_blit(bmp , dest , 0 , 0 , x , y , w , h);
            break;
         case TRANSLUCENT :
            ASSERT(cd == 32);
            switch (blender_type) {
               case USE_ALPHA_BLENDER :
                  set_alpha_blender();
                  break;
               case USE_ALPHA_LAYERING_BLENDER :
                  ASSERT(bitmap_color_depth(dest) == 32);
                  set_alpha_layering_blender();
                  break;
            }
            draw_trans_sprite(dest , bmp , x , y);
            break;
      }
   }
}



bool BitmapHandler::ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   /// color_depth will be ignored for SYSTEM and VIDEO bitmap memory types, and will be set to 
   /// the color depth of the current graphics mode. A graphics mode must be set before attempting to change
   /// to SYSTEM or VIDEO type bitmaps if a bitmap is allocated. Using an invalid color depth will also fail.

   ASSERT(bmp);
   COLORDEPTH cdi = ColorDepthIndex(color_depth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   if (cdi == NUMCOLORDEPTHS) {return false;}
   
   if (registry) {
      registry->RegisterBH(this);
   }
   
   if (!bmp) {
      cd = color_depth;
      memtype = bmp_mem_type;
      drawtype = bmp_draw_type;
      return true;
   }

   if ((cd == color_depth) && (memtype == bmp_mem_type)) {// No major changes necessary
      SetDrawType(bmp_draw_type);
      return true;
   }

   if (bmp_mem_type != MEMORY) {
      // SYSTEM and VIDEO memory will always be the same color depth as the screen, and so require
      // the graphics mode to be successfully set first.
      ASSERT(screen);
      if (!screen) {return false;}
      color_depth = bitmap_color_depth(screen);
   }

   
   BitmapHandler temp(color_depth , w , h , clear_color , bmp_mem_type , bmp_draw_type);
   if (temp) {
      blit(bmp , temp.bmp , 0 , 0 , 0 , 0 , bmp->w , bmp->h);
      BITMAP* old = bmp;
      CarbonCopy(temp);
      temp.bmp = old;
      return true;
   }
   return false;
}



void BitmapHandler::SetResourceRegistry(ResourceRegistry* res_reg) {
   if (registry) {registry->UnRegisterBH(this);}
   registry = res_reg;
   if (registry) {registry->RegisterBH(this);}
}



void BitmapHandler::SetBlenderType(ALPHA_BLENDER_TYPE type) {
   blender_type = type;
}



void BitmapHandler::PrintTo(std::ostream& os) const {
   Indenter indent(0,3);
   PrintTo(os,indent);
}


void BitmapHandler::PrintTo(std::ostream& os , Indenter& indent) const {
   using std::endl;
   os << indent << "BitmapHandler (" << this << ") : {" << endl;
   ++indent;
   os << indent << "bmp = " << bmp << " , (cd,w,h = " << cd << "," << w << "," << h << ")" << endl;
   os << indent << "Clear color :" << endl;
   clear_color.PrintTo(os,indent);
   os << indent << "memtype = ";
   switch (memtype) {
      case MEMORY : os << "MEMORY";break;
      case SYSTEM : os << "SYSTEM";break;
      case VIDEO  : os << "VIDEO";break;
   }
   os << " , drawtype = ";
   switch (drawtype) {
      case SOLID       : os << "SOLID";break;
      case TRANSPARENT : os << "TRANSPARENT";break;
      case TRANSLUCENT : os << "TRANSLUCENT";break;
   }
   os << endl;
   os << indent << "Image path = '" << image_path.c_str() << "' , Store palette = " << store_palette << endl;
   os << indent << "Registry = " << registry << " , blender type = ";
   switch (blender_type) {
      case USE_ALPHA_BLENDER : os << "USE_ALPHA_BLENDER";break;
      case USE_ALPHA_LAYERING_BLENDER : os << "USE_ALPHA_LAYERING_BLENDER";break;
   }
   os << endl;
   --indent;
   os << indent << "}" << endl;
}



std::ostream& operator<<(std::ostream& os , const BitmapHandler& bh) {
   bh.PrintTo(os);
   return os;
}





