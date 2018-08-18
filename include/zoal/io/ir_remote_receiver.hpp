#ifndef ZOAL_IO_IR_REMOTE_RECEIVER_HPP
#define ZOAL_IO_IR_REMOTE_RECEIVER_HPP

#include "../gpio/pin_mode.hpp"
#include "digital_sampler.hpp"

namespace zoal { namespace io {
    template<class Pin, uint32_t PeriodMicroseconds, class ResultType = uint32_t>
    class ir_remote_receiver {
    public:
        static constexpr uint16_t start_holdoff_us = 5000; //us
        static constexpr uint16_t stop_holdoff_us = 10000; //us
        static constexpr uint16_t zero_space_us = 550; //us
        static constexpr uint16_t one_space_us = 1600; //us

        static constexpr uint16_t start_holdoff_iterations = start_holdoff_us / PeriodMicroseconds;
        static constexpr uint16_t stop_holdoff_iterations = stop_holdoff_us / PeriodMicroseconds;
        static constexpr uint16_t zero_space_iterations = zero_space_us / PeriodMicroseconds;
        static constexpr uint16_t one_space_iterations = one_space_us / PeriodMicroseconds;
        static constexpr uint16_t threshold = zero_space_iterations + 200 / PeriodMicroseconds;

        typedef uint16_t duration_type;
        typedef samling_trigger<duration_type, trigger_mode::falling_edge, start_holdoff_iterations> start_trigger;
        typedef samling_trigger<duration_type, trigger_mode::rising_edge, stop_holdoff_iterations> stop_trigger;
        typedef sample<duration_type> sample_type;
        typedef digital_sampler<Pin, start_trigger, stop_trigger> digital_sampler_type;

        enum state {
            state_stopped,
            state_running,
            state_processed
        };

        void begin() {
            using namespace zoal::gpio;
            Pin::template mode<pin_mode::input>();
            run();
        }

        void handle() {
            if (state != state_running) {
                return;
            }

            sample_type sample;
            sampling_result result = sampler.process(sample);
            switch (result) {
                case sampling_result::sample:
                    if (sample.signal == 1) {
                        data <<= 1;
                        data |= (sample.duration > zero_space_iterations + 10) ? 1 : 0;
                    }
                    break;
                case sampling_result::finished:
                    state = state_processed;
                    break;
                default:
                    break;
            }
        }

        bool processed() {
            return state == state_processed;
        }

        void run() {
            state = state_running;
            data = 0;
        }

        void stop() {
            state = state_stopped;
        }

        ResultType result() {
            return data;
        }

    private:
        state state;
        ResultType data;
        digital_sampler_type sampler;
    };
}}

#endif
