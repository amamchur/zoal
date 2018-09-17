#ifndef ZOAL_CT_CHECK_HPP
#define ZOAL_CT_CHECK_HPP

#include "constant.hpp"

namespace zoal { namespace ct {
    template<class T, class U>
    struct is_same : false_type {};

    template<class T>
    struct is_same<T, T> : true_type {};

    template<class T>
    struct is_pin : integral_constant<bool, T::port::address != 0 && T::offset != 0> {};

    template<class Pin, class Port>
    struct belongs_to_port : is_same<typename Pin::port, Port> {};

    template<typename T, typename... Rest>
    struct has_duplicates : false_type {};

    template<typename T, typename First>
    struct has_duplicates<T, First> : is_same<T, First> {};

    template<typename T, typename First, typename... Rest>
    struct has_duplicates<T, First, Rest...>
        : integral_constant<bool, is_same<T, First>::value || has_duplicates<T, Rest...>::value> {};
}}

#endif
