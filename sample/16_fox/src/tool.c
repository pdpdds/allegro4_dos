#include "TOOL.H"
#include "game.h"

#ifdef _WIN32

struct timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
};

struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

#include "util.h"

#endif


int keyreleased(int k)
{
    static int initialized = 0;
    static int keyp[KEY_MAX];

    if(!initialized)
    {
        /* Set the keyp (key pressed) flags to false */
        int i;
        for(i = 0; i < KEY_MAX; i++) keyp[i] = 0;
        initialized = 1;
    }

    /* Now for the checking
       Check if the key was pressed
    */
    if(key[k] && !keyp[k])
    {
        /* Set the flag and return */
        keyp[k] = 1;
        return 0;
    }
    else if(!key[k] && keyp[k])
    {
        /* The key was released */
        keyp[k] = 0;
        return 1;
    }
    /* Nothing happened? */
    return 0;
}

int value_in_range(int value, int min, int max) 
{
	if ( (value >= min) && (value <= max) ) 
		return 1;
	return 0;
}

int check_collision_bbox(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    int x_overlap = value_in_range(ax, bx, bx + bw) ||
                    value_in_range(bx, ax, ax + aw);

    int y_overlap = value_in_range(ay, by, by + bh) ||
                    value_in_range(by, ay, ay + ah);

    if ( x_overlap && y_overlap ) return 1;
	return 0;
}

// ---------------------------------------------------
int load_tile_bitmaps(BITMAP *bitmaps[], BITMAP *b, int count_col, int count_row, float scale)
{
	int x, y;
	int i=0;

	int w = b->w/count_col;
	int h = b->h/count_row;

	for(y=0; y<count_row; y++) {
		for(x=0; x<count_col; x++) {
			int xx = x*w;
			int yy = y*h;

			BITMAP *b2 = create_sub_bitmap(b, xx, yy, w, h);
			bitmaps[i] = resize_bitmap(b2, scale);
			destroy_bitmap(b2); 

			i++;
		}
	}

	return count_col * count_row;
}

int load_tile_compiled_sprites(COMPILED_SPRITE *sprites[], BITMAP *b, 
		int count_col, int count_row, float scale)
{
	int x, y;
	int i=0;

	int w = b->w/count_col;
	int h = b->h/count_row;

	for(y=0; y<count_row; y++) {
		for(x=0; x<count_col; x++) {
			int xx = x*w;
			int yy = y*h;

			BITMAP *b2 = create_sub_bitmap(b, xx, yy, w, h);
			BITMAP *b3 = resize_bitmap(b2, scale);
			sprites[i] = get_compiled_sprite(b3, FALSE);
			destroy_bitmap(b3); 
			destroy_bitmap(b2); 

			i++;
		}
	}

	return count_col * count_row;
}

// ---------------------------------------------------
BITMAP *resize_bitmap(BITMAP *b, float scale)
{
	int new_w = (int)((float)b->w * scale);
	int new_h = (int)((float)b->h * scale);

	if ( new_w <= 0 ) new_w = 1;
	if ( new_h <= 0 ) new_h = 1;

	BITMAP *out = create_bitmap(new_w, new_h);
	clear(out);
	stretch_blit(b, out, 0, 0, b->w, b->h, 0, 0, new_w, new_h);

	return out;
}

COMPILED_SPRITE *resize_compiled_sprite(BITMAP *b, float scale)
{
	BITMAP *out = resize_bitmap(b, scale);
	COMPILED_SPRITE *out2 = get_compiled_sprite(out, FALSE);
	destroy_bitmap(out); 
	return out2;
}

// ---------------------------------------------------
BITMAP *resize_vertical_bitmap(BITMAP *b, float scale)
{
	int new_h = (int)((float)b->h * scale);
	if ( new_h <= 0 ) new_h = 1;

	BITMAP *out = create_bitmap(b->w, new_h);
	clear(out);
	stretch_blit(b, out, 0, 0, b->w, b->h, 0, 0, b->w, new_h);

	return out;
}

COMPILED_SPRITE *resize_vertical_compiled_sprite(BITMAP *b, float scale)
{
	BITMAP *out = resize_vertical_bitmap(b, scale);
	COMPILED_SPRITE *out2 = get_compiled_sprite(out, FALSE);
	destroy_bitmap(out);
	return out2;
}

