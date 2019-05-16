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



#include "Eagle/TileMap.hpp"

using std::map;
using std::vector;
using std::string;
using std::endl;

typedef map<string , SimpleBitmapAnimation*> SAMAP;
typedef SAMAP::iterator SAMIT;



AnimatedTileMap::AnimatedTileMap() :
   tile_width(0),
   tile_height(0),
   num_tiles_wide(0),
   num_tiles_tall(0),
   tilemap(0),
   animap(),
   anime_to_free()
{}



AnimatedTileMap::AnimatedTileMap(int tilewidth , int tileheight , int numtileswide , int numtilestall) :
   tile_width(0),
   tile_height(0),
   num_tiles_wide(0),
   num_tiles_tall(0),
   tilemap(0),
   animap(),
   anime_to_free()
{
   ReallocTileMap(tilewidth , tileheight , numtileswide , numtilestall);
}



AnimatedTileMap::~AnimatedTileMap() {
   FreeTileMap();
   FreeRegisteredAnime();
}



void AnimatedTileMap::ReallocTileMap(int tilewidth , int tileheight , int numtileswide , int numtilestall) {
   ASSERT(tilewidth > 0);
   ASSERT(tileheight > 0);
   ASSERT(numtileswide > 0);
   ASSERT(numtilestall > 0);
   FreeTileMap();
   tile_width = tilewidth;
   tile_height = tileheight;
   num_tiles_wide = numtileswide;
   num_tiles_tall = numtilestall;
   num_tiles = num_tiles_wide*num_tiles_tall;
   tilemap = new SimpleBitmapAnimation*[num_tiles];
   for (int i = 0 ; i < num_tiles ; ++i) {
      tilemap[i] = 0;
   }
}



void AnimatedTileMap::FreeTileMap() {
   if (tilemap) {
      delete [] tilemap;
      tilemap = 0;
      tile_width = 0;
      tile_height = 0;
      num_tiles_wide = 0;
      num_tiles_tall = 0;
      num_tiles = 0;
   }
}



void AnimatedTileMap::FreeRegisteredAnime() {
   for (unsigned int i = 0 ; i < anime_to_free.size() ; ++i) {
      delete anime_to_free[i];
   }
   anime_to_free.clear();
}



void AnimatedTileMap::RegisterTile(string tilename , BITMAP* bmp , BMP_DRAW_TYPE bmp_draw_type) {
   ASSERT(tilename.size());// Give the tile a name
   ASSERT(bmp);
   if (tilename.size() && bmp) {
      SimpleBitmapAnimation* sba = new SimpleBitmapAnimation(
            string("tile-") + tilename , Rectangle(0 , 0 , bmp->w , bmp->h) , FORWARD_REPEAT , bmp_draw_type , 1 , 5.0
      );
      sba->SetBitmap(bmp , false);
      sba->Pause();
      sba->SetRedrawFlag();
      anime_to_free.push_back(sba);
      SAMIT it = animap.find(tilename);
      if (it != animap.end()) {// Old tile already on map , replace it
         SimpleBitmapAnimation* sbaold = it->second;
         for (int i = 0 ; i < num_tiles ; ++i) {
            if (tilemap[i] == sbaold) {
               tilemap[i] = sba;
            }
         }
      }
      animap[tilename] = sba;
   } else {
      OutputLog() << "Tile not registered. tilename = '" << tilename.c_str() << "' , bmp = " << bmp;
      if (tilename.size() == 0) {OutputLog() << " Empty tilename string not allowed.";}
      if (bmp == 0) {OutputLog() << " Null BITMAP* not allowed.";}
      OutputLog() << endl;
   }
}



void AnimatedTileMap::RegisterTile(string tilename , SimpleBitmapAnimation* sba) {
   ASSERT(tilename.size());// Give the tile a name
   ASSERT(sba);
   if (tilename.size() && sba) {
      SAMIT it = animap.find(tilename);
      if (it != animap.end()) {// Old tile already on map , replace it
         SimpleBitmapAnimation* sbaold = it->second;
         for (int i = 0 ; i < num_tiles ; ++i) {
            if (tilemap[i] == sbaold) {
               tilemap[i] = sba;
            }
         }
      }
      animap[tilename] = sba;
   } else {
      OutputLog() << "Tile not registered. tilename = '" << tilename.c_str() << "' , sba = " << sba;
      if (tilename.size() == 0) {OutputLog() << " Empty tilename string not allowed.";}
      if (sba == 0) {OutputLog() << " Null SimpleBitmapAnimation* not allowed.";}
      OutputLog() << endl;
   }
}



