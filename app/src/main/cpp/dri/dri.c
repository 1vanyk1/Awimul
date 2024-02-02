#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../GL/gl.h"
#include "../GL/internal/dri_interface.h"
#include "../GL/glxtokens.h"
#include "winsys_handle.h"
#include "dri_interface.h"
#include "../zstd/common/debug.h"
#include <pthread.h>
#include "dri.h"
#include "dri_private.h"

void *_glapi_Context;

static const
__DRIconfig **stub_error_init_screen(__DRIscreen *psp)
{
    fprintf(stderr, "An updated DRI driver loader (libGL.so or X Server) is "
                    "required for this Mesa driver.\n");
    return NULL;
}

const struct __DriverAPIRec driDriverAPI = {
        .InitScreen = stub_error_init_screen,
};

const struct __DriverAPIRec *globalDriverAPI = &driDriverAPI;


static void dri_get_drawable(__DRIdrawable *pdp)
{
    pdp->refcount++;
}


static void
setupLoaderExtensions(__DRIscreen *psp,
                      const __DRIextension **extensions)
{
    int i;

    for (i = 0; extensions[i]; i++) {
        if (strcmp(extensions[i]->name, __DRI_DRI2_LOADER) == 0)
            psp->dri2.loader = (__DRIdri2LoaderExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_IMAGE_LOOKUP) == 0)
            psp->dri2.image = (__DRIimageLookupExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_USE_INVALIDATE) == 0)
            psp->dri2.useInvalidate = (__DRIuseInvalidateExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_BACKGROUND_CALLABLE) == 0)
            psp->dri2.backgroundCallable = (__DRIbackgroundCallableExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_SWRAST_LOADER) == 0)
            psp->swrast_loader = (__DRIswrastLoaderExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_IMAGE_LOADER) == 0)
            psp->image.loader = (__DRIimageLoaderExtension *) extensions[i];
        if (strcmp(extensions[i]->name, __DRI_MUTABLE_RENDER_BUFFER_LOADER) == 0)
            psp->mutableRenderBuffer.loader = (__DRImutableRenderBufferLoaderExtension *) extensions[i];
    }
}


driOptionDescription __dri2ConfigOptions[] = {
        DRI_CONF_SECTION_DEBUG
        DRI_CONF_GLX_EXTENSION_OVERRIDE()
        DRI_CONF_INDIRECT_GL_EXTENSION_OVERRIDE()
        DRI_CONF_SECTION_END

        DRI_CONF_SECTION_PERFORMANCE
        DRI_CONF_VBLANK_MODE(DRI_CONF_VBLANK_DEF_INTERVAL_1)
        DRI_CONF_SECTION_END
};

static const char *execname;

const char *
util_get_process_name(void)
{
    return "";
}


//void
//driParseConfigFiles(driOptionCache *cache, const driOptionCache *info,
//                    int screenNum, const char *driverName,
//                    const char *kernelDriverName,
//                    const char *deviceName,
//                    const char *applicationName, uint32_t applicationVersion,
//                    const char *engineName, uint32_t engineVersion)
//{
//    initOptionCache(cache, info);
//    struct OptConfData userData;
//
//    userData.cache = cache;
//    userData.screenNum = screenNum;
//    userData.driverName = driverName;
//    userData.kernelDriverName = kernelDriverName;
//    userData.deviceName = deviceName;
//    userData.applicationName = applicationName ? applicationName : "";
//    userData.applicationVersion = applicationVersion;
//    userData.engineName = engineName ? engineName : "";
//    userData.engineVersion = engineVersion;
//    userData.execName = execname ? execname : util_get_process_name();
//
//    parseStaticConfig(&userData);
//}

static bool
be_verbose(void)
{
    const char *s = getenv("MESA_DEBUG");
    if (!s)
        return true;

    return strstr(s, "silent") == NULL;
}

static bool
checkValue(const driOptionValue *v, const driOptionInfo *info)
{
    switch (info->type) {
        case DRI_ENUM: /* enum is just a special integer */
        case DRI_INT:
            return (info->range.start._int == info->range.end._int ||
                    (v->_int >= info->range.start._int &&
                     v->_int <= info->range.end._int));

        case DRI_FLOAT:
            return (info->range.start._float == info->range.end._float ||
                    (v->_float >= info->range.start._float &&
                     v->_float <= info->range.end._float));

        default:
            return true;
    }
}


static int
strToI(const char *string, const char **tail, int base)
{
    int radix = base == 0 ? 10 : base;
    int result = 0;
    int sign = 1;
    bool numberFound = false;
    const char *start = string;

    assert(radix >= 2 && radix <= 36);

    if (*string == '-') {
        sign = -1;
        string++;
    } else if (*string == '+')
        string++;
    if (base == 0 && *string == '0') {
        numberFound = true;
        if (*(string+1) == 'x' || *(string+1) == 'X') {
            radix = 16;
            string += 2;
        } else {
            radix = 8;
            string++;
        }
    }
    do {
        int digit = -1;
        if (radix <= 10) {
            if (*string >= '0' && *string < '0' + radix)
                digit = *string - '0';
        } else {
            if (*string >= '0' && *string <= '9')
                digit = *string - '0';
            else if (*string >= 'a' && *string < 'a' + radix - 10)
                digit = *string - 'a' + 10;
            else if (*string >= 'A' && *string < 'A' + radix - 10)
                digit = *string - 'A' + 10;
        }
        if (digit != -1) {
            numberFound = true;
            result = radix*result + digit;
            string++;
        } else
            break;
    } while (true);
    *tail = numberFound ? string : start;
    return sign * result;
}


static float
strToF(const char *string, const char **tail)
{
    int nDigits = 0, pointPos, exponent;
    float sign = 1.0f, result = 0.0f, scale;
    const char *start = string, *numStart;

    /* sign */
    if (*string == '-') {
        sign = -1.0f;
        string++;
    } else if (*string == '+')
        string++;

    /* first pass: determine position of decimal point, number of
     * digits, exponent and the end of the number. */
    numStart = string;
    while (*string >= '0' && *string <= '9') {
        string++;
        nDigits++;
    }
    pointPos = nDigits;
    if (*string == '.') {
        string++;
        while (*string >= '0' && *string <= '9') {
            string++;
            nDigits++;
        }
    }
    if (nDigits == 0) {
        /* no digits, no number */
        *tail = start;
        return 0.0f;
    }
    *tail = string;
    if (*string == 'e' || *string == 'E') {
        const char *expTail;
        exponent = strToI(string+1, &expTail, 10);
        if (expTail == string+1)
            exponent = 0;
        else
            *tail = expTail;
    } else
        exponent = 0;
    string = numStart;

    /* scale of the first digit */
    scale = sign * (float)pow(10.0, (double)(pointPos-1 + exponent));

    /* second pass: parse digits */
    do {
        if (*string != '.') {
            assert(*string >= '0' && *string <= '9');
            result += scale * (float)(*string - '0');
            scale *= 0.1f;
            nDigits--;
        }
        string++;
    } while (nDigits > 0);

    return result;
}


static unsigned char
parseValue(driOptionValue *v, driOptionType type, const char *string)
{
    const char *tail = NULL;
    /* skip leading white-space */
    string += strspn(string, " \f\n\r\t\v");
    switch (type) {
        case DRI_BOOL:
            if (!strcmp(string, "false")) {
                v->_bool = false;
                tail = string + 5;
            } else if (!strcmp(string, "true")) {
                v->_bool = true;
                tail = string + 4;
            }
            else
                return false;
            break;
        case DRI_ENUM: /* enum is just a special integer */
        case DRI_INT:
            v->_int = strToI(string, &tail, 0);
            break;
        case DRI_FLOAT:
            v->_float = strToF(string, &tail);
            break;
        case DRI_STRING:
            free(v->_string);
            v->_string = strndup(string, STRING_CONF_MAXLEN);
            return true;
        case DRI_SECTION:
            unreachable("shouldn't be parsing values in section declarations");
    }

    if (tail == string)
        return false; /* empty string (or containing only white-space) */
    /* skip trailing white space */
    if (*tail)
        tail += strspn(tail, " \f\n\r\t\v");
    if (*tail)
        return false; /* something left over that is not part of value */

    return true;
}


static uint32_t
findOption(const driOptionCache *cache, const char *name)
{
    uint32_t len = strlen(name);
    uint32_t size = 1 << cache->tableSize, mask = size - 1;
    uint32_t hash = 0;
    uint32_t i, shift;

    /* compute a hash from the variable length name */
    for (i = 0, shift = 0; i < len; ++i, shift = (shift+8) & 31)
        hash += (uint32_t)name[i] << shift;
    hash *= hash;
    hash = (hash >> (16-cache->tableSize/2)) & mask;

    /* this is just the starting point of the linear search for the option */
    for (i = 0; i < size; ++i, hash = (hash+1) & mask) {
        /* if we hit an empty entry then the option is not defined (yet) */
        if (cache->info[hash].name == 0)
            break;
        else if (!strcmp(name, cache->info[hash].name))
            break;
    }
    /* this assertion fails if the hash table is full */
    assert (i < size);

    return hash;
}

#define XSTRDUP(dest,source) do {                                       \
      if (!(dest = strdup(source))) {                                   \
         fprintf(stderr, "%s: %d: out of memory.\n", __FILE__, __LINE__); \
         abort();                                                       \
      }                                                                 \
   } while (0)

