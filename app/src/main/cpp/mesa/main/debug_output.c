#include <stdarg.h>
#include <stdio.h>
#include "context.h"
#include "debug_output.h"
#include "enums.h"

#include "hash.h"
#include "mtypes.h"
#include "version.h"
#include "../util/hash_table.h"
#include "../util/list.h"
#include "../util/u_memory.h"


static GLuint PrevDynamicID = 0;


/**
 * A namespace element.
 */
struct gl_debug_element
{
    struct list_head link;

    GLuint ID;
    /* at which severity levels (mesa_debug_severity) is the message enabled */
    GLbitfield State;
};


struct gl_debug_namespace
{
    struct list_head Elements;
    GLbitfield DefaultState;
};


struct gl_debug_group {
    struct gl_debug_namespace Namespaces[MESA_DEBUG_SOURCE_COUNT][MESA_DEBUG_TYPE_COUNT];
};


/**
 * An error, warning, or other piece of debug information for an application
 * to consume via GL_ARB_debug_output/GL_KHR_debug.
 */
struct gl_debug_message
{
    enum mesa_debug_source source;
    enum mesa_debug_type type;
    GLuint id;
    enum mesa_debug_severity severity;
    /* length as given by the user - if message was explicitly null terminated,
     * length can be negative */
    GLsizei length;
    GLcharARB *message;
};


/**
 * Debug message log.  It works like a ring buffer.
 */
struct gl_debug_log {
    struct gl_debug_message Messages[MAX_DEBUG_LOGGED_MESSAGES];
    GLint NextMessage;
    GLint NumMessages;
};


struct gl_debug_state
{
    GLDEBUGPROC Callback;
    const void *CallbackData;
    GLboolean SyncOutput;
    GLboolean DebugOutput;
    GLboolean LogToStderr;

    struct gl_debug_group *Groups[MAX_DEBUG_GROUP_STACK_DEPTH];
    struct gl_debug_message GroupMessages[MAX_DEBUG_GROUP_STACK_DEPTH];
    GLint CurrentGroup; // GroupStackDepth - 1

    struct gl_debug_log Log;
};


static char out_of_memory[] = "Debugging error: out of memory";

static const GLenum debug_source_enums[] = {
        GL_DEBUG_SOURCE_API,
        GL_DEBUG_SOURCE_WINDOW_SYSTEM,
        GL_DEBUG_SOURCE_SHADER_COMPILER,
        GL_DEBUG_SOURCE_THIRD_PARTY,
        GL_DEBUG_SOURCE_APPLICATION,
        GL_DEBUG_SOURCE_OTHER,
};

static const GLenum debug_type_enums[] = {
        GL_DEBUG_TYPE_ERROR,
        GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
        GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
        GL_DEBUG_TYPE_PORTABILITY,
        GL_DEBUG_TYPE_PERFORMANCE,
        GL_DEBUG_TYPE_OTHER,
        GL_DEBUG_TYPE_MARKER,
        GL_DEBUG_TYPE_PUSH_GROUP,
        GL_DEBUG_TYPE_POP_GROUP,
};

static const GLenum debug_severity_enums[] = {
        GL_DEBUG_SEVERITY_LOW,
        GL_DEBUG_SEVERITY_MEDIUM,
        GL_DEBUG_SEVERITY_HIGH,
        GL_DEBUG_SEVERITY_NOTIFICATION,
};


static enum mesa_debug_source
gl_enum_to_debug_source(GLenum e)
{
    unsigned i;

    for (i = 0; i < ARRAY_SIZE(debug_source_enums); i++) {
        if (debug_source_enums[i] == e)
            break;
    }
    return i;
}

static enum mesa_debug_type
gl_enum_to_debug_type(GLenum e)
{
    unsigned i;

    for (i = 0; i < ARRAY_SIZE(debug_type_enums); i++) {
        if (debug_type_enums[i] == e)
            break;
    }
    return i;
}

static enum mesa_debug_severity
gl_enum_to_debug_severity(GLenum e)
{
    unsigned i;

    for (i = 0; i < ARRAY_SIZE(debug_severity_enums); i++) {
        if (debug_severity_enums[i] == e)
            break;
    }
    return i;
}


/**
 * Handles generating a GL_ARB_debug_output message ID generated by the GL or
 * GLSL compiler.
 *
 * The GL API has this "ID" mechanism, where the intention is to allow a
 * client to filter in/out messages based on source, type, and ID.  Of course,
 * building a giant enum list of all debug output messages that Mesa might
 * generate is ridiculous, so instead we have our caller pass us a pointer to
 * static storage where the ID should get stored.  This ID will be shared
 * across all contexts for that message (which seems like a desirable
 * property, even if it's not expected by the spec), but note that it won't be
 * the same between executions if messages aren't generated in the same order.
 */
