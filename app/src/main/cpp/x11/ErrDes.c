#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xos.h"
#include "headers/xresource.h"
#include <stdio.h>

#ifndef ERRORDB
#ifndef XERRORDB
#define ERRORDB "data/XErrorDB"
#else
#define ERRORDB XERRORDB
#endif
#endif

/*
 * descriptions of errors in Section 4 of Protocol doc (pp. 350-351); more
 * verbose descriptions are given in the error database
 */
static const char _XErrorList[] =
        /* No error */          "no error\0"
                                /* BadRequest */        "BadRequest\0"
                                /* BadValue */          "BadValue\0"
                                /* BadWindow */         "BadWindow\0"
                                /* BadPixmap */         "BadPixmap\0"
                                /* BadAtom */           "BadAtom\0"
                                /* BadCursor */         "BadCursor\0"
                                /* BadFont */           "BadFont\0"
                                /* BadMatch */          "BadMatch\0"
                                /* BadDrawable */       "BadDrawable\0"
                                /* BadAccess */         "BadAccess\0"
                                /* BadAlloc */          "BadAlloc\0"
                                /* BadColor */          "BadColor\0"
                                /* BadGC */             "BadGC\0"
                                /* BadIDChoice */       "BadIDChoice\0"
                                /* BadName */           "BadName\0"
                                /* BadLength */         "BadLength\0"
                                /* BadImplementation */ "BadImplementation"
;

/* offsets into _XErrorList */
static const unsigned char _XErrorOffsets[] = {
        0, 9, 20, 29, 39, 49, 57, 67, 75, 84, 96,
        106, 115, 124, 130, 142, 150, 160
};


int
XGetErrorText(
        register Display *dpy,
        register int code,
        char *buffer,
        int nbytes)
{
    char buf[150];
    register _XExtension *ext;
    _XExtension *bext = (_XExtension *)NULL;

    if (nbytes == 0) return 0;
    if (code <= BadImplementation && code > 0) {
        snprintf(buf, sizeof(buf), "%d", code);
        (void) XGetErrorDatabaseText(dpy, "XProtoError", buf,
                                     _XErrorList + _XErrorOffsets[code],
                                     buffer, nbytes);
    } else
        buffer[0] = '\0';
    /* call out to any extensions interested */
    for (ext = dpy->ext_procs; ext; ext = ext->next) {
        if (ext->error_string)
            (*ext->error_string)(dpy, code, &ext->codes, buffer, nbytes);
        if (ext->codes.first_error &&
            ext->codes.first_error <= code &&
            (!bext || ext->codes.first_error > bext->codes.first_error))
            bext = ext;
    }
    if (!buffer[0] && bext) {
        snprintf(buf, sizeof(buf), "%s.%d",
                 bext->name, code - bext->codes.first_error);
        (void) XGetErrorDatabaseText(dpy, "XProtoError", buf, "", buffer, nbytes);
    }
    if (!buffer[0])
        snprintf(buffer, nbytes, "%d", code);
    return 0;
}

int
/*ARGSUSED*/
XGetErrorDatabaseText(
        Display *dpy,
        register _Xconst char *name,
        register _Xconst char *type,
        _Xconst char *defaultp,
        char *buffer,
        int nbytes)
{

    static XrmDatabase db = NULL;
    XrmString type_str;
    XrmValue result;
    char temp[BUFSIZ];
    char* tptr;
    unsigned long tlen;

    if (nbytes == 0) return 0;

    if (!db) {
        /* the Xrm routines expect to be called with the global
           mutex unlocked. */
        XrmDatabase temp_db;
        int do_destroy;
        const char *dbname;

        XrmInitialize();
#ifdef WIN32
        dbname = getenv("XERRORDB");
	if (!dbname)
	    dbname = ERRORDB;
#else
        dbname = ERRORDB;
#endif
        temp_db = XrmGetFileDatabase(dbname);

        _XLockMutex(_Xglobal_lock);
        if (!db) {
            db = temp_db;
            do_destroy = 0;
        } else
            do_destroy = 1;	/* we didn't need to get it after all */
        _XUnlockMutex(_Xglobal_lock);

        if (do_destroy)
            XrmDestroyDatabase(temp_db);
    }

    if (db)
    {
        tlen = strlen (name) + strlen (type) + 2;
        if (tlen <= sizeof(temp))
            tptr = temp;
        else
            tptr = Xmalloc (tlen);
        if (tptr) {
            snprintf(tptr, tlen, "%s.%s", name, type);
            XrmGetResource(db, tptr, "ErrorType.ErrorNumber",
                           &type_str, &result);
            if (tptr != temp)
                Xfree (tptr);
        } else {
            result.addr = (XPointer) NULL;
        }
    }
    else
        result.addr = (XPointer)NULL;
    if (!result.addr) {
        result.addr = (XPointer) defaultp;
        result.size = (unsigned)strlen(defaultp) + 1;
    }
    (void) strncpy (buffer, (char *) result.addr, (size_t)nbytes);
    if (result.size > nbytes) buffer[nbytes-1] = '\0';
    return 0;
}