// ---------------------------------------------------
BITMAP *resize_horizontal_bitmap(BITMAP *b, float scale)
{
	int new_w = (int)((float)b->w * scale);
	if ( new_w <= 0 ) new_w = 1;

	BITMAP *out = create_bitmap(new_w, b->h);
	clear(out);
	stretch_blit(b, out, 0, 0, b->w, b->h, 0, 0, new_w, b->h);

	return out;
}

COMPILED_SPRITE *resize_horizontal_compiled_sprite(BITMAP *b, float scale)
{
	BITMAP *out = resize_horizontal_bitmap(b, scale);
	COMPILED_SPRITE *out2 = get_compiled_sprite(out, TRUE);
	destroy_bitmap(out);
	return out2;
}

// ---------------------------------------------------
BITMAP *copy_sub_bitmap(BITMAP *b, int x, int y, int w, int h, float scale)
{
	int new_w = (int)((float)w * scale);
	int new_h = (int)((float)h * scale);
	
	if ( new_w <= 0 ) new_w = 1;
	if ( new_h <= 0 ) new_h = 1;

	BITMAP *out = create_bitmap(new_w, new_h);
	clear(out);
	stretch_blit(b, out, x, y, w, h, 0, 0, new_w, new_h);
	return out;
}

COMPILED_SPRITE *copy_sub_compiled_sprite(BITMAP *b, int x, int y, int w, int h, float scale)
{
	BITMAP *out = copy_sub_bitmap(b, x, y, w, h, scale);
	COMPILED_SPRITE *out2 = get_compiled_sprite(out, FALSE);
	destroy_bitmap(out);
	return out2;
}

// ---------------------------------------------------
BITMAP *copy_bitmap(BITMAP *b)
{
	BITMAP *out = create_bitmap(b->w, b->h);
    clear(out);
	stretch_blit(b, out, 0, 0, b->w, b->h, 0, 0, b->w, b->h);
	return out;
}

COMPILED_SPRITE *copy_compiled_sprite(BITMAP *b)
{
	BITMAP *out = copy_bitmap(b);
	COMPILED_SPRITE *out2 = get_compiled_sprite(out, FALSE);
	destroy_bitmap(out);
	return out2;
}

// ---------------------------------------------------
void replace_colour(BITMAP *b, int colour, int newcolour) 
{
	int x, y;

	for(y = 0; y < b->h; y++) {
		for(x = 0; x < b->w; x++) {
			if(getpixel(b, x, y) == colour) 
				putpixel(b, x, y, newcolour);
		}
	}
}

void replace_all_colour(BITMAP *b, int newcolour) 
{
	int x, y;

	for(y = 0; y < b->h; y++) {
		for(x = 0; x < b->w; x++) {
			if(getpixel(b, x, y) != makecol(255,0,255)) 
				putpixel(b, x, y, newcolour);
		}
	}
}

long get_time_ms(void) 
{
#if ALLEGRO_VERSION == 4
	struct timeval tv; 
    gettimeofday(&tv, NULL); // get current time
    long ms = tv.tv_sec*1000LL + tv.tv_usec/1000; // calculate milliseconds
    return ms;
#else
	return (long)(al_get_time() * 1000);
#endif
}

void write_str(char *msg)
{
	FILE *fp;
	fp = fopen("LOG.TXT", "a");
	fprintf(fp, "%s\n", msg);
	fclose(fp);
}

void write_int(int i)
{
	FILE *fp;
	fp = fopen("LOG.TXT", "a");
	fprintf(fp, "%d\n", i);
	fclose(fp);
}

void write_float(float i)
{
	FILE *fp;
	fp = fopen("LOG.TXT", "a");
	fprintf(fp, "%2.6f\n", i);
	fclose(fp);
}

void write_long(long i)
{
	FILE *fp;
	fp = fopen("LOG.TXT", "a");
	fprintf(fp, "%ld\n", i);
	fclose(fp);
}


char *ltrim(char *s)
{
    while(isspace((unsigned char)*s)) s++;
    return s;
}

