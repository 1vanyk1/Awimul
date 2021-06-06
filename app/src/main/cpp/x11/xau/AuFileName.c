
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xauth.h"
#include "../headers/xos.h"
#include <assert.h>
#include <stdlib.h>

static char *buf = NULL;

static void
free_filename_buffer(void)
{
    free(buf);
    buf = NULL;
}

char *
XauFileName (void)
{
    const char *slashDotXauthority = "/.Xauthority";
    char    *name;
    static size_t	bsize;
    static int atexit_registered = 0;
#ifdef WIN32
    char    dir[128];
#endif
    size_t  size;

    if ((name = getenv ("XAUTHORITY")))
        return name;
    name = getenv ("HOME");
    if (!name) {
#ifdef WIN32
        if ((name = getenv("USERNAME"))) {
	    snprintf(dir, sizeof(dir), "/users/%s", name);
	    name = dir;
	}
	if (!name)
#endif
        return NULL;
    }
    size = strlen (name) + strlen(&slashDotXauthority[1]) + 2;
    if ((size > bsize) || (buf == NULL)) {
        free (buf);
        assert(size > 0);
        buf = malloc (size);
        if (!buf) {
            bsize = 0;
            return NULL;
        }

        if (!atexit_registered) {
            atexit(free_filename_buffer);
            atexit_registered = 1;
        }

        bsize = size;
    }
    snprintf (buf, bsize, "%s%s", name,
              slashDotXauthority + (name[0] == '/' && name[1] == '\0' ? 1 : 0));
    return buf;
}
