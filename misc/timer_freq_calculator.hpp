#ifndef ZOAL_UTILS_TIMER_FREQ_CALCULATOR_HPP
#define ZOAL_UTILS_TIMER_FREQ_CALCULATOR_HPP

namespace zoal { namespace misc {
    template<uint32_t DesirableFreq, uint32_t TimerFreq, uint32_t MaxTimerPeriod, uint32_t Prescaler, uint32_t... Prescalers>
    class timer_freq_calculator {
    public:
        static constexpr uint32_t desirable_freq = DesirableFreq;
        static constexpr uint32_t timer_freq = TimerFreq;
        static constexpr uint32_t max_timer_period = MaxTimerPeriod;
        static constexpr uint32_t current_prescaler = Prescaler;

        static constexpr double ns_per_tick = 1000.0 / (timer_freq / 1000000.0);
        static constexpr double total_ticks = 1000000000.0 / desirable_freq / ns_per_tick;
        static constexpr int32_t current_value = total_ticks / Prescaler - 1 > 0 ? total_ticks / Prescaler - 1 : 1;
        static constexpr double real_freq = static_cast<double>(timer_freq) / (current_value + 1) / Prescaler;

        using next = timer_freq_calculator<DesirableFreq, TimerFreq, MaxTimerPeriod, Prescalers...>;
        static constexpr double delta_freq = desirable_freq - real_freq;
        static constexpr double delta_freq_abs = delta_freq >= 0 ? delta_freq : -delta_freq;

        static constexpr bool current_valid = current_value < max_timer_period && current_value > 0;
        static constexpr bool delta_freq_better = next::delta_freq_abs < delta_freq_abs;
        static constexpr bool delta_freq_good = next::delta_freq_abs <= delta_freq_abs;
        static constexpr bool bigger_prescale = (next::current_prescaler > current_prescaler);
        static constexpr bool next_better = !current_valid || delta_freq_better || (bigger_prescale && delta_freq_good);

        static constexpr uint32_t prescaler = next_better ? next::prescaler : Prescaler;
        static constexpr uint32_t compare_value = next_better ? next::compare_value : current_value;
    };

    template<uint32_t DesirableFreq, uint32_t TimerFreq, uint32_t MaxTimerPeriod, uint32_t Prescaler>
    class timer_freq_calculator<DesirableFreq, TimerFreq, MaxTimerPeriod, Prescaler> {
    public:
        static constexpr uint32_t desirable_freq = DesirableFreq;
        static constexpr uint32_t timer_freq = TimerFreq;
        static constexpr uint32_t max_timer_period = MaxTimerPeriod;
        static constexpr uint32_t current_prescaler = Prescaler;

        static constexpr double ns_per_tick = 1000.0 / (timer_freq / 1000000.0);
        static constexpr double total_ticks = 1000000000.0 / desirable_freq / ns_per_tick;
        static constexpr int32_t current_value = total_ticks / current_prescaler - 1;
        static constexpr bool valid = current_value > 0;

        static constexpr uint32_t prescaler = Prescaler;
        static constexpr uint32_t compare_value = current_value;
        static constexpr double real_freq = static_cast<double>(timer_freq) / (compare_value + 1) / prescaler;

        using next = void;
        static constexpr double delta_freq = desirable_freq - real_freq;
        static constexpr double delta_freq_abs = delta_freq >= 0 ? delta_freq : -delta_freq;
    };
}}

#endif
