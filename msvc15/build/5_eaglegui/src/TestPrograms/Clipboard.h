

#ifndef Clipboard_H
#define Clipboard_H

#include <allegro.h>

#include <stdio.h>



#ifdef __cplusplus
   extern "C" {
#endif



FILE* Log(void);
void CloseLog(void);



int IsClipboardStringAvailable();
int IsClipboardBitmapAvailable(const char* image_type);



// returns 0 if successful, non-zero otherwise
int SetClipboardString(const char* cstr);



// SetClipboardBitmap returns 0 if successful, non-zero otherwise
// image_extension can be png, bmp, or jpg and indicates the type of image to transfer it as
// image_extension does nothing on Windows
int SetClipboardBitmap(BITMAP* bmp , const char* image_extension);



// returns a new string if successful, else returns 0
char* GetNewClipboardString(void);



// GetNewClipboardBitmap returns a new bitmap if successful, else returns 0
// image_extension can be png, bmp, or jpg and indicates what kind of image to transfer it as
// image_extension does nothing on Windows
BITMAP* GetNewClipboardBitmap(const char* image_extension);




void LogAvailableClipboardTargets(void);



#ifdef __cplusplus
   }
#endif



#endif // Clipboard_H


