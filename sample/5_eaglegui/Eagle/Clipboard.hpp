

#ifndef ClipBoard_H
#define ClipBoard_H


#include <string>

#include <allegro.h>

bool ClipboardStringAvailable();
bool ClipboardImageAvailable(const char* ext);

void SetClipboardString(const std::string& str);

std::string GetClipboardString();

void SetClipboardImage(BITMAP* bmp);

BITMAP* GetNewClipboardImage();


#endif // #ifndef ClipBoard_H
