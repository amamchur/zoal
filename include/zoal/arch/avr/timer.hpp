#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_HPP

#include <stdint.h>
#include "../../periph/timer_mode.hpp"
#include "../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class TimerModel, uintptr_t Address, class TIFRs, class TIMRs, class ClockSource, uint8_t N>
    class timer {
    public:
        using self_type = timer<TimerModel, Address, TIFRs, TIMRs, ClockSource, N>;
        using model = TimerModel;
        using word = typename model::word;
        using clock_source = ClockSource;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t channels_count = 2;
        static constexpr uint8_t resolution = sizeof(word) * 8;

        timer() = delete;

        template<class Clock>
        static inline void select_clock_source() {
            zoal::utils::memory_segment<uint8_t, Address> mem;
            mem[model::TCCRxB] = (mem[model::TCCRxB] & ~static_cast<uint8_t >(7u)) | Clock::mask;;
            mem.happyInspection();
        }

        static inline void counter(word value) {
            zoal::utils::memory_segment<word, Address> memWord;
            memWord[model::TCNTx] = value;
            memWord.happyInspection();
        }

        static inline word counter() {
            zoal::utils::memory_segment<word, Address> memWord;
            return memWord[model::TCNTx];
        }

        static inline void enable_overflow_interrupt() {
            TIMRs::template enable_overflow_interrupt<no>();
        }

        static inline void disable_overflow_interrupt() {
            TIMRs::template disable_overflow_interrupt<no>();
        }

        static inline void clear_overflow_interrupt_flag() {
            TIFRs::template clear_counter_flag<no>();
        }

        template<uint8_t channel>
        static inline void enable_compare_match_interrupt() {
            static_assert(channel < channels_count, "Channel index is out of range");
            TIMRs::template enable_compare_match_interrupt<no, channel>();
        }

        template<uint8_t channel>
        static inline void disable_compare_match_nterrupt() {
            static_assert(channel < channels_count, "Channel index is out of range");
            TIMRs::template disable_compare_match_interrupt<no, channel>();
        }

        template<uint8_t channel>
        static inline void clear_channel_interrupt_flag() {
            static_assert(channel < channels_count, "Channel index is out of range");
            TIFRs::template clear_channel_flag<no, channel>();
        }

        template<zoal::periph::timer_mode TimerMode>
        static inline void mode() {
            model::template mode<self_type, TimerMode>();
        }

        static void reset() {
            disable_overflow_interrupt();
            disable_compare_match_nterrupt<0>();
            disable_compare_match_nterrupt<1>();

            clear_overflow_interrupt_flag();
            clear_channel_interrupt_flag<0>();
            clear_channel_interrupt_flag<1>();

            mode<zoal::periph::timer_mode::normal>();

            counter(0);
            output_compare_value<0>(0);
            output_compare_value<1>(0);
        }

        template<class Config>
        static void connect_output_channel() {
            static_assert(Config::channel < channels_count, "Output channel index is out of range");

            zoal::utils::memory_segment<uint8_t, Address> mem;
            switch (Config::channel) {
                case 0:
                    mem[model::TCCRxA] |= static_cast<uint8_t >(1u << 7u); // COM2A1
                    break;
                case 1:
                    mem[model::TCCRxA] |= static_cast<uint8_t >(1u << 5u); // COM2B1
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
                    mem[model::TCCRxA] &= ~static_cast<uint8_t >(1u << 7u); // COM2A1
                    break;
                case 1:
                    mem[model::TCCRxA] &= ~static_cast<uint8_t >(1u << 5u); // COM2B1
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
                    memWord[model::OCRxA] = value;
                    break;
                case 1:
                    memWord[model::OCRxB] = value;
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
                    return memWord[model::OCRxA];
                case 1:
                    return memWord[model::OCRxB];
                default:
                    break;
            }

            return 0;
        }
    };
}}}

#endif
