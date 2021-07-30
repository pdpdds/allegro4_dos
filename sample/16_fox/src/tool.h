#include "LIST.H"

#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h> // isspace()
#include <string.h> // strcpy_s() strlen()
#include <stdlib.h>       // calloc
#include <stdarg.h>       // va_*
#include <string.h>       // strlen, strcpy

int keyreleased(int k);

int value_in_range(int value, int min, int max);
int check_collision_bbox(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);

// ---------------------------------------------------
int load_tile_bitmaps(BITMAP *bitmaps[], BITMAP *b, int count_col, int count_row, float scale);
int load_tile_compiled_sprites(COMPILED_SPRITE *sprites[], BITMAP *b, int count_col, int count_row, float scale);

BITMAP *resize_bitmap(BITMAP *b, float scale);
COMPILED_SPRITE *resize_compiled_sprite(BITMAP *b, float scale);

BITMAP *resize_vertical_bitmap(BITMAP *b, float scale);
COMPILED_SPRITE *resize_vertical_compiled_sprite(BITMAP *b, float scale);

BITMAP *resize_horizontal_bitmap(BITMAP *b, float scale);
COMPILED_SPRITE *resize_horizontal_compiled_sprite(BITMAP *b, float scale);

BITMAP *copy_sub_bitmap(BITMAP *b, int x, int y, int w, int h, float scale);
COMPILED_SPRITE *copy_sub_compiled_sprite(BITMAP *b, int x, int y, int w, int h, float scale);

BITMAP *copy_bitmap(BITMAP *b);
COMPILED_SPRITE *copy_compiled_sprite(BITMAP *b);

int show_bitmap(BITMAP *b, PALETTE pal);

void replace_colour(BITMAP *b, int colour, int newcolour);
void replace_all_colour(BITMAP *b, int newcolour);

long get_time_ms(void);

void write_str(char *msg);
void write_int(int i);
void write_float(float i);
void write_long(long i);

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

int trim_ch(char c);
int trimmed(const char* src, char** dst) ;

int string_to_list (const char *str, const char *tok, List *list);
int string_to_int_list (const char *str, const char *tok, List *list);

char** split(char* sentence, char separator, int* num_tokens);

char* concat(int count, ...);

void err(char *errmsg);

void show_available_memory(BITMAP *b, int x, int y);
int get_available_memory(unsigned long *phymem, unsigned long *virtmem);

void fade_in_new(BITMAP *bmp_orig, int speed);
void fade_out_new(int speed);

void capture_screen(BITMAP *bmp);

int is_number (char *str);
int rand_number(int min, int max);

void *copy_to_int(void *e1);

void read_text_file(char *filename, char **buf);
