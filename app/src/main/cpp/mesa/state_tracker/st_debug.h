#ifndef ST_DEBUG_H
#define ST_DEBUG_H

#include "../include/pipe/p_compiler.h"
#include "../util/u_debug.h"

struct st_context;

#define DEBUG_MESA            BITFIELD_BIT(0)
#define DEBUG_PRINT_IR        BITFIELD_BIT(1)
#define DEBUG_FALLBACK        BITFIELD_BIT(2)
#define DEBUG_BUFFER          BITFIELD_BIT(3)
#define DEBUG_WIREFRAME       BITFIELD_BIT(4)
#define DEBUG_GREMEDY         BITFIELD_BIT(5)
#define DEBUG_NOREADPIXCACHE  BITFIELD_BIT(6)

extern int ST_DEBUG;

void st_debug_init( void );

void st_update_debug_callback(struct st_context *st);

static inline void
ST_DBG( unsigned flag, const char *fmt, ... )
{
    if (ST_DEBUG & flag)
    {
        va_list args;

        va_start( args, fmt );
        debug_vprintf( fmt, args );
        va_end( args );
    }
}


#endif /* ST_DEBUG_H */
