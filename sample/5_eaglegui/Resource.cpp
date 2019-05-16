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
#include "Eagle/Logging.hpp"
#include "Eagle/Resource.hpp"


using std::string;
using std::map;
using std::vector;
using std::endl;


Resource::Resource(const string& file , RES_TYPE res_type , bool acquire , ResourceRegistry* reg) :
         src(RES_SRC_FILE) , 
         type(res_type) , 
         location(file) , 
         pdatfile(0) , 
         dat_index(0) , 
         resource(0),
         palette(0),
         registry(reg)
{
   if (acquire) {Acquire();}
}



Resource::Resource(DATAFILE** dat_addr , unsigned int datafile_index , RES_TYPE res_type , bool acquire , ResourceRegistry* reg) :
         src(RES_SRC_DATAFILE) , 
         type(res_type) , 
         location() , 
         pdatfile(dat_addr) , 
         dat_index(datafile_index) , 
         resource(0),
         palette(0),
         registry(reg)
{
   if (acquire) {Acquire();}
}



Resource::Resource(const Resource& datafile_res , unsigned int datafile_index , RES_TYPE res_type , bool acquire , ResourceRegistry* reg) :
         src(RES_SRC_DATAFILE) , 
         type(res_type) , 
         location() , 
         pdatfile((DATAFILE**)&datafile_res.resource) , 
         dat_index(datafile_index) , 
         resource(0),
         palette(0),
         registry(reg)
{
   if (acquire) {Acquire();}
}



Resource::~Resource() {
   if (registry) {
      registry->UnRegisterRes(this);
   }
   Free();
}



void Resource::Free() {
   if (resource) {
      if (src != RES_SRC_DATAFILE) {
         switch (type) {
            case RES_TYPE_NONE     : 
               break;
            case RES_TYPE_BITMAP   : 
               delete [] palette;
               palette = 0;
               destroy_bitmap(BitmapRes());
               break;
            case RES_TYPE_SAMPLE   : 
               destroy_sample(SampleRes());
               break;
            case RES_TYPE_MIDI     : 
               destroy_midi(MidiRes());
               break;
            case RES_TYPE_DATAFILE : 
               unload_datafile(DatafileRes());
               break;
         }
      }
      resource = (void*)0;
   }
}



bool Resource::Acquire() {
   Free();
   if (registry) {
      registry->RegisterRes(this);
   }
   OutputLog() << "Acquiring resource : Src = ";
   switch (src) {
      case RES_SRC_NONE :
         OutputLog() << "NONE." << endl;
         break;
      case RES_SRC_FILE :
         OutputLog() << "FILE , Type = ";
         switch (type) {
            case RES_TYPE_NONE     :
               OutputLog() << "NONE. Resource is NULL." << endl;
               break;
            case RES_TYPE_BITMAP   :
               palette = new RGB[PAL_SIZE];
               resource = (void*)load_bitmap(location.c_str() , palette);
               if (!resource) {
                  delete [] palette;
                  palette = 0;
               }
               OutputLog() << "BITMAP : Loading " << location.c_str() << (resource?" succeeded":" failed") << "." << endl;
               break;
            case RES_TYPE_SAMPLE   :
               resource = (void*)load_sample(location.c_str());
               OutputLog() << "SAMPLE : Loading " << location.c_str() << (resource?" succeeded":" failed") << "." << endl;
               break;
            case RES_TYPE_MIDI :
               resource = (void*)load_midi(location.c_str());
               OutputLog() << "MIDI : Loading " << location.c_str() << (resource?" succeeded":" failed") << "." << endl;
               break;
            case RES_TYPE_DATAFILE :
               resource = (void*)load_datafile(location.c_str());
               OutputLog() << "DATAFILE : Loading " << location.c_str() << (resource?" succeeded":" failed") << "." << endl;
               break;
         }
         break;
      case RES_SRC_DATAFILE :
         OutputLog() << "DATAFILE , Type = ";
         ASSERT(pdatfile);
         ASSERT(*pdatfile);
         DATAFILE* datafile = *pdatfile;
         switch (type) {
            case RES_TYPE_NONE     :
               OutputLog() << "NONE. Resource is NULL." << endl;
               break;
            case RES_TYPE_BITMAP   :
               resource = datafile[dat_index].dat;
               OutputLog() << "BITMAP : Index " << dat_index << " " << (resource?"appears valid.":"is invalid.") << endl;
               break;
            case RES_TYPE_SAMPLE   :
               resource = datafile[dat_index].dat;
               OutputLog() << "SAMPLE : Index " << dat_index << " " << (resource?"appears valid.":"is invalid.") << endl;
               break;
            case RES_TYPE_MIDI     :
               resource = datafile[dat_index].dat;
               OutputLog() << "MIDI : Index " << dat_index << " " << (resource?"appears valid.":"is invalid.") << endl;
               break;
            case RES_TYPE_DATAFILE :
               resource = datafile[dat_index].dat;
               OutputLog() << "DATAFILE : Index " << dat_index << " " << (resource?"appears valid.":"is invalid.") << endl;
               break;
         }
         break;
   }
   return Ready();
}



