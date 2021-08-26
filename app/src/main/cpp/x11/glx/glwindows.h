#ifndef GLWINDOWS_H
#define GLWINDOWS_H

#include "../../GL/gl.h"

typedef struct {
    unsigned int enableDebug:1;
    unsigned int enableTrace:1;
    unsigned int dumpPFD:1;
    unsigned int dumpHWND:1;
    unsigned int dumpDC:1;
    unsigned int enableGLcallTrace:1;
    unsigned int enableWGLcallTrace:1;
} glxWinDebugSettingsRec;

extern glxWinDebugSettingsRec glxWinDebugSettings;

void glxWinPushNativeProvider(void);
void glAddSwapHintRectWINWrapper(GLint x, GLint y, GLsizei width, GLsizei height);
int glWinSelectImplementation(int native);

#if 1
#define GLWIN_TRACE_MSG(msg, args...) if (glxWinDebugSettings.enableTrace) ErrorF(msg " [%s:%d]\n" , ##args , __FUNCTION__, __LINE__ )
#define GLWIN_DEBUG_MSG(msg, args...) if (glxWinDebugSettings.enableDebug) ErrorF(msg " [%s:%d]\n" , ##args , __FUNCTION__, __LINE__ )
#else
#define GLWIN_TRACE_MSG(a, ...)
#define GLWIN_DEBUG_MSG(a, ...)
#endif

#endif
