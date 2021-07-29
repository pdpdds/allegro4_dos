/* Mappy playback library functions and variables
 * (C)2001 Robin Burrows - rburrows@bigfoot.com
 * This version (R11C) released May 2002 - for Mappy FMP maps
 * Please read the readmeal.txt file and look at the examples
 */

#include <stdio.h>
#include <string.h>
#include <allegro.h>

#ifdef __cplusplus
#error "Hey, stop compiling mappyal.c as C++!, see MappyAL readme.txt"
#endif

/* Comment out next line to disable index0 to truecolour pink conversion */
#define RB8BITTOPINK


#define MER_NONE 0		/* All the horrible things that can go wrong */
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3
#define MER_NOSCREEN 4
#define MER_NOACCELERATION 5
#define MER_CVBFAILED 6
#define MER_MAPTOONEW 7

#define AN_END -1			/* Animation types, AN_END = end of anims */
#define AN_NONE 0			/* No anim defined */
#define AN_LOOPF 1		/* Loops from start to end, then jumps to start etc */
#define AN_LOOPR 2		/* As above, but from end to start */
#define AN_ONCE 3			/* Only plays once */
#define AN_ONCEH 4		/* Only plays once, but holds end frame */
#define AN_PPFF 5			/* Ping Pong start-end-start-end-start etc */
#define AN_PPRR 6			/* Ping Pong end-start-end-start-end etc */
#define AN_PPRF 7			/* Used internally by playback */
#define AN_PPFR 8			/* Used internally by playback */
#define AN_ONCES 9		/* Used internally by playback */

typedef struct {				/* Structure for data blocks */
long int bgoff, fgoff;			/* offsets from start of graphic blocks */
long int fgoff2, fgoff3; 		/* more overlay blocks */
unsigned long int user1, user2;	/* user long data */
unsigned short int user3, user4;	/* user short data */
unsigned char user5, user6, user7;	/* user byte data */
unsigned char tl : 1;				/* bits for collision detection */
unsigned char tr : 1;
unsigned char bl : 1;
unsigned char br : 1;
unsigned char trigger : 1;			/* bit to trigger an event */
unsigned char unused1 : 1;
unsigned char unused2 : 1;
unsigned char unused3 : 1;
} BLKSTR;

typedef struct {		/* Animation control structure */
signed char antype;	/* Type of anim, AN_? */
signed char andelay;	/* Frames to go before next frame */
signed char ancount;	/* Counter, decs each frame, till 0, then resets to andelay */
signed char anuser;	/* User info */
long int ancuroff;	/* Points to current offset in list */
long int anstartoff;	/* Points to start of blkstr offsets list, AFTER ref. blkstr offset */
long int anendoff;	/* Points to end of blkstr offsets list */
} ANISTR;


/* All global variables used by Mappy playback are here */
int maperror, mapgfxinbitmaps;		/* Set to a MER_ error if something wrong happens */
short int mapwidth, mapheight, mapblockwidth, mapblockheight, mapdepth;
short int mapblockstrsize, mapnumblockstr, mapnumblockgfx;
PACKFILE * mapfilept;
short int * mappt = NULL;
short int ** maparraypt = NULL;
char * mapcmappt = NULL;
char * mapblockgfxpt = NULL;
char * mapblockstrpt = NULL;
ANISTR * mapanimstrpt = NULL;
int * mapanimseqpt = NULL;
ANISTR * mapanimstrendpt;
RGB mapcmap6bit[256];
short int * mapmappt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
short int ** mapmaparraypt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
BITMAP ** abmTiles = NULL;
/* NEW for Release 6 */
int mapaltdepth;
int maptype, mapislsb, mapclickmask;
int mapblockgapx, mapblockgapy, mapblockstaggerx, mapblockstaggery;
int mapblocksinvidmem, mapblocksinsysmem;
char mapnovctext[80];
/* End of Mappy globals */

int MapGenerateYLookup (void)
{
int i, j;

	for (i=0;i<8;i++) {
		if (mapmaparraypt[i]!=NULL) { free (mapmaparraypt[i]); mapmaparraypt[i] = NULL; }
		if (mapmappt[i]!=NULL) {
			mapmaparraypt[i] = malloc (mapheight*sizeof(short int *));
			if (mapmaparraypt[i] == NULL) return -1;
			for (j=0;j<mapheight;j++) mapmaparraypt[i][j] = (mapmappt[i]+(j*mapwidth));
			if (mapmappt[i] == mappt) maparraypt = mapmaparraypt[i];
		}
	}
	return 0;
}

static int MEClickmask (int x, int y, int xory)
{
	if (abmTiles == NULL) return 0;

	x %= mapblockgapx; y %= mapblockgapy;

	if (x >= mapblockwidth && xory == 0) return 0;
	if (x >= mapblockwidth && xory == 1) {
		if (y < mapblockstaggery) return -1;
		else return 1;
	}
	if (y >= mapblockheight && xory == 1) return 1;
	if (y >= mapblockheight && xory == 0) {
		if (x < mapblockstaggerx) return -1;
		else return 0;
	}

	switch (mapdepth) {
		case 8:
			if (getpixel (abmTiles[mapclickmask], x, y) == 0) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
		default:
			if (getpixel (abmTiles[mapclickmask], x, y) == makecol (255,0,255)) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
	}
	return 0;
}

int MapGetXOffset (int xpix, int ypix)
{
int xb;

	if (mapblockstaggerx || mapblockstaggery) {
		xpix += (mapblockstaggerx);
		ypix += (mapblockstaggery);
	}
	xb = xpix/mapblockgapx;

	if ((mapblockstaggerx || mapblockstaggery) && mapclickmask) xb += MEClickmask (xpix, ypix, 0);

	if (xb < 0) xb = 0;
	if (xb >= mapwidth) xb = mapwidth-1;
	return xb;
}

int MapGetYOffset (int xpix, int ypix)
{
int yb;

	if (mapblockstaggerx || mapblockstaggery) {
		xpix += (mapblockstaggerx);
		ypix += (mapblockstaggery);
	}
	yb = ypix/mapblockgapy;

	if ((mapblockstaggerx || mapblockstaggery) && mapclickmask) {
		yb *= 2;
		yb += MEClickmask (xpix, ypix, 1);
	}

	if (yb < 0) yb = 0;
	if (yb >= mapheight) yb = mapheight-1;
	return yb;
}

