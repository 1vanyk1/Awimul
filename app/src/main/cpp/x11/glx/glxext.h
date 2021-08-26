#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _glxext_h_
#define _glxext_h_


#ifndef GLX_RGBA_FLOAT_BIT_ARB
#define GLX_RGBA_FLOAT_BIT_ARB             0x00000004
#endif
#ifndef GLX_RGBA_FLOAT_TYPE_ARB
#define GLX_RGBA_FLOAT_TYPE_ARB            0x20B9
#endif
#ifndef GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT
#define GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT    0x00000008
#endif
#ifndef GLX_RGBA_UNSIGNED_FLOAT_TYPE_EXT
#define GLX_RGBA_UNSIGNED_FLOAT_TYPE_EXT   0x20B1
#endif
#ifndef GLX_CONTEXT_RELEASE_BEHAVIOR_ARB
#define GLX_CONTEXT_RELEASE_BEHAVIOR_ARB   0x2097
#define GLX_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0
#define GLX_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#endif

extern void __glXFlushContextCache(void);

extern Bool __glXAddContext(__GLXcontext * cx);
extern void __glXErrorCallBack(GLenum code);
extern void __glXClearErrorOccured(void);
extern GLboolean __glXErrorOccured(void);

extern const char GLServerVersion[];
extern int DoGetString(__GLXclientState * cl, GLbyte * pc, GLboolean need_swap);

extern int
xorgGlxMakeCurrent(ClientPtr client, GLXContextTag tag, XID drawId, XID readId,
                   XID contextId, GLXContextTag newContextTag);

#endif                          /* _glxext_h_ */
