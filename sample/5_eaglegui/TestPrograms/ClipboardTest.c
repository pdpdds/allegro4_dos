


#include <allegro.h>

#include "Clipboard.h"



int main(int argc , char** argv) {
   int cd;
   int dw = 800;
   int dh = 600;
   BITMAP* cyberman = 0;
   BITMAP* buffer = 0;
   BITMAP* fromclipbmp = 0;
   char* fromclipstr = 0;
   int redraw = 1;
   int black = 0;
   int white = 0;
   int orange = 0;
   int ret = 0;
   
   if (allegro_init() != 0) {
      printf("Allegro init failed.\n");
      return 1;
   }
   if (install_keyboard() != 0) {return 0;}
   cd = desktop_color_depth();
   if (cd == 0) {cd = 32;}
   set_color_depth(cd);
   
   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED , dw , dh , 0 , 0) != 0) {
      printf("set_gfx_mode failed.\n");
      return 1;
   }
   
   if (!exists("CyberMan_80X80.bmp")) {
      printf("CyberMan_80X80.bmp does not exist. Ending program.\n");
      return 1;
   }
   
   buffer = create_bitmap(dw,dh);
   cyberman = load_bitmap("CyberMan_80X80.bmp" , NULL);
   
   if (!buffer || !cyberman) {
      if (!buffer) {
         printf("failed to allocate buffer.\n");
      }
      if (!cyberman) {
         printf("failed to load cyberman.\n");
      }
      if (buffer) {destroy_bitmap(buffer);}
      if (cyberman) {destroy_bitmap(cyberman);}
      return 1;
   }
   
   
   white = makecol(255,255,255);
   black = makecol(0,0,0);
   orange = makecol(255,127,0);
   
#ifdef ALLEGRO_UNIX
   if (!exists("xcsi")) {
      printf("xcsi does not exist. Ending program.\n");
      fprintf(Log() , "xcsi does not exist. Ending program.\n");
      if (buffer) {destroy_bitmap(buffer);}
      if (cyberman) {destroy_bitmap(cyberman);}
      return 1;
   }
#endif   
   while (!key[KEY_ESC]) {
      if (redraw) {
         clear(buffer);
         textout_ex(buffer , font , "Press C to copy the CyberMan image into the Clipboard" , dw/10 , dh/2 - 28 , white , -1);
         textout_ex(buffer , font , "Press S to copy the string \"CyberMan\" into the Clipboard" , dw/10 , dh/2 - 12 , white , -1);
         textout_ex(buffer , font , "Press V to view the image in the clipboard" , dw/10 , dh/2 + 4 , white , -1);
         textout_ex(buffer , font , "Press B to view the string in the clipboard" , dw/10 , dh/2 + 20 , white , -1);
         textout_ex(buffer , font , "Press L to log the available clipboard targets" , dw/10 , dh/2 + 36 , white , -1);
         textout_ex(buffer , font , "To return to the menu, press any key to continue" , dw/10 , dh/2 + 52 , white , -1);
      }
      
      if (key[KEY_C]) {
         ret = SetClipboardBitmap(cyberman , "bmp");
         clear(screen);
         if (ret == 0) {
            textout_ex(screen , font , "Successfully set the clipboard bitmap to the CyberMan image." , dw/10 , dh/2 - 4 , white , -1);
         } else {
            textout_ex(screen , font , "Could not set the clipboard bitmap to the CyberMan image." , dw/10 , dh/2 - 4 , orange , -1);
         }
         clear_keybuf();
         readkey();
         rest(100);
         redraw = 1;
      }
      if (key[KEY_S]) {
         ret = SetClipboardString("CyberMan");
         clear(screen);
         if (ret == 0) {
            textout_ex(screen , font , "Successfully set the clipboard string to \"CyberMan\"" , dw/10 , dh/2 - 4 , white , -1);
         } else {
            textout_ex(screen , font , "Could not set the clipboard string to \"CyberMan\"" , dw/10 , dh/2 - 4 , orange , -1);
         }
         clear_keybuf();
         readkey();
         rest(100);
         redraw = 1;
      }
      if (key[KEY_V]) {
         if (fromclipbmp) {
            destroy_bitmap(fromclipbmp);
            fromclipbmp = 0;
         }
         fromclipbmp = GetNewClipboardBitmap("bmp");
         clear(screen);
         if (fromclipbmp) {
            blit(fromclipbmp , screen , 0 , 0 , (SCREEN_W - fromclipbmp->w)/2 , (SCREEN_H - fromclipbmp->h)/2 , fromclipbmp->w , fromclipbmp->h);
         } else {
            textout_ex(screen , font , "There is no image in the clipboard to view." , dw/10 , dh/2 - 4 , orange , -1);
         }
         clear_keybuf();
         readkey();
         rest(100);
         redraw = 1;
      }
      if (key[KEY_B]) {
         if (fromclipstr) {
            free(fromclipstr);
            fromclipstr = 0;
         }
         fromclipstr = GetNewClipboardString();
         clear(screen);
         if (fromclipstr) {
            textout_ex(screen , font , fromclipstr , dw/10 , dh/2 - 4 , white , -1);
         } else {
            textout_ex(screen , font , "There is no string in the clipboard to view." , dw/10 , dh/2 - 4 , orange , -1);
         }
         clear_keybuf();
         readkey();
         rest(100);
         redraw = 1;
      }
      if (key[KEY_L]) {
         LogAvailableClipboardTargets();
         clear(screen);
         textout_ex(screen , font , "Logging available clipboard targets" , dw/10 , dh/2 - 4 , white , -1);
         clear_keybuf();
         readkey();
         rest(100);
         redraw = 1;
      }
      
      if (redraw) {
         blit(buffer , screen , 0 , 0 , 0 , 0 , buffer->w , buffer->h);
         redraw = 0;
      }
      rest(16);
   }
   
   if (fromclipbmp) {destroy_bitmap(fromclipbmp);}
   destroy_bitmap(buffer);
   destroy_bitmap(cyberman);
   if (fromclipstr) {free(fromclipstr);}
   
   return 0;
}
END_OF_MAIN()