void
driParseOptionInfo(driOptionCache *info,
                   const driOptionDescription *configOptions,
                   unsigned numOptions)
{
    /* Make the hash table big enough to fit more than the maximum number of
     * config options we've ever seen in a driver.
     */
    info->tableSize = 7;
    info->info = calloc((size_t)1 << info->tableSize, sizeof(driOptionInfo));
    info->values = calloc((size_t)1 << info->tableSize, sizeof(driOptionValue));
    if (info->info == NULL || info->values == NULL) {
        fprintf(stderr, "%s: %d: out of memory.\n", __FILE__, __LINE__);
        abort();
    }

    bool in_section = false;
    for (int o = 0; o < numOptions; o++) {
        const driOptionDescription *opt = &configOptions[o];

        if (opt->info.type == DRI_SECTION) {
            in_section = true;
            continue;
        }

        /* for driconf xml generation, options must always be preceded by a
         * DRI_CONF_SECTION
         */
        assert(in_section);

        const char *name = opt->info.name;
        int i = findOption(info, name);
        driOptionInfo *optinfo = &info->info[i];
        driOptionValue *optval = &info->values[i];

        assert(!optinfo->name); /* No duplicate options in your list. */

        optinfo->type = opt->info.type;
        optinfo->range = opt->info.range;
        XSTRDUP(optinfo->name, name);

        switch (opt->info.type) {
            case DRI_BOOL:
                optval->_bool = opt->value._bool;
                break;

            case DRI_INT:
            case DRI_ENUM:
                optval->_int = opt->value._int;
                break;

            case DRI_FLOAT:
                optval->_float = opt->value._float;
                break;

            case DRI_STRING:
                XSTRDUP(optval->_string, opt->value._string);
                break;

            case DRI_SECTION:
                unreachable("handled above");
        }

        /* Built-in default values should always be valid. */
        assert(checkValue(optval, optinfo));

        char *envVal = getenv(name);
        if (envVal != NULL) {
            driOptionValue v;

            /* make sure the value is initialized to something sensible */
            v._string = NULL;

            if (parseValue(&v, opt->info.type, envVal) &&
                checkValue(&v, optinfo)) {
                /* don't use XML_WARNING, we want the user to see this! */
                if (be_verbose()) {
                    fprintf(stderr,
                            "ATTENTION: default value of option %s overridden by environment.\n",
                            name);
                }
                *optval = v;
            } else {
                fprintf(stderr, "illegal environment value for %s: \"%s\".  Ignoring.\n",
                        name, envVal);
            }
        }
    }
}

static inline int
mtx_lock(mtx_t *mtx)
{
    assert(mtx != NULL);
    return (pthread_mutex_lock(mtx) == 0) ? thrd_success : thrd_error;
}

static inline int
mtx_unlock(mtx_t *mtx)
{
    assert(mtx != NULL);
    return (pthread_mutex_unlock(mtx) == 0) ? thrd_success : thrd_error;
}

static inline void
simple_mtx_lock(simple_mtx_t *mtx)
{
    mtx_lock(mtx);
}

static inline void
simple_mtx_unlock(simple_mtx_t *mtx)
{
    mtx_unlock(mtx);
}

const char *
os_get_option(const char *name)
{
    const char *opt = getenv(name);
#if DETECT_OS_ANDROID
    if (!opt) {
      opt = os_get_android_option(name);
   }
#endif
    return opt;
}

static bool
check_for_ending(const char *string, const char *ending)
{
    const size_t len1 = strlen(string);
    const size_t len2 = strlen(ending);

    if (len2 > len1)
        return false;

    return strcmp(string + (len1 - len2), ending) == 0;
}

static void
get_gl_override(gl_api api, int *version, bool *fwd_context,
                bool *compat_context)
{
    const char *env_var = (api == API_OPENGL_CORE || api == API_OPENGL_COMPAT)
                          ? "MESA_GL_VERSION_OVERRIDE" : "MESA_GLES_VERSION_OVERRIDE";
    const char *version_str;
    int major, minor, n;
    static struct override_info {
        int version;
        bool fc_suffix;
        bool compat_suffix;
    } override[] = {
            [API_OPENGL_COMPAT] = { -1, false, false},
            [API_OPENGLES]      = { -1, false, false},
            [API_OPENGLES2]     = { -1, false, false},
            [API_OPENGL_CORE]   = { -1, false, false},
    };

    STATIC_ASSERT(ARRAY_SIZE(override) == API_OPENGL_LAST + 1);

    simple_mtx_lock(&override_lock);

    if (api == API_OPENGLES)
        goto exit;

    if (override[api].version < 0) {
        override[api].version = 0;

        version_str = os_get_option(env_var);
        if (version_str) {
            override[api].fc_suffix = check_for_ending(version_str, "FC");
            override[api].compat_suffix = check_for_ending(version_str, "COMPAT");

            n = sscanf(version_str, "%u.%u", &major, &minor);
            if (n != 2) {
                fprintf(stderr, "error: invalid value for %s: %s\n",
                        env_var, version_str);
                override[api].version = 0;
            } else {
                override[api].version = major * 10 + minor;

                /* There is no such thing as compatibility or forward-compatible for
                 * OpenGL ES 2.0 or 3.x APIs.
                 */
                if ((override[api].version < 30 && override[api].fc_suffix) ||
                    (api == API_OPENGLES2 && (override[api].fc_suffix ||
                                              override[api].compat_suffix))) {
                    fprintf(stderr, "error: invalid value for %s: %s\n",
                            env_var, version_str);
                }
            }
        }
    }

    exit:
    *version = override[api].version;
    *fwd_context = override[api].fc_suffix;
    *compat_context = override[api].compat_suffix;

    simple_mtx_unlock(&override_lock);
}

bool
_mesa_override_gl_version_contextless(struct gl_constants *consts,
                                      gl_api *apiOut, GLuint *versionOut)
{
    int version;
    bool fwd_context, compat_context;

    get_gl_override(*apiOut, &version, &fwd_context, &compat_context);

    if (version > 0) {
        *versionOut = version;

        /* Modify the API and context flags as needed. */
        if (*apiOut == API_OPENGL_CORE || *apiOut == API_OPENGL_COMPAT) {
            if (version >= 30 && fwd_context) {
                *apiOut = API_OPENGL_CORE;
                consts->ContextFlags |= GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
            } else if (compat_context) {
                *apiOut = API_OPENGL_COMPAT;
            }
        }

        return true;
    }
    return false;
}

static __DRIscreen *
driCreateNewScreen2(int scrn, int fd,
                    const __DRIextension **extensions,
                    const __DRIextension **driver_extensions,
                    const __DRIconfig ***driver_configs, void *data)
{
    static const __DRIextension *emptyExtensionList[] = { NULL };
    __DRIscreen *psp;

    psp = calloc(1, sizeof(*psp));
    if (!psp)
        return NULL;

    /* By default, use the global driDriverAPI symbol (non-megadrivers). */
    psp->driver = globalDriverAPI;

    /* If the driver exposes its vtable through its extensions list
     * (megadrivers), use that instead.
     */
    if (driver_extensions) {
        for (int i = 0; driver_extensions[i]; i++) {
            if (strcmp(driver_extensions[i]->name, __DRI_DRIVER_VTABLE) == 0) {
                psp->driver =
                        ((__DRIDriverVtableExtension *)driver_extensions[i])->vtable;
            }
        }
    }

    setupLoaderExtensions(psp, extensions);

    psp->loaderPrivate = data;

    psp->extensions = emptyExtensionList;
    psp->fd = fd;
    psp->myNum = scrn;

    /* Option parsing before ->InitScreen(), as some options apply there. */
    driParseOptionInfo(&psp->optionInfo,
                       __dri2ConfigOptions, ARRAY_SIZE(__dri2ConfigOptions));
//    driParseConfigFiles(&psp->optionCache, &psp->optionInfo, psp->myNum,
//                        "dri2", NULL, NULL, NULL, 0, NULL, 0);

    *driver_configs = psp->driver->InitScreen(psp);
    if (*driver_configs == NULL) {
        free(psp);
        return NULL;
    }
//
    struct gl_constants consts = { 0 };
    gl_api api;
    unsigned version;

    api = API_OPENGLES2;
    if (_mesa_override_gl_version_contextless(&consts, &api, &version))
        psp->max_gl_es2_version = version;

    api = API_OPENGL_COMPAT;
    if (_mesa_override_gl_version_contextless(&consts, &api, &version)) {
        psp->max_gl_core_version = version;
        if (api == API_OPENGL_COMPAT)
            psp->max_gl_compat_version = version;
    }

    psp->api_mask = 0;
    if (psp->max_gl_compat_version > 0)
        psp->api_mask |= (1 << __DRI_API_OPENGL);
    if (psp->max_gl_core_version > 0)
        psp->api_mask |= (1 << __DRI_API_OPENGL_CORE);
    if (psp->max_gl_es1_version > 0)
        psp->api_mask |= (1 << __DRI_API_GLES);
    if (psp->max_gl_es2_version > 0)
        psp->api_mask |= (1 << __DRI_API_GLES2);
    if (psp->max_gl_es2_version >= 30)
        psp->api_mask |= (1 << __DRI_API_GLES3);

    return psp;
}

static __DRIdrawable *
driCreateNewDrawable(__DRIscreen *screen,
                     const __DRIconfig *config,
                     void *data)
{
    __DRIdrawable *pdraw;

    assert(data != NULL);

    pdraw = malloc(sizeof *pdraw);
    if (!pdraw)
        return NULL;

    pdraw->loaderPrivate = data;

    pdraw->driScreenPriv = screen;
    pdraw->driContextPriv = NULL;
    pdraw->refcount = 0;
    pdraw->lastStamp = 0;
    pdraw->w = 0;
    pdraw->h = 0;

    dri_get_drawable(pdraw);

    if (!screen->driver->CreateBuffer(screen, pdraw, &config->modes,
                                      GL_FALSE)) {
        free(pdraw);
        return NULL;
    }

    pdraw->dri2.stamp = pdraw->lastStamp + 1;

    return pdraw;
}


