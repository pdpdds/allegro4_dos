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


#ifndef Error_H
#define Error_H

#include <string>
#include <allegro.h>

#include "Eagle/Logging.hpp"



class EagleError : public std::exception {
protected :
   std::string error;
   
public :
   EagleError(std::string e) : error(e) {
      OutputLog() << e << std::endl;
#ifdef EAGLE_ASSERT_EXCEPTION
      ASSERT(0);
#endif
   }
   EagleError(const char* e) : error(e) {
      OutputLog() << e << std::endl;
#ifdef EAGLE_ASSERT_EXCEPTION
      ASSERT(0);
#endif
   }
   ~EagleError() throw () {}
   
   const char* what() const throw() {return error.c_str();}
};



class NotFinishedError : public EagleError {
   static const std::string nfestr;
public :
   NotFinishedError(std::string e) :
      EagleError(nfestr)
   {
      OutputLog() << e << std::endl;
   }
   NotFinishedError(const char* e) :
      EagleError(nfestr)
   {
      OutputLog() << e << std::endl;
   }
};

#endif // Error_H

