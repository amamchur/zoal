#ifndef ZOAL_UTILS_TICKS_CALCULATOR_HPP
#define ZOAL_UTILS_TICKS_CALCULATOR_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace utils {
    template<class Timer, uint32_t Value, uint8_t Index = 0, bool Final = false>
    class prescaler_le {
    private:
        typedef typename Timer::clock_source clock_source;
        typedef typename clock_source::template PrescalerMode<Index> prescaler;

        static constexpr auto next_index = Index + 1;
        static constexpr bool next_final = next_index + 1 == clock_source::prescaler_modes;
    public:
        typedef prescaler_le<Timer, Value, Index + 1, next_final> next;

        static constexpr bool applicable = prescaler::value <= Value;
        static constexpr auto result_index = next::applicable ? next::result_index : Index;
        typedef typename clock_source::template PrescalerMode<result_index> result;
    };

    template<class Timer, uint32_t Value, uint8_t Index>
    class prescaler_le<Timer, Value, Index, true> {
    private:
        typedef typename Timer::clock_source clock_source;
        typedef typename clock_source::template PrescalerMode<Index> prescaler;
    public:
        static constexpr bool applicable = prescaler::value <= Value;
        static constexpr auto result_index = Index;
        typedef typename clock_source::template PrescalerMode<result_index> result;
    };

    template<class Timer, uint32_t Value, uint8_t Index = 0, bool Final = false>
    class prescaler_ge {
    private:
        typedef typename Timer::ClockSource clock_source;
        typedef typename clock_source::template PrescalerMode<Index> prescaler;

        static constexpr auto next_index = Index + 1;
        static constexpr bool next_final = next_index + 1 == clock_source::prescaler_modes;
    public:
        typedef prescaler_ge<Timer, Value, Index + 1, next_final> Next;

        static constexpr bool applicable = prescaler::value >= Value;
        static constexpr auto result_index = applicable ? Index : Next::result_index;
        typedef typename clock_source::template PrescalerMode<result_index> result;
    };

    template<class Timer, uint32_t Value, uint8_t Index>
    class prescaler_ge<Timer, Value, Index, true> {
    private:
        typedef typename Timer::ClockSource clock_source;
        typedef typename clock_source::template PrescalerMode<Index> prescaler;
    public:
        static constexpr bool applicable = prescaler::Value >= Value;
        static constexpr auto result_index = Index;
        typedef typename clock_source::template PrescalerMode<result_index> result;
    };
}}

#endif
