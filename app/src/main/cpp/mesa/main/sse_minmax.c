#include "sse_minmax.h"
//#include <smmintrin.h>
#include "../../impl/smmintrin.h"
#include <stdint.h>


void
_mesa_uint_array_min_max(const unsigned *ui_indices, unsigned *min_index,
                         unsigned *max_index, const unsigned count)
{
    unsigned max_ui = 0;
    unsigned min_ui = ~0U;
    unsigned i = 0;
    unsigned aligned_count = count;

    /* handle the first few values without SSE until the pointer is aligned */
    while (((uintptr_t)ui_indices & 15) && aligned_count) {
        if (*ui_indices > max_ui)
            max_ui = *ui_indices;
        if (*ui_indices < min_ui)
            min_ui = *ui_indices;

        aligned_count--;
        ui_indices++;
    }

    /* TODO: The actual threshold for SSE begin useful may be higher than 8.
     * Some careful microbenchmarks and measurement are required to
     * find the actual tipping point.
     */
    if (aligned_count >= 8) {
        unsigned max_arr[4] __attribute__ ((aligned (16)));
        unsigned min_arr[4] __attribute__ ((aligned (16)));
        unsigned vec_count;
        __m128i max_ui4 = _mm_setzero_si128();
        __m128i min_ui4 = _mm_set1_epi32(~0U);
        __m128i ui_indices4;
        __m128i *ui_indices_ptr;

        vec_count = aligned_count & ~0x3;
        ui_indices_ptr = (__m128i *)ui_indices;
        for (i = 0; i < vec_count / 4; i++) {
            ui_indices4 = _mm_load_si128(&ui_indices_ptr[i]);
            max_ui4 = _mm_max_epu32(ui_indices4, max_ui4);
            min_ui4 = _mm_min_epu32(ui_indices4, min_ui4);
        }

        _mm_store_si128((__m128i *)max_arr, max_ui4);
        _mm_store_si128((__m128i *)min_arr, min_ui4);

        for (i = 0; i < 4; i++) {
            if (max_arr[i] > max_ui)
                max_ui = max_arr[i];
            if (min_arr[i] < min_ui)
                min_ui = min_arr[i];
        }
        i = vec_count;
    }

    for (; i < aligned_count; i++) {
        if (ui_indices[i] > max_ui)
            max_ui = ui_indices[i];
        if (ui_indices[i] < min_ui)
            min_ui = ui_indices[i];
    }

    *min_index = min_ui;
    *max_index = max_ui;
}