static bool
validate_context_version(__DRIscreen *screen,
                         int mesa_api,
                         unsigned major_version,
                         unsigned minor_version,
                         unsigned *dri_ctx_error)
{
    unsigned req_version = 10 * major_version + minor_version;
    unsigned max_version = 0;

    switch (mesa_api) {
        case API_OPENGL_COMPAT:
            max_version = screen->max_gl_compat_version;
            break;
        case API_OPENGL_CORE:
            max_version = screen->max_gl_core_version;
            break;
        case API_OPENGLES:
            max_version = screen->max_gl_es1_version;
            break;
        case API_OPENGLES2:
            max_version = screen->max_gl_es2_version;
            break;
        default:
            max_version = 0;
            break;
    }

    if (max_version == 0) {
        *dri_ctx_error = __DRI_CTX_ERROR_BAD_API;
        return false;
    } else if (req_version > max_version) {
        *dri_ctx_error = __DRI_CTX_ERROR_BAD_VERSION;
        return false;
    }

    return true;
}


static __DRIcontext *
driCreateContextAttribs(__DRIscreen *screen, int api,
                        const __DRIconfig *config,
                        __DRIcontext *shared,
                        unsigned num_attribs,
                        const uint32_t *attribs,
                        unsigned *error,
                        void *data)
{
    __DRIcontext *context;
    const struct gl_config *modes = (config != NULL) ? &config->modes : NULL;
    void *shareCtx = (shared != NULL) ? shared->driverPrivate : NULL;
    gl_api mesa_api;
    struct __DriverContextConfig ctx_config;

    ctx_config.major_version = 1;
    ctx_config.minor_version = 0;
    ctx_config.flags = 0;
    ctx_config.attribute_mask = 0;
    ctx_config.priority = __DRI_CTX_PRIORITY_MEDIUM;

    assert((num_attribs == 0) || (attribs != NULL));

    if (!(screen->api_mask & (1 << api))) {
        *error = __DRI_CTX_ERROR_BAD_API;
        return NULL;
    }

    switch (api) {
        case __DRI_API_OPENGL:
            mesa_api = API_OPENGL_COMPAT;
            break;
        case __DRI_API_GLES:
            mesa_api = API_OPENGLES;
            break;
        case __DRI_API_GLES2:
        case __DRI_API_GLES3:
            mesa_api = API_OPENGLES2;
            break;
        case __DRI_API_OPENGL_CORE:
            mesa_api = API_OPENGL_CORE;
            break;
        default:
            *error = __DRI_CTX_ERROR_BAD_API;
            return NULL;
    }

    for (unsigned i = 0; i < num_attribs; i++) {
        switch (attribs[i * 2]) {
            case __DRI_CTX_ATTRIB_MAJOR_VERSION:
                ctx_config.major_version = attribs[i * 2 + 1];
                break;
            case __DRI_CTX_ATTRIB_MINOR_VERSION:
                ctx_config.minor_version = attribs[i * 2 + 1];
                break;
            case __DRI_CTX_ATTRIB_FLAGS:
                ctx_config.flags = attribs[i * 2 + 1];
                break;
            case __DRI_CTX_ATTRIB_RESET_STRATEGY:
                if (attribs[i * 2 + 1] != __DRI_CTX_RESET_NO_NOTIFICATION) {
                    ctx_config.attribute_mask |=
                            __DRIVER_CONTEXT_ATTRIB_RESET_STRATEGY;
                    ctx_config.reset_strategy = attribs[i * 2 + 1];
                } else {
                    ctx_config.attribute_mask &=
                            ~__DRIVER_CONTEXT_ATTRIB_RESET_STRATEGY;
                }
                break;
            case __DRI_CTX_ATTRIB_PRIORITY:
                ctx_config.attribute_mask |= __DRIVER_CONTEXT_ATTRIB_PRIORITY;
                ctx_config.priority = attribs[i * 2 + 1];
                break;
            case __DRI_CTX_ATTRIB_RELEASE_BEHAVIOR:
                if (attribs[i * 2 + 1] != __DRI_CTX_RELEASE_BEHAVIOR_FLUSH) {
                    ctx_config.attribute_mask |=
                            __DRIVER_CONTEXT_ATTRIB_RELEASE_BEHAVIOR;
                    ctx_config.release_behavior = attribs[i * 2 + 1];
                } else {
                    ctx_config.attribute_mask &=
                            ~__DRIVER_CONTEXT_ATTRIB_RELEASE_BEHAVIOR;
                }
                break;
            default:
                /* We can't create a context that satisfies the requirements of an
                 * attribute that we don't understand.  Return failure.
                 */
                assert(!"Should not get here.");
                *error = __DRI_CTX_ERROR_UNKNOWN_ATTRIBUTE;
                return NULL;
        }
    }

    /* The specific Mesa driver may not support the GL_ARB_compatibilty
     * extension or the compatibility profile.  In that case, we treat an
     * API_OPENGL_COMPAT 3.1 as API_OPENGL_CORE. We reject API_OPENGL_COMPAT
     * 3.2+ in any case.
     */
    if (mesa_api == API_OPENGL_COMPAT &&
        ctx_config.major_version == 3 && ctx_config.minor_version == 1 &&
        screen->max_gl_compat_version < 31)
        mesa_api = API_OPENGL_CORE;

    /* The latest version of EGL_KHR_create_context spec says:
     *
     *     "If the EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR flag bit is set in
     *     EGL_CONTEXT_FLAGS_KHR, then a <debug context> will be created.
     *     [...] This bit is supported for OpenGL and OpenGL ES contexts.
     *
     * No other EGL_CONTEXT_OPENGL_*_BIT is legal for an ES context.
     *
     * However, Mesa's EGL layer translates the context attribute
     * EGL_CONTEXT_OPENGL_ROBUST_ACCESS into the context flag
     * __DRI_CTX_FLAG_ROBUST_BUFFER_ACCESS.  That attribute is legal for ES
     * (with EGL 1.5 or EGL_EXT_create_context_robustness) and GL (only with
     * EGL 1.5).
     *
     * From the EGL_EXT_create_context_robustness spec:
     *
     *     This extension is written against the OpenGL ES 2.0 Specification
     *     but can apply to OpenGL ES 1.1 and up.
     *
     * From the EGL 1.5 (2014.08.27) spec, p55:
     *
     *     If the EGL_CONTEXT_OPENGL_ROBUST_ACCESS attribute is set to
     *     EGL_TRUE, a context supporting robust buffer access will be created.
     *     OpenGL contexts must support the GL_ARB_robustness extension, or
     *     equivalent core API functional- ity. OpenGL ES contexts must support
     *     the GL_EXT_robustness extension, or equivalent core API
     *     functionality.
     */
    if (mesa_api != API_OPENGL_COMPAT
        && mesa_api != API_OPENGL_CORE
        && (ctx_config.flags & ~(__DRI_CTX_FLAG_DEBUG |
                                 __DRI_CTX_FLAG_ROBUST_BUFFER_ACCESS |
                                 __DRI_CTX_FLAG_NO_ERROR))) {
        *error = __DRI_CTX_ERROR_BAD_FLAG;
        return NULL;
    }

    /* There are no forward-compatible contexts before OpenGL 3.0.  The
     * GLX_ARB_create_context spec says:
     *
     *     "Forward-compatible contexts are defined only for OpenGL versions
     *     3.0 and later."
     *
     * Forward-looking contexts are supported by silently converting the
     * requested API to API_OPENGL_CORE.
     *
     * In Mesa, a debug context is the same as a regular context.
     */
    if ((ctx_config.flags & __DRI_CTX_FLAG_FORWARD_COMPATIBLE) != 0) {
        mesa_api = API_OPENGL_CORE;
    }

    const uint32_t allowed_flags = (__DRI_CTX_FLAG_DEBUG
                                    | __DRI_CTX_FLAG_FORWARD_COMPATIBLE
                                    | __DRI_CTX_FLAG_ROBUST_BUFFER_ACCESS
                                    | __DRI_CTX_FLAG_NO_ERROR);
    if (ctx_config.flags & ~allowed_flags) {
        *error = __DRI_CTX_ERROR_UNKNOWN_FLAG;
        return NULL;
    }

    if (!validate_context_version(screen, mesa_api,
                                  ctx_config.major_version,
                                  ctx_config.minor_version,
                                  error))
        return NULL;

    context = calloc(1, sizeof *context);
    if (!context) {
        *error = __DRI_CTX_ERROR_NO_MEMORY;
        return NULL;
    }

    context->loaderPrivate = data;

    context->driScreenPriv = screen;
    context->driDrawablePriv = NULL;
    context->driReadablePriv = NULL;

    if (!screen->driver->CreateContext(mesa_api, modes, context,
                                       &ctx_config, error, shareCtx)) {
        free(context);
        return NULL;
    }

    *error = __DRI_CTX_ERROR_SUCCESS;
    return context;
}


static __DRIcontext *
driCreateNewContextForAPI(__DRIscreen *screen, int api,
                          const __DRIconfig *config,
                          __DRIcontext *shared, void *data)
{
    unsigned error;

    return driCreateContextAttribs(screen, api, config, shared, 0, NULL,
                                   &error, data);
}



static __DRIscreen *
dri2CreateNewScreen(int scrn, int fd,
                    const __DRIextension **extensions,
                    const __DRIconfig ***driver_configs, void *data)
{
    return driCreateNewScreen2(scrn, fd, extensions, NULL,
                               driver_configs, data);
}

/** swrast driver createNewScreen entrypoint. */
static __DRIscreen *
driSWRastCreateNewScreen(int scrn, const __DRIextension **extensions,
                         const __DRIconfig ***driver_configs, void *data)
{
    return driCreateNewScreen2(scrn, -1, extensions, NULL,
                               driver_configs, data);
}

static __DRIscreen *
driSWRastCreateNewScreen2(int scrn, const __DRIextension **extensions,
                          const __DRIextension **driver_extensions,
                          const __DRIconfig ***driver_configs, void *data)
{
    return driCreateNewScreen2(scrn, -1, extensions, driver_extensions,
                               driver_configs, data);
}

