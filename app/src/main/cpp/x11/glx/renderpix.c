#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "unpack.h"
#include "indirect_dispatch.h"

void
__glXDisp_SeparableFilter2D(GLbyte * pc)
{
    __GLXdispatchConvolutionFilterHeader *hdr =
            (__GLXdispatchConvolutionFilterHeader *) pc;
    GLint hdrlen, image1len;

    hdrlen = __GLX_PAD(__GLX_CONV_FILT_CMD_DISPATCH_HDR_SIZE);

    glPixelStorei(GL_UNPACK_SWAP_BYTES, hdr->swapBytes);
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
