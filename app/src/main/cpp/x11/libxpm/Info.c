#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "XpmI.h"

/*
 * Init returned data to free safely later on
 */
void
xpmInitXpmInfo(XpmInfo *info)
{
    if (info) {
        info->hints_cmt = NULL;
        info->colors_cmt = NULL;
        info->pixels_cmt = NULL;
        info->extensions = NULL;
        info->nextensions = 0;
    }
}

/*
 * Free the XpmInfo data which have been allocated
 */
void
XpmFreeXpmInfo(XpmInfo *info)
{
    if (info) {
        if (info->valuemask & XpmComments) {
            if (info->hints_cmt) {
                XpmFree(info->hints_cmt);
                info->hints_cmt = NULL;
            }
            if (info->colors_cmt) {
                XpmFree(info->colors_cmt);
                info->colors_cmt = NULL;
            }
            if (info->pixels_cmt) {
                XpmFree(info->pixels_cmt);
                info->pixels_cmt = NULL;
            }
        }
        if (info->valuemask & XpmReturnExtensions && info->nextensions) {
            XpmFreeExtensions(info->extensions, info->nextensions);
            info->extensions = NULL;
            info->nextensions = 0;
        }
        info->valuemask = 0;
    }
}

/*
 * Set the XpmInfo valuemask to retrieve required info
 */
void
xpmSetInfoMask(
        XpmInfo		*info,
        XpmAttributes	*attributes)
{
    info->valuemask = 0;
    if (attributes->valuemask & XpmReturnInfos)
        info->valuemask |= XpmReturnComments;
    if (attributes->valuemask & XpmReturnExtensions)
        info->valuemask |= XpmReturnExtensions;
}

/*
 * Fill in the XpmInfo with the XpmAttributes
 */
void
xpmSetInfo(
        XpmInfo		*info,
        XpmAttributes	*attributes)
{
    info->valuemask = 0;
    if (attributes->valuemask & XpmInfos) {
        info->valuemask |= XpmComments | XpmColorTable;
        info->hints_cmt = attributes->hints_cmt;
        info->colors_cmt = attributes->colors_cmt;
        info->pixels_cmt = attributes->pixels_cmt;
    }
    if (attributes->valuemask & XpmExtensions) {
        info->valuemask |= XpmExtensions;
        info->extensions = attributes->extensions;
        info->nextensions = attributes->nextensions;
    }
    if (attributes->valuemask & XpmHotspot) {
        info->valuemask |= XpmHotspot;
        info->x_hotspot = attributes->x_hotspot;
        info->y_hotspot = attributes->y_hotspot;
    }
}