void
driDestroyOptionCache(driOptionCache *cache)
{
    if (cache->info) {
        unsigned i, size = 1 << cache->tableSize;
        for (i = 0; i < size; ++i) {
            if (cache->info[i].type == DRI_STRING)
                free(cache->values[i]._string);
        }
    }
    free(cache->values);
}

void
driDestroyOptionInfo(driOptionCache *info)
{
    driDestroyOptionCache(info);
    if (info->info) {
        uint32_t i, size = 1 << info->tableSize;
        for (i = 0; i < size; ++i) {
            if (info->info[i].name) {
                free(info->info[i].name);
            }
        }
        free(info->info);
    }
}

char *
driQueryOptionstr(const driOptionCache *cache, const char *name)
{
    uint32_t i = findOption(cache, name);
    /* make sure the option is defined and has the correct type */
    assert(cache->info[i].name != NULL);
    assert(cache->info[i].type == DRI_STRING);
    return cache->values[i]._string;
}


static const __DRIextension **driGetExtensions(__DRIscreen *psp)
{
    return psp->extensions;
}



static void driDestroyScreen(__DRIscreen *psp)
{
    if (psp) {
        /* No interaction with the X-server is possible at this point.  This
         * routine is called after XCloseDisplay, so there is no protocol
         * stream open to the X-server anymore.
         */

        psp->driver->DestroyScreen(psp);

        driDestroyOptionCache(&psp->optionCache);
        driDestroyOptionInfo(&psp->optionInfo);

        free(psp);
    }
}


static int
driGetConfigAttribIndex(const __DRIconfig *config,
                        unsigned int index, unsigned int *value)
{
    switch (index + 1) {
        __ATTRIB(__DRI_ATTRIB_BUFFER_SIZE,			rgbBits);
        __ATTRIB(__DRI_ATTRIB_RED_SIZE,			redBits);
        __ATTRIB(__DRI_ATTRIB_GREEN_SIZE,			greenBits);
        __ATTRIB(__DRI_ATTRIB_BLUE_SIZE,			blueBits);
        case __DRI_ATTRIB_LEVEL:
        case __DRI_ATTRIB_LUMINANCE_SIZE:
        case __DRI_ATTRIB_AUX_BUFFERS:
            *value = 0;
            break;
            __ATTRIB(__DRI_ATTRIB_ALPHA_SIZE,			alphaBits);
        case __DRI_ATTRIB_ALPHA_MASK_SIZE:
            /* I have no idea what this value was ever meant to mean, it's
             * never been set to anything, just say 0.
             */
            *value = 0;
            break;
            __ATTRIB(__DRI_ATTRIB_DEPTH_SIZE,			depthBits);
            __ATTRIB(__DRI_ATTRIB_STENCIL_SIZE,			stencilBits);
            __ATTRIB(__DRI_ATTRIB_ACCUM_RED_SIZE,		accumRedBits);
            __ATTRIB(__DRI_ATTRIB_ACCUM_GREEN_SIZE,		accumGreenBits);
            __ATTRIB(__DRI_ATTRIB_ACCUM_BLUE_SIZE,		accumBlueBits);
            __ATTRIB(__DRI_ATTRIB_ACCUM_ALPHA_SIZE,		accumAlphaBits);
        case __DRI_ATTRIB_SAMPLE_BUFFERS:
            *value = !!config->modes.samples;
            break;
            __ATTRIB(__DRI_ATTRIB_SAMPLES,			samples);
        case __DRI_ATTRIB_RENDER_TYPE:
            /* no support for color index mode */
            *value = __DRI_ATTRIB_RGBA_BIT;
            if (config->modes.floatMode)
                *value |= __DRI_ATTRIB_FLOAT_BIT;
            break;
        case __DRI_ATTRIB_CONFIG_CAVEAT:
            if (config->modes.accumRedBits != 0)
                *value = __DRI_ATTRIB_SLOW_BIT;
            else
                *value = 0;
            break;
        case __DRI_ATTRIB_CONFORMANT:
            *value = GL_TRUE;
            break;
            __ATTRIB(__DRI_ATTRIB_DOUBLE_BUFFER,		doubleBufferMode);
            __ATTRIB(__DRI_ATTRIB_STEREO,			stereoMode);
        case __DRI_ATTRIB_TRANSPARENT_TYPE:
        case __DRI_ATTRIB_TRANSPARENT_INDEX_VALUE: /* horrible bc hack */
            *value = GLX_NONE;
            break;
        case __DRI_ATTRIB_TRANSPARENT_RED_VALUE:
        case __DRI_ATTRIB_TRANSPARENT_GREEN_VALUE:
        case __DRI_ATTRIB_TRANSPARENT_BLUE_VALUE:
        case __DRI_ATTRIB_TRANSPARENT_ALPHA_VALUE:
            *value = GLX_DONT_CARE;
            break;
        case __DRI_ATTRIB_FLOAT_MODE:
            *value = config->modes.floatMode;
            break;
            __ATTRIB(__DRI_ATTRIB_RED_MASK,			redMask);
            __ATTRIB(__DRI_ATTRIB_GREEN_MASK,			greenMask);
            __ATTRIB(__DRI_ATTRIB_BLUE_MASK,			blueMask);
            __ATTRIB(__DRI_ATTRIB_ALPHA_MASK,			alphaMask);
        case __DRI_ATTRIB_MAX_PBUFFER_WIDTH:
        case __DRI_ATTRIB_MAX_PBUFFER_HEIGHT:
        case __DRI_ATTRIB_MAX_PBUFFER_PIXELS:
        case __DRI_ATTRIB_OPTIMAL_PBUFFER_WIDTH:
        case __DRI_ATTRIB_OPTIMAL_PBUFFER_HEIGHT:
        case __DRI_ATTRIB_VISUAL_SELECT_GROUP:
            *value = 0;
            break;
            __ATTRIB(__DRI_ATTRIB_SWAP_METHOD,			swapMethod);
        case __DRI_ATTRIB_MAX_SWAP_INTERVAL:
            *value = INT_MAX;
            break;
        case __DRI_ATTRIB_MIN_SWAP_INTERVAL:
            *value = 0;
            break;
        case __DRI_ATTRIB_BIND_TO_TEXTURE_RGB:
        case __DRI_ATTRIB_BIND_TO_TEXTURE_RGBA:
        case __DRI_ATTRIB_YINVERTED:
            *value = GL_TRUE;
            break;
        case __DRI_ATTRIB_BIND_TO_MIPMAP_TEXTURE:
            *value = GL_FALSE;
            break;
        case __DRI_ATTRIB_BIND_TO_TEXTURE_TARGETS:
            *value = __DRI_ATTRIB_TEXTURE_1D_BIT |
                     __DRI_ATTRIB_TEXTURE_2D_BIT |
                     __DRI_ATTRIB_TEXTURE_RECTANGLE_BIT;
            break;
            __ATTRIB(__DRI_ATTRIB_FRAMEBUFFER_SRGB_CAPABLE,	sRGBCapable);
        case __DRI_ATTRIB_MUTABLE_RENDER_BUFFER:
            *value = GL_FALSE;
            break;
            __ATTRIB(__DRI_ATTRIB_RED_SHIFT,			redShift);
            __ATTRIB(__DRI_ATTRIB_GREEN_SHIFT,			greenShift);
            __ATTRIB(__DRI_ATTRIB_BLUE_SHIFT,			blueShift);
            __ATTRIB(__DRI_ATTRIB_ALPHA_SHIFT,			alphaShift);
        default:
            /* XXX log an error or smth */
            return GL_FALSE;
    }

    return GL_TRUE;
}

int
driGetConfigAttrib(const __DRIconfig *config,
                   unsigned int attrib, unsigned int *value)
{
    return driGetConfigAttribIndex(config, attrib - 1, value);
}

int
driIndexConfigAttrib(const __DRIconfig *config, int index,
                     unsigned int *attrib, unsigned int *value)
{
    if (driGetConfigAttribIndex(config, index, value)) {
        *attrib = index + 1;
        return GL_TRUE;
    }

    return GL_FALSE;
}

static void dri_put_drawable(__DRIdrawable *pdp)
{
    if (pdp) {
        pdp->refcount--;
        if (pdp->refcount)
            return;

        pdp->driScreenPriv->driver->DestroyBuffer(pdp);
        free(pdp);
    }
}

static void
driDestroyDrawable(__DRIdrawable *pdp)
{
    pdp->loaderPrivate = NULL;

    dri_put_drawable(pdp);
}

static void
driSwapBuffers(__DRIdrawable *pdp)
{
    assert(pdp->driScreenPriv->swrast_loader);

    pdp->driScreenPriv->driver->SwapBuffers(pdp);
}

static __DRIcontext *
driCreateNewContext(__DRIscreen *screen, const __DRIconfig *config,
                    __DRIcontext *shared, void *data)
{
    return driCreateNewContextForAPI(screen, __DRI_API_OPENGL,
                                     config, shared, data);
}

static void
driDestroyContext(__DRIcontext *pcp)
{
    if (pcp) {
        pcp->driScreenPriv->driver->DestroyContext(pcp);
        free(pcp);
    }
}

static int
driCopyContext(__DRIcontext *dest, __DRIcontext *src, unsigned long mask)
{
    (void) dest;
    (void) src;
    (void) mask;
    return GL_FALSE;
}

static int driBindContext(__DRIcontext *pcp,
                          __DRIdrawable *pdp,
                          __DRIdrawable *prp)
{
    if (!pcp)
        return GL_FALSE;

    pcp->driDrawablePriv = pdp;
    pcp->driReadablePriv = prp;
    if (pdp) {
        pdp->driContextPriv = pcp;
        dri_get_drawable(pdp);
    }
    if (prp && pdp != prp) {
        dri_get_drawable(prp);
    }

    return pcp->driScreenPriv->driver->MakeCurrent(pcp, pdp, prp);
}

