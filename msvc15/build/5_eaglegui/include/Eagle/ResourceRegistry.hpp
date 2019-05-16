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


#ifndef ResourceRegistry_H
#define ResourceRegistry_H



#include <vector>
#include <list>



/** Purposes :
   1 - Track all system and video bitmaps created so that they can be destroyed before changing a new graphics mode or
         so that when the program is running in fullscreen mode on Windows they can be transferred to memory bitmaps
         and then back to system or video bitmaps. (DX driver on Windows will not remember contents of video memory when
         the program loses the focus).
   2 - Reload images from disk when changing graphics modes?
   3 - Reload datafiles when changing color depths.
*/



class BitmapHandler;
class Resource;

class ResourceRegistry {
   
   typedef std::list<BitmapHandler*>  BH_LIST;
   typedef BH_LIST::iterator          BH_LIT;

   typedef std::vector<BitmapHandler*> BH_REG;

   typedef std::list<Resource*>   RES_LIST;
   typedef RES_LIST::iterator     RES_LIT;
   
protected :
   /// In the Resource and BitmapHandler constructors, each object is registered with these main registries,
   /// as well as being unregistered in their destructors.
   BH_LIST bh_list;
   RES_LIST res_list;

   /// These two lists are for sorting the bitmap handlers into system and video memory types.
   /// That way the system and video bitmaps can be transferred temporarily into main memory when necessary.
   BH_REG sys_bh;
   BH_REG vid_bh;
   BH_REG failed_sys_bh;
   BH_REG failed_vid_bh;


   BH_LIT FindBitmapHandlerIterator(BitmapHandler* bh);
   RES_LIT FindResourceIterator(Resource* r);

   void BuildSystemRegistry();
   void BuildVideoRegistry();

public :
   
   ResourceRegistry();
   ~ResourceRegistry() {}
   
   void RegisterBH(BitmapHandler* bh);
   void RegisterRes(Resource* res);

   void UnRegisterBH(BitmapHandler* bh);
   void UnRegisterRes(Resource* res);
   
   /// If new_color_depth is 0 then the color depth will be preserved
   /// These functions may fail if out of memory but will attempt to finish if called again.
   /// If successful, do not call these functions again or it will destroy the list keeping track
   /// of the system or video bitmaphandlers and calling Restore* will do nothing then.
   bool MoveSysVidBHToMainMemory(int new_color_depth = 0);
   bool MoveSysBHToMainMemory(int new_color_depth = 0);
   bool MoveVidBHToMainMemory(int new_color_depth = 0);
   
   /// The Restore* functions may fail if out of memory but will attempt to finish completion if called again
   /// They should only be called after the appropriate call to Move*BHToMainMemory.
   bool RestoreSysVidBHFromMainMemory();
   bool RestoreSysBHFromMainMemory();
   bool RestoreVidBHFromMainMemory();

   bool ChangeBHColorDepth(int new_color_depth);

   void FreeBitmapHandlers();
   
   bool BitmapHandlersAllocated();
   
   /// Safe to call Realloc and Reload on a mixed source registry (ie. BitmapHandlers w/ and w/o source images)
   /// Call Reload first to prevent extra allocations
   bool ReallocBitmapHandlers();
   bool ReloadBitmapHandlers();

   void FreeResources();
   bool AcquireResources();
   
   bool ResourcesReady();
   
   
   bool Ready();

};

extern ResourceRegistry resource_registry;





#endif // ResourceRegistry_H






