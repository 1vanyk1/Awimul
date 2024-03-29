#ifndef _UNWIND_H
#define _UNWIND_H
#if defined (__SEH__) && !defined (__USING_SJLJ_EXCEPTIONS__)
/* Only for _GCC_specific_handler.  */
#include "../windows/windows.h"
#endif
#include <unwind.h>
#ifndef HIDE_EXPORTS
#pragma GCC visibility push(default)
#endif
#ifdef __cplusplus
extern "C" {
#endif
/* Level 1: Base ABI  */
/* @@@ The IA-64 ABI uses uint64 throughout.  Most places this is
   inefficient for 32-bit and smaller machines.  */
typedef unsigned _Unwind_Word __attribute__((__mode__(__unwind_word__)));
typedef signed _Unwind_Sword __attribute__((__mode__(__unwind_word__)));
#if defined(__ia64__) && defined(__hpux__)
typedef unsigned _Unwind_Ptr __attribute__((__mode__(__word__)));
#else
typedef unsigned _Unwind_Ptr __attribute__((__mode__(__pointer__)));
#endif
typedef unsigned _Unwind_Internal_Ptr __attribute__((__mode__(__pointer__)));
/* @@@ The IA-64 ABI uses a 64-bit word to identify the producer and
   consumer of an exception.  We'll go along with this for now even on
   32-bit machines.  We'll need to provide some other option for
   16-bit machines and for machines with > 8 bits per byte.  */
typedef unsigned _Unwind_Exception_Class __attribute__((__mode__(__DI__)));
/* The unwind interface uses reason codes in several contexts to
   identify the reasons for failures or other actions.  */
/* The unwind interface uses a pointer to an exception header object
   as its representation of an exception being thrown. In general, the
   full representation of an exception object is language- and
   implementation-specific, but it will be prefixed by a header
   understood by the unwind interface.  */
typedef void (*_Unwind_Exception_Cleanup_Fn) (_Unwind_Reason_Code,
                                              _Unwind_Exception *);
struct _Unwind_Exception
{
    _Unwind_Exception_Class exception_class;
    _Unwind_Exception_Cleanup_Fn exception_cleanup;
#if !defined (__USING_SJLJ_EXCEPTIONS__) && defined (__SEH__)
    _Unwind_Word private_[6];
#else
    _Unwind_Word private_1;
    _Unwind_Word private_2;
#endif
    /* @@@ The IA-64 ABI says that this structure must be double-word aligned.
       Taking that literally does not make much sense generically.  Instead we
       provide the maximum alignment required by any type for the machine.  */
} __attribute__((__aligned__));
/* The ACTIONS argument to the personality routine is a bitwise OR of one
   or more of the following constants.  */
#define _UA_SEARCH_PHASE        1
#define _UA_CLEANUP_PHASE        2
#define _UA_HANDLER_FRAME        4
#define _UA_FORCE_UNWIND        8
#define _UA_END_OF_STACK        16
/* The target can override this macro to define any back-end-specific
   attributes required for the lowest-level stack frame.  */
#ifndef LIBGCC2_UNWIND_ATTRIBUTE
#define LIBGCC2_UNWIND_ATTRIBUTE
#endif
/* This is an opaque type used to refer to a system-specific data
   structure used by the system unwinder. This context is created and
   destroyed by the system, and passed to the personality routine
   during unwinding.  */
struct _Unwind_Context;
/* Raise an exception, passing along the given exception object.  */
/* @@@ Use unwind data to perform a stack backtrace.  The trace callback
   is called for every stack frame in the call chain, but no cleanup
   actions are performed.  */
typedef _Unwind_Reason_Code (*_Unwind_Trace_Fn)
        (struct _Unwind_Context *, void *);
extern _Unwind_Reason_Code LIBGCC2_UNWIND_ATTRIBUTE
_Unwind_Backtrace (_Unwind_Trace_Fn, void *);
/* These functions are used for communicating information about the unwind
   context (i.e. the unwind descriptors and the user register state) between
   the unwind library and the personality routine and landing pad.  Only
   selected registers may be manipulated.  */
extern _Unwind_Word _Unwind_GetGR (struct _Unwind_Context *, int);
extern void _Unwind_SetGR (struct _Unwind_Context *, int, _Unwind_Word);
extern _Unwind_Ptr _Unwind_GetIP (struct _Unwind_Context *);
extern _Unwind_Ptr _Unwind_GetIPInfo (struct _Unwind_Context *, int *);
extern void _Unwind_SetIP (struct _Unwind_Context *, _Unwind_Ptr);
/* @@@ Retrieve the CFA of the given context.  */
extern _Unwind_Word _Unwind_GetCFA (struct _Unwind_Context *);
extern void *_Unwind_GetLanguageSpecificData (struct _Unwind_Context *);
extern _Unwind_Ptr _Unwind_GetRegionStart (struct _Unwind_Context *);
/* The personality routine is the function in the C++ (or other language)
   runtime library which serves as an interface between the system unwind
   library and language-specific exception handling semantics.  It is
   specific to the code fragment described by an unwind info block, and
   it is always referenced via the pointer in the unwind info block, and
   hence it has no ABI-specified name.
   Note that this implies that two different C++ implementations can
   use different names, and have different contents in the language
   specific data area.  Moreover, that the language specific data
   area contains no version info because name of the function invoked
   provides more effective versioning by detecting at link time the
   lack of code to handle the different data format.  */
/* @@@ The following alternate entry points are for setjmp/longjmp
   based unwinding.  */
struct SjLj_Function_Context;
extern void _Unwind_SjLj_Register (struct SjLj_Function_Context *);
extern void _Unwind_SjLj_Unregister (struct SjLj_Function_Context *);
/* @@@ The following provide access to the base addresses for text
   and data-relative addressing in the LDSA.  In order to stay link
   compatible with the standard ABI for IA-64, we inline these.  */
#ifdef __ia64__
static inline _Unwind_Ptr
_Unwind_GetDataRelBase (struct _Unwind_Context *_C)
{
  /* The GP is stored in R1.  */
  return _Unwind_GetGR (_C, 1);
}
static inline _Unwind_Ptr
_Unwind_GetTextRelBase (struct _Unwind_Context *_C __attribute__ ((__unused__)))
{
  __builtin_abort ();
  return 0;
}
/* @@@ Retrieve the Backing Store Pointer of the given context.  */
extern _Unwind_Word _Unwind_GetBSP (struct _Unwind_Context *);
#else
extern _Unwind_Ptr _Unwind_GetDataRelBase (struct _Unwind_Context *);
extern _Unwind_Ptr _Unwind_GetTextRelBase (struct _Unwind_Context *);
#endif
/* @@@ Given an address, return the entry point of the function that
   contains it.  */
extern void * _Unwind_FindEnclosingFunction (void *pc);
#ifndef __SIZEOF_LONG__
#error "__SIZEOF_LONG__ macro not defined"
#endif
#ifndef __SIZEOF_POINTER__
#error "__SIZEOF_POINTER__ macro not defined"
#endif
/* leb128 type numbers have a potentially unlimited size.
   The target of the following definitions of _sleb128_t and _uleb128_t
   is to have efficient data types large enough to hold the leb128 type
   numbers used in the unwind code.
   Mostly these types will simply be defined to long and unsigned long
   except when a unsigned long data type on the target machine is not
   capable of storing a pointer.  */
#if defined (__SEH__) && !defined (__USING_SJLJ_EXCEPTIONS__)
/* Handles the mapping from SEH to GCC interfaces.  */
//EXCEPTION_DISPOSITION _GCC_specific_handler (PEXCEPTION_RECORD, void *,
//                                             PCONTEXT, PDISPATCHER_CONTEXT,
//                                             _Unwind_Personality_Fn);
#endif
#ifdef __cplusplus
}
#endif
#ifndef HIDE_EXPORTS
//#pragma GCC visibility pop
#endif
/* Additional actions to unwind number of stack frames.  */
#define _Unwind_Frames_Extra(frames)
#define _Unwind_Frames_Increment(context, frames) frames++

#endif /* unwind.h */