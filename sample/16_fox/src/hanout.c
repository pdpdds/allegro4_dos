#include "HANOUT.H"
#include "HANCONV.H"

const unsigned char johab_table[3][32] = {
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	  15,16,17,18,19,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 6, 7, 8, 9,10,11,
	  0, 0,12,13,14,15,16,17, 0, 0,18,19,20,21, 0, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16, 0,17,18,19,20,21,22,23,24,25,26,27, 0, 0 } 
};

unsigned char fist[8][20][32];
unsigned char mid[4][22][32];
unsigned char end[4][28][32];
unsigned char english[256][16];

int __cur_x=0,__cur_y=0,__b,__attr=0;

struct h_i{
	int x;
	int y;
	unsigned char b[32];
} hib;

struct e_i{
	int x;
	int y;
	unsigned char b[16];
} eib;

void al_load_buff_hanfont(unsigned char *buf)
{
	memcpy(fist, buf, 5120);
	memcpy(mid, buf+5120, 2816);
	memcpy(end, buf+5120+2816, 3584);
}

void al_load_hanfont(char *file)
{
	int h = open(file,O_RDONLY | O_BINARY);
	if (h == -1) {
		exit(1);
	}
	read(h,fist,5120);
	read(h,mid,2816);
	read(h,end,3584);
	close(h);
}

void al_load_buff_engfont(unsigned char *buf)
{
	memcpy(english, buf, 4096);
}

void al_load_engfont(char *file)
{
	int h = open(file,O_RDONLY | O_BINARY);
	if (h == -1) {
		exit(1);
	}
	read(h,english,4096);
	close(h);
}

void IMG_1(unsigned char *dest, int bytes)
{
	int i;
	for(i=0; i !=bytes; i++) *(dest+i) = ~*(dest+i);
}

void IMG_2(unsigned char *dest, int bytes)
{
	int i;
	for(i=0; i != bytes;)
	{
		*(dest + i++) |= 0xaa;
		if (bytes==32) *(dest + i++) |= 0xaa;
		*(dest + i++) |= 0x55;
		if (bytes==32) *(dest + i++) |= 0x55;
	}
}

void IMG_3(unsigned char *dest, int bytes)
{
	int i;
	char temp1[32],temp2[32];
	memcpy(temp1, dest, bytes);
	memcpy(temp2, dest, bytes);

	if (bytes == 16)
	{
		for(i=0; i<= 15; i++) temp1[i] |= temp2[i] >> 1;
		for(i=0; i<= 14; i++) temp1[i+1] |= temp2[i];
		for(i=0; i<= 14; i++) temp1[i] ^= temp2[i];
	}
	else  {
		for(i=0; i<= 31; i++) temp1[i] |= temp2[i] >> 1;
		for(i=0; i<= 29; i++) temp1[i+2] |= temp2[i];
		for(i=0; i<= 31; i++) temp1[i] ^= temp2[i];
	}
	memcpy(dest, temp1, bytes);
}

void IMG_4(unsigned char *dest, int bytes)
{
	int i;
	char temp1[32],temp2[32];
	memcpy(temp1, dest, bytes);
	memcpy(temp2, dest, bytes);

	if (bytes == 16)
	{
		for(i=0;i < 16;i++)
		{
			temp1[i] |= temp2[i] >> 1;
			temp1[i] |= temp2[i] << 1;
		}
		for(i=0; i<= 14; i++) temp1[i+1] |= temp2[i];
		for(i=1; i<= 15; i++) temp1[i-1] |= temp2[i];
		for(i=0; i<= 14; i++) temp1[i] ^= temp2[i];
	}
	else {
		for(i = 0;i < 32; i++) 
		{
			temp1[i] |= temp2[i] >> 1;
			temp1[i] |= temp2[i] << 1;
		}
		for(i=0; i<= 29; i++) temp1[i+2] |= temp2[i];
		for(i=2; i<= 31; i++) temp1[i-2] |= temp2[i];
		for(i=0; i<= 31; i++) temp1[i] ^= temp2[i];
	}
	memcpy(dest, temp1, bytes);
}

void IMG_5(unsigned char *dest, int bytes)
{
	int i;
	if (bytes == 16)
		for(i = 0; i < 16 ;i++) dest[i] |= dest[i] >>1;
	else
		for(i = 0; i < 32 ;i++) dest[i] |= dest[i] >>1;
}

