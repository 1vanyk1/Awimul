#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include "glheader.h"

extern void
_mesa_init_performance_monitors(struct gl_context *ctx);

extern void
_mesa_free_performance_monitors(struct gl_context *ctx);

extern void GLAPIENTRY
_mesa_GetPerfMonitorGroupsAMD(GLint *numGroups, GLsizei groupsSize,
                              GLuint *groups);

extern void GLAPIENTRY
_mesa_GetPerfMonitorCountersAMD(GLuint group, GLint *numCounters,
                                GLint *maxActiveCounters,
                                GLsizei countersSize, GLuint *counters);

extern void GLAPIENTRY
_mesa_GetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize,
                                   GLsizei *length, GLchar *groupString);

extern void GLAPIENTRY
_mesa_GetPerfMonitorCounterStringAMD(GLuint group, GLuint counter,
                                     GLsizei bufSize, GLsizei *length,
                                     GLchar *counterString);

extern void GLAPIENTRY
_mesa_GetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname,
                                   GLvoid *data);

extern void GLAPIENTRY
_mesa_GenPerfMonitorsAMD(GLsizei n, GLuint *monitors);

extern void GLAPIENTRY
_mesa_DeletePerfMonitorsAMD(GLsizei n, GLuint *monitors);

extern void GLAPIENTRY
_mesa_SelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable,
                                   GLuint group, GLint numCounters,
                                   GLuint *counterList);

extern void GLAPIENTRY
_mesa_BeginPerfMonitorAMD(GLuint monitor);

extern void GLAPIENTRY
_mesa_EndPerfMonitorAMD(GLuint monitor);

extern void GLAPIENTRY
_mesa_GetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname,
                                   GLsizei dataSize, GLuint *data,
                                   GLint *bytesWritten);

unsigned
_mesa_perf_monitor_counter_size(const struct gl_perf_monitor_counter *);


#endif
