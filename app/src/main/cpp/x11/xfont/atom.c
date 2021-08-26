#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "replace.h"
#include "../headers/fonts/fontmisc.h"

typedef struct _AtomList {
    char *name;
    int len;
    int hash;
    Atom atom;
} AtomListRec, *AtomListPtr;

static AtomListPtr *hashTable;

static int hashSize, hashUsed;
static int hashMask;
static int rehash;

static AtomListPtr *reverseMap;
static int reverseMapSize;
static Atom lastAtom;

static int
Hash(const char *string, int len)
{
    int h;

    h = 0;
    while (len--)
        h = (h << 3) ^ *string++;
    if (h < 0)
        return -h;
    return h;
}

static int
ResizeHashTable(void)
{
    int newHashSize;
    int newHashMask;
    AtomListPtr *newHashTable;
    int i;
    int h;
    int newRehash;
    int r;

    if (hashSize == 0)
        newHashSize = 1024;
    else
        newHashSize = hashSize * 2;
    newHashTable = calloc(newHashSize, sizeof(AtomListPtr));
    if (!newHashTable) {
        fprintf(stderr, "ResizeHashTable(): Error: Couldn't allocate"
                        " newHashTable (%ld)\n",
                newHashSize * (unsigned long) sizeof(AtomListPtr));
        return FALSE;
    }
    newHashMask = newHashSize - 1;
    newRehash = (newHashMask - 2);
    for (i = 0; i < hashSize; i++) {
        if (hashTable[i]) {
            h = (hashTable[i]->hash) & newHashMask;
            if (newHashTable[h]) {
                r = hashTable[i]->hash % newRehash | 1;
                do {
                    h += r;
                    if (h >= newHashSize)
                        h -= newHashSize;
                } while (newHashTable[h]);
            }
            newHashTable[h] = hashTable[i];
        }
    }
    free(hashTable);
    hashTable = newHashTable;
    hashSize = newHashSize;
    hashMask = newHashMask;
    rehash = newRehash;
    return TRUE;
}

static int
ResizeReverseMap(void)
{
    AtomListPtr *newMap;
    int newMapSize;

    if (reverseMapSize == 0)
        newMapSize = 1000;
    else
        newMapSize = reverseMapSize * 2;
    newMap = reallocarray(reverseMap, newMapSize, sizeof(AtomListPtr));
    if (newMap == NULL) {
        fprintf(stderr, "ResizeReverseMap(): Error: Couldn't reallocate"
                        " reverseMap (%ld)\n",
                newMapSize * (unsigned long) sizeof(AtomListPtr));
        return FALSE;
    }
    reverseMap = newMap;
    reverseMapSize = newMapSize;
    return TRUE;
}

static int
NameEqual(const char *a, const char *b, int l)
{
    while (l--)
        if (*a++ != *b++)
            return FALSE;
    return TRUE;
}

Atom
__libxfont_internal__MakeAtom(const char *string, unsigned len, int makeit)
{
    AtomListPtr a;
    int hash;
    int h = 0;
    int r;

    hash = Hash(string, len);
    if (hashTable) {
        h = hash & hashMask;
        if (hashTable[h]) {
            if (hashTable[h]->hash == hash && hashTable[h]->len == len &&
                NameEqual(hashTable[h]->name, string, len)) {
                return hashTable[h]->atom;
            }
            r = (hash % rehash) | 1;
            for (;;) {
                h += r;
                if (h >= hashSize)
                    h -= hashSize;
                if (!hashTable[h])
                    break;
                if (hashTable[h]->hash == hash && hashTable[h]->len == len &&
                    NameEqual(hashTable[h]->name, string, len)) {
                    return hashTable[h]->atom;
                }
            }
        }
    }
    if (!makeit)
        return None;
    a = malloc(sizeof(AtomListRec) + len + 1);
    if (a == NULL) {
        fprintf(stderr, "MakeAtom(): Error: Couldn't allocate AtomListRec"
                        " (%ld)\n", (unsigned long) sizeof(AtomListRec) + len + 1);
        return None;
    }
    a->name = (char *) (a + 1);
    a->len = len;
    strncpy(a->name, string, len);
    a->name[len] = '\0';
    a->atom = ++lastAtom;
    a->hash = hash;
    if (hashUsed >= hashSize / 2) {
        if ((ResizeHashTable() == FALSE) &&
            ((hashTable == NULL) || (hashUsed == hashSize)))
            return None;
        h = hash & hashMask;
        if (hashTable[h]) {
            r = (hash % rehash) | 1;
            do {
                h += r;
                if (h >= hashSize)
                    h -= hashSize;
            } while (hashTable[h]);
        }
    }
    hashTable[h] = a;
    hashUsed++;
    if (reverseMapSize <= a->atom) {
        if (!ResizeReverseMap())
            return None;
    }
    reverseMap[a->atom] = a;
    return a->atom;
}

int
__libxfont_internal__ValidAtom(Atom atom)
{
    return (atom != None) && (atom <= lastAtom);
}

const char *
__libxfont_internal__NameForAtom(Atom atom)
{
    if (atom != None && atom <= lastAtom)
        return reverseMap[atom]->name;
    return NULL;
}
