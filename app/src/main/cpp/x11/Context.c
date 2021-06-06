#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xutil.h"
#ifdef XTHREADS
#include "locking.h"
#endif

#define INITHASHMASK 63 /* Number of entries originally in the hash table. */

typedef struct _TableEntryRec {	/* Stores one entry. */
    XID 			rid;
    XContext			context;
    XPointer			data;
    struct _TableEntryRec	*next;
} TableEntryRec, *TableEntry;

typedef struct _XContextDB {	/* Stores hash table for one display. */
    TableEntry *table;		/* Pointer to array of hash entries. */
    int mask;			/* Current size of hash table minus 1. */
    int numentries;		/* Number of entries currently in table. */
#ifdef XTHREADS
    LockInfoRec linfo;
#endif
} DBRec, *DB;

#ifdef MOTIFBC
static DB NullDB = (DB)0;
#endif

/* Given an XID and a context, returns a value between 0 and HashSize-1.
   Currently, this requires that HashSize be a power of 2.
*/

#define Hash(db,rid,context) \
    (db)->table[(((rid) << 1) + context) & (db)->mask]

/* Resize the given db */

static void ResizeTable(DB db)
{
    TableEntry *otable;
    register TableEntry entry, next, *pold, *head;
    register int i, j;

    otable = db->table;
    for (i = INITHASHMASK+1; (i + i) < db->numentries; )
        i += i;
    db->table = Xcalloc(i, sizeof(TableEntry));
    if (!db->table) {
        db->table = otable;
        return;
    }
    j = db->mask + 1;
    db->mask = i - 1;
    for (pold = otable ; --j >= 0; pold++) {
        for (entry = *pold; entry; entry = next) {
            next = entry->next;
            head = &Hash(db, entry->rid, entry->context);
            entry->next = *head;
            *head = entry;
        }
    }
    Xfree(otable);
}

static void _XFreeContextDB(Display *display)
{
    register DB db;
    register int i;
    register TableEntry *pentry, entry, next;

    db = display->context_db;
    if (db) {
        for (i = db->mask + 1, pentry = db->table ; --i >= 0; pentry++) {
            for (entry = *pentry; entry; entry = next) {
                next = entry->next;
                Xfree(entry);
            }
        }
        Xfree(db->table);
        _XFreeMutex(&db->linfo);
        Xfree(db);
        display->context_db = NULL;
    }
}

/* Public routines. */

/* Save the given value of data to correspond with the keys XID and context.
   Returns nonzero error code if an error has occurred, 0 otherwise.
   Possible errors are Out-of-memory.
*/

int XSaveContext(
        Display *display,
        register XID rid,
        register XContext context,
        _Xconst char* data)
{
    DB *pdb;
    register DB db;
    TableEntry *head;
    register TableEntry entry;

#ifdef MOTIFBC
    if (!display) {
	pdb = &NullDB;
	db = *pdb;
    } else
#endif
    {
        LockDisplay(display);
        pdb = &display->context_db;
        db = *pdb;
        UnlockDisplay(display);
    }
    if (!db) {
        db = Xmalloc(sizeof(DBRec));
        if (!db)
            return XCNOMEM;
        db->mask = INITHASHMASK;
        db->table = Xcalloc(db->mask + 1, sizeof(TableEntry));
        if (!db->table) {
            Xfree(db);
            return XCNOMEM;
        }
        db->numentries = 0;
        _XCreateMutex(&db->linfo);
#ifdef MOTIFBC
        if (!display) *pdb = db; else
#endif
        {
            LockDisplay(display);
            *pdb = db;
            display->free_funcs->context_db = _XFreeContextDB;
            UnlockDisplay(display);
        }
    }
    _XLockMutex(&db->linfo);
    head = &Hash(db, rid, context);
    _XUnlockMutex(&db->linfo);
    for (entry = *head; entry; entry = entry->next) {
        if (entry->rid == rid && entry->context == context) {
            entry->data = (XPointer)data;
            return 0;
        }
    }
    entry = Xmalloc(sizeof(TableEntryRec));
    if (!entry)
        return XCNOMEM;
    entry->rid = rid;
    entry->context = context;
    entry->data = (XPointer)data;
    entry->next = *head;
    *head = entry;
    _XLockMutex(&db->linfo);
    db->numentries++;
    if (db->numentries > (db->mask << 2))
        ResizeTable(db);
    _XUnlockMutex(&db->linfo);
    return 0;
}



/* Given an XID and context, returns the associated data.  Note that data
   here is a pointer since it is a return value.  Returns nonzero error code
   if an error has occurred, 0 otherwise.  Possible errors are Entry-not-found.
*/

int XFindContext(Display *display, XID rid, XContext context, XPointer *data)
{
    register DB db;
    register TableEntry entry;

#ifdef MOTIFBC
    if (!display) db = NullDB; else
#endif
    {
        LockDisplay(display);
        db = display->context_db;
        UnlockDisplay(display);
    }
    if (!db)
        return XCNOENT;
    _XLockMutex(&db->linfo);
    for (entry = Hash(db, rid, context); entry; entry = entry->next)
    {
        if (entry->rid == rid && entry->context == context) {
            *data = (XPointer)entry->data;
            _XUnlockMutex(&db->linfo);
            return 0;
        }
    }
    _XUnlockMutex(&db->linfo);
    return XCNOENT;
}



/* Deletes the entry for the given XID and context from the datastructure.
   This returns the same thing that FindContext would have returned if called
   with the same arguments.
*/

int XDeleteContext(Display *display, XID rid, XContext context)
{
    register DB db;
    register TableEntry entry, *prev;

#ifdef MOTIFBC
    if (!display) db = NullDB; else
#endif
    {
        LockDisplay(display);
        db = display->context_db;
        UnlockDisplay(display);
    }
    if (!db)
        return XCNOENT;
    _XLockMutex(&db->linfo);
    for (prev = &Hash(db, rid, context);
         (entry = *prev);
         prev = &entry->next) {
        if (entry->rid == rid && entry->context == context) {
            *prev = entry->next;
            Xfree(entry);
            db->numentries--;
            if (db->numentries < db->mask && db->mask > INITHASHMASK)
                ResizeTable(db);
            _XUnlockMutex(&db->linfo);
            return 0;
        }
    }
    _XUnlockMutex(&db->linfo);
    return XCNOENT;
}