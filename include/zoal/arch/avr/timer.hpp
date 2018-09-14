#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_HPP

#include <stdint.h>
#include "../../periph/timer_mode.hpp"
#include "../../utils/memory_segment.hpp"
#include "../bus.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class ...Mixin>
    class timer : public Mixin ... {
    public:
        using self_type = timer<Address, N, Mixin...>;
        using word = typename self_type::word;

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
            zoal::utils::memory_segment<word, Address> memWord;
            memWord[self_type::TCNTx] = value;
            memWord.happyInspection();
        }

        static word counter() {
            zoal::utils::memory_segment<word, Address> memWord;
            return memWord[self_type::TCNTx];
        }

        template<uint8_t Channel>
        static word output_compare_value() {
            static_assert(Channel < channels_count, "Channel index is out of range");

            zoal::utils::memory_segment<word, Address> memWord;
            switch (Channel) {
            case 0:
                return memWord[self_type::OCRxA];
            case 1:
                return memWord[self_type::OCRxB];
            default:
                break;
            }

            return 0;
        }
    };
}}}

namespace zoal { namespace metadata {
    template<::zoal::periph::timer_mode Mode>
    struct timer_mode;
}}

#endif
