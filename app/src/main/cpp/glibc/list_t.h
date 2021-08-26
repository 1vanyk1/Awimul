#ifndef _LIST_T_H
#define _LIST_T_H 1
/* Internal: doubly linked lists.  */
/* Basic type for the double-link list.  */
typedef struct list_head
{
    struct list_head *next;
    struct list_head *prev;
} list_t;
#endif /* list_t.h */
