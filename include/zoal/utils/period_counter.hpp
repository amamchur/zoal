#ifndef ZOAL_UTILS_PERIOD_CALCULATOR_HPP
#define ZOAL_UTILS_PERIOD_CALCULATOR_HPP

#include <stdint.h>

namespace zoal { namespace utils {
    template<uint32_t TicksPerOverflow, uint32_t TimerTicksPerPeriod, uint32_t Depth>
    class period_precision_calculator {
    private:
        using next = period_precision_calculator<TicksPerOverflow - 1, TimerTicksPerPeriod, Depth - 1>;

        static constexpr uint32_t current_overflows_per_period = TimerTicksPerPeriod / TicksPerOverflow;
        static constexpr uint32_t current_precision =
                TimerTicksPerPeriod - current_overflows_per_period * TicksPerOverflow;

        static constexpr uint32_t next_overflows_per_period = next::overflows_per_period;
        static constexpr uint32_t next_precision = next::OverflowsCorrectionTicks;
    public:
        static constexpr uint32_t overflows_per_period = (current_precision < next::precision)
                                                         ? current_overflows_per_period
                                                         : next::overflows_per_period;
        static constexpr uint32_t precision = (current_precision < next::precision)
                                              ? current_precision
                                              : next::precision;
        static constexpr uint32_t ticks = (current_precision < next::precision)
                                          ? TicksPerOverflow
                                          : next::ticks;
    };

    template<uint32_t TicksPerOverflow, uint32_t TimerTicksPerPeriod>
    class period_precision_calculator<TicksPerOverflow, TimerTicksPerPeriod, 0> {
    public:
        static constexpr uint32_t overflows_per_period = 0xFFFFFFFF;
        static constexpr uint32_t ticks = 0xFFFFFFFF;
        static constexpr uint32_t precision = 0xFFFFFFFF;
    };

    template<class Timer, uint32_t Cycles, uint8_t Index, bool Final>
    class prescaler_calculator {
    private:
        using clock_source = typename Timer::ClockSource;
        using current_prescaler = typename clock_source::template Prescaler<Index>;

        static constexpr auto next_index = Index + 1;
        static constexpr bool next_final = (next_index + 1 == clock_source::Prescalers);
        using Next = prescaler_calculator<Timer, Cycles, Index + 1, next_final>;

        static constexpr auto cycles_per_timer_overflow =
                (uint32_t) (1 << Timer::Resolution) * current_prescaler::Value;
        static constexpr auto timer_overflow = Cycles / cycles_per_timer_overflow;
        static constexpr bool applicable = (Cycles < cycles_per_timer_overflow);
    public:
        static constexpr auto prescaler_index = applicable ? Index : Next::prescaler_index;
        using prescaler = typename clock_source::template Prescaler<prescaler_index>;
    };

    template<class Timer, uint32_t Cycles, uint8_t Index>
    class prescaler_calculator<Timer, Cycles, Index, true> {
    private:
        using clock_source = typename Timer::clock_source;
    public:
        using prescaler = typename clock_source::template Prescaler<Index>;
        static constexpr auto prescaler_index = Index;
    };

    template<class T, uint32_t Frequency, uint32_t PeriodMicroseconds>
    class period_calculator {
    public:
        using timer = T;
        using word = typename timer::word;

        static constexpr uint32_t cycles_per_microsecond = Frequency / 1000000;
        static constexpr uint32_t cycles_per_period = PeriodMicroseconds * cycles_per_microsecond;

        using prescaler_calc = prescaler_calculator<timer, cycles_per_period, 0, false>;
        using prescaler = typename prescaler_calc::prescaler;

        static constexpr uint32_t timer_ticks_per_second = Frequency / prescaler::Value;
        static constexpr uint32_t ticks_per_second = Frequency / prescaler::Value;
        static constexpr uint32_t timer_ticks_per_period = cycles_per_period / prescaler::Value;
        static constexpr uint32_t max_ticks_per_overflow = 1 << timer::Resolution;

        using precision_calculator = period_precision_calculator<max_ticks_per_overflow, timer_ticks_per_period, 128>;

        static constexpr uint32_t overflows_per_period = precision_calculator::overflows_per_period;
        static constexpr uint32_t ticks_per_overflow = precision_calculator::ticks;
        static constexpr uint32_t precision = precision_calculator::precision;
        static constexpr uint32_t timer_counter_value = max_ticks_per_overflow - ticks_per_overflow;
        static constexpr uint32_t real_period = ticks_per_overflow * overflows_per_period * prescaler::Value / cycles_per_microsecond;

        static void init_timer() {
            timer::template prescaler<prescaler_calc::prescaler_index>();
            timer::counter(timer_counter_value);
        }

        static void update_counter() {
            timer::counter(timer_counter_value);
        }

        period_calculator() : overflow(overflows_per_period) {
        }

        template<class Callback>
        void handle(Callback callback) {
            timer::counter(timer_counter_value);
            overflow--;
            if (overflow == 0) {
                overflow = overflows_per_period;
                callback();
            }
        }

    private:
        uint16_t overflow;
    };

    template<class T, uint32_t Frequency, uint32_t PeriodMicroseconds, uint16_t Prescaler, uint32_t TicksPerOverflow>
    class period_counter {
    public:
        static constexpr uint32_t cycles_per_microsecond = Frequency / 1000000;
        static constexpr uint32_t cycles_per_period = PeriodMicroseconds * cycles_per_microsecond;

        static constexpr uint32_t timer_ticks_per_period = cycles_per_period / Prescaler;
        static constexpr uint32_t cycles_per_overflow = TicksPerOverflow * Prescaler;

        static constexpr uint32_t overflows_per_period_lo = timer_ticks_per_period / TicksPerOverflow;
        static constexpr uint32_t overflows_per_period_hi = overflows_per_period_lo + 1;

        static constexpr uint32_t desirable_period = PeriodMicroseconds;
        static constexpr uint32_t real_period_lo = cycles_per_overflow * overflows_per_period_lo / cycles_per_microsecond;
        static constexpr uint32_t real_period_hi = cycles_per_overflow * overflows_per_period_hi / cycles_per_microsecond;
    private:
        uint16_t overflow;
    };
}}

#endif
