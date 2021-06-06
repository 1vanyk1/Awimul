#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../x11/headers/xlibint.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/XimTrInt.h"
#include "../x11/xlc/Ximint.h"

TransportSW _XimTransportRec[] = {
        { "X",          _XimXConf },  /* 1st entry must be X.
					This will be a fallback */
#ifdef TCPCONN
        { "tcp",        _XimTransConf }, /* use X transport lib */
#endif /* TCPCONN */
#if defined(UNIXCONN) || defined(LOCALCONN)
        { "local",      _XimTransConf }, /* use X transport lib */
#endif /* UNIXCONN */
        { (char *)NULL, (Bool (*)(Xim, char *))NULL },
};

Bool
_XimConnect(Xim im)
{
    return im->private.proto.connect(im);
}

Bool
_XimShutdown(Xim im)
{
    return im->private.proto.shutdown(im);
}

Bool
_XimWrite(Xim im, INT16 len, XPointer data)
{
    return im->private.proto.write(im, len, data);
}

static int
_CheckProtocolData(
        Xim		  im,
        char	 *recv_buf)
{
    int		 data_len;

    data_len = (int)(((*((CARD16 *)recv_buf + 1)) * 4) + XIM_HEADER_SIZE);
    return data_len;
}

static int
_XimReadData(
        Xim		 im,
        INT16	*len,
        XPointer	 buf,
        int		 buf_size)
{
    char	*hold_buf;
    char	*tmp;
    int		 data_len;
    int		 packet_size;
    int		 ret_len;
    register int i;

    if (buf_size < XIM_HEADER_SIZE) {
        *len = (INT16)XIM_HEADER_SIZE;
        return XIM_OVERFLOW;
    }

    bzero(buf, buf_size);
    packet_size = 0;
    data_len = 0;

    if ((hold_buf = im->private.proto.hold_data)) {
        data_len = im->private.proto.hold_data_len;
        if (data_len >= XIM_HEADER_SIZE) {
            packet_size = _CheckProtocolData(im, hold_buf);
            if (packet_size > buf_size) {
                *len = (INT16)packet_size;
                return XIM_OVERFLOW;
            }
            if (packet_size <= data_len) {
                memcpy(buf, hold_buf, packet_size);
                for (i = packet_size; i < data_len; i++) {
                    if (hold_buf[i])
                        break;
                }
                data_len -= i;

                if (data_len) {
                    if (!(tmp = Xmalloc(data_len))) {
                        return XIM_FALSE;
                    }
                    memcpy(tmp, &hold_buf[i], data_len);
                    im->private.proto.hold_data = tmp;
                    im->private.proto.hold_data_len = data_len;
                } else {
                    im->private.proto.hold_data = 0;
                    im->private.proto.hold_data_len = 0;
                }
                Xfree(hold_buf);
                *len = (INT16)packet_size;
                return XIM_TRUE;
            }
        }
        memcpy(buf, hold_buf, data_len);
        buf_size -= data_len;
        Xfree(hold_buf);
        im->private.proto.hold_data = 0;
        im->private.proto.hold_data_len = 0;
    }

    if (!packet_size) {
        while (data_len < XIM_HEADER_SIZE) {
            if (!(im->private.proto.read(im,
                                         (XPointer)&buf[data_len], buf_size, &ret_len))) {
                return XIM_FALSE;
            }
            data_len += ret_len;
            buf_size -= ret_len;
        }
        packet_size = _CheckProtocolData(im, buf);
    }

    if (packet_size > buf_size) {
        if (!(tmp = Xmalloc(data_len))) {
            return XIM_FALSE;
        }
        memcpy(tmp, buf, data_len);
        bzero(buf, data_len);
        im->private.proto.hold_data = tmp;
        im->private.proto.hold_data_len = data_len;
        *len = (INT16)packet_size;
        return XIM_OVERFLOW;
    }

    while (data_len < packet_size) {
        if (!(im->private.proto.read(im,
                                     (XPointer)&buf[data_len], buf_size, &ret_len))) {
            return XIM_FALSE;
        }
        data_len += ret_len;
        buf_size -= ret_len;
    }

    for (i = packet_size; i < data_len; i++) {
        if (buf[i])
            break;
    }
    data_len -= i;

    if (data_len) {
        if (!(tmp = Xmalloc(data_len))) {
            return XIM_FALSE;
        }
        memcpy(tmp, &buf[i], data_len);
        bzero(&buf[i], data_len);
        im->private.proto.hold_data = tmp;
        im->private.proto.hold_data_len = data_len;
    } else {
        im->private.proto.hold_data = 0;
        im->private.proto.hold_data_len = 0;
    }
    *len = (INT16)packet_size;
    return XIM_TRUE;
}

static Bool
_XimCallDispatcher(
        Xim		 im,
        INT16	 len,
        XPointer	 data)
{
    return im->private.proto.call_dispatcher(im, len, data);
}

int
_XimRead(Xim im, INT16 *len, XPointer buf, int buf_size,
         Bool (*predicate)(Xim, INT16, XPointer, XPointer), XPointer arg)
{
    INT16	 read_len;
    int		 ret_code;

    for (;;) {
        ret_code = _XimReadData(im, &read_len, buf, buf_size);
        if(ret_code != XIM_TRUE) {
            return ret_code;
        }
        if ((*predicate)(im, read_len, buf, arg))
            break;
        if (_XimCallDispatcher(im, read_len, buf))
            continue;
        _XimError(im, 0, XIM_BadProtocol, (INT16)0, (CARD16)0, (char *)NULL);
    }
    *len = read_len;
    return True;
}

Bool
_XimRegisterDispatcher(
        Xim		 im,
        Bool	 (*callback)(
                Xim, INT16, XPointer, XPointer
        ),
        XPointer	 call_data)
{
    return im->private.proto.register_dispatcher(im, callback, call_data);
}

void
_XimFlush(Xim im)
{
    im->private.proto.flush(im);
    return;
}

Bool
_XimFilterWaitEvent(Xim im)
{
    INT16	 read_len;
    CARD32	 reply32[BUFSIZE/4];
    char	*reply = (char *)reply32;
    XPointer	 preply;
    int		 buf_size;
    int		 ret_code;

    buf_size = BUFSIZE;
    ret_code = _XimReadData(im, &read_len, (XPointer)reply, buf_size);
    if(ret_code == XIM_TRUE) {
        preply = reply;
    } else if(ret_code == XIM_OVERFLOW) {
        if(read_len <= 0) {
            preply = reply;
        } else {
            buf_size = (int)read_len;
            preply = Xmalloc(buf_size);
            ret_code = _XimReadData(im, &read_len, preply, buf_size);
            if(ret_code != XIM_TRUE) {
                if (preply != reply)
                    Xfree(preply);
                return False;
            }
        }
    } else {
        return False;
    }
    if (_XimCallDispatcher(im, read_len, preply)) {
        if(reply != preply)
            Xfree(preply);
        return True;
    }
    _XimError(im, 0, XIM_BadProtocol, (INT16)0, (CARD16)0, (char *)NULL);
    if(reply != preply)
        Xfree(preply);
    return True;
}