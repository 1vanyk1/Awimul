#ifndef _SHA1_H
#define _SHA1_H

#if defined(__cplusplus)
extern "C"
{
#endif
#if 0
} /* Appleasing Emacs */
#endif

#include <stdint.h>

/* Size of SHA1 digest */

#define SHA1_DIGEST_SIZE 20

/* type to hold the SHA1 context  */

typedef struct
{   uint32_t count[2];
    uint32_t hash[5];
    uint32_t wbuf[16];
} sha1_ctx;

void sha1_begin(sha1_ctx ctx[1]);
void sha1_hash(const unsigned char data[], unsigned long len, sha1_ctx ctx[1]);
void sha1_end(unsigned char hval[], sha1_ctx ctx[1]);
void sha1(unsigned char hval[], const unsigned char data[], unsigned long len);

#if defined(__cplusplus)
}
#endif

#endif