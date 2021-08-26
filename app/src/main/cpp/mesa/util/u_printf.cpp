#include "u_printf.h"

size_t util_printf_next_spec_pos(const std::string &s, size_t pos)
{
    size_t next_tok, spec_pos;
    do {
        pos = s.find_first_of('%', pos);

        if (pos == std::string::npos)
            return -1;

        if (s[pos + 1] == '%') {
            pos += 2;
            continue;
        }

        next_tok = s.find_first_of('%', pos + 1);
        spec_pos = s.find_first_of("cdieEfFgGaAosuxXp", pos + 1);
        if (spec_pos != std::string::npos)
            if (spec_pos < next_tok)
                return spec_pos;

        pos++;
    } while (1);
}

size_t util_printf_next_spec_pos(const char *str, size_t pos)
{
    return util_printf_next_spec_pos(std::string(str), pos);
}
