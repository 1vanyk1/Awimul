#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../x11/headers/keysym.h"

#if defined(sgi)
#include <malloc.h>
#endif

#define	DEBUG_VAR listingDebug
#include "xkbcomp.h"
#include <stdlib.h>

#ifdef _POSIX_SOURCE
# include <limits.h>
#else
# define _POSIX_SOURCE
# include <limits.h>
# undef _POSIX_SOURCE
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

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include "../x11/headers/xwindows.h"
# define FileName(file) file.cFileName
# undef TEXT
# undef ALTERNATE
#else
# include <dirent.h>
# define FileName(file) file->d_name
#endif

#include "xkbpath.h"
#include "parseutils.h"
#include "misc.h"
#include "tokens.h"
#include "../x11/headers/extensions/XKBgeom.h"

#define	lowbit(x)	((x) & (-(x)))

unsigned int listingDebug;

static int szListing = 0;
static int nListed = 0;
static int nFilesListed = 0;

typedef struct _Listing
{
    char *file;
    char *map;
} Listing;

static int szMapOnly;
static int nMapOnly;
static char **mapOnly;

static Listing *list = NULL;

/***====================================================================***/

int
AddMapOnly(char *map)
{
    if (nMapOnly >= szMapOnly)
    {
        if (szMapOnly < 1)
            szMapOnly = 5;
        else
            szMapOnly *= 2;
        mapOnly = uTypedRealloc(list, szMapOnly, char *);
        if (!mapOnly)
        {
            WSGO("Couldn't allocate list of maps\n");
            return 0;
        }
    }
    mapOnly[nMapOnly++] = map;
    return 1;
}

int
AddListing(char *file, char *map)
{
    if (nListed >= szListing)
    {
        if (szListing < 1)
            szListing = 10;
        else
            szListing *= 2;
        list = uTypedRealloc(list, szListing, Listing);
        if (!list)
        {
            WSGO("Couldn't allocate list of files and maps\n");
            ACTION("Exiting\n");
            exit(1);
        }
    }

    list[nListed].file = file;
    list[nListed].map = map;
    nListed++;
    if (file != NULL)
        nFilesListed++;
    return 1;
}

/***====================================================================***/

static void
ListFile(FILE * outFile, char *fileName, XkbFile * map)
{
    register unsigned flags;
    char *mapName;

    flags = map->flags;
    if ((flags & XkbLC_Hidden) && (!(verboseLevel & WantHiddenMaps)))
        return;
    if ((flags & XkbLC_Partial) && (!(verboseLevel & WantPartialMaps)))
        return;
    if (verboseLevel & WantLongListing)
    {
        fprintf(outFile, (flags & XkbLC_Hidden) ? "h" : "-");
        fprintf(outFile, (flags & XkbLC_Default) ? "d" : "-");
        fprintf(outFile, (flags & XkbLC_Partial) ? "p" : "-");
        fprintf(outFile, "----- ");
        if (map->type == XkmSymbolsIndex)
        {
            fprintf(outFile, (flags & XkbLC_AlphanumericKeys) ? "a" : "-");
            fprintf(outFile, (flags & XkbLC_ModifierKeys) ? "m" : "-");
            fprintf(outFile, (flags & XkbLC_KeypadKeys) ? "k" : "-");
            fprintf(outFile, (flags & XkbLC_FunctionKeys) ? "f" : "-");
            fprintf(outFile, (flags & XkbLC_AlternateGroup) ? "g" : "-");
            fprintf(outFile, "--- ");
        }
        else
            fprintf(outFile, "-------- ");
    }
    mapName = map->name;
    if ((!(verboseLevel & WantFullNames)) && ((flags & XkbLC_Default) != 0))
        mapName = NULL;
    if (dirsToStrip > 0)
    {
        char *tmp, *last;
        int i;
        for (i = 0, tmp = last = fileName; (i < dirsToStrip) && tmp; i++)
        {
            last = tmp;
            tmp = strchr(tmp, '/');
            if (tmp != NULL)
                tmp++;
        }
        fileName = (tmp ? tmp : last);
    }
    if (mapName)
        fprintf(outFile, "%s(%s)\n", fileName, mapName);
    else
        fprintf(outFile, "%s\n", fileName);
    return;
}

/***====================================================================***/

