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

#ifndef BitmapHandler_H
#define BitmapHandler_H

#include <allegro.h>

#include <string>

#include "Eagle/Color.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/BitmapUtility.hpp"
#include "Eagle/ResourceRegistry.hpp"
#include "Eagle/Blitting.hpp"
#include "Eagle/Area.hpp"


/** TODO list :
Major :
   Implement virtual widths and heights, so that a bitmap can be resized easily without
   reallocating memory all the time, like a std::vector. The actual bitmap used by the user
   could just be a sub bitmap of the potentially larger actual bitmap. This may only be useful
   for an image resizing tool though, so perhaps implement it separately.

Moderate :

   Add support for sub bitmaps?
   Remove the clear_to_color?
*/

enum ALPHA_BLENDER_TYPE {
   USE_ALPHA_BLENDER          = 0,// default
   USE_ALPHA_LAYERING_BLENDER = 1
};



class BitmapHandler {
private :
   BITMAP* bmp;
   int cd,w,h;
   Color clear_color;
   BMP_MEM_TYPE memtype;
   BMP_DRAW_TYPE drawtype;
   std::string image_path;
   RGB* store_palette;
   ResourceRegistry* registry;
   ALPHA_BLENDER_TYPE blender_type;

   void CarbonCopy(const BitmapHandler& bh);
   
   void SetDefaultClearColor();

public :
   
   /** Global BitmapHandlers need to have Free() called before main exits (and allegro shuts down) 
         Only a few constructors are safe for use as global objects because if you try to allocate a bitmap
         before allegro_init is called it will fail, as well as system and video type bitmaps failing to
         allocate if not in a graphics mode first. */
   
   
   // BitmapHandler constructor that is safe for use with global objects, does not use the resource registry
   BitmapHandler();

   // BitmapHandler constructor that is safe for use with global objects
   BitmapHandler(ResourceRegistry* reg);
   
   /// The BITMAP* constructor takes ownership of the bitmap - if you don't want that, use BitmapHandler() and Clone().
   /// This allows you to initialize it like this - BitmapHandler buffer(create_bitmap_function call, drawing type);
   BitmapHandler(BITMAP* bitmap , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg = &resource_registry);

   /// If you pass false for clone, the BitmapHandler will take ownership of the BITMAP*.
   /// If clone is false , then bmp_mem_type is ignored.
   /// Don't pass create_bitmap calls to this constructor if clone is true, it will leak memory.
   BitmapHandler(BITMAP* bitmap , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , bool clone , ResourceRegistry* reg = &resource_registry);

   /// This BITMAP* constructor clones a specific area of the bitmap
   BitmapHandler(BITMAP* bitmap , int x , int y , int width , int height , int color_depth ,
                  BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg = &resource_registry);

   // Uses current color depth and default clear color based on the drawing type
   BitmapHandler(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg = &resource_registry);

   // Generic constructor that gives you full control over the type of bitmap and clears it
   BitmapHandler(int color_depth , int width , int height ,
                 Color clear_to_color , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg = &resource_registry);

   // Constructor that loads an image from disk. If load_now is false, this constructor is safe for global objects
   BitmapHandler(const char* filepath , RGB* pal , int color_depth ,
                  BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , bool load_now , ResourceRegistry* reg = &resource_registry);

   // Constructor that loads an image from disk using allegro's current color depth
   // If load_now is false, this constructor is safe for global objects
   BitmapHandler(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type ,
                  bool load_now , ResourceRegistry* reg = &resource_registry);
   
   // Constructor that clones another bitmap handler
   BitmapHandler(const BitmapHandler& bh , ResourceRegistry* reg = &resource_registry);

   // Constructor that clones another bitmap handler and allows you to choose the new color depth and memory type
   BitmapHandler(const BitmapHandler& bh , int color_depth , BMP_MEM_TYPE bmp_mem_type , ResourceRegistry* reg = &resource_registry);
   
   ~BitmapHandler();

   void Free();// retains characteristics of bitmap or source image so you can use Realloc() or Reload() after Free().
   void ClearData();// Calls Free() and clears the width and height data. Realloc() will fail after this but Reload() will work normally.
   
