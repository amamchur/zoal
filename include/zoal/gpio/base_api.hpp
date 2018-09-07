#pragma once

#ifndef ZOAL_GPIO_BASE_API_HPP
#define ZOAL_GPIO_BASE_API_HPP

#include "pin.hpp"
#include "port_link.hpp"
#include "../utils/helpers.hpp"

namespace zoal { namespace gpio {
    template<zoal::gpio::pin_mode PinMode>
    struct mode_functor {
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::template mode<PinMode, static_cast<typename Port::register_type>(mask)>();
            }
        }
    };

    struct low_functor {
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::low(static_cast<typename Port::register_type>(mask));
            }
        }
    };

    struct high_functor {
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::high(static_cast<typename Port::register_type>(mask));
            }
        }
    };

    struct toggle_functor {
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::toggle(static_cast<typename Port::register_type>(mask));
            }
        }
    };

    struct power_on_functor {
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::power_on();
            }
        }
    };

    struct power_off_functor {
    public:
        template <class Port, uintptr_t mask>
        static inline void apply() {
            if (mask) {
                Port::power_off();
            }
        }
    };

    template<class Port, class FirstPin, class ... Rest>
    struct pin_filter {
        using register_type = typename Port::register_type;
        using next = pin_filter<Port, Rest...>;

        static constexpr auto mask = (zoal::utils::belongs_to_port<FirstPin, Port>::value ? FirstPin::mask : 0) | next::mask;
    };

    template<class Port, class LastPin>
    struct pin_filter<Port, LastPin> {
        using register_type = typename Port::register_type;
        static constexpr auto mask = zoal::utils::belongs_to_port<LastPin, Port>::value ? LastPin::mask : 0;
    };

    template<>
    struct pin_filter<null_port, null_pin> {
        static constexpr auto mask = 0;
    };

    template<class Link, class Functor, class ... Pins>
    struct executor {
        using port = typename Link::port;
        using functor = Functor;
        using next = executor<typename Link::next, Functor, Pins...>;
        using filter = pin_filter<port, Pins...>;

        static constexpr auto mask = filter::mask;

        static void apply() {
            functor::template apply<port, mask>();
            next::apply();
        }
    };

    template<class Functor, class ... Pins>
    struct executor<port_link_terminator, Functor, Pins...> {
        using port = null_port;
        using functor = Functor;
        using next = void;
        static constexpr auto mask = 0;

        static void apply() {
        }
    };

    template <class A, class B>
    struct merge {
        using port = typename A::port;
        using functor = typename A::functor;
        using next = merge<typename A::next, typename B::next>;
        static constexpr auto mask = A::mask | B::mask;

        static void apply() {
            functor::template apply<port, mask>();
            next::apply();
        }
    };

    template <>
    struct merge<void, void> {
        using next = void;
        static constexpr auto mask = 0;

        static void apply() {
        }
    };

    template<class Chain>
    struct base_api {
        using chain = Chain;
        using register_type = typename chain::port::register_type;

        template<class ... Pins>
        using power_on = executor<chain, power_on_functor, Pins...>;

        template<class ... Pins>
        using power_off = executor<chain, power_off_functor, Pins...>;

        template<class ... Pins>
        using low = executor<chain, low_functor, Pins...>;

        template<class ... Pins>
        using high = executor<chain, high_functor, Pins...>;

        template<class ... Pins>
        using toggle = executor<chain, toggle_functor, Pins...>;

        template<zoal::gpio::pin_mode PinMode, class ... Pins>
        using mode = executor<chain, mode_functor<PinMode>, Pins...>;
    };
}}

#endif
