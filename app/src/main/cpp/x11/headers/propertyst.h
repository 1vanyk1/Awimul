#ifndef PROPERTYSTRUCT_H
#define PROPERTYSTRUCT_H
#include "misc.h"
#include "property.h"
#include "privates.h"
#include <stdint.h>

/*
 *   PROPERTY -- property element
 */

typedef struct _Property {
    struct _Property *next;
    ATOM propertyName;
    ATOM type;                  /* ignored by server */
    uint32_t format;            /* format of data for swapping - 8,16,32 */
    uint32_t size;              /* size of data in (format/8) bytes */
    void *data;                 /* private to client */
    PrivateRec *devPrivates;
} PropertyRec;

#endif                          /* PROPERTYSTRUCT_H */