void
_mesa_debug_get_id(GLuint *id)
{
    if (!(*id)) {
        *id = p_atomic_inc_return(&PrevDynamicID);
    }
}

static void
debug_message_clear(struct gl_debug_message *msg)
{
    if (msg->message != (char*)out_of_memory)
        free(msg->message);
    msg->message = NULL;
    msg->length = 0;
}

static void
debug_message_store(struct gl_debug_message *msg,
                    enum mesa_debug_source source,
                    enum mesa_debug_type type, GLuint id,
                    enum mesa_debug_severity severity,
                    GLsizei len, const char *buf)
{
    GLsizei length = len;

    assert(!msg->message && !msg->length);

    if (length < 0)
        length = strlen(buf);

    msg->message = malloc(length+1);
    if (msg->message) {
        (void) strncpy(msg->message, buf, (size_t)length);
        msg->message[length] = '\0';

        msg->length = len;
        msg->source = source;
        msg->type = type;
        msg->id = id;
        msg->severity = severity;
    } else {
        static GLuint oom_msg_id = 0;
        _mesa_debug_get_id(&oom_msg_id);

        /* malloc failed! */
        msg->message = out_of_memory;
        msg->length = -1;
        msg->source = MESA_DEBUG_SOURCE_OTHER;
        msg->type = MESA_DEBUG_TYPE_ERROR;
        msg->id = oom_msg_id;
        msg->severity = MESA_DEBUG_SEVERITY_HIGH;
    }
}

static void
debug_namespace_init(struct gl_debug_namespace *ns)
{
    list_inithead(&ns->Elements);

    /* Enable all the messages with severity HIGH or MEDIUM by default */
    ns->DefaultState = (1 << MESA_DEBUG_SEVERITY_MEDIUM ) |
                       (1 << MESA_DEBUG_SEVERITY_HIGH) |
                       (1 << MESA_DEBUG_SEVERITY_NOTIFICATION);
}

static void
debug_namespace_clear(struct gl_debug_namespace *ns)
{
    list_for_each_entry_safe(struct gl_debug_element, elem, &ns->Elements, link)
    free(elem);
}

static bool
debug_namespace_copy(struct gl_debug_namespace *dst,
                     const struct gl_debug_namespace *src)
{
    dst->DefaultState = src->DefaultState;

    list_inithead(&dst->Elements);
    list_for_each_entry(struct gl_debug_element, elem, &src->Elements, link) {
        struct gl_debug_element *copy;

        copy = malloc(sizeof(*copy));
        if (!copy) {
            debug_namespace_clear(dst);
            return false;
        }

        copy->ID = elem->ID;
        copy->State = elem->State;
        list_addtail(&copy->link, &dst->Elements);
    }

    return true;
}

/**
 * Set the state of \p id in the namespace.
 */
static bool
debug_namespace_set(struct gl_debug_namespace *ns,
                    GLuint id, bool enabled)
{
    const uint32_t state = (enabled) ?
                           ((1 << MESA_DEBUG_SEVERITY_COUNT) - 1) : 0;
    struct gl_debug_element *elem = NULL;

    /* find the element */
    list_for_each_entry(struct gl_debug_element, tmp, &ns->Elements, link) {
        if (tmp->ID == id) {
            elem = tmp;
            break;
        }
    }

    /* we do not need the element if it has the default state */
    if (ns->DefaultState == state) {
        if (elem) {
            list_del(&elem->link);
            free(elem);
        }
        return true;
    }

    if (!elem) {
        elem = malloc(sizeof(*elem));
        if (!elem)
            return false;

        elem->ID = id;
        list_addtail(&elem->link, &ns->Elements);
    }

    elem->State = state;

    return true;
}

/**
 * Set the default state of the namespace for \p severity.  When \p severity
 * is MESA_DEBUG_SEVERITY_COUNT, the default values for all severities are
 * updated.
 */
static void
debug_namespace_set_all(struct gl_debug_namespace *ns,
                        enum mesa_debug_severity severity,
                        bool enabled)
{
    uint32_t mask, val;

    /* set all elements to the same state */
    if (severity == MESA_DEBUG_SEVERITY_COUNT) {
        ns->DefaultState = (enabled) ? ((1 << severity) - 1) : 0;
        debug_namespace_clear(ns);
        list_inithead(&ns->Elements);
        return;
    }

    mask = 1 << severity;
    val = (enabled) ? mask : 0;

    ns->DefaultState = (ns->DefaultState & ~mask) | val;

    list_for_each_entry_safe(struct gl_debug_element, elem, &ns->Elements,
            link) {
        elem->State = (elem->State & ~mask) | val;
        if (elem->State == ns->DefaultState) {
            list_del(&elem->link);
            free(elem);
        }
    }
}