static int driUnbindContext(__DRIcontext *pcp)
{
    __DRIdrawable *pdp;
    __DRIdrawable *prp;

    /*
    ** Assume error checking is done properly in glXMakeCurrent before
    ** calling driUnbindContext.
    */

    if (pcp == NULL)
        return GL_FALSE;

    /*
    ** Call driUnbindContext before checking for valid drawables
    ** to handle surfaceless contexts properly.
    */
    pcp->driScreenPriv->driver->UnbindContext(pcp);

    pdp = pcp->driDrawablePriv;
    prp = pcp->driReadablePriv;

    /* already unbound */
    if (!pdp && !prp)
        return GL_TRUE;

    assert(pdp);
    if (pdp->refcount == 0) {
        /* ERROR!!! */
        return GL_FALSE;
    }

    dri_put_drawable(pdp);

    if (prp != pdp) {
        if (prp->refcount == 0) {
            /* ERROR!!! */
            return GL_FALSE;
        }

        dri_put_drawable(prp);
    }

    pcp->driDrawablePriv = NULL;
    pcp->driReadablePriv = NULL;

    return GL_TRUE;
}

const __DRIcoreExtension driCoreExtension = {
        .base = { __DRI_CORE, 2 },

        .createNewScreen            = NULL,
        .destroyScreen              = driDestroyScreen,
        .getExtensions              = driGetExtensions,
        .getConfigAttrib            = driGetConfigAttrib,
        .indexConfigAttrib          = driIndexConfigAttrib,
        .createNewDrawable          = NULL,
        .destroyDrawable            = driDestroyDrawable,
        .swapBuffers                = driSwapBuffers, /* swrast */
        .createNewContext           = driCreateNewContext, /* swrast */
        .copyContext                = driCopyContext,
        .destroyContext             = driDestroyContext,
        .bindContext                = driBindContext,
        .unbindContext              = driUnbindContext
};


const __DRIswrastExtension driSWRastExtension = {
        .base = { __DRI_SWRAST, 4 },
        .createNewScreen            = driSWRastCreateNewScreen,
        .createNewDrawable          = driCreateNewDrawable,
        .createNewContextForAPI     = driCreateNewContextForAPI,
        .createContextAttribs       = driCreateContextAttribs,
        .createNewScreen2           = driSWRastCreateNewScreen2,
};

static void driCopySubBuffer(__DRIdrawable *pdp, int x, int y,
                             int w, int h)
{
    assert(pdp->driScreenPriv->swrast_loader);

    pdp->driScreenPriv->driver->CopySubBuffer(pdp, x, y, w, h);
}

/* for swrast only */
const __DRIcopySubBufferExtension driCopySubBufferExtension = {
        .base = { __DRI_COPY_SUB_BUFFER, 1 },

        .copySubBuffer               = driCopySubBuffer,
};


const __DRIextension *galliumsw_driver_extensions[] = {
        &driCoreExtension.base,
        &driSWRastExtension.base,
        &driCopySubBufferExtension.base,
//        &gallium_config_options.base,
        NULL
};

const struct __DriverAPIRec galliumsw_driver_api = {
        .InitScreen = stub_error_init_screen,
//        .InitScreen = drisw_init_screen,
//        .DestroyScreen = dri_destroy_screen,
//        .CreateContext = dri_create_context,
//        .DestroyContext = dri_destroy_context,
//        .CreateBuffer = drisw_create_buffer,
//        .DestroyBuffer = dri_destroy_buffer,
//        .SwapBuffers = drisw_swap_buffers,
//        .MakeCurrent = dri_make_current,
//        .UnbindContext = dri_unbind_context,
//        .CopySubBuffer = drisw_copy_sub_buffer,
};

const __DRIextension **__driDriverGetExtensions_swrast(void)
{
    globalDriverAPI = &galliumsw_driver_api;
    return galliumsw_driver_extensions;
}

void
_mesa_hash_table_remove(struct hash_table *ht,
                        struct hash_entry *entry)
{
    if (!entry)
        return;

    entry->key = ht->deleted_key;
    ht->entries--;
    ht->deleted_entries++;
}

static tss_t u_current_context_tsd;
static int ThreadSafe;

static inline void *
tss_get(tss_t key)
{
    return pthread_getspecific(key);
}

typedef thrd_t thread_id;

static inline int
thrd_equal(thrd_t thr0, thrd_t thr1)
{
    return pthread_equal(thr0, thr1);
}

static inline int
util_thread_id_equal(thread_id t1, thread_id t2)
{
#ifdef _WIN32
    return t1 == t2;
#else
    return thrd_equal(t1, t2);
#endif
}

static inline thrd_t
thrd_current(void)
{
    return pthread_self();
}

static thread_id knownID;

static inline thread_id
util_get_thread_id(void)
{
    /*
     * XXX: Callers of of this function assume it is a lightweight function.
     * But unfortunately C11's thrd_current() gives no such guarantees.  In
     * fact, it's pretty hard to have a compliant implementation of
     * thrd_current() on Windows with such characteristics.  So for now, we
     * side-step this mess and use Windows thread primitives directly here.
     */
#ifdef _WIN32
    return GetCurrentThreadId();
#else
    return thrd_current();
#endif
}

void *
u_current_get_context_internal(void)
{
#if defined(USE_ELF_TLS)
    return u_current_context;
#else
    if (ThreadSafe)
        return tss_get(u_current_context_tsd);
    else if (!util_thread_id_equal(knownID, util_get_thread_id()))
        return NULL;
    else
        return u_current_context;
#endif
}

static void
init_program_limits(struct gl_constants *consts, gl_shader_stage stage,
                    struct gl_program_constants *prog)
{
    prog->MaxInstructions = MAX_PROGRAM_INSTRUCTIONS;
    prog->MaxAluInstructions = MAX_PROGRAM_INSTRUCTIONS;
    prog->MaxTexInstructions = MAX_PROGRAM_INSTRUCTIONS;
    prog->MaxTexIndirections = MAX_PROGRAM_INSTRUCTIONS;
    prog->MaxTemps = MAX_PROGRAM_TEMPS;
    prog->MaxEnvParams = MAX_PROGRAM_ENV_PARAMS;
    prog->MaxLocalParams = MAX_PROGRAM_LOCAL_PARAMS;
    prog->MaxAddressOffset = MAX_PROGRAM_LOCAL_PARAMS;

    switch (stage) {
        case MESA_SHADER_VERTEX:
            prog->MaxParameters = MAX_VERTEX_PROGRAM_PARAMS;
            prog->MaxAttribs = MAX_VERTEX_GENERIC_ATTRIBS;
            prog->MaxAddressRegs = MAX_VERTEX_PROGRAM_ADDRESS_REGS;
            prog->MaxUniformComponents = 4 * MAX_UNIFORMS;
            prog->MaxInputComponents = 0; /* value not used */
            prog->MaxOutputComponents = 16 * 4; /* old limit not to break tnl and swrast */
            break;
        case MESA_SHADER_FRAGMENT:
            prog->MaxParameters = MAX_FRAGMENT_PROGRAM_PARAMS;
            prog->MaxAttribs = MAX_FRAGMENT_PROGRAM_INPUTS;
            prog->MaxAddressRegs = MAX_FRAGMENT_PROGRAM_ADDRESS_REGS;
            prog->MaxUniformComponents = 4 * MAX_UNIFORMS;
            prog->MaxInputComponents = 16 * 4; /* old limit not to break tnl and swrast */
            prog->MaxOutputComponents = 0; /* value not used */
            break;
        case MESA_SHADER_TESS_CTRL:
        case MESA_SHADER_TESS_EVAL:
        case MESA_SHADER_GEOMETRY:
            prog->MaxParameters = MAX_VERTEX_PROGRAM_PARAMS;
            prog->MaxAttribs = MAX_VERTEX_GENERIC_ATTRIBS;
            prog->MaxAddressRegs = MAX_VERTEX_PROGRAM_ADDRESS_REGS;
            prog->MaxUniformComponents = 4 * MAX_UNIFORMS;
            prog->MaxInputComponents = 16 * 4; /* old limit not to break tnl and swrast */
            prog->MaxOutputComponents = 16 * 4; /* old limit not to break tnl and swrast */
            break;
        case MESA_SHADER_COMPUTE:
            prog->MaxParameters = 0; /* not meaningful for compute shaders */
            prog->MaxAttribs = 0; /* not meaningful for compute shaders */
            prog->MaxAddressRegs = 0; /* not meaningful for compute shaders */
            prog->MaxUniformComponents = 4 * MAX_UNIFORMS;
            prog->MaxInputComponents = 0; /* not meaningful for compute shaders */
            prog->MaxOutputComponents = 0; /* not meaningful for compute shaders */
            break;
        default:
            assert(0 && "Bad shader stage in init_program_limits()");
    }

    /* Set the native limits to zero.  This implies that there is no native
     * support for shaders.  Let the drivers fill in the actual values.
     */
    prog->MaxNativeInstructions = 0;
    prog->MaxNativeAluInstructions = 0;
    prog->MaxNativeTexInstructions = 0;
    prog->MaxNativeTexIndirections = 0;
    prog->MaxNativeAttribs = 0;
    prog->MaxNativeTemps = 0;
    prog->MaxNativeAddressRegs = 0;
    prog->MaxNativeParameters = 0;

    /* Set GLSL datatype range/precision info assuming IEEE float values.
     * Drivers should override these defaults as needed.
     */
    prog->MediumFloat.RangeMin = 127;
    prog->MediumFloat.RangeMax = 127;
    prog->MediumFloat.Precision = 23;
    prog->LowFloat = prog->HighFloat = prog->MediumFloat;

