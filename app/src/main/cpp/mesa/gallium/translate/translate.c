#include "../../include/pipe/p_config.h"
#include "../../include/pipe/p_state.h"
#include "translate.h"

struct translate *translate_create( const struct translate_key *key )
{
    struct translate *translate = NULL;

#if defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)
    translate = translate_sse2_create( key );
   if (translate)
      return translate;
#else
    (void)translate;
#endif

    return translate_generic_create( key );
}

boolean translate_is_output_format_supported(enum pipe_format format)
{
    return translate_generic_is_output_format_supported(format);
}
