#ifndef _XMESA_X_H_
#define _XMESA_X_H_

typedef Display      XMesaDisplay;
typedef Pixmap       XMesaPixmap;
typedef Colormap     XMesaColormap;
typedef Drawable     XMesaDrawable;
typedef Window       XMesaWindow;
typedef GC           XMesaGC;
typedef XVisualInfo *XMesaVisualInfo;
typedef XImage       XMesaImage;
typedef XPoint       XMesaPoint;
typedef XColor       XMesaColor;

#define XMesaDestroyImage      XDestroyImage

#define XMesaPutPixel          XPutPixel
#define XMesaGetPixel          XGetPixel

#define XMesaSetForeground     XSetForeground
#define XMesaSetBackground     XSetBackground
#define XMesaSetPlaneMask      XSetPlaneMask
#define XMesaSetFunction       XSetFunction
#define XMesaSetFillStyle      XSetFillStyle
#define XMesaSetTile           XSetTile

#define XMesaDrawPoint         XDrawPoint
#define XMesaDrawPoints        XDrawPoints
#define XMesaDrawLine          XDrawLine
#define XMesaFillRectangle     XFillRectangle
#define XMesaGetImage          XGetImage
#define XMesaPutImage          XPutImage
#define XMesaCopyArea          XCopyArea

#define XMesaCreatePixmap      XCreatePixmap
#define XMesaFreePixmap        XFreePixmap
#define XMesaFreeGC            XFreeGC

#define GET_COLORMAP_SIZE(__v)  __v->visinfo->colormap_size
#define GET_REDMASK(__v)        __v->mesa_visual.redMask
#define GET_GREENMASK(__v)      __v->mesa_visual.greenMask
#define GET_BLUEMASK(__v)       __v->mesa_visual.blueMask
#define GET_VISUAL_DEPTH(__v)   __v->visinfo->depth
#define GET_BLACK_PIXEL(__v)    BlackPixel(__v->display, __v->screen)
#define CHECK_BYTE_ORDER(__v)   host_byte_order()==ImageByteOrder(__v->display)

#endif
