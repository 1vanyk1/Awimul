#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../headers/xlibint.h"
#include "xlcPubI.h"
#include "../headers/xos.h"
#if 0
#include <unistd.h>  /* in theory delivers getresuid/gid prototypes,
		      * in practice only the Linux syscall wrapper is there. */
#endif


/************************************************************************/

#ifdef __UNIXOS2__
# define seteuid setuid
#endif
#define	iscomment(ch)	((ch) == '#' || (ch) == '\0')
#if defined(WIN32)
#define isreadable(f)	(_XAccessFile(f))
#else
#define isreadable(f)	((access((f), R_OK) != -1) ? 1 : 0)
#endif

#ifndef __UNIXOS2__
#define LC_PATHDELIM ':'
#else
#define LC_PATHDELIM ';'
#endif

#define XLC_BUFSIZE 256

#ifndef X_NOT_POSIX
#ifdef _POSIX_SOURCE
#include <limits.h>
#else
#define _POSIX_SOURCE
#include <limits.h>
#undef _POSIX_SOURCE
#endif
#endif
#ifndef PATH_MAX
#ifdef WIN32
#define PATH_MAX 512
#else
#include <sys/param.h>
#endif
#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif
#endif

#define NUM_LOCALEDIR	64

/* Splits a NUL terminated line into constituents, at colons and newline
   characters. Leading whitespace is removed from constituents. The
   constituents are stored at argv[0..argsize-1]. The number of stored
   constituents (<= argsize) is returned. The line is destructively
   modified. */
static int
parse_line(
        char *line,
        char **argv,
        int argsize)
{
    int argc = 0;
    char *p = line;

    while (argc < argsize) {
        while (isspace(*p)) {
            ++p;
        }
        if (*p == '\0') {
            break;
        }
        argv[argc++] = p;
        while (*p != ':' && *p != '\n' && *p != '\0') {
            ++p;
        }
        if (*p == '\0') {
            break;
        }
        *p++ = '\0';
    }

    return argc;
}

#ifdef __UNIXOS2__

/* fg021216: entries in locale files are separated by colons while under
   OS/2, path entries are separated by semicolon, so we need two functions */

static int
parse_line1(
    char *line,
    char **argv,
    int argsize)
{
    int argc = 0;
    char *p = line;

    while (argc < argsize) {
	while (isspace(*p)) {
	    ++p;
	}
	if (*p == '\0') {
	    break;
	}
	argv[argc++] = p;
	while (*p != ';' && *p != '\n' && *p != '\0') {
	    ++p;
	}
	if (*p == '\0') {
	    break;
	}
	*p++ = '\0';
    }

    return argc;
}
#endif   /* __UNIXOS2__ */

/* Splits a colon separated list of directories, and returns the constituent
   paths (without trailing slash). At most argsize constituents are stored
   at argv[0..argsize-1]. The number of stored constituents is returned. */
static int
_XlcParsePath(
        char *path,
        char **argv,
        int argsize)
{
    char *p = path;
    int n, i;

#ifndef __UNIXOS2__
    n = parse_line(path, argv, argsize);
#else
    n = parse_line1(path, argv, argsize);
#endif
    for (i = 0; i < n; ++i) {
        int len;
        p = argv[i];
        len = strlen(p);
        if (len > 0 && p[len - 1] == '/') {
            /* eliminate trailing slash */
            p[len - 1] = '\0';
        }
    }
    return n;
}

#ifndef XLOCALEDIR
#define XLOCALEDIR "/usr/lib/X11/locale"
#endif

void
xlocaledir(
        char *buf,
        int buf_len)
{
    char *p = buf;
    int len = 0;

#ifndef NO_XLOCALEDIR
    char *dir;
    int priv = 1;

    dir = getenv("XLOCALEDIR");

    if (dir) {
        /*
         * Only use the user-supplied path if the process isn't priviledged.
         */
        if (getuid() == geteuid() && getgid() == getegid()) {
#if defined(HASSETUGID)
            priv = issetugid();
#elif defined(HASGETRESUID)
            {
		uid_t ruid, euid, suid;
		gid_t rgid, egid, sgid;
		if ((getresuid(&ruid, &euid, &suid) == 0) &&
		    (getresgid(&rgid, &egid, &sgid) == 0))
		    priv = (euid != suid) || (egid != sgid);
	    }
#else
            /*
             * If there are saved ID's the process might still be priviledged
             * even though the above test succeeded.  If issetugid() and
             * getresgid() aren't available, test this by trying to set
             * euid to 0.
             *
             * Note: this only protects setuid-root clients.  It doesn't
             * protect other setuid or any setgid clients.  If this tradeoff
             * isn't acceptable, set DisableXLocaleDirEnv to YES in host.def.
             */
            unsigned int oldeuid;
            oldeuid = geteuid();
            if (seteuid(0) != 0) {
                priv = 0;
            } else {
                seteuid(oldeuid);
                priv = 1;
            }
#endif
        }
        if (!priv) {
            len = strlen(dir);
            strncpy(p, dir, buf_len);
            if (len < buf_len) {
                p[len++] = LC_PATHDELIM;
                p += len;
            }
        }
    }
#endif /* NO_XLOCALEDIR */

    if (len < buf_len)
#ifndef __UNIXOS2__
        strncpy(p, XLOCALEDIR, buf_len - len);
#else
    strncpy(p,__XOS2RedirRoot(XLOCALEDIR), buf_len - len);
#endif
    buf[buf_len-1] = '\0';
}

