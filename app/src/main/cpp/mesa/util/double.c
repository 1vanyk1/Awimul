#include "softfloat.h"
#include "double.h"

float
_mesa_double_to_float(double val)
{
    return _mesa_double_to_f32(val, false);
}

float
_mesa_double_to_float_rtz(double val)
{
    return _mesa_double_to_f32(val, true);
}
