#ifndef _FASTSQRT_H
#define _FASTSQRT_H

/*
 * A High Speed, Low Precision Square Root
 * by Paul Lalonde and Robert Dawson
 * from "Graphics Gems", Academic Press, 1990

 * Modified by Javier Arevalo to avoid some unnecessary shifts.
 * Seems like this adds some precision too, but I'm unsure.

 * Ported to DJGPP/GCC environment by Brennan Underwood (brennan@rt66.com)

 * NO WARRANTY!
 */

#ifdef _WIN32
#define __attribute__
#define __inline__ inline
#endif

/* function prototypes */
void build_sqrt_table(void); /* you MUST call this first! */

#ifdef _WIN32
float fast_fsqrt(float n) __attribute__;
float fast_fsqrt_asm(float n) __attribute__;
#else
float fast_fsqrt(float n) __attribute__((const));
float fast_fsqrt_asm(float n) __attribute__((const));

/* this is based on Javier Arevalo's Watcom code. Rearranged a bit
   for Pentium scheduling and GCC's superior inline asm, now 8 cycles
   (*including* the AGI) theoretically. I have clocked this at 19-30 cycles
   in actual use, w/RDTSC. -- BU */
extern __inline__ float fast_fsqrt_asm(float n) {
  asm (
	"leal 0xc0800000(%%eax), %%edx\n"/*U1 movl eax, edx; subl 0x3f800000,edx */
	"andl $0x007fffff, %%eax\n"	/* V1 */

	"movl %%edx, %%ebx\n"		/* U2 */
	"andl $0xff000000, %%edx\n"	/* V2 */

	"sarl $1, %%edx\n"		/* U3 */
	"andl $0x00800000, %%ebx\n"	/* V3 */

	"addl $0x3f800000, %%edx\n"	/* U4 */
	"orl %%ebx, %%eax\n"		/* V4 */

	"shrl $16, %%eax\n"		/* U5 */
					/* U6 - AGI */
	"movl _sqrt_table(,%%eax,4), %%eax\n" /* U7 */
	"addl %%edx, %%eax\n"		/* U8 */
	: "=a" (n)
	: "a" (n)
	: "%ebx", "%edx");
  return n;
}
#endif

#endif