   /** Realloc will retain the old bitmap if it fails to reallocate a new one.
       It will also handle freeing the old one when it succeeds.
       This guarantee is extended to all cloning, assignment, and resizing functions.
       When Realloc is successful, the BITMAP is allocated but is not cleared.
       Reallocating to a SYSTEM or VIDEO type bitmap will force the color depth to 
       the color depth of the screen. Make sure the graphics mode is set first.
   */
   bool Realloc(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   bool Realloc();

   /** ###################       Loading routines       ##################
      Loading bitmaps from PNG/JPG, and BMP/LBM/PCX/TGA formats
   */

   /* Generic loading method that allows you to pick the destination color depth */
   bool LoadImage(const char* filepath , RGB* pal , int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   /* Uses the current color depth that allegro is in */
   bool LoadImage(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   /* Loading method that loads an image using the stored file path, palette pointer, color depth, and memory and drawing types */
   bool Reload();

   /** ###################       Saving routines         ##################
      Saving bitmaps to PNG/JPG, and BMP/PCX/TGA formats (not LBM)
      These will overwrite the destination file without warning.
   */
   
   /* Generic saving routine that allows you to pick the color depth to save it as */
   bool SaveImage(const char* filepath , RGB* pal);

   /** ##################        Cloning routines        ##################
      Cloning other BITMAP*'s and BitmapHandlers :
         Cloning will fail when out of memory or the source bitmap is NULL,
         so check the return value.
      Do not pass create_bitmap* function calls as parameters to clone*, it will leak memory.
   */


   /* Generic BITMAP* clone that allows you to change from one type of bitmap memory to another */
   bool Clone(BITMAP* src , Color clear_to_color , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   
   /* More specific clone that uses the default clear to color and allows you to pick the memory type */
   bool Clone(BITMAP* src , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   
   /* More specific clone that uses a default clear to color and uses the same type of memory as src does */
   bool Clone(BITMAP* src , BMP_DRAW_TYPE bmp_draw_type);

   /* Generic BitmapHandler clone that allows you to change memory type and color depth */
   bool Clone(const BitmapHandler& bh , int color_depth , BMP_MEM_TYPE bmp_mem_type);
   
   /* Clone another BitmapHandler and choose the destination memory type */
   bool Clone(const BitmapHandler& bh , BMP_MEM_TYPE bmp_mem_type);

   /* Exact clone of another BitmapHandler */
   bool Clone(const BitmapHandler& bh);


   /** Adoption routine. Allows you to adopt BITMAP*'s from create_bitmap* function calls.*/
   void Adopt(BITMAP* src , BMP_DRAW_TYPE bmp_draw_type);


   /** Resizing functions */
   
   /* PreserveResize stretches the old contents onto the new bitmap. */
   bool PreserveResize(int color_depth , int width , int height ,
                        BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type);

   inline bool PreserveResize(int color_depth , int width , int height , STRETCH_TYPE stretch_type);
   
   inline bool PreserveResize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type);
   inline bool PreserveResize(int width , int height , STRETCH_TYPE stretch_type);


   /* Resize resizes the bitmap, but does not initialize the pixel data */
   bool Resize(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   
   inline bool Resize(int color_depth , int width , int height);
   
   inline bool Resize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   inline bool Resize(int width , int height);

   /* ClearResize clears the newly resized bitmap to clear_color */
   bool ClearResize(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   inline bool ClearResize(int color_depth , int width , int height);

   inline bool ClearResize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   inline bool ClearResize(int width , int height);


   /** Stretching routins. Allows you to easily create a stretched copy of another bitmap.*/
   bool Stretch(BITMAP* src , int color_depth , int width , int height ,
                BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type);
   
   inline bool Stretch(BITMAP* src , int color_depth , int width , int height , STRETCH_TYPE stretch_type);
   
   inline bool Stretch(BITMAP* src , int width , int height ,
                       BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type);
   inline bool Stretch(BITMAP* src , int width , int height , STRETCH_TYPE stretch_type);


   
   /** Drawing functions */
   
   inline void Clear();
   inline void ClearTo(int color);
   inline void DrawTo(BITMAP* dest , int x , int y) const ;
   inline void CenteredDrawTo(BITMAP* dest , const Rectangle& r) const ;
   void        TypedDrawTo(BITMAP* dest , int x , int y , BMP_DRAW_TYPE bmp_draw_type) const ;
   

   /** ##################        Operators        ################## */

   /** Assignment from other BitmapHandlers, creates clones */
   /* Cloning will fail when out of memory, so check integrity with an (implicit) (bool) cast */
   inline BitmapHandler& operator=(const BitmapHandler& bh);
   
   
   /* Quick integrity checks, gives BITMAP* behavior */
   inline operator bool() const {return (bool)bmp;}
   inline bool operator!() const {return !bmp;}
   
   /* Cast to BITMAP* for interchangeable use as a BITMAP*   */
   inline operator BITMAP* () {return bmp;}
   

   
   /** Data member, image, and memory type manipulation */

   bool ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   inline bool ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type);

   inline bool ChangeColorDepth(int color_depth);
   inline bool ChangeToMemoryType(BMP_MEM_TYPE bmp_mem_type);
   inline void SetDrawType(BMP_DRAW_TYPE bmp_draw_type);
   


   /** Setters */
   void SetClearColor(Color c) {clear_color = c;}
   void SetClearColor(int col) {clear_color.SetColor(col);}
   void SetResourceRegistry(ResourceRegistry* res_reg);
   void SetBlenderType(ALPHA_BLENDER_TYPE type);
   
   /** Getters */
   const int   CD()                const {return cd;}
   const int   W()                 const {return w;}
   const int   H()                 const {return h;}
   const Color ClearColor()        const {return clear_color;}
   const int   AllegroClearColor() const {return (int)clear_color;}
   const char* ImagePath()         const {return image_path.c_str();}
   std::string ImagePathString()   const {return image_path;}
   RGB* StorePalette()      const {return store_palette;}
   ResourceRegistry* Registry() const {return registry;}

   const BMP_MEM_TYPE  MemType()      const {return memtype;}
   const BMP_DRAW_TYPE DrawType()     const {return drawtype;}
   
   void PrintTo(std::ostream& os) const ;
   void PrintTo(std::ostream& os , Indenter& indent) const ;
   friend std::ostream& operator<<(std::ostream& os , const BitmapHandler& bh);

   
};



/** #################################################################
         Inline BitmapHandler member function definitions
*/



inline bool BitmapHandler::PreserveResize(int color_depth , int width , int height , STRETCH_TYPE stretch_type) {
   return PreserveResize(color_depth , width , height , memtype , drawtype , stretch_type);
}



inline bool BitmapHandler::PreserveResize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type) {
   return PreserveResize(cd , width , height , bmp_mem_type , bmp_draw_type , stretch_type);
}



inline bool BitmapHandler::PreserveResize(int width , int height , STRETCH_TYPE stretch_type) {
   return PreserveResize(cd , width , height , memtype , drawtype , stretch_type);
}



inline bool BitmapHandler::Resize(int color_depth , int width , int height) {
   return Resize(color_depth , width , height , memtype , drawtype);
}



inline bool BitmapHandler::Resize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   return Resize(cd , width , height , bmp_mem_type , bmp_draw_type);
}



inline bool BitmapHandler::Resize(int width , int height) {
   return Resize(cd , width , height , memtype , drawtype);
}



inline bool BitmapHandler::ClearResize(int color_depth , int width , int height) {
   return ClearResize(color_depth , width , height , memtype , drawtype);
}



inline bool BitmapHandler::ClearResize(int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   return ClearResize(cd , width , height , bmp_mem_type , bmp_draw_type);
}



inline bool BitmapHandler::ClearResize(int width , int height) {
   return ClearResize(cd , width , height , memtype , drawtype);
}



inline bool BitmapHandler::Stretch(BITMAP* src , int color_depth , int width , int height , STRETCH_TYPE stretch_type) {
   return Stretch(src , color_depth , width , height , memtype , drawtype , stretch_type);
}



inline bool BitmapHandler::Stretch(BITMAP* src , int width , int height ,
                                   BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , STRETCH_TYPE stretch_type) {
   return Stretch(src , cd , width , height , bmp_mem_type , bmp_draw_type , stretch_type);
}



inline bool BitmapHandler::Stretch(BITMAP* src , int width , int height , STRETCH_TYPE stretch_type) {
   return Stretch(src , cd , width , height , memtype , drawtype , stretch_type);
}



inline void BitmapHandler::Clear() {
   clear_to_color(bmp , clear_color);
}



inline void BitmapHandler::ClearTo(int color) {
   clear_to_color(bmp , color);
}



inline void BitmapHandler::DrawTo(BITMAP* dest , int x , int y) const {
   TypedDrawTo(dest , x , y , drawtype);
}



inline void BitmapHandler::CenteredDrawTo(BITMAP* dest , const Rectangle& r) const {
   DrawTo(dest , r.X() + (r.W() - w)/2 , r.Y() + (r.H() - h)/2);
}



inline BitmapHandler& BitmapHandler::operator=(const BitmapHandler& bh) {
   Clone(bh);
   return (*this);
}



inline bool BitmapHandler::ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type) {
   return ChangeAttributes(color_depth , bmp_mem_type , drawtype);
}



inline bool BitmapHandler::ChangeColorDepth(int color_depth) {
   return ChangeAttributes(color_depth , memtype , drawtype);
}



inline bool BitmapHandler::ChangeToMemoryType(BMP_MEM_TYPE bmp_mem_type) {
   return ChangeAttributes(cd , bmp_mem_type , drawtype);
}



inline void BitmapHandler::SetDrawType(BMP_DRAW_TYPE bmp_draw_type) {
   drawtype = bmp_draw_type;
}










#endif // BitmapHandler_H

