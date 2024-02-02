#ifndef _XMU_DRAWING_H_
#define _XMU_DRAWING_H_

#include "../headers/xlib.h"
#include "../headers/xfuncproto.h"

#include <stdio.h>
#if ! defined(_XtIntrinsic_h) && ! defined(PIXEL_ALREADY_TYPEDEFED)
typedef unsigned long Pixel;
#endif

_XFUNCPROTOBEGIN

void XmuDrawRoundedRectangle
        (
                Display		*dpy,
                Drawable	 	draw,
                GC 			gc,
                int			x,
                int			y,
                int			w,
                int			h,
                int			ew,
                int			eh
        );

void XmuFillRoundedRectangle
        (
                Display		*dpy,
                Drawable 		draw,
                GC 			gc,
                int			x,
                int			y,
                int			w,
                int			h,
                int			ew,
                int			eh
        );

void XmuDrawLogo
        (
                Display		*dpy,
                Drawable 		drawable,
                GC			gcFore,
                GC			gcBack,
                int			x,
                int			y,
                unsigned int		width,
                unsigned int		height
        );

Pixmap XmuCreatePixmapFromBitmap
        (
                Display		*dpy,
                Drawable 		d,
                Pixmap 		bitmap,
                unsigned int		width,
                unsigned int		height,
                unsigned int		depth,
                unsigned long		fore,
                unsigned long		back
        );

Pixmap XmuCreateStippledPixmap
        (
                Screen			*screen,
                Pixel			fore,
                Pixel			back,
                unsigned int		depth
        );

void XmuReleaseStippledPixmap
        (
                Screen			*screen,
                Pixmap 		pixmap
        );

Pixmap XmuLocateBitmapFile
        (
                Screen			*screen,
                _Xconst char		*name,
                char			*srcname_return,
                int 			srcnamelen,
                int			*width_return,
                int			*height_return,
                int			*xhot_return,
                int			*yhot_return
        );

Pixmap XmuLocatePixmapFile
        (
                Screen			*screen,
                _Xconst char		*name,
                unsigned long		fore,
                unsigned long		back,
                unsigned int		depth,
                char			*srcname_return,
                int 			srcnamelen,
                int			*width_return,
                int			*height_return,
                int			*xhot_return,
                int			*yhot_return
        );

int XmuReadBitmapData
        (
                FILE			*fstream,
                unsigned int		*width_return,
                unsigned int		*height_return,
                unsigned char		**datap_return,
                int			*xhot_return,
                int			*yhot_return
        );

int XmuReadBitmapDataFromFile
        (
                _Xconst char		*filename,
                unsigned int		*width_return,
                unsigned int		*height_return,
                unsigned char		**datap_return,
                int			*xhot_return,
                int			*yhot_return
        );

_XFUNCPROTOEND

#endif /* _XMU_DRAWING_H_ */