void IMG_6(unsigned char *dest, int bytes)
{
	if (bytes == 16) 
		dest[bytes-1] = 0xff;
	else {
		dest[bytes-2] = 0xff;
		dest[bytes-1] = 0xff;
	}
}

void hputch(BITMAP *bmp, int x,int y, unsigned char *f,int c)
{
	int i;
	if (__b==16){
		for (i=0; i< 16 ; i++) {
			if ((*(f+i) & 0x80)==0x80) putpixel(bmp, x  ,y,c);
			if ((*(f+i) & 0x40)==0x40) putpixel(bmp, x+1,y,c);
			if ((*(f+i) & 0x20)==0x20) putpixel(bmp, x+2,y,c);
			if ((*(f+i) & 0x10)==0x10) putpixel(bmp, x+3,y,c);
			if ((*(f+i) & 0x08)==0x08) putpixel(bmp, x+4,y,c);
			if ((*(f+i) & 0x04)==0x04) putpixel(bmp, x+5,y,c);
			if ((*(f+i) & 0x02)==0x02) putpixel(bmp, x+6,y,c);
			if ((*(f+i) & 0x01)==0x01) putpixel(bmp, x+7,y,c);
			y++;
		}
		return;
	}
	if (__b==32){
		for (i=0; i< 32 ; i++) {
			if ((*(f+i) & 0x80)==0x80) putpixel(bmp, x  ,y,c);
			if ((*(f+i) & 0x40)==0x40) putpixel(bmp, x+1,y,c);
			if ((*(f+i) & 0x20)==0x20) putpixel(bmp, x+2,y,c);
			if ((*(f+i) & 0x10)==0x10) putpixel(bmp, x+3,y,c);
			if ((*(f+i) & 0x08)==0x08) putpixel(bmp, x+4,y,c);
			if ((*(f+i) & 0x04)==0x04) putpixel(bmp, x+5,y,c);
			if ((*(f+i) & 0x02)==0x02) putpixel(bmp, x+6,y,c);
			if ((*(f+i) & 0x01)==0x01) putpixel(bmp, x+7,y,c);
			i++;
			if ((*(f+i) & 0x80)==0x80) putpixel(bmp, x+8 ,y,c);
			if ((*(f+i) & 0x40)==0x40) putpixel(bmp, x+9 ,y,c);
			if ((*(f+i) & 0x20)==0x20) putpixel(bmp, x+10,y,c);
			if ((*(f+i) & 0x10)==0x10) putpixel(bmp, x+11,y,c);
			if ((*(f+i) & 0x08)==0x08) putpixel(bmp, x+12,y,c);
			if ((*(f+i) & 0x04)==0x04) putpixel(bmp, x+13,y,c);
			if ((*(f+i) & 0x02)==0x02) putpixel(bmp, x+14,y,c);
			if ((*(f+i) & 0x01)==0x01) putpixel(bmp, x+15,y,c);
			y++;
		}
	}
}

void or_IMG(unsigned char *src,unsigned char *dest, int bytes)
{
	int i;
	for(i=0; i != bytes; i++) *(dest+i) |= *(src+i);
}

void pan_font(int f)
{
	if (__attr==f) {} else {
		switch(f)
		{
			case 0 : __attr =0; break;
			case 1 : __attr =1; break;
			case 2 : __attr =2; break;
			case 3 : __attr =3; break;
			case 4 : __attr =4; break;
			case 5 : __attr =5; break;
			case 6 : __attr =6; break;
		}
	}
}

