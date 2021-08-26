#ifndef TGSI_SANITY_H
#define TGSI_SANITY_H

#include "../../include/pipe/p_compiler.h"

#if defined __cplusplus
extern "C" {
#endif

struct tgsi_token;

/* Check the given token stream for errors and common mistakes.
 * Diagnostic messages are printed out to the debug output, and is
 * controlled by the debug option TGSI_PRINT_SANITY (default false).
 * Returns TRUE if there are no errors, even though there could be some warnings.
 */
boolean
tgsi_sanity_check(
        const struct tgsi_token *tokens );

#if defined __cplusplus
}
#endif

#endif /* TGSI_SANITY_H */
