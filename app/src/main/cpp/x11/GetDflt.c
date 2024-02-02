#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xos.h"
#include "headers/xresource.h"
#include "pathmax.h"

#ifdef XTHREADS
#include "headers/xthreads.h"
#endif
#ifndef WIN32
#define X_INCLUDE_PWD_H
#define XOS_USE_XLIB_LOCKING
#include "headers/xos_r.h"
#endif
#include <stdio.h>
#include <ctype.h>


/*ARGSUSED*/
static char *
GetHomeDir(
        char *dest,
        int len)
{
#ifdef WIN32
    register char *ptr1 = NULL;
    register char *ptr2 = NULL;
    int len1 = 0, len2 = 0;

    if ((ptr1 = getenv("HOME"))) {	/* old, deprecated */
	len1 = strlen (ptr1);
    } else if ((ptr1 = getenv("HOMEDRIVE")) && (ptr2 = getenv("HOMEDIR"))) {
	len1 = strlen (ptr1);
	len2 = strlen (ptr2);
    } else if ((ptr2 = getenv("USERNAME"))) {
	len1 = strlen (ptr1 = "/users/");
	len2 = strlen (ptr2);
    }
    if ((len1 + len2 + 1) < len)
	snprintf (dest, len, "%s%s", ptr1, (ptr2) ? ptr2 : "");
    else
	*dest = '\0';
#else
#ifdef X_NEEDS_PWPARAMS
    _Xgetpwparams pwparams;
#endif
    struct passwd *pw;
    register char *ptr;

    if (len <= 0 || dest == NULL)
        return NULL;

    if ((ptr = getenv("HOME"))) {
        (void) strncpy(dest, ptr, len-1);
        dest[len-1] = '\0';
    } else {
        if ((ptr = getenv("USER")))
            pw = _XGetpwnam(ptr,pwparams);
        else
            pw = _XGetpwuid(getuid(),pwparams);
        if (pw != NULL) {
            (void) strncpy(dest, pw->pw_dir, len-1);
            dest[len-1] = '\0';
        } else
            *dest = '\0';
    }
#endif
    return dest;
}


static XrmDatabase
InitDefaults(
        Display *dpy)			/* display for defaults.... */
{
    XrmDatabase userdb;
    XrmDatabase xdb;
    char fname[PATH_MAX];               /* longer than any conceivable size */
    char *xenv;

    XrmInitialize();

    /*
     * See lib/Xt/Initialize.c
     *
     * First, get the defaults from the server; if none, then load from
     * ~/.Xdefaults.  Next, if there is an XENVIRONMENT environment variable,
     * then load that file.
     */

    if (dpy->xdefaults == NULL) {
        const char *slashDotXdefaults = "/.Xdefaults";

        (void) GetHomeDir (fname, (int) (PATH_MAX - strlen (slashDotXdefaults) - 1));
        (void) strcat (fname, slashDotXdefaults);
        xdb = XrmGetFileDatabase (fname);
    } else {
        xdb = XrmGetStringDatabase(dpy->xdefaults);
    }

    if (!(xenv = getenv ("XENVIRONMENT"))) {
        const char *slashDotXdefaultsDash = "/.Xdefaults-";
        int len;

        (void) GetHomeDir (fname, (int) (PATH_MAX - strlen (slashDotXdefaultsDash) - 1));
        (void) strcat (fname, slashDotXdefaultsDash);
        len = (int) strlen (fname);
        (void) _XGetHostname (fname+len, PATH_MAX-len);
        xenv = fname;
    }
    userdb = XrmGetFileDatabase (xenv);
    XrmMergeDatabases (userdb, &xdb);
    return (xdb);

#ifdef old
    if (fname[0] != '\0') userdb =  XrmGetFileDatabase(fname);
    xdb = XrmGetStringDatabase(dpy->xdefaults);
    XrmMergeDatabases(userdb, &xdb);
    return xdb;
#endif
}

char *
XGetDefault(
        Display *dpy,			/* display for defaults.... */
        char _Xconst *prog,		/* name of program for option	*/
register _Xconst char *name)	/* name of option program wants */
{					/* to get, for example, "font"  */
XrmName names[3];
XrmClass classes[3];
XrmRepresentation fromType;
XrmValue result;
char *progname;
#ifdef WIN32
char *progname2;
#endif
#ifdef __UNIXOS2__
char *progname2;
	char *dotpos;
#endif

/*
 * strip path off of program name (XXX - this is OS specific)
 */
progname = strrchr (prog, '/');
#ifdef WIN32
progname2 = strrchr (prog, '\\');
	if (progname2 && (!progname || progname < progname2))
	    progname = progname2;
#endif
#ifdef __UNIXOS2__  /* Very similar to WIN32 */
progname2 = strrchr (prog, '\\');
	if (progname2 && (!progname || progname < progname2))
	    progname = progname2;
	dotpos = strrchr (prog, '.');
	if (dotpos && (dotpos>progname2)) *dotpos='\0';
#endif  /* We take out the .exe suffix  */

if (progname)
progname++;
else
progname = (char *)prog;

/*
 * see if database has ever been initialized.  Lookups can be done
 * without locks held.
 */
LockDisplay(dpy);
if (dpy->db == NULL) {
dpy->db = InitDefaults(dpy);
dpy->flags |= XlibDisplayDfltRMDB;
}
UnlockDisplay(dpy);

names[0] = XrmStringToName(progname);
names[1] = XrmStringToName(name);
names[2] = NULLQUARK;
classes[0] = XrmStringToClass("Program");
classes[1] = XrmStringToClass("Name");
classes[2] = NULLQUARK;
(void)XrmQGetResource(dpy->db, names, classes, &fromType, &result);
return (result.addr);
}

