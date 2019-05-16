
#include "Eagle/ClipBoard.hpp"
#include <winalleg.h>

#include "Eagle/Logging.hpp"
using std::endl;

int IsClipboardStringAvailable() {
   /// TODO : Implement
   return 0;
}



int IsClipboardBitmapAvailable(const char* image_type) {
   /// TODO : Implement
   return 0;
}



int SetClipboardString(const char* cstr) {
   HGLOBAL clipBuffer = 0;
   char* buffer = 0;
   HWND allegro_window = win_get_window();
   
   if (!cstr) {return 1;}
   if (OpenClipboard(allegro_window) == 0) {return 1;}
   
   EmptyClipboard();
   
   clipBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT , strlen(cstr) + 1);
   buffer = (char*)GlobalLock(clipBuffer);
   strcpy(buffer , cstr);
   GlobalUnlock(clipBuffer);
   
   if (SetClipboardData(CF_TEXT , clipBuffer) == (void*)0) {
      CloseClipboard();
      return 1;
   }
   CloseClipboard();
   return 0;
}



int SetClipboardBitmap(BITMAP* bmp , const char* image_extension) {
   HWND allegro_window = win_get_window();
   HBITMAP hbitmap = 0;
   
   if (!bmp) {return 1;}
   hbitmap = convert_bitmap_to_hbitmap(bmp);
   if (!bmp) {return 1;}
   
   if (OpenClipboard(allegro_window) == 0) {return 1;}
   
   if (SetClipboardData(CF_BITMAP , hbitmap) == (void*)0) {
      CloseClipboard();
      return 1;
   }
   CloseClipboard();
   return 0;
}



char* GetNewClipboardString() {
   char* clipstr = 0;
   char* newstr = 0;
   HWND allegro_window = win_get_window();
   HANDLE cliphandle = 0;
   
   if (IsClipboardFormatAvailable(CF_TEXT) == 0) {return (char*)0;}

   if (OpenClipboard(allegro_window) == 0) {return (char*)0;}
   cliphandle = GetClipboardData(CF_TEXT);
   if (cliphandle == (void*)0) {
      CloseClipboard();
      return (char*)0;
   }
   clipstr = (char*)GlobalLock(cliphandle);
//   clipstr = (char*)cliphandle;
   newstr = strdup(clipstr);
   GlobalUnlock(cliphandle);
   CloseClipboard();
   return newstr;
}



BITMAP* GetNewClipboardBitmap(const char* image_extension) {
   HBITMAP clipbmp = 0;
   BITMAP* bmp = 0;
   HWND allegro_window = win_get_window();
   HANDLE cliphandle = 0;
   
   if (IsClipboardFormatAvailable(CF_BITMAP) == 0) {return (BITMAP*)0;}
   
   if (OpenClipboard(allegro_window) == 0) {return (BITMAP*)0;}
   cliphandle = GetClipboardData(CF_BITMAP);
   if (cliphandle == (void*)0) {
      CloseClipboard();
      return (BITMAP*)0;
   }
   clipbmp = (HBITMAP)cliphandle;
   bmp = convert_hbitmap_to_bitmap(clipbmp);
   CloseClipboard();
   return bmp;
}



void LogAvailableClipboardTargets(void) {
   OutputLog() << "The clipboard library does not support getting available clipboard targets on Windows yet." << endl;
}

