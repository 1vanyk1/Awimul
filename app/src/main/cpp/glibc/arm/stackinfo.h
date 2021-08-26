#ifndef _STACKINFO_H
#define _STACKINFO_H        1
#include <elf.h>
/* On Arm the stack grows down.  */
#define _STACK_GROWS_DOWN        1
/* Default to an executable stack.  PF_X can be overridden if PT_GNU_STACK is
 * present, but it is presumed absent.  */
#define DEFAULT_STACK_PERMS (PF_R|PF_W|PF_X)
#endif        /* stackinfo.h */