#ifndef _ENUMS_H_
#define _ENUMS_H_


#ifdef __cplusplus
extern "C" {
#endif


extern const char *_mesa_enum_to_string( int nr );

/* Get the name of an enum given that it is a primitive type.  Avoids
 * GL_FALSE/GL_POINTS ambiguity and others.
 */
const char *_mesa_lookup_prim_by_nr( unsigned nr );


#ifdef __cplusplus
}
#endif


#endif