char *rtrim(char *s)
{
	int length = strlen(s);
	if (length == 0)
		return s;

    char* back = s + length;
    while(isspace((unsigned char)(*--back)));
	*(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

int trim_ch(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}

int trimmed(const char* src, char** dst) 
{
	if (strlen(src) == 0)
		return 0;

	size_t src_size = strlen(src);

	int lp /* left padding */ = 0;
	int rp /* right padding */ = 0;
	while (trim_ch(src[lp]) && ++lp);;
	while (trim_ch(src[src_size-rp-1]) && ++rp);;

	/* optionally reallocate to not waste unused memory */
	/* NOTE: change this to malloc if you don't want to pre-allocate the out buffer */
	if (lp+rp != 0)
		(*dst) = (char*)realloc((*dst), sizeof(char) * src_size-lp-rp);	

	int i;
	for (i = lp; i <(int)( src_size-rp); ++i)
		(*dst)[i-lp] = src[i];

#ifdef  _WIN32
	(*dst)[src_size - lp - rp - 1] = '\0';
#else
	(*dst)[src_size - lp - rp] = '\0';
#endif //  _WIN32	
	
	return lp+rp;
}

/*
	char *a = "1 2 3      4 5";
	List *lst;
	list_new(&lst);
	int c = string_to_list (a, " ", lst);
	int i=0;
	for(i=0; i<c; i++) {
		char *v;
		list_get_at(lst, i, &v);
		printf("-- %s\n", v);
	}
*/
int string_to_list (const char *str, const char *tok, List *list)
{
	char *tmp = strdup(str);
	char *p = strtok (tmp, tok);
	while (p != NULL)
    {
		char *v = strdup(p);
		list_add(list, v);

        p = strtok (NULL, tok);
    }
	free(tmp);

	return list_size(list);
}

/*
	char *a = "1 2 3      4 5";
	List *lst;
	list_new(&lst);
	int c = string_to_int_list (a, " ", lst);
	int i=0;
	for(i=0; i<c; i++) {
		int *v;
		list_get_at(lst, i, &v);
		printf("-- %d\n", *v);
	}
*/
int string_to_int_list (const char *str, const char *tok, List *list)
{
	char *tmp = strdup(str);
	char *p = strtok (tmp, tok);
	while (p != NULL)
    {
		int *v = malloc(sizeof(int));
		*v = atoi(p);
		list_add(list, v);

        p = strtok (NULL, tok);
    }
	free(tmp);

	return list_size(list);
}

#define INIT_SIZE 8

char* str(int size) {
	char* string = (char*)malloc(sizeof(char) * size);
	int i;
	for (i = 0; i < size; i++)
		string[i] = '\0';

	return string;
}

char** split(char* sentence, char separator, int* num_tokens) {
	char** tokens;
	int* lengths;
	int tokens_idx = 0;
	int token_idx = 0;
	*num_tokens = 1;
	int i;
	for (i = 0; i < strlen(sentence); i++) {
		if (sentence[i] == separator)
			(*num_tokens)++;
	}

	lengths = (int*)malloc(sizeof(int) * (*num_tokens));
	tokens = (char**)malloc(sizeof(char*) * (*num_tokens));
	
	for (i = 0; i < *num_tokens; i++) {
		tokens[i] = str(INIT_SIZE);
		lengths[i] = INIT_SIZE;
	}
	
	for (i = 0; i < strlen(sentence); i++) {
		if (sentence[i] == separator && strlen(tokens[tokens_idx]) != 0) {
			token_idx = 0;
			tokens_idx++;
		}
		else if (sentence[i] == separator && strlen(tokens[tokens_idx]) == 0) {
			continue;
		}
		else {
			/* Memory reallocation, If  array is full. */

			if (strlen(tokens[tokens_idx]) == lengths[tokens_idx] - 1) {
				tokens[tokens_idx] = realloc(tokens[tokens_idx], (lengths[tokens_idx] * sizeof(char)) << 1);
				int j;
				for (j = lengths[tokens_idx]; j < lengths[tokens_idx] << 1; j++)
					tokens[tokens_idx][j] = '\0';

				lengths[tokens_idx] <<= 1;
			}

			tokens[tokens_idx][token_idx] = sentence[i];
			token_idx++;
		}
	}

	return tokens;
}


void err(char *errmsg)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	allegro_message("%s", errmsg);
	exit(1);
}

/*
char *str;
str = concat(0);             println(str); free(str);
str = concat(1,"a");         println(str); free(str);
str = concat(2,"a","b");     println(str); free(str);
str = concat(3,"a","b","c"); println(str); free(str);
*/
char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}

int is_number (char *str)
{
	int i=0;
	for(i=0; i<(int)strlen(str); i++) {
		char ch = *(str+i);
		int is_digit = isdigit(ch);
		if (is_digit == 0) return FALSE;
	}
	return TRUE;
}

