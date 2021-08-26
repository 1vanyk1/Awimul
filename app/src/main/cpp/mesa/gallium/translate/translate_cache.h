#ifndef _TRANSLATE_CACHE_H
#define _TRANSLATE_CACHE_H


/*******************************************************************************
 * Translate cache.
 * Simply used to cache created translates. Avoids unecessary creation of
 * translate's if one suitable for a given translate_key has already been
 * created.
 *
 * Note: this functionality depends and requires the CSO module.
 */
struct translate_cache;

struct translate_key;
struct translate;

struct translate_cache *translate_cache_create( void );
void translate_cache_destroy(struct translate_cache *cache);

/**
 * Will try to find a translate structure matched by the given key.
 * If such a structure doesn't exist in the cache the function
 * will automatically create it, insert it in the cache and
 * return the created version.
 *
 */
struct translate *translate_cache_find(struct translate_cache *cache,
                                       struct translate_key *key);

#endif
