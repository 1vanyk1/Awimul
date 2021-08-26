#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _ROOTLESSWINDOW_H
#define _ROOTLESSWINDOW_H

#include "rootlessCommon.h"

Bool RootlessCreateWindow(WindowPtr pWin);
Bool RootlessDestroyWindow(WindowPtr pWin);

void RootlessSetShape(WindowPtr pWin, int kind);

Bool RootlessChangeWindowAttributes(WindowPtr pWin, unsigned long vmask);
Bool RootlessPositionWindow(WindowPtr pWin, int x, int y);
Bool RootlessRealizeWindow(WindowPtr pWin);
Bool RootlessUnrealizeWindow(WindowPtr pWin);
void RootlessRestackWindow(WindowPtr pWin, WindowPtr pOldNextSib);
void RootlessCopyWindow(WindowPtr pWin, DDXPointRec ptOldOrg,
                        RegionPtr prgnSrc);
void RootlessPaintWindow(WindowPtr pWin, RegionPtr prgn, int what);
void RootlessMoveWindow(WindowPtr pWin, int x, int y, WindowPtr pSib,
                        VTKind kind);
void RootlessResizeWindow(WindowPtr pWin, int x, int y, unsigned int w,
                          unsigned int h, WindowPtr pSib);
void RootlessReparentWindow(WindowPtr pWin, WindowPtr pPriorParent);
void RootlessChangeBorderWidth(WindowPtr pWin, unsigned int width);

#ifdef __APPLE__
void RootlessNativeWindowMoved(WindowPtr pWin);
void RootlessNativeWindowStateChanged(WindowPtr pWin, unsigned int state);
#endif

#endif
