#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "../headers/xos.h"
#include "../headers/xlib.h"
#include "../headers/XKBlib.h"

#include "../headers/extensions/XKMformat.h"
#include "XKBfileInt.h"

/***====================================================================***/

#define InitialTableSize 100

typedef struct _Node {
    struct _Node *left, *right;
    Atom a;
    unsigned int fingerPrint;
    char *string;
} NodeRec, *NodePtr;

#define BAD_RESOURCE 0xe0000000

static Atom lastAtom = None;
static NodePtr atomRoot = (NodePtr) NULL;
static unsigned long tableLength;
static NodePtr *nodeTable;

static Atom
_XkbMakeAtom(const char *string, size_t len, Bool makeit)
{
    register NodePtr *np;
    unsigned i;
    int comp;
    register unsigned int fp = 0;

    np = &atomRoot;
    for (i = 0; i < (len + 1) / 2; i++) {
        fp = fp * 27 + string[i];
        fp = fp * 27 + string[len - 1 - i];
    }
    while (*np != (NodePtr) NULL) {
        if (fp < (*np)->fingerPrint)
            np = &((*np)->left);
        else if (fp > (*np)->fingerPrint)
            np = &((*np)->right);
        else {                  /* now start testing the strings */
            comp = strncmp(string, (*np)->string, len);
            if ((comp < 0) || ((comp == 0) && (len < strlen((*np)->string))))
                np = &((*np)->left);
            else if (comp > 0)
                np = &((*np)->right);
            else
                return (*np)->a;
        }
    }
    if (makeit) {
        register NodePtr nd;

        nd = (NodePtr) _XkbAlloc(sizeof(NodeRec));
        if (!nd)
            return BAD_RESOURCE;
#ifdef HAVE_STRNDUP
        nd->string = strndup(string, len);
#else
        nd->string = (char *) _XkbAlloc(len + 1);
#endif
        if (!nd->string) {
            _XkbFree(nd);
            return BAD_RESOURCE;
        }
#ifndef HAVE_STRNDUP
        strncpy(nd->string, string, len);
        nd->string[len] = 0;
#endif
        if ((lastAtom + 1) >= tableLength) {
            NodePtr *table;

            table = (NodePtr *) _XkbRealloc(nodeTable,
                                            tableLength * (2 *
                                                           sizeof(NodePtr)));
            if (!table) {
                if (nd->string != string)
                    _XkbFree(nd->string);
                _XkbFree(nd);
                return BAD_RESOURCE;
            }
            tableLength <<= 1;
            nodeTable = table;
        }
        *np = nd;
        nd->left = nd->right = (NodePtr) NULL;
        nd->fingerPrint = fp;
        nd->a = (++lastAtom);
        *(nodeTable + lastAtom) = nd;
        return nd->a;
    }
    else
        return None;
}

static char *
_XkbNameForAtom(Atom atom)
{
    NodePtr node;

    if (atom > lastAtom)
        return NULL;
    if ((node = nodeTable[atom]) == (NodePtr) NULL)
        return NULL;
    return strdup(node->string);
}

static void
_XkbInitAtoms(void)
{
    tableLength = InitialTableSize;
    nodeTable = (NodePtr *) _XkbAlloc(InitialTableSize * sizeof(NodePtr));
    nodeTable[None] = (NodePtr) NULL;
}

/***====================================================================***/

char *
XkbAtomGetString(Display *dpy, Atom atm)
{
    if (atm == None)
        return NULL;
    if (dpy == NULL)
        return _XkbNameForAtom(atm);
    return XGetAtomName(dpy, atm);
}

/***====================================================================***/

Atom
XkbInternAtom(Display *dpy, const char *name, Bool onlyIfExists)
{
    if (name == NULL)
        return None;
    if (dpy == NULL) {
        return _XkbMakeAtom(name, strlen(name), (!onlyIfExists));
    }
    return XInternAtom(dpy, name, onlyIfExists);
}

/***====================================================================***/

Atom
XkbChangeAtomDisplay(Display *oldDpy, Display *newDpy, Atom atm)
{
    char *tmp;

    if (atm != None) {
        tmp = XkbAtomGetString(oldDpy, atm);
        if (tmp != NULL)
            return XkbInternAtom(newDpy, tmp, False);
    }
    return None;
}

/***====================================================================***/

void
XkbInitAtoms(Display *dpy)
{
    static int been_here = 0;

    if ((dpy == NULL) && (!been_here)) {
        _XkbInitAtoms();
        been_here = 1;
    }
    return;
}
