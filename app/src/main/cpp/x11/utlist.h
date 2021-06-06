#ifndef UTLIST_H
#define UTLIST_H

#define UTLIST_VERSION 1.7

/* From: http://uthash.sourceforge.net/utlist.html */
/*
 * This file contains macros to manipulate singly and doubly-linked lists.
 *
 * 1. LL_ macros:  singly-linked lists.
 * 2. DL_ macros:  doubly-linked lists.
 * 3. CDL_ macros: circular doubly-linked lists.
 *
 * To use singly-linked lists, your structure must have a "next" pointer.
 * To use doubly-linked lists, your structure must "prev" and "next" pointers.
 * Either way, the pointer to the head of the list must be initialized to NULL.
 *
 * ----------------.EXAMPLE -------------------------
 * struct item {
 *      int id;
 *      struct item *prev, *next;
 * }
 *
 * struct item *list = NULL:
 *
 * int main() {
 *      struct item *item;
 *      ... allocate and populate item ...
 *      DL_APPEND(list, item);
 * }
 * --------------------------------------------------
 *
 * For doubly-linked lists, the append and delete macros are O(1)
 * For singly-linked lists, append and delete are O(n) but prepend is O(1)
 * The sort macro is O(n log(n)) for all types of single/double/circular lists.
 */


/******************************************************************************
 * doubly linked list macros (non-circular)                                   *
 *****************************************************************************/
#define DL_PREPEND(head,add)                                                     \
do {                                                                             \
 (add)->next = head;                                                             \
 if (head) {                                                                     \
   (add)->prev = (head)->prev;                                                   \
   (head)->prev = (add);                                                         \
 } else {                                                                        \
   (add)->prev = (add);                                                          \
 }                                                                               \
 (head) = (add);                                                                 \
} while (0)

#define DL_APPEND(head,add)                                                      \
do {                                                                             \
  if (head) {                                                                    \
      (add)->prev = (head)->prev;                                                \
      (head)->prev->next = (add);                                                \
      (head)->prev = (add);                                                      \
      (add)->next = NULL;                                                        \
  } else {                                                                       \
      (head)=(add);                                                              \
      (head)->prev = (head);                                                     \
      (head)->next = NULL;                                                       \
  }                                                                              \
} while (0);

#define DL_DELETE(head,del)                                                      \
do {                                                                             \
  if ((del)->prev == (del)) {                                                    \
      (head)=NULL;                                                               \
  } else if ((del)==(head)) {                                                    \
      (del)->next->prev = (del)->prev;                                           \
      (head) = (del)->next;                                                      \
  } else {                                                                       \
      (del)->prev->next = (del)->next;                                           \
      if ((del)->next) {                                                         \
          (del)->next->prev = (del)->prev;                                       \
      } else {                                                                   \
          (head)->prev = (del)->prev;                                            \
      }                                                                          \
  }                                                                              \
} while (0);


#define DL_FOREACH(head,el)                                                      \
    for(el=head;el;el=el->next)

#define DL_FOREACH_SAFE(head,el,tmp)                                             \
    for(el=head,tmp=el->next;el;el=tmp,tmp=(el) ? (el->next) : NULL)

#endif /* UTLIST_H */
