#ifndef ZOAL_CT_CONDITIONAL_VALUE_HPP
#define ZOAL_CT_CONDITIONAL_VALUE_HPP

namespace zoal { namespace ct {
    template<bool condition, class T, T A, T B>
    struct conditional_value {};

    template<class T, T A, T B>
    struct conditional_value<true, T, A, B> {
        static constexpr auto value = A;
    };

    template<class T, T A, T B>
    struct conditional_value<false, T, A, B> {
        static constexpr auto value = B;
    };
}}

#endif