void han_compose(BITMAP *bmp, unsigned char cho, unsigned char jung,unsigned char jong, int c,int f)
{
	int t1 = 0, t2=0, t3=0;
	memset(hib.b,0,32*4);
	cho  = johab_table[0][cho];
	jung = johab_table[1][jung];
	jong = johab_table[2][jong];

	pan_font(f);
	switch(jung)
	{
		case 1 : case 3: case 10: t3=0; break;
		case 5 : case 7: case 12: case 15: case 17: case 20:
		case 21:  t3=1; break;
		case 2 : case 4: case  6: case  8: case 11: case 16:
		t3=2; break;
		case 9 : case 13: case 14: case 18: case 19: t3=3; break;
	}
	if (cho == 1 || cho == 16) { 
		if (jong == 0) 
			t2 = 0; 
		else 
			t2 = 2; 
	}
	else {
		if (jong == 0) 
			t2 = 1; 
		else 
			t2 = 3; 
	}

	switch(jung)
	{
		case 1: case  2: case 3: case 4: case 5: case 6: case 7:
		case 8: case 21: if (jong == 0) t1 = 0; else  t1 = 5; break;
		case 9: case 13: case 19: if (jong == 0) t1 = 1; else t1 = 6; break;
		case 14: case 18: if (jong == 0) t1 = 2; else  t1 = 6; break;
		case 10: case 11: case 12: case 20: if (jong == 0) t1 = 3; else  t1 = 7;break;
		case 15: case 16: case 17: if (jong == 0) t1 = 4; else t1 = 7; break;
	}
	if ((jung == 0) && (jong == 0)) t1=0;

	if (cho  != 0) or_IMG( fist[t1][cho], hib.b, 32);
	if (jung != 0) or_IMG( mid[t2][jung], hib.b, 32);
	if (jong != 0) or_IMG( end[t3][jong], hib.b, 32);

	switch (__attr)
	{
		case 1 : IMG_1(hib.b, 32); break;
		case 2 : IMG_2(hib.b, 32); break;
		case 3 : IMG_3(hib.b, 32); break;
		case 4 : IMG_4(hib.b, 32); break;
		case 5 : IMG_5(hib.b, 32); break;
		case 6 : IMG_6(hib.b, 32); break;
	}
	//if (__cur_x > 304) { __cur_x = 0; __cur_y += 16; }
	__b=32; hputch(bmp, __cur_x,__cur_y, hib.b,c);
	__cur_x+=16;
}

void eng_compose(BITMAP *bmp, unsigned char keyin, int c,int f)
{
	pan_font(f);
	memset(eib.b, 0, 16*4);
	memmove(eib.b, english[keyin], 16);

	switch (__attr)
	{
		case 1 : IMG_1(eib.b, 16); break;
		case 2 : IMG_2(eib.b, 16); break;
		case 3 : IMG_3(eib.b, 16); break;
		case 4 : IMG_4(eib.b, 16); break;
		case 5 : IMG_5(eib.b, 16); break;
		case 6 : IMG_6(eib.b, 16); break;
	}
	//if (__cur_x > 302) { __cur_x = 0; __cur_y += 16; }
	__b=16;hputch(bmp, __cur_x ,__cur_y, eib.b,c);
	__cur_x += 8;
}

void al_han_puts(BITMAP *bmp, int x, int y, int color, int attr, char *str)
{
	unsigned char data1, data2;
	unsigned char cho, jung,jong;
	int t1,t2,t3,i;

	__cur_x=x;
	__cur_y=y;

	for(i=0; i<= (int)strlen(str); i++)
	{
		data1 = *(str+i);
		if (data1 > 127) {
			i++;
			data2 = *(str+i);
			cho  = (data1 & 124) >> 2;
			jung = (data1 & 3) * 8 + (data2 >> 5);
			jong = (data2 & 31);
			han_compose(bmp, cho,jung,jong,color,attr);

		} else { 
			eng_compose(bmp, data1,color,attr); 
		}
	}
}

void al_han_textout_centre_ex(BITMAP *b, char *msg, int x, int y, int color)
{
	int x2 = x - (strlen(msg)*8) / 2;
	int y2 = y - 8;

	al_han_puts(b, x2, y2, color, ATTR_NORMAL, msg);
}

void al_han_textout(BITMAP *b, char *msg, int x, int y, int color)
{
	al_han_puts(b, x, y, color, ATTR_NORMAL, msg);
}

#ifdef _WIN32
extern int vasprintf(char **strp, const char *fmt, va_list ap);
#endif

int al_han_printf(BITMAP *bmp, int x, int y, int color, int attr, char *fmt, ...) 
{
	va_list ap;
	char* s;
	int len;

	va_start(ap, fmt);
	len = vasprintf(&s, fmt, ap);
	va_end(ap);

	al_han_puts(bmp, x, y, color, attr, s);
	
	free((void*)s);
	
	return len;
}