/**
 * Get the state of \p id in the namespace.
 */
static bool
debug_namespace_get(const struct gl_debug_namespace *ns, GLuint id,
                    enum mesa_debug_severity severity)
{
    uint32_t state;

    state = ns->DefaultState;
    list_for_each_entry(struct gl_debug_element, elem, &ns->Elements, link) {
        if (elem->ID == id) {
            state = elem->State;
            break;
        }
    }

    return (state & (1 << severity));
}

/**
 * Allocate and initialize context debug state.
 */
static struct gl_debug_state *
debug_create(void)
{
    struct gl_debug_state *debug;
    int s, t;

    debug = CALLOC_STRUCT(gl_debug_state);
    if (!debug)
        return NULL;

    debug->Groups[0] = malloc(sizeof(*debug->Groups[0]));
    if (!debug->Groups[0]) {
        free(debug);
        return NULL;
    }

    /* Initialize state for filtering known debug messages. */
    for (s = 0; s < MESA_DEBUG_SOURCE_COUNT; s++) {
        for (t = 0; t < MESA_DEBUG_TYPE_COUNT; t++)
            debug_namespace_init(&debug->Groups[0]->Namespaces[s][t]);
    }

    return debug;
}

/**
 * Return true if the top debug group points to the group below it.
 */
static bool
debug_is_group_read_only(const struct gl_debug_state *debug)
{
    const GLint gstack = debug->CurrentGroup;
    return (gstack > 0 && debug->Groups[gstack] == debug->Groups[gstack - 1]);
}

/**
 * Make the top debug group writable.
 */
static bool
debug_make_group_writable(struct gl_debug_state *debug)
{
    const GLint gstack = debug->CurrentGroup;
    const struct gl_debug_group *src = debug->Groups[gstack];
    struct gl_debug_group *dst;
    int s, t;

    if (!debug_is_group_read_only(debug))
        return true;

    dst = malloc(sizeof(*dst));
    if (!dst)
        return false;

    for (s = 0; s < MESA_DEBUG_SOURCE_COUNT; s++) {
        for (t = 0; t < MESA_DEBUG_TYPE_COUNT; t++) {
            if (!debug_namespace_copy(&dst->Namespaces[s][t],
                                      &src->Namespaces[s][t])) {
                /* error path! */
                for (t = t - 1; t >= 0; t--)
                    debug_namespace_clear(&dst->Namespaces[s][t]);
                for (s = s - 1; s >= 0; s--) {
                    for (t = 0; t < MESA_DEBUG_TYPE_COUNT; t++)
                        debug_namespace_clear(&dst->Namespaces[s][t]);
                }
                free(dst);
                return false;
            }
        }
    }

    debug->Groups[gstack] = dst;

    return true;
}

/**
 * Free the top debug group.
 */
static void
debug_clear_group(struct gl_debug_state *debug)
{
    const GLint gstack = debug->CurrentGroup;

    if (!debug_is_group_read_only(debug)) {
        struct gl_debug_group *grp = debug->Groups[gstack];
        int s, t;

        for (s = 0; s < MESA_DEBUG_SOURCE_COUNT; s++) {
            for (t = 0; t < MESA_DEBUG_TYPE_COUNT; t++)
                debug_namespace_clear(&grp->Namespaces[s][t]);
        }

        free(grp);
    }

    debug->Groups[gstack] = NULL;
}

/**
 * Delete the oldest debug messages out of the log.
 */
static void
debug_delete_messages(struct gl_debug_state *debug, int count)
{
    struct gl_debug_log *log = &debug->Log;

    if (count > log->NumMessages)
        count = log->NumMessages;

    while (count--) {
        struct gl_debug_message *msg = &log->Messages[log->NextMessage];

        debug_message_clear(msg);

        log->NumMessages--;
        log->NextMessage++;
        log->NextMessage %= MAX_DEBUG_LOGGED_MESSAGES;
    }
}

/**
 * Loop through debug group stack tearing down states for
 * filtering debug messages.  Then free debug output state.
 */
static void
debug_destroy(struct gl_debug_state *debug)
{
    while (debug->CurrentGroup > 0) {
        debug_clear_group(debug);
        debug->CurrentGroup--;
    }

    debug_clear_group(debug);
    debug_delete_messages(debug, debug->Log.NumMessages);
    free(debug);
}

