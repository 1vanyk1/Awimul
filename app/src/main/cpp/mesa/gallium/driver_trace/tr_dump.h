#ifndef TR_DUMP_H
#define TR_DUMP_H


#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_format.h"

struct pipe_resource;
struct pipe_surface;
struct pipe_transfer;
struct pipe_box;

/*
 * Low level dumping controls.
 *
 * Opening the trace file and checking if that is opened.
 */
bool trace_dump_trace_begin(void);
bool trace_dump_trace_enabled(void);
void trace_dump_trace_flush(void);

/*
 * Lock and unlock the call mutex.
 *
 * It used by the none locked version of dumping control
 * and begin/end call dump functions.
 *
 * Begin takes the lock while end unlocks it. Use the _locked
 * version to avoid locking/unlocking it.
 */
void trace_dump_call_lock(void);
void trace_dump_call_unlock(void);

/*
 * High level dumping control.
 */
void trace_dumping_start_locked(void);
void trace_dumping_stop_locked(void);
bool trace_dumping_enabled_locked(void);
void trace_dumping_start(void);
void trace_dumping_stop(void);
bool trace_dumping_enabled(void);

void trace_dump_call_begin_locked(const char *klass, const char *method);
void trace_dump_call_end_locked(void);
void trace_dump_call_begin(const char *klass, const char *method);
void trace_dump_call_end(void);

void trace_dump_arg_begin(const char *name);
void trace_dump_arg_end(void);
void trace_dump_ret_begin(void);
void trace_dump_ret_end(void);
void trace_dump_bool(int value);
void trace_dump_int(long long int value);
void trace_dump_uint(long long unsigned value);
void trace_dump_float(double value);
void trace_dump_bytes(const void *data, size_t size);
void trace_dump_box_bytes(const void *data,
                          struct pipe_resource *resource,
                          const struct pipe_box *box,
                          unsigned stride,
                          unsigned slice_stride);
void trace_dump_string(const char *str);
void trace_dump_enum(const char *value);
void trace_dump_array_begin(void);
void trace_dump_array_end(void);
void trace_dump_elem_begin(void);
void trace_dump_elem_end(void);
void trace_dump_struct_begin(const char *name);
void trace_dump_struct_end(void);
void trace_dump_member_begin(const char *name);
void trace_dump_member_end(void);
void trace_dump_null(void);
void trace_dump_ptr(const void *value);
/* will turn a wrapped object into the real one and dump ptr */
void trace_dump_surface_ptr(struct pipe_surface *_surface);
void trace_dump_transfer_ptr(struct pipe_transfer *_transfer);

void trace_dump_trigger_active(bool active);
void trace_dump_check_trigger(void);
bool trace_dump_is_triggered(void);

/*
 * Code saving macros.
 */

#define trace_dump_arg(_type, _arg) \
   do { \
      trace_dump_arg_begin(#_arg); \
      trace_dump_##_type(_arg); \
      trace_dump_arg_end(); \
   } while(0)

#define trace_dump_arg_struct(_type, _arg) \
   do { \
      trace_dump_arg_begin(#_arg); \
      trace_dump_##_type(&_arg); \
      trace_dump_arg_end(); \
   } while(0)

#define trace_dump_ret(_type, _arg) \
   do { \
      trace_dump_ret_begin(); \
      trace_dump_##_type(_arg); \
      trace_dump_ret_end(); \
   } while(0)

#define trace_dump_array(_type, _obj, _size) \
   do { \
      if (_obj) { \
         size_t idx; \
         trace_dump_array_begin(); \
         for(idx = 0; idx < (_size); ++idx) { \
            trace_dump_elem_begin(); \
            trace_dump_##_type((_obj)[idx]); \
            trace_dump_elem_end(); \
         } \
         trace_dump_array_end(); \
      } else { \
         trace_dump_null(); \
      } \
   } while(0)

#define trace_dump_struct_array(_type, _obj, _size) \
   do { \
      if (_obj) { \
         size_t idx; \
         trace_dump_array_begin(); \
         for(idx = 0; idx < (_size); ++idx) { \
            trace_dump_elem_begin(); \
            trace_dump_##_type(&(_obj)[idx]); \
            trace_dump_elem_end(); \
         } \
         trace_dump_array_end(); \
      } else { \
         trace_dump_null(); \
      } \
   } while(0)

#define trace_dump_member(_type, _obj, _member) \
   do { \
      trace_dump_member_begin(#_member); \
      trace_dump_##_type((_obj)->_member); \
      trace_dump_member_end(); \
   } while(0)

#define trace_dump_arg_array(_type, _arg, _size) \
   do { \
      trace_dump_arg_begin(#_arg); \
      trace_dump_array(_type, _arg, _size); \
      trace_dump_arg_end(); \
   } while(0)

#define trace_dump_member_array(_type, _obj, _member) \
   do { \
      trace_dump_member_begin(#_member); \
      trace_dump_array(_type, (_obj)->_member, sizeof((_obj)->_member)/sizeof((_obj)->_member[0])); \
      trace_dump_member_end(); \
   } while(0)


#endif /* TR_DUMP_H */
