#include "string_to_uint_map.h"

extern "C" struct string_to_uint_map *
string_to_uint_map_ctor()
{
    return new string_to_uint_map;
}

extern "C" void
string_to_uint_map_dtor(struct string_to_uint_map *map)
{
    delete map;
}