BLKSTR * MapGetBlockInPixels (int x, int y)
{
int xp, yp;
short int * mymappt;
ANISTR * myanpt;

	if (x < 0 || y < 0 || x >= (mapwidth*mapblockwidth) || y >= (mapheight*mapblockheight)) return NULL;

	xp = x; yp = y;
	x = MapGetXOffset (xp, yp);
	y = MapGetYOffset (xp, yp);

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (*mymappt>=0) return ((BLKSTR*) mapblockstrpt) + *mymappt;
	else { myanpt = mapanimstrendpt + *mymappt;
		return ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
}

BLKSTR * MapGetBlock (int x, int y)
{
short int * mymappt;
ANISTR * myanpt;

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (*mymappt>=0) return ((BLKSTR*) mapblockstrpt) + *mymappt;
	else { myanpt = mapanimstrendpt + *mymappt;
		return ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
}

void MapSetBlockInPixels (int x, int y, int strvalue)
{
int xp, yp;
short int * mymappt;

	if (x < 0 || y < 0 || x >= (mapwidth*mapblockwidth) || y >= (mapheight*mapblockheight)) return;
	xp = x; yp = y;
	x = MapGetXOffset (xp, yp);
	y = MapGetYOffset (xp, yp);

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	*mymappt = strvalue;
}

void MapSetBlock (int x, int y, int strvalue)
{
short int * mymappt;

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	*mymappt = strvalue;
}

int MapChangeLayer (int newlyr)
{
	if (newlyr<0 || newlyr>7 || mapmappt[newlyr] == NULL) return -1;
	mappt = mapmappt[newlyr]; maparraypt = mapmaparraypt[newlyr];
	return newlyr;
}

int MapGetBlockID (int blid, int usernum)
{
int i;
BLKSTR * myblkpt;

	myblkpt = (BLKSTR *) mapblockstrpt;
	if (myblkpt == NULL) return -1;

	for (i=0;i<mapnumblockstr;i++) {
		switch (usernum) {
			case 1:
				if (myblkpt[i].user1 == blid) return i;
				break;
			case 2:
				if (myblkpt[i].user2 == blid) return i;
				break;
			case 3:
				if (myblkpt[i].user3 == blid) return i;
				break;
			case 4:
				if (myblkpt[i].user4 == blid) return i;
				break;
			case 5:
				if (myblkpt[i].user5 == blid) return i;
				break;
			case 6:
				if (myblkpt[i].user6 == blid) return i;
				break;
			case 7:
				if (myblkpt[i].user7 == blid) return i;
				break;
		}
	}

	return -1;
}

int MapDecodeMAR (unsigned char * mrpt, int marlyr)
{
int i, j;
short int * mymarpt;

	if (marlyr < 0 || marlyr > 7) return -1;

	if (mapmappt[marlyr] == NULL)
		mapmappt[marlyr] = malloc (mapwidth*mapheight*sizeof(short int));

	memcpy (mapmappt[marlyr], mrpt, (mapwidth*mapheight*sizeof(short int)));

	mymarpt = mapmappt[marlyr];
	j = 0; for (i=0;i<(mapwidth*mapheight);i++) { if (mymarpt[i]&0xF) j = 1; }
	if (j == 0) {
		for (i=0;i<(mapwidth*mapheight);i++) {
			if (mymarpt[i] >= 0) mymarpt[i] /= 32;
			else mymarpt[i] /= 16;
		}
	}

	return 0;
}

int MapLoadMAR (char * mname, int marlyr)
{
int i, j;
short int * mymarpt;
PACKFILE * marfpt;

	if (marlyr < 0 || marlyr > 7) return -1;

	marfpt = pack_fopen (mname, "rp");
	if (marfpt==NULL) { marfpt = pack_fopen (mname, "r");
		if (marfpt==NULL) { return -1; } }

	if (mapmappt[marlyr] == NULL)
		mapmappt[marlyr] = malloc (mapwidth*mapheight*sizeof(short int));

	if (pack_fread (mapmappt[marlyr], (mapwidth*mapheight*sizeof(short int)), marfpt) !=
		(mapwidth*mapheight*sizeof(short int))) { pack_fclose (marfpt); return -1; }

	pack_fclose (marfpt);

	mymarpt = mapmappt[marlyr];
	j = 0; for (i=0;i<(mapwidth*mapheight);i++) { if (mymarpt[i]&0xF) j = 1; }
	if (j == 0) {
		for (i=0;i<(mapwidth*mapheight);i++) {
			if (mymarpt[i] >= 0) mymarpt[i] /= 32;
			else mymarpt[i] /= 16;
		}
	}

	return 0;
}

void MapInitAnims (void)
{
ANISTR * myanpt;
	if (mapanimstrpt==NULL) return;
	myanpt = (ANISTR *) mapanimstrendpt; myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype==AN_PPFR) myanpt->antype = AN_PPFF;
		if (myanpt->antype==AN_PPRF) myanpt->antype = AN_PPRR;
		if (myanpt->antype==AN_ONCES) myanpt->antype = AN_ONCE;
		if ((myanpt->antype==AN_LOOPR) || (myanpt->antype==AN_PPRR))
		{
		myanpt->ancuroff = myanpt->anstartoff;
		if ((myanpt->anstartoff)!=(myanpt->anendoff)) myanpt->ancuroff=(myanpt->anendoff)-1;
		} else {
		myanpt->ancuroff = myanpt->anstartoff;
		}
		myanpt->ancount = myanpt->andelay;
		myanpt--;
	}
}

void MapUpdateAnims (void)
{
ANISTR * myanpt;

	if (mapanimstrpt==NULL) return;
	myanpt = (ANISTR *) mapanimstrendpt; myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype!=AN_NONE) { myanpt->ancount--; if (myanpt->ancount<0) {
		myanpt->ancount = myanpt->andelay;
		if (myanpt->antype==AN_LOOPF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) myanpt->ancuroff = myanpt->anstartoff;
		} }
		if (myanpt->antype==AN_LOOPR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1))
				myanpt->ancuroff = (myanpt->anendoff)-1;
		} }
		if (myanpt->antype==AN_ONCE)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->antype = AN_ONCES;
				myanpt->ancuroff = myanpt->anstartoff; }
		} }
		if (myanpt->antype==AN_ONCEH)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) {
			if (myanpt->ancuroff!=((myanpt->anendoff)-1)) myanpt->ancuroff++;
		} }
		if (myanpt->antype==AN_PPFF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 2;
			myanpt->antype = AN_PPFR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff++; }
		} } else {
		if (myanpt->antype==AN_PPFR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1)) { myanpt->ancuroff += 2;
			myanpt->antype = AN_PPFF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff --; }
		} } }
		if (myanpt->antype==AN_PPRR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1)) { myanpt->ancuroff += 2;
			myanpt->antype = AN_PPRF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff--; }
		} } else {
		if (myanpt->antype==AN_PPRF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 2;
			myanpt->antype = AN_PPRR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff++; }
		} } }
	} } myanpt--; }
}

void Mapconv8to6pal (unsigned char * palpt)
{
int i;
	for (i=0;i<256;i++)
	{
		mapcmap6bit[i].r=(*(palpt)>>2);
		mapcmap6bit[i].g=(*(palpt+1)>>2);
		mapcmap6bit[i].b=(*(palpt+2)>>2);
		palpt+=3;
	}
}

void MapFreeMem (void)
{
int i;
	for (i=0;i<8;i++) { if (mapmappt[i]!=NULL) { free (mapmappt[i]); mapmappt[i] = NULL; } }
	mappt = NULL;
	for (i=0;i<8;i++) { if (mapmaparraypt[i]!=NULL) { free (mapmaparraypt[i]); mapmaparraypt[i] = NULL; } }
	maparraypt = NULL;
	if (mapcmappt!=NULL) { free (mapcmappt); mapcmappt = NULL; }
	if (mapblockgfxpt!=NULL) { free (mapblockgfxpt); mapblockgfxpt = NULL; }
	if (mapblockstrpt!=NULL) { free (mapblockstrpt); mapblockstrpt = NULL; }
	if (mapanimseqpt!=NULL) { free (mapanimseqpt); mapanimseqpt = NULL; }
	if (mapanimstrpt!=NULL) { free (mapanimstrpt); mapanimstrpt = NULL; }
	if (abmTiles != NULL) {
		i = 0; while (abmTiles[i]!=NULL) { destroy_bitmap (abmTiles[i]); i++; }
		free (abmTiles); abmTiles = NULL;
	}
	mapnovctext[0] = 0;
	mapblocksinvidmem = 0; mapblocksinsysmem = 0;
}