/**
 * Sets the state of the given message source/type/ID tuple.
 */
static void
debug_set_message_enable(struct gl_debug_state *debug,
                         enum mesa_debug_source source,
                         enum mesa_debug_type type,
                         GLuint id, GLboolean enabled)
{
    const GLint gstack = debug->CurrentGroup;
    struct gl_debug_namespace *ns;

    debug_make_group_writable(debug);
    ns = &debug->Groups[gstack]->Namespaces[source][type];

    debug_namespace_set(ns, id, enabled);
}

/*
 * Set the state of all message IDs found in the given intersection of
 * 'source', 'type', and 'severity'.  The _COUNT enum can be used for
 * GL_DONT_CARE (include all messages in the class).
 *
 * This requires both setting the state of all previously seen message
 * IDs in the hash table, and setting the default state for all
 * applicable combinations of source/type/severity, so that all the
 * yet-unknown message IDs that may be used in the future will be
 * impacted as if they were already known.
 */
static void
debug_set_message_enable_all(struct gl_debug_state *debug,
                             enum mesa_debug_source source,
                             enum mesa_debug_type type,
                             enum mesa_debug_severity severity,
                             GLboolean enabled)
{
    const GLint gstack = debug->CurrentGroup;
    int s, t, smax, tmax;

    if (source == MESA_DEBUG_SOURCE_COUNT) {
        source = 0;
        smax = MESA_DEBUG_SOURCE_COUNT;
    } else {
        smax = source+1;
    }

    if (type == MESA_DEBUG_TYPE_COUNT) {
        type = 0;
        tmax = MESA_DEBUG_TYPE_COUNT;
    } else {
        tmax = type+1;
    }

    debug_make_group_writable(debug);

    for (s = source; s < smax; s++) {
        for (t = type; t < tmax; t++) {
            struct gl_debug_namespace *nspace =
                    &debug->Groups[gstack]->Namespaces[s][t];
            debug_namespace_set_all(nspace, severity, enabled);
        }
    }
}

/**
 * Returns if the given message source/type/ID tuple is enabled.
 */
bool
_mesa_debug_is_message_enabled(const struct gl_debug_state *debug,
                               enum mesa_debug_source source,
                               enum mesa_debug_type type,
                               GLuint id,
                               enum mesa_debug_severity severity)
{
    const GLint gstack = debug->CurrentGroup;
    struct gl_debug_group *grp = debug->Groups[gstack];
    struct gl_debug_namespace *nspace = &grp->Namespaces[source][type];

    if (!debug->DebugOutput)
        return false;

    return debug_namespace_get(nspace, id, severity);
}

/**
 * 'buf' is not necessarily a null-terminated string. When logging, copy
 * 'len' characters from it, store them in a new, null-terminated string,
 * and remember the number of bytes used by that string, *including*
 * the null terminator this time.
 */
static void
debug_log_message(struct gl_debug_state *debug,
                  enum mesa_debug_source source,
                  enum mesa_debug_type type, GLuint id,
                  enum mesa_debug_severity severity,
                  GLsizei len, const char *buf)
{
    struct gl_debug_log *log = &debug->Log;
    GLint nextEmpty;
    struct gl_debug_message *emptySlot;

    if (debug->LogToStderr) {
        _mesa_log("Mesa debug output: %.*s\n", len, buf);
    }

    assert(len < MAX_DEBUG_MESSAGE_LENGTH);

    if (log->NumMessages == MAX_DEBUG_LOGGED_MESSAGES)
        return;

    nextEmpty = (log->NextMessage + log->NumMessages)
                % MAX_DEBUG_LOGGED_MESSAGES;
    emptySlot = &log->Messages[nextEmpty];

    debug_message_store(emptySlot, source, type,
                        id, severity, len, buf);

    log->NumMessages++;
}

/**
 * Return the oldest debug message out of the log.
 */
static const struct gl_debug_message *
debug_fetch_message(const struct gl_debug_state *debug)
{
    const struct gl_debug_log *log = &debug->Log;

    return (log->NumMessages) ? &log->Messages[log->NextMessage] : NULL;
}

static struct gl_debug_message *
debug_get_group_message(struct gl_debug_state *debug)
{
    return &debug->GroupMessages[debug->CurrentGroup];
}

static void
debug_push_group(struct gl_debug_state *debug)
{
    const GLint gstack = debug->CurrentGroup;

    /* just point to the previous stack */
    debug->Groups[gstack + 1] = debug->Groups[gstack];
    debug->CurrentGroup++;
}

