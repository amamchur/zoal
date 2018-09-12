#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_HPP

#include <stdint.h>
#include "../../periph/timer_mode.hpp"
#include "../../utils/memory_segment.hpp"
#include "../bus.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class TimerModel, uintptr_t Address, uint8_t N>
    class timer {
    public:
        using self_type = timer<TimerModel, Address, N>;
        using model = TimerModel;
        using word = typename model::word;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t channels_count = 2;
        static constexpr uint8_t resolution = sizeof(word) * 8;

        static constexpr uintptr_t TCCRxA = model::TCCRxA;
        static constexpr uintptr_t TCCRxB = model::TCCRxB;
        static constexpr uintptr_t TCNTx = model::TCNTx;
        static constexpr uintptr_t OCRxA = model::OCRxA;
        static constexpr uintptr_t OCRxB = model::OCRxB;

        timer() = delete;

        static void power_on() {}

        static void power_off() {}

        static void enable() {}

        static void disable() {}

        static void counter(word value) {
            zoal::utils::memory_segment<word, Address> memWord;
            memWord[TCNTx] = value;
            memWord.happyInspection();
        }

        static word counter() {
            zoal::utils::memory_segment<word, Address> memWord;
            return memWord[TCNTx];
        }

        template<uint8_t Channel>
        static word output_compare_value() {
            static_assert(Channel < channels_count, "Channel index is out of range");

            zoal::utils::memory_segment<word, Address> memWord;
            switch (Channel) {
            case 0:
                return memWord[OCRxA];
            case 1:
                return memWord[OCRxB];
            default:
                break;
            }

            return 0;
        }
    };
}}}

#endif