/* Mapping direction */
typedef enum {
    LtoR,		/* Map first field to second field */
    RtoL		/* Map second field to first field */
} MapDirection;

static char *
resolve_name(
        const char *lc_name,
        char *file_name,
        MapDirection direction)
{
    FILE *fp;
    char buf[XLC_BUFSIZE], *name = NULL;

    fp = _XFopenFile (file_name, "r");
    if (fp == NULL)
        return NULL;

    while (fgets(buf, XLC_BUFSIZE, fp) != NULL) {
        char *p = buf;
        int n;
        char *args[2], *from, *to;
#ifdef __UNIXOS2__  /* Take out CR under OS/2 */
        int len;

	len = strlen(p);
	if (len > 1) {
	    if (*(p+len-2) == '\r' && *(p+len-1) == '\n') {
		*(p+len-2) = '\n';
		*(p+len-1) = '\0';
	    }
	}
#endif
        while (isspace(*p)) {
            ++p;
        }
        if (iscomment(*p)) {
            continue;
        }
        n = parse_line(p, args, 2);		/* get first 2 fields */
        if (n != 2) {
            continue;
        }
        if (direction == LtoR) {
            from = args[0], to = args[1];	/* left to right */
        } else {
            from = args[1], to = args[0];	/* right to left */
        }
        if (! strcmp(from, lc_name)) {
            name = Xmalloc(strlen(to) + 1);
            if (name != NULL) {
                strcpy(name, to);
            }
            break;
        }
    }
    fclose(fp);
    return name;
}

#define	c_tolower(ch)	((ch) >= 'A' && (ch) <= 'Z' ? (ch) - 'A' + 'a' : (ch))

static char *
lowercase(
        char *dst,
        const char *src)
{
    const char *s;
    char *t;

    for (s = src, t = dst; *s; ++s, ++t)
        *t = c_tolower(*s);
    *t = '\0';
    return dst;
}

/************************************************************************/
char *
_XlcFileName(
        XLCd lcd,
        const char *category)
{
    char *siname;
    char cat[XLC_BUFSIZE], dir[XLC_BUFSIZE];
    int i, n;
    char *args[NUM_LOCALEDIR];
    char *file_name = NULL;

    if (lcd == (XLCd)NULL)
        return NULL;

    siname = XLC_PUBLIC(lcd, siname);

    lowercase(cat, category);
    xlocaledir(dir,XLC_BUFSIZE);
    n = _XlcParsePath(dir, args, NUM_LOCALEDIR);
    for (i = 0; i < n; ++i) {
        char buf[PATH_MAX], *name;

        name = NULL;
        if ((5 + (args[i] ? strlen (args[i]) : 0) +
             (cat ? strlen (cat) : 0)) < PATH_MAX) {
            sprintf(buf, "%s/%s.dir", args[i], cat);
            name = resolve_name(siname, buf, RtoL);
        }
        if (name == NULL) {
            continue;
        }
        if (*name == '/') {
            /* supposed to be absolute path name */
            file_name = name;
        } else {
            file_name = Xmalloc(2 + (args[i] ? strlen (args[i]) : 0) +
                                (name ? strlen (name) : 0));
            if (file_name != NULL)
                sprintf(file_name, "%s/%s", args[i], name);
            Xfree(name);
        }
        if (isreadable(file_name)) {
            break;
        }
        Xfree(file_name);
        file_name = NULL;
        /* Then, try with next dir */
    }
    return file_name;
}

/************************************************************************/
#ifndef LOCALE_ALIAS
#define LOCALE_ALIAS    "locale.alias"
#endif