void AnimatedTileMap::PlaceTile(string tilename , int tile_x , int tile_y) {
   ASSERT(tile_x >= 0);
   ASSERT(tile_x < num_tiles_wide);
   ASSERT(tile_y >= 0);
   ASSERT(tile_y < num_tiles_tall);
   int tile_num = tile_y*num_tiles_wide + tile_x;
   if (tilename.size() == 0) {
      if ( ((tile_x >= 0) && (tile_x < num_tiles_wide)) &&
           ((tile_y >= 0) && (tile_y < num_tiles_tall)) ) {
         tilemap[tile_num] = 0;// empty tile
      } else {
         OutputLog() << "Couldn't place empty tile. tile_x = " << tile_x << " , tile_y = " << tile_y << ".";
         if (!((tile_x >= 0) && (tile_x < num_tiles_wide))) {OutputLog() << " Tile x is out of bounds.";}
         if (!((tile_y >= 0) && (tile_y < num_tiles_tall))) {OutputLog() << " Tile y is out of bounds.";}
         OutputLog() << endl;
      }
      return;
   }
   
   SAMIT it = animap.find(tilename);
   if (it != animap.end() &&
      ((tile_x >= 0) && (tile_x < num_tiles_wide)) &&
      ((tile_y >= 0) && (tile_y < num_tiles_tall)) ) {
      SimpleBitmapAnimation* sba = it->second;
      sba->SetRedrawFlag();
      tilemap[tile_num] = sba;
   } else {
      OutputLog() << "Couldn't place tile. tilename = " << tilename.c_str() << " , tile_x = " << tile_x << " , tile_y = " << tile_y;
      if (it == animap.end()) {OutputLog() << " Tilename not found in tile registry.";}
      if (!((tile_x >= 0) && (tile_x < num_tiles_wide))) {OutputLog() << " Tile x is out of bounds.";}
      if (!((tile_y >= 0) && (tile_y < num_tiles_tall))) {OutputLog() << " Tile y is out of bounds.";}
      OutputLog() << endl;
   }
}



void AnimatedTileMap::Update(double tsec) {
   for (SAMIT it = animap.begin() ; it != animap.end() ; ++it) {
      SimpleBitmapAnimation* sba = it->second;
      sba->Update(tsec);
   }
}



void AnimatedTileMap::NormalFullDrawOnGui(WidgetHandler& gui , int guix , int guiy) {
   BITMAP* bmp = gui.BackgroundBitmap();
   ASSERT(bmp);
   if (bmp) {
      Rectangle r(guix , guiy , num_tiles_wide*tile_width , num_tiles_tall*tile_height);
      gui.MakeAreaDirty(r);
      int tx = guix;
      int ty = guiy;
      int tile_num;
      for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
         tx = guix;
         for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
            tile_num = gy*num_tiles_wide + gx;
            SimpleBitmapAnimation* sba = tilemap[tile_num];
            if (sba) {
               sba->DrawOn(bmp , tx , ty);
            }
            tx += tile_width;
         }
         ty += tile_height;
      }
   }
}



void AnimatedTileMap::NormalDirtyDrawOnGui(WidgetHandler& gui , int guix , int guiy) {
   BITMAP* bmp = gui.BackgroundBitmap();
   ASSERT(bmp);
   if (bmp) {
      int tx = guix;
      int ty = guiy;
      int tile_num;
      for (SAMIT it = animap.begin() ; it != animap.end() ; ++it) {
         SimpleBitmapAnimation* csba = it->second;
         UINT wflags = csba->Flags();
         if (wflags & NEEDS_REDRAW) {
            for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
               tx = guix;
               for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
                  tile_num = gy*num_tiles_wide + gx;
                  SimpleBitmapAnimation* sba = tilemap[tile_num];
                  if (csba == sba) {
                     sba->DrawOn(bmp , tx , ty);
                     Rectangle r(tx , ty , tile_width , tile_height);
                     gui.MakeAreaDirty(r);
                  }
                  tx += tile_width;
               }
               ty += tile_height;
            }
         }
      }
   }
}



