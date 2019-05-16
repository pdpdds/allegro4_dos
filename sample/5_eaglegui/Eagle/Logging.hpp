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


#ifndef Logging_H
#define Logging_H



#include <string>
#include <iostream>
#include <fstream>




std::ostream& OutputLog();/// Returns a single global output destination for logging

/// These alter the destination of the output log returned by OutputLog()
bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);
void SendOutputTo(std::ostream& output_stream);



class Indenter {
private :
   unsigned int indent_level;
   unsigned int num_spaces;
   std::string indent;

   void ResetSpaces();
   
public :
   Indenter();
   Indenter(int level , int spaces);

   void SetLevel(unsigned int indentation_level);
   void SetSpaces(unsigned int number_of_spaces);

   
   /// Prefix increment and decrement operators for changing the indentation level.
   Indenter& operator++();
   Indenter& operator--();
   /// Postfix
   Indenter& operator++(int);
   Indenter& operator--(int);

   friend std::ostream& operator<<(std::ostream& os , const Indenter& i);
};


/// Simple interface for output logging

/** TODO : FIXME?
   The interface wrapper for the underlying ostream* just won't work right. I think it has to do with
   manipulators, as it can't figure out what to do with operator<<(Logger& , std::endl) and such. Lame.
*/

/**
typedef enum LogLevel {
   LOG_CRITICAL = 0,
   LOG_ERROR    = 1,
   LOG_WARNING  = 2,
   LOG_INFO     = 3,
   LOG_DEBUG    = 4,
   LOG_RESTORE  = 5 // To restore the previous logging level, not an actual logging level itself.
};


class Logger {
private :
   std::ofstream logfile;
   std::ostream* os;
   
   LogLevel max;
   LogLevel current;
   LogLevel previous;
   bool enabled;
   
public :
   Logger() : logfile() , os(&std::cout) , max(LOG_WARNING) , current(LOG_WARNING) , previous(LOG_WARNING) , enabled(true) {}
   
   void SetMaxLoggingLevel(LogLevel m);
   void SetLoggingLevel(LogLevel ll);

   bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);


   
   inline void SendOutputTo(std::ostream& output_stream);
   inline std::ostream& OS();


   friend inline Logger& operator<<(Logger& logger , const LogLevel& ll);

   friend inline Logger& operator<<(Logger& logger , const char* array);

   template <class Data>
   friend inline Logger& operator<<(Logger& logger , const Data& d);

   friend inline Logger& operator<<(Logger& logger , __ostream_type& (*__pf)(__ostream_type&)) {
      *(logger.os) << pf;
      return *this;
   }
   friend inline Logger& operator<<(Logger& logger , __ios_type& (*__pf)(__ios_type&)) {
      *(logger.os) << pf;
      return *this;
   }
   friend inline Logger& operator<<(Logger& logger , ios_base& (*__pf)(ios_base&)) {
      *(logger.os) << pf;
      return *this;
   }

   operator std::ostream&() {return *os;}

};

/// #########     A single Logger instance to use for all logging      ##############
///   Set the output destination once at the beginning of the program and then just use 
///   OutputLog like an ostream. Default output is to std::cout.


extern Logger OutputLog;



/// #########      Logger class inline method definitions     ############

inline void Logger::SendOutputTo(std::ostream& output_stream) {
   os = &output_stream;
}

//inline std::ostream& Logger::OS() {return *os;}


template <class Data>
inline Logger& operator<<(Logger& logger , const Data& d) {
   if (logger.enabled) {*(logger.os) << d;}
   return logger;
}

template <>
inline Logger& operator<<(Logger& logger , const LogLevel& ll) {
   logger.SetLoggingLevel(ll);
   return logger;
}

//template <>
inline Logger& operator<<(Logger& logger , const char* array) {
      if (logger.enabled) {
         unsigned int i = 0;
         std::ostream& ostr = *(logger.os);
         while(array[i]) {
            ostr << array[i];
            ++i;
         }
      }
      return logger;
}

*/



#endif // Logging_H





