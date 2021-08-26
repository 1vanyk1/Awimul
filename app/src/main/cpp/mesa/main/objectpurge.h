#ifndef OBJECTPURGE_H
#define OBJECTPURGE_H


GLenum GLAPIENTRY
_mesa_ObjectPurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);

GLenum GLAPIENTRY
_mesa_ObjectUnpurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);

void GLAPIENTRY
_mesa_GetObjectParameterivAPPLE(GLenum objectType, GLuint name,
        GLenum pname, GLint* params);


#endif /* OBJECTPURGE_H */
