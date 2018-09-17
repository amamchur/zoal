#ifndef ZOAL_CT_CONSTANT_HPP
#define ZOAL_CT_CONSTANT_HPP

namespace zoal { namespace ct {
    template<class T, T Value>
    struct integral_constant {
        using value_type = T;
        using type = integral_constant<T, Value>;

        static constexpr T value = Value;

        constexpr operator T() const {
            return Value;
        }
    };

    struct false_type : integral_constant<bool, false> {
    };

    struct true_type : integral_constant<bool, true> {
    };
}}

#endif
