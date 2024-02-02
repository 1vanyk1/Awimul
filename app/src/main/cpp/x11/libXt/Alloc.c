#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "InitialI.h"
#undef _XBCOPYFUNC

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define Xmalloc(size) malloc((size))
#define Xrealloc(ptr, size) realloc((ptr), (size))
#define Xcalloc(nelem, elsize) calloc((nelem), (elsize))
#define Xfree(ptr) free(ptr)

#ifdef _XNEEDBCOPYFUNC
void
_XtBcopy(char *src, char *dst, int length)
{
    if (src < dst) {
        dst += length;
        src += length;
        while (length--)
            *--dst = *--src;
    }
    else {
        while (length--)
            *dst++ = *src++;
    }
}
#endif

void
_XtAllocError(String type)
{
    Cardinal num_params = 1;

    if (type == NULL)
        type = "local memory allocation";
    XtErrorMsg("allocError", type, XtCXtToolkitError,
               "Cannot perform %s", &type, &num_params);
}

void
_XtHeapInit(Heap *heap)
{
    heap->start = NULL;
    heap->bytes_remaining = 0;
}

/* Version of asprintf() using XtMalloc
 * Not currently available in XTTRACEMEMORY version, since that would
 * require varargs macros everywhere, which are only standard in C99 & later.
 */
Cardinal
XtAsprintf(_XtString *new_string, _Xconst char *_X_RESTRICT_KYWD format, ...)
{
    char buf[256];
    int len;
    va_list ap;

    va_start(ap, format);
    len = vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    if (len < 0)
        _XtAllocError("vsnprintf");

    *new_string = XtMalloc((Cardinal) len + 1); /* snprintf doesn't count trailing '\0' */
    if ((size_t) len < sizeof(buf)) {
        strncpy(*new_string, buf, (size_t) len);
        (*new_string)[len] = '\0';
    }
    else {
        va_start(ap, format);
        if (vsnprintf(*new_string, (size_t) (len + 1), format, ap) < 0)
            _XtAllocError("vsnprintf");
        va_end(ap);
    }
    return (Cardinal) len;
}

#ifndef XTTRACEMEMORY

char *
XtMalloc(unsigned size)
{
    char *ptr;

#if defined(MALLOC_0_RETURNS_NULL) && defined(XTMALLOC_BC)
    /* preserve this (broken) behavior until everyone fixes their apps */
    if (!size)
        size = 1;
#endif
    if ((ptr = Xmalloc(size)) == NULL)
        _XtAllocError("malloc");

    return (ptr);
}

char *
XtRealloc(char *ptr, unsigned size)
{
    if (ptr == NULL) {
#ifdef MALLOC_0_RETURNS_NULL
        if (!size)
            size = 1;
#endif
        return (XtMalloc(size));
    }
    else if ((ptr = Xrealloc(ptr, size)) == NULL
#ifdef MALLOC_0_RETURNS_NULL
        && size
#endif
            )
        _XtAllocError("realloc");

    return (ptr);
}

char *
XtCalloc(unsigned num, unsigned size)
{
    char *ptr;

#if defined(MALLOC_0_RETURNS_NULL) && defined(XTMALLOC_BC)
    /* preserve this (broken) behavior until everyone fixes their apps */
    if (!size)
        num = size = 1;
#endif
    if ((ptr = Xcalloc(num, size)) == NULL)
        _XtAllocError("calloc");

    return (ptr);
}

void
XtFree(char *ptr)
{
    free(ptr);
}

char *
__XtMalloc(unsigned size)
{
#ifdef MALLOC_0_RETURNS_NULL
    if (!size)
        size = 1;
#endif
    return XtMalloc(size);
}

char *
__XtCalloc(unsigned num, unsigned size)
{
#ifdef MALLOC_0_RETURNS_NULL
    if (!size)
        num = size = 1;
#endif
    return XtCalloc(num, size);
}

#ifndef HEAP_SEGMENT_SIZE
#define HEAP_SEGMENT_SIZE 1492
#endif