void AnimatedTileMap::UprightFullDrawOnGui(WidgetHandler& gui , int guix , int guiy) {
   BITMAP* bmp = gui.BackgroundBitmap();
   ASSERT(bmp);
   if (bmp) {
      Rectangle r(guix , guiy , num_tiles_wide*tile_width , num_tiles_tall*tile_height);
      gui.MakeAreaDirty(r);
      int tx = guix;
      int ty = guiy - tile_height;
      int tile_num;
      for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
         tx = guix;
         for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
            tile_num = gy*num_tiles_wide + gx;
            SimpleBitmapAnimation* sba = tilemap[tile_num];
            if (sba) {
               sba->DrawOn(bmp , tx , ty);
            }
            tx += tile_width;
         }
         ty -= tile_height;
      }
   }
}



void AnimatedTileMap::UprightDirtyDrawOnGui(WidgetHandler& gui , int guix , int guiy) {
   BITMAP* bmp = gui.BackgroundBitmap();
   ASSERT(bmp);
   if (bmp) {
      int tx = guix;
      int ty = guiy - tile_height;
      int tile_num;
      for (SAMIT it = animap.begin() ; it != animap.end() ; ++it) {
         SimpleBitmapAnimation* csba = it->second;
         UINT wflags = csba->Flags();
         if (wflags & NEEDS_REDRAW) {
            for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
               tx = guix;
               for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
                  tile_num = gy*num_tiles_wide + gx;
                  SimpleBitmapAnimation* sba = tilemap[tile_num];
                  if (csba == sba) {
                     sba->DrawOn(bmp , tx , ty);
                     Rectangle r(tx , ty , tile_width , tile_height);
                     gui.MakeAreaDirty(r);
                  }
                  tx += tile_width;
               }
               ty -= tile_height;
            }
         }
      }
   }
}



void AnimatedTileMap::NormalFullDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy) {
   ASSERT(bmp);
   if (bmp) {
      int tx = bmpx;
      int ty = bmpy;
      int tile_num;
      for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
         tx = bmpx;
         for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
            tile_num = gy*num_tiles_wide + gx;
            SimpleBitmapAnimation* sba = tilemap[tile_num];
            if (sba) {
               sba->DrawOn(bmp , tx , ty);
            }
            tx += tile_width;
         }
         ty += tile_height;
      }
   }
}



void AnimatedTileMap::NormalDirtyDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy) {
   ASSERT(bmp);
   if (bmp) {
      int tx = bmpx;
      int ty = bmpy;
      int tile_num;
      for (SAMIT it = animap.begin() ; it != animap.end() ; ++it) {
         SimpleBitmapAnimation* csba = it->second;
         UINT wflags = csba->Flags();
         if (wflags & NEEDS_REDRAW) {
            for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
               tx = bmpx;
               for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
                  tile_num = gy*num_tiles_wide + gx;
                  SimpleBitmapAnimation* sba = tilemap[tile_num];
                  if (csba == sba) {
                     sba->DrawOn(bmp , tx , ty);
                  }
                  tx += tile_width;
               }
               ty += tile_height;
            }
         }
      }
   }
}



void AnimatedTileMap::UprightFullDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy) {
   ASSERT(bmp);
   if (bmp) {
      int tx = bmpx;
      int ty = bmpy - tile_height;
      int tile_num;
      for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
         tx = bmpx;
         for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
            tile_num = gy*num_tiles_wide + gx;
            SimpleBitmapAnimation* sba = tilemap[tile_num];
            if (sba) {
               sba->DrawOn(bmp , tx , ty);
            }
            tx += tile_width;
         }
         ty -= tile_height;
      }
   }
}



void AnimatedTileMap::UprightDirtyDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy) {
   ASSERT(bmp);
   if (bmp) {
      int tx = bmpx;
      int ty = bmpy - tile_height;
      int tile_num;
      for (SAMIT it = animap.begin() ; it != animap.end() ; ++it) {
         SimpleBitmapAnimation* csba = it->second;
         UINT wflags = csba->Flags();
         if (wflags & NEEDS_REDRAW) {
            for (int gy = 0 ; gy < num_tiles_tall ; ++gy) {
               tx = bmpx;
               for (int gx = 0 ; gx < num_tiles_wide ; ++gx) {
                  tile_num = gy*num_tiles_wide + gx;
                  SimpleBitmapAnimation* sba = tilemap[tile_num];
                  if (csba == sba) {
                     sba->DrawOn(bmp , tx , ty);
                  }
                  tx += tile_width;
               }
               ty -= tile_height;
            }
         }
      }
   }
}