void MapSetPal8 (void)
{
	if (screen!=NULL) { if ((bitmap_color_depth (screen)==8) && mapdepth == 8) set_palette (mapcmap6bit); }
}

void MapCorrectColours (void)
{
/* Remember, Intel stores shorts and longs the wrong way round!
 * ie. 0x12345678 will be 0x78563412
 */
	return;
}

void MapRestore (void)
{
int i, j, k;
unsigned char * newgfxpt;

	if (mapgfxinbitmaps!=1 || abmTiles == NULL) return;
	i = 0; newgfxpt = mapblockgfxpt; while (abmTiles[i]!=NULL) {
		acquire_bitmap (abmTiles[i]);
		for (k=0;k<mapblockheight;k++) {
		for (j=0;j<mapblockwidth;j++) {
		switch (mapdepth) {
			case 8:
				putpixel (abmTiles[i], j, k, *((unsigned char *) newgfxpt));
				newgfxpt++;
				break;
			case 15:
			case 16:
				putpixel (abmTiles[i], j, k, *((unsigned short int *) newgfxpt));
				newgfxpt+=2;
				break;
			case 24:
				putpixel (abmTiles[i], j, k, makecol (newgfxpt[0], newgfxpt[1], newgfxpt[2]));
				newgfxpt+=3;
				break;
			case 32:
				putpixel (abmTiles[i], j, k, makecol (newgfxpt[1], newgfxpt[2], newgfxpt[3]));
				newgfxpt+=4;
				break;
		} } }
		release_bitmap (abmTiles[i]);
		i++;
	}
}

int MapRelocate2 (void)
{
int i, j, k;
BLKSTR * myblkstrpt;
//ANISTR * myanpt;
unsigned char * newgfxpt, * novcarray;
char ascnum[80];
//long int * myanblkpt;

	i = mapnumblockstr;
	myblkstrpt = (BLKSTR *) mapblockstrpt;

	novcarray = malloc (mapnumblockgfx);
	memset (novcarray, 0, mapnumblockgfx);
	i = 0; while (mapnovctext[i] != 0) {
		j = 0; while (mapnovctext[i] >= '0' && mapnovctext[i] <= '9') {
			ascnum[j] = mapnovctext[i];
			i++; j++;
		}
		ascnum[j] = 0;
		k = atoi(ascnum);
		if (k < 0 || k >= mapnumblockgfx) break;
		if (mapnovctext[i] == '-') {
			i++;
			j = 0; while (mapnovctext[i] >= '0' && mapnovctext[i] <= '9') {
				ascnum[j] = mapnovctext[i];
				i++; j++;
			}
			ascnum[j] = 0;
			j = atoi(ascnum);
			if (j < k || j >= mapnumblockgfx) break;
			while (k <= j) {
				novcarray[k] = 1; k++;
			}
		} else {
			novcarray[k] = 1;
		}
		if (mapnovctext[i] == ',') i++;
	}

	if (abmTiles == NULL) abmTiles = malloc ((sizeof (BITMAP *))*(mapnumblockgfx+2));
	abmTiles[0] = NULL;
	i = 0; newgfxpt = mapblockgfxpt; while (i<mapnumblockgfx) {
		abmTiles[i+1] = NULL;
		if (mapgfxinbitmaps==1 && novcarray[i]==0) {
			abmTiles[i] = create_video_bitmap (mapblockwidth, mapblockheight);
			if (abmTiles[i] != NULL) {
					mapblocksinvidmem++;
					acquire_bitmap (abmTiles[i]);
			} else {
				abmTiles[i] = create_bitmap (mapblockwidth, mapblockheight);
				if (abmTiles[i] == NULL) { MapFreeMem (); maperror = MER_CVBFAILED ; return -1; }
					mapblocksinsysmem++;
			}
			set_clip (abmTiles[i], 0, 0, 0, 0);
		} else {
			abmTiles[i] = create_bitmap (mapblockwidth, mapblockheight);
			if (abmTiles[i] == NULL) { MapFreeMem (); maperror = MER_CVBFAILED ; return -1; }
			mapblocksinsysmem++;
			set_clip (abmTiles[i], 0, 0, 0, 0);
		}
		for (k=0;k<mapblockheight;k++) {
		for (j=0;j<mapblockwidth;j++) {
		switch (mapdepth) {
			case 8:
				putpixel (abmTiles[i], j, k, *((unsigned char *) newgfxpt));
				newgfxpt++;
				break;
			case 15:
			case 16:
				putpixel (abmTiles[i], j, k, *((unsigned short int *) newgfxpt));
				newgfxpt+=2;
				break;
			case 24:
				putpixel (abmTiles[i], j, k, makecol (newgfxpt[0], newgfxpt[1], newgfxpt[2]));
				newgfxpt+=3;
				break;
			case 32:
				putpixel (abmTiles[i], j, k, makecol (newgfxpt[1], newgfxpt[2], newgfxpt[3]));
				newgfxpt+=4;
				break;
		} } }
		if (is_video_bitmap(abmTiles[i])) release_bitmap (abmTiles[i]);
		i++;
	}
	i = mapnumblockstr; while (i) {
		myblkstrpt->bgoff = abmTiles[myblkstrpt->bgoff];
		
		if (myblkstrpt->fgoff!=0)
			myblkstrpt->fgoff = abmTiles[myblkstrpt->fgoff];
		if (myblkstrpt->fgoff2!=0)
			myblkstrpt->fgoff2 = abmTiles[myblkstrpt->fgoff2];
		if (myblkstrpt->fgoff3!=0)
			myblkstrpt->fgoff3 = abmTiles[myblkstrpt->fgoff3];
		myblkstrpt++; i--;
	}

	free (novcarray);
	return 0;
}

