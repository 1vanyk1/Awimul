#ifndef __CUTILS_PROPERTIES_H
#define __CUTILS_PROPERTIES_H
#ifdef __cplusplus
extern "C" {
#endif
/* System properties are *small* name value pairs managed by the
** property service.  If your data doesn't fit in the provided
** space it is not appropriate for a system property.
**
** WARNING: system/bionic/include/sys/system_properties.h also defines
**          these, but with different names.  (TODO: fix that)
*/
#define PROPERTY_KEY_MAX   32
#define PROPERTY_VALUE_MAX  92
/* property_get: returns the length of the value which will never be
** greater than PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
** (the length does not include the terminating zero).
**
** If the property read fails or returns an empty value, the default
** value is used (if nonnull).
*/
int property_get(const char *key, char *value, const char *default_value);
/* property_set: returns 0 on success, < 0 on failure
*/
int property_set(const char *key, const char *value);

int property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie);
#ifdef HAVE_SYSTEM_PROPERTY_SERVER
/*
 * We have an external property server instead of built-in libc support.
 * Used by the simulator.
 */
#define SYSTEM_PROPERTY_PIPE_NAME       "/tmp/android-sysprop"
enum {
    kSystemPropertyUnknown = 0,
    kSystemPropertyGet,
    kSystemPropertySet,
    kSystemPropertyList
};
#endif /*HAVE_SYSTEM_PROPERTY_SERVER*/
#ifdef __cplusplus
}
#endif
#endif