#include "sha1/sha1.h"
#include "mesa-sha1.h"

void
_mesa_sha1_compute(const void *data, size_t size, unsigned char result[20])
{
    struct mesa_sha1 ctx;

    _mesa_sha1_init(&ctx);
    _mesa_sha1_update(&ctx, data, size);
    _mesa_sha1_final(&ctx, result);
}

void
_mesa_sha1_format(char *buf, const unsigned char *sha1)
{
    static const char hex_digits[] = "0123456789abcdef";
    int i;

    for (i = 0; i < 40; i += 2) {
        buf[i] = hex_digits[sha1[i >> 1] >> 4];
        buf[i + 1] = hex_digits[sha1[i >> 1] & 0x0f];
    }
    buf[i] = '\0';
}

/* Convert a hashs string hexidecimal representation into its more compact
 * form.
 */
void
_mesa_sha1_hex_to_sha1(unsigned char *buf, const char *hex)
{
    for (unsigned i = 0; i < 20; i++) {
        char tmp[3];
        tmp[0] = hex[i * 2];
        tmp[1] = hex[(i * 2) + 1];
        tmp[2] = '\0';
        buf[i] = strtol(tmp, NULL, 16);
    }
}
