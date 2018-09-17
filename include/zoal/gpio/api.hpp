#pragma once

#ifndef ZOAL_GPIO_BASE_API_HPP
#define ZOAL_GPIO_BASE_API_HPP

#include "../ct/check.hpp"
#include "../utils/helpers.hpp"
#include "pin.hpp"

namespace zoal { namespace gpio {
    template<zoal::gpio::pin_mode PinMode>
    struct mode_functor {
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::template mode<PinMode, static_cast<typename Port::register_type>(Mask)>();
        }
    };

    struct low_functor {
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::template low<static_cast<typename Port::register_type>(Mask)>();
        }
    };

    struct high_functor {
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::template high<static_cast<typename Port::register_type>(Mask)>();
        }
    };

    struct toggle_functor {
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::template toggle<static_cast<typename Port::register_type>(Mask)>();
        }
    };

    struct power_on_functor {
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::power_on();
        }
    };

    struct power_off_functor {
    public:
        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::power_off();
        }
    };

    template<class Port, class Pin, class... Rest>
    struct port_usage {
        using register_type = typename Port::register_type;
        using next = port_usage<Port, Rest...>;

        static constexpr auto belongs = zoal::ct::belongs_to_port<Pin, Port>::value;
        static constexpr auto mask = (belongs ? Pin::mask : 0) | next::mask;
    };

    template<class Port, class Pin>
    struct port_usage<Port, Pin> {
        using register_type = typename Port::register_type;
        using next = void;

        static constexpr auto belongs = zoal::ct::belongs_to_port<Pin, Port>::value;
        static constexpr auto mask = belongs ? Pin::mask : 0;
    };

    template<>
    struct port_usage<null_port, null_pin> {
        static constexpr auto mask = 0;
    };

    template<class Port, class Functor, class Next, uintptr_t Mask>
    struct functor_invoker {
        static void invoke() {
            Functor::template apply<Port, Mask>();
            Next::apply();
        }
    };

    template<class Functor, class Next, uintptr_t Mask>
    struct functor_invoker<null_port, Functor, Next, Mask> {
        static void invoke() {}
    };

    template<class Functor, class Next>
    struct functor_invoker<null_port, Functor, Next, 0> {
        static void invoke() {
            Next::apply();
        }
    };

    template<class Port, class Functor, uintptr_t Mask>
    struct functor_invoker<Port, Functor, void, Mask> {
        static void invoke() {
            Functor::template apply<Port, Mask>();
        }
    };

    template<class Port, class Functor, class Next>
    struct functor_invoker<Port, Functor, Next, 0> {
        static void invoke() {
            Next::apply();
        }
    };

    template<class Link, class Functor, class... Pins>
    struct port_action {
        using port = typename Link::type;
        using functor = Functor;
        using next = port_action<typename Link::next, Functor, Pins...>;
        using usage = port_usage<port, Pins...>;

        static constexpr auto mask = usage::mask;

        port_action() = delete;

        static void apply() {
            functor_invoker<port, functor, next, mask>::invoke();
        }
    };

    template<class Functor, class... Pins>
    struct port_action<void, Functor, Pins...> {
        using port = null_port;
        using functor = Functor;
        using next = void;
        static constexpr auto mask = 0;

        port_action() = delete;

        static void apply() {}
    };

    template<class A, class B>
    struct merge_actions {
        static_assert(zoal::ct::is_same<typename A::functor, typename B::functor>::value,
                      "Actions have different functor");

        static_assert(zoal::ct::is_same<typename A::port, typename B::port>::value, "Port list does not match");

        using port = typename A::port;
        using functor = typename A::functor;
        using next = merge_actions<typename A::next, typename B::next>;
        static constexpr auto mask = A::mask | B::mask;

        merge_actions() = delete;

        static void apply() {
            functor_invoker<port, functor, next, mask>::invoke();
        }
    };

    template<>
    struct merge_actions<void, void> {
        using port = null_port;
        using next = void;
        using functor = void;

        static constexpr auto mask = 0;

        merge_actions() = delete;

        static void apply() {}
    };

    template<class A>
    struct merge_actions<A, void> {
        using port = null_port;
        using next = void;
        using functor = void;

        static constexpr auto mask = 0;

        merge_actions() = delete;

        static void apply() {}
    };

    template<class B>
    struct merge_actions<void, B> {
        using port = null_port;
        using next = void;
        using functor = void;

        static constexpr auto mask = 0;

        merge_actions() = delete;

        static void apply() {}
    };

    template<class Ports>
    struct api {
        using ports = Ports;
        using register_type = typename ports::type::register_type;

        template<class... Pins>
        using power_on = port_action<ports, power_on_functor, Pins...>;

        template<class... Pins>
        using power_off = port_action<ports, power_off_functor, Pins...>;

        template<class... Pins>
        using low = port_action<ports, low_functor, Pins...>;

        template<class... Pins>
        using high = port_action<ports, high_functor, Pins...>;

        template<class... Pins>
        using toggle = port_action<ports, toggle_functor, Pins...>;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = port_action<ports, mode_functor<PinMode>, Pins...>;
    };
}}

#endif
