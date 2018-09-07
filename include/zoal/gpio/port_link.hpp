#pragma once

#ifndef ZOAL_GPIO_Port_ITERATOR_HPP
#define ZOAL_GPIO_Port_ITERATOR_HPP

#include "null_port.hpp"
#include "../utils/helpers.hpp"

namespace zoal { namespace gpio {

    class port_link_terminator {
    public:
        using port = null_port;

        template<class T>
        inline void operator&(T) {}

        template<class T>
        inline void flush() const {}
    };

    template<class Port, class Next>
    class port_link {
    public:
        using port = Port;
        using next = Next;

        typename port::register_type mask{0};
        typename port::register_type data{0};

        template<class T>
        inline void flush() const {
            if (mask) {
                T::flush(*this);
            }
            next_.template flush<T>();
        }

        template<class Pin>
        inline port_link operator&(Pin pin) {
            if (Port::address == Pin::port::address) {
                mask |= Pin::mask;
                data |= pin.value << Pin::offset;
            }
            next_.operator &(pin);
            return *this;
        }

    private:
        Next next_;
    };

    template<class First, class ... Rest>
    class chain_builder {
    public:
        static_assert(!zoal::utils::has_duplicates<First, Rest...>::value, "Some port was used twice");

        using next_builder = chain_builder<Rest...>;
        using chain = port_link<First, typename next_builder::chain>;
    };

    template<class First>
    class chain_builder<First> {
    public:
        using chain = port_link<First, ::zoal::gpio::port_link_terminator>;
    };
}}

#endif
