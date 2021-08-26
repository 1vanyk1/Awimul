#ifndef _TRANSLATE_H
#define _TRANSLATE_H


#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_format.h"
#include "../../include/pipe/p_state.h"

/**
 * Translate has to work on one more attribute because
 * the draw module has to be able to pass the vertex
 * position even if the fragment shader already consumes
 * PIPE_MAX_ATTRIBS inputs.
 */
#define TRANSLATE_MAX_ATTRIBS (PIPE_MAX_ATTRIBS + 1)

enum translate_element_type {
    TRANSLATE_ELEMENT_NORMAL,
    TRANSLATE_ELEMENT_INSTANCE_ID
};

struct translate_element
{
    enum translate_element_type type;
    enum pipe_format input_format;
    enum pipe_format output_format;
    unsigned input_buffer:8;
    unsigned input_offset:24;
    unsigned instance_divisor;
    unsigned output_offset;
};


struct translate_key {
    unsigned output_stride;
    unsigned nr_elements;
    struct translate_element element[TRANSLATE_MAX_ATTRIBS];
};


struct translate;


typedef void (PIPE_CDECL *run_elts_func)(struct translate *,
const unsigned *elts,
unsigned count,
unsigned start_instance,
unsigned instance_id,
void *output_buffer);

typedef void (PIPE_CDECL *run_elts16_func)(struct translate *,
const uint16_t *elts,
unsigned count,
unsigned start_instance,
unsigned instance_id,
void *output_buffer);

typedef void (PIPE_CDECL *run_elts8_func)(struct translate *,
const uint8_t *elts,
unsigned count,
unsigned start_instance,
unsigned instance_id,
void *output_buffer);

typedef void (PIPE_CDECL *run_func)(struct translate *,
unsigned start,
unsigned count,
unsigned start_instance,
unsigned instance_id,
void *output_buffer);

struct translate {
    struct translate_key key;

    void (*release)( struct translate * );

    void (*set_buffer)( struct translate *,
                        unsigned i,
                        const void *ptr,
                        unsigned stride,
                        unsigned max_index );

    run_elts_func run_elts;
    run_elts16_func run_elts16;
    run_elts8_func run_elts8;
    run_func run;
};



struct translate *translate_create( const struct translate_key *key );

boolean translate_is_output_format_supported(enum pipe_format format);

static inline int translate_keysize( const struct translate_key *key )
{
    return 2 * sizeof(int) + key->nr_elements * sizeof(struct translate_element);
}

static inline int translate_key_compare( const struct translate_key *a,
                                         const struct translate_key *b )
{
    int keysize_a = translate_keysize(a);
    int keysize_b = translate_keysize(b);

    if (keysize_a != keysize_b) {
        return keysize_a - keysize_b;
    }
    return memcmp(a, b, keysize_a);
}


static inline void translate_key_sanitize( struct translate_key *a )
{
    int keysize = translate_keysize(a);
    char *ptr = (char *)a;
    memset(ptr + keysize, 0, sizeof(*a) - keysize);
}


/*******************************************************************************
 *  Private:
 */
struct translate *translate_sse2_create( const struct translate_key *key );

struct translate *translate_generic_create( const struct translate_key *key );

boolean translate_generic_is_output_format_supported(enum pipe_format format);

#endif
