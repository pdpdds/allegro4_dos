#ifndef _FASTFLOT_H
#define _FASTFLOT_H

/* file provided by Brennan Underwood, brennan@rt66.com */
/* Absolutely NO WARRANTY */

/* fast float to int rounding, based on a post to rec.games.programmer
by Glenn Corpes, glennc@cix.co.uk */
/* make sure your floats never exceed 1<<23 (8,388,608.0) or are negative */

static inline int F_TO_I(float f) __attribute__((const));

static inline int F_TO_I(float f) {
  f += (1<<23);
  return *((int *)&f) & 0x7fffff;
}

#endif
