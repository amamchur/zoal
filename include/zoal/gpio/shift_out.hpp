#ifndef ZOAL_GPIO_SHIFT_OUT_HPP
#define ZOAL_GPIO_SHIFT_OUT_HPP

#include <stdint.h>
#include "../utils/nop.hpp"

namespace zoal { namespace gpio {
    template<uint64_t DataRiseDelay = 1, uint64_t ClockRiseDelay = 1>
    class shift_out_config {
    public:
        static constexpr uint64_t data_edge_rise_delay_ops = DataRiseDelay;
        static constexpr uint64_t clock_edge_rise_delay_ops = ClockRiseDelay;
    };

    template<class PinOut, class PinClk, class T, class Config = shift_out_config<>>
    void shift_out_lsbf(T value) {
        using namespace zoal::utils;

        auto bits = static_cast<uint8_t >(sizeof(T) << 3);
        auto mask = 1;
        for (auto i = 0; i < bits; i++) {
            uint8_t v = value & mask;
            PinOut::write(v);
            nop<Config::data_edge_rise_delay_ops>::place();
            PinClk::high();
            nop<Config::clock_edge_rise_delay_ops>::place();
            PinClk::low();
            value >>= 1;
        }
    }

    template<class PinOut, class PinClk, class T, class Config = shift_out_config<>>
    void shift_out_msbf(T value) {
        using namespace zoal::utils;

        auto bits = static_cast<uint8_t >(sizeof(T) << 3);
        auto mask = static_cast<uint8_t >(1u << (bits - 1));
        for (auto i = 0; i < bits; i++) {
            uint8_t v = value & mask;
            PinOut::write(v);
            nop<Config::data_edge_rise_delay_ops>::place();
            PinClk::high();
            nop<Config::clock_edge_rise_delay_ops>::place();
            PinClk::low();
            value <<= 1;
        }
    }
}}

#endif
