#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"


void
_IceAddReplyWait (
        IceConn			iceConn,
        IceReplyWaitInfo	*replyWait
)
{
    /*
     * Add this replyWait to the end of the list (only if the
     * replyWait is not already in the list).
     */

    _IceSavedReplyWait	*savedReplyWait;
    _IceSavedReplyWait	*prev, *last;

    prev = NULL;
    last = iceConn->saved_reply_waits;

    while (last)
    {
        if (last->reply_wait == replyWait)
            return;

        prev = last;
        last = last->next;
    }

    savedReplyWait = malloc (sizeof (_IceSavedReplyWait));

    savedReplyWait->reply_wait = replyWait;
    savedReplyWait->reply_ready = False;
    savedReplyWait->next = NULL;

    if (prev == NULL)
        iceConn->saved_reply_waits = savedReplyWait;
    else
        prev->next = savedReplyWait;
}



IceReplyWaitInfo *
_IceSearchReplyWaits (
        IceConn	iceConn,
        int	majorOpcode
)
{
    /*
     * Return the first replyWait in the list with the given majorOpcode
     */

    _IceSavedReplyWait	*savedReplyWait = iceConn->saved_reply_waits;

    while (savedReplyWait && !savedReplyWait->reply_ready &&
           savedReplyWait->reply_wait->major_opcode_of_request != majorOpcode)
    {
        savedReplyWait = savedReplyWait->next;
    }

    return (savedReplyWait ? savedReplyWait->reply_wait : NULL);
}



void
_IceSetReplyReady (
        IceConn			iceConn,
        IceReplyWaitInfo	*replyWait
)
{
    /*
     * The replyWait specified has a reply ready.
     */

    _IceSavedReplyWait	*savedReplyWait = iceConn->saved_reply_waits;

    while (savedReplyWait && savedReplyWait->reply_wait != replyWait)
        savedReplyWait = savedReplyWait->next;

    if (savedReplyWait)
        savedReplyWait->reply_ready = True;
}



Bool
_IceCheckReplyReady (
        IceConn			iceConn,
        IceReplyWaitInfo	*replyWait
)
{
    _IceSavedReplyWait	*savedReplyWait = iceConn->saved_reply_waits;
    _IceSavedReplyWait	*prev = NULL;
    Bool		found = False;
    Bool		ready;

    while (savedReplyWait && !found)
    {
        if (savedReplyWait->reply_wait == replyWait)
            found = True;
        else
        {
            prev = savedReplyWait;
            savedReplyWait = savedReplyWait->next;
        }
    }

    ready = found && savedReplyWait->reply_ready;

    if (ready)
    {
        if (prev == NULL)
            iceConn->saved_reply_waits = savedReplyWait->next;
        else
            prev->next = savedReplyWait->next;

        free (savedReplyWait);
    }

    return (ready);
}
