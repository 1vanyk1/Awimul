#ifndef BUILD_ID_H
#define BUILD_ID_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_DL_ITERATE_PHDR

#include <stdint.h>

struct build_id_note;

const struct build_id_note *
build_id_find_nhdr_for_addr(const void *addr);

unsigned
build_id_length(const struct build_id_note *note);

const uint8_t *
build_id_data(const struct build_id_note *note);

#endif

#ifdef __cplusplus
}
#endif
#endif /* BUILD_ID_H */
