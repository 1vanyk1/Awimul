#ifndef DRI_ERRORS_H
#define DRI_ERRORS_H

#include "../gl4es/gl/gles.h"

#define MAX_DEBUG_LOGGED_MESSAGES   10
#define MAX_DEBUG_MESSAGE_LENGTH    4096

#ifdef HAVE_FUNC_ATTRIBUTE_FORMAT
#if defined (__MINGW_PRINTF_FORMAT)
# define PRINTFLIKE(f, a) __attribute__ ((format(__MINGW_PRINTF_FORMAT, f, a)))
#else
# define PRINTFLIKE(f, a) __attribute__ ((format(__printf__, f, a)))
#endif
#else
#define PRINTFLIKE(f, a)
#endif

static inline struct pipe_screen * debug_screen_wrap(struct pipe_screen *screen);

extern void _mesa_problem( const struct gl_context *ctx, const char *fmtString, ... );

extern void _mesa_debug( const struct gl_context *ctx, const char *fmtString, ... ) PRINTFLIKE(2, 3);

extern void _mesa_error( struct gl_context *ctx, GLenum error, const char *fmtString, ... ) PRINTFLIKE(3, 4);

void _mesa_warning( struct gl_context *ctx, const char *fmtString, ... );

#endif //DRI_ERRORS_H
