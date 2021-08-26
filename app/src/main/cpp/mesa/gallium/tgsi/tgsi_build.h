#ifndef TGSI_BUILD_H
#define TGSI_BUILD_H

#include "tgsi_parse.h"

#if defined __cplusplus
extern "C" {
#endif


/*
 * header
 */

struct tgsi_header
tgsi_build_header( void );

struct tgsi_processor
tgsi_build_processor(
        unsigned processor,
        struct tgsi_header *header );

/*
 * declaration
 */

struct tgsi_full_declaration
tgsi_default_full_declaration( void );

unsigned
tgsi_build_full_declaration(
        const struct tgsi_full_declaration *full_decl,
        struct tgsi_token *tokens,
        struct tgsi_header *header,
        unsigned maxsize );

/*
 * immediate
 */

struct tgsi_full_immediate
tgsi_default_full_immediate( void );

unsigned
tgsi_build_full_immediate(
        const struct tgsi_full_immediate *full_imm,
        struct tgsi_token *tokens,
        struct tgsi_header *header,
        unsigned maxsize );

/*
 * properties
 */

struct tgsi_full_property
tgsi_default_full_property( void );

unsigned
tgsi_build_full_property(
        const struct tgsi_full_property *full_prop,
        struct tgsi_token *tokens,
        struct tgsi_header *header,
        unsigned maxsize );

/*
 * instruction
 */

struct tgsi_instruction
tgsi_default_instruction( void );

struct tgsi_full_instruction
tgsi_default_full_instruction( void );

unsigned
tgsi_build_full_instruction(
        const struct tgsi_full_instruction *full_inst,
        struct tgsi_token *tokens,
        struct tgsi_header *header,
        unsigned maxsize );

struct tgsi_full_src_register
tgsi_full_src_register_from_dst(const struct tgsi_full_dst_register *dst);

#if defined __cplusplus
}
#endif

#endif /* TGSI_BUILD_H */
