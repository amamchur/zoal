#pragma once

#ifndef ZOAL_GPIO_API_HPP
#define ZOAL_GPIO_API_HPP

#include "../ct/check.hpp"
#include "../ct/type_list.hpp"
#include "../mem/cas.hpp"
#include "./pin_mode.hpp"
#include "./pin_outout_speed.hpp"

namespace zoal::gpio {
    struct enable_cas {
        template<class Module>
        using list = Module::enable_cas;
    };

    struct disable_cas {
        template<class Module>
        using list = Module::disable_cas;
    };

    struct clock_on_cas {
        template<class Module>
        using list = typename Module::clock_on_cas;
    };

    struct clock_off_cas {
        template<class Module>
        using list = Module::clock_off_cas;
    };

    struct low_cas {
        template<class Pin>
        using list = Pin::port::template low_cas<Pin::mask>;
    };

    struct high_cas {
        template<class Pin>
        using list = Pin::port::template high_cas<Pin::mask>;
    };

    template<pin_mode PinMode>
    struct mode_cas {
        template<class Pin>
        using list = Pin::port::template mode_cas<PinMode, Pin::mask>;
    };

    template<pin_output_speed Speed>
    struct speed_cas {
        template<class Pin>
        using list = Pin::port::template output_speed_cas<Speed, Pin::mask>;
    };

    template<class Selector, class T, class... Rest>
    struct collect_cas {
        using current = Selector::template list<T>;
        using next = collect_cas<Selector, Rest...>::result;
        using result = ct::type_list_join<current, next>::result;
    };

    template<class Selector, class T>
    struct collect_cas<Selector, T> {
        using result = Selector::template list<T>;
    };

    struct api {
        template<class... Module>
        using clock_on = mem::merge_cas_in_list<typename collect_cas<clock_on_cas, Module...>::result>::result;

        template<class... Module>
        using clock_off = mem::merge_cas_in_list<typename collect_cas<clock_off_cas, Module...>::result>::result;

        template<class... Module>
        using enable = mem::merge_cas_in_list<typename collect_cas<enable_cas, Module...>::result>::result;

        template<class... Module>
        using disable = mem::merge_cas_in_list<typename collect_cas<disable_cas, Module...>::result>::result;

        template<class... Pins>
        using low = mem::merge_cas_in_list<typename collect_cas<low_cas, Pins...>::result>::result;

        template<class... Pins>
        using _0 = low<Pins...>;

        template<class... Pins>
        using high = mem::merge_cas_in_list<typename collect_cas<high_cas, Pins...>::result>::result;

        template<class... Pins>
        using _1 = high<Pins...>;

        template<pin_mode PinMode, class... Pins>
        using mode = mem::merge_cas_in_list<typename collect_cas<mode_cas<PinMode>, Pins...>::result>::result;

        template<pin_mode PinMode, class... Pins>
        using _m = mode<PinMode, Pins...>;

        template<pin_output_speed Speed, class... Pins>
        using output_speed = mem::merge_cas_in_list<typename collect_cas<speed_cas<Speed>, Pins...>::result>::result;

        template<class L, class... Rest>
        struct perform_cas_optimization {
            using all = ct::type_list_join<L, Rest...>::result;
            using result = mem::merge_cas_in_list<all>::result;
        };

        template<class L>
        struct perform_cas_optimization<L> {
            using result = mem::merge_cas_in_list<L>::result;
        };

        template<class L, class... Rest>
        struct perform_optimization {
            using optimized = perform_cas_optimization<L, Rest...>::result;
            using is_void = ct::is_same<void, optimized>;
            using list = ct::conditional_type<is_void::value, mem::null_cas_list, optimized>::type;
            using is_null = ct::is_same<mem::null_cas_list, list>;
            using clist = mem::callable_cas_list<list>;
            using result = ct::conditional_type<is_null::value, void, clist>::type;
        };

        template<class L, class... Rest>
        using optimize = perform_optimization<L, Rest...>::result;
    };
}

#endif
