#ifndef U_DEBUG_SYMBOL_H_
#define U_DEBUG_SYMBOL_H_


/**
 * @file
 * Symbol lookup.
 *
 * @author Jose Fonseca <jfonseca@vmware.com>
 */


#ifdef	__cplusplus
extern "C" {
#endif


void
debug_symbol_name(const void *addr, char* buf, unsigned size);

const char*
debug_symbol_name_cached(const void *addr);

void
debug_symbol_print(const void *addr);

#ifdef	__cplusplus
}
#endif

#endif /* U_DEBUG_SYMBOL_H_ */
