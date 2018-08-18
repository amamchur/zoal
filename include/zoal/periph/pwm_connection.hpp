#ifndef ZOAL_PERIPH_PWM_CONNECTION_HPP
#define ZOAL_PERIPH_PWM_CONNECTION_HPP

#include "../gpio/pin_mode.hpp"

namespace zoal { namespace periph {
    template<uint8_t Channel, uint32_t Extra = 0>
    class pwm_config {
    public:
        static constexpr uint8_t channel = Channel;
        static constexpr uint32_t extra = Extra;
    };

    template<class Pin, class Timer, uint8_t Channel, uint32_t Extra = 0>
    class pwm_connection {
    public:
        using pin = Pin;
        using timer = Timer;
        using config = pwm_config<Channel, Extra>;
        using word = typename timer::word;

        static constexpr uint8_t resolution = config::resolution;
        static constexpr uint16_t max_value = ((uint16_t) 1 << config::resolution) - 1;

        static inline void on() {
            pin::template mode<zoal::gpio::pin_mode::output>();
            timer::template connect_output_channel<config>();
        }

        static inline void off() {
            timer::template disconnect_output_channel<config>();
        }

        static inline void value(word value) {
            timer::template output_compare_value<config::channel>(value);
        }

        static inline word value() {
            return timer::template output_compare_value<config::channel>();
        }

        static inline void duty(float d) {
            word v = timer::topValue() * d;
            timer::template output_compare_value<config::Channel>(v);
        }
    };
}}

#endif