char *
_XtHeapAlloc(Heap *heap, Cardinal bytes)
{
    register char *heap_loc;

    if (heap == NULL)
        return XtMalloc(bytes);
    if (heap->bytes_remaining < (int) bytes) {
        if ((bytes + sizeof(char *)) >= (HEAP_SEGMENT_SIZE >> 1)) {
            /* preserve current segment; insert this one in front */
#ifdef _TRACE_HEAP
            printf("allocating large segment (%d bytes) on heap %p\n",
                   bytes, heap);
#endif
            heap_loc = XtMalloc(bytes + (Cardinal) sizeof(char *));
            if (heap->start) {
                *(char **) heap_loc = *(char **) heap->start;
                *(char **) heap->start = heap_loc;
            }
            else {
                *(char **) heap_loc = NULL;
                heap->start = heap_loc;
            }
            return heap_loc + sizeof(char *);
        }
        /* else discard remainder of this segment */
#ifdef _TRACE_HEAP
        printf("allocating new segment on heap %p\n", heap);
#endif
        heap_loc = XtMalloc((unsigned) HEAP_SEGMENT_SIZE);
        *(char **) heap_loc = heap->start;
        heap->start = heap_loc;
        heap->current = heap_loc + sizeof(char *);
        heap->bytes_remaining = HEAP_SEGMENT_SIZE - sizeof(char *);
    }
    bytes = (Cardinal) ((bytes + (sizeof(long) - 1)) & (~(sizeof(long) - 1)));
    heap_loc = heap->current;
    heap->current += bytes;
    heap->bytes_remaining = (heap->bytes_remaining - (int) bytes);      /* can be negative, if rounded */
    return heap_loc;
}

void
_XtHeapFree(Heap *heap)
{
    char *segment = heap->start;

    while (segment != NULL) {
        char *next_segment = *(char **) segment;

        XtFree(segment);
        segment = next_segment;
    }
    heap->start = NULL;
    heap->bytes_remaining = 0;
}

#else

/*
 * X Toolkit Memory Trace Allocation Routines
 */

#undef XtMalloc
#undef XtRealloc
#undef XtCalloc
#undef XtFree

typedef struct _Stats *StatsPtr;
typedef struct _Stats {
    StatsPtr prev, next;
    const char *file;
    int line;
    unsigned size;
    unsigned long seq;
    XtPointer heap;
} Stats;

static StatsPtr XtMemory = (StatsPtr) NULL;
static unsigned long ActiveXtMemory = 0;
static unsigned long XtSeqId = 0;
static unsigned long XtSeqBreakpoint = (unsigned long) (~0UL);

#define StatsSize(n) (unsigned)((((n) + (sizeof(long) - 1)) & ~(sizeof(long) - 1)) + sizeof(Stats))
#define ToStats(ptr) ((StatsPtr)(ptr - sizeof(Stats)))
#define ToMem(ptr) (((char *)ptr) + sizeof(Stats))

#define CHAIN(ptr,len,hp) \
    ptr->next = XtMemory; \
    if (XtMemory) \
        XtMemory->prev = ptr; \
    XtMemory = ptr; \
    ptr->prev = (StatsPtr)NULL; \
    ptr->file = file; \
    ptr->line = line; \
    ptr->size = len; \
    ptr->heap = hp; \
    if (file) \
        ActiveXtMemory += len; \
    ptr->seq = XtSeqId; \
    if (XtSeqId == XtSeqBreakpoint) \
        _XtBreakpoint(ptr); \
    XtSeqId++

/*ARGUSED*/
static void
_XtBreakpoint(StatsPtr mem)
{
    mem->seq = XtSeqId;         /* avoid being optimized out of existence */
}

char *
_XtMalloc(unsigned size, const char *file, int line)
{
    StatsPtr ptr;
    unsigned newsize;
    char *retval = NULL;

    LOCK_PROCESS;
    newsize = StatsSize(size);
    if ((ptr = (StatsPtr) Xmalloc(newsize)) == NULL)
        _XtAllocError("malloc");
    CHAIN(ptr, size, NULL);
    retval = (ToMem(ptr));
    UNLOCK_PROCESS;
    return retval;
}

char *
XtMalloc(unsigned size)
{
    return _XtMalloc(size, (char *) NULL, 0);
}

