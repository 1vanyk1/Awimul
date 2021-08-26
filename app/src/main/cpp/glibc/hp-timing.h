#ifndef _HP_TIMING_H
#define _HP_TIMING_H	1

/* There are no generic definitions for the times.  We could write something
   using the `gettimeofday' system call where available but the overhead of
   the system call might be too high.  */

/* Provide dummy definitions.  */
#define HP_TIMING_AVAIL		(0)
#define HP_SMALL_TIMING_AVAIL	(0)
#define HP_TIMING_INLINE	(0)
typedef int hp_timing_t;
#define HP_TIMING_NOW(var)
#define HP_TIMING_DIFF(Diff, Start, End)
#define HP_TIMING_ACCUM_NT(Sum, Diff)
#define HP_TIMING_PRINT(Buf, Len, Val)

/* Since this implementation is not available we tell the user about it.  */
#define HP_TIMING_NONAVAIL	1

#endif	/* hp-timing.h */