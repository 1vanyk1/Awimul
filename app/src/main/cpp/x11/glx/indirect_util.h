#ifndef __GLX_INDIRECT_UTIL_H__
#define __GLX_INDIRECT_UTIL_H__

extern GLint __glGetBooleanv_variable_size(GLenum e);

extern void *__glXGetAnswerBuffer(__GLXclientState * cl,
                                  size_t required_size, void *local_buffer,
                                  size_t local_size, unsigned alignment);

extern void __glXSendReply(ClientPtr client, const void *data,
                           size_t elements, size_t element_size,
                           GLboolean always_array, CARD32 retval);

extern void __glXSendReplySwap(ClientPtr client, const void *data,
                               size_t elements, size_t element_size,
                               GLboolean always_array, CARD32 retval);

struct __glXDispatchInfo;

extern void *__glXGetProtocolDecodeFunction(const struct __glXDispatchInfo
                                            *dispatch_info, int opcode,
                                            int swapped_version);

extern int __glXGetProtocolSizeData(const struct __glXDispatchInfo
                                    *dispatch_info, int opcode,
                                    __GLXrenderSizeData * data);

#endif                          /* __GLX_INDIRECT_UTIL_H__ */
