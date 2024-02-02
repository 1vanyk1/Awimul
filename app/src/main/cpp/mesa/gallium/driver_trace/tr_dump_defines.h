#ifndef TR_DUMP_DEFINES_H_
#define TR_DUMP_DEFINES_H_

#include "../../include/pipe/p_compiler.h"
#include "../../util/format/u_format.h"
#include "../../util/u_dump.h"
#include "tr_dump.h"


static inline void
trace_dump_format(enum pipe_format format)
{
    if (!trace_dumping_enabled_locked())
        return;

    trace_dump_enum(util_format_name(format));
}


static inline void
trace_dump_query_type(unsigned value)
{
    if (!trace_dumping_enabled_locked())
        return;

    trace_dump_enum(util_str_query_type(value, false));
}



#endif /* TR_DUMP_DEFINES_H_ */