static void
debug_pop_group(struct gl_debug_state *debug)
{
    debug_clear_group(debug);
    debug->CurrentGroup--;
}


/**
 * Lock and return debug state for the context.  The debug state will be
 * allocated and initialized upon the first call.  When NULL is returned, the
 * debug state is not locked.
 */
static struct gl_debug_state *
_mesa_lock_debug_state(struct gl_context *ctx)
{
    simple_mtx_lock(&ctx->DebugMutex);

    if (!ctx->Debug) {
        ctx->Debug = debug_create();
        if (!ctx->Debug) {
            GET_CURRENT_CONTEXT(cur);
            simple_mtx_unlock(&ctx->DebugMutex);

            /*
             * This function may be called from other threads.  When that is the
             * case, we cannot record this OOM error.
             */
            if (ctx == cur)
                _mesa_error(ctx, GL_OUT_OF_MEMORY, "allocating debug state");

            return NULL;
        }
    }

    return ctx->Debug;
}

static void
_mesa_unlock_debug_state(struct gl_context *ctx)
{
    simple_mtx_unlock(&ctx->DebugMutex);
}

/**
 * Set the integer debug state specified by \p pname.  This can be called from
 * _mesa_set_enable for example.
 */
bool
_mesa_set_debug_state_int(struct gl_context *ctx, GLenum pname, GLint val)
{
    struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);

    if (!debug)
        return false;

    switch (pname) {
        case GL_DEBUG_OUTPUT:
            debug->DebugOutput = (val != 0);
            break;
        case GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB:
            debug->SyncOutput = (val != 0);
            break;
        default:
            assert(!"unknown debug output param");
            break;
    }

    _mesa_unlock_debug_state(ctx);

    return true;
}

/**
 * Query the integer debug state specified by \p pname.  This can be called
 * _mesa_GetIntegerv for example.
 */
GLint
_mesa_get_debug_state_int(struct gl_context *ctx, GLenum pname)
{
    GLint val;

    struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);
    if (!debug)
        return 0;

    switch (pname) {
        case GL_DEBUG_OUTPUT:
            val = debug->DebugOutput;
            break;
        case GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB:
            val = debug->SyncOutput;
            break;
        case GL_DEBUG_LOGGED_MESSAGES:
            val = debug->Log.NumMessages;
            break;
        case GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH:
            val = (debug->Log.NumMessages) ?
                  debug->Log.Messages[debug->Log.NextMessage].length + 1 : 0;
            break;
        case GL_DEBUG_GROUP_STACK_DEPTH:
            val = debug->CurrentGroup + 1;
            break;
        default:
            assert(!"unknown debug output param");
            val = 0;
            break;
    }

    _mesa_unlock_debug_state(ctx);

    return val;
}

/**
 * Query the pointer debug state specified by \p pname.  This can be called
 * _mesa_GetPointerv for example.
 */
void *
_mesa_get_debug_state_ptr(struct gl_context *ctx, GLenum pname)
{
    void *val;
    struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);

    if (!debug)
        return NULL;

    switch (pname) {
        case GL_DEBUG_CALLBACK_FUNCTION_ARB:
            val = (void *) debug->Callback;
            break;
        case GL_DEBUG_CALLBACK_USER_PARAM_ARB:
            val = (void *) debug->CallbackData;
            break;
        default:
            assert(!"unknown debug output param");
            val = NULL;
            break;
    }

    _mesa_unlock_debug_state(ctx);

    return val;
}

/**
 * Insert a debug message.  The mutex is assumed to be locked, and will be
 * unlocked by this call.
 */
static void
log_msg_locked_and_unlock(struct gl_context *ctx,
                          enum mesa_debug_source source,
                          enum mesa_debug_type type, GLuint id,
                          enum mesa_debug_severity severity,
                          GLint len, const char *buf)
{
    struct gl_debug_state *debug = ctx->Debug;

    if (!_mesa_debug_is_message_enabled(debug, source, type, id, severity)) {
        _mesa_unlock_debug_state(ctx);
        return;
    }

    if (ctx->Debug->Callback) {
        /* Call the user's callback function */
        GLenum gl_source = debug_source_enums[source];
        GLenum gl_type = debug_type_enums[type];
        GLenum gl_severity = debug_severity_enums[severity];
        GLDEBUGPROC callback = ctx->Debug->Callback;
        const void *data = ctx->Debug->CallbackData;

        /*
         * When ctx->Debug->SyncOutput is GL_FALSE, the client is prepared for
         * unsynchronous calls.  When it is GL_TRUE, we will not spawn threads.
         * In either case, we can call the callback unlocked.
         */
        _mesa_unlock_debug_state(ctx);
        callback(gl_source, gl_type, id, gl_severity, len, buf, data);
    }
    else {
        /* add debug message to queue */
        debug_log_message(ctx->Debug, source, type, id, severity, len, buf);
        _mesa_unlock_debug_state(ctx);
    }
}

