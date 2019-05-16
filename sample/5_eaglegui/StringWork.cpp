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


#include "Eagle/StringWork.hpp"
#include <allegro.h>

#include <cstdio>
#include <cstdarg>

using std::string;
using std::list;

#include <cstring>

string StringPrintF(const char* format_str , ...) {
   char buffer[1025];
   va_list args;
   va_start(args , format_str);
   vsprintf(buffer , format_str , args);
   va_end(args);
   return std::string(buffer);
}



string GetFileExtension(string& s) {
   string ext("");
   unsigned int index = s.find_last_of('.');
   if ((index != string::npos) && (index + 1 < s.size())) {
      ext = s.substr(index + 1);
   }
   return ext;
}



list<string> TokenizeString(const string& str , char delimiter) {
   list<string> tokens;
   string token;
   const char* s = str.c_str();
   unsigned int size = strlen(s);
   for (unsigned int i = 0 ; i < size ; ++i) {
      if (s[i] == delimiter) {
         tokens.push_back(token);
         token = "";
      } else {
         token.push_back(s[i]);
      }
   }
   tokens.push_back(token);
   return tokens;
}



string RemoveTrailingSlash(string str) {
   if (str.size() == 0) {return str;}
   char tail = str.at(str.size() - 1);
   if ((tail == '\\') || (tail == '/')) {
      str.erase(str.size() - 1 , 1);
   }
   return str;
}



string FixFilenameSlashes(const string& str) {
   char* copy = strdup(str.c_str());
   string s = fix_filename_slashes(copy);
   free(copy);
   return s;
}



string GetShortFileName(const string& s) {
   if (s.size() == 0) {return string("");}
   unsigned int index = s.find_last_of(OTHER_PATH_SEPARATOR);
   if (index == s.size() - 1) {return string("");}
   string short_name = s.substr(index + 1);
   return short_name;
}



std::string StripFileName(const std::string& s) {
   const char* str = s.c_str();
   int index = 0;
   for (index = strlen(str) ; index >= 0 ; --index) {
      if (str[index] == OTHER_PATH_SEPARATOR) {break;}
   }
   if (index != -1) {
      return s.substr(0 , index);
   }
   return std::string();
}




int MatchingParenthesisIndex(const char* cstr) {
   ASSERT(cstr);
   ASSERT(cstr[0] == '(');
   int parenthesis = 0;
   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      char c = cstr[i];
      if (c == '(') {
         ++parenthesis;
      } else if (c == ')') {
         --parenthesis;
         if (parenthesis == 0) {
            return i;
         }
      } else if (c == '\0') {
         return -1;
      }
   }
   return -1;
}



int ReverseStringSearch(const char* str , int start , char c) {
   int index = 0;
   for (int i = start ; i >= 0 ; --i , --index) {
      if (str[i] == c) {
         return index;
      }
   }
   return 1;
}



int StringSearch(const char* str , int start , char c) {
   int index = 0;
   for (int i = start ; i < (int)strlen(str) ; ++i , ++index) {
      if (str[i] == c) {
         return index;
      }
   }
   return -1;
}



int StringSearch(const char* str , int start , const char* find) {
   int index = 0;
   int length = strlen(find);
   for (int i = start ; i < (int)strlen(str) ; ++i , ++index) {
      if (strncmp(str + i , find , length) == 0) {
         return index;
      }
   }
   return -1;
}



string MakeDoubleString(bool is_negative , string numerals , int decimal_point , int exponent) {
   string output;
   char buffer[64];
   if (is_negative) {
      output += '-';
   }
   for (int i = 0 ; (i < decimal_point) && (i < (int)numerals.size()) ; ++i) {
      output += numerals[i];
   }
   if (decimal_point < (int)numerals.size()) {
      output += '.';
   }
   for (int i = decimal_point ; i < (int)numerals.size() ; ++i) {
      output += numerals[i];
   }
   if (exponent) {
      output += 'e';
      sprintf(buffer , "%i" , exponent);
      output += buffer;
   }
   return output;
//   printf("%s\n" , output.c_str());
}


