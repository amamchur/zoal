#pragma once

#ifndef ZOAL_GPIO_Port_ITERATOR_HPP
#define ZOAL_GPIO_Port_ITERATOR_HPP

#include "../utils/helpers.hpp"

namespace zoal { namespace gpio {

    class port_link_terminator {
    public:
        template<class T>
        inline void operator<<(T) {}

        template<class T>
        inline void operator>>(T) {}

        template<class T>
        inline void operator&(T) {}

        template<class T>
        inline void aggregate(T) {}

        template<class T>
        void flush() const {}
    };

    template<class Port, class Next>
    class port_link {
    protected:
        Next next;
    public:
        using port = Port;

        typename port::register_type mask;
        typename port::register_type data;

        port_link() : mask(0), data(0) {
        }
    };

    template<class Port, class Next>
    class aggregation_link {
    public:
        using port = Port;
        typename port::register_type mask;
        typename port::register_type data;

        Next next;

        inline aggregation_link() : mask(0), data(0) {
        }

        template<class T>
        inline void flush() const {
            if (mask) {
                T::flush(*this);
            }
            next.template flush<T>();
        }

        template<class Pin>
        inline void aggregate(Pin pin) {
            if (Port::address == Pin::port::address) {
                mask |= Pin::mask;
                data |= pin.value << Pin::offset;
            }

            next.aggregate(pin);
        }

        template<class Pin>
        inline aggregation_link &operator&(Pin &&pin) {
            this->aggregate(pin);
            return *this;
        }
    };

    template<class Ctrl, class Next, class Functor>
    class functor_link : public port_link<Ctrl, Next> {
    protected:
        Functor functor_;
    public:

        template<class Pin>
        inline functor_link &operator>>(Pin pin) {
            if (Ctrl::Base == Pin::Port::Base) {
                functor_(*this, pin);
                return *this;
            }

            this->next << pin;

            return *this;
        }
    };

    template<class First, class ... Rest>
    class chain_builder {
    public:
        static_assert(!zoal::utils::has_duplicates<First, Rest...>::value, "Some port was used twice");

        using next_builder = chain_builder<Rest...>;
        using chain = aggregation_link<First, typename next_builder::chain>;
    };

    template<class First>
    class chain_builder<First> {
    public:
        using chain = aggregation_link<First, ::zoal::gpio::port_link_terminator>;
    };
}}

#endif
