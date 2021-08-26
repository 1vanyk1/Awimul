#ifndef EXTENSION_H
#define EXTENSION_H

#include "xfuncproto.h"

#include "dixstruct.h"
#include "xdefs.h"

typedef void (*InitExtension) (void);

typedef struct {
    InitExtension initFunc;
    const char *name;
    Bool *disablePtr;
} ExtensionModule;

extern _X_EXPORT unsigned short StandardMinorOpcode(ClientPtr /*client */ );

extern _X_EXPORT Bool EnableDisableExtension(const char *name, Bool enable);

extern _X_EXPORT void EnableDisableExtensionError(const char *name,
                                                  Bool enable);

extern _X_EXPORT void InitExtensions(int argc, char **argv);

extern _X_EXPORT void CloseDownExtensions(void);

extern _X_EXPORT void LoadExtensionList(const ExtensionModule ext[],
                                        int listSize, Bool external);

#endif                          /* EXTENSION_H */