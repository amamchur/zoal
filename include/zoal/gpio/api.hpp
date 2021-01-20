#ifndef ZOAL_GPIO_API_HPP
#define ZOAL_GPIO_API_HPP

#include "../ct/check.hpp"
#include "../ct/type_list.hpp"
#include "../mem/cas.hpp"
#include "./pin_mode.hpp"

namespace zoal { namespace gpio {
    struct enable_cas {
        template<class Module>
        using list = typename Module::enable_cas;
    };

    struct disable_cas {
        template<class Module>
        using list = typename Module::disable_cas;
    };

    struct clock_on_cas {
        template<class Module>
        using list = typename Module::clock_on_cas;
    };

    struct clock_off_cas {
        template<class Module>
        using list = typename Module::clock_off_cas;
    };

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

    template<class Selector, class T, class... Rest>
    struct collect_cas {
        using current = typename Selector::template list<T>;
        using next = typename collect_cas<Selector, Rest...>::result;
        using result = typename zoal::ct::type_list_join<current, next>::result;
    };

    template<class Selector, class T>
    struct collect_cas<Selector, T> {
        using result = typename Selector::template list<T>;
    };

    struct api {
        template<class... Module>
        using clock_on = typename zoal::mem::merge_cas_in_list<typename collect_cas<clock_on_cas, Module...>::result>::result;

        template<class... Module>
        using clock_off = typename zoal::mem::merge_cas_in_list<typename collect_cas<clock_off_cas, Module...>::result>::result;

        template<class... Module>
        using enable = typename zoal::mem::merge_cas_in_list<typename collect_cas<enable_cas, Module...>::result>::result;

        template<class... Module>
        using disable = typename zoal::mem::merge_cas_in_list<typename collect_cas<disable_cas, Module...>::result>::result;

        template<class... Pins>
        using low = typename zoal::mem::merge_cas_in_list<typename collect_cas<low_cas, Pins...>::result>::result;

        template<class... Pins>
        using high = typename zoal::mem::merge_cas_in_list<typename collect_cas<high_cas, Pins...>::result>::result;

        template<zoal::gpio::pin_mode PinMode, class... Pins>
        using mode = typename zoal::mem::merge_cas_in_list<typename collect_cas<mode_cas<PinMode>, Pins...>::result>::result;

        template<class L, class... Rest>
        struct perform_cas_optimization {
            using all = typename zoal::ct::type_list_join<L, Rest...>::result;
            using result = typename zoal::mem::merge_cas_in_list<all>::result;
        };

        template<class L>
        struct perform_cas_optimization<L> {
            using result = typename zoal::mem::merge_cas_in_list<L>::result;
        };

        template<class L, class... Rest>
        using optimize = zoal::mem::callable_cas_list<typename perform_cas_optimization<L, Rest...>::result>;
    };
}}

#endif
