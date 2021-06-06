#ifndef _XIMTRANS_H
#define _XIMTRANS_H

typedef struct _TransIntrCallbackRec	*TransIntrCallbackPtr;

typedef struct _TransIntrCallbackRec {
    Bool			(*func)(
            Xim, INT16, XPointer, XPointer
    );
    XPointer			 call_data;
    TransIntrCallbackPtr	 next;
} TransIntrCallbackRec ;

typedef struct {
    TransIntrCallbackPtr	 intr_cb;
    struct _XtransConnInfo 	*trans_conn; /* transport connection object */
    int				 fd;
    char			*address;
    Window			 window;
    Bool			 is_putback;
} TransSpecRec;


/*
 * Prototypes
 */

extern Bool _XimTransRegisterDispatcher(
        Xim		 im,
        Bool	 (*callback)(
                Xim, INT16, XPointer, XPointer
        ),
        XPointer	 call_data
);


extern Bool _XimTransIntrCallback(
        Xim		 im,
        Bool	 (*callback)(
                Xim, INT16, XPointer, XPointer
        ),
        XPointer	 call_data
);

extern Bool _XimTransCallDispatcher(
        Xim		 im,
        INT16	 len,
        XPointer	 data
);

extern void _XimFreeTransIntrCallback(
        Xim		 im
);

extern Bool _XimTransFilterWaitEvent(
        Display	*d,
        Window	 w,
        XEvent	*ev,
        XPointer	 arg
);

extern void _XimTransInternalConnection(
        Display	*d,
        int		 fd,
        XPointer	 arg
);

extern Bool _XimTransWrite(
        Xim		 im,
        INT16	 len,
        XPointer	 data
);

extern Bool _XimTransRead(
        Xim		 im,
        XPointer	 recv_buf,
        int		 buf_len,
        int		*ret_len
);

extern void _XimTransFlush(
        Xim		 im
);

#endif /* _XIMTRANS__H */