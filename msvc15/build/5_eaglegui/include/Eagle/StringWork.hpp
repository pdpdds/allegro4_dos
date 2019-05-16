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


#ifndef StringWork_H
#define StringWork_H


#include <string>
#include <list>
#include <iostream>
#include <cstdio>

std::string StringPrintF(const char* format_str , ...);/// 1024 character limit!

std::string GetFileExtension(std::string& s);
std::list<std::string> TokenizeString(const std::string& str , char delimiter);
std::string RemoveTrailingSlash(std::string str);
std::string FixFilenameSlashes(const std::string& str);
std::string GetShortFileName(const std::string& s);
std::string StripFileName(const std::string& s);


int MatchingParenthesisIndex(const char* cstr);

int ReverseStringSearch(const char* str , int start , char c);// returns offset from start or 1 if not found

int StringSearch(const char* str , int start , char c);// returns offset from start or -1 if not found
int StringSearch(const char* str , int start , const char* find);// returns offset from start or -1 if not found

std::string FormatDouble(double d);
std::string FormatDoubleString(std::string str);





#endif // StringWork_H


