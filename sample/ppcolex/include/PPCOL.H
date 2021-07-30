#ifndef PPCOL_H
#define PPCOL_H
#ifndef ALLEGRO_H
#include <allegro.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

//Lets define some *maybe* usefull macros:
#define PPCOL_VERSION          1
#define PPCOL_SUB_VERSION      4
#define PPCOL_VERSION_STR      "1.4"
#define PPCOL_DATE_STR         "1998"

//Pixel perfect collision sprite mask structure:
typedef struct PPCOL_MASK
{
 int h;                          //Bounding box height.
 int w;                          //Bounding box width.
 int max_chunk;                  //This determines the total number of 32bits chunks - 1.
 void *dat;                      //The mask data pointer.
 unsigned long int *sp_mask[0];  //Pointers to the begining of each row of chunks.
 } PPCOL_MASK;


//This is the bounding box collision detection macro.
//It is a general purpose macro. Pass it the coordinates of one rectangle and the width and
//height of it, then pass the coordinates of a second rectangle and the width and height of
//it. It will return 0 if theres not collision or 1 if there is a collision between the
//rectangles (the rectangles overlap).
//This macro works looking for out of range values, if some value is out of
//range it returns 0, if all the values are in range it returns true.
#define check_bb_collision_general(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                                ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

//This macro checks for collision between bounding boxes of 2 masks or 2 Allegro bitmaps.
//Pass it a 2 proper initialized PPCOL_MASK's or Allegro BITMAPS and the X, Y positions of
//each one respectively.
//Returns 0 if theres not collision, other value if there is collision.
#define check_bb_collision(mask1,mask2,x1,y1,x2,y2) check_bb_collision_general(x1,y1,mask1->w,mask1->h,x2,y2,mask2->w,mask2->h)

//Bitmask method routines prototypes, these are in ppmaskc.c:
extern PPCOL_MASK *create_ppcol_mask(BITMAP *sprite);
extern int check_ppmask_collision_normal(PPCOL_MASK *mask1, PPCOL_MASK *mask2, int x1, int y1, int x2, int y2);
extern void destroy_ppcol_mask(PPCOL_MASK *mask);
//Bitmap method routine prototype, it is in ppcol.c:
extern int check_pp_collision_normal(BITMAP *spr1, BITMAP *spr2, int x1, int y1, int x2, int y2);

//Collision amount functions, these are in ppamount.c and ppmamoun.c:
#define PPCAMOUNT_UP            0,-1
#define PPCAMOUNT_UP_RIGHT      1,-1
#define PPCAMOUNT_RIGHT         1, 0
#define PPCAMOUNT_DOWN_RIGHT    1, 1
#define PPCAMOUNT_DOWN          0, 1
#define PPCAMOUNT_DOWN_LEFT    -1, 1
#define PPCAMOUNT_LEFT         -1, 0
#define PPCAMOUNT_UP_LEFT      -1,-1
extern int check_pp_collision_amount_normal(BITMAP *spr1, BITMAP *spr2, int x1, int y1, int x2, int y2, int addx, int addy);
extern int check_ppmask_collision_amount_normal(PPCOL_MASK *mask1, PPCOL_MASK *mask2, int x1, int y1, int x2, int y2, int addx, int addy);


//Here comes the debugging support:
#ifdef PPCOL_DEBUG
#define destroy_ppcol_mask(mask) destroy_ppcol_mask(mask); mask=NULL;
int check_ppmask_collision(PPCOL_MASK *mask1, PPCOL_MASK *mask2, int x1, int y1, int x2, int y2)
{
 if(mask1==NULL || mask2==NULL) {
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   fprintf(stderr,"Passed NULL mask pointer to check_ppmask_collision()\nAborting!!!\n\n");
   __asm__ __volatile__ ("movb $0x1b,%al;call ___djgpp_hw_exception"); //This forces the display of the traceback.
   } else {
   return check_ppmask_collision_normal(mask1,mask2,x1,y1,x2,y2);
   }
 return 0; //This will never be reached, but the compiler complains if we not put it here.
 }
int check_pp_collision(BITMAP *spr1, BITMAP *spr2, int x1, int y1, int x2, int y2)
{
 if(spr1==NULL || spr2==NULL) {
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   fprintf(stderr,"Passed NULL bitmap pointer to check_pp_collision()\nAborting!!!\n\n");
   __asm__ __volatile__ ("movb $0x1b,%al;call ___djgpp_hw_exception"); //This forces the display of the traceback.
   } else {
   return check_pp_collision_normal(spr1,spr2,x1,y1,x2,y2);
   }
 return 0; //This will never be reached, but the compiler complains if we not put it here.
 }
int check_pp_collision_amount(BITMAP *spr1, BITMAP *spr2, int x1, int y1, int x2, int y2, int addx, int addy)
{
 if(spr1==NULL || spr2==NULL || (addx!=-1 && addx!=0 && addx!=1) || (addy!=-1 && addy!=0 && addy!=1)) {
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   fprintf(stderr,"Passed NULL bitmap pointer or invalid direction to\ncheck_pp_collision_amount()\nAborting!!!\n\n");
   __asm__ __volatile__ ("movb $0x1b,%al;call ___djgpp_hw_exception"); //This forces the display of the traceback.
   } else {
   return check_pp_collision_amount_normal(spr1,spr2,x1,y1,x2,y2,addx,addy);
   }
 return 0; //This will never be reached, but the compiler complains if we not put it here.
 }
int check_ppmask_collision_amount(PPCOL_MASK *mask1, PPCOL_MASK *mask2, int x1, int y1, int x2, int y2, int addx, int addy)
{
 if(mask1==NULL || mask2==NULL || (addx!=-1 && addx!=0 && addx!=1) || (addy!=-1 && addy!=0 && addy!=1)) {
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   fprintf(stderr,"Passed NULL mask pointer or invalid direction to\ncheck_ppmask_collision_amount()\nAborting!!!\n\n");
   __asm__ __volatile__ ("movb $0x1b,%al;call ___djgpp_hw_exception"); //This forces the display of the traceback.
   } else {
   return check_ppmask_collision_amount_normal(mask1,mask2,x1,y1,x2,y2,addx,addy);
   }
 return 0; //This will never be reached, but the compiler complains if we not put it here.
 }
#else
#define check_ppmask_collision  check_ppmask_collision_normal
#define check_pp_collision  check_pp_collision_normal
#define check_pp_collision_amount  check_pp_collision_amount_normal
#define check_ppmask_collision_amount  check_ppmask_collision_amount_normal
#endif /* of debugging support */

#ifdef __cplusplus
}
#endif
#endif          /* ifndef PPCOL_H */

