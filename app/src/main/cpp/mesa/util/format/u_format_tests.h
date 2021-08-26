#ifndef U_FORMAT_TESTS_H_
#define U_FORMAT_TESTS_H_


#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_format.h"


#define UTIL_FORMAT_MAX_PACKED_BYTES 32  // R64G64B64A64_FLOAT
#define UTIL_FORMAT_MAX_UNPACKED_WIDTH 12  // ASTC 12x12
#define UTIL_FORMAT_MAX_UNPACKED_HEIGHT 12


/**
 * A (packed, unpacked) color pair.
 */
struct util_format_test_case
{
    enum pipe_format format;

    /**
     * Mask of the bits that actually meaningful data. Used to mask out the
     * "X" channels.
     */
    uint8_t mask[UTIL_FORMAT_MAX_PACKED_BYTES];

    uint8_t packed[UTIL_FORMAT_MAX_PACKED_BYTES];

    /**
     * RGBA.
     */
    double unpacked[UTIL_FORMAT_MAX_UNPACKED_HEIGHT][UTIL_FORMAT_MAX_UNPACKED_WIDTH][4];
};


extern const struct util_format_test_case
        util_format_test_cases[];


extern const unsigned util_format_nr_test_cases;


#endif /* U_FORMAT_TESTS_H_ */