std::string FormatDouble(double d) {
   return FormatDoubleString(StringPrintF("%.30lg" , d));
}



std::string FormatDoubleString(string str) {
   const char* cstr = str.c_str();
   string numerals;
   int exponent = 0;
   int decimal_point = -1;
   // 12345.67890000000e-5
   // 1.2345e10
   int i = 0;
   bool is_negative = false;
   bool exp_is_negative = false;

///   printf("Before formatting double, string is '%s'\n" , cstr);

   if (cstr[i] == '-') {
      is_negative = true;
      ++i;
   }

   for (; i < (int)strlen(cstr) ; ++i) {
      char c = cstr[i];
      if (isdigit(c)) {
         numerals.push_back(c);
      } else if (c == '.') {
         decimal_point = i;
      } else if ((c == 'e') || (c == 'E')) {
         ++i;
         if (cstr[i] == '-') {
            exp_is_negative = true;
            ++i;
         }
         if (cstr[i] == '+') {
            ++i;
         }
         while (cstr[i] == '0') {++i;}
         if (1 != sscanf(cstr + i , "%i" , &exponent)) {
            /// Hmm? No number following the 'e'.
            exponent = 0;
         }
         if (exp_is_negative) {exponent *= -1;}
         break;
      }
   }
   // 12345e2 12345. e2
   if (decimal_point == -1) {decimal_point = (int)strlen(numerals.c_str());}
   
///   printf("After reading string, formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());
   
   // strip trailing zeros
   // 12300.00 -> 12300
   int index = -1;
   cstr = numerals.c_str();
   for (i = (int)strlen(cstr) - 1 ; i >= 0 ; --i) {
      if (cstr[i] == '0') {
         if (i < decimal_point) {
            --decimal_point;
            ++exponent;
         }
         index = i;
      } else {
         break;
      }
   }
   if (index != -1) {
      numerals.erase(index);
      cstr = numerals.c_str();
   }
   
///   printf("After stripping trailing zeros, formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());
   
   // strip leading zeros 0.000123 -> 0000123 1 , 0 -> 000123 1 -1 -> 00123 1 -2 -> 0123 1 -3 -> 123 1 -4
   int zerocount = 0;
   for (i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '0') {
         ++zerocount;
         --exponent;
      } else {
         break;
      }
   }
   if (zerocount) {
      numerals.erase(0 , zerocount);
      cstr = numerals.c_str();
   }
   
///   printf("After stripping leading zeros , formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());
   
   
   // 0.00000123 - 000000123
   
   // 123 2 -5 -> 123 1 -4
   // shift decimal point so that it is at index 1 - One numeral followed by decimal point followed by rest of numerals and then exponent
   if (decimal_point != -1) {
      if (decimal_point > 1) {
         exponent += decimal_point - 1;
         decimal_point = 1;
      }
      // 1.23 e3 1230 1.2345 e4 12345 1.23 e-5
      // if exponent is > 0 and < 3 then shift decimal point
      if ((exponent > 0) && (exponent < 3)) {
         decimal_point += exponent;
         exponent = 0;
         
         // add trailing zeros if necessary
         // 123. 3 0 -> 123 , 12 4 0 -> 1200
         int length = (int)strlen(numerals.c_str());
         if (decimal_point > length) {
            zerocount = decimal_point - length;
            numerals.insert(numerals.end() , zerocount , '0');
            cstr = numerals.c_str();
         }
      }
      // 1.23e-2 0.0123 , 1.23 e-3 0.00123
      if ((exponent > -3) && (exponent  < 0)) {
         zerocount = -exponent;
         exponent = 0;
         // add leading zeros if necessary
         numerals.insert(numerals.begin() , zerocount , '0');
         cstr = numerals.c_str();
      }
   }
   
   // render string
   string output = MakeDoubleString(is_negative , numerals , decimal_point , exponent);
   
///   printf("After formatting double, string is : '%s'\n" , output.c_str());
   
   return output;
}






