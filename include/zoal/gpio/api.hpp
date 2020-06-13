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
            using list = typename Port::template mode_modifiers<PinMode, Mask>::modifiers;
        };
    };

    struct low_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = typename Port::template low_modifiers<Mask>::modifiers;
        };
    };

    struct high_functor {
        template<class Port, uintptr_t Mask>
        struct modifiers {
            using list = typename Port::template high_modifiers<Mask>::modifiers;
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

    template<class A, class Next>
    struct action_item {
        using port = typename A::port;
        using next = Next;
        using modifiers = typename A::modifiers;

        static constexpr auto mask = A::mask;
    };

    template<class ActionList>
    struct filter_port_actions {
        static constexpr bool empty = ActionList::mask == 0;

        using next = typename filter_port_actions<typename ActionList::next>::result;
        using current = action_item<ActionList, next>;
        using result = typename zoal::ct::conditional_type<empty, next, current>::type;
    };

    template<>
    struct filter_port_actions<void> {
        using result = void;
    };

    template<class Actions>
    struct apply_port_actions {
        using port = typename Actions::port;
        using fm = typename zoal::mem::filter_modifiers<typename Actions::modifiers>::result;
        using current = zoal::mem::apply_modifiers<port::address, fm>;
        using next = apply_port_actions<typename Actions::next>;

        ZOAL_INLINE_MF apply_port_actions() {
            current();
            next();
        }
    };

    template<>
    struct apply_port_actions<void> {};

    template<class Link, class Functor, class... Pins>
    struct port_action {
        using self_type = port_action<Link, Functor, Pins...>;
        using port = typename Link::type;
        using functor = Functor;
        using next = typename port_action<typename Link::next, Functor, Pins...>::self_type;

        static constexpr auto mask = port_usage<port, Pins...>::mask;
        using modifiers = typename Functor::template modifiers<port, mask>::list;

        ZOAL_INLINE_MF port_action() {
            using fpa = typename filter_port_actions<self_type>::result;
            apply_port_actions<fpa>();
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

        ZOAL_INLINE_MF merge_ports_actions_lists() {
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
        class merge {
        public:
            using list = zoal::ct::type_list<Actions...>;
            using result = typename merge_ports_actions_test<list>::result;

            merge() {
                apply_port_actions<result>();
            }
        };

        template<class... Pins>
        using power_on = port_action<ports, power_on_functor, Pins...>;

        template<class... Pins>
        using power_off = port_action<ports, power_off_functor, Pins...>;

        template<class... Pins>
        using low = port_action<ports, low_functor, Pins...>;

        template<class... Pins>
        using _0 = low<Pins...>;

        template<class... Pins>
        using high = port_action<ports, high_functor, Pins...>;

        template<class... Pins>
        using _1 = high<Pins...>;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = port_action<ports, mode_functor<PinMode>, Pins...>;
    };

    // New API

    struct low_cas {
        template<class Pin>
        using list = typename Pin::port::template low_cas<Pin::mask>;
    };

    struct high_cas {
        template<class Pin>
        using list = typename Pin::port::template high_cas<Pin::mask>;
    };

    template<zoal::gpio::pin_mode PinMode>
    struct mode_cas {
        template<class Pin>
        using list = typename Pin::port::template mode_cas<PinMode, Pin::mask>;
    };

    template<class Collector, class Pin, class... Rest>
    struct collect_cas {
        using current = typename Collector::template list<Pin>;
        using next = typename collect_cas<Collector, Rest...>::result;
        using result = typename zoal::ct::type_list_join<current, next>::result;
    };

    template<class Collector, class Pin>
    struct collect_cas<Collector, Pin> {
        using result = typename Collector::template list<Pin>;
    };

    struct api_new {
        template<class... Pins>
        using low = typename zoal::mem::merge_cas_in_list<typename collect_cas<low_cas, Pins...>::result>::result;

        template<class... Pins>
        using high = typename zoal::mem::merge_cas_in_list<typename collect_cas<high_cas, Pins...>::result>::result;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = typename zoal::mem::merge_cas_in_list<typename collect_cas<mode_cas<PinMode>, Pins...>::result>::result;

        template<class L, class... Rest>
        struct apply {
            using all = typename zoal::ct::type_list_join<L, Rest...>::result;
            using result = typename zoal::mem::merge_cas_in_list<all>::result;
            apply() {
                zoal::mem::apply_cas_list<result>();
            }
        };

        template<class L>
        struct apply<L> {
            apply() {
                zoal::mem::apply_cas_list<L>();
            }
        };
    };
}}

#endif
