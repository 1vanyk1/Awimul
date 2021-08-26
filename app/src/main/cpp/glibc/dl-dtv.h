#ifndef _DL_DTV_H
#define _DL_DTV_H

typedef unsigned int     size_t;

struct dtv_pointer
{
    void *val;                    /* Pointer to data, or TLS_DTV_UNALLOCATED.  */
    void *to_free;                /* Unaligned pointer, for deallocation.  */
};
/* Type for the dtv.  */
typedef union dtv
{
    size_t counter;
    struct dtv_pointer pointer;
} dtv_t;
/* Value used for dtv entries for which the allocation is delayed.  */
#define TLS_DTV_UNALLOCATED ((void *) -1l)
#endif /* _DLT_DTV_H */