#ifndef PERFORMANCE_QUERY_H
#define PERFORMANCE_QUERY_H

#include "glheader.h"

extern void
_mesa_init_performance_queries(struct gl_context *ctx);

extern void
_mesa_free_performance_queries(struct gl_context *ctx);

extern void GLAPIENTRY
_mesa_GetFirstPerfQueryIdINTEL(GLuint *queryId);

extern void GLAPIENTRY
_mesa_GetNextPerfQueryIdINTEL(GLuint queryId, GLuint *nextQueryId);

extern void GLAPIENTRY
_mesa_GetPerfQueryIdByNameINTEL(char *queryName, GLuint *queryId);

extern void GLAPIENTRY
_mesa_GetPerfQueryInfoINTEL(GLuint queryId,
                            GLuint queryNameLength, char *queryName,
                            GLuint *dataSize, GLuint *noCounters,
                            GLuint *noActiveInstances,
                            GLuint *capsMask);

extern void GLAPIENTRY
_mesa_GetPerfCounterInfoINTEL(GLuint queryId, GLuint counterId,
                              GLuint counterNameLength, char *counterName,
                              GLuint counterDescLength, char *counterDesc,
                              GLuint *counterOffset, GLuint *counterDataSize, GLuint *counterTypeEnum,
                              GLuint *counterDataTypeEnum, GLuint64 *rawCounterMaxValue);

extern void GLAPIENTRY
_mesa_CreatePerfQueryINTEL(GLuint queryId, GLuint *queryHandle);

extern void GLAPIENTRY
_mesa_DeletePerfQueryINTEL(GLuint queryHandle);

extern void GLAPIENTRY
_mesa_BeginPerfQueryINTEL(GLuint queryHandle);

extern void GLAPIENTRY
_mesa_EndPerfQueryINTEL(GLuint queryHandle);

extern void GLAPIENTRY
_mesa_GetPerfQueryDataINTEL(GLuint queryHandle, GLuint flags,
                            GLsizei dataSize, void *data, GLuint *bytesWritten);

#endif
