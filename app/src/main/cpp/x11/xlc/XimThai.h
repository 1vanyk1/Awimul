#ifndef _XIMTHAI_H_
#define _XIMTHAI_H_

#include "../headers/xlib.h"

/* Classification of characters in TIS620 according to WTT */

#define CTRL    0   /* control chars */
#define NON     1   /* non composibles */
#define CONS    2   /* consonants */
#define LV      3   /* leading vowels */
#define FV1     4   /* following vowels */
#define FV2     5
#define FV3     6
#define BV1     7   /* below vowels */
#define BV2     8
#define BD      9   /* below diacritics */
#define TONE    10  /* tonemarks */
#define AD1     11  /* above diacritics */
#define AD2     12
#define AD3     13
#define AV1     14  /* above vowels */
#define AV2     15
#define AV3     16


/* extended classification */

#define DEAD    17  /* group of non-spacing characters */


/* display levels in display cell */

#define NONDISP 0   /* non displayable */
#define TOP     1
#define ABOVE   2
#define BASE    3
#define BELOW   4


/* Input Sequence Check modes */

#define WTT_ISC1        1   /* WTT default ISC mode */
#define WTT_ISC2        2   /* WTT strict ISC mode */
#define THAICAT_ISC     3   /* THAICAT ISC mode */
#define NOISC	      255   /* No ISC */


#endif /* _XIMTHAI_H_ */