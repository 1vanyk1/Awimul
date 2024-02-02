#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XIChangeHierarchy(Display* dpy,
                  XIAnyHierarchyChangeInfo* changes,
                  int num_changes)
{
    XIAnyHierarchyChangeInfo* any;
    xXIChangeHierarchyReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);
    char *data = NULL, *dptr;
    int dlen = 0, i, ret = Success;

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return (NoSuchExtension);

    if (num_changes <= 0)
        goto out;

    GetReq(XIChangeHierarchy, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIChangeHierarchy;
    req->num_changes = num_changes;

    /* alloc required memory */
    for (i = 0, any = changes; i < num_changes; i++, any++)
    {
        switch(any->type)
        {
            case XIAddMaster:
            {
                int slen = (strlen(any->add.name));
                dlen += sizeof(xXIAddMasterInfo) +
                        slen + (4 - (slen % 4));
            }
                break;
            case XIRemoveMaster:
                dlen += sizeof(xXIRemoveMasterInfo);
                break;
            case XIAttachSlave:
                dlen += sizeof(xXIAttachSlaveInfo);
                break;
            case XIDetachSlave:
                dlen += sizeof(xXIDetachSlaveInfo);
                break;
            default:
                return BadValue;
        }
    }

    req->length += dlen / 4; /* dlen is 4-byte aligned */
    data = Xmalloc(dlen);
    if (!data) {
        ret = BadAlloc;
        goto out;
    }

    dptr = data;
    for (i = 0, any = changes; i < num_changes; i++, any++)
    {
        switch(any->type)
        {
            case XIAddMaster:
            {
                XIAddMasterInfo* C = &any->add;
                xXIAddMasterInfo* c = (xXIAddMasterInfo*)dptr;
                c->type = C->type;
                c->send_core = C->send_core;
                c->enable = C->enable;
                c->name_len = strlen(C->name);
                c->length = (sizeof(xXIAddMasterInfo) + c->name_len + 3)/4;
                strncpy((char*)&c[1], C->name, c->name_len);
                dptr += 4 * c->length;
            }
                break;
            case XIRemoveMaster:
            {
                XIRemoveMasterInfo* R = &any->remove;
                xXIRemoveMasterInfo* r = (xXIRemoveMasterInfo*)dptr;
                r->type = R->type;
                r->return_mode = R->return_mode;
                r->deviceid = R->deviceid;
                r->length = sizeof(xXIRemoveMasterInfo)/4;
                if (r->return_mode == XIAttachToMaster)
                {
                    r->return_pointer = R->return_pointer;
                    r->return_keyboard = R->return_keyboard;
                }
                dptr += sizeof(xXIRemoveMasterInfo);
            }
                break;
            case XIAttachSlave:
            {
                XIAttachSlaveInfo* C = &any->attach;
                xXIAttachSlaveInfo* c = (xXIAttachSlaveInfo*)dptr;

                c->type = C->type;
                c->deviceid = C->deviceid;
                c->length = sizeof(xXIAttachSlaveInfo)/4;
                c->new_master = C->new_master;

                dptr += sizeof(xXIAttachSlaveInfo);
            }
                break;
            case XIDetachSlave:
            {
                XIDetachSlaveInfo *D = &any->detach;
                xXIDetachSlaveInfo *d = (xXIDetachSlaveInfo*)dptr;

                d->type = D->type;
                d->deviceid = D->deviceid;
                d->length = sizeof(xXIDetachSlaveInfo)/4;
                dptr += sizeof(xXIDetachSlaveInfo);
            }
        }
    }

    Data(dpy, data, dlen);

    out:
    Xfree(data);
    UnlockDisplay(dpy);
    SyncHandle();
    return ret;
}
