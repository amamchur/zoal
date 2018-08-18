#ifndef ZOAL_UTILS_PERIOD_CALCULATOR_HPP
#define ZOAL_UTILS_PERIOD_CALCULATOR_HPP

#include <stdint.h>

namespace zoal { namespace utils {
    template<uint32_t TicksPerOverflow, uint32_t TimerTicksPerPeriod, uint32_t Depth>
    class PeriodPrecisionCalculator {
    private:
        typedef PeriodPrecisionCalculator<TicksPerOverflow - 1, TimerTicksPerPeriod, Depth - 1> next;

        static constexpr uint32_t CurrentOverflowsPerPeriod = TimerTicksPerPeriod / TicksPerOverflow;
        static constexpr uint32_t CurrentPrecision = TimerTicksPerPeriod - CurrentOverflowsPerPeriod * TicksPerOverflow;

        static constexpr uint32_t NextOverflowsPerPeriod = next::OverflowsPerPeriod;
        static constexpr uint32_t NextPrecision = next::OverflowsCorrectionTicks;
    public:
        static constexpr uint32_t OverflowsPerPeriod = (CurrentPrecision < next::Precision)
                                                       ? CurrentOverflowsPerPeriod
                                                       : next::OverflowsPerPeriod;
        static constexpr uint32_t Precision = (CurrentPrecision < next::Precision)
                                              ? CurrentPrecision
                                              : next::Precision;
        static constexpr uint32_t Ticks = (CurrentPrecision < next::Precision)
                                          ? TicksPerOverflow
                                          : next::Ticks;
    };

    template<uint32_t TicksPerOverflow, uint32_t TimerTicksPerPeriod>
    class PeriodPrecisionCalculator<TicksPerOverflow, TimerTicksPerPeriod, 0> {
    public:
        static constexpr uint32_t OverflowsPerPeriod = 0xFFFFFFFF;
        static constexpr uint32_t Ticks = 0xFFFFFFFF;
        static constexpr uint32_t Precision = 0xFFFFFFFF;
    };

    template<class Timer, uint32_t cycles, uint8_t index, bool final>
    class PrescalerCalculator {
    private:
        typedef typename Timer::ClockSource ClockSource;
        typedef typename ClockSource::template Prescaler<index> CurrentPrescaler;

        static constexpr auto NextIndex = index + 1;
        static constexpr bool NextFinal = (NextIndex + 1 == ClockSource::Prescalers);
        typedef PrescalerCalculator<Timer, cycles, index + 1, NextFinal> Next;

        static constexpr auto CyclesPerTimerOverflow = (uint32_t) (1 << Timer::Resolution) * CurrentPrescaler::Value;
        static constexpr auto TimerOverflow = cycles / CyclesPerTimerOverflow;
        static constexpr bool Applicable = (cycles < CyclesPerTimerOverflow);
    public:
        static constexpr auto PrescalerIndex = Applicable ? index : Next::PrescalerIndex;
        typedef typename ClockSource::template Prescaler<PrescalerIndex> Prescaler;
    };

    template<class Timer, uint32_t cycles, uint8_t index>
    class PrescalerCalculator<Timer, cycles, index, true> {
    private:
        typedef typename Timer::ClockSource ClockSource;
    public:
        typedef typename ClockSource::template Prescaler<index> Prescaler;
        static constexpr auto PrescalerIndex = index;
    };

    template<class T, uint32_t freq, uint32_t periodMicroseconds>
    class PeriodCalculator {
    public:
        typedef T Timer;
        typedef typename Timer::Word Type;

        static constexpr uint32_t CyclesPerMicrosecond = freq / 1000000;
        static constexpr uint32_t CyclesPerPeriod = periodMicroseconds * CyclesPerMicrosecond;

        typedef PrescalerCalculator<Timer, CyclesPerPeriod, 0, false> PrescalerCalc;
        typedef typename PrescalerCalc::Prescaler Prescaler;

        static constexpr uint32_t TimerTicksPerSecond = freq / Prescaler::Value;
        static constexpr uint32_t TicksPerSecond = freq / Prescaler::Value;
        static constexpr uint32_t TimerTicksPerPeriod = CyclesPerPeriod / Prescaler::Value;
        static constexpr uint32_t MaxTicksPerOverflow = 1 << Timer::Resolution;

        typedef PeriodPrecisionCalculator<MaxTicksPerOverflow, TimerTicksPerPeriod, 128> PrecisionCalculator;

        static constexpr uint32_t OverflowsPerPeriod = PrecisionCalculator::OverflowsPerPeriod;
        static constexpr uint32_t TicksPerOverflow = PrecisionCalculator::Ticks;
        static constexpr uint32_t Precision = PrecisionCalculator::Precision;
        static constexpr uint32_t TimerCounterValue = MaxTicksPerOverflow - TicksPerOverflow;
        static constexpr uint32_t RealPeriod = TicksPerOverflow * OverflowsPerPeriod * Prescaler::Value / CyclesPerMicrosecond;

        static void initTimer() {
            Timer::template prescaler<PrescalerCalc::PrescalerIndex>();
            Timer::counter(TimerCounterValue);
        }

        static void updateCounter() {
            Timer::counter(TimerCounterValue);
        }

        PeriodCalculator() : overflow(OverflowsPerPeriod) {
        }

        template<class Callback>
        void handle(Callback callback) {
            Timer::counter(TimerCounterValue);
            overflow--;
            if (overflow == 0) {
                overflow = OverflowsPerPeriod;
                callback();
            }
        }

    private:
        uint16_t overflow;
    };

    template<class T, uint32_t freq, uint32_t periodMicroseconds, uint16_t prescaler, uint32_t ticksPerOverflow>
    class period_counter {
    public:
        static constexpr uint32_t CyclesPerMicrosecond = freq / 1000000;
        static constexpr uint32_t CyclesPerPeriod = periodMicroseconds * CyclesPerMicrosecond;

        static constexpr uint32_t TimerTicksPerPeriod = CyclesPerPeriod / prescaler;
        static constexpr uint32_t CyclesPerOverflow = ticksPerOverflow * prescaler;

        static constexpr uint32_t OverflowsPerPeriodLo = TimerTicksPerPeriod / ticksPerOverflow;
        static constexpr uint32_t OverflowsPerPeriodHi = OverflowsPerPeriodLo + 1;

        static constexpr uint32_t DesirablePeriod = periodMicroseconds;
        static constexpr uint32_t RealPeriodLo = CyclesPerOverflow * OverflowsPerPeriodLo / CyclesPerMicrosecond;
        static constexpr uint32_t RealPeriodHi = CyclesPerOverflow * OverflowsPerPeriodHi / CyclesPerMicrosecond;
    private:
        uint16_t overflow;
    };
}}

#endif
