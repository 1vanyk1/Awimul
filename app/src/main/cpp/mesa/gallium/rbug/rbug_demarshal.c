#include "rbug.h"

/**
 * Small function that looks at the proto_header and selects the correct
 * demarshal functions and return the result.
 */
struct rbug_header * rbug_demarshal(struct rbug_proto_header *header)
{
    switch(header->opcode) {
        case RBUG_OP_NOOP:
            return (struct rbug_header *)rbug_demarshal_noop(header);
        case RBUG_OP_PING:
            return (struct rbug_header *)rbug_demarshal_ping(header);
        case RBUG_OP_ERROR:
            return (struct rbug_header *)rbug_demarshal_error(header);
        case RBUG_OP_PING_REPLY:
            return (struct rbug_header *)rbug_demarshal_ping_reply(header);
        case RBUG_OP_ERROR_REPLY:
            return (struct rbug_header *)rbug_demarshal_error_reply(header);
        case RBUG_OP_TEXTURE_LIST:
            return (struct rbug_header *)rbug_demarshal_texture_list(header);
        case RBUG_OP_TEXTURE_INFO:
            return (struct rbug_header *)rbug_demarshal_texture_info(header);
        case RBUG_OP_TEXTURE_WRITE:
            return (struct rbug_header *)rbug_demarshal_texture_write(header);
        case RBUG_OP_TEXTURE_READ:
            return (struct rbug_header *)rbug_demarshal_texture_read(header);
        case RBUG_OP_TEXTURE_LIST_REPLY:
            return (struct rbug_header *)rbug_demarshal_texture_list_reply(header);
        case RBUG_OP_TEXTURE_INFO_REPLY:
            return (struct rbug_header *)rbug_demarshal_texture_info_reply(header);
        case RBUG_OP_TEXTURE_READ_REPLY:
            return (struct rbug_header *)rbug_demarshal_texture_read_reply(header);
        case RBUG_OP_CONTEXT_LIST:
            return (struct rbug_header *)rbug_demarshal_context_list(header);
        case RBUG_OP_CONTEXT_INFO:
            return (struct rbug_header *)rbug_demarshal_context_info(header);
        case RBUG_OP_CONTEXT_DRAW_BLOCK:
            return (struct rbug_header *)rbug_demarshal_context_draw_block(header);
        case RBUG_OP_CONTEXT_DRAW_STEP:
            return (struct rbug_header *)rbug_demarshal_context_draw_step(header);
        case RBUG_OP_CONTEXT_DRAW_UNBLOCK:
            return (struct rbug_header *)rbug_demarshal_context_draw_unblock(header);
        case RBUG_OP_CONTEXT_DRAW_RULE:
            return (struct rbug_header *)rbug_demarshal_context_draw_rule(header);
        case RBUG_OP_CONTEXT_FLUSH:
            return (struct rbug_header *)rbug_demarshal_context_flush(header);
        case RBUG_OP_CONTEXT_LIST_REPLY:
            return (struct rbug_header *)rbug_demarshal_context_list_reply(header);
        case RBUG_OP_CONTEXT_INFO_REPLY:
            return (struct rbug_header *)rbug_demarshal_context_info_reply(header);
        case RBUG_OP_CONTEXT_DRAW_BLOCKED:
            return (struct rbug_header *)rbug_demarshal_context_draw_blocked(header);
        case RBUG_OP_SHADER_LIST:
            return (struct rbug_header *)rbug_demarshal_shader_list(header);
        case RBUG_OP_SHADER_INFO:
            return (struct rbug_header *)rbug_demarshal_shader_info(header);
        case RBUG_OP_SHADER_DISABLE:
            return (struct rbug_header *)rbug_demarshal_shader_disable(header);
        case RBUG_OP_SHADER_REPLACE:
            return (struct rbug_header *)rbug_demarshal_shader_replace(header);
        case RBUG_OP_SHADER_LIST_REPLY:
            return (struct rbug_header *)rbug_demarshal_shader_list_reply(header);
        case RBUG_OP_SHADER_INFO_REPLY:
            return (struct rbug_header *)rbug_demarshal_shader_info_reply(header);
        default:
            return NULL;
    }
}

const char* rbug_proto_get_name(enum rbug_opcode opcode)
{
    switch(opcode) {
        case RBUG_OP_NOOP:
            return "RBUG_OP_NOOP";
        case RBUG_OP_PING:
            return "RBUG_OP_PING";
        case RBUG_OP_ERROR:
            return "RBUG_OP_ERROR";
        case RBUG_OP_PING_REPLY:
            return "RBUG_OP_PING_REPLY";
        case RBUG_OP_ERROR_REPLY:
            return "RBUG_OP_ERROR_REPLY";
        case RBUG_OP_TEXTURE_LIST:
            return "RBUG_OP_TEXTURE_LIST";
        case RBUG_OP_TEXTURE_INFO:
            return "RBUG_OP_TEXTURE_INFO";
        case RBUG_OP_TEXTURE_WRITE:
            return "RBUG_OP_TEXTURE_WRITE";
        case RBUG_OP_TEXTURE_READ:
            return "RBUG_OP_TEXTURE_READ";
        case RBUG_OP_TEXTURE_LIST_REPLY:
            return "RBUG_OP_TEXTURE_LIST_REPLY";
        case RBUG_OP_TEXTURE_INFO_REPLY:
            return "RBUG_OP_TEXTURE_INFO_REPLY";
        case RBUG_OP_TEXTURE_READ_REPLY:
            return "RBUG_OP_TEXTURE_READ_REPLY";
        case RBUG_OP_CONTEXT_LIST:
            return "RBUG_OP_CONTEXT_LIST";
        case RBUG_OP_CONTEXT_INFO:
            return "RBUG_OP_CONTEXT_INFO";
        case RBUG_OP_CONTEXT_DRAW_BLOCK:
            return "RBUG_OP_CONTEXT_DRAW_BLOCK";
        case RBUG_OP_CONTEXT_DRAW_STEP:
            return "RBUG_OP_CONTEXT_DRAW_STEP";
        case RBUG_OP_CONTEXT_DRAW_UNBLOCK:
            return "RBUG_OP_CONTEXT_DRAW_UNBLOCK";
        case RBUG_OP_CONTEXT_DRAW_RULE:
            return "RBUG_OP_CONTEXT_DRAW_RULE";
        case RBUG_OP_CONTEXT_FLUSH:
            return "RBUG_OP_CONTEXT_FLUSH";
        case RBUG_OP_CONTEXT_LIST_REPLY:
            return "RBUG_OP_CONTEXT_LIST_REPLY";
        case RBUG_OP_CONTEXT_INFO_REPLY:
            return "RBUG_OP_CONTEXT_INFO_REPLY";
        case RBUG_OP_CONTEXT_DRAW_BLOCKED:
            return "RBUG_OP_CONTEXT_DRAW_BLOCKED";
        case RBUG_OP_SHADER_LIST:
            return "RBUG_OP_SHADER_LIST";
        case RBUG_OP_SHADER_INFO:
            return "RBUG_OP_SHADER_INFO";
        case RBUG_OP_SHADER_DISABLE:
            return "RBUG_OP_SHADER_DISABLE";
        case RBUG_OP_SHADER_REPLACE:
            return "RBUG_OP_SHADER_REPLACE";
        case RBUG_OP_SHADER_LIST_REPLY:
            return "RBUG_OP_SHADER_LIST_REPLY";
        case RBUG_OP_SHADER_INFO_REPLY:
            return "RBUG_OP_SHADER_INFO_REPLY";
        default:
            return NULL;
    }
}
