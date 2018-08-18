#ifndef ZOAL_UTILS_TICKS_CALCULATOR_HPP
#define ZOAL_UTILS_TICKS_CALCULATOR_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace utils {
    template<class Timer, uint32_t Value, uint8_t Index = 0, bool Final = false>
    class prescaler_le {
    private:
        using clock_source = typename Timer::clock_source;
        using prescaler = typename clock_source::template prescaler_mode<Index>;

        static constexpr auto next_index = Index + 1;
        static constexpr bool next_final = next_index + 1 == clock_source::prescaler_modes;
    public:
        using next = prescaler_le<Timer, Value, Index + 1, next_final>;

        static constexpr bool applicable = prescaler::value <= Value;
        static constexpr auto result_index = next::applicable ? next::result_index : Index;
        using result = typename clock_source::template prescaler_mode<result_index>;
    };

    template<class Timer, uint32_t Value, uint8_t Index>
    class prescaler_le<Timer, Value, Index, true> {
    private:
        using clock_source = typename Timer::clock_source;
        using prescaler = typename clock_source::template prescaler_mode<Index>;
    public:
        static constexpr bool applicable = prescaler::value <= Value;
        static constexpr auto result_index = Index;
        using result = typename clock_source::template prescaler_mode<result_index>;
    };

    template<class Timer, uint32_t Value, uint8_t Index = 0, bool Final = false>
    class prescaler_ge {
    private:
        using clock_source = typename Timer::ClockSource;
        using prescaler = typename clock_source::template prescaler_mode<Index>;

        static constexpr auto next_index = Index + 1;
        static constexpr bool next_final = next_index + 1 == clock_source::prescaler_modes;
    public:
        using next = prescaler_le<Timer, Value, Index + 1, next_final>;

        static constexpr bool applicable = prescaler::value >= Value;
        static constexpr auto result_index = applicable ? Index : next::result_index;
        using result = typename clock_source::template prescaler_mode<result_index>;
    };

    template<class Timer, uint32_t Value, uint8_t Index>
    class prescaler_ge<Timer, Value, Index, true> {
    private:
        using clock_source = typename Timer::ClockSource;
        using prescaler = typename clock_source::template prescaler_mode<Index>;
    public:
        static constexpr bool applicable = prescaler::Value >= Value;
        static constexpr auto result_index = Index;
        using result = typename clock_source::template prescaler_mode<result_index>;
    };
}}

#endif
