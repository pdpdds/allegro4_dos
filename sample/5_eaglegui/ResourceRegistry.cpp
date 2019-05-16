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


#include "Eagle/ResourceRegistry.hpp"
#include "Eagle/BitmapHandler.hpp"
#include "Eagle/Resource.hpp"
#include "Eagle/AllegroUtilities.hpp"


using std::vector;
using std::list;



typedef list<BitmapHandler*>  BH_LIST;
typedef BH_LIST::iterator          BH_LIT;

typedef vector<BitmapHandler*> BH_REG;

typedef list<Resource*>   RES_LIST;
typedef RES_LIST::iterator     RES_LIT;



ResourceRegistry resource_registry;



BH_LIT ResourceRegistry::FindBitmapHandlerIterator(BitmapHandler* bh) {
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      if (*it == bh) {
         return it;
      }
   }
   return bh_list.end();
}



RES_LIT ResourceRegistry::FindResourceIterator(Resource* r) {
   for (RES_LIT it = res_list.begin() ; it != res_list.end() ; ++it) {
      if (*it == r) {
         return it;
      }
   }
   return res_list.end();
}



void ResourceRegistry::BuildSystemRegistry() {
   sys_bh.clear();
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (bh->MemType() == SYSTEM) {
         sys_bh.push_back(bh);
      }
   }
}



void ResourceRegistry::BuildVideoRegistry() {
   vid_bh.clear();
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (bh->MemType() == VIDEO) {
         vid_bh.push_back(bh);
      }
   }
}



ResourceRegistry::ResourceRegistry() :
      bh_list(),
      res_list(),
      sys_bh(),
      vid_bh()
{
   
}



void ResourceRegistry::RegisterBH(BitmapHandler* bh) {
   ASSERT(bh);
   if (bh) {
      if (FindBitmapHandlerIterator(bh) == bh_list.end()) {
         bh_list.push_back(bh);
      }
   }
}



void ResourceRegistry::RegisterRes(Resource* res) {
   ASSERT(res);
   if (res) {
      if (FindResourceIterator(res) == res_list.end()) {
         res_list.push_back(res);
      }
   }
}



void ResourceRegistry::UnRegisterBH(BitmapHandler* bh) {
   BH_LIT bh_lit = FindBitmapHandlerIterator(bh);
   if (bh_lit != bh_list.end()) {
      bh_list.erase(bh_lit);
   }
}



void ResourceRegistry::UnRegisterRes(Resource* res) {
   RES_LIT res_lit = FindResourceIterator(res);
   if (res_lit != res_list.end()) {
      res_list.erase(res_lit);
   }
}



bool ResourceRegistry::MoveSysVidBHToMainMemory(int new_color_depth) {
   if (ColorDepthIndex(new_color_depth) == NUMCOLORDEPTHS) {
      new_color_depth = 0;
   }
   BH_REG* sys_bh_reg = &sys_bh;
   BH_REG* vid_bh_reg = &vid_bh;
   if ((failed_sys_bh.size() == 0) && (failed_vid_bh.size() == 0)) {
      BuildSystemRegistry();
      BuildVideoRegistry();
   } else {
      sys_bh_reg = &failed_sys_bh;
      vid_bh_reg = &failed_vid_bh;
   }
   BH_REG fail_sys;
   BH_REG fail_vid;
   bool success = true;
   for (unsigned int i = 0 ; i < sys_bh_reg->size() ; ++i) {
      BitmapHandler* bh = (*sys_bh_reg)[i];
      if (new_color_depth == 0) {
         if (!bh->ChangeToMemoryType(MEMORY)) {
            success = false;
            fail_sys.push_back(bh);
         }
      } else {
         if (!bh->ChangeAttributes(new_color_depth , MEMORY)) {
            success = false;
            fail_sys.push_back(bh);
         }
      }
   }
   failed_sys_bh = fail_sys;
   
   for (unsigned int i = 0 ; i < vid_bh_reg->size() ; ++i) {
      BitmapHandler* bh = (*vid_bh_reg)[i];
      if (new_color_depth == 0) {
         if (!bh->ChangeToMemoryType(MEMORY)) {
            success = false;
            fail_vid.push_back(bh);
         }
      } else {
         if (!bh->ChangeAttributes(new_color_depth , MEMORY)) {
            success = false;
            fail_vid.push_back(bh);
         }
      }
   }
   failed_vid_bh = fail_vid;
   
   return success;
}



bool ResourceRegistry::MoveSysBHToMainMemory(int new_color_depth) {
   if (ColorDepthIndex(new_color_depth) == NUMCOLORDEPTHS) {
      new_color_depth = 0;
   }
   BH_REG* sys_bh_reg = &sys_bh;
   if (failed_sys_bh.size() == 0) {
      BuildSystemRegistry();
   } else {
      sys_bh_reg = &failed_sys_bh;
   }
   BH_REG fail_sys;
   bool success = true;
   for (unsigned int i = 0 ; i < sys_bh_reg->size() ; ++i) {
      BitmapHandler* bh = (*sys_bh_reg)[i];
      if (new_color_depth == 0) {
         if (!bh->ChangeToMemoryType(MEMORY)) {
            success = false;
            fail_sys.push_back(bh);
         }
      } else {
         if (!bh->ChangeAttributes(new_color_depth , MEMORY)) {
            success = false;
            fail_sys.push_back(bh);
         }
      }
   }
   failed_sys_bh = fail_sys;
   
   return success;
}



