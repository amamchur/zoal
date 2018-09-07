#pragma once

#ifndef ZOAL_GPIO_Port_ITERATOR_HPP
#define ZOAL_GPIO_Port_ITERATOR_HPP

#include "null_port.hpp"
#include "../utils/helpers.hpp"

namespace zoal { namespace gpio {
    template<class Port, class Next>
    class port_link {
    public:
        using port = Port;
        using next = Next;
    };

    template<class First, class ... Rest>
    class chain_builder {
    public:
        static_assert(!zoal::utils::has_duplicates<First, Rest...>::value, "Some port was been used twice");

        using next_builder = chain_builder<Rest...>;
        using chain = port_link<First, typename next_builder::chain>;
    };

    template<class First>
    class chain_builder<First> {
    public:
        using chain = port_link<First, void>;
    };
}}

#endif
