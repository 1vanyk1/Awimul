#ifndef TGSI_ITERATE_H
#define TGSI_ITERATE_H

#include "../../include/pipe/p_shader_tokens.h"
#include "tgsi_parse.h"

#if defined __cplusplus
extern "C" {
#endif

struct tgsi_iterate_context
{
    boolean
    (* prolog)(
            struct tgsi_iterate_context *ctx );

    boolean
    (* iterate_instruction)(
            struct tgsi_iterate_context *ctx,
            struct tgsi_full_instruction *inst );

    boolean
    (* iterate_declaration)(
            struct tgsi_iterate_context *ctx,
            struct tgsi_full_declaration *decl );

    boolean
    (* iterate_immediate)(
            struct tgsi_iterate_context *ctx,
            struct tgsi_full_immediate *imm );

    boolean
    (* iterate_property)(
            struct tgsi_iterate_context *ctx,
            struct tgsi_full_property *prop );

    boolean
    (* epilog)(
            struct tgsi_iterate_context *ctx );

    struct tgsi_processor processor;
};

boolean
tgsi_iterate_shader(
        const struct tgsi_token *tokens,
        struct tgsi_iterate_context *ctx );

#if defined __cplusplus
}
#endif

#endif /* TGSI_ITERATE_H */
