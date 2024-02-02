#include <stdbool.h>
#include <string.h>
#include "remap.h"

#include "../mapi/glapi/glapi.h"

#define MAX_ENTRY_POINTS 16

#define need_MESA_remap_table
#include "remap_helper.h"
#include "errors.h"
#include "../../main_wm.h"


/* this is global for quick access */
int driDispatchRemapTable[driDispatchRemapTable_size];


/**
 * Map a function by its spec.  The function will be added to glapi,
 * and the dispatch offset will be returned.
 *
 * \param spec a '\0'-separated string array specifying a function.
 *        It begins with the parameter signature of the function,
 *        followed by the names of the entry points.  An empty entry
 *        point name terminates the array.
 *
 * \return the offset of the (re-)mapped function in the dispatch
 *         table, or -1.
 */
static int
map_function_spec(const char *spec)
{
    const char *signature;
    const char *names[MAX_ENTRY_POINTS + 1];
    int num_names = 0;

    if (!spec)
        return -1;

    signature = spec;
    spec += strlen(spec) + 1;

    /* spec is terminated by an empty string */
    while (*spec) {
        names[num_names] = spec;
        num_names++;
        if (num_names >= MAX_ENTRY_POINTS)
            break;
        spec += strlen(spec) + 1;
    }
    if (!num_names)
        return -1;

    names[num_names] = NULL;

    /* add the entry points to the dispatch table */
    return _glapi_add_dispatch(names, signature);
}


/**
 * Initialize the remap table.  This is called in one_time_init().
 * The remap table needs to be initialized before calling the
 * CALL/GET/SET macros defined in main/dispatch.h.
 */
void
_mesa_init_remap_table(void)
{
    static bool initialized = false;
    GLint i;

    if (initialized)
        return;
    initialized = true;

    /* initialize the MESA_remap_table_functions table */
    for (i = 0; i < driDispatchRemapTable_size; i++) {
        int offset;
        const char *spec;

        /* sanity check */
        assert(i == MESA_remap_table_functions[i].remap_index);
        spec = _mesa_function_pool + MESA_remap_table_functions[i].pool_index;

        offset = map_function_spec(spec);
        /* store the dispatch offset in the MESA_remap_table_functions table */
        driDispatchRemapTable[i] = offset;
        if (offset < 0) {
            const char *name = spec + strlen(spec) + 1;
            _mesa_warning(NULL, "failed to remap %s", name);
        }
    }
}