int MapRelocate (void)
{
int i, j, cdepth, pixcol, ccr, ccg, ccb;
//BLKSTR * myblkstrpt;
unsigned char * oldgfxpt;
unsigned char * mycmappt;
unsigned char * newgfxpt;
unsigned char * newgfx2pt;

	if (screen == NULL) { MapFreeMem (); maperror = MER_NOSCREEN; return -1; }
	if (!gfx_capabilities&GFX_HW_VRAM_BLIT && mapgfxinbitmaps==1)
		{ MapFreeMem (); maperror = MER_NOACCELERATION; return -1; }
		cdepth = bitmap_color_depth (screen);
		oldgfxpt = (unsigned char *) mapblockgfxpt;
		newgfxpt = (unsigned char *)
			malloc (mapblockwidth*mapblockheight*((mapdepth+1)/8)*mapnumblockgfx*((cdepth+1)/8));
		if (newgfxpt==NULL) { MapFreeMem (); maperror = MER_OUTOFMEM; return -1; }
		newgfx2pt = newgfxpt;
		mycmappt = (unsigned char *) mapcmappt; pixcol = 0;
		for (i=0;i<(mapblockwidth*mapblockheight*mapnumblockgfx);i++)
		{
			switch (mapdepth) {
			case 8:
				if (cdepth==8) pixcol = (int) *oldgfxpt; else {
				j = (*oldgfxpt)*3;
				pixcol = makecol (mycmappt[j], mycmappt[j+1], mycmappt[j+2]);
#ifdef RB8BITTOPINK
				if (j == 0 && cdepth!=8) pixcol = makecol (255, 0, 255); }
#endif
				oldgfxpt++;
				break;
			case 15:
				ccr = ((((int) *oldgfxpt)&0x7C)<<1);
				ccg = ((((((int) *oldgfxpt)&0x3)<<3)|(((int) *(oldgfxpt+1))>>5))<<3);
				ccb = (((int) *(oldgfxpt+1)&0x1F)<<3);
				ccr |= ((ccr>>5)&0x07);
				ccg |= ((ccg>>5)&0x07);
				ccb |= ((ccb>>5)&0x07);
				pixcol = makecol (ccr, ccg, ccb);
				if (cdepth==8) { if (ccr == 0xFF && ccg == 0 && ccb == 0xFF) pixcol = 0; }
				oldgfxpt += 2;
				break;
			case 16:
				ccr = (((int) *oldgfxpt)&0xF8);
				ccg = ((((((int) *oldgfxpt)&0x7)<<3)|(((int) *(oldgfxpt+1))>>5))<<2);
				ccb = (((int) *(oldgfxpt+1)&0x1F)<<3);
				ccr |= ((ccr>>5)&0x07);
				ccg |= ((ccg>>6)&0x03);
				ccb |= ((ccb>>5)&0x07);
				pixcol = makecol (ccr, ccg, ccb);
				if (cdepth==8) { if (ccr == 0xFF && ccg == 0 && ccb == 0xFF) pixcol = 0; }
				oldgfxpt += 2;
				break;
			case 24:
				pixcol = makecol (*oldgfxpt, *(oldgfxpt+1), *(oldgfxpt+2));
				if (cdepth==8) { if (*oldgfxpt == 0xFF && *(oldgfxpt+1) == 0 &&
					*(oldgfxpt+2) == 0xFF) pixcol = 0; }
				oldgfxpt += 3;
				break;
			case 32:
				pixcol = makecol (*(oldgfxpt+1), *(oldgfxpt+2), *(oldgfxpt+3));
				if (cdepth==8) { if (*(oldgfxpt+1) == 0xFF && *(oldgfxpt+2) == 0 &&
					*(oldgfxpt+3) == 0xFF) pixcol = 0; }
				oldgfxpt += 4;
				break;
			}
			switch (cdepth) {
			case 8:
				*newgfxpt = (unsigned char) pixcol;
				newgfxpt++;
				break;
			case 15:
			case 16:
				*((unsigned short int *) newgfxpt) = (unsigned short int) pixcol;
				newgfxpt+=2;
				break;
			case 24:
				*newgfxpt = (unsigned char) (pixcol>>16)&0xFF;
				*(newgfxpt+1) = (unsigned char) (pixcol>>8)&0xFF;
				*(newgfxpt+2) = (unsigned char) pixcol&0xFF;
				newgfxpt+=3;
				break;
			case 32:
				*newgfxpt = 0;
				*(newgfxpt+1) = (unsigned char) (pixcol>>16)&0xFF;
				*(newgfxpt+2) = (unsigned char) (pixcol>>8)&0xFF;
				*(newgfxpt+3) = (unsigned char) pixcol&0xFF;
				newgfxpt+=4;
				break;
			}
		}
		free (mapblockgfxpt); mapblockgfxpt = (char *) newgfx2pt;

	mapdepth = cdepth;

	return MapRelocate2 ();
}

static int MapGetchksz (unsigned char * locpt)
{
	return ((((int) (locpt[0]))<<24)|(((int) (locpt[1]))<<16)|
		(((int) (locpt[2]))<<8)|((int) (locpt[3])));
}

static int MapGetshort (unsigned char * locpt)
{
int rval;

	if (mapislsb)
	rval = ((((int) (locpt[1]))<<8)|((int) (locpt[0])));
	else
	rval = ((((int) (locpt[0]))<<8)|((int) (locpt[1])));
	if (rval & 0x8000) rval -= 0x10000;
	return rval;
}

static int MapGetlong (unsigned char * locpt)
{
	if (mapislsb)
	return ((((int) (locpt[3]))<<24)|(((int) (locpt[2]))<<16)|
		(((int) (locpt[1]))<<8)|((int) (locpt[0])));
	else
	return ((((int) (locpt[0]))<<24)|(((int) (locpt[1]))<<16)|
		(((int) (locpt[2]))<<8)|((int) (locpt[3])));
}

int MapDecodeMPHD (unsigned char * mdatpt)
{
	mdatpt += 8;
	if (mdatpt[0] > 1) { maperror = MER_MAPTOONEW; return -1; }
	if (mdatpt[2] == 1) mapislsb = 1; else mapislsb = 0;

	maptype = (int) mdatpt[3];
	if (maptype > 3) { maperror = MER_MAPTOONEW; return -1; }
	mapwidth = (short) MapGetshort (mdatpt+4);
	mapheight = (short) MapGetshort (mdatpt+6);
	mapblockwidth = (short) MapGetshort (mdatpt+12);
	mapblockheight = (short) MapGetshort (mdatpt+14);
	mapdepth = (short) MapGetshort (mdatpt+16);
	mapaltdepth = mapdepth;
	mapblockstrsize = (short) MapGetshort (mdatpt+18);
	mapnumblockstr = (short) MapGetshort (mdatpt+20);
	mapnumblockgfx = (short) MapGetshort (mdatpt+22);

	if (MapGetchksz (mdatpt-4) > 28) {
		mapblockgapx = (int) MapGetshort (mdatpt+28);
		mapblockgapy = (int) MapGetshort (mdatpt+30);
		mapblockstaggerx = (int) MapGetshort (mdatpt+32);
		mapblockstaggery = (int) MapGetshort (mdatpt+34);
	} else {
		mapblockgapx = (int) mapblockwidth;
		mapblockgapy = (int) mapblockheight;
		mapblockstaggerx = 0;
		mapblockstaggery = 0;
	}
	if (MapGetchksz (mdatpt-4) > 36) mapclickmask = (short) MapGetshort (mdatpt+36);
	else mapclickmask = 0;

	return 0;
}

/*
int DecodeEDHD (unsigned char * mdatpt)
{
int i, j;
short int * mybrshpt;
char * mynamept;
short int * mybrsh2pt;

	mdatpt += 8;
	xmapoffset = (short) MapGetshort (mdatpt);
	ymapoffset = (short) MapGetshort (mdatpt+2);
	fgcolour = (int) MapGetlong (mdatpt+4);
	bgcolour = (int) MapGetlong (mdatpt+8);
	swidth = (short) MapGetshort (mdatpt+12);
	sheight = (short) MapGetshort (mdatpt+14);
	strtstr = (short) MapGetshort (mdatpt+16);
	strtblk = (short) MapGetshort (mdatpt+18);
	curstr = (short) MapGetshort (mdatpt+20);
	curanim = (short) MapGetshort (mdatpt+22); curanim = -1;
	animspd = (short) MapGetshort (mdatpt+24);
	span = (short) MapGetshort (mdatpt+26);
	numbrushes = (short) MapGetshort (mdatpt+28);
	if (clickmask == 0) {
		clickmask = (int) MapGetshort (mdatpt+30);
		if (clickmask<0 || clickmask >= numblockgfx) clickmask = 0;
	}

	if (numbrushes>0)
	{
	mybrshpt =(short int *) (mdatpt+32);
		for (i=0;i<8;i++)
		{
			j = *mybrshpt; j *= *(mybrshpt+1); j *= 2; j += 4;
			brshpt[i] = malloc (j); j /= 2; mybrsh2pt = brshpt[i];
			*mybrsh2pt = *mybrshpt;
			mybrsh2pt++; mybrshpt++;
			*mybrsh2pt = *mybrshpt;
			mybrsh2pt++; mybrshpt++;
			j -= 2;
			if (maptype == 0) {
				while (j) {
					j--;
					*mybrsh2pt = *mybrshpt;
					if (*mybrsh2pt >= 0) *mybrsh2pt /= BLOCKSTRSIZE;
					mybrsh2pt++; mybrshpt++;
				}
			} else {
				while (j) {
					j--;
					*mybrsh2pt = *mybrshpt;
					if (*mybrsh2pt < 0) *mybrsh2pt *= sizeof(ANISTR);
					mybrsh2pt++; mybrshpt++;
				}
			}
			numbrushes--; if (!numbrushes) i=8;
		}
	mynamept = (char *) mybrshpt;
	if ((MapGetchksz (mdatpt-4)) > (mynamept-((char *) mdatpt))) {
		for (i=0;i<8;i++) {
			if (brshpt[i] != NULL) {
				strcpy (brshname[i], mynamept);
				mynamept += strlen (mynamept);
				mynamept++;
			}
		}
	}
	}

	return 0;
}

int DecodeATHR (unsigned char * mdatpt)
{
int i, j;

	mdatpt += 8;
	i = 0; while (i < MapGetchksz (mdatpt-4))
	{
		authorname[i]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo1[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo2[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo3[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	return 0;
}
*/

