#ifndef ZOAL_UTILS_HELPERS_HPP
#define ZOAL_UTILS_HELPERS_HPP

#include <stddef.h>

namespace zoal { namespace utils {
    template<size_t Radix>
    class radix {
    public:
        template<class T, class U>
        static U split(T value, U pos) {
            for (; value > 0; value /= Radix, ++pos) {
                auto v = value % Radix;
                *pos = v;
            }
            return pos;
        }
    };

    template<>
    class radix<16> {
    public:
        template<class T, class U>
        static U split(T value, U pos) {
            for (; value > 0; value >>= 4, ++pos) {
                auto v = value & 0x0F;
                *pos = v;
            }
            return pos;
        }
    };

    template<>
    class radix<8> {
    public:
        template<class T, class U>
        static U split(T value, U pos) {
            for (; value > 0; value >>= 2, ++pos) {
                auto v = value & 0x03;
                *pos = v;
            }
            return pos;
        }
    };

    template<>
    class radix<2> {
    public:
        template<class T, class U>
        static U split(T value, U pos) {
            for (; value > 0; value >>= 1, ++pos) {
                auto v = value & 0x01;
                *pos = v;
            }
            return pos;
        }
    };

    template<class T, class U>
    void apply(T obj, U begin, U end) {
        for (; begin != end; ++begin) {
            *begin = obj(*begin);
        }
    }
}}

#endif
