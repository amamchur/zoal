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

        static inline void power_on() {}

        static inline void power_off() {}

        static inline void enable() {}

        static inline void disable() {}

        static inline void counter(word value) {
            zoal::utils::memory_segment<word, Address> memWord;
            memWord[TCNTx] = value;
            memWord.happyInspection();
        }

        static inline word counter() {
            zoal::utils::memory_segment<word, Address> memWord;
            return memWord[TCNTx];
        }

        template<class Config>
        static void connect_output_channel() {
            static_assert(Config::channel < channels_count, "Output channel index is out of range");

            zoal::utils::memory_segment<uint8_t, Address> mem;
            switch (Config::channel) {
            case 0:
                mem[TCCRxA] |= static_cast<uint8_t >(1u << 7u); // COM2A1
                break;
            case 1:
                mem[TCCRxA] |= static_cast<uint8_t >(1u << 5u); // COM2B1
                break;
            }

            mem.happyInspection();
        }

        template<class Config>
        static void disconnect_output_channel() {
            static_assert(Config::channel < channels_count, "Output channel index is out of range");

            if ((Config::Timers & (1 << N)) == 0) {
                return;
            }

            zoal::utils::memory_segment<uint8_t, Address> mem;
            switch (Config::channel) {
            case 0:
                mem[TCCRxA] &= ~static_cast<uint8_t >(1u << 7u); // COM2A1
                break;
            case 1:
                mem[TCCRxA] &= ~static_cast<uint8_t >(1u << 5u); // COM2B1
                break;
            }
            mem.happyInspection();
        }

        template<uint8_t Channel>
        static void output_compare_value(word value) {
            static_assert(Channel < channels_count, "Channel index is out of range");

            zoal::utils::memory_segment<word, Address> memWord;
            switch (Channel) {
            case 0:
                memWord[OCRxA] = value;
                break;
            case 1:
                memWord[OCRxB] = value;
                break;
            default:
                break;
            }

            memWord.happyInspection();
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