    /* Assume ints are stored as floats for now, since this is the least-common
     * denominator.  The OpenGL ES spec implies (page 132) that the precision
     * of integer types should be 0.  Practically speaking, IEEE
     * single-precision floating point values can only store integers in the
     * range [-0x01000000, 0x01000000] without loss of precision.
     */
    prog->MediumInt.RangeMin = 24;
    prog->MediumInt.RangeMax = 24;
    prog->MediumInt.Precision = 0;
    prog->LowInt = prog->HighInt = prog->MediumInt;

    prog->MaxUniformBlocks = 12;
    prog->MaxCombinedUniformComponents = (prog->MaxUniformComponents +
                                          consts->MaxUniformBlockSize / 4 *
                                          prog->MaxUniformBlocks);

    prog->MaxAtomicBuffers = 0;
    prog->MaxAtomicCounters = 0;

    prog->MaxShaderStorageBlocks = 8;
}

void
_mesa_init_constants(struct gl_constants *consts, gl_api api)
{
    int i;
    assert(consts);

    /* Constants, may be overriden (usually only reduced) by device drivers */
    consts->MaxTextureMbytes = MAX_TEXTURE_MBYTES;
    consts->MaxTextureSize = 1 << (MAX_TEXTURE_LEVELS - 1);
    consts->Max3DTextureLevels = MAX_TEXTURE_LEVELS;
    consts->MaxCubeTextureLevels = MAX_TEXTURE_LEVELS;
    consts->MaxTextureRectSize = MAX_TEXTURE_RECT_SIZE;
    consts->MaxArrayTextureLayers = MAX_ARRAY_TEXTURE_LAYERS;
    consts->MaxTextureCoordUnits = MAX_TEXTURE_COORD_UNITS;
    consts->Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits = MAX_TEXTURE_IMAGE_UNITS;
    consts->MaxTextureUnits = MIN2(consts->MaxTextureCoordUnits,
                                   consts->Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits);
    consts->MaxTextureMaxAnisotropy = MAX_TEXTURE_MAX_ANISOTROPY;
    consts->MaxTextureLodBias = MAX_TEXTURE_LOD_BIAS;
    consts->MaxTextureBufferSize = 65536;
    consts->TextureBufferOffsetAlignment = 1;
    consts->MaxArrayLockSize = MAX_ARRAY_LOCK_SIZE;
    consts->SubPixelBits = SUB_PIXEL_BITS;
    consts->MinPointSize = MIN_POINT_SIZE;
    consts->MaxPointSize = MAX_POINT_SIZE;
    consts->MinPointSizeAA = MIN_POINT_SIZE;
    consts->MaxPointSizeAA = MAX_POINT_SIZE;
    consts->PointSizeGranularity = (GLfloat) POINT_SIZE_GRANULARITY;
    consts->MinLineWidth = MIN_LINE_WIDTH;
    consts->MaxLineWidth = MAX_LINE_WIDTH;
    consts->MinLineWidthAA = MIN_LINE_WIDTH;
    consts->MaxLineWidthAA = MAX_LINE_WIDTH;
    consts->LineWidthGranularity = (GLfloat) LINE_WIDTH_GRANULARITY;
    consts->MaxClipPlanes = 6;
    consts->MaxLights = MAX_LIGHTS;
    consts->MaxShininess = 128.0;
    consts->MaxSpotExponent = 128.0;
    consts->MaxViewportWidth = 16384;
    consts->MaxViewportHeight = 16384;
    consts->MinMapBufferAlignment = 64;

    /* Driver must override these values if ARB_viewport_array is supported. */
    consts->MaxViewports = 1;
    consts->ViewportSubpixelBits = 0;
    consts->ViewportBounds.Min = 0;
    consts->ViewportBounds.Max = 0;

    /** GL_ARB_uniform_buffer_object */
    consts->MaxCombinedUniformBlocks = 36;
    consts->MaxUniformBufferBindings = 36;
    consts->MaxUniformBlockSize = 16384;
    consts->UniformBufferOffsetAlignment = 1;

    /** GL_ARB_shader_storage_buffer_object */
    consts->MaxCombinedShaderStorageBlocks = 8;
    consts->MaxShaderStorageBufferBindings = 8;
    consts->MaxShaderStorageBlockSize = 128 * 1024 * 1024; /* 2^27 */
    consts->ShaderStorageBufferOffsetAlignment = 256;

    /* GL_ARB_explicit_uniform_location, GL_MAX_UNIFORM_LOCATIONS */
    consts->MaxUserAssignableUniformLocations =
            4 * MESA_SHADER_STAGES * MAX_UNIFORMS;

    for (i = 0; i < MESA_SHADER_STAGES; i++)
        init_program_limits(consts, i, &consts->Program[i]);

    consts->MaxProgramMatrices = MAX_PROGRAM_MATRICES;
    consts->MaxProgramMatrixStackDepth = MAX_PROGRAM_MATRIX_STACK_DEPTH;

    /* Set the absolute minimum possible GLSL version.  API_OPENGL_CORE can
     * mean an OpenGL 3.0 forward-compatible context, so that implies a minimum
     * possible version of 1.30.  Otherwise, the minimum possible version 1.20.
     * Since Mesa unconditionally advertises GL_ARB_shading_language_100 and
     * GL_ARB_shader_objects, every driver has GLSL 1.20... even if they don't
     * advertise any extensions to enable any shader stages (e.g.,
     * GL_ARB_vertex_shader).
     */
    consts->GLSLVersion = api == API_OPENGL_CORE ? 130 : 120;
    consts->GLSLVersionCompat = consts->GLSLVersion;

    consts->GLSLLowerConstArrays = true;

    /* Assume that if GLSL 1.30+ (or GLSL ES 3.00+) is supported that
     * gl_VertexID is implemented using a native hardware register with OpenGL
     * semantics.
     */
    consts->VertexID_is_zero_based = false;

    /* GL_ARB_draw_buffers */
    consts->MaxDrawBuffers = MAX_DRAW_BUFFERS;

    consts->MaxColorAttachments = MAX_COLOR_ATTACHMENTS;
    consts->MaxRenderbufferSize = MAX_RENDERBUFFER_SIZE;

    consts->Program[MESA_SHADER_VERTEX].MaxTextureImageUnits = MAX_TEXTURE_IMAGE_UNITS;
    consts->MaxCombinedTextureImageUnits = MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    consts->MaxVarying = 16; /* old limit not to break tnl and swrast */
    consts->Program[MESA_SHADER_GEOMETRY].MaxTextureImageUnits = MAX_TEXTURE_IMAGE_UNITS;
    consts->MaxGeometryOutputVertices = MAX_GEOMETRY_OUTPUT_VERTICES;
    consts->MaxGeometryTotalOutputComponents = MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS;
    consts->MaxGeometryShaderInvocations = MAX_GEOMETRY_SHADER_INVOCATIONS;

#ifdef DEBUG
    consts->GenerateTemporaryNames = true;
#else
    consts->GenerateTemporaryNames = false;
#endif

    /* GL_ARB_framebuffer_object */
    consts->MaxSamples = 0;

    /* GLSL default if NativeIntegers == FALSE */
    consts->UniformBooleanTrue = FLOAT_AS_UNION(1.0f).u;

    /* GL_ARB_sync */
    consts->MaxServerWaitTimeout = 0x7fffffff7fffffffULL;

    /* GL_EXT_provoking_vertex */
    consts->QuadsFollowProvokingVertexConvention = GL_TRUE;

    /** GL_ARB_viewport_array */
    consts->LayerAndVPIndexProvokingVertex = GL_UNDEFINED_VERTEX;

    /* GL_EXT_transform_feedback */
    consts->MaxTransformFeedbackBuffers = MAX_FEEDBACK_BUFFERS;
    consts->MaxTransformFeedbackSeparateComponents = 4 * MAX_FEEDBACK_ATTRIBS;
    consts->MaxTransformFeedbackInterleavedComponents = 4 * MAX_FEEDBACK_ATTRIBS;
    consts->MaxVertexStreams = 1;

    /* GL 3.2  */
    consts->ProfileMask = api == API_OPENGL_CORE
                          ? GL_CONTEXT_CORE_PROFILE_BIT
                          : GL_CONTEXT_COMPATIBILITY_PROFILE_BIT;

    /* GL 4.4 */
    consts->MaxVertexAttribStride = 2048;

    /** GL_EXT_gpu_shader4 */
    consts->MinProgramTexelOffset = -8;
    consts->MaxProgramTexelOffset = 7;

    /* GL_ARB_texture_gather */
    consts->MinProgramTextureGatherOffset = -8;
    consts->MaxProgramTextureGatherOffset = 7;

    /* GL_ARB_robustness */
    consts->ResetStrategy = GL_NO_RESET_NOTIFICATION_ARB;

    /* GL_KHR_robustness */
    consts->RobustAccess = GL_FALSE;

    /* ES 3.0 or ARB_ES3_compatibility */
    consts->MaxElementIndex = 0xffffffffu;

    /* GL_ARB_texture_multisample */
    consts->MaxColorTextureSamples = 1;
    consts->MaxDepthTextureSamples = 1;
    consts->MaxIntegerSamples = 1;

    /* GL_ARB_shader_atomic_counters */
    consts->MaxAtomicBufferBindings = MAX_COMBINED_ATOMIC_BUFFERS;
    consts->MaxAtomicBufferSize = MAX_ATOMIC_COUNTERS * ATOMIC_COUNTER_SIZE;
    consts->MaxCombinedAtomicBuffers = MAX_COMBINED_ATOMIC_BUFFERS;
    consts->MaxCombinedAtomicCounters = MAX_ATOMIC_COUNTERS;

    /* GL_ARB_vertex_attrib_binding */
    consts->MaxVertexAttribRelativeOffset = 2047;
    consts->MaxVertexAttribBindings = MAX_VERTEX_GENERIC_ATTRIBS;

    /* GL_ARB_compute_shader */
    consts->MaxComputeWorkGroupCount[0] = 65535;
    consts->MaxComputeWorkGroupCount[1] = 65535;
    consts->MaxComputeWorkGroupCount[2] = 65535;
    consts->MaxComputeWorkGroupSize[0] = 1024;
    consts->MaxComputeWorkGroupSize[1] = 1024;
    consts->MaxComputeWorkGroupSize[2] = 64;
    /* Enables compute support for GLES 3.1 if >= 128 */
    consts->MaxComputeWorkGroupInvocations = 0;

    /** GL_ARB_gpu_shader5 */
    consts->MinFragmentInterpolationOffset = MIN_FRAGMENT_INTERPOLATION_OFFSET;
    consts->MaxFragmentInterpolationOffset = MAX_FRAGMENT_INTERPOLATION_OFFSET;

    /** GL_KHR_context_flush_control */
    consts->ContextReleaseBehavior = GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH;

    /** GL_ARB_tessellation_shader */
    consts->MaxTessGenLevel = MAX_TESS_GEN_LEVEL;
    consts->MaxPatchVertices = MAX_PATCH_VERTICES;
    consts->Program[MESA_SHADER_TESS_CTRL].MaxTextureImageUnits = MAX_TEXTURE_IMAGE_UNITS;
    consts->Program[MESA_SHADER_TESS_EVAL].MaxTextureImageUnits = MAX_TEXTURE_IMAGE_UNITS;
    consts->MaxTessPatchComponents = MAX_TESS_PATCH_COMPONENTS;
    consts->MaxTessControlTotalOutputComponents = MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS;
    consts->PrimitiveRestartForPatches = false;

    /** GL_ARB_compute_variable_group_size */
    consts->MaxComputeVariableGroupSize[0] = 512;
    consts->MaxComputeVariableGroupSize[1] = 512;
    consts->MaxComputeVariableGroupSize[2] = 64;
    consts->MaxComputeVariableGroupInvocations = 512;

    /** GL_NV_conservative_raster */
    consts->MaxSubpixelPrecisionBiasBits = 0;

    /** GL_NV_conservative_raster_dilate */
    consts->ConservativeRasterDilateRange[0] = 0.0;
    consts->ConservativeRasterDilateRange[1] = 0.0;
    consts->ConservativeRasterDilateGranularity = 0.0;

    consts->glBeginEndBufferSize = 512 * 1024;
}

