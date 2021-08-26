#ifndef NIR_VLA_H
#define NIR_VLA_H

#include "../../include/c99_alloca.h"


/* Declare a variable length array, with no initialization */
#define NIR_VLA(_type, _name, _length) \
   _type *_name = alloca((_length) * sizeof *_name)


/* Declare a variable length array, and initialize it with the given byte.
 *
 * _length is evaluated twice, so expressions with side-effects must be
 * avoided.
 */
#define NIR_VLA_FILL(_type, _name, _length, _byte) \
   _type *_name = memset(alloca((_length) * sizeof *_name), _byte, (_length) * sizeof *_name)


/* Declare a variable length array, and zero it.
 *
 * Just like NIR_VLA_FILL, _length is evaluated twice, so expressions with
 * side-effects must be avoided.
 */
#define NIR_VLA_ZERO(_type, _name, _length) \
   NIR_VLA_FILL(_type, _name, _length, 0)

#endif /* NIR_VLA_H */