int MapDecodeCMAP (unsigned char * mdatpt)
{
	mdatpt += 8;
	mapcmappt = (unsigned char *) malloc (MapGetchksz (mdatpt-4));
	if (mapcmappt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapcmappt, mdatpt, MapGetchksz (mdatpt-4));
	Mapconv8to6pal (mapcmappt);
	return 0;
}

int MapDecodeBKDT (unsigned char * mdatpt)
{
int i, j;
BLKSTR * myblkpt;

	mdatpt += 8;
	mapblockstrpt = malloc (mapnumblockstr*sizeof(BLKSTR));
	if (mapblockstrpt==NULL) { maperror = MER_OUTOFMEM; return -1; }

	myblkpt = (BLKSTR *) mapblockstrpt;
	j = MapGetchksz (mdatpt-4);
	i = 0; while (i < (mapnumblockstr*mapblockstrsize)) {
		myblkpt->bgoff = (int) MapGetlong (mdatpt+i);
		myblkpt->fgoff = (int) MapGetlong (mdatpt+i+4);
		myblkpt->fgoff2 = (int) MapGetlong (mdatpt+i+8);
		myblkpt->fgoff3 = (int) MapGetlong (mdatpt+i+12);
		if (maptype == 0) {
			myblkpt->bgoff /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
			myblkpt->fgoff /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
			myblkpt->fgoff2 /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
			myblkpt->fgoff3 /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
		}
		myblkpt->user1 = (unsigned int) MapGetlong (mdatpt+i+16);
		myblkpt->user2 = (unsigned int) MapGetlong (mdatpt+i+20);
		myblkpt->user3 = (unsigned short int) MapGetshort (mdatpt+i+24);
		myblkpt->user4 = (unsigned short int) MapGetshort (mdatpt+i+26);
		myblkpt->user5 = mdatpt[i+28];
		myblkpt->user6 = mdatpt[i+29];
		myblkpt->user7 = mdatpt[i+30];
		if (mdatpt[i+31]&0x80) myblkpt->unused3 = 1; else myblkpt->unused3 = 0;
		if (mdatpt[i+31]&0x40) myblkpt->unused2 = 1; else myblkpt->unused2 = 0;
		if (mdatpt[i+31]&0x20) myblkpt->unused1 = 1; else myblkpt->unused1 = 0;
		if (mdatpt[i+31]&0x10) myblkpt->trigger = 1; else myblkpt->trigger = 0;
		if (mdatpt[i+31]&0x08) myblkpt->br = 1; else myblkpt->br = 0;
		if (mdatpt[i+31]&0x04) myblkpt->bl = 1; else myblkpt->bl = 0;
		if (mdatpt[i+31]&0x02) myblkpt->tr = 1; else myblkpt->tr = 0;
		if (mdatpt[i+31]&0x01) myblkpt->tl = 1; else myblkpt->tl = 0;
		i += mapblockstrsize;
		myblkpt++;
	}
	return 0;
}

int MapDecodeANDT (unsigned char * mdatpt)
{
int numani, i, ancksz;
unsigned char * mdatendpt;

	mdatpt += 8;
	ancksz = MapGetchksz(mdatpt-4);
	mdatendpt = mdatpt+ancksz;

	numani = 0; while (1) {
		mdatendpt -= 16;
		numani++;
		if (*mdatendpt == 255) break;
	}

	mapanimseqpt = malloc (((mdatendpt-mdatpt)/4)*sizeof(int));
	if (mapanimseqpt == NULL) { maperror = MER_OUTOFMEM; return -1; }
	i = 0; while (mdatpt != mdatendpt) {
		mapanimseqpt[i] = MapGetlong (mdatpt);
		if (maptype == 0) mapanimseqpt[i] /= mapblockstrsize;
		mdatpt += 4; i++;
	}

	mapanimstrpt = malloc (numani*sizeof(ANISTR));
	if (mapanimstrpt == NULL) { maperror = MER_OUTOFMEM; return -1; }
	mapanimstrendpt = mapanimstrpt;
	mapanimstrendpt += numani;

	i = 0; while (i<numani) {
		mapanimstrpt[i].antype = mdatendpt[0];
		mapanimstrpt[i].andelay = mdatendpt[1];
		mapanimstrpt[i].ancount = mdatendpt[2];
		mapanimstrpt[i].anuser = mdatendpt[3];
		if (maptype == 0) {
			mapanimstrpt[i].ancuroff = (int) ((MapGetlong (mdatendpt+4)+ancksz)/4);
			mapanimstrpt[i].anstartoff = (int) ((MapGetlong (mdatendpt+8)+ancksz)/4);
			mapanimstrpt[i].anendoff = (int) ((MapGetlong (mdatendpt+12)+ancksz)/4);
		} else {
			mapanimstrpt[i].ancuroff = (int) MapGetlong (mdatendpt+4);
			mapanimstrpt[i].anstartoff = (int) MapGetlong (mdatendpt+8);
			mapanimstrpt[i].anendoff = (int) MapGetlong (mdatendpt+12);
		}
		mdatendpt += 16; i++;
	}

	MapInitAnims ();
	return 0;
}

int MapDecodeAGFX (unsigned char * mdatpt)
{
	if (bitmap_color_depth (screen) > 8) return 0;
	if (mapblockgfxpt != NULL) free (mapblockgfxpt);
	mapblockgfxpt = malloc (MapGetchksz (mdatpt+4));
	if (mapblockgfxpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapblockgfxpt, mdatpt+8, MapGetchksz(mdatpt+4));
	mapaltdepth = 8;
	return 0;
}

int MapDecodeBGFX (unsigned char * mdatpt)
{
	if (mapblockgfxpt != NULL) return 0;
	mapblockgfxpt = malloc (MapGetchksz (mdatpt+4));
	if (mapblockgfxpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapblockgfxpt, mdatpt+8, MapGetchksz(mdatpt+4));
	return 0;
}

int MapDecodeNOVC (unsigned char * mdatpt)
{
	memset (mapnovctext, 0, 70);
	if (MapGetchksz (mdatpt+4) < 70) strcpy (mapnovctext, mdatpt+8);
	return 0;
}

