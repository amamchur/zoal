#ifndef ZOAL_STATIC_ASSERTION_HPP
#define ZOAL_STATIC_ASSERTION_HPP

#include <stddef.h>

namespace zoal { namespace utils {
    template<class T, T v>
    struct integral_constant {
        using value_type = T;
        using type = integral_constant<T, v>;

        static constexpr T value = v;

        constexpr operator T() { return v; }
    };

    struct false_type : integral_constant<bool, false> {
    };

    struct true_type : integral_constant<bool, true> {
    };

    template<class T, class U>
    struct is_same : public false_type {
    };

    template<class T>
    struct is_same<T, T> : public true_type {
    };

    template<typename T, typename... Rest>
    struct has_duplicates : false_type {
    };

    template<typename T, typename First>
    struct has_duplicates<T, First> : is_same<T, First> {
    };

    template<typename T, typename First, typename... Rest>
    struct has_duplicates<T, First, Rest...> : integral_constant<bool, is_same<T, First>::value || has_duplicates<T, Rest...>::value> {
    };

    template<class T, class U>
    struct optional_type {
        using type = T;
    };

    template<class U>
    struct optional_type<void, U> {
        using type = U;
    };

    template<class First, class ... Rest>
    struct pin_count {
        using other = pin_count<Rest...>;
        static constexpr auto value = First::not_pin ? 0 : 1 + other::value;
    };

    template<class First>
    struct pin_count<First> {
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