void show_available_memory(BITMAP *b, int x, int y)
{
	char tmp[256];
	unsigned long phymem, virtmem;

	get_available_memory(&phymem, &virtmem);

	sprintf(tmp, "P:%luK/V:%luK", phymem/1024, virtmem/1024);
	int width = text_length(font, tmp);
	int height = text_height(font);
	//textout_ex(b, font, tmp, SCREEN_W-width, 0, makecol(255,255,255), -1);
	textout_ex(b, font, tmp, x, y, makecol(255,255,255), -1);
}

// http://horis.kanardia.eu/rok2/Builder/igep_old/buildroot-2015.08-rc1/output/build/host-lzop-1.03/src/djgpp2.c
int get_available_memory(unsigned long *phymem, unsigned long *virtmem)
{
#if 0
    __dpmi_free_mem_info info;
    const unsigned long err = (unsigned long) -1;
    unsigned long ps = 0;
    unsigned long k;

    if (__dpmi_get_page_size(&ps) != 0 || ps <= 0)
        return FALSE;
    if (__dpmi_get_free_memory_information(&info) != 0)
        return FALSE;

    k = info.total_number_of_free_pages;
    if (k != err)
    {
        // DPMI physical memory available
        k = (k * ps);
		*phymem = k;
    }
#else
#ifndef _WIN32
	*phymem = _go32_dpmi_remaining_physical_memory();
#endif
#endif


#if 0
    k = info.maximum_unlocked_page_allocation_in_pages;
    if (k != err)
    {
        // DPMI virtual memory available
        k = (k * ps);
		*virtmem = k;
    }
#else
#ifndef _WIN32
	*virtmem = _go32_dpmi_remaining_virtual_memory();
#endif
#endif

	return TRUE;
}

void fade_in_new(BITMAP *bmp_orig, int speed)
{
   BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H)))
    {
        int a;
        if (speed <= 0) speed = 16;
        
        for (a = 0; a < 256; a+=speed)
        {
            clear(bmp_buff);
            set_trans_blender(0,0,0,a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            vsync();
            blit(bmp_buff, screen, 0,0, 0,0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
        }
        destroy_bitmap(bmp_buff);
    } 

    blit(bmp_orig, screen, 0,0, 0,0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
}

void fade_out_new(int speed)
{
    BITMAP *bmp_orig, *bmp_buff;

    if ((bmp_orig = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H)))
    {
        if ((bmp_buff = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H)))
        {
            int a;
            blit(screen, bmp_orig, 0,0, 0,0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
            if (speed <= 0) speed = 16;
        
            for (a = 255-speed; a > 0; a-=speed)
            {
                clear(bmp_buff);
                set_trans_blender(0,0,0,a);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                vsync();
                blit(bmp_buff, screen, 0,0, 0,0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }

    rectfill(screen, 0,0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H, makecol(0,0,0));
}

void capture_screen(BITMAP *bmp)
{
	char filename[256];
	char path[256];
	char buf[13];
	PALETTE pal;
	int c;

	get_executable_name(path, 256);

	for (c=0; c<100; c++) {
		sprintf(buf, "tetris%02d.bmp", c);
		replace_filename(filename, path, buf, 256);

		if (!exists(buf))
			break;
	}

	get_palette(pal);

	save_bmp(filename, bmp, pal);
}

int rand_number(int min, int max)
{
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void *copy_to_int(void *e1)
{
	char *str = (char*)e1;
    int *cp = (int *) malloc(sizeof(int));
    *cp = atoi(str);
    return cp;
}

int show_bitmap(BITMAP *b, PALETTE pal)
{
	clear_keybuf();

	BITMAP *buffer = create_bitmap(LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	clear(buffer);
	
	acquire_screen();
	set_pallete(black_pallete);
	blit(b, screen, 0, 0, 0, 0, LOGICAL_SCREEN_W, LOGICAL_SCREEN_H);
	release_screen();
	
	//set_palette(pal);
	fade_in(pal, 5);

	rest(2000);

	fade_out(3);

	clear_keybuf();
	clear(screen);

	return TRUE;
}

void read_text_file(char *filename, char **buf)
{
	FILE *f;

	long size = file_size(filename);
	f = fopen(filename, "r");

	*buf = malloc(size+1);

#ifdef _WIN32
	memset(*buf, 0, size+1);
#else
	memset(*buf, '\0', size);
#endif

	fread(*buf, sizeof(char), size, f);

	fclose(f);
}
