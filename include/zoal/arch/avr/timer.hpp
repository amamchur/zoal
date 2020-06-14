#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_HPP

#include "../../periph/timer_mode.hpp"
#include "../bus.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class timer : public Mixin... {
    public:
        using self_type = timer<Address, N, Mixin...>;
        using word = typename self_type::word;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t channels_count = 2;
        static constexpr uint8_t resolution = sizeof(self_type::word) * 8;

        using enable_cas = zoal::ct::type_list<zoal::mem::null_cas>;
        using disable_cas = zoal::ct::type_list<zoal::mem::null_cas>;
        using power_on_cas = zoal::ct::type_list<zoal::mem::null_cas>;
        using power_off_cas = zoal::ct::type_list<zoal::mem::null_cas>;

        static void power_on() {}

        static void power_off() {}

        static void enable() {}

        static void disable() {}

        static void counter(word value) {
            self_type::TCNTx::ref() = value;
        }

        static word counter() {
            return self_type::TCNTx::ref();
        }

        template<uint8_t Channel>
        static word compare_value() {
            static_assert(Channel < channels_count, "Channel index is out of range");

            switch (Channel) {
            case 0:
                return self_type::OCRxA::ref();
            case 1:
                return self_type::OCRxB::ref();
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
                self_type::OCRxA::ref() = value;
                break;
            case 1:
                self_type::OCRxB::ref() = value;
                break;
            default:
                break;
            }
        }
    };
}}}

namespace zoal { namespace metadata {
    template<class T, ::zoal::periph::timer_mode Mode>
    struct timer_mode;
}}

#endif
