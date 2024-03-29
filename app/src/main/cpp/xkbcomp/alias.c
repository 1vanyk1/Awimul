#include "xkbcomp.h"
#include "misc.h"
#include "alias.h"
#include "keycodes.h"
#include "../x11/headers/xkbsrv.h"

//#include "../x11/headers/extensions/XKBgeom.h"
#include "../x11/xkb/xkbgeom.h"

static void
HandleCollision(AliasInfo * old, AliasInfo * new)
{
    if (strncmp(new->real, old->real, XkbKeyNameLength) == 0)
    {
        if (((new->def.fileID == old->def.fileID) && (warningLevel > 0)) ||
            (warningLevel > 9))
        {
            WARN("Alias of %s for %s declared more than once\n",
                 XkbKeyNameText(new->alias, XkbMessage),
                 XkbKeyNameText(new->real, XkbMessage));
            ACTION("First definition ignored\n");
        }
    }
    else
    {
        char *use, *ignore;
        if (new->def.merge == MergeAugment)
        {
            use = old->real;
            ignore = new->real;
        }
        else
        {
            use = new->real;
            ignore = old->real;
        }
        if (((old->def.fileID == new->def.fileID) && (warningLevel > 0)) ||
            (warningLevel > 9))
        {
            WARN("Multiple definitions for alias %s\n",
                 XkbKeyNameText(old->alias, XkbMessage));
            ACTION("Using %s, ignoring %s\n",
                   XkbKeyNameText(use, XkbMessage),
                   XkbKeyNameText(ignore, XkbMessage));
        }
        if (use != old->real)
            memcpy(old->real, use, XkbKeyNameLength);
    }
    old->def.fileID = new->def.fileID;
    old->def.merge = new->def.merge;
    return;
}

static void
InitAliasInfo(AliasInfo * info,
              unsigned merge, unsigned file_id, char *alias, char *real)
{
    bzero(info, sizeof(AliasInfo));
    info->def.merge = merge;
    info->def.fileID = file_id;
    strncpy(info->alias, alias, XkbKeyNameLength);
    strncpy(info->real, real, XkbKeyNameLength);
    return;
}

int
HandleAliasDef(KeyAliasDef * def,
               unsigned merge, unsigned file_id, AliasInfo ** info_in)
{
    AliasInfo *info;

    for (info = *info_in; info != NULL; info = (AliasInfo *) info->def.next)
    {
        if (strncmp(info->alias, def->alias, XkbKeyNameLength) == 0)
        {
            AliasInfo new;
            InitAliasInfo(&new, merge, file_id, def->alias, def->real);
            HandleCollision(info, &new);
            return True;
        }
    }
    info = uTypedCalloc(1, AliasInfo);
    if (info == NULL)
    {
        WSGO("Allocation failure in HandleAliasDef\n");
        return False;
    }
    info->def.fileID = file_id;
    info->def.merge = merge;
    info->def.next = (CommonInfo *) * info_in;
    memcpy(info->alias, def->alias, XkbKeyNameLength);
    memcpy(info->real, def->real, XkbKeyNameLength);
    *info_in = (AliasInfo *) AddCommonInfo(&(*info_in)->def, &info->def);
    return True;
}

void
ClearAliases(AliasInfo ** info_in)
{
    if ((info_in) && (*info_in))
        ClearCommonInfo(&(*info_in)->def);
    return;
}

Bool
MergeAliases(AliasInfo ** into, AliasInfo ** merge, unsigned how_merge)
{
    AliasInfo *tmp;
    KeyAliasDef def;

    if ((*merge) == NULL)
        return True;
    if ((*into) == NULL)
    {
        *into = *merge;
        *merge = NULL;
        return True;
    }
    bzero((char *) &def, sizeof(KeyAliasDef));
    for (tmp = *merge; tmp != NULL; tmp = (AliasInfo *) tmp->def.next)
    {
        if (how_merge == MergeDefault)
            def.merge = tmp->def.merge;
        else
            def.merge = how_merge;
        memcpy(def.alias, tmp->alias, XkbKeyNameLength);
        memcpy(def.real, tmp->real, XkbKeyNameLength);
        if (!HandleAliasDef(&def, def.merge, tmp->def.fileID, into))
            return False;
    }
    return True;
}

