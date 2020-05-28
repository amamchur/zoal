#ifndef ZOAL_UTILS_TOOLS_HPP
#define ZOAL_UTILS_TOOLS_HPP

#include "../../gpio/api.hpp"
#include "../interrupts.hpp"
#include "../logger.hpp"
#include "../nop.hpp"
#include "../scheduler.hpp"
#include "delay.hpp"
#include "ms_counter.hpp"

namespace zoal { namespace utils { namespace cmsis_os2 {
    template<class Microcontroller, class Logger, class... Mixin>
    class tool_set : public Mixin... {
    public:
        using mcu = Microcontroller;
        using api = typename mcu::api;
        using counter = ::zoal::utils::cmsis_os2::ms_counter;
        using logger = Logger;
        using counter_value_type = typename counter::value_type;
        using delay = utils::cmsis_os2::delay<mcu>;

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
}}}

#endif
