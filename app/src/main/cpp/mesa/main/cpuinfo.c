#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cpuinfo.h"


/**
 * This function should be called before the various "cpu_has_foo" macros
 * are used.
 */
void
_mesa_get_cpu_features(void)
{
#if defined USE_X86_ASM || defined USE_X86_64_ASM
    _mesa_get_x86_features();
#endif
}


/**
 * Return a string describing the CPU architexture and extensions that
 * Mesa is using (such as SSE or Altivec).
 * \return information string, free it with free()
 */
char *
_mesa_get_cpu_string(void)
{
#define MAX_STRING 50
    char *buffer;

    buffer = malloc(MAX_STRING);
    if (!buffer)
        return NULL;

    buffer[0] = 0;

#ifdef USE_X86_ASM

    if (_mesa_x86_cpu_features) {
      strcat(buffer, "x86");
   }

# ifdef USE_MMX_ASM
   if (cpu_has_mmx) {
      strcat(buffer, (cpu_has_mmxext) ? "/MMX+" : "/MMX");
   }
# endif
# ifdef USE_3DNOW_ASM
   if (cpu_has_3dnow) {
      strcat(buffer, (cpu_has_3dnowext) ? "/3DNow!+" : "/3DNow!");
   }
# endif
# ifdef USE_SSE_ASM
   if (cpu_has_xmm) {
      strcat(buffer, (cpu_has_xmm2) ? "/SSE2" : "/SSE");
   }
# endif

#elif defined(USE_SPARC_ASM)

    strcat(buffer, "SPARC");

#endif

    assert(strlen(buffer) < MAX_STRING);

    return buffer;
}