int
ApplyAliases(XkbDescPtr xkb, Bool toGeom, AliasInfo ** info_in)
{
    register int i;
    XkbKeyAliasPtr old, a;
    AliasInfo *info;
    int nNew, nOld;
    Status status;

    if (*info_in == NULL)
        return True;
    if (toGeom)
    {
        nOld = (xkb->geom ? xkb->geom->num_key_aliases : 0);
        old = (xkb->geom ? xkb->geom->key_aliases : NULL);
    }
    else
    {
        nOld = (xkb->names ? xkb->names->num_key_aliases : 0);
        old = (xkb->names ? xkb->names->key_aliases : NULL);
    }
    for (nNew = 0, info = *info_in; info != NULL;
         info = (AliasInfo *) info->def.next)
    {
        unsigned long lname;
        unsigned int kc;

        lname = KeyNameToLong(info->real);
        if (!FindNamedKey(xkb, lname, &kc, False, CreateKeyNames(xkb), 0))
        {
            if (warningLevel > 4)
            {
                WARN("Attempt to alias %s to non-existent key %s\n",
                     XkbKeyNameText(info->alias, XkbMessage),
                     XkbKeyNameText(info->real, XkbMessage));
                ACTION("Ignored\n");
            }
            info->alias[0] = '\0';
            continue;
        }
        lname = KeyNameToLong(info->alias);
        if (FindNamedKey(xkb, lname, &kc, False, False, 0))
        {
            if (warningLevel > 4)
            {
                WARN("Attempt to create alias with the name of a real key\n");
                ACTION("Alias \"%s = %s\" ignored\n",
                       XkbKeyNameText(info->alias, XkbMessage),
                       XkbKeyNameText(info->real, XkbMessage));
            }
            info->alias[0] = '\0';
            continue;
        }
        nNew++;
        if (old)
        {
            for (i = 0, a = old; i < nOld; i++, a++)
            {
                if (strncmp(a->alias, info->alias, XkbKeyNameLength) == 0)
                {
                    AliasInfo oldai;
                    InitAliasInfo(&oldai, MergeAugment, 0, a->alias, a->real);
                    HandleCollision(&oldai, info);
                    memcpy(oldai.real, a->real, XkbKeyNameLength);
                    info->alias[0] = '\0';
                    nNew--;
                    break;
                }
            }
        }
    }
    if (nNew == 0)
    {
        ClearCommonInfo(&(*info_in)->def);
        *info_in = NULL;
        return True;
    }
    status = Success;
    if (toGeom)
    {
        if (!xkb->geom)
        {
            XkbGeometrySizesRec sizes;
            bzero((char *) &sizes, sizeof(XkbGeometrySizesRec));
            sizes.which = XkbGeomKeyAliasesMask;
            sizes.num_key_aliases = nOld + nNew;
            status = XkbAllocGeometry(xkb, &sizes);
        }
        else
        {
            status = XkbAllocGeomKeyAliases(xkb->geom, nOld + nNew);
        }
        if (xkb->geom)
            old = xkb->geom->key_aliases;
    }
    else
    {
        status = XkbAllocNames(xkb, XkbKeyAliasesMask, 0, nOld + nNew);
        if (xkb->names)
            old = xkb->names->key_aliases;
    }
    if (status != Success)
    {
        WSGO("Allocation failure in ApplyAliases\n");
        return False;
    }
    if (toGeom)
        a = &xkb->geom->key_aliases[nOld];
    else
        a = &xkb->names->key_aliases[nOld];
    for (info = *info_in; info != NULL; info = (AliasInfo *) info->def.next)
    {
        if (info->alias[0] != '\0')
        {
            strncpy(a->alias, info->alias, XkbKeyNameLength);
            strncpy(a->real, info->real, XkbKeyNameLength);
            a++;
        }
    }
#ifdef DEBUG
    if ((a - old) != (nOld + nNew))
    {
        WSGO("Expected %d aliases total but created %d\n", nOld + nNew,
              (int)(a - old));
    }
#endif
    if (toGeom)
        xkb->geom->num_key_aliases += nNew;
    ClearCommonInfo(&(*info_in)->def);
    *info_in = NULL;
    return True;
}
