#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/fonts/fontmisc.h"
#include "../headers/fonts/fontstruct.h"

/*
 * Static sized hash table for looking up font name patterns
 *
 * LRU entries, reusing old entries
 */

#define NBUCKETS	16
#define NENTRIES	64

#define UNSET		(NENTRIES+1)

typedef unsigned char	EntryPtr;

typedef struct _FontPatternCacheEntry {
    struct _FontPatternCacheEntry   *next, **prev;
    short			    patlen;
    const char			    *pattern;
    int				    hash;
    FontPtr			    pFont;	/* associated font */
} FontPatternCacheEntryRec, *FontPatternCacheEntryPtr;

typedef struct _xfont2_pattern_cache {
    FontPatternCacheEntryPtr	buckets[NBUCKETS];
    FontPatternCacheEntryRec	entries[NENTRIES];
    FontPatternCacheEntryPtr	free;
} xfont2_pattern_cache_rec;

/* Empty cache (for rehash) */
void
xfont2_empty_font_pattern_cache(xfont2_pattern_cache_ptr cache)
{
    int	    i;

    for (i = 0; i < NBUCKETS; i++)
        cache->buckets[i] = 0;
    for (i = 0; i < NENTRIES; i++)
    {
        cache->entries[i].next = &cache->entries[i+1];
        cache->entries[i].prev = 0;
        cache->entries[i].pFont = 0;
        free ((void *) cache->entries[i].pattern);
        cache->entries[i].pattern = 0;
        cache->entries[i].patlen = 0;
    }
    cache->free = &cache->entries[0];
    cache->entries[NENTRIES - 1].next = 0;
}

/* Create and initialize cache */
xfont2_pattern_cache_ptr
xfont2_make_font_pattern_cache(void)
{
    xfont2_pattern_cache_ptr	cache;
    int			i;
    cache = malloc (sizeof *cache);
    if (!cache)
        return 0;
    for (i = 0; i < NENTRIES; i++) {
        cache->entries[i].patlen = 0;
        cache->entries[i].pattern = 0;
        cache->entries[i].pFont = 0;
    }
    xfont2_empty_font_pattern_cache (cache);
    return cache;
}

/* toss cache */
void
xfont2_free_font_pattern_cache(xfont2_pattern_cache_ptr cache)
{
    int	    i;

    for (i = 0; i < NENTRIES; i++)
        free ((void *) cache->entries[i].pattern);
    free (cache);
}

/* compute id for string */
static int
Hash (const char *string, int len)
{
    int	hash;

    hash = 0;
    while (len--)
        hash = (hash << 1) ^ *string++;
    if (hash < 0)
        hash = -hash;
    return hash;
}

/* add entry */
void
xfont2_cache_font_pattern(xfont2_pattern_cache_ptr cache,
                          const char * pattern,
                          int patlen,
                          FontPtr pFont)
{
    FontPatternCacheEntryPtr	e;
    char			*newpat;
    int				i;

    newpat = malloc (patlen);
    if (!newpat)
        return;
    if (cache->free)
    {
        e = cache->free;
        cache->free = e->next;
    }
    else
    {
        i = rand ();
        if (i < 0)
            i = -i;
        i %= NENTRIES;
        e = &cache->entries[i];
        if (e->next)
            e->next->prev = e->prev;
        *e->prev = e->next;
        free ((void *) e->pattern);
    }
    /* set pattern */
    memcpy (newpat, pattern, patlen);
    e->pattern = newpat;
    e->patlen = patlen;
    /* link to new hash chain */
    e->hash = Hash (pattern, patlen);
    i = e->hash % NBUCKETS;
    e->next = cache->buckets[i];
    if (e->next)
        e->next->prev = &(e->next);
    cache->buckets[i] = e;
    e->prev = &(cache->buckets[i]);
    e->pFont = pFont;
}

/* find matching entry */
FontPtr
xfont2_find_cached_font_pattern(xfont2_pattern_cache_ptr cache,
                                const char * pattern,
                                int patlen)
{
    int				hash;
    int				i;
    FontPatternCacheEntryPtr	e;

    hash = Hash (pattern, patlen);
    i = hash % NBUCKETS;
    for (e = cache->buckets[i]; e; e = e->next)
    {
        if (e->patlen == patlen && e->hash == hash &&
            !memcmp (e->pattern, pattern, patlen))
        {
            return e->pFont;
        }
    }
    return 0;
}

void
xfont2_remove_cached_font_pattern(xfont2_pattern_cache_ptr cache,
                                  FontPtr pFont)
{
    FontPatternCacheEntryPtr	e;
    int				i;

    for (i = 0; i < NENTRIES; i++)
    {
        if ((e = &cache->entries[i])->pFont == pFont)
        {
            e->pFont = 0;
            if (e->next)
                e->next->prev = e->prev;
            *e->prev = e->next;
            e->next = cache->free;
            cache->free = e;
            free ((void *) e->pattern);
            e->pattern = 0;
        }
    }
}
