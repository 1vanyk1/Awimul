#ifndef _BUSFAULT_H_
#define _BUSFAULT_H_

//#include <dix-config.h>

#include <sys/types.h>

typedef void (*busfault_notify_ptr) (void *context);

struct busfault *
busfault_register_mmap(void *addr, size_t size, busfault_notify_ptr notify, void *context);

void
busfault_unregister(struct busfault *busfault);

void
busfault_check(void);

Bool
busfault_init(void);

#endif /* _BUSFAULT_H_ */
