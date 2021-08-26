#ifndef TGSI_DUMP_H
#define TGSI_DUMP_H

#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_defines.h"
#include "../../include/pipe/p_shader_tokens.h"

#include <stdio.h>

#if defined __cplusplus
extern "C" {
#endif

#define TGSI_DUMP_FLOAT_AS_HEX (1 << 0)

bool
tgsi_dump_str(
        const struct tgsi_token *tokens,
        uint flags,
        char *str,
        size_t size);

void
tgsi_dump_to_file(const struct tgsi_token *tokens, uint flags, FILE *file);

void
tgsi_dump(
        const struct tgsi_token *tokens,
        uint flags );

struct tgsi_full_immediate;
struct tgsi_full_instruction;
struct tgsi_full_declaration;
struct tgsi_full_property;

void
tgsi_dump_immediate(
        const struct tgsi_full_immediate *imm );

void
tgsi_dump_instruction_str(
        const struct tgsi_full_instruction *inst,
        uint instno,
        char *str,
        size_t size);

void
tgsi_dump_instruction(
        const struct tgsi_full_instruction *inst,
        uint instno );

void
tgsi_dump_declaration(
        const struct tgsi_full_declaration *decl );

void
tgsi_dump_property(
        const struct tgsi_full_property *prop );

#if defined __cplusplus
}
#endif

#endif /* TGSI_DUMP_H */
