#ifndef PROG_PARAMETER_LAYOUT_H
#define PROG_PARAMETER_LAYOUT_H

extern unsigned _mesa_combine_swizzles(unsigned base, unsigned applied);

struct asm_parser_state;

extern GLboolean _mesa_layout_parameters(struct asm_parser_state *state);

#endif /* PROG_PARAMETER_LAYOUT_H */
