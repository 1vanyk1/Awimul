#ifndef TGSI_GL_EMULATION_H_
#define TGSI_GL_EMULATION_H_

#include "../../include/pipe/p_shader_tokens.h"

#define TGSI_EMU_CLAMP_COLOR_OUTPUTS      (1 << 0)
#define TGSI_EMU_PASSTHROUGH_EDGEFLAG     (1 << 1)
#define TGSI_EMU_FORCE_PERSAMPLE_INTERP   (1 << 2)

const struct tgsi_token *
tgsi_emulate(const struct tgsi_token *tokens, unsigned flags);

#endif /* TGSI_GL_EMULATION_H_ */
