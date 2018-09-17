#ifndef ZOAL_UTILS_HELPERS_HPP
#define ZOAL_UTILS_HELPERS_HPP

#include <stddef.h>

namespace zoal { namespace utils {
    template<class T, class U>
    U to_exponents(T value, U pos, size_t radix) {
        for (; value > 0; value /= radix, pos++) {
            auto v = value % radix;
            *pos = v;
        }
        return pos;
    }

    template<class T, class U>
    void apply(T obj, U begin, U end) {
        for (; begin != end; begin++) {
            *begin = obj(*begin);
        }
    }
}}

#endif
