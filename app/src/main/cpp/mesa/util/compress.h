#ifdef HAVE_COMPRESSION

#include <stdbool.h>
#include <inttypes.h>

size_t
util_compress_max_compressed_len(size_t in_data_size);

bool
util_compress_inflate(const uint8_t *in_data, size_t in_data_size,
                      uint8_t *out_data, size_t out_data_size);

size_t
util_compress_deflate(const uint8_t *in_data, size_t in_data_size,
                      uint8_t *out_data, size_t out_buff_size);

#endif
