#ifndef ZOAL_STATIC_ASSERTION_HPP
#define ZOAL_STATIC_ASSERTION_HPP

#include <stddef.h>

namespace zoal { namespace utils {
    template<class T, class U>
    struct same_type {
        static constexpr bool value = false;
    };

    template<class T>
    struct same_type<T, T> {
        static constexpr bool value = true;
    };

    template<class T, class U>
    struct optional_type {
        typedef T type;
    };

    template<class U>
    struct optional_type<void, U> {
        typedef U type;
    };

    template<class First, class ... Rest>
    class pin_count {
    public:
        typedef pin_count<Rest...> Other;
        static constexpr auto value = First::not_pin ? 0 : 1 + Other::value;

    };

    template<class First>
    class pin_count<First> {
    public:
        static constexpr auto value = First::not_pin ? 0 : 1;
    };

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
