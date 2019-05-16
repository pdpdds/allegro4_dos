

#include "Clipboard.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE* logfile = 0;


#ifdef ALLEGRO_WINDOWS
#include <winalleg.h>

/** Vanneto's implementation
#include <windows.h>
#include <string>
#include <stdio.h>

void clipboard_paste (const std::string &text)
{
    if (OpenClipboard(NULL)) {
        HGLOBAL clipBuffer;
        char *buffer;

        EmptyClipboard();
        clipBuffer = GlobalAlloc (GMEM_DDESHARE, text.size() + 1);
        buffer = (char*)GlobalLock(clipBuffer);

        strcpy(buffer, text.c_str());
        GlobalUnlock (clipBuffer);
        SetClipboardData(CF_TEXT, clipBuffer);
        CloseClipboard();
    }
}

std::string clipboard_copy ()
{
    std::string fromClipboard;
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char *buffer = (char*)GlobalLock(hData);
        fromClipboard = buffer;
        GlobalUnlock (hData);
        CloseClipboard ();
    }

    return fromClipboard;
}

int main (int argc, char *argv[])
{
    clipboard_paste ("foobar");
    printf("%s\n", clipboard_copy().c_str());
    return 0;
}

*/

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
   fprintf(Log() , "The clipboard library does not support getting available clipboard targets on Windows yet.\n");
}



#endif // #ifdef ALLEGRO_WINDOWS



#ifdef ALLEGRO_UNIX

#include <stdio.h>
#include <stdlib.h>


/** Get/Set..ClipboardString using xclip

int SetClipboardString(const char* cstr) {
   const char* tempfilename = "temp_clipboard.txt";
   const char* command = "cat %s | xclip -selection c"
   char* command_buffer = 0;
   FILE* tempfile = 0;
   
   if (!cstr) {return 1;}
   tempfile = fopen(tempfilename , "w");
   if (!tempfile) {return 1;}
   fwrite(cstr , sizeof(char) , strlen(cstr) , tempfile);
   fflush(tempfile);
   if (fclose(tempfile) != 0) {return 1;}
   
   command_buffer = (char*)malloc(strlen(tempfilename) + strlen(command) - 2 + 1);
   if (!command_buffer) {return 1;}
   sprintf(command_buffer , command , tempfilename);
   system(command_buffer)
   free(command_buffer);
   return 0;
}



int SetClipboardBitmap(BITMAP* bmp) {
   return 1;
}



char* GetNewClipboardString() {
   FILE* tempfile = 0;
   char* newstr = 0;
   int size = 0;
   int i = 0;
   
   system("xclip -o -selection c > temp_clipboard.txt");
   tempfile = fopen("temp_clipboard.txt" , "r");
   if (!tempfile) {return (char*)0;}
   while(fgetc(tempfile) != EOF) {++size;}
   if (fseek(tempfile , 0 , SEEK_SET) != 0) {return (char*)0;}
   newstr = (char*)malloc(size + 1);
   if (!newstr) {return (char*)0;
   while (i < size) {
      newstr[i] = fgetc(tempfile);
      ++i;
   }
   newstr[size] = '\0';
   fclose(tempfile);
   return newstr;
}



BITMAP* GetNewClipboardBitmap() {
   return (BITMAP*)0;
}
//*/



/** Get/Set text using pipes and xclip

//*/


#include <xalleg.h>
//#include <X11/Xatom.h>


int IsClipboardStringAvailable() {
   const char* command = "./xcsi -c -q=text";
   char buf[128];
   
   memset((void*)buf , '\0' , 128);
   
   FILE* readpipe = popen(command , "r");
   if (!readpipe) {return 0;}
   
   fgets(buf , 128 , readpipe);
   pclose(readpipe);
   
   if (strcmp(buf , "available") == 0) {return 1;}
   return 0;
}



int IsClipboardBitmapAvailable(const char* image_type) {
   const char* basecommand = "./xcsi -c -q=%s";
   char command[256];
   char buf[128];
   const char* ext = 0;
   
   if (strncmp(image_type , "bmp" , 3) == 0) {
      ext = "bmp";
   } else if (strncmp(image_type , "png" , 3) == 0) {
      ext = "png";
   } else if (strncmp(image_type , "jpg" , 3) == 0) {
      ext = "jpg";
   } else {
      return 0;
   }
   
   sprintf(command , basecommand , ext);
   
   memset((void*)buf , '\0' , 128);
   memset((void*)command , '\0' , 256);

   FILE* readpipe = popen(command , "r");
   if (!readpipe) {return 0;}
   
   fgets(buf , 128 , readpipe);
   pclose(readpipe);
   
   if (strcmp(buf , "available") == 0) {return 1;}
   return 0;
}



int SetClipboardString(const char* cstr) {
   FILE* wpipe = 0;
   const char* command = "./xcsi -c -s=text";
   int retval = 0;
   
   if (!cstr) {return 1;}
   
   wpipe = popen(command , "w");
   if (!wpipe) {return 1;}
   
//int fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
   fwrite((void*)cstr , sizeof(char) , strlen(cstr) , wpipe);
   fflush(wpipe);
   
   retval = pclose(wpipe);
   
   return retval;
}// returns 0 if successful, non-zero otherwise



