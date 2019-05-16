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


#ifndef TileMap_H
#define TileMap_H


#include <map>
#include <vector>
#include <string>

#include "GuiCode/WidgetHandler.hpp"
#include "GuiCode/Animation.hpp"



// Normal arrangement
///   0 1 2 3
/// 0
/// 1
/// 2
/// 3


// Upright arrangement
/// 3
/// 2
/// 1
/// 0
///   0 1 2 3



class AnimatedTileMap {
protected :

   typedef std::map<std::string , SimpleBitmapAnimation*> SAMAP;
   typedef SAMAP::iterator SAMIT;

   int tile_width;
   int tile_height;
   int num_tiles_wide;
   int num_tiles_tall;
   int num_tiles;
   SimpleBitmapAnimation** tilemap;
   
   SAMAP animap;
   std::vector<SimpleBitmapAnimation*> anime_to_free;
   
   
public :
   AnimatedTileMap();
   AnimatedTileMap(int tilewidth , int tileheight , int numtileswide , int numtilestall);
   ~AnimatedTileMap();

   void ReallocTileMap(int tilewidth , int tileheight , int numtileswide , int numtilestall);
   void FreeTileMap();
   void FreeRegisteredAnime();

   void RegisterTile(std::string tilename , BITMAP* bmp , BMP_DRAW_TYPE bmp_draw_type);// will replace tiles if already registered
   void RegisterTile(std::string tilename , SimpleBitmapAnimation* sba);// will replace tiles if already registered

   void PlaceTile(std::string tilename , int tile_x , int tile_y);// tilename = "" places Null tile (empty space)
   
   void Update(double tsec);

   // [Normal | Upright][Full | Dirty]DrawOn[Gui | Bitmap]
   void NormalFullDrawOnGui(WidgetHandler& gui , int guix , int guiy);// top left corner position
   void NormalDirtyDrawOnGui(WidgetHandler& gui , int guix , int guiy);// top left corner position
   void UprightFullDrawOnGui(WidgetHandler& gui , int guix , int guiy);// bottom left corner position
   void UprightDirtyDrawOnGui(WidgetHandler& gui , int guix , int guiy);// bottom left corner position

   void NormalFullDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy);// top left corner position
   void NormalDirtyDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy);// top left corner position
   void UprightFullDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy);// bottom left corner position
   void UprightDirtyDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy);// bottom left corner position

   void NormalPartialDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy , int tile_x , int tile_y , int numtileswide , int numtilestall);// top left corner position
   void UprightPartialDrawOnBitmap(BITMAP* bmp , int bmpx , int bmpy , int tile_x , int tile_y , int numtileswide , int numtilestall);// bottom left corner position

   void TilePosToRealPos(int tx , int ty , int* rx , int* ry);
   void RealPosToTilePos(int rx , int ry , int* tx , int* ty);
   Rectangle TileArea(int tile_x , int tile_y);

   /// Getters
   SimpleBitmapAnimation* GetSBAPointer(std::string tilename);
   int TileWidth() {return tile_width;}
   int TileHeight() {return tile_height;}
   int NumTilesWide() {return num_tiles_wide;}
   int NumTilesTall() {return num_tiles_tall;}
   int NumTiles() {return num_tiles;}

};



#endif // TileMap_H