int MapDecodeLayer (unsigned char * mdatpt, int lnum)
{
int i, j, k, l;
short int * mymappt, * mymap2pt;

	mapmappt[lnum] = malloc (mapwidth*mapheight*sizeof(short int));
	if (mapmappt[lnum] == NULL) { maperror = MER_OUTOFMEM; return -1; }

	mdatpt += 8;
	mymappt = mapmappt[lnum];
	if (maptype == 0) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;i++) {
			*mymappt = (short int) MapGetshort (mdatpt);
			if (*mymappt >= 0) { *mymappt /= mapblockstrsize; }
			else { *mymappt /= 16; }
			mdatpt+=2; mymappt++;
		}
	}
	} else {
	if (maptype == 1) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;i++) {
			*mymappt = (short int) MapGetshort (mdatpt);
			mdatpt+=2; mymappt++;
		}
	}
	} else {
	if (maptype == 2) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;) {
			k = (int) MapGetshort (mdatpt);
			mdatpt += 2;
			if (k > 0) {
				while (k) {
					*mymappt = (short int) MapGetshort (mdatpt);
					mymappt++; mdatpt += 2;
					i++; k--;
				}
			} else {
			if (k < 0) {
				l = (int) MapGetshort (mdatpt); mdatpt += 2;
				while (k) {
					*mymappt = (short int) l;
					mymappt++;
					i++; k++;
				}
			} else {
			} }
		}
	}
	} else {
	if (maptype == 3) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;) {
			k = (int) MapGetshort (mdatpt);
			mdatpt += 2;
			if (k > 0) {
				while (k) {
					*mymappt = (short int) MapGetshort (mdatpt);
					mymappt++; mdatpt += 2;
					i++; k--;
				}
			} else {
			if (k < 0) {
				mymap2pt = mymappt + (int) MapGetshort (mdatpt); mdatpt += 2;
				while (k) {
					*mymappt = *mymap2pt;
					mymappt++; mymap2pt++;
					i++; k++;
				}
			} else {
			} }
		}
	}
	} } } }

	if (lnum == 0) { mappt = mapmappt[lnum]; }
	return 0;
}

int MapDecodeNULL (unsigned char * mdatpt)
{
	return 0;
}

int MapRealDecode (PACKFILE * mfpt, unsigned char * mmpt, long int mpfilesize)
{
int chkdn;
unsigned char * fmappospt;
char mphdr[8];

	MapFreeMem ();
	mpfilesize -= 12;

	while (mpfilesize > 0) {

		if (mfpt != NULL) {
			if (pack_fread (mphdr, 8, mfpt) != 8) {
				maperror = MER_MAPLOADERROR;
				MapFreeMem ();
				return -1;
			}
			fmappospt = malloc (MapGetchksz(mphdr+4)+8);
			if (fmappospt == NULL) {
				maperror = MER_OUTOFMEM;
				MapFreeMem ();
				return -1;
			}
			memcpy (fmappospt, mphdr, 8);
			if (pack_fread (fmappospt+8, MapGetchksz(mphdr+4), mfpt) != MapGetchksz(mphdr+4)) {
				maperror = MER_MAPLOADERROR;
				MapFreeMem ();
				return -1;
			}
		} else {
			fmappospt = mmpt;
			mmpt += MapGetchksz(mmpt+4);
			mmpt += 8;
		}

		chkdn = 0;
		if (!strncmp (fmappospt, "MPHD", 4)) { chkdn = 1; MapDecodeMPHD (fmappospt); }
/*		if (!strncmp (fmappospt, "ATHR", 4)) { chkdn = 1; MapDecodeATHR (fmappospt); }
		if (!strncmp (fmappospt, "EDHD", 4)) { chkdn = 1; MapDecodeEDHD (fmappospt); }
*/
		if (!strncmp (fmappospt, "CMAP", 4)) { chkdn = 1; MapDecodeCMAP (fmappospt); }
		if (!strncmp (fmappospt, "BKDT", 4)) { chkdn = 1; MapDecodeBKDT (fmappospt); }
		if (!strncmp (fmappospt, "ANDT", 4)) { chkdn = 1; MapDecodeANDT (fmappospt); }
		if (!strncmp (fmappospt, "AGFX", 4)) { chkdn = 1; MapDecodeAGFX (fmappospt); }
		if (!strncmp (fmappospt, "BGFX", 4)) { chkdn = 1; MapDecodeBGFX (fmappospt); }
		if (!strncmp (fmappospt, "NOVC", 4)) { chkdn = 1; MapDecodeNOVC (fmappospt); }
		if (!strncmp (fmappospt, "BODY", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 0); }
		if (!strncmp (fmappospt, "LYR1", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 1); }
		if (!strncmp (fmappospt, "LYR2", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 2); }
		if (!strncmp (fmappospt, "LYR3", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 3); }
		if (!strncmp (fmappospt, "LYR4", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 4); }
		if (!strncmp (fmappospt, "LYR5", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 5); }
		if (!strncmp (fmappospt, "LYR6", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 6); }
		if (!strncmp (fmappospt, "LYR7", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 7); }
		if (!chkdn) MapDecodeNULL (fmappospt);

		mpfilesize -= 8;
		mpfilesize -= MapGetchksz (fmappospt+4);
		if (mfpt != NULL) free (fmappospt);

		if (maperror != MER_NONE) { MapFreeMem (); return -1; }
	}

	mapdepth = mapaltdepth;
	return MapRelocate ();
}

int MapRealLoad (char * mname)
{
int mretval;
char idtag[4];
unsigned char tempc;
int mapfilesize = 0;

	maperror = MER_NONE;
	mapfilept = pack_fopen (mname, "rp");
	if (mapfilept==NULL) { mapfilept = pack_fopen (mname, "r");
		if (mapfilept==NULL) { maperror = MER_NOOPEN; return -1; } }

	maperror = MER_MAPLOADERROR;
	if (pack_fread (&idtag[0], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[1], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[2], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[3], 1, mapfilept) == 1) {
	if (pack_fread (&tempc, 1, mapfilept) == 1) {
	mapfilesize = (((int) tempc)<<24);
	if (pack_fread (&tempc, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc)<<16);
	if (pack_fread (&tempc, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc)<<8);
	if (pack_fread (&tempc, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc));
	mapfilesize += 8;
	if (!strncmp (idtag, "FORM", 4)) {
	if (pack_fread (&idtag[0], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[1], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[2], 1, mapfilept) == 1) {
	if (pack_fread (&idtag[3], 1, mapfilept) == 1) {
	if (!strncmp (idtag, "FMAP", 4)) maperror = MER_NONE;
	} } } } }
	} } } } } } } }

	if (maperror != MER_NONE) { pack_fclose (mapfilept); return -1; }

	mretval = MapRealDecode (mapfilept, NULL, mapfilesize);
	pack_fclose (mapfilept);

	return mretval;
}

int MapLoad (char * mapname)
{
	mapgfxinbitmaps = 2;
	return MapRealLoad (mapname);
}

int MapLoadVRAM (char * mapname)
{
	mapgfxinbitmaps = 1;
	return MapRealLoad (mapname);
}

int MapLoadABM (char * mapname)
{
	mapgfxinbitmaps = 2;
	return MapRealLoad (mapname);
}

