#ifndef _M_TRANSLATE_H_
#define _M_TRANSLATE_H_

#include "../main/glheader.h"

/**
 * Array translation.
 * For example, convert array of GLushort[3] to GLfloat[4].
 * The function name specifies the destination format/size.
 * \param  to  the destination address
 * \param  ptr  the source address
 * \param  stride  the source stride (in bytes) between elements
 * \param  type  the source datatype (GL_SHORT, GL_UNSIGNED_INT, etc)
 * \param  size  number of values per element in source array (1,2,3 or 4)
 * \param  start  first element in source array to convert
 * \param  n  number of elements to convert
 *
 * Note: "element" means a tuple like GLfloat[3] or GLubyte[4].
 */


extern void _math_trans_1f(GLfloat *to,
                           const void *ptr,
                           GLuint stride,
                           GLenum type,
                           GLuint start,
                           GLuint n );

extern void _math_trans_1ui(GLuint *to,
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint start,
                            GLuint n );

extern void _math_trans_1ub(GLubyte *to,
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint start,
                            GLuint n );

extern void _math_trans_4ub(GLubyte (*to)[4],
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint size,
                            GLuint start,
                            GLuint n );

extern void _math_trans_4us(GLushort (*to)[4],
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint size,
                            GLuint start,
                            GLuint n );

/** Convert to floats w/out normalization (i.e. just cast) */
extern void _math_trans_4f(GLfloat (*to)[4],
                           const void *ptr,
                           GLuint stride,
                           GLenum type,
                           GLuint size,
                           GLuint start,
                           GLuint n );

/** Convert to normalized floats in [0,1] or [-1, 1] */
extern void _math_trans_4fn(GLfloat (*to)[4],
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint size,
                            GLuint start,
                            GLuint n );

extern void _math_trans_3fn(GLfloat (*to)[3],
                            const void *ptr,
                            GLuint stride,
                            GLenum type,
                            GLuint start,
                            GLuint n );

extern void _math_init_translate( void );


#endif
