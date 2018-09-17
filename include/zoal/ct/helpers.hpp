#ifndef ZOAL_HELPERS_HPP
#define ZOAL_HELPERS_HPP

namespace zoal { namespace ct {
    template<class T, class U>
    struct optional_type {
        using type = T;
    };

    template<class U>
    struct optional_type<void, U> {
        using type = U;
    };

    template<class First, class... Rest>
    struct pin_count {
        using other = pin_count<Rest...>;
        static constexpr auto value = First::not_pin ? 0 : 1 + other::value;
    };

    template<class First>
    struct pin_count<First> {
        static constexpr auto value = First::not_pin ? 0 : 1;
    };
}}

#endif
