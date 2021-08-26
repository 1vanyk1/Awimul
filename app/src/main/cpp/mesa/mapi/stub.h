#ifndef _STUB_H_
#define _STUB_H_

#include "entry.h"

struct mapi_stub;

void
stub_init_once(void);

const struct mapi_stub *
stub_find_public(const char *name);

struct mapi_stub *
stub_find_dynamic(const char *name, int generate);

const struct mapi_stub *
stub_find_by_slot(int slot);

void
stub_fix_dynamic(struct mapi_stub *stub, const struct mapi_stub *alias);

const char *
stub_get_name(const struct mapi_stub *stub);

int
stub_get_slot(const struct mapi_stub *stub);

mapi_func
stub_get_addr(const struct mapi_stub *stub);

#endif /* _STUB_H_ */