/// ----------------------------     ResourceManager     -------------------------------------



typedef std::map<std::string , Resource*> RESMAP;
typedef RESMAP::iterator RMIT;



void ResourceManager::Clear() {
   for (int i = (int)resvec.size() - 1 ; i >= 0 ; --i) {
      Resource* r = resvec[i];
      delete r;
   }
   resmap.clear();
   resvec.clear();
}



void ResourceManager::Clear(const std::string& name) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      Resource* r = it->second;
      r->Free();
      delete it->second;
      resmap.erase(it);
      for (vector<Resource*>::iterator vit = resvec.begin() ; vit != resvec.end() ; ++vit) {
         if (*vit == r) {
            resvec.erase(vit);
            break;
         }
      }
   }
}



void ResourceManager::FreeAllResources() {
   for (RMIT it = resmap.begin() ; it != resmap.end() ; ++it) {
      it->second->Free();
   }
}



void ResourceManager::FreeResource(const std::string& name) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      it->second->Free();
   }
}



bool ResourceManager::AcquireAllResources(bool stop_on_failure) {
   bool success = true;
   for (unsigned int i = 0 ; i < resvec.size() ; ++i) {
      Resource* r = resvec[i];
      if (!(r->Acquire())) {
         if (stop_on_failure) {return false;}
         success = false;
      }
   }
   return success;
}



bool ResourceManager::AcquireResource(const std::string& name) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      return (it->second->Acquire());
   }
   return false;
}



bool ResourceManager::LoadFileResource(const std::string& name , const std::string& file , RES_TYPE res_type , 
                                       bool acquire , ResourceRegistry* reg) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      it->second->Free();
      delete it->second;
      resmap[name] = 0;
   }
   Resource* r = new Resource(file , res_type , acquire , reg);
   resmap[name] = r;
   return r->Ready();
}



bool ResourceManager::LoadDataFileResource(const std::string& name , DATAFILE** dat_addr , unsigned int datafile_index , RES_TYPE res_type ,
                                           bool acquire , ResourceRegistry* reg) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      it->second->Free();
      delete it->second;
      resmap[name] = 0;
   }
   Resource* r = new Resource(dat_addr , datafile_index , res_type , acquire , reg);
   resmap[name] = r;
   return r->Ready();
}



bool ResourceManager::LoadDataFileResource(const std::string& name , const Resource& datafile_res , unsigned int datafile_index , RES_TYPE res_type ,
                                           bool acquire , ResourceRegistry* reg) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      it->second->Free();
      delete it->second;
      resmap[name] = 0;
   }
   Resource* r = new Resource(datafile_res , datafile_index , res_type , acquire , reg);
   resmap[name] = r;
   return r->Ready();
}



Resource* ResourceManager::GetResourceByName(const std::string& name) {
   RMIT it = resmap.find(name);
   if (it != resmap.end()) {
      return it->second;
   }
   return 0;
}



BITMAP* ResourceManager::GetBitmapByName(const std::string& name) {
   Resource* r = GetResourceByName(name);
   if (r) {return (BITMAP*)(*r);}
   return 0;
}



SAMPLE* ResourceManager::GetSampleByName(const std::string& name) {
   Resource* r = GetResourceByName(name);
   if (r) {return (SAMPLE*)(*r);}
   return 0;
}



MIDI* ResourceManager::GetMidiByName(const std::string& name) {
   Resource* r = GetResourceByName(name);
   if (r) {return (MIDI*)(*r);}
   return 0;
}



DATAFILE* ResourceManager::GetDatafileByName(const std::string& name) {
   Resource* r = GetResourceByName(name);
   if (r) {return (DATAFILE*)(*r);}
   return 0;
}








