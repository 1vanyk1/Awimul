#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xatom.h"
#include <stdio.h>
#include <string.h>
#include "../headers/misc.h"
#include "../headers/resource.h"
#include "../headers/dix.h"

#define InitialTableSize 256

typedef struct _Node {
    struct _Node *left, *right;
    Atom a;
    unsigned int fingerPrint;
    const char *string;
} NodeRec, *NodePtr;

static Atom lastAtom = None;
static NodePtr atomRoot = NULL;
static unsigned long tableLength;
static NodePtr *nodeTable;

Atom
MakeAtom(const char *string, unsigned len, Bool makeit)
{
    NodePtr *np;
    unsigned i;
    int comp;
    unsigned int fp = 0;

    np = &atomRoot;
    for (i = 0; i < (len + 1) / 2; i++) {
        fp = fp * 27 + string[i];
        fp = fp * 27 + string[len - 1 - i];
    }
    while (*np != NULL) {
        if (fp < (*np)->fingerPrint)
            np = &((*np)->left);
        else if (fp > (*np)->fingerPrint)
            np = &((*np)->right);
        else {                  /* now start testing the strings */
            comp = strncmp(string, (*np)->string, (int) len);
            if ((comp < 0) || ((comp == 0) && (len < strlen((*np)->string))))
                np = &((*np)->left);
            else if (comp > 0)
                np = &((*np)->right);
            else
                return (*np)->a;
        }
    }
    if (makeit) {
        NodePtr nd;

        nd = malloc(sizeof(NodeRec));
        if (!nd)
            return BAD_RESOURCE;
        if (lastAtom < XA_LAST_PREDEFINED) {
            nd->string = string;
        }
        else {
            nd->string = strndup(string, len);
            if (!nd->string) {
                free(nd);
                return BAD_RESOURCE;
            }
        }
        if ((lastAtom + 1) >= tableLength) {
            NodePtr *table;

            table = reallocarray(nodeTable, tableLength, 2 * sizeof(NodePtr));
            if (!table) {
                if (nd->string != string) {
                    /* nd->string has been strdup'ed */
                    free((char *) nd->string);
                }
                free(nd);
                return BAD_RESOURCE;
            }
            tableLength <<= 1;
            nodeTable = table;
        }
        *np = nd;
        nd->left = nd->right = NULL;
        nd->fingerPrint = fp;
        nd->a = ++lastAtom;
        nodeTable[lastAtom] = nd;
        return nd->a;
    }
    else
        return None;
}

Bool
ValidAtom(Atom atom)
{
    return (atom != None) && (atom <= lastAtom);
}

const char *
NameForAtom(Atom atom)
{
    NodePtr node;

    if (atom > lastAtom)
        return 0;
    if ((node = nodeTable[atom]) == NULL)
        return 0;
    return node->string;
}

void
AtomError(void)
{
    FatalError("initializing atoms");
}

static void
FreeAtom(NodePtr patom)
{
    if (patom->left)
        FreeAtom(patom->left);
    if (patom->right)
        FreeAtom(patom->right);
    if (patom->a > XA_LAST_PREDEFINED) {
        /*
         * All strings above XA_LAST_PREDEFINED are strdup'ed, so it's safe to
         * cast here
         */
        free((char *) patom->string);
    }
    free(patom);
}

void
FreeAllAtoms(void)
{
    if (atomRoot == NULL)
        return;
    FreeAtom(atomRoot);
    atomRoot = NULL;
    free(nodeTable);
    nodeTable = NULL;
    lastAtom = None;
}

void
InitAtoms(void)
{
    FreeAllAtoms();
    tableLength = InitialTableSize;
    nodeTable = xallocarray(InitialTableSize, sizeof(NodePtr));
    if (!nodeTable)
        AtomError();
    nodeTable[None] = NULL;
    MakePredeclaredAtoms();
    if (lastAtom != XA_LAST_PREDEFINED)
        AtomError();
}
