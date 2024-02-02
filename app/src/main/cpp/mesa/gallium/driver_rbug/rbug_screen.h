#ifndef RBUG_SCREEN_H
#define RBUG_SCREEN_H

#include "../../include/pipe/p_screen.h"
#include "../../include/pipe/p_defines.h"

#include "../../os/os_thread.h"

struct rbug_list {
    struct rbug_list *next;
    struct rbug_list *prev;
};


struct rbug_screen
{
    struct pipe_screen base;

    struct pipe_screen *screen;
    struct pipe_context *private_context;

    /* remote debugger */
    struct rbug_rbug *rbug;

    mtx_t list_mutex;
    int num_contexts;
    int num_resources;
    int num_surfaces;
    int num_transfers;
    struct rbug_list contexts;
    struct rbug_list resources;
    struct rbug_list surfaces;
    struct rbug_list transfers;
};

static inline struct rbug_screen *
rbug_screen(struct pipe_screen *screen)
{
    return (struct rbug_screen *)screen;
}

#define rbug_screen_add_to_list(scr, name, obj) \
   do {                                          \
      mtx_lock(&scr->list_mutex);          \
      insert_at_head(&scr->name, &obj->list);    \
      scr->num_##name++;                         \
      mtx_unlock(&scr->list_mutex);        \
   } while (0)

#define rbug_screen_remove_from_list(scr, name, obj) \
   do {                                               \
      mtx_lock(&scr->list_mutex);               \
      remove_from_list(&obj->list);                   \
      scr->num_##name--;                              \
      mtx_unlock(&scr->list_mutex);             \
   } while (0)



/**********************************************************
 * rbug_core.c
 */

struct rbug_rbug;

struct rbug_rbug *
rbug_start(struct rbug_screen *rb_screen);

void
rbug_stop(struct rbug_rbug *rbug);


#endif /* RBUG_SCREEN_H */