/**
 * Log a client or driver debug message.
 */
void
_mesa_log_msg(struct gl_context *ctx, enum mesa_debug_source source,
              enum mesa_debug_type type, GLuint id,
              enum mesa_debug_severity severity, GLint len, const char *buf)
{
    struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);

    if (!debug)
        return;

    log_msg_locked_and_unlock(ctx, source, type, id, severity, len, buf);
}


/**
 * Verify that source, type, and severity are valid enums.
 *
 * The 'caller' param is used for handling values available
 * only in glDebugMessageInsert or glDebugMessageControl
 */
static GLboolean
validate_params(struct gl_context *ctx, unsigned caller,
                const char *callerstr, GLenum source, GLenum type,
                GLenum severity)
{
#define INSERT 1
#define CONTROL 2
    switch(source) {
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            break;
        case GL_DEBUG_SOURCE_API_ARB:
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        case GL_DEBUG_SOURCE_OTHER_ARB:
            if (caller != INSERT)
                break;
            else
                goto error;
        case GL_DONT_CARE:
            if (caller == CONTROL)
                break;
            else
                goto error;
        default:
            goto error;
    }

    switch(type) {
        case GL_DEBUG_TYPE_ERROR_ARB:
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
        case GL_DEBUG_TYPE_OTHER_ARB:
        case GL_DEBUG_TYPE_MARKER:
        case GL_DEBUG_TYPE_PUSH_GROUP:
        case GL_DEBUG_TYPE_POP_GROUP:
            break;
        case GL_DONT_CARE:
            if (caller == CONTROL)
                break;
            else
                goto error;
        default:
            goto error;
    }

    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        case GL_DEBUG_SEVERITY_LOW_ARB:
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            break;
        case GL_DONT_CARE:
            if (caller == CONTROL)
                break;
            else
                goto error;
        default:
            goto error;
    }
    return GL_TRUE;

    error:
    _mesa_error(ctx, GL_INVALID_ENUM, "bad values passed to %s"
                                      "(source=0x%x, type=0x%x, severity=0x%x)", callerstr,
                source, type, severity);

    return GL_FALSE;
}


static GLboolean
validate_length(struct gl_context *ctx, const char *callerstr, GLsizei length,
                const GLchar *buf)
{

    if (length < 0) {
        GLsizei len = strlen(buf);

        if (len >= MAX_DEBUG_MESSAGE_LENGTH) {
            _mesa_error(ctx, GL_INVALID_VALUE,
                        "%s(null terminated string length=%d, is not less than "
                        "GL_MAX_DEBUG_MESSAGE_LENGTH=%d)", callerstr, len,
                        MAX_DEBUG_MESSAGE_LENGTH);
            return GL_FALSE;
        }
    }

    if (length >= MAX_DEBUG_MESSAGE_LENGTH) {
        _mesa_error(ctx, GL_INVALID_VALUE,
                    "%s(length=%d, which is not less than "
                    "GL_MAX_DEBUG_MESSAGE_LENGTH=%d)", callerstr, length,
                    MAX_DEBUG_MESSAGE_LENGTH);
        return GL_FALSE;
    }

    return GL_TRUE;
}


void GLAPIENTRY
_mesa_DebugMessageInsert(GLenum source, GLenum type, GLuint id,
                         GLenum severity, GLint length,
                         const GLchar *buf)
{
    GET_CURRENT_CONTEXT(ctx);
    const char *callerstr;

    if (_mesa_is_desktop_gl(ctx))
        callerstr = "glDebugMessageInsert";
    else
        callerstr = "glDebugMessageInsertKHR";

    if (!validate_params(ctx, INSERT, callerstr, source, type, severity))
        return; /* GL_INVALID_ENUM */

    if (!validate_length(ctx, callerstr, length, buf))
        return; /* GL_INVALID_VALUE */

    /* if length not specified, string will be null terminated: */
    if (length < 0)
        length = strlen(buf);

    _mesa_log_msg(ctx, gl_enum_to_debug_source(source),
                  gl_enum_to_debug_type(type), id,
                  gl_enum_to_debug_severity(severity),
                  length, buf);

    if (type == GL_DEBUG_TYPE_MARKER && ctx->Driver.EmitStringMarker) {
        ctx->Driver.EmitStringMarker(ctx, buf, length);
    }
}


