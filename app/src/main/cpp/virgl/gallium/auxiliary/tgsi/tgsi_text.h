#ifndef TGSI_TEXT_H
#define TGSI_TEXT_H

#if defined __cplusplus
extern "C" {
#endif

#include "../../../mesa/pipe/p_compiler.h"

struct tgsi_token;

boolean
tgsi_text_translate(
        const char *text,
        struct tgsi_token *tokens,
        uint num_tokens );

#if defined __cplusplus
}
#endif

#endif /* TGSI_TEXT_H */
