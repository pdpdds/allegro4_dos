


#include <allegro.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>



/// If font_to_use is not NULL, it will be used to print the characters on the font template as a guide.
/// Using a translucent font with this function is not recommended. If the font sizes are different, the 
/// characters from the font_to_use will be stretched to fit the new size.
BITMAP* CreateFontBitmap(int color_depth , int charwidth , int charheight , FONT* font_to_use , bool use_color_font);



int main(int argc , char** argv) {
   
   if (allegro_init() != 0) {return 0;}
   
   bool drawfont = true;
   char filename[256];
   char* extstart = 0;
   char extension[8] = {'.' , 'b' , 'm' , 'p' , '\0' , '\0' , '\0' , '\0'};
   bool use_default_filename = true;
   bool use_user_extension = false;
   for (int i = 0 ; i < 256 ; ++i) {filename[i] = '\0';}
   int fontw = 0;
   int fonth = 0;
   
   bool print_usage = false;
   
   if (argc < 2) {
      printf("No arguments given, check usage :\n");
      print_usage = true;
   }
   for (int i = 1 ; i < argc ; ++i) {
      char* arg = argv[i];
      char* subarg = 0;
      bool validarg = false;
      if (strncmp(arg , "--help" , 6) == 0) {
         print_usage = true;
         validarg = true;
      } else if (strncmp(arg , "blank" , 5) == 0) {
         drawfont = false;
         validarg = true;
      } else if (strncmp(arg , "w=" , 2) == 0) {
         subarg = arg + 2;
         fontw = atoi(subarg);
         if (fontw >= 4) {validarg = true;}
      } else if (strncmp(arg , "h=" , 2) == 0) {
         subarg = arg + 2;
         fonth = atoi(subarg);
         if (fonth >= 4) {validarg = true;}
      } else if (strncmp(arg , "file=" , 5) == 0) {
         subarg = arg + 5;
         strncpy(filename , subarg , 251);
         
         extstart = strchr(filename , '.');
         if (extstart == (char*)0) {
            // dot not found, add extension to end of filename
            strncat(filename , extension , 4);
            validarg = true;
            use_default_filename = false;
         } else if (extstart == filename) {
            // filename begins with a dot, use this as the extension
            strncpy(extension , filename , 4);
            validarg = true;
            use_user_extension = true;
         } else {
            extstart[4] = '\0';
            // dot in middle of filename, check for validity
            if  ((*(extstart - 1) == '\\') || (*(extstart - 1) == '/')) {
               printf("File name base must not end with a slash.\n");
            } else if ((strncmp(extstart , ".bmp" , 4) != 0) &&
                       (strncmp(extstart , ".pcx" , 4) != 0) &&
                       (strncmp(extstart , ".tga" , 4) != 0)) {
               printf("Valid extensions are .bmp , .pcx , and .tga.\n");
            } else {
               validarg = true;
               use_default_filename = false;
            }
            
         }
      }
      if (!validarg) {
         printf("'%s' is not a valid argument.\n" , arg);
         print_usage = true;
      }
   }
   
   if (print_usage) {
      printf("FontMaker [--help] [blank] [w=<width>] [h=<height>] [file=<filename>|<ext>]\n");
      printf("   --help    : Print this usage information.\n");
      printf("   blank     : Do not print the default font onto the new font.\n");
      printf("               Default behaviour is to output using the default font.\n");
      printf("   w=<width> : Replace <width> with the desired width in pixels.\n");
      printf("   h=<height>: Replace <height> with the desired height in pixels.\n");
      printf("   file=<filename>|<ext>\n");
      printf("             : Specify the filename with or without an extension.\n");
      printf("               Default file extension is .bmp.\n");
      printf("               Or specify the extension only. Valid extensions are\n");
      printf("               .pcx , .bmp , and .tga.\n");
      printf("\n");
   } else {
      if (use_default_filename) {
         if (use_user_extension) {
            sprintf(filename , "fmfont%dx%d%s%s" , fontw , fonth , (drawfont?"default":"blank") , extension);
         } else {
            sprintf(filename , "fmfont%dx%d%s%s" , fontw , fonth , (drawfont?"default":"blank") , ".bmp");
         }
      }
      printf("Creating %s\n" , filename);
      BITMAP* newfontbmp = CreateFontBitmap(24 , fontw , fonth , (drawfont?font:0) , false);
      if (!newfontbmp) {
         printf("Failed to create font bitmap.\n");
      } else {
         if (save_bitmap(filename , newfontbmp , 0) != 0) {
            printf("Failed to properly save the font bitmap.\n");
         } else {
            printf("Success!\n");
         }
         
      }
   }
   return 0;
}
END_OF_MAIN()




BITMAP* CreateFontBitmap(int color_depth , int charwidth , int charheight , FONT* font_to_use , bool use_color_font) {
   ASSERT((charwidth > 0) && (charheight > 0));
   // 16 X 6 grid with 17 X 7 spaces
   int hspace = charwidth/2;
   if (hspace < 2) {hspace = 2;}
   int vspace = charheight/2;
   if (vspace < 2) {vspace = 2;}
   int bw = 16*charwidth + 17*hspace;
   int bh = 6*charheight + 7*vspace;
   BITMAP* bmp = create_bitmap_ex(color_depth , bw , bh);
   if (!bmp) {return NULL;}
   BITMAP* letterbmp = 0;
   int lw = 0;
   int lh = 0;
   if (font_to_use) {
      lw = text_length(font_to_use , " ");
      lh = text_height(font_to_use);
      letterbmp = create_bitmap_ex(color_depth , lw , lh);
   }
   char letter[2] = {'\0' , '\0'};
   char c = 32;// start with space
   int transcol;
   int bgcol;
   int textcol = makecol_depth(color_depth , 0 , 0 , 0);
   if (use_color_font) {textcol = -1;}
   if (color_depth == 8) {
      transcol = 0;
      bgcol = 255;
   } else {
      transcol = makecol_depth(color_depth , 255 , 0 , 255);
      bgcol = makecol_depth(color_depth , 255 , 255 , 0);
   }
   clear_to_color(bmp , bgcol);
   int cy = vspace;
   for (int y = 0 ; y < 6 ; ++y) {
      int cx = hspace;
      for (int x = 0 ; x < 16 ; ++x) {
         letter[0] = c;
         rectfill(bmp , cx , cy , cx + charwidth - 1 , cy + charheight - 1 , transcol);
         if (letterbmp) {
            clear_to_color(letterbmp , transcol);
            textout_ex(letterbmp , font_to_use , letter , 0 , 0 , textcol , -1);
            stretch_blit(letterbmp , bmp , 0 , 0 , lw , lh , cx , cy , charwidth , charheight);
         }
         ++c;
         cx += charwidth + hspace;
      }
      cy += charheight + vspace;
   }
   return bmp;
}

