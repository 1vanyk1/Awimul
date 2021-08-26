#ifndef CRC32_H_
#define CRC32_H_

#include <stdlib.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


uint32_t
util_hash_crc32(const void *data, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* CRC32_H_ */
