#ifndef _ENTRY_H_
#define _ENTRY_H_


typedef void (*mapi_func)(void);

void
entry_patch_public(void);

mapi_func
entry_get_public(int slot);

mapi_func
entry_generate(int slot);

void
entry_patch(mapi_func entry, int slot);

#endif /* _ENTRY_H_ */
