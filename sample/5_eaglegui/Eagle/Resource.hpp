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


#ifndef Resource_H
#define Resource_H


#include <allegro.h>
#include <string>
#include <map>
#include <vector>

#include "Eagle/ResourceRegistry.hpp"



/** Loading a bitmap's palette is not presently supported */

enum RES_SRC {
   RES_SRC_NONE     = 0,
   RES_SRC_FILE     = 1,
   RES_SRC_DATAFILE = 2
};

enum RES_TYPE {
   RES_TYPE_NONE     = 0,
   RES_TYPE_BITMAP   = 1,
   RES_TYPE_SAMPLE   = 2,
   RES_TYPE_MIDI     = 3,
   RES_TYPE_DATAFILE = 4
};
   

/// Not designed to be container friendly or copied often. Use pointers instead.
/// Copy constructor may make a full 'copy', either loading a fresh (full) copy of the resource or
///   obtaining a new copy of the pointer from the datafile (shallow copy). Destructor frees the resources
///   so if you have global Resource objects, you need to call Free() on them before main exits and
///   don't call Acquire() until allegro is initialized. You can still have global Resource objects by
///   setting acquire to false in the Resource constructor calls.

/// The resources that a Resource object holds are freed automatically when the object goes out of scope.
/// In the case of RES_SRC_DATAFILE type objects, no memory is actually freed. However, you can have a
/// RES_SRC_FILE type object that holds a RES_TYPE_DATAFILE, and the datafile would be freed then.

class ResourceRegistry;

class Resource {
private :
//   std::string name;
   RES_SRC src;
   RES_TYPE type;

   std::string location;// For RES_SRC_FILE's

   DATAFILE** pdatfile;// For RES_SRC_DATAFILE's , the address of the datafile is taken so that it may be reloaded if necessary
   unsigned int dat_index;// For RES_SRC_DATAFILE's
   
   void* resource;
   RGB* palette;
   
   ResourceRegistry* registry;

public :
   
   // The acquire parameter in the constructors allows for delayed loading and global resources

   Resource(const std::string& file , RES_TYPE res_type ,
            bool acquire = true , ResourceRegistry* reg = &resource_registry);

   Resource(DATAFILE** dat_addr , unsigned int datafile_index , RES_TYPE res_type ,
            bool acquire = true , ResourceRegistry* reg = &resource_registry);

   Resource(const Resource& datafile_res , unsigned int datafile_index , RES_TYPE res_type ,
            bool acquire = true , ResourceRegistry* reg = &resource_registry);

   ~Resource();

   void Free();
   bool Acquire();
   

   inline bool Ready();

   inline BITMAP*   BitmapRes();
   inline RGB*      PaletteRes();
   inline SAMPLE*   SampleRes();
   inline MIDI*     MidiRes();
   inline DATAFILE* DatafileRes();

   inline operator BITMAP*()   {return BitmapRes();}
   inline operator RGB*()      {return PaletteRes();}
   inline operator SAMPLE*()   {return SampleRes();}
   inline operator MIDI*()     {return MidiRes();}
   inline operator DATAFILE*() {return DatafileRes();}
   inline operator bool() {return (bool)resource;}
   
///   void* ResourceAddress() {return &resource;} // TODO : This would have to be available to 
                                                  ///       use datafile Resources as a DATAFILE source.
                                                  ///       The Resource constructor that takes another Resource
                                                  ///       as a parameter will handle this for you though.
};

/* Usage examples

   Resource bgpic("PrettyPicture.bmp" , RES_TYPE_BITMAP);// Loads automatically
   Resource bgpic("AnotherPrettyPicture.bmp" , RES_TYPE_BITMAP , false);// Does not load until Acquire is called.
   
   Resource datfile("resources.dat" , RES_TYPE_DATAFILE);
   Resource spritesheet(datfile , PLAYER_SPRITE , RES_TYPE_BITMAP);// Grabs the PLAYER_SPRITE BITMAP* from the datafile
                                                                   // just loaded from 'resources.dat' in the previous line.
   Resource sound_of_waves(datfile , WAVES_SOUND , RES_TYPE_SAMPLE);

   /// You can use a Resource object mostly like it's actual counter part -
   // Show off the spritesheet -
   blit(spritesheet , screen , 0 , 0 , 0 , 0 , ((BITMAP*)spritesheet)->w , ((BITMAP*)spritesheet)->h);
   // to the soothing sound of ocean waves. Why not?
   play_sample(sound_of_waves , 255 , 127 , 1000 , 1);

//*/


class ResourceManager {
   
   typedef std::map<std::string , Resource*> RESMAP;
   typedef RESMAP::iterator RMIT;

private :
   RESMAP resmap;
   std::vector<Resource*> resvec;
   
public :
   ResourceManager() : resmap() , resvec() {}
   ~ResourceManager() {Clear();}
   
   void Clear();
   void Clear(const std::string& name);
   void FreeAllResources();
   void FreeResource(const std::string& name);
   bool AcquireAllResources(bool stop_on_failure);
   bool AcquireResource(const std::string& name);
   
   bool LoadFileResource(const std::string& name , const std::string& file , RES_TYPE res_type , 
                         bool acquire = true , ResourceRegistry* reg = &resource_registry);
   bool LoadDataFileResource(const std::string& name , DATAFILE** dat_addr , unsigned int datafile_index , RES_TYPE res_type ,
                             bool acquire = true , ResourceRegistry* reg = &resource_registry);
   bool LoadDataFileResource(const std::string& name , const Resource& datafile_res , unsigned int datafile_index , RES_TYPE res_type ,
                             bool acquire = true , ResourceRegistry* reg = &resource_registry);

   Resource* GetResourceByName(const std::string& name);
   BITMAP* GetBitmapByName(const std::string& name);
   SAMPLE* GetSampleByName(const std::string& name);
   MIDI* GetMidiByName(const std::string& name);
   DATAFILE* GetDatafileByName(const std::string& name);

};


/** Inline Resource class methods */


inline bool Resource::Ready() {return (bool)resource;}

inline BITMAP* Resource::BitmapRes() {
   ASSERT(type == RES_TYPE_BITMAP);
   if (type == RES_TYPE_BITMAP) {
      return (BITMAP*)resource;
   }
   return 0;
}

inline RGB* Resource::PaletteRes() {
   ASSERT((type == RES_TYPE_BITMAP) && (src == RES_SRC_FILE));
   if ((type == RES_TYPE_BITMAP) && (src == RES_SRC_FILE)) {
      return palette;
   }
   return 0;
}

inline SAMPLE* Resource::SampleRes() {
   ASSERT(type == RES_TYPE_SAMPLE);
   if (type == RES_TYPE_SAMPLE) {
      return (SAMPLE*)resource;
   }
   return 0;
}

inline MIDI* Resource::MidiRes() {
   ASSERT(type == RES_TYPE_MIDI);
   if (type == RES_TYPE_MIDI) {
      return (MIDI*)resource;
   }
   return 0;
}

inline DATAFILE* Resource::DatafileRes() {
   ASSERT(type == RES_TYPE_DATAFILE);
   if (type == RES_TYPE_DATAFILE) {
      return (DATAFILE*)resource;
   }
   return 0;
}





#endif // Resource_H

