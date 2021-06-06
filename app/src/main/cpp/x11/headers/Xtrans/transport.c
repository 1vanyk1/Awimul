#include <stdlib.h>

#define TRANS_CLIENT
#define XTRANS_TRANSPORT_C  /* used to flag Xtransint.h that it's being used
			       here, not just #included in another file */

#include "Xtransint.h"
#include "Xtrans.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef LOCALCONN
#include "Xtranslcl.c"
#endif
#if defined(TCPCONN) || defined(UNIXCONN)
#include "Xtranssock.c"
#endif
//#include "Xtrans.c"
//#include "Xtransutil.c"

#ifdef __clang__
#pragma clang diagnostic pop
#endif
