#include "virgl_context.h"

#include <errno.h>

#include "mesa/util/os_misc.h"
#include "gallium/auxiliary/util/u_hash_table.h"
#include "gallium/auxiliary/util/u_pointer.h"
#include "virgl_util.h"

static struct util_hash_table *virgl_context_table;

static void
virgl_context_destroy_func(void *val)
{
    struct virgl_context *ctx = val;
    ctx->destroy(ctx);
}

int
virgl_context_table_init(void)
{
    virgl_context_table = util_hash_table_create(hash_func_u32,
                                                 compare_func,
                                                 virgl_context_destroy_func);
    return virgl_context_table ? 0 : ENOMEM;
}

void
virgl_context_table_cleanup(void)
{
    util_hash_table_destroy(virgl_context_table);
}

void
virgl_context_table_reset(void)
{
    util_hash_table_clear(virgl_context_table);
}

int
virgl_context_add(struct virgl_context *ctx)
{
    const enum pipe_error err = util_hash_table_set(
            virgl_context_table, uintptr_to_pointer(ctx->ctx_id), ctx);
    return err == PIPE_OK ? 0 : ENOMEM;
}

void
virgl_context_remove(uint32_t ctx_id)
{
    util_hash_table_remove(virgl_context_table, uintptr_to_pointer(ctx_id));
}

struct virgl_context *
virgl_context_lookup(uint32_t ctx_id)
{
    return util_hash_table_get(virgl_context_table,
                               uintptr_to_pointer(ctx_id));
}

static enum pipe_error
virgl_context_foreach_func(UNUSED void *key, void *val, void *data)
{
    const struct virgl_context_foreach_args *args = data;
    struct virgl_context *ctx = val;

    return args->callback(ctx, args->data) ? PIPE_OK : PIPE_ERROR;
}

void
virgl_context_foreach(const struct virgl_context_foreach_args *args)
{
    util_hash_table_foreach(virgl_context_table,
                            virgl_context_foreach_func,
                            (void *)args);
}
