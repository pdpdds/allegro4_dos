

#include "Eagle/FileWork.hpp"

#include <stdlib.h>

using std::string;

#define ASSERT(x)



std::istream& GetLinePN(std::istream& is , std::string& s) {
   s = "";
   char c;
   if (!is.good()) {return is;}
   while (!((is.get(c)).eof())) {
      if (c == '\r') {// WINDOWS OR OSX
         int c2 = is.peek();
         if (c2 == '\n') {
            is.get(c);// use up \n
         }
         return is;
      }
      if (c == '\n') {// NIX
         return is;
      }
      s += c;
   }
   return is;
}



FILE* GetLinePN(FILE* f , std::string& s) {
   s = "";
   if (!f || feof(f) || ferror(f)) {return f;}
   int c;
   while ((c = fgetc(f)) != EOF) {
      if (c == '\r') {// WINDOWS OR OSX
         int c2 = fpeek(f);
         if (c2 == '\n') {
            c = fgetc(f);// use up \n
         }
         return f;
      }
      if (c == '\n') {// NIX
         return f;
      }
      s += c;
   }
   return f;
}



void SkipWhiteSpace(FILE* f) {
   int c = 0;
   while (1) {
      c = fgetc(f);
      if (!((c == ' ') ||
          (c == '\t') ||
          (c == '\n') ||
          (c == '\f') ||
          (c == '\r'))) {
         // not whitespace
         if (c != EOF) {
            ungetc(c , f);
         }
         break;
      }
   }
}


bool ReadTextInteger(FILE* f , int* store) {
   ASSERT(store);

   string s;
   int i = 0;
   bool isnumber = false;

   while (1) {
      int c = fgetc(f);
      if (isdigit(c)) {
         isnumber = true;
         s += (char)c;
      } else {
         if (c != EOF) {
            ungetc(c , f);
         }
         break;
      }
   }

   if (isnumber) {
      i = atoi(s.c_str());
      *store = i;
      return true;
   }

   return false;
}


int fpeek(FILE* f) {
   if (!f || feof(f) || ferror(f)) {return EOF;}
   long pos = ftell(f);
   int c = fgetc(f);
   fseek(f , pos , SEEK_SET);
   ASSERT(pos == ftell(f));
   return c;
}