static int
AddDirectory(char *head, char *ptrn, char *rest, char *map)
{
#ifdef WIN32
    HANDLE dirh;
    WIN32_FIND_DATA file;
#else
    DIR *dirp;
    struct dirent *file;
#endif
    int nMatch;

    if (map == NULL)
    {
        char *tmp = ptrn;
        if ((rest == NULL) && (ptrn != NULL) && (strchr(ptrn, '/') == NULL))
        {
            tmp = ptrn;
            map = strchr(ptrn, '(');
        }
        else if ((rest == NULL) && (ptrn == NULL) &&
                 (head != NULL) && (strchr(head, '/') == NULL))
        {
            tmp = head;
            map = strchr(head, '(');
        }
        if (map != NULL)
        {
            tmp = strchr(tmp, ')');
            if ((tmp == NULL) || (tmp[1] != '\0'))
            {
                ERROR("File and map must have the format file(map)\n");
                return 0;
            }
            *map = '\0';
            map++;
            *tmp = '\0';
        }
    }
#ifdef WIN32
    if ((dirh = FindFirstFile("*.*", &file)) == INVALID_HANDLE_VALUE)
        return 0;
#else
    if ((dirp = opendir((head ? head : "."))) == NULL)
        return 0;
#endif
    nMatch = 0;
#ifdef WIN32
    do
#else
    while ((file = readdir(dirp)) != NULL)
#endif
    {
        char *tmp, *filename;
        struct stat sbuf;
        size_t tmpsize;

        filename = FileName(file);
        if (!filename || filename[0] == '.')
            continue;
        if (ptrn && (!XkbNameMatchesPattern2(filename, ptrn)))
            continue;
        tmpsize = (head ? strlen(head) : 0) + strlen(filename) + 2;
        tmp = uAlloc(tmpsize);
        if (!tmp)
            continue;
        snprintf(tmp, tmpsize, "%s%s%s",
                 (head ? head : ""), (head ? "/" : ""), filename);
        if (stat(tmp, &sbuf) < 0)
        {
            uFree(tmp);
            continue;
        }
        if (((rest != NULL) && (!S_ISDIR(sbuf.st_mode))) ||
            ((map != NULL) && (S_ISDIR(sbuf.st_mode))))
        {
            uFree(tmp);
            continue;
        }
        if (S_ISDIR(sbuf.st_mode))
        {
            if ((rest != NULL) || (verboseLevel & ListRecursive))
                nMatch += AddDirectory(tmp, rest, NULL, map);
        }
        else
            nMatch += AddListing(tmp, map);
    }
#ifdef WIN32
    while (FindNextFile(dirh, &file));
#endif
    return nMatch;
}

/***====================================================================***/

Bool
AddMatchingFiles(char *head_in)
{
    char *str, *head, *ptrn, *rest = NULL;

    if (head_in == NULL)
        return 0;
    ptrn = NULL;
    for (str = head_in; (*str != '\0') && (*str != '?') && (*str != '*');
         str++)
    {
        if ((str != head_in) && (*str == '/'))
            ptrn = str;
    }
    if (*str == '\0')
    {                           /* no wildcards */
        head = head_in;
        ptrn = NULL;
        rest = NULL;
    }
    else if (ptrn == NULL)
    {                           /* no slash before the first wildcard */
        head = NULL;
        ptrn = head_in;
    }
    else
    {                           /* slash followed by wildcard */
        head = head_in;
        *ptrn = '\0';
        ptrn++;
    }
    if (ptrn)
    {
        rest = strchr(ptrn, '/');
        if (rest != NULL)
        {
            *rest = '\0';
            rest++;
        }
    }
    if (((rest && ptrn)
         && ((strchr(ptrn, '(') != NULL) || (strchr(ptrn, ')') != NULL)))
        || (head
            && ((strchr(head, '(') != NULL) || (strchr(head, ')') != NULL))))
    {
        ERROR("Files/maps to list must have the form file(map)\n");
        ACTION("Illegal specifier ignored\n");
        return 0;
    }
    return AddDirectory(head, ptrn, rest, NULL);
}

/***====================================================================***/

static Bool
MapMatches(char *mapToConsider, char *ptrn)
{
    int i;

    if (ptrn != NULL)
        return XkbNameMatchesPattern2(mapToConsider, ptrn);
    if (nMapOnly < 1)
        return True;
    for (i = 0; i < nMapOnly; i++)
    {
        if (XkbNameMatchesPattern2(mapToConsider, mapOnly[i]))
            return True;
    }
    return False;
}

int
GenerateListing(char *out_name)
{
    int i;
    FILE *inputFile, *outFile;
    XkbFile *rtrn, *mapToUse;
    unsigned oldWarningLevel;
    char *mapName;

    if (nFilesListed < 1)
    {
        ERROR("Must specify at least one file or pattern to list\n");
        return 0;
    }
    if ((!out_name) || ((out_name[0] == '-') && (out_name[1] == '\0')))
        outFile = stdout;
    else if ((outFile = fopen(out_name, "w")) == NULL)
    {
        ERROR("Cannot open \"%s\" to write keyboard description\n",
              out_name);
        ACTION("Exiting\n");
        return 0;
    }
#ifdef DEBUG
    if (warningLevel > 9)
        fprintf(stderr, "should list:\n");
#endif
    for (i = 0; i < nListed; i++)
    {
#ifdef DEBUG
        if (warningLevel > 9)
        {
            fprintf(stderr, "%s(%s)\n",
                    (list[i].file ? list[i].file : "*"),
                    (list[i].map ? list[i].map : "*"));
        }
#endif
        oldWarningLevel = warningLevel;
        warningLevel = 0;
        if (list[i].file)
        {
            struct stat sbuf;

            if (stat(list[i].file, &sbuf) < 0)
            {
                if (oldWarningLevel > 5)
                    WARN("Couldn't open \"%s\"\n", list[i].file);
                continue;
            }
            if (S_ISDIR(sbuf.st_mode))
            {
                if (verboseLevel & ListRecursive)
                    AddDirectory(list[i].file, NULL, NULL, NULL);
                continue;
            }

            inputFile = fopen(list[i].file, "r");
            if (!inputFile)
            {
                if (oldWarningLevel > 5)
                    WARN("Couldn't open \"%s\"\n", list[i].file);
                continue;
            }
            setScanState(list[i].file, 1);
            if (XKBParseFile(inputFile, &rtrn) && (rtrn != NULL))
            {
                mapName = list[i].map;
                mapToUse = rtrn;
                for (; mapToUse; mapToUse = (XkbFile *) mapToUse->common.next)
                {
                    if (!MapMatches(mapToUse->name, mapName))
                        continue;
                    ListFile(outFile, list[i].file, mapToUse);
                }
            }
            fclose(inputFile);
        }
        warningLevel = oldWarningLevel;
    }
    return 1;
}
