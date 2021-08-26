#ifndef PROG_EXECUTE_H
#define PROG_EXECUTE_H

#include "../main/config.h"
#include "../main/glheader.h"
#include "../compiler/shader_enums.h"

struct gl_context;

typedef void (*FetchTexelLodFunc)(struct gl_context *ctx, const GLfloat texcoord[4],
                                  GLfloat lambda, GLuint unit, GLfloat color[4]);

typedef void (*FetchTexelDerivFunc)(struct gl_context *ctx, const GLfloat texcoord[4],
                                    const GLfloat texdx[4],
                                    const GLfloat texdy[4],
                                    GLfloat lodBias,
                                    GLuint unit, GLfloat color[4]);


/** NOTE: This must match SWRAST_MAX_WIDTH */
#define PROG_MAX_WIDTH 16384


/**
 * Virtual machine state used during execution of vertex/fragment programs.
 */
struct gl_program_machine
{
    const struct gl_program *CurProgram;

    /** Fragment Input attributes */
    GLfloat (*Attribs)[PROG_MAX_WIDTH][4];
    GLfloat (*DerivX)[4];
    GLfloat (*DerivY)[4];
    GLuint NumDeriv; /**< Max index into DerivX/Y arrays */
    GLuint CurElement; /**< Index into Attribs arrays */

    /** Vertex Input attribs */
    GLfloat VertAttribs[VERT_ATTRIB_MAX][4];

    GLfloat Temporaries[MAX_PROGRAM_TEMPS][4];
    GLfloat Outputs[MAX_PROGRAM_OUTPUTS][4];
    GLfloat (*EnvParams)[4]; /**< Vertex or Fragment env parameters */
    GLint AddressReg[MAX_PROGRAM_ADDRESS_REGS][4];
    GLfloat SystemValues[SYSTEM_VALUE_MAX][4];

    const GLubyte *Samplers;  /** Array mapping sampler var to tex unit */

    GLuint CallStack[MAX_PROGRAM_CALL_DEPTH]; /**< For CAL/RET instructions */
    GLuint StackDepth; /**< Index/ptr to top of CallStack[] */

    /** Texture fetch functions */
    FetchTexelLodFunc FetchTexelLod;
    FetchTexelDerivFunc FetchTexelDeriv;
};


extern GLboolean
_mesa_execute_program(struct gl_context *ctx,
                      const struct gl_program *program,
                      struct gl_program_machine *machine);


#endif /* PROG_EXECUTE_H */
