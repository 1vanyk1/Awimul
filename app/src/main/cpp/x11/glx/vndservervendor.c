#include "vndservervendor.h"

struct xorg_list GlxVendorList = { &GlxVendorList, &GlxVendorList };

GlxServerVendor *GlxCreateVendor(const GlxServerImports *imports)
{
    GlxServerVendor *vendor = NULL;

    if (imports == NULL) {
        ErrorF("GLX: Vendor library did not provide an imports table\n");
        return NULL;
    }

    if (imports->extensionCloseDown == NULL
        || imports->handleRequest == NULL
        || imports->getDispatchAddress == NULL
        || imports->makeCurrent == NULL) {
        ErrorF("GLX: Vendor library is missing required callback functions.\n");
        return NULL;
    }

    vendor = (GlxServerVendor *) calloc(1, sizeof(GlxServerVendor));
    if (vendor == NULL) {
        ErrorF("GLX: Can't allocate vendor library.\n");
        return NULL;
    }
    memcpy(&vendor->glxvc, imports, sizeof(GlxServerImports));

    xorg_list_append(&vendor->entry, &GlxVendorList);
    return vendor;
}

void GlxDestroyVendor(GlxServerVendor *vendor)
{
    if (vendor != NULL) {
        xorg_list_del(&vendor->entry);
        free(vendor);
    }
}

void GlxVendorExtensionReset(const ExtensionEntry *extEntry)
{
    GlxServerVendor *vendor, *tempVendor;

    // TODO: Do we allow the driver to destroy a vendor library handle from
    // here?
    xorg_list_for_each_entry_safe(vendor, tempVendor, &GlxVendorList, entry) {
        if (vendor->glxvc.extensionCloseDown != NULL) {
            vendor->glxvc.extensionCloseDown(extEntry);
        }
    }

    // If the server is exiting instead of starting a new generation, then
    // free the remaining GlxServerVendor structs.
    //
    // XXX this used to be conditional on xf86ServerIsExiting, but it's
    // cleaner to just always create the vendor struct on every generation,
    // if nothing else so all ddxes get the same behavior.
    xorg_list_for_each_entry_safe(vendor, tempVendor, &GlxVendorList, entry) {
        GlxDestroyVendor(vendor);
    }
}
