#ifndef ZOAL_CT_HELPERS_HPP
#define ZOAL_CT_HELPERS_HPP

#include "type_list.hpp"

namespace zoal { namespace ct {
    template<class First, class... Rest>
    struct max_type_size {
        using other = max_type_size<Rest...>;
        static constexpr auto value = sizeof(First) > other::value ? sizeof(First) : other::value;
    };

    template<class First>
    struct max_type_size<First> {
        static constexpr auto value = sizeof(First);
    };

    template<class First, class... Rest>
    struct min_type_size {
        using other = max_type_size<Rest...>;
        static constexpr auto value = sizeof(First) < other::value ? sizeof(First) : other::value;
    };

    template<class First>
    struct min_type_size<First> {
        static constexpr auto value = sizeof(First);
    };

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
