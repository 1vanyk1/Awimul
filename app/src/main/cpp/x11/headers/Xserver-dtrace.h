#ifndef	_XSERVER_DTRACE_H
#define	_XSERVER_DTRACE_H

#include <unistd.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if _DTRACE_VERSION

#define	XSERVER_CLIENT_AUTH(arg0, arg1, arg2, arg3) \
	__dtrace_Xserver___client__auth(arg0, arg1, arg2, arg3)
#define	XSERVER_CLIENT_CONNECT(arg0, arg1) \
	__dtrace_Xserver___client__connect(arg0, arg1)
#define	XSERVER_CLIENT_DISCONNECT(arg0) \
	__dtrace_Xserver___client__disconnect(arg0)
#define	XSERVER_REQUEST_DONE(arg0, arg1, arg2, arg3, arg4) \
	__dtrace_Xserver___request__done(arg0, arg1, arg2, arg3, arg4)
#define	XSERVER_REQUEST_START(arg0, arg1, arg2, arg3, arg4) \
	__dtrace_Xserver___request__start(arg0, arg1, arg2, arg3, arg4)
#define	XSERVER_RESOURCE_ALLOC(arg0, arg1, arg2, arg3) \
	__dtrace_Xserver___resource__alloc(arg0, arg1, arg2, arg3)
#define	XSERVER_RESOURCE_FREE(arg0, arg1, arg2, arg3) \
	__dtrace_Xserver___resource__free(arg0, arg1, arg2, arg3)
#define	XSERVER_SEND_EVENT(arg0, arg1, arg2) \
	__dtrace_Xserver___send__event(arg0, arg1, arg2)
#define	XSERVER_INPUT_EVENT(arg0, arg1, arg2, arg3, arg4, arg5, arg6) \
	__dtrace_Xserver___input__event(arg0, arg1, arg2, arg3, arg4, arg5, arg6)

extern void __dtrace_Xserver___client__auth(int, string, pid_t, zoneid_t);
extern void __dtrace_Xserver___client__connect(int, int);
extern void __dtrace_Xserver___client__disconnect(int);
extern void __dtrace_Xserver___request__done(string, uint8_t, uint32_t, int, int);
extern void __dtrace_Xserver___request__start(string, uint8_t, uint16_t, int, void *);
extern void __dtrace_Xserver___resource__alloc(uint32_t, uint32_t, void *, string);
extern void __dtrace_Xserver___resource__free(uint32_t, uint32_t, void *, string);
extern void __dtrace_Xserver___send__event(int, uint8_t, void *);
extern void __dtrace_Xserver___input__event(int, uint16_t, uint32_t, uint32_t, int8_t, uint8_t *, double *);


#else

#define	XSERVER_CLIENT_AUTH(arg0, arg1, arg2, arg3)
#define	XSERVER_CLIENT_CONNECT(arg0, arg1)
#define	XSERVER_CLIENT_DISCONNECT(arg0)
#define	XSERVER_REQUEST_DONE(arg0, arg1, arg2, arg3, arg4)
#define	XSERVER_REQUEST_START(arg0, arg1, arg2, arg3, arg4)
#define	XSERVER_RESOURCE_ALLOC(arg0, arg1, arg2, arg3)
#define	XSERVER_RESOURCE_FREE(arg0, arg1, arg2, arg3)
#define	XSERVER_SEND_EVENT(arg0, arg1, arg2)
#define	XSERVER_INPUT_EVENT(arg0, arg1, arg2, arg3, arg4, arg5, arg6)

#endif

#define	XSERVER_CLIENT_AUTH_ENABLED() (1)
#define	XSERVER_CLIENT_CONNECT_ENABLED() (1)
#define	XSERVER_CLIENT_DISCONNECT_ENABLED() (1)
#define	XSERVER_REQUEST_DONE_ENABLED() (1)
#define	XSERVER_REQUEST_START_ENABLED() (1)
#define	XSERVER_RESOURCE_ALLOC_ENABLED() (1)
#define	XSERVER_RESOURCE_FREE_ENABLED() (1)
#define	XSERVER_SEND_EVENT_ENABLED() (1)
#define	XSERVER_INPUT_EVENT_ENABLED() (1)

#ifdef	__cplusplus
}
#endif

#endif	/* _XSERVER_DTRACE_H */