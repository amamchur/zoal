#ifndef ZOAL_PERIPH_A2DC_CONN_HPP
#define ZOAL_PERIPH_A2DC_CONN_HPP

#include <stdint.h>

namespace zoal { namespace periph {
    template<uint8_t Channel, uint32_t Extra = 0>
    class adc_config {
    public:
        static constexpr uint8_t channel = Channel;
        static constexpr uint32_t extra = Extra;
    };

    template<class Pin, class Converter, uint8_t Channel, uint32_t Extra = 0>
    class adc_connection {
    public:
        using self_type = adc_connection<Pin, Converter, Channel, Extra>;
        using pin = Pin;
        using adc = Converter;
        using port = typename pin::port;

        static constexpr uint8_t channel = Channel;
        static constexpr uint32_t extra = Extra;
        static constexpr uint8_t resolution = adc::resolution;
        static constexpr uint16_t max_value = static_cast<const uint16_t>((1 << resolution) - 1);

        static void on() {
            port::analog_mode(pin::mask);
            adc::template connect<self_type>();
        }

        static void off() {
            adc::template disconnect<self_type>();
        }

        static uint16_t read() {
            return adc::read();
        }

        static float readf() {
            return (float) read() / max_value;
        }
    };
}}

#endif