GLuint GLAPIENTRY
_mesa_GetDebugMessageLog(GLuint count, GLsizei logSize, GLenum *sources,
                         GLenum *types, GLenum *ids, GLenum *severities,
                         GLsizei *lengths, GLchar *messageLog)
{
    GET_CURRENT_CONTEXT(ctx);
    struct gl_debug_state *debug;
    const char *callerstr;
    GLuint ret;

    if (_mesa_is_desktop_gl(ctx))
        callerstr = "glGetDebugMessageLog";
    else
        callerstr = "glGetDebugMessageLogKHR";

    if (!messageLog)
        logSize = 0;

    if (logSize < 0) {
        _mesa_error(ctx, GL_INVALID_VALUE,
                    "%s(logSize=%d : logSize must not be negative)",
                    callerstr, logSize);
        return 0;
    }

    debug = _mesa_lock_debug_state(ctx);
    if (!debug)
        return 0;

    for (ret = 0; ret < count; ret++) {
        const struct gl_debug_message *msg = debug_fetch_message(debug);
        GLsizei len;

        if (!msg)
            break;

        len = msg->length;
        if (len < 0)
            len = strlen(msg->message);

        if (logSize < len+1 && messageLog != NULL)
            break;

        if (messageLog) {
            assert(msg->message[len] == '\0');
            (void) strncpy(messageLog, msg->message, (size_t)len+1);

            messageLog += len+1;
            logSize -= len+1;
        }

        if (lengths)
            *lengths++ = len+1;
        if (severities)
            *severities++ = debug_severity_enums[msg->severity];
        if (sources)
            *sources++ = debug_source_enums[msg->source];
        if (types)
            *types++ = debug_type_enums[msg->type];
        if (ids)
            *ids++ = msg->id;

        debug_delete_messages(debug, 1);
    }

    _mesa_unlock_debug_state(ctx);

    return ret;
}


void GLAPIENTRY
_mesa_DebugMessageControl(GLenum gl_source, GLenum gl_type,
                          GLenum gl_severity, GLsizei count,
                          const GLuint *ids, GLboolean enabled)
{
    GET_CURRENT_CONTEXT(ctx);
    enum mesa_debug_source source = gl_enum_to_debug_source(gl_source);
    enum mesa_debug_type type = gl_enum_to_debug_type(gl_type);
    enum mesa_debug_severity severity = gl_enum_to_debug_severity(gl_severity);
    const char *callerstr;
    struct gl_debug_state *debug;

    if (_mesa_is_desktop_gl(ctx))
        callerstr = "glDebugMessageControl";
    else
        callerstr = "glDebugMessageControlKHR";

    if (count < 0) {
        _mesa_error(ctx, GL_INVALID_VALUE,
                    "%s(count=%d : count must not be negative)", callerstr,
                    count);
        return;
    }

    if (!validate_params(ctx, CONTROL, callerstr, gl_source, gl_type,
                         gl_severity))
        return; /* GL_INVALID_ENUM */

    if (count && (gl_severity != GL_DONT_CARE || gl_type == GL_DONT_CARE
                  || gl_source == GL_DONT_CARE)) {
        _mesa_error(ctx, GL_INVALID_OPERATION,
                    "%s(When passing an array of ids, severity must be"
                    " GL_DONT_CARE, and source and type must not be GL_DONT_CARE.",
                    callerstr);
        return;
    }

    debug = _mesa_lock_debug_state(ctx);
    if (!debug)
        return;

    if (count) {
        GLsizei i;
        for (i = 0; i < count; i++)
            debug_set_message_enable(debug, source, type, ids[i], enabled);
    }
    else {
        debug_set_message_enable_all(debug, source, type, severity, enabled);
    }

    _mesa_unlock_debug_state(ctx);
}


void GLAPIENTRY
_mesa_DebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
{
    GET_CURRENT_CONTEXT(ctx);
    struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);
    if (debug) {
        debug->Callback = callback;
        debug->CallbackData = userParam;
        _mesa_unlock_debug_state(ctx);
    }
}


