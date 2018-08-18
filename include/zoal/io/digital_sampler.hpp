#ifndef ZOAL_IO_DIGITAL_SAMPLER_HPP
#define ZOAL_IO_DIGITAL_SAMPLER_HPP

#include <stdint.h>

namespace zoal { namespace io {
    enum class signal_transition {
        none,
        falling_edge,
        rising_edge
    };

    enum class trigger_mode {
        falling_edge,
        rising_edge,
        logical_change
    };

    template<class Duration, trigger_mode Mode, Duration Holdoff>
    struct samling_trigger {
        typedef Duration duration_type;

        static constexpr trigger_mode mode = Mode;
        static constexpr Duration holdoff = Holdoff;
    };

    template<trigger_mode mode>
    struct trigger_condition {
        static bool matched(signal_transition transition) {
            return false;
        }
    };

    template<>
    struct trigger_condition<trigger_mode::falling_edge> {
        static bool matched(signal_transition transition) {
            return transition == signal_transition::falling_edge;
        }
    };

    template<>
    struct trigger_condition<trigger_mode::rising_edge> {
        static bool matched(signal_transition transition) {
            return transition == signal_transition::rising_edge;
        }
    };

    template<>
    struct trigger_condition<trigger_mode::logical_change> {
        static bool matched(signal_transition transition) {
            return transition == signal_transition::none;
        }
    };

    template<class Duration, uint8_t Level, Duration Holdoff>
    struct trigger {
        typedef Duration duration_type;

        static constexpr uint8_t level = Level;
        static constexpr Duration holdoff = Holdoff;
    };

    template<class Duration>
    struct sample {
        typedef Duration duration_type;

        uint8_t signal;
        duration_type duration;
    };

    enum class sampling_result {
        idle,
        working,
        sample,
        finished,
        overflow
    };

    template<class Pin, class StartTrigger, class StopTrigger>
    class digital_sampler {
    public:
        using pin = Pin;
        using duration_type = typename StartTrigger::duration_type;
        using start_condition = trigger_condition<StartTrigger::mode>;
        using stop_condition = trigger_condition<StopTrigger::mode>;

        static_assert(duration_type(0) < duration_type(-1), "DurationType is signed");

        enum : uint8_t {
            idle,
            start_trigger_level,
            recording
        };

        digital_sampler() {
            reset();
        }

        void reset() {
            transition = signal_transition::none;
            state = 0;
            time = 0;
            value = pin::read();
        }

        static signal_transition get_signal_transitoin(uint8_t from, uint8_t to) {
            if (from == 1 && to == 0) {
                return signal_transition::falling_edge;
            }

            if (from == 0 && to == 1) {
                return signal_transition::rising_edge;
            }
            return signal_transition::none;
        }

        template<class Sample>
        sampling_result process(Sample &sample) {
            if (++time <= 0) {
                reset();
                return sampling_result::overflow;
            }

            uint8_t signal = pin::read();
            signal_transition current = get_signal_transitoin(value, signal);
            if (current != signal_transition::none && current != transition) {
                transition = current;
            }

            switch (state) {
                case idle:
                    time = 0;

                    if (!start_condition::matched(transition)) {
                        time = 0;
                        return sampling_result::idle;
                    }

                    state = start_trigger_level;
                    return sampling_result::working;
                case start_trigger_level:
                    if (time < StartTrigger::Holdoff) {
                        return sampling_result::working;
                    }

                    if (!start_condition::matched(transition)) {
                        reset();
                        return sampling_result::working;
                    }

                    value = signal;
                    state = recording;
                    return sampling_result::working;
                case recording:
                    if (value != signal) {
                        sample.signal = value;
                        sample.duration = time;

                        time = 0;
                        value = signal;
                        return sampling_result::sample;
                    }

                    if (time < StopTrigger::Holdoff) {
                        return sampling_result::working;
                    }

                    if (stop_condition::matched(transition)) {
                        sample.signal = value;
                        sample.duration = time;
                        reset();
                        return sampling_result::finished;
                    }

                    return sampling_result::working;
            }

            return sampling_result::working;
        }

        template<class Sample>
        void get_sample(Sample &sample) {
            sample.signal = value;
            sample.duration = time;
        }

    private:
        signal_transition transition;
        uint8_t state;
        uint8_t value;
        duration_type time;
    };
}}

#endif