void AnimatedTileMap::NormalPartialDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy , int tile_x , int tile_y , int numtileswide , int numtilestall) {
   ASSERT(bmp);
   if (bmpx >= bmp->w) {return;}
   if (bmpy >= bmp->h) {return;}
   if (tile_x >= num_tiles_wide) {return;}
   if (tile_y >= num_tiles_tall) {return;}
   if (numtileswide < 1) {return;}
   if (numtilestall < 1) {return;}
   if (tile_x < 0) {
      numtileswide += tile_x;
      tile_x = 0;
   }
   if (tile_y < 0) {
      numtilestall += tile_y;
      tile_y = 0;
   }
   if (tile_x + numtileswide > num_tiles_wide) {
      numtileswide = num_tiles_wide - tile_x;
   }
   if (tile_y + numtilestall > num_tiles_tall) {
      numtilestall = num_tiles_tall - tile_y;
   }
   int tx = tile_x;
   int ty = tile_y;
   int tile_num;
   int bx = bmpx;
   int by = bmpy;
   for (int y = 0 ; y < numtilestall ; ++y) {
      tx = tile_x;
      bx = bmpx;
      for (int x = 0 ; x < numtileswide ; ++x) {
         tile_num = ty*num_tiles_wide + tx;
         SimpleBitmapAnimation* sba = tilemap[tile_num];
         if (sba) {
            sba->DrawOn(bmp , bx , by);
         }
         ++tx;
         bx += tile_width;
      }
      ++ty;
      by += tile_height;
   }
}



void AnimatedTileMap::UprightPartialDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy , int tile_x , int tile_y , int numtileswide , int numtilestall) {
   ASSERT(bmp);
   if (bmpx >= bmp->w) {return;}
   if (bmpy < 0) {return;}
   if (tile_x >= num_tiles_wide) {return;}
   if (tile_y >= num_tiles_tall) {return;}
   if (numtileswide < 1) {return;}
   if (numtilestall < 1) {return;}
   if (tile_x < 0) {
      numtileswide += tile_x;
      tile_x = 0;
   }
   if (tile_y < 0) {
      numtilestall += tile_y;
      tile_y = 0;
   }
   if (tile_x + numtileswide > num_tiles_wide) {
      numtileswide = num_tiles_wide - tile_x;
   }
   if (tile_y + numtilestall > num_tiles_tall) {
      numtilestall = num_tiles_tall - tile_y;
   }
   int tx = tile_x;
   int ty = tile_y;
   int tile_num;
   int bx = bmpx;
   int by = bmpy - tile_height;
   for (int y = 0 ; y < numtilestall ; ++y) {
      tx = tile_x;
      bx = bmpx;
      for (int x = 0 ; x < numtileswide ; ++x) {
         tile_num = ty*num_tiles_wide + tx;
         SimpleBitmapAnimation* sba = tilemap[tile_num];
         if (sba) {
            sba->DrawOn(bmp , bx , by);
         }
         ++tx;
         bx += tile_width;
      }
      ++ty;
      by -= tile_height;
   }
}



void AnimatedTileMap::TilePosToRealPos(int tx , int ty , int* rx , int* ry) {
   if (rx) {*rx = tx*tile_width;}
   if (ry) {*ry = ty*tile_height;}
}



void AnimatedTileMap::RealPosToTilePos(int rx , int ry , int* tx , int* ty) {
   if (tx) {*tx = rx/tile_width;}
   if (ty) {*ty = ry/tile_height;}
}



Rectangle AnimatedTileMap::TileArea(int tile_x , int tile_y) {
   return Rectangle(tile_x*tile_width , tile_y*tile_height , tile_width , tile_height);
}



SimpleBitmapAnimation* AnimatedTileMap::GetSBAPointer(string tilename) {
   SAMIT it = animap.find(tilename);
   if (it != animap.end()) {
      return it->second;
   }
   return 0;
}