int
_XlcResolveLocaleName(
        const char* lc_name,
        XLCdPublicPart* pub)
{
    char dir[PATH_MAX], buf[PATH_MAX], *name = NULL;
    char *dst;
    int i, n, sinamelen;
    char *args[NUM_LOCALEDIR];
    static const char locale_alias[] = LOCALE_ALIAS;
    char *tmp_siname;

    xlocaledir (dir, PATH_MAX);
    n = _XlcParsePath(dir, args, NUM_LOCALEDIR);
    for (i = 0; i < n; ++i) {
        if ((2 + (args[i] ? strlen (args[i]) : 0) +
             strlen (locale_alias)) < PATH_MAX) {
            sprintf (buf, "%s/%s", args[i], locale_alias);
            name = resolve_name (lc_name, buf, LtoR);
        }
        if (name != NULL) {
            break;
        }
    }

    if (name == NULL) {
        /* vendor locale name == Xlocale name, no expansion of alias */
        pub->siname = Xmalloc (strlen (lc_name) + 1);
        strcpy (pub->siname, lc_name);
    } else {
        pub->siname = name;
    }

    sinamelen = strlen (pub->siname);
    if (sinamelen == 1 && pub->siname[0] == 'C') {
        pub->language = pub->siname;
        pub->territory = pub->codeset = NULL;
        return 1;
    }

    /* 
     * pub->siname is in the format <lang>_<terr>.<codeset>, typical would
     * be "en_US.ISO8859-1", "en_US.utf8", "ru_RU.KOI-8", or ja_JP.SJIS,
     * although it could be ja.SJIS too.
     */
    tmp_siname = Xrealloc (pub->siname, 2 * (sinamelen + 1));
    if (tmp_siname == NULL) {
        return 0;
    }
    pub->siname = tmp_siname;

    /* language */
    dst = &pub->siname[sinamelen + 1];
    strcpy (dst, pub->siname);
    pub->language = dst;

    /* territory */
    dst = strchr (dst, '_');
    if (dst) {
        *dst = '\0';
        pub->territory = ++dst;
    } else
        dst = &pub->siname[sinamelen + 1];

    /* codeset */
    dst = strchr (dst, '.');
    if (dst) {
        *dst = '\0';
        pub->codeset = ++dst;
    }

    return (pub->siname[0] != '\0') ? 1 : 0;
}

/************************************************************************/
int
_XlcResolveI18NPath(buf, buf_len)
        char *buf;
        int buf_len;
{
    if (buf != NULL) {
        xlocaledir(buf, buf_len);
    }
    return 1;
}

char *
_XlcLocaleDirName(dir_name, dir_len, lc_name)
        char *dir_name;
        size_t dir_len;
        char *lc_name;
{
    char dir[PATH_MAX], buf[PATH_MAX], *name = NULL;
    int i, n;
    char *args[NUM_LOCALEDIR];
    static char locale_alias[] = LOCALE_ALIAS;
    char *target_name = (char*)0;
    char *target_dir = (char*)0;

    xlocaledir (dir, PATH_MAX);
    n = _XlcParsePath(dir, args, 256);
    for (i = 0; i < n; ++i) {

        if ((2 + (args[i] ? strlen(args[i]) : 0) +
             strlen(locale_alias)) < PATH_MAX) {
            sprintf (buf, "%s/%s", args[i], locale_alias);
            name = resolve_name(lc_name, buf, LtoR);
        }

        /* If name is not an alias, use lc_name for locale.dir search */
        if (name == NULL)
            name = lc_name;

        /* look at locale.dir */

        target_dir = args[i];
        if (!target_dir) {
            /* something wrong */
            if (name != lc_name)
                Xfree(name);
            continue;
        }
        if ((1 + (target_dir ? strlen (target_dir) : 0) +
             strlen("locale.dir")) < PATH_MAX) {
            sprintf(buf, "%s/locale.dir", target_dir);
            target_name = resolve_name(name, buf, RtoL);
        }
        if (name != lc_name)
            Xfree(name);
        if (target_name != NULL) {
            char *p = 0;
            if ((p = strstr(target_name, "/XLC_LOCALE"))) {
                *p = '\0';
                break;
            }
            Xfree(target_name);
            target_name = NULL;
        }
        name = NULL;
    }
    if (target_name == NULL) {
        /* vendor locale name == Xlocale name, no expansion of alias */
        target_dir = args[0];
        target_name = lc_name;
    }
    /* snprintf(dir_name, dir_len, "%s/%", target_dir, target_name); */
    strncpy(dir_name, target_dir, dir_len - 1);
    if (strlen(target_dir) >= dir_len - 1) {
        dir_name[dir_len - 1] = '\0';
    } else  {
        strcat(dir_name, "/");
        strncat(dir_name, target_name, dir_len - strlen(dir_name) - 1);
        if (strlen(target_name) >= dir_len - strlen(dir_name) - 1)
            dir_name[dir_len - 1] = '\0';
    }
    if (target_name != lc_name)
        Xfree(target_name);
    return dir_name;
}