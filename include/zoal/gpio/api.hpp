#ifndef ZOAL_GPIO_API_HPP
#define ZOAL_GPIO_API_HPP

#include "../ct/check.hpp"
#include "../ct/type_list.hpp"
#include "pin.hpp"

namespace zoal { namespace gpio {
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

    struct api {
        template<class... Pins>
        using low = typename zoal::mem::merge_cas_in_list<typename collect_cas<low_cas, Pins...>::result>::result;

        template<class... Pins>
        using high = typename zoal::mem::merge_cas_in_list<typename collect_cas<high_cas, Pins...>::result>::result;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = typename zoal::mem::merge_cas_in_list<typename collect_cas<mode_cas<PinMode>, Pins...>::result>::result;

        template<class L, class... Rest>
        struct optimize {
            using all = typename zoal::ct::type_list_join<L, Rest...>::result;
            using result = typename zoal::mem::merge_cas_in_list<all>::result;

            optimize() = delete;

            static void apply() {
                zoal::mem::apply_cas_list<result>();
            }
        };

        template<class L>
        struct optimize<L> {
            using result = L;

            optimize() = delete;

            static void apply() {
                zoal::mem::apply_cas_list<L>();
            }
        };
    };
}}

#endif
