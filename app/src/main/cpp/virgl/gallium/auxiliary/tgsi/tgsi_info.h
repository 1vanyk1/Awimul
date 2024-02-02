#ifndef TGSI_INFO_H
#define TGSI_INFO_H

#include "../../../mesa/pipe/p_compiler.h"
#include "../../include/pipe/p_shader_tokens.h"
#include "../util/u_format.h"

#if defined __cplusplus
extern "C" {
#endif

/* This enum describes how an opcode calculates its result. */
enum tgsi_output_mode {
    /** The opcode produces no result. */
    TGSI_OUTPUT_NONE            = 0,

    /** When this opcode writes to a channel of the destination register,
     *  it takes as arguments values from the same channel of the source
     *  register(s).
     *
     *  Example: TGSI_OPCODE_ADD
     */
    TGSI_OUTPUT_COMPONENTWISE   = 1,

    /** This opcode writes the same value to all enabled channels of the
     * destination register.
     *
     *  Example: TGSI_OPCODE_RSQ
     */
    TGSI_OUTPUT_REPLICATE       = 2,

    /** The operation performed by this opcode is dependent on which channel
     *  of the destination register is being written.
     *
     *  Example: TGSI_OPCODE_LOG
     */
    TGSI_OUTPUT_CHAN_DEPENDENT  = 3,

    /**
     * Example: TGSI_OPCODE_TEX
     */
    TGSI_OUTPUT_OTHER           = 4
};

struct tgsi_opcode_info
{
    unsigned num_dst:3;
    unsigned num_src:3;
    unsigned is_tex:1;
    unsigned is_branch:1;
    int pre_dedent:2;
    int post_indent:2;
    enum tgsi_output_mode output_mode:3;
    const char *mnemonic;
    uint opcode;
};

const struct tgsi_opcode_info *
tgsi_get_opcode_info( uint opcode );

const char *
tgsi_get_opcode_name( uint opcode );

const char *
tgsi_get_processor_name( uint processor );

enum tgsi_opcode_type {
    TGSI_TYPE_UNTYPED, /* for MOV */
    TGSI_TYPE_VOID,
    TGSI_TYPE_UNSIGNED,
    TGSI_TYPE_SIGNED,
    TGSI_TYPE_FLOAT,
    TGSI_TYPE_DOUBLE,
    TGSI_TYPE_UNSIGNED64,
    TGSI_TYPE_SIGNED64
};

enum tgsi_opcode_type
tgsi_opcode_infer_src_type( uint opcode );

enum tgsi_opcode_type
tgsi_opcode_infer_dst_type( uint opcode );

#if defined __cplusplus
}
#endif

#endif /* TGSI_INFO_H */
