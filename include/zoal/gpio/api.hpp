#ifndef ZOAL_GPIO_API_HPP
#define ZOAL_GPIO_API_HPP

#include "../ct/check.hpp"
#include "../ct/type_list.hpp"
#include "../mem/modifier.hpp"
#include "pin.hpp"

namespace zoal { namespace gpio {
    template<zoal::gpio::pin_mode PinMode>
    struct mode_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = typename Port::template mode<PinMode, Mask>::modifiers;
        };
    };

    struct low_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = typename Port::template low<Mask>::modifiers;
        };
    };

    struct high_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = typename Port::template high<Mask>::modifiers;
        };
    };

    struct power_on_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = void;
        };

        template<class Port, uintptr_t Mask>
        static void apply() {
            Port::power_on();
        }
    };

    struct power_off_functor {
    public:
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = void;
        };

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

    template<class Port, class Functor>
    struct functor_invoker<Port, Functor, void, 0> {
        static void invoke() {
        }
    };

    template<class Action>
    struct apply_port_actions {
        apply_port_actions() = delete;

        static void apply() {
            using port = typename Action::port;
            using mds = typename Action::modifiers;
            zoal::mem::apply_modifiers<port::address, mds>();

            apply_port_actions<typename Action::next>::apply();
        }
    };

    template<>
    struct apply_port_actions<void> {
        apply_port_actions() = delete;

        static void apply() {}
    };

    template<class Link, class Functor, class... Pins>
    struct port_action {
        using self_type = port_action<Link, Functor, Pins...>;
        using port = typename Link::type;
        using functor = Functor;
        using next = typename port_action<typename Link::next, Functor, Pins...>::self_type;
        using usage = port_usage<port, Pins...>;

        static constexpr auto mask = usage::mask;

        using modifiers = typename Functor::template modifiers<port, mask>::list;

        port_action() {
            apply_port_actions<self_type>::apply();
        }

        static void apply() {
            functor_invoker<port, functor, next, mask>::invoke();
        }
    };

    template<class Functor, class... Pins>
    struct port_action<void, Functor, Pins...> {
        using self_type = void;
    };

    template<class A, class B>
    struct merge_ports_actions_lists {
        using port = typename A::port;
        using self_type = merge_ports_actions_lists<A, B>;
        using am = typename A::modifiers;
        using bm = typename B::modifiers;
        using modifiers = typename zoal::mem::merge_modifiers<am, bm>::result;
        using next = typename merge_ports_actions_lists<typename A::next, typename B::next>::self_type;

        merge_ports_actions_lists() {
            apply_port_actions<self_type>::apply();
        }
    };

    template<>
    struct merge_ports_actions_lists<void, void> {
        using self_type = void;
    };

    template<class A, class B>
    struct pre_merge {
        using a = typename A::type;
        using b = typename B::type;
        using result = merge_ports_actions_lists<a, b>;
    };

    template<class A>
    struct pre_merge<A, void> {
        using a = typename A::type;
        using result = typename A::type;
    };

    template<class A, class B>
    struct post_merge {
        using result = merge_ports_actions_lists<A, B>;
    };

    template<class A>
    struct post_merge<A, void> {
        using result = A;
    };

    template<class Link>
    struct merge_ports_actions_test {
        using tmp = typename pre_merge<Link, typename Link::next>::result;
        using next = typename merge_ports_actions_test<typename Link::next>::result;
        using result = typename post_merge<tmp, next>::result;
    };

    template<>
    struct merge_ports_actions_test<void> {
        using result = void;
    };

    template<class ListA, class ListB, class... Lists>
    struct merge_ports_actions {
        using tmp = merge_ports_actions_lists<ListA, ListB>;
        using result = typename merge_ports_actions<tmp, Lists...>::result;
    };

    template<class ListA, class ListB>
    struct merge_ports_actions<ListA, ListB> {
        using result = merge_ports_actions_lists<ListA, ListB>;
    };

    template<class Ports>
    struct api {
        using ports = Ports;
        using register_type = typename ports::type::register_type;

        template<class... Actions>
        class merge_actions {
        public:
            using list = zoal::ct::type_list<Actions...>;
            using result = typename merge_ports_actions_test<list>::result;

            merge_actions() {
                apply_port_actions<result>::apply();
            }
        };

        template<class... Pins>
        using power_on = port_action<ports, power_on_functor, Pins...>;

        template<class... Pins>
        using power_off = port_action<ports, power_off_functor, Pins...>;

        template<class... Pins>
        using low = port_action<ports, low_functor, Pins...>;

        template<class... Pins>
        using high = port_action<ports, high_functor, Pins...>;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = port_action<ports, mode_functor<PinMode>, Pins...>;
    };
}}

#endif
