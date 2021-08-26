#ifndef RAND_XOR_H
#define RAND_XOR_H

#include <stdint.h>
#include <stdbool.h>

uint64_t
rand_xorshift128plus(uint64_t seed[2]);

void
s_rand_xorshift128plus(uint64_t seed[2], bool randomised_seed);

#endif /* RAND_XOR_H */
