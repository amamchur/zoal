#ifndef ZOAL_IO_IR_REMOTE_RECEIVER_HPP
#define ZOAL_IO_IR_REMOTE_RECEIVER_HPP

#include "../gpio/api.hpp"
#include "../gpio/bit_order.hpp"
#include "../gpio/pin_mode.hpp"
#include "digital_sampler.hpp"

namespace zoal { namespace io {
    template<class Pin, uint32_t PeriodMicroseconds, class ResultType = uint32_t, zoal::gpio::bit_order Order = zoal::gpio::bit_order::msbf>
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
        static constexpr uint16_t threshold = (one_space_us - zero_space_us) / 2 / PeriodMicroseconds;

        using duration_type = uint16_t;
        using start_trigger = sampling_trigger<duration_type, trigger_mode::falling_edge, start_holdoff_iterations>;
        using stop_trigger = sampling_trigger<duration_type, trigger_mode::rising_edge, stop_holdoff_iterations>;
        using sample_type = sample<duration_type>;
        using digital_sampler_type = digital_sampler<start_trigger, stop_trigger>;
        using bit_op = zoal::gpio::push_bit_op<ResultType, Order>;

        using api = zoal::gpio::api;
        using gpio_cfg = api::optimize<
            //
            typename Pin::port::clock_on_cas,
            api::mode<zoal::gpio::pin_mode::input_floating, Pin>>;

        enum state { state_stopped, state_running, state_processed };

        void handle() {
            if (state != state_running) {
                return;
            }

            sample_type sample;
            sampling_result result = sampler.process(Pin::read(), sample);
            switch (result) {
            case sampling_result::sample:
                if (sample.signal == 1) {
                    data = bit_op::push(data, (sample.duration > zero_space_iterations + threshold) ? 1 : 0);
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

        void start() {
            state = state_running;
            data = 0;
            sampler.reset(Pin::read());
        }

        void stop() {
            state = state_stopped;
        }

        ResultType result() {
            return data;
        }

    private:
        state state{state_stopped};
        ResultType data{0};
        digital_sampler_type sampler;
    };
}}

#endif