#define o(x) offsetof(struct gl_extensions, x)

void
_mesa_init_extensions(struct gl_extensions *extensions)
{
    GLboolean *base = (GLboolean *) extensions;
    GLboolean *sentinel = base + o(extension_sentinel);
    GLboolean *i;

    /* First, turn all extensions off. */
    for (i = base; i != sentinel; ++i)
        *i = GL_FALSE;

    /* Then, selectively turn default extensions on. */
    extensions->dummy_true = GL_TRUE;

    /* Always enable these extensions for all drivers.
     * We can't use dummy_true in extensions_table.h for these
     * because this would make them non-disablable using
     * _mesa_override_extensions.
     */
    extensions->MESA_pack_invert = GL_TRUE;
    extensions->MESA_window_pos = GL_TRUE;

    extensions->ARB_ES2_compatibility = GL_TRUE;
    extensions->ARB_draw_elements_base_vertex = GL_TRUE;
    extensions->ARB_explicit_attrib_location = GL_TRUE;
    extensions->ARB_explicit_uniform_location = GL_TRUE;
    extensions->ARB_fragment_coord_conventions = GL_TRUE;
    extensions->ARB_fragment_program = GL_TRUE;
    extensions->ARB_fragment_shader = GL_TRUE;
    extensions->ARB_half_float_vertex = GL_TRUE;
    extensions->ARB_internalformat_query = GL_TRUE;
    extensions->ARB_internalformat_query2 = GL_TRUE;
    extensions->ARB_map_buffer_range = GL_TRUE;
    extensions->ARB_occlusion_query = GL_TRUE;
    extensions->ARB_sync = GL_TRUE;
    extensions->ARB_vertex_program = GL_TRUE;
    extensions->ARB_vertex_shader = GL_TRUE;

    extensions->EXT_EGL_image_storage = GL_TRUE;
    extensions->EXT_gpu_program_parameters = GL_TRUE;
    extensions->EXT_pixel_buffer_object = GL_TRUE;
    extensions->EXT_provoking_vertex = GL_TRUE;
    extensions->EXT_stencil_two_side = GL_TRUE;
    extensions->EXT_texture_env_dot3 = GL_TRUE;

    extensions->ATI_fragment_shader = GL_TRUE;
    extensions->ATI_texture_env_combine3 = GL_TRUE;

    extensions->MESA_framebuffer_flip_y = GL_TRUE;

    extensions->NV_copy_image = GL_TRUE;
    extensions->NV_fog_distance = GL_TRUE;
    extensions->NV_texture_env_combine4 = GL_TRUE;
    extensions->NV_texture_rectangle = GL_TRUE;

    extensions->OES_EGL_image = GL_TRUE;
    extensions->OES_EGL_image_external = GL_TRUE;
    extensions->OES_draw_texture = GL_TRUE;
}

static GLuint
compute_version(const struct gl_extensions *extensions,
                const struct gl_constants *consts, gl_api api)
{
    GLuint major, minor, version;

    const bool ver_1_4 = (extensions->ARB_shadow);
    const bool ver_1_5 = (ver_1_4 &&
                          extensions->ARB_occlusion_query);
    const bool ver_2_0 = (ver_1_5 &&
                          extensions->ARB_point_sprite &&
                          extensions->ARB_vertex_shader &&
                          extensions->ARB_fragment_shader &&
                          extensions->ARB_texture_non_power_of_two &&
                          extensions->EXT_blend_equation_separate &&
                          extensions->EXT_stencil_two_side);
    const bool ver_2_1 = (ver_2_0 &&
                          extensions->EXT_pixel_buffer_object &&
                          extensions->EXT_texture_sRGB);
    /* We lie about the minimum number of color attachments. Strictly, OpenGL
     * 3.0 requires 8, whereas OpenGL ES requires 4. OpenGL ES 3.0 class
     * hardware may only support 4 render targets. Advertise non-conformant
     * OpenGL 3.0 anyway. Affects freedreno on a3xx
     */
    const bool ver_3_0 = (ver_2_1 &&
                          consts->GLSLVersion >= 130 &&
                          consts->MaxColorAttachments >= 4 &&
                          (consts->MaxSamples >= 4 || consts->FakeSWMSAA) &&
                          (api == API_OPENGL_CORE ||
                           extensions->ARB_color_buffer_float) &&
                          extensions->ARB_depth_buffer_float &&
                          extensions->ARB_half_float_vertex &&
                          extensions->ARB_map_buffer_range &&
                          extensions->ARB_shader_texture_lod &&
                          extensions->ARB_texture_float &&
                          extensions->ARB_texture_rg &&
                          extensions->ARB_texture_compression_rgtc &&
                          extensions->EXT_draw_buffers2 &&
                          extensions->ARB_framebuffer_object &&
                          extensions->EXT_framebuffer_sRGB &&
                          extensions->EXT_packed_float &&
                          extensions->EXT_texture_array &&
                          extensions->EXT_texture_shared_exponent &&
                          extensions->EXT_transform_feedback &&
                          extensions->NV_conditional_render);
    const bool ver_3_1 = (ver_3_0 &&
                          consts->GLSLVersion >= 140 &&
                          extensions->ARB_draw_instanced &&
                          extensions->ARB_texture_buffer_object &&
                          extensions->ARB_uniform_buffer_object &&
                          extensions->EXT_texture_snorm &&
                          extensions->NV_primitive_restart &&
                          extensions->NV_texture_rectangle &&
                          consts->Program[MESA_SHADER_VERTEX].MaxTextureImageUnits >= 16);
    const bool ver_3_2 = (ver_3_1 &&
                          consts->GLSLVersion >= 150 &&
                          extensions->ARB_depth_clamp &&
                          extensions->ARB_draw_elements_base_vertex &&
                          extensions->ARB_fragment_coord_conventions &&
                          extensions->EXT_provoking_vertex &&
                          extensions->ARB_seamless_cube_map &&
                          extensions->ARB_sync &&
                          extensions->ARB_texture_multisample &&
                          extensions->EXT_vertex_array_bgra);
    const bool ver_3_3 = (ver_3_2 &&
                          consts->GLSLVersion >= 330 &&
                          extensions->ARB_blend_func_extended &&
                          extensions->ARB_explicit_attrib_location &&
                          extensions->ARB_instanced_arrays &&
                          extensions->ARB_occlusion_query2 &&
                          extensions->ARB_shader_bit_encoding &&
                          extensions->ARB_texture_rgb10_a2ui &&
                          extensions->ARB_timer_query &&
                          extensions->ARB_vertex_type_2_10_10_10_rev &&
                          extensions->EXT_texture_swizzle);
    /* ARB_sampler_objects is always enabled in mesa */

    const bool ver_4_0 = (ver_3_3 &&
                          consts->GLSLVersion >= 400 &&
                          extensions->ARB_draw_buffers_blend &&
                          extensions->ARB_draw_indirect &&
                          extensions->ARB_gpu_shader5 &&
                          extensions->ARB_gpu_shader_fp64 &&
                          extensions->ARB_sample_shading &&
                          extensions->ARB_tessellation_shader &&
                          extensions->ARB_texture_buffer_object_rgb32 &&
                          extensions->ARB_texture_cube_map_array &&
                          extensions->ARB_texture_query_lod &&
                          extensions->ARB_transform_feedback2 &&
                          extensions->ARB_transform_feedback3);
    const bool ver_4_1 = (ver_4_0 &&
                          consts->GLSLVersion >= 410 &&
                          consts->MaxTextureSize >= 16384 &&
                          consts->MaxRenderbufferSize >= 16384 &&
                          extensions->ARB_ES2_compatibility &&
                          extensions->ARB_shader_precision &&
                          extensions->ARB_vertex_attrib_64bit &&
                          extensions->ARB_viewport_array);
    const bool ver_4_2 = (ver_4_1 &&
                          consts->GLSLVersion >= 420 &&
                          extensions->ARB_base_instance &&
                          extensions->ARB_conservative_depth &&
                          extensions->ARB_internalformat_query &&
                          extensions->ARB_shader_atomic_counters &&
                          extensions->ARB_shader_image_load_store &&
                          extensions->ARB_shading_language_420pack &&
                          extensions->ARB_shading_language_packing &&
                          extensions->ARB_texture_compression_bptc &&
                          extensions->ARB_transform_feedback_instanced);
    const bool ver_4_3 = (ver_4_2 &&
                          consts->GLSLVersion >= 430 &&
                          consts->Program[MESA_SHADER_VERTEX].MaxUniformBlocks >= 14 &&
                          extensions->ARB_ES3_compatibility &&
                          extensions->ARB_arrays_of_arrays &&
                          extensions->ARB_compute_shader &&
                          extensions->ARB_copy_image &&
                          extensions->ARB_explicit_uniform_location &&
                          extensions->ARB_fragment_layer_viewport &&
                          extensions->ARB_framebuffer_no_attachments &&
                          extensions->ARB_internalformat_query2 &&
                          extensions->ARB_robust_buffer_access_behavior &&
                          extensions->ARB_shader_image_size &&
                          extensions->ARB_shader_storage_buffer_object &&
                          extensions->ARB_stencil_texturing &&
                          extensions->ARB_texture_buffer_range &&
                          extensions->ARB_texture_query_levels &&
                          extensions->ARB_texture_view);
    const bool ver_4_4 = (ver_4_3 &&
                          consts->GLSLVersion >= 440 &&
                          consts->MaxVertexAttribStride >= 2048 &&
                          extensions->ARB_buffer_storage &&
                          extensions->ARB_clear_texture &&
                          extensions->ARB_enhanced_layouts &&
                          extensions->ARB_query_buffer_object &&
                          extensions->ARB_texture_mirror_clamp_to_edge &&
                          extensions->ARB_texture_stencil8 &&
                          extensions->ARB_vertex_type_10f_11f_11f_rev);
    const bool ver_4_5 = (ver_4_4 &&
                          consts->GLSLVersion >= 450 &&
                          extensions->ARB_ES3_1_compatibility &&
                          extensions->ARB_clip_control &&
                          extensions->ARB_conditional_render_inverted &&
                          extensions->ARB_cull_distance &&
                          extensions->ARB_derivative_control &&
                          extensions->ARB_shader_texture_image_samples &&
                          extensions->NV_texture_barrier);
    const bool ver_4_6 = (ver_4_5 &&
                          consts->GLSLVersion >= 460 &&
                          extensions->ARB_gl_spirv &&
                          extensions->ARB_spirv_extensions &&
                          extensions->ARB_indirect_parameters &&
                          extensions->ARB_pipeline_statistics_query &&
                          extensions->ARB_polygon_offset_clamp &&
                          extensions->ARB_shader_atomic_counter_ops &&
                          extensions->ARB_shader_draw_parameters &&
                          extensions->ARB_shader_group_vote &&
                          extensions->ARB_texture_filter_anisotropic &&
                          extensions->ARB_transform_feedback_overflow_query);

    if (ver_4_6) {
        major = 4;
        minor = 6;
    }
    else if (ver_4_5) {
        major = 4;
        minor = 5;
    }
    else if (ver_4_4) {
        major = 4;
        minor = 4;
    }
    else if (ver_4_3) {
        major = 4;
        minor = 3;
    }
    else if (ver_4_2) {
        major = 4;
        minor = 2;
    }
    else if (ver_4_1) {
        major = 4;
        minor = 1;
    }
    else if (ver_4_0) {
        major = 4;
        minor = 0;
    }
    else if (ver_3_3) {
        major = 3;
        minor = 3;
    }
    else if (ver_3_2) {
        major = 3;
        minor = 2;
    }
    else if (ver_3_1) {
        major = 3;
        minor = 1;
    }
    else if (ver_3_0) {
        major = 3;
        minor = 0;
    }
    else if (ver_2_1) {
        major = 2;
        minor = 1;
    }
    else if (ver_2_0) {
        major = 2;
        minor = 0;
    }
    else if (ver_1_5) {
        major = 1;
        minor = 5;
    }
    else if (ver_1_4) {
        major = 1;
        minor = 4;
    }
    else {
        major = 1;
        minor = 3;
    }

    version = major * 10 + minor;

    if (api == API_OPENGL_CORE && version < 31)
        return 0;

    return version;
}

