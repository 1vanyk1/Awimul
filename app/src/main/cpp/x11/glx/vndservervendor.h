#ifndef VND_SERVER_VENDOR_H
#define VND_SERVER_VENDOR_H

//#include "../headers/dix-config.h"

#include "../headers/glxvndabi.h"
#include "../headers/list.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Info related to a single vendor library.
 */
struct GlxServerVendorRec {
    GlxServerImports glxvc;

    struct xorg_list entry;
};

/**
 * A linked list of vendor libraries.
 *
 * Note that this list only includes vendor libraries that were successfully
 * initialized.
 */
extern struct xorg_list GlxVendorList;

GlxServerVendor *GlxCreateVendor(const GlxServerImports *imports);
void GlxDestroyVendor(GlxServerVendor *vendor);

void GlxVendorExtensionReset(const ExtensionEntry *extEntry);

#if defined(__cplusplus)
}
#endif

#endif // VND_SERVER_VENDOR_H