int SetClipboardBitmap(BITMAP* bmp , const char* image_extension) {
   char command[256];
   const char* ext = 0;
   const char* tempfile = "xcsi_image_temp";
   char tempfilename[32];
   
   if (!bmp) {return 1;}
   
   if (strncmp(image_extension , "png" , 3) == 0) {ext = "png";}
   else if (strncmp(image_extension , "bmp" , 3) == 0) {ext = "bmp";}
   else if (strncmp(image_extension , "jpg" , 3) == 0) {ext = "jpg";}
   else {return 1;}
   
   memset((void*)command , '\0' , 256);
   memset((void*)tempfilename , '\0' , 32);
   
   sprintf(tempfilename , "%s.%s" , tempfile , ext);
   
   if (save_bitmap(tempfilename , bmp , NULL) != 0) {return 1;}
//xcsi -c -s=bmp < xcsi_image_temp.bmp   
   sprintf(command , "./xcsi -c -s=%s %s" , ext , tempfilename);
//   sprintf(command , "cat %s | xcsi -c -s=%s" , tempfilename , ext);
   if (system(command) != 0) {return 1;}
   
   remove(tempfilename);
   
   return 0;
}// returns 0 if successful, non-zero otherwise


char* GetNewClipboardString() {
   FILE* rpipe = 0;
   const char* command = "./xcsi -c -g=text";
   char* str = 0;
   int str_size = 0;
   int buffer_size = 0;
   char c;
   
   rpipe = popen(command , "r");
   if (!rpipe) {
      fprintf(Log() , "GetNewClipboardString failed, could not open pipe for reading.\n");
      return (char*)0;
   }
   
   buffer_size = 256*sizeof(char);
   str = (char*)malloc(buffer_size);
   
   while((c = fgetc(rpipe)) != EOF) {
      str[str_size] = c;
      ++str_size;
      if (str_size == buffer_size) {
         buffer_size *= 2;
         str = (char*)realloc(str , buffer_size);
         if (!str) {
            pclose(rpipe);
            return (char*)0;
         }
      }
   }
   pclose(rpipe);
   str = (char*)realloc(str , str_size + sizeof(char));
   if (!str) {
      return (char*)0;
   }
   str[str_size] = '\0';
   if (strlen(str) == 0) {
      free(str);
      fprintf(Log() , "GetNewClipboardString returned a zero length string.\n");
      return (char*)0;
   }
   return str;
}// returns a new string if successful, else returns 0



BITMAP* GetNewClipboardBitmap(const char* image_extension) {
   char command[256];
   const char* ext = 0;
   const char* tempfile = "xcsi_image_temp";
   char tempfilename[32];
   BITMAP* bmp = 0;
   
   if (strncmp(image_extension , "png" , 3) == 0) {ext = "png";}
   else if (strncmp(image_extension , "bmp" , 3) == 0) {ext = "bmp";}
   else if (strncmp(image_extension , "jpg" , 3) == 0) {ext = "jpg";}
   else {return (BITMAP*)0;}
   
   memset((void*)command , '\0' , 256);
   memset((void*)tempfilename , '\0' , 32);
   
   sprintf(tempfilename , "%s.%s" , tempfile , ext);
   
   sprintf(command , "./xcsi -c -g=%s %s" , ext , tempfilename);
   
   if (system(command) != 0) {/*return (BITMAP*)0;*/}
   
   bmp = load_bitmap(tempfilename , NULL);
   
   remove(tempfilename);
   
   return bmp;
}// returns a new bitmap if successful, else returns 0



void LogAvailableClipboardTargets(void) {
   system("./xcsi -c -q=all");
   system("./xcsi -p -q=all");
}


#endif // #ifdef ALLEGRO_UNIX

#ifdef ALLEGRO_MACOSX
//   #include <MACOSX_HEADER> // TODO : Find out what mac header to include



int IsClipboardStringAvailable() {
   /// TODO : Implement
   return 0;
}



int IsClipboardBitmapAvailable(const char* image_type) {
   /// TODO : Implement
   return 0;
}



int SetClipboardString(const char* cstr) {
   /// TODO : Implement
   return 1;
}



int SetClipboardBitmap(BITMAP* bmp) {
   /// TODO : Implement
   return 1;
}



char* GetNewClipboardString() {
   /// TODO : Implement
   return (const char*)0;
}



BITMAP* GetNewClipboardBitmap() {
   /// TODO : Implement
   return (BITMAP*)0;
}



void LogAvailableClipboardTargets(void) {
   /// TODO : Implement
   fprintf(Log() , "The clipboard library does not support getting available clipboard targets on MacOSX yet.\n");
}



#endif // #ifdef ALLEGRO_MACOSX



FILE* Log(void) {
   if (logfile) {return logfile;}
   logfile = fopen("ClipboardLog.txt" , "w");
   if (logfile) {
      atexit(CloseLog);
   }
   return logfile;
}



void CloseLog(void) {
   if (logfile) {
      fflush(logfile);
      fclose(logfile);
      logfile = 0;
   }
}


