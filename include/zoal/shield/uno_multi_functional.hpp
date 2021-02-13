#ifndef ZOAL_SHIELD_UNO_MULTI_FUNCTIONAL_HPP
#define ZOAL_SHIELD_UNO_MULTI_FUNCTIONAL_HPP

#include "../data/segment7.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/shift_out.hpp"
#include "../io/button.hpp"

namespace zoal { namespace shield {
    template<class UnoCompatibleBoard, class TimeType>
    class uno_multi_functional {
    public:
        using button_0_pin  = typename UnoCompatibleBoard::ard_a01;
        using button_1_pin  = typename UnoCompatibleBoard::ard_a02;
        using button_2_pin  = typename UnoCompatibleBoard::ard_a03;
        using led_0 = ::zoal::gpio::active_drain<typename UnoCompatibleBoard::ard_d13>;
        using led_1 = ::zoal::gpio::active_drain<typename UnoCompatibleBoard::ard_d12>;
        using led_2 = ::zoal::gpio::active_drain<typename UnoCompatibleBoard::ard_d11>;
        using led_3 = ::zoal::gpio::active_drain<typename UnoCompatibleBoard::ard_d10>;
        using beeper = ::zoal::gpio::active_drain<typename UnoCompatibleBoard::ard_d03>;
        using shift_reg_latch = typename UnoCompatibleBoard::ard_d04;
        using shift_reg_clk = typename UnoCompatibleBoard::ard_d07;
        using shift_reg_data = typename UnoCompatibleBoard::ard_d08;

        using gpio_cfg = zoal::gpio::api::optimize<
            //
            typename led_0::gpio_cfg,
            typename led_1::gpio_cfg,
            typename led_2::gpio_cfg,
            typename led_3::gpio_cfg,
            typename beeper::gpio_cfg,
            // Shield already contains pull-up resistors. Keep input floating to minimize current consumption.
            zoal::gpio::api::mode<zoal::gpio::pin_mode::input_floating, button_0_pin, button_1_pin, button_2_pin>,
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output_push_pull, shift_reg_latch, shift_reg_clk, shift_reg_data>,
            zoal::gpio::api::low<shift_reg_latch, shift_reg_clk, shift_reg_data>>;

        void dec_to_segments(uint32_t value) {
            segments[0] = 0xFF;
            segments[1] = 0xFF;
            segments[2] = 0xFF;
            segments[3] = ~::zoal::data::segment7::abcd_hex(0);

            for (int i = 3; value > 0 && i >= 0; i--) {
                uint16_t v = value % 10;
                segments[i] = ~::zoal::data::segment7::abcd_hex(v);
                value /= 10;
            }
        }

        void hex_to_segments(uint16_t value) {
            uint16_t v = value;
            for (int i = 3; i >= 0; i--) {
                auto d = static_cast<uint8_t>(v & 0x0Fu);
                segments[i] = ~::zoal::data::segment7::abcd_hex(d);
                v >>= 4;
            }
        }

        void dynamic_indication() {
            auto segmentMask = static_cast<uint8_t>(1u << segment_index);
            auto segmentValue = segments[segment_index];

            typename shift_reg_latch::low();
            ::zoal::gpio::shift_out_msbf<shift_reg_data, shift_reg_clk>(segmentValue);
            ::zoal::gpio::shift_out_msbf<shift_reg_data, shift_reg_clk>(segmentMask);
            typename shift_reg_latch::high();

            segment_index = static_cast<uint8_t>((segment_index + 1) & 0x3);
        }

        template<class H>
        void handle_buttons(TimeType time, H button_handler) {
            button_0.handle(time, button_handler, static_cast<uint8_t>(0));
            button_1.handle(time, button_handler, static_cast<uint8_t>(1));
            button_2.handle(time, button_handler, static_cast<uint8_t>(2));
        }

        uint8_t segments[4]{0, 0, 0, 0};
        uint8_t segment_index{0};

        zoal::io::button<TimeType, button_0_pin> button_0;
        zoal::io::button<TimeType, button_1_pin> button_1;
        zoal::io::button<TimeType, button_2_pin> button_2;
    };
}}

#endif
