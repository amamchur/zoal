#ifndef ZOAL_UTILS_TOOLS_HPP
#define ZOAL_UTILS_TOOLS_HPP

#include "../gpio/api.hpp"
#include "delay.hpp"
#include "interrupts.hpp"
#include "logger.hpp"
#include "nop.hpp"
#include "scheduler.hpp"

namespace zoal { namespace utils {
    template<class Microcontroller, uint32_t Frequency, class Counter, class Logger, class... Mixin>
    class tool_set : public Mixin... {
    public:
        using mcu = Microcontroller;
        using counter = Counter;
        using logger = Logger;
        using counter_value_type = typename Counter::value_type;
        using delay = utils::delay<Frequency, counter>;

        template<size_t Capacity, class Token = void>
        using function_scheduler = typename ::zoal::utils::function_scheduler<counter, Capacity, Token>;

        template<class T, size_t Capacity, class Token = void>
        using method_scheduler = typename ::zoal::utils::method_scheduler<T, counter, Capacity, Token>;
    };

    template<class T>
    T atomic_read(volatile T &src) {
        zoal::utils::interrupts ni(false);
        return src;
    }

    template<class T>
    void atomic_write(volatile T &dest, T value) {
        zoal::utils::interrupts ni(false);
        dest = value;
    }
}}

#endif
