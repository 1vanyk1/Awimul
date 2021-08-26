#ifndef ZSTD_DDICT_H
#define ZSTD_DDICT_H

/*-*******************************************************
 *  Dependencies
 *********************************************************/
#include "../common/zstd_deps.h"   /* size_t */
#include "../zstd.h"     /* ZSTD_DDict, and several public functions */


/*-*******************************************************
 *  Interface
 *********************************************************/

/* note: several prototypes are already published in `zstd.h` :
 * ZSTD_createDDict()
 * ZSTD_createDDict_byReference()
 * ZSTD_createDDict_advanced()
 * ZSTD_freeDDict()
 * ZSTD_initStaticDDict()
 * ZSTD_sizeof_DDict()
 * ZSTD_estimateDDictSize()
 * ZSTD_getDictID_fromDict()
 */

const void* ZSTD_DDict_dictContent(const ZSTD_DDict* ddict);
size_t ZSTD_DDict_dictSize(const ZSTD_DDict* ddict);

void ZSTD_copyDDictParameters(ZSTD_DCtx* dctx, const ZSTD_DDict* ddict);



#endif /* ZSTD_DDICT_H */