#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#elif defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../headers/xproto.h"
#include "../headers/xlib.h"
#include "../headers/xos.h"
#include "../headers/xfuncs.h"
#include "../headers/XKBlib.h"
#include "../headers/extensions/XKBfile.h"

const char *_XkbErrMessages[] = {
        "success",                                  /* _XkbSuccess               */
        "key names not defined",                    /* _XkbErrMissingNames       */
        "key types not defined",                    /* _XkbErrMissingTypes       */
        "required key types not present",           /* _XkbErrMissingReqTypes    */
        "symbols not defined",                      /* _XkbErrMissingSymbols     */
        "virtual modifier bindings not defined",    /* _XkbErrMissingVMods       */
        "indicators not defined",                   /* _XkbErrMissingIndicators  */
        "compatibility map not defined",            /* _XkbErrMissingCompatMap   */
        "symbol interpretations not defined",       /* _XkbErrMissingSymInterps  */
        "geometry not defined",                     /* _XkbErrMissingGeometry    */
        "illegal doodad type",                      /* _XkbErrIllegalDoodad      */
        "illegal TOC type",                         /* _XkbErrIllegalTOCType     */
        "illegal contents",                         /* _XkbErrIllegalContents    */
        "empty file",                               /* _XkbErrEmptyFile          */
        "file not found",                           /* _XkbErrFileNotFound       */
        "cannot open",                              /* _XkbErrFileCannotOpen     */
        "bad value",                                /* _XkbErrBadValue           */
        "bad match",                                /* _XkbErrBadMatch           */
        "illegal name for type",                    /* _XkbErrBadTypeName        */
        "illegal width for type",                   /* _XkbErrBadTypeWidth       */
        "bad file type",                            /* _XkbErrBadFileType        */
        "bad file version",                         /* _XkbErrBadFileVersion     */
        "error in Xkm file",                        /* _XkbErrBadFileFormat      */
        "allocation failed",                        /* _XkbErrBadAlloc           */
        "bad length",                               /* _XkbErrBadLength          */
        "X request failed",                         /* _XkbErrXReqFailure        */
        "not implemented"                           /* _XkbErrBadImplementation  */
};

unsigned        _XkbErrCode;
const char *    _XkbErrLocation = NULL;
unsigned        _XkbErrData;
