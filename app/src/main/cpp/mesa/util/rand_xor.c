#include "detect_os.h"

#if !DETECT_OS_WINDOWS
#if defined(HAVE_GETRANDOM)
#include <sys/random.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#endif

#include <time.h>

#include "rand_xor.h"

/* Super fast random number generator.
 *
 * This rand_xorshift128plus function by Sebastiano Vigna belongs
 * to the public domain.
 */
uint64_t
rand_xorshift128plus(uint64_t seed[2])
{
    uint64_t *s = seed;

    uint64_t s1 = s[0];
    const uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

    return s[1] + s0;
}

void
s_rand_xorshift128plus(uint64_t seed[2], bool randomised_seed)
{
    if (!randomised_seed) {
        /* Use a fixed seed */
        seed[0] = 0x3bffb83978e24f88;
        seed[1] = 0x9238d5d56c71cd35;
        return;
    }

#if !DETECT_OS_WINDOWS
    size_t seed_size = sizeof(uint64_t) * 2;

#if defined(HAVE_GETRANDOM)
    ssize_t ret = getrandom(seed, seed_size, GRND_NONBLOCK);
   if (ret == seed_size)
      return;
#endif

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0) {
        if (read(fd, seed, seed_size) == seed_size) {
            close(fd);
            return;
        }
        close(fd);
    }
#endif

    seed[0] = 0x3bffb83978e24f88;
    seed[1] = time(NULL);
}
