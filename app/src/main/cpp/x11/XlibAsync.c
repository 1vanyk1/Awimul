#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xos.h"

/*
 * Xlib's _XAsyncErrorState sequence number may wrap in 32bit
 * and we cannot use 64bit as it's public API.
 */
#ifdef LONG64
#define _XLIB_ASYNC_SEQUENCE_CMP(a,op,b)     ((a == 0) || (a op b))
#else /* !LONG64 */
#define _XLIB_ASYNC_SEQUENCE_CMP(a,op,b)     ((a == 0) || \
                                              (((a op b) && (b - a op (UINT32_MAX >> 1))) || \
                                               ((b op a) && ((UINT32_MAX >> 1) op a - b))))
#endif /* !LONG64 */

/*ARGSUSED*/
Bool
_XAsyncErrorHandler(
        register Display *dpy,
        register xReply *rep,
        char *buf,
        int len,
        XPointer data)
{
    register _XAsyncErrorState *state;

    state = (_XAsyncErrorState *)data;
    if (rep->generic.type == X_Error &&
        (!state->error_code ||
         rep->error.errorCode == state->error_code) &&
        (!state->major_opcode ||
         rep->error.majorCode == state->major_opcode) &&
        (!state->minor_opcode ||
         rep->error.minorCode == state->minor_opcode) &&
        (_XLIB_ASYNC_SEQUENCE_CMP(state->min_sequence_number,<=,dpy->last_request_read)) &&
        (_XLIB_ASYNC_SEQUENCE_CMP(state->max_sequence_number,>=,dpy->last_request_read))) {
        state->last_error_received = rep->error.errorCode;
        state->error_count++;
        return True;
    }
    return False;
}

void _XDeqAsyncHandler(
        Display *dpy,
        register _XAsyncHandler *handler)
{
    register _XAsyncHandler **prev;
    register _XAsyncHandler *async;

    for (prev = &dpy->async_handlers;
         (async = *prev) && (async != handler);
         prev = &async->next)
        ;
    if (async)
        *prev = async->next;
}

char *
_XGetAsyncReply(
        register Display *dpy,
        register char *replbuf,	/* data is read into this buffer */
        register xReply *rep,	/* value passed to calling handler */
        char *buf,			/* value passed to calling handler */
        int len,			/* value passed to calling handler */
        int extra,			/* extra words to read, ala _XReply */
        Bool discard)		/* discard after extra?, ala _XReply */
{
    if (extra == 0) {
        if (discard && (rep->generic.length << 2) > len)
            _XEatData (dpy, (rep->generic.length << 2) - len);
        return (char *)rep;
    }

    if (extra <= rep->generic.length) {
        int size = SIZEOF(xReply) + (extra << 2);
        if (size > len) {
            memcpy(replbuf, buf, len);
            _XRead(dpy, replbuf + len, size - len);
            buf = replbuf;
            len = size;
        }

        if (discard && rep->generic.length > extra &&
            (rep->generic.length << 2) > len)
            _XEatData (dpy, (rep->generic.length << 2) - len);

        return buf;
    }
    /*
     *if we get here, then extra > rep->generic.length--meaning we
     * read a reply that's shorter than we expected.  This is an
     * error,  but we still need to figure out how to handle it...
     */
    if ((rep->generic.length << 2) > len)
        _XEatData (dpy, (rep->generic.length << 2) - len);
    _XIOError (dpy);
    return (char *)rep;
}

void
_XGetAsyncData(
        Display *dpy,
        char *data,			/* data is read into this buffer */
        char *buf,			/* value passed to calling handler */
        int len,			/* value passed to calling handler */
        int skip,			/* number of bytes already read in previous
				   _XGetAsyncReply or _XGetAsyncData calls */
        int datalen,		/* size of data buffer in bytes */
        int discardtotal)		/* min. bytes to consume (after skip) */
{
    buf += skip;
    len -= skip;
    if (!data) {
        if (datalen > len)
            _XEatData(dpy, datalen - len);
    } else if (datalen <= len) {
        memcpy(data, buf, datalen);
    } else {
        memcpy(data, buf, len);
        _XRead(dpy, data + len, datalen - len);
    }
    if (discardtotal > len) {
        if (datalen > len)
            len = datalen;
        _XEatData(dpy, discardtotal - len);
    }
}