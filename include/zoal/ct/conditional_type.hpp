#ifndef ZOAL_CT_CONDITIONAL_TYPE_HPP
#define ZOAL_CT_CONDITIONAL_TYPE_HPP

namespace zoal { namespace ct {
    template<bool condition, class T, class U>
    struct conditional_type {};

    template<class T, class U>
    struct conditional_type<true, T, U> {
        using type = T;
    };

    template<class T, class U>
    struct conditional_type<false, T, U> {
        using type = U;
    };
}}

#endif
