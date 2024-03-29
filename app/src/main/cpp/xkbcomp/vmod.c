#define	DEBUG_VAR debugFlags
#include <stdio.h>
#include "xkbcomp.h"
#include "tokens.h"
#include "expr.h"
#include "misc.h"

#include "../x11/headers/extensions/XKB.h"
#include "../x11/headers/extensions/XKBstr.h"

#include "vmod.h"

void
InitVModInfo(VModInfo * info, XkbDescPtr xkb)
{
    ClearVModInfo(info, xkb);
    info->errorCount = 0;
    return;
}

void
ClearVModInfo(VModInfo * info, XkbDescPtr xkb)
{
    register int i;

    if (XkbAllocNames(xkb, XkbVirtualModNamesMask, 0, 0) != Success)
        return;
    if (XkbAllocServerMap(xkb, XkbVirtualModsMask, 0) != Success)
        return;
    info->xkb = xkb;
    info->newlyDefined = info->defined = info->available = 0;
    if (xkb && xkb->names)
    {
        register int bit;
        for (i = 0, bit = 1; i < XkbNumVirtualMods; i++, bit <<= 1)
        {
            if (xkb->names->vmods[i] != None)
                info->defined |= bit;
        }
    }
    return;
}

/***====================================================================***/

/**
 * Handle one entry in the virtualModifiers line (e.g. NumLock).
 * If the entry is e.g. NumLock=Mod1, stmt->value is not NULL, and the
 * XkbServerMap's vmod is set to the given modifier. Otherwise, the vmod is 0.
 *
 * @param stmt The statement specifying the name and (if any the value).
 * @param mergeMode Merge strategy (e.g. MergeOverride)
 */
Bool
HandleVModDef(VModDef * stmt, unsigned mergeMode, VModInfo * info)
{
    register int i, bit, nextFree;
    ExprResult mod;
    XkbServerMapPtr srv;
    XkbNamesPtr names;
    Atom stmtName;

    srv = info->xkb->server;
    names = info->xkb->names;
    stmtName =
            XkbInternAtom(info->xkb->dpy, XkbAtomGetString(NULL, stmt->name),
                          False);
    for (i = 0, bit = 1, nextFree = -1; i < XkbNumVirtualMods; i++, bit <<= 1)
    {
        if (info->defined & bit)
        {
            if (names->vmods[i] == stmtName)
            {                   /* already defined */
                info->available |= bit;
                if (stmt->value == NULL)
                    return True;
                else
                {
                    char *str1;
                    const char *str2 = "";
                    if (!ExprResolveModMask(stmt->value, &mod, NULL, NULL))
                    {
                        str1 = XkbAtomText(NULL, stmt->name, XkbMessage);
                        ACTION("Declaration of %s ignored\n", str1);
                        return False;
                    }
                    if (mod.uval == srv->vmods[i])
                        return True;

                    str1 = XkbAtomText(NULL, stmt->name, XkbMessage);
                    WARN("Virtual modifier %s multiply defined\n", str1);
                    str1 = XkbModMaskText(srv->vmods[i], XkbCFile);
                    if (mergeMode == MergeOverride)
                    {
                        str2 = str1;
                        str1 = XkbModMaskText(mod.uval, XkbCFile);
                    }
                    ACTION("Using %s, ignoring %s\n", str1, str2);
                    if (mergeMode == MergeOverride)
                        srv->vmods[i] = mod.uval;
                    return True;
                }
            }
        }
        else if (nextFree < 0)
            nextFree = i;
    }
    if (nextFree < 0)
    {
        ERROR("Too many virtual modifiers defined (maximum %d)\n",
              XkbNumVirtualMods);
        ACTION("Exiting\n");
        return False;
    }
    info->defined |= (1 << nextFree);
    info->newlyDefined |= (1 << nextFree);
    info->available |= (1 << nextFree);
    names->vmods[nextFree] = stmtName;
    if (stmt->value == NULL)
        return True;
    if (ExprResolveModMask(stmt->value, &mod, NULL, NULL))
    {
        srv->vmods[nextFree] = mod.uval;
        return True;
    }
    ACTION("Declaration of %s ignored\n",
           XkbAtomText(NULL, stmt->name, XkbMessage));
    return False;
}