int MapPreRealDecode (unsigned char * mapmempt)
{
long int maplength;

	MapFreeMem ();
	maperror = 0;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='O') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='R') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='M') maperror = MER_MAPLOADERROR;
	mapmempt += 4;
	maplength = (MapGetchksz (mapmempt))+8;

	if (maperror) return -1;
	mapmempt += 4;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='M') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='A') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='P') maperror = MER_MAPLOADERROR;
	mapmempt+=4;

	if (maperror) return -1;
	return MapRealDecode (NULL, mapmempt, maplength);
}

int MapDecode (unsigned char * mapmempt)
{
	mapgfxinbitmaps = 2;
	return MapPreRealDecode (mapmempt);
}

int MapDecodeVRAM (unsigned char * mapmempt)
{
	mapgfxinbitmaps = 1;
	return MapPreRealDecode (mapmempt);
}

int MapDecodeABM (unsigned char * mapmempt)
{
	mapgfxinbitmaps = 2;
	return MapPreRealDecode (mapmempt);
}

BITMAP * MapMakeParallaxBitmap (BITMAP * sourcebm, int style)
{
BITMAP * newbm;

	if (mappt == NULL) return NULL;
	if (style < 0 || style > 1) return NULL;

	if (style) newbm = create_video_bitmap (sourcebm->w+mapblockwidth, sourcebm->h+mapblockheight);
	else newbm = create_bitmap (sourcebm->w+mapblockwidth, sourcebm->h+mapblockheight);

	if (newbm == NULL) return NULL;
	blit (sourcebm, newbm, 0, 0, 0, 0, sourcebm->w, sourcebm->h);
	blit (sourcebm, newbm, 0, 0, 0, sourcebm->h, sourcebm->w, mapblockheight);
	blit (newbm, newbm, 0, 0, sourcebm->w, 0, mapblockwidth, sourcebm->h+mapblockheight);
	return newbm;
}

void MapDrawParallax (BITMAP * mapdestpt, BITMAP * parbm, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph)
/* mapdestpt = standard allegro bitmap, MEMORY or VIDEO bitmap.
 * parbm = standard allegro bitmap. MEMORY or VIDEO bitmap.
 * mapxo = offset, in pixels, from the left edge of the map.
 * mapyo = offset, in pixels, from the top edge of the map.
 * mapx  = offset, in pixels, from the left edge of the BITMAP.
 * mapy  = offset, in pixels, from the top edge of the BITMAP.
 * mapw  = width, in pixels, of drawn area.
 * maph  = height, in pixels, of drawn area.
 */
{
int mycl, mycr, myct, mycb;
int i, i2, j;
int paraxo, paraxo2, parayo;
short int * mymappt, * mymappt2;
BLKSTR * blkdatapt;
ANISTR * myanpt;

	if (mapblockstaggerx || mapblockstaggery) return;
	mycl = mapdestpt->cl;
	mycr = mapdestpt->cr;
	myct = mapdestpt->ct;
	mycb = mapdestpt->cb;
	set_clip (mapdestpt, mapx, mapy, mapx+mapw-1, mapy+maph-1);

	mymappt = (short int *) mappt;
	mymappt += (mapxo/mapblockwidth)+((mapyo/mapblockheight)*mapwidth);

	paraxo = ((mapxo-(mapxo%mapblockwidth))%(parbm->w-mapblockwidth))-((mapxo/2)%(parbm->w-mapblockwidth));
	parayo = ((mapyo-(mapyo%mapblockheight))%(parbm->h-mapblockheight))-((mapyo/2)%(parbm->h-mapblockheight));
	while (paraxo < 0) paraxo += (parbm->w-mapblockwidth);
	while (parayo < 0) parayo += (parbm->h-mapblockheight);

	i = mapx-(mapxo%mapblockwidth);
	j = mapy-(mapyo%mapblockheight);

	i2 = i; paraxo2 = paraxo; mymappt2 = mymappt;
	while (j < (mapy+maph)) {
		while (i < (mapx+mapw)) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (blkdatapt->trigger)
				blit (parbm, mapdestpt, paraxo, parayo, i, j, mapblockwidth, mapblockheight);
			paraxo += mapblockwidth;
			if (paraxo >= (parbm->w-mapblockwidth)) paraxo -= (parbm->w-mapblockwidth);
			i += mapblockwidth; mymappt++;
		}
		parayo += mapblockheight;
		if (parayo >= (parbm->h-mapblockheight)) parayo -= (parbm->h-mapblockheight);
		i = i2; paraxo = paraxo2; mymappt2 += mapwidth; mymappt = mymappt2;
		j += mapblockheight;
	}
	set_clip (mapdestpt, mycl, myct, mycr+1, mycb+1);
}

void MapDrawBG (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph)
{
int i, j, mycl, mycr, myct, mycb, mapvclip, maphclip;
int mbgx, mbgy;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;

	if (!mapgfxinbitmaps) {
		return;
	} else {
		mycl = mapdestpt->cl;
		mycr = mapdestpt->cr;
		myct = mapdestpt->ct;
		mycb = mapdestpt->cb;
		set_clip (mapdestpt, mapx, mapy, mapx+mapw-1, mapy+maph-1);
		mapxo -= mapblockstaggerx;
		mapyo -= mapblockstaggery;
		mymappt = (short int *) mappt;
		if (mapblockstaggerx || mapblockstaggery) {
			mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth*2);
			mbgx = mapblockgapx;
			mbgy = mapblockgapy;
		} else {
			mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);
			mbgx = 0;
			mbgy = 0;
		}
		mapvclip = mapyo%mapblockgapy;
		maphclip = mapxo%mapblockgapx;

		mymap2pt = mymappt;
		for (j=((mapy-mapvclip)-mbgy);j<((mapy+maph));j+=mapblockgapy) {
		for (i=((mapx-maphclip)-mbgx);i<((mapx+mapw));i+=mapblockgapx) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (mapblockstaggerx || mapblockstaggery) {
			if (abmTiles[0] != (BITMAP *) blkdatapt->bgoff)
				masked_blit ((BITMAP *) blkdatapt->bgoff, mapdestpt, 0, 0, i, j, mapblockwidth, mapblockheight);
			} else {
				blit ((BITMAP *) blkdatapt->bgoff, mapdestpt, 0, 0, i, j, mapblockwidth, mapblockheight);
			}
			mymappt++;
		}
		if (mapblockstaggerx || mapblockstaggery) {
			mymap2pt += mapwidth;
			mymappt = mymap2pt;
			for (i=(((mapx-maphclip)-mbgx)+mapblockstaggerx);i<((mapx+mapw));i+=mapblockgapx) {
				if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
				else { myanpt = mapanimstrendpt + *mymappt;
					blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
				if (abmTiles[0] != (BITMAP *) blkdatapt->bgoff)
					masked_blit ((BITMAP *) blkdatapt->bgoff, mapdestpt, 0, 0, i, j+mapblockstaggery, mapblockwidth, mapblockheight);
				mymappt++;
			}
		}
		mymap2pt += mapwidth;
		mymappt = mymap2pt;
		}

		set_clip (mapdestpt, mycl, myct, mycr+1, mycb+1);
	}
}

