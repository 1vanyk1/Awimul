#ifndef _XIMTRX_H
#define _XIMTRX_H

typedef struct _XIntrCallbackRec	*XIntrCallbackPtr;

typedef struct _XIntrCallbackRec {
    Bool		 (*func)(
            Xim, INT16, XPointer, XPointer
    );
    XPointer		 call_data;
    XIntrCallbackPtr	 next;
} XIntrCallbackRec ;

typedef struct _XSpecRec {
    XIntrCallbackPtr	 intr_cb;
    Atom		 imconnectid;
    Atom		 improtocolid;
    Atom		 immoredataid;
    Window		 lib_connect_wid;
    Window		 ims_connect_wid;
    XPointer		 ev;
    CARD32		 major_code;
    CARD32		 minor_code;
    CARD32		 BoundarySize;
} XSpecRec;

#define _XIM_PROTOCOL		"_XIM_PROTOCOL"
#define _XIM_XCONNECT		"_XIM_XCONNECT"
#define _XIM_MOREDATA		"_XIM_MOREDATA"

#define MAJOR_TRANSPORT_VERSION         0
#define MINOR_TRANSPORT_VERSION         0

#endif /* _XIMTRX_H */