void GLAPIENTRY
_mesa_PushDebugGroup(GLenum source, GLuint id, GLsizei length,
                     const GLchar *message)
{
    GET_CURRENT_CONTEXT(ctx);
    const char *callerstr;
    struct gl_debug_state *debug;
    struct gl_debug_message *emptySlot;

    if (_mesa_is_desktop_gl(ctx))
        callerstr = "glPushDebugGroup";
    else
        callerstr = "glPushDebugGroupKHR";

    switch(source) {
        case GL_DEBUG_SOURCE_APPLICATION:
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            break;
        default:
            _mesa_error(ctx, GL_INVALID_ENUM, "bad value passed to %s"
                                              "(source=0x%x)", callerstr, source);
            return;
    }

    if (!validate_length(ctx, callerstr, length, message))
        return; /* GL_INVALID_VALUE */

    if (length < 0)
        length = strlen(message);

    debug = _mesa_lock_debug_state(ctx);
    if (!debug)
        return;

    if (debug->CurrentGroup >= MAX_DEBUG_GROUP_STACK_DEPTH-1) {
        _mesa_unlock_debug_state(ctx);
        _mesa_error(ctx, GL_STACK_OVERFLOW, "%s", callerstr);
        return;
    }

    /* pop reuses the message details from push so we store this */
    emptySlot = debug_get_group_message(debug);
    debug_message_store(emptySlot,
                        gl_enum_to_debug_source(source),
                        gl_enum_to_debug_type(GL_DEBUG_TYPE_PUSH_GROUP),
                        id,
                        gl_enum_to_debug_severity(GL_DEBUG_SEVERITY_NOTIFICATION),
                        length, message);

    debug_push_group(debug);

    log_msg_locked_and_unlock(ctx,
                              gl_enum_to_debug_source(source),
                              MESA_DEBUG_TYPE_PUSH_GROUP, id,
                              MESA_DEBUG_SEVERITY_NOTIFICATION, length,
                              message);
}


void GLAPIENTRY
_mesa_PopDebugGroup(void)
{
    GET_CURRENT_CONTEXT(ctx);
    const char *callerstr;
    struct gl_debug_state *debug;
    struct gl_debug_message *gdmessage, msg;

    if (_mesa_is_desktop_gl(ctx))
        callerstr = "glPopDebugGroup";
    else
        callerstr = "glPopDebugGroupKHR";

    debug = _mesa_lock_debug_state(ctx);
    if (!debug)
        return;

    if (debug->CurrentGroup <= 0) {
        _mesa_unlock_debug_state(ctx);
        _mesa_error(ctx, GL_STACK_UNDERFLOW, "%s", callerstr);
        return;
    }

    debug_pop_group(debug);

    /* make a shallow copy */
    gdmessage = debug_get_group_message(debug);
    msg = *gdmessage;
    gdmessage->message = NULL;
    gdmessage->length = 0;

    log_msg_locked_and_unlock(ctx,
                              msg.source,
                              gl_enum_to_debug_type(GL_DEBUG_TYPE_POP_GROUP),
                              msg.id,
                              gl_enum_to_debug_severity(GL_DEBUG_SEVERITY_NOTIFICATION),
                              msg.length, msg.message);

    debug_message_clear(&msg);
}


void
_mesa_init_debug_output(struct gl_context *ctx)
{
    simple_mtx_init(&ctx->DebugMutex, mtx_plain);

    if (MESA_DEBUG_FLAGS & DEBUG_CONTEXT) {
        /* If the MESA_DEBUG env is set to "context", we'll turn on the
         * GL_CONTEXT_FLAG_DEBUG_BIT context flag and log debug output
         * messages to stderr (or whatever MESA_LOG_FILE points at).
         */
        struct gl_debug_state *debug = _mesa_lock_debug_state(ctx);
        if (!debug) {
            return;
        }
        debug->DebugOutput = GL_TRUE;
        debug->LogToStderr = GL_TRUE;
        ctx->Const.ContextFlags |= GL_CONTEXT_FLAG_DEBUG_BIT;
        _mesa_unlock_debug_state(ctx);
    }
}


void
_mesa_destroy_debug_output(struct gl_context *ctx)
{
    if (ctx->Debug) {
        debug_destroy(ctx->Debug);
        /* set to NULL just in case it is used before context is completely gone. */
        ctx->Debug = NULL;
    }

    simple_mtx_destroy(&ctx->DebugMutex);
}

void GLAPIENTRY
_mesa_StringMarkerGREMEDY(GLsizei len, const GLvoid *string)
{
    GET_CURRENT_CONTEXT(ctx);
    if (ctx->Extensions.GREMEDY_string_marker) {
        /* if length not specified, string will be null terminated: */
        if (len <= 0)
            len = strlen(string);
        ctx->Driver.EmitStringMarker(ctx, string, len);
    } else {
        _mesa_error(ctx, GL_INVALID_OPERATION, "StringMarkerGREMEDY");
    }
}
