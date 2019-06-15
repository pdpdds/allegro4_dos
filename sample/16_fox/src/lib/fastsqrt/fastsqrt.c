/*
 * A High Speed, Low Precision Square Root
 * by Paul Lalonde and Robert Dawson
 * from "Graphics Gems", Academic Press, 1990

 * Modified by Javier Arevalo to avoid some unnecessary shifts.
 * Seems like this adds some precision too, but I'm unsure.

 * Ported to DJGPP/GCC environment by Brennan Underwood (brennan@rt66.com)

 * NO WARRANTY!
 */

#include <math.h>
#include "fastsqrt.h"

long sqrt_table[0x100];

void build_sqrt_table(void) {
    unsigned short i;
    float f;
    unsigned int *fi = (unsigned*)&f;   /* To access the bits of a float in */
                                        /* C quickly we must misuse pointers */
    for (i = 0; i <= 0x7f; i++) {
        *fi = 0;

            /* Build a float with the bit pattern i as mantissa */
            /* and an exponent of 0, stored as 127 */
        *fi = (i << 16) | (127 << 23);
        f = sqrt(f);

            /* Take the square root then strip the first 7 bits of */
            /* the mantissa into the table */
        sqrt_table[i] = (*fi & 0x7fffff);

            /* Repeat the process, this time with an exponent of 1, */
            /* stored as 128 */
        *fi = 0;
        *fi = (i << 16) | (128 << 23);
        f = sqrt(f);
        sqrt_table[i+0x80] = (*fi & 0x7fffff);
    }
}

#ifndef _WIN32
/* original C version */
float fast_fsqrt(float n) {
    unsigned int *num = (unsigned *)&n; /* to access the bits of a float in C */
                                        /* we must misuse pointers */

    short e;        /* the exponent */
    if (n == 0) return (0); /* check for square root of 0 */
    e = (*num >> 23) - 127; /* get the exponent - on a SPARC the */
                            /* exponent is stored with 127 added */
    *num &= 0x7fffff;   /* leave only the mantissa */
    if (e & 0x01) *num |= 0x800000;
                /* the exponent is odd so we have to */
                /* look it up in the second half of  */
                /* the lookup table, so we set the high bit */
    e >>= 1;    /* divide the exponent by two */
                /* note that in C the shift */
                /* operators are sign preserving */
                /* for signed operands */
        /* Do the table lookup, based on the quaternary mantissa, */
        /* then reconstruct the result back into a float */
    *num = ((sqrt_table[*num >> 16])) + ((e + 127) << 23);
    return(n);
}

/* this is based on Javier Arevalo's Watcom code. Rearranged a bit
   for Pentium scheduling and GCC's superior inline asm, now 8 cycles
   *including* the AGI, theoretically. I have clocked this at 19-30 cycles
   in actual use, w/RDTSC. -- BU */
/* non-inline version for those who aren't optimizing */
float fast_fsqrt_asm(float n) {
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