char *
_XtRealloc(char *ptr, unsigned size, const char *file, int line)
{
    char *newptr;

    LOCK_PROCESS;
    newptr = _XtMalloc(size, file, line);
    if (ptr) {
        unsigned copysize = ToStats(ptr)->size;

        if (copysize > size)
            copysize = size;
        memmove(newptr, ptr, copysize);
        _XtFree(ptr);
    }
    UNLOCK_PROCESS;
    return (newptr);
}

char *
XtRealloc(char *ptr, unsigned size)
{
    return _XtRealloc(ptr, size, (char *) NULL, 0);
}

char *
_XtCalloc(unsigned num, unsigned size, const char *file, int line)
{
    StatsPtr ptr;
    unsigned total, newsize;
    char *retval = NULL;

    LOCK_PROCESS;
    total = num * size;
    newsize = StatsSize(total);
    if ((ptr = (StatsPtr) Xcalloc(newsize, 1)) == NULL)
        _XtAllocError("calloc");
    CHAIN(ptr, total, NULL);
    retval = (ToMem(ptr));
    UNLOCK_PROCESS;
    return retval;
}

char *
XtCalloc(unsigned num, unsigned size)
{
    return _XtCalloc(num, size, (char *) NULL, 0);
}

Boolean
_XtIsValidPointer(char *ptr)
{
    register StatsPtr mem;
    register StatsPtr stp = ToStats(ptr);

    LOCK_PROCESS;
    for (mem = XtMemory; mem; mem = mem->next) {
        if (mem == stp) {
            UNLOCK_PROCESS;
            return True;
        }
    }
    UNLOCK_PROCESS;
    return False;
}

Boolean _XtValidateMemory = False;

void
_XtFree(char *ptr)
{
    register StatsPtr stp;

    LOCK_PROCESS;
    if (ptr) {
        if (_XtValidateMemory && !_XtIsValidPointer(ptr))
            abort();
        stp = ToStats(ptr);
        if (stp->file)
            ActiveXtMemory -= stp->size;
        if (stp->prev)
            stp->prev->next = stp->next;
        else
            XtMemory = stp->next;
        if (stp->next)
            stp->next->prev = stp->prev;
        Xfree((char *) stp);
    }
    UNLOCK_PROCESS;
}

void
XtFree(char *ptr)
{
    _XtFree(ptr);
}

char *
_XtHeapMalloc(Heap *heap, Cardinal size, const char *file, int line)
{
    StatsPtr ptr;
    unsigned newsize;
    XtPointer hp = (XtPointer) heap;
    char *retval = NULL;

    LOCK_PROCESS;
    newsize = StatsSize(size);
    if ((ptr = (StatsPtr) Xmalloc(newsize)) == NULL)
        _XtAllocError("malloc");
    CHAIN(ptr, size, hp);
    retval = (ToMem(ptr));
    UNLOCK_PROCESS;
    return retval;
}

void
_XtHeapFree(Heap *heap)
{
    register StatsPtr mem, next;

    LOCK_PROCESS;
    for (mem = XtMemory; mem; mem = next) {
        next = mem->next;
        if (mem->heap == heap) {
            if (mem->file)
                ActiveXtMemory -= mem->size;
            if (mem->prev)
                mem->prev->next = next;
            else
                XtMemory = next;
            if (next)
                next->prev = mem->prev;
            Xfree((char *) mem);
        }
    }
    UNLOCK_PROCESS;
}

#include <stdio.h>

void
_XtPrintMemory(const char *filename)
{
    register StatsPtr mem;
    FILE *f;

    if (filename == NULL)
        f = stderr;
    else
        f = fopen(filename, "w");
    LOCK_PROCESS;
    fprintf(f, "total size: %lu\n", ActiveXtMemory);
    for (mem = XtMemory; mem; mem = mem->next) {
        if (mem->file)
            fprintf(f, "size: %6d  seq: %5lu  %12s(%4d)  %s\n",
                    mem->size, mem->seq,
                    mem->file, mem->line, mem->heap ? "heap" : "");
    }
    UNLOCK_PROCESS;
    if (filename)
        fclose(f);
}

#endif                          /* XTTRACEMEMORY */
