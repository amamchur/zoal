#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_HPP

#include "../../mem/accessor.hpp"
#include "../../periph/timer_mode.hpp"
#include "../bus.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class timer : public Mixin... {
    public:
        using self_type = timer<Address, N, Mixin...>;
        using word = typename self_type::word;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<word, Address, Offset>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t channels_count = 2;
        static constexpr uint8_t resolution = sizeof(self_type::word) * 8;

        timer() = delete;

        static void power_on() {}

        static void power_off() {}

        static void enable() {}

        static void disable() {}

        static void counter(word value) {
            *accessor<self_type::TCNTx>::p = value;
        }

        static word counter() {
            return *accessor<self_type::TCNTx>::p;
        }

        template<uint8_t Channel>
        static word compare_value() {
            static_assert(Channel < channels_count, "Channel index is out of range");

            switch (Channel) {
            case 0:
                return *accessor<self_type::OCRxA>::p;
            case 1:
                return *accessor<self_type::OCRxB>::p;
            default:
                break;
            }

            return 0;
        }

        template<uint8_t Channel>
        static void compare_value(word value) {
            static_assert(Channel < channels_count, "Channel index is out of range");

            switch (Channel) {
            case 0:
                *accessor<self_type::OCRxA>::p = value;
                break;
            case 1:
                *accessor<self_type::OCRxB>::p = value;
                break;
            default:
                break;
            }
        }
    };
}}}

namespace zoal { namespace metadata {
    template<::zoal::periph::timer_mode Mode>
    struct timer_mode;
}}

#endif