static GLuint
compute_version_es2(const struct gl_extensions *extensions,
                    const struct gl_constants *consts)
{
    /* OpenGL ES 2.0 is derived from OpenGL 2.0 */
    const bool ver_2_0 = (extensions->ARB_vertex_shader &&
                          extensions->ARB_fragment_shader &&
                          extensions->ARB_texture_non_power_of_two &&
                          extensions->EXT_blend_equation_separate);
    /* FINISHME: This list isn't quite right. */
    const bool ver_3_0 = (extensions->ARB_half_float_vertex &&
                          extensions->ARB_internalformat_query &&
                          extensions->ARB_map_buffer_range &&
                          extensions->ARB_shader_texture_lod &&
                          extensions->OES_texture_float &&
                          extensions->OES_texture_half_float &&
                          extensions->OES_texture_half_float_linear &&
                          extensions->ARB_texture_rg &&
                          extensions->ARB_depth_buffer_float &&
                          extensions->ARB_framebuffer_object &&
                          extensions->EXT_sRGB &&
                          extensions->EXT_packed_float &&
                          extensions->EXT_texture_array &&
                          extensions->EXT_texture_shared_exponent &&
                          extensions->EXT_texture_sRGB &&
                          extensions->EXT_transform_feedback &&
                          extensions->ARB_draw_instanced &&
                          extensions->ARB_uniform_buffer_object &&
                          extensions->EXT_texture_snorm &&
                          (extensions->NV_primitive_restart ||
                           consts->PrimitiveRestartFixedIndex) &&
                          extensions->OES_depth_texture_cube_map &&
                          extensions->EXT_texture_type_2_10_10_10_REV &&
                          consts->MaxColorAttachments >= 4);
    const bool es31_compute_shader =
            consts->MaxComputeWorkGroupInvocations >= 128 &&
            consts->Program[MESA_SHADER_COMPUTE].MaxShaderStorageBlocks &&
            consts->Program[MESA_SHADER_COMPUTE].MaxAtomicBuffers &&
            consts->Program[MESA_SHADER_COMPUTE].MaxImageUniforms;
    const bool ver_3_1 = (ver_3_0 &&
                          consts->MaxVertexAttribStride >= 2048 &&
                          extensions->ARB_arrays_of_arrays &&
                          es31_compute_shader &&
                          extensions->ARB_draw_indirect &&
                          extensions->ARB_explicit_uniform_location &&
                          extensions->ARB_framebuffer_no_attachments &&
                          extensions->ARB_shading_language_packing &&
                          extensions->ARB_stencil_texturing &&
                          extensions->ARB_texture_multisample &&
                          extensions->ARB_texture_gather &&
                          extensions->MESA_shader_integer_functions &&
                          extensions->EXT_shader_integer_mix);
    const bool ver_3_2 = (ver_3_1 &&
                          /* ES 3.2 requires that images/buffers be accessible
                           * from fragment shaders as well
                           */
                          extensions->ARB_shader_atomic_counters &&
                          extensions->ARB_shader_image_load_store &&
                          extensions->ARB_shader_image_size &&
                          extensions->ARB_shader_storage_buffer_object &&

                          extensions->EXT_draw_buffers2 &&
                          extensions->KHR_blend_equation_advanced &&
                          extensions->KHR_robustness &&
                          extensions->KHR_texture_compression_astc_ldr &&
                          extensions->OES_copy_image &&
                          extensions->ARB_draw_buffers_blend &&
                          extensions->ARB_draw_elements_base_vertex &&
                          extensions->OES_geometry_shader &&
                          extensions->OES_primitive_bounding_box &&
                          extensions->OES_sample_variables &&
                          extensions->ARB_tessellation_shader &&
                          extensions->OES_texture_buffer &&
                          extensions->OES_texture_cube_map_array &&
                          extensions->ARB_texture_stencil8);

    if (ver_3_2) {
        return 32;
    } else if (ver_3_1) {
        return 31;
    } else if (ver_3_0) {
        return 30;
    } else if (ver_2_0) {
        return 20;
    } else {
        return 0;
    }
}

static GLuint
compute_version_es1(const struct gl_extensions *extensions)
{
    return 11;
}

GLuint
_mesa_get_version(const struct gl_extensions *extensions,
                  struct gl_constants *consts, gl_api api)
{
    switch (api) {
        case API_OPENGL_COMPAT:
            /* Disable higher GLSL versions for legacy contexts.
             * This disallows creation of higher compatibility contexts. */
            if (!consts->AllowHigherCompatVersion) {
                consts->GLSLVersion = consts->GLSLVersionCompat;
            }
            FALLTHROUGH;
        case API_OPENGL_CORE:
            return compute_version(extensions, consts, api);
        case API_OPENGLES:
            return compute_version_es1(extensions);
        case API_OPENGLES2:
            return compute_version_es2(extensions, consts);
    }
    return 0;
}

void
_mesa_override_glsl_version(struct gl_constants *consts)
{
    const char *env_var = "MESA_GLSL_VERSION_OVERRIDE";
    const char *version;
    int n;

    version = getenv(env_var);
    if (!version) {
        return;
    }

    n = sscanf(version, "%u", &consts->GLSLVersion);
    if (n != 1) {
        fprintf(stderr, "error: invalid value for %s: %s\n", env_var, version);
        return;
    }
}