bool ResourceRegistry::MoveVidBHToMainMemory(int new_color_depth) {
   if (ColorDepthIndex(new_color_depth) == NUMCOLORDEPTHS) {
      new_color_depth = 0;
   }
   BH_REG* vid_bh_reg = &vid_bh;
   if (failed_vid_bh.size() == 0) {
      BuildVideoRegistry();
   } else {
      vid_bh_reg = &failed_vid_bh;
   }
   BH_REG fail_vid;
   bool success = true;
   for (unsigned int i = 0 ; i < vid_bh_reg->size() ; ++i) {
      BitmapHandler* bh = (*vid_bh_reg)[i];
      if (new_color_depth == 0) {
         if (!bh->ChangeToMemoryType(MEMORY)) {
            success = false;
            fail_vid.push_back(bh);
         }
      } else {
         if (!bh->ChangeAttributes(new_color_depth , MEMORY)) {
            success = false;
            fail_vid.push_back(bh);
         }
      }
   }
   failed_vid_bh = fail_vid;
   
   return success;
}



bool ResourceRegistry::RestoreSysVidBHFromMainMemory() {
   if ((sys_bh.size() == 0) && (vid_bh.size() == 0)) {
      return true;
   }
   bool success = true;
   failed_sys_bh.clear();
   failed_vid_bh.clear();
   for (unsigned int i = 0 ; i < sys_bh.size() ; ++i) {
      BitmapHandler* bh = sys_bh[i];
      if (!bh->ChangeToMemoryType(SYSTEM)) {
         success = false;
         failed_sys_bh.push_back(bh);
      }
   }
   for (unsigned int i = 0 ; i < vid_bh.size() ; ++i) {
      BitmapHandler* bh = vid_bh[i];
      if (!bh->ChangeToMemoryType(VIDEO)) {
         success = false;
         failed_vid_bh.push_back(bh);
      }
   }
   sys_bh = failed_sys_bh;
   vid_bh = failed_vid_bh;
   return success;
}



bool ResourceRegistry::RestoreSysBHFromMainMemory() {
   if (sys_bh.size() == 0) {return true;}
   bool success = true;
   failed_sys_bh.clear();
   for (unsigned int i = 0 ; i < sys_bh.size() ; ++i) {
      BitmapHandler* bh = sys_bh[i];
      if (!bh->ChangeToMemoryType(SYSTEM)) {
         success = false;
         failed_sys_bh.push_back(bh);
      }
   }
   sys_bh = failed_sys_bh;
   return success;
}



bool ResourceRegistry::RestoreVidBHFromMainMemory() {
   if (vid_bh.size() == 0) {return true;}
   bool success = true;
   failed_vid_bh.clear();
   for (unsigned int i = 0 ; i < vid_bh.size() ; ++i) {
      BitmapHandler* bh = vid_bh[i];
      if (!bh->ChangeToMemoryType(VIDEO)) {
         success = false;
         failed_vid_bh.push_back(bh);
      }
   }
   vid_bh = failed_vid_bh;
   return success;
}



bool ResourceRegistry::ChangeBHColorDepth(int new_color_depth) {
   bool success = true;
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (!bh->ChangeColorDepth(new_color_depth)) {success = false;}
   }
   return success;
}



void ResourceRegistry::FreeBitmapHandlers() {
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      bh->Free();
   }
}



bool ResourceRegistry::BitmapHandlersAllocated() {
   bool allocated = true;
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (!(*bh)) {allocated = false;}
   }
   return allocated;
}



bool ResourceRegistry::ReallocBitmapHandlers() {
   bool success = true;
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (!bh->Realloc()) {success = false;}
   }
   return success;
}



bool ResourceRegistry::ReloadBitmapHandlers() {
   bool success = true;
   for (BH_LIT it = bh_list.begin() ; it != bh_list.end() ; ++it) {
      BitmapHandler* bh = *it;
      if (!bh->Reload()) {success = false;}
   }
   return success;
}



void ResourceRegistry::FreeResources() {
   for (RES_LIT it = res_list.begin() ; it != res_list.end() ; ++it) {
      Resource* r = *it;
      r->Free();
   }
}



bool ResourceRegistry::AcquireResources() {
   bool success = true;
   for (RES_LIT it = res_list.begin() ; it != res_list.end() ; ++it) {
      Resource* r = *it;
      if (!r->Acquire()) {success = false;}
   }
   return success;
}



bool ResourceRegistry::ResourcesReady() {
   bool ready = true;
   for (RES_LIT it = res_list.begin() ; it != res_list.end() ; ++it) {
      Resource* r = *it;
      if (!r->Ready()) {ready = false;}
   }
   return ready;
}



bool ResourceRegistry::Ready() {
   return (BitmapHandlersAllocated() && ResourcesReady());
}