void MapDrawBGT (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph)
{
int i, j, mycl, mycr, myct, mycb, mapvclip, maphclip;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;

	if (mapblockstaggerx || mapblockstaggery) {
		MapDrawBG (mapdestpt, mapxo, mapyo, mapx, mapy, mapw, maph);
		return;
	}
	if (!mapgfxinbitmaps) {
		return;
	} else {
		mycl = mapdestpt->cl;
		mycr = mapdestpt->cr;
		myct = mapdestpt->ct;
		mycb = mapdestpt->cb;
		set_clip (mapdestpt, mapx, mapy, mapx+mapw-1, mapy+maph-1);
		mymappt = (short int *) mappt;
		mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);
		mapvclip = mapyo%mapblockgapy;
		maphclip = mapxo%mapblockgapx;

		mymap2pt = mymappt;
		for (j=(mapy-mapvclip);j<((mapy+maph));j+=mapblockgapy) {
		for (i=(mapx-maphclip);i<((mapx+mapw));i+=mapblockgapx) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (blkdatapt->trigger) {
			if (abmTiles[0] != (BITMAP *) blkdatapt->bgoff)
				masked_blit ((BITMAP *) blkdatapt->bgoff, mapdestpt, 0, 0, i, j, mapblockwidth, mapblockheight);
			} else {
				blit ((BITMAP *) blkdatapt->bgoff, mapdestpt, 0, 0, i, j, mapblockwidth, mapblockheight);
			}
			mymappt++;
		}
		mymap2pt += mapwidth;
		mymappt = mymap2pt;
		}

		set_clip (mapdestpt, mycl, myct, mycr+1, mycb+1);
	}
}

void MapDrawFG (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph, int mapfg)
{
int i, j, mycl, mycr, myct, mycb, mapvclip, maphclip;
int mbgx, mbgy;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;
BITMAP *mapgfxpt;

	if (!mapgfxinbitmaps) {
		return;
	} else {
		mycl = mapdestpt->cl;
		mycr = mapdestpt->cr;
		myct = mapdestpt->ct;
		mycb = mapdestpt->cb;
		set_clip (mapdestpt, mapx, mapy, mapx+mapw-1, mapy+maph-1);
		mapxo -= mapblockstaggerx;
		mapyo -= mapblockstaggery;
		mymappt = (short int *) mappt;
		if (mapblockstaggerx || mapblockstaggery) {
			mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth*2);
			mbgx = mapblockgapx;
			mbgy = mapblockgapy;
		} else {
			mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);
			mbgx = 0;
			mbgy = 0;
		}
		mapvclip = mapyo%mapblockgapy;
		maphclip = mapxo%mapblockgapx;

		mymap2pt = mymappt;
		for (j=((mapy-mapvclip)-mbgy);j<((mapy+maph));j+=mapblockgapy) {
		for (i=((mapx-maphclip)-mbgx);i<((mapx+mapw));i+=mapblockgapx) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (!mapfg) mapgfxpt = (BITMAP *) blkdatapt->fgoff;
			else if (mapfg == 1) mapgfxpt = (BITMAP *) blkdatapt->fgoff2;
			else mapgfxpt = (BITMAP *) blkdatapt->fgoff3;
			if (((int)mapgfxpt) != 0)
				masked_blit (mapgfxpt, mapdestpt, 0, 0, i, j, mapblockwidth, mapblockheight);
			mymappt++;
		}
		if (mapblockstaggerx || mapblockstaggery) {
		mymap2pt += mapwidth;
		mymappt = mymap2pt;
		for (i=(((mapx-maphclip)-mbgx)+mapblockstaggerx);i<((mapx+mapw));i+=mapblockgapx) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (!mapfg) mapgfxpt = (BITMAP *) blkdatapt->fgoff;
			else if (mapfg == 1) mapgfxpt = (BITMAP *) blkdatapt->fgoff2;
			else mapgfxpt = (BITMAP *) blkdatapt->fgoff3;
			if (((int)mapgfxpt) != 0)
				masked_blit (mapgfxpt, mapdestpt, 0, 0, i, j+mapblockstaggery, mapblockwidth, mapblockheight);
			mymappt++;
		}
		}
		mymap2pt += mapwidth;
		mymappt = mymap2pt;
		}

		set_clip (mapdestpt, mycl, myct, mycr+1, mycb+1);
	}
}

void MapDrawRow (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph, int maprw, void (*cellcall) (int cx, int cy, int dx, int dy))
{
int i, j, mycl, mycr, myct, mycb, mapvclip, maphclip;
int mbgx, mbgy, bfield, bysub;
int cx, cy;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;
BITMAP *mapgfxpt;

	if (((mapyo/mapblockgapy)+maprw) >= mapheight) return;
	if (mapblockstaggerx || mapblockstaggery) {
		mapxo -= mapblockstaggerx;
		mapyo -= mapblockstaggery;
		if ((((mapyo/mapblockgapy)*2)+maprw) >= (mapheight-1)) return;
	}
	if (!mapgfxinbitmaps) {
		return;
	} else {
		mycl = mapdestpt->cl;
		mycr = mapdestpt->cr;
		myct = mapdestpt->ct;
		mycb = mapdestpt->cb;
		set_clip (mapdestpt, mapx, mapy, mapx+mapw-1, mapy+maph-1);
		mymappt = (short int *) mappt;
		mapvclip = mapyo%mapblockgapy;
		maphclip = mapxo%mapblockgapx;
		j = (mapy-mapvclip); i = 0;
		if (mapblockstaggerx || mapblockstaggery) {
			cx = mapxo/mapblockgapx;
			cy = (((mapyo/mapblockgapy)*2)+maprw);
			mymappt += (cx)+(cy*mapwidth);
			mbgx = mapblockgapx;
			mbgy = mapblockgapy;
			j -= mbgy;
			j += ((maprw/2)*mapblockgapy);
			if (maprw&1) { j += mapblockstaggery; i = mapblockstaggerx; }
		} else {
			cx = mapxo/mapblockgapx;
			cy = ((mapyo/mapblockgapy)+maprw);
			mymappt += (cx)+(cy*mapwidth);
			mbgx = 0;
			mbgy = 0;
			j += (maprw*mapblockgapy);
		}

		mymap2pt = mymappt;
		for (i+=((mapx-maphclip)-mbgx);i<((mapx+mapw));i+=mapblockgapx) {
			if (cellcall != NULL) cellcall (cx, cy, i, j);
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			bfield = 1; bysub = 0;
			do {
			if (!bfield) blkdatapt++;
			for (;bfield<4;bfield++) {
			switch (bfield) {
				case 0: mapgfxpt = (BITMAP *) blkdatapt->bgoff; break;
				case 1: mapgfxpt = (BITMAP *) blkdatapt->fgoff; break;
				case 2: mapgfxpt = (BITMAP *) blkdatapt->fgoff2; break;
				default:
				case 3: mapgfxpt = (BITMAP *) blkdatapt->fgoff3; break;
			}
			if (((int)mapgfxpt) != 0) {
				if (blkdatapt->unused2 && !blkdatapt->unused3) {
					masked_blit (mapgfxpt, mapdestpt, 0, 0, i, j-bysub, mapblockwidth/2, mapblockheight);
				} else {
					if (!blkdatapt->unused2 && blkdatapt->unused3) {
						masked_blit (mapgfxpt, mapdestpt, mapblockwidth/2, 0, i, j-bysub, mapblockwidth/2, mapblockheight);
					} else {
						masked_blit (mapgfxpt, mapdestpt, 0, 0, i, j-bysub, mapblockwidth, mapblockheight);
					}
				}
			}
			bysub += mapblockheight;
			}
			bfield = 0;
			} while (blkdatapt->unused1);
			mymappt++; cx++;
		}

		set_clip (mapdestpt, mycl, myct, mycr+1, mycb+1);
	}
}
