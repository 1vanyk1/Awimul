#ifndef CPUINFO_H
#define CPUINFO_H


#if defined USE_X86_ASM || defined USE_X86_64_ASM
#include "x86/common_x86_asm.h"
#endif


extern void
_mesa_get_cpu_features(void);


extern char *
_mesa_get_cpu_string(void);


#endif /* CPUINFO_H */