/**
 * Returns the index of the given modifier in the xkb->names->vmods array.
 *
 * @param priv Pointer to the xkb data structure.
 * @param elem Must be None, otherwise return False.
 * @param field The Atom of the modifier's name (e.g. Atom for LAlt)
 * @param type Must be TypeInt, otherwise return False.
 * @param val_rtrn Set to the index of the modifier that matches.
 *
 * @return True on success, False otherwise. If False is returned, val_rtrn is
 * undefined.
 */
int
LookupVModIndex(XPointer priv,
                Atom elem, Atom field, unsigned type, ExprResult * val_rtrn)
{
    register int i;
    register char *fieldStr;
    register char *modStr;
    XkbDescPtr xkb;

    xkb = (XkbDescPtr) priv;
    if ((xkb == NULL) || (xkb->names == NULL) || (elem != None)
        || (type != TypeInt))
    {
        return False;
    }
    /* get the actual name */
    fieldStr = XkbAtomGetString(xkb->dpy, field);
    if (fieldStr == NULL)
        return False;
    /* For each named modifier, get the name and compare it to the one passed
     * in. If we get a match, return the index of the modifier.
     * The order of modifiers is the same as in the virtual_modifiers line in
     * the xkb_types section.
     */
    for (i = 0; i < XkbNumVirtualMods; i++)
    {
        modStr = XkbAtomGetString(xkb->dpy, xkb->names->vmods[i]);
        if ((modStr != NULL) && (uStrCaseCmp(fieldStr, modStr) == 0))
        {
            val_rtrn->uval = i;
            return True;
        }
    }
    return False;
}

/**
 * Get the mask for the given modifier and set val_rtrn.uval to the mask.
 * Note that the mask returned is always > 512.
 *
 * @param priv Pointer to xkb data structure.
 * @param val_rtrn Set to the mask returned.
 *
 * @return True on success, False otherwise. If False is returned, val_rtrn is
 * undefined.
 */
int
LookupVModMask(XPointer priv,
               Atom elem, Atom field, unsigned type, ExprResult * val_rtrn)
{
    if (LookupVModIndex(priv, elem, field, type, val_rtrn))
    {
        register unsigned ndx = val_rtrn->uval;
        val_rtrn->uval = (1 << (XkbNumModifiers + ndx));
        return True;
    }
    return False;
}

int
FindKeypadVMod(XkbDescPtr xkb)
{
    Atom name;
    ExprResult rtrn;

    name = XkbInternAtom(xkb->dpy, "NumLock", False);
    if ((xkb) && LookupVModIndex((XPointer) xkb, None, name, TypeInt, &rtrn))
    {
        return rtrn.ival;
    }
    return -1;
}

Bool
ResolveVirtualModifier(ExprDef * def, ExprResult * val_rtrn, VModInfo * info)
{
    XkbNamesPtr names;

    names = info->xkb->names;
    if (def->op == ExprIdent)
    {
        register int i, bit;
        for (i = 0, bit = 1; i < XkbNumVirtualMods; i++, bit <<= 1)
        {
            char *str1, *str2;
            str1 = XkbAtomGetString(info->xkb->dpy, names->vmods[i]);
            str2 = XkbAtomGetString(NULL, def->value.str);
            if ((info->available & bit) && (uStrCaseCmp(str1, str2) == Equal))
            {
                val_rtrn->uval = i;
                return True;
            }
        }
    }
    if (ExprResolveInteger(def, val_rtrn, NULL, NULL))
    {
        if (val_rtrn->uval < XkbNumVirtualMods)
            return True;
        ERROR("Illegal virtual modifier %d (must be 0..%d inclusive)\n",
              val_rtrn->uval, XkbNumVirtualMods - 1);
    }
    return False;
}
