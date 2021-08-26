#include <stdlib.h>
#include <stdio.h>

#include "table.h"

static nop_handler_proc nop_handler = NULL;

void
table_set_noop_handler(nop_handler_proc func)
{
    nop_handler = func;
}

static void
noop_warn(const char *name)
{
    if (nop_handler) {
        nop_handler(name);
    }
    else {
        static int debug = -1;

        if (debug < 0)
            debug = (getenv("MESA_DEBUG") || getenv("LIBGL_DEBUG"));

        if (debug)
            fprintf(stderr, "%s is no-op\n", name);
    }
}

static int
noop_generic(void)
{
    noop_warn("function");
    return 0;
}

/* define noop_array */
#define MAPI_TMP_DEFINES
#define MAPI_TMP_NOOP_ARRAY
#include "mapi_tmp.h"
