#ifndef _RBUG_PROTO_H_
#define _RBUG_PROTO_H_

#include "../../include/pipe/p_compiler.h"

/**
 * Uniqe indentifier for each command.
 *
 * Replys are designated by negative.
 */
enum rbug_opcode
{
    RBUG_OP_NOOP = 0,
    RBUG_OP_PING = 1,
    RBUG_OP_ERROR = 2,
    RBUG_OP_PING_REPLY = -1,
    RBUG_OP_ERROR_REPLY = -2,
    RBUG_OP_TEXTURE_LIST = 256,
    RBUG_OP_TEXTURE_INFO = 257,
    RBUG_OP_TEXTURE_WRITE = 258,
    RBUG_OP_TEXTURE_READ = 259,
    RBUG_OP_TEXTURE_LIST_REPLY = -256,
    RBUG_OP_TEXTURE_INFO_REPLY = -257,
    RBUG_OP_TEXTURE_READ_REPLY = -259,
    RBUG_OP_CONTEXT_LIST = 512,
    RBUG_OP_CONTEXT_INFO = 513,
    RBUG_OP_CONTEXT_DRAW_BLOCK = 514,
    RBUG_OP_CONTEXT_DRAW_STEP = 515,
    RBUG_OP_CONTEXT_DRAW_UNBLOCK = 516,
    RBUG_OP_CONTEXT_DRAW_RULE = 518,
    RBUG_OP_CONTEXT_FLUSH = 519,
    RBUG_OP_CONTEXT_LIST_REPLY = -512,
    RBUG_OP_CONTEXT_INFO_REPLY = -513,
    RBUG_OP_CONTEXT_DRAW_BLOCKED = 517,
    RBUG_OP_SHADER_LIST = 768,
    RBUG_OP_SHADER_INFO = 769,
    RBUG_OP_SHADER_DISABLE = 770,
    RBUG_OP_SHADER_REPLACE = 771,
    RBUG_OP_SHADER_LIST_REPLY = -768,
    RBUG_OP_SHADER_INFO_REPLY = -769
};

/**
 * Header for demarshaled message.
 */
struct rbug_header
{
    enum rbug_opcode opcode;
    void *__message;
};

/**
 * Header for a message in wire format.
 */
struct rbug_proto_header
{
    int32_t opcode;
    uint32_t length;
};

/**
 * Forward declare connection here, as this file is included by all users.
 */
struct rbug_connection;

/**
 * Get printable string for opcode.
 */
const char* rbug_proto_get_name(enum rbug_opcode opcode);

#endif
