#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include "xcb.h"
#include "xcbint.h"

typedef struct node {
    struct node *next;
    unsigned int key;
    void *data;
} node;

struct _xcb_map {
    node *head;
    node **tail;
};

/* Private interface */

_xcb_map *_xcb_map_new(void)
{
    _xcb_map *list;
    list = malloc(sizeof(_xcb_map));
    if(!list)
        return 0;
    list->head = 0;
    list->tail = &list->head;
    return list;
}

void _xcb_map_delete(_xcb_map *list, xcb_list_free_func_t do_free)
{
    if(!list)
        return;
    while(list->head)
    {
        node *cur = list->head;
        if(do_free)
            do_free(cur->data);
        list->head = cur->next;
        free(cur);
    }
    free(list);
}

int _xcb_map_put(_xcb_map *list, unsigned int key, void *data)
{
    node *cur = malloc(sizeof(node));
    if(!cur)
        return 0;
    cur->key = key;
    cur->data = data;
    cur->next = 0;
    *list->tail = cur;
    list->tail = &cur->next;
    return 1;
}

void *_xcb_map_remove(_xcb_map *list, unsigned int key)
{
    node **cur;
    for(cur = &list->head; *cur; cur = &(*cur)->next)
        if((*cur)->key == key)
        {
            node *tmp = *cur;
            void *ret = (*cur)->data;
            *cur = (*cur)->next;
            if(!*cur)
                list->tail = cur;

            free(tmp);
            return ret;
        }
    return 0;
}
