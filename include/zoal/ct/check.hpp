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
    struct has_same : false_type {};

    template<typename T, typename U>
    struct has_same<T, U> : is_same<T, U> {};

    template<typename T, typename U, typename... O>
    struct has_same<T, U, O...> : integral_constant<bool, is_same<T, U>::value || has_same<T, O...>::value> {};
}}

#endif
