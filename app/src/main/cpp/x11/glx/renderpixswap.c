#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "unpack.h"
#include "indirect_dispatch.h"

void
__glXDispSwap_SeparableFilter2D(GLbyte * pc)
{
    __GLXdispatchConvolutionFilterHeader *hdr =
            (__GLXdispatchConvolutionFilterHeader *) pc;
    GLint hdrlen, image1len;

    __GLX_DECLARE_SWAP_VARIABLES;

    hdrlen = __GLX_PAD(__GLX_CONV_FILT_CMD_HDR_SIZE);

    __GLX_SWAP_INT((GLbyte *) &hdr->rowLength);
    __GLX_SWAP_INT((GLbyte *) &hdr->skipRows);
    __GLX_SWAP_INT((GLbyte *) &hdr->skipPixels);
    __GLX_SWAP_INT((GLbyte *) &hdr->alignment);

    __GLX_SWAP_INT((GLbyte *) &hdr->target);
    __GLX_SWAP_INT((GLbyte *) &hdr->internalformat);
    __GLX_SWAP_INT((GLbyte *) &hdr->width);
    __GLX_SWAP_INT((GLbyte *) &hdr->height);
    __GLX_SWAP_INT((GLbyte *) &hdr->format);
    __GLX_SWAP_INT((GLbyte *) &hdr->type);

    /*
     ** Just invert swapBytes flag; the GL will figure out if it needs to swap
     ** the pixel data.
     */
    glPixelStorei(GL_UNPACK_SWAP_BYTES, !hdr->swapBytes);
    glPixelStorei(GL_UNPACK_LSB_FIRST, hdr->lsbFirst);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, hdr->rowLength);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, hdr->skipRows);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, hdr->skipPixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, hdr->alignment);

    /* XXX check this usage - internal code called
     ** a version without the packing parameters
     */
    image1len = __glXImageSize(hdr->format, hdr->type, 0, hdr->width, 1, 1,
                               0, hdr->rowLength, 0, hdr->skipRows,
                               hdr->alignment);
    image1len = __GLX_PAD(image1len);

    glSeparableFilter2D(hdr->target, hdr->internalformat, hdr->width,
                        hdr->height, hdr->format, hdr->type,
                        ((GLubyte *) hdr + hdrlen),
                        ((GLubyte *) hdr + hdrlen + image1len));
}
