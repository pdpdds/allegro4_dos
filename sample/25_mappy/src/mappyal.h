/* Header file for mappyAL V1.0 */
/* (C)2001 Robin Burrows  -  rburrows@bigfoot.com */

#ifdef __cplusplus
extern "C" {
#endif

#define MER_NONE 0		/* All the horrible things that can go wrong */
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3
#define MER_NOSCREEN 4
#define MER_NOACCELERATION 5
#define MER_CVBFAILED 6

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

/* Obsolete functions */
#define MapDraw8BG MapDrawBG
#define MapDraw8BGT MapDrawBGT
#define MapDraw8FG MapDrawFG
#define MapDraw15BG MapDrawBG
#define MapDraw15BGT MapDrawBGT
#define MapDraw15FG MapDrawFG
#define MapDraw16BG MapDrawBG
#define MapDraw16BGT MapDrawBGT
#define MapDraw16FG MapDrawFG
#define MapDraw32BG MapDrawBG
#define MapDraw32BGT MapDrawBGT
#define MapDraw32FG MapDrawFG

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


/* All global variables used bt Mappy playback are here */
extern int maperror;		/* Set to a MER_ error if something wrong happens */
extern short int mapwidth, mapheight, mapblockwidth, mapblockheight, mapdepth;
extern short int mapblockstrsize, mapnumblockstr, mapnumblockgfx;
extern short int * mappt;
extern short int ** maparraypt;
extern char * mapcmappt;
extern char * mapblockgfxpt;
extern char * mapblockstrpt;
extern ANISTR * mapanimstrpt;
extern ANISTR * mapanimstrendpt;
extern RGB * mapcmap6bit;
extern short int ** mapmappt;
extern short int *** mapmaparraypt;
extern BITMAP ** abmTiles;
extern int mapblocksinvidmem, mapblocksinsysmem;
extern int mapblockgapx, mapblockgapy;
extern int mapblockstaggerx, mapblockstaggery;
/* End of Mappy globals */

void Mapconv8to6pal (unsigned char *);
void MapFreeMem (void);
void MapSetPal8 (void);
void MapCorrectColours (void);
int MapRelocate (void);
int MapLoad (char *);
int MapLoadVRAM (char *);
int MapLoadABM (char *);
int MapDecode (unsigned char *);
int MapDecodeVRAM (unsigned char *);
int MapDecodeABM (unsigned char *);
int MapLoadMAR (char *, int);
int MapDecodeMAR (unsigned char *, int);
int MapGetBlockID (int, int);
int MapGenerateYLookup (void);
int MapChangeLayer (int);
int MapGetXOffset (int, int);
int MapGetYOffset (int, int);
BLKSTR * MapGetBlockInPixels (int, int);
BLKSTR * MapGetBlock (int, int);
void MapSetBlockInPixels (int, int, int);
void MapSetBlock (int, int, int);
void MapRestore (void);
void MapInitAnims (void);
void MapUpdateAnims (void);
void MapDrawBG (BITMAP *, int, int, int, int, int, int);
void MapDrawBGT (BITMAP *, int, int, int, int, int, int);
void MapDrawFG (BITMAP *, int, int, int, int, int, int, int);
void MapDrawRow (BITMAP *, int, int, int, int, int, int, int, void (*cellcall) (int cx, int cy, int dx, int dy));
BITMAP * MapMakeParallaxBitmap (BITMAP *, int);
void MapDrawParallax (BITMAP *, BITMAP *, int, int, int, int, int, int);

#ifdef __cplusplus
}
#endif
