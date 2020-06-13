#pragma once

#ifndef ZOAL_SHIELD_UNO_LCD_HPP
#define ZOAL_SHIELD_UNO_LCD_HPP

#include "../ct/helpers.hpp"
#include "../gpio/pin_mode.hpp"
#include "../ic/hd44780.hpp"
#include "../io/analog_keypad.hpp"
#include "../utils/helpers.hpp"
#include "../utils/nop.hpp"

#include <stdint.h>

namespace zoal { namespace shield {
    template<class PinA = void, class PinB = void>
    class uno_lcd_shield_config {
    public:
        using calibration_pin_a = PinA;
        using calibration_pin_b = PinB;
    };

    template<class Tools, class Board, class Adc, class Cfg = uno_lcd_shield_config<>>
    class uno_lcd {
    public:
        static const size_t select_btn = 0;
        static const size_t left_btn = 1;
        static const size_t down_btn = 2;
        static const size_t up_btn = 3;
        static const size_t right_btn = 4;

        using tools = Tools;
        using mcu = typename Board::mcu;
        using logger = typename Tools::logger;

        using interface_type = zoal::ic::hd44780_interface_4bit<Tools,
                                                                typename Board::ard_d08,
                                                                typename Board::ard_d09,
                                                                typename Board::ard_d04,
                                                                typename Board::ard_d05,
                                                                typename Board::ard_d06,
                                                                typename Board::ard_d07>;
        using address_selector = zoal::ic::hd44780_address_selector<16, 2>;
        using lcd = zoal::ic::hd44780<interface_type, address_selector>;

        template<class T, class U>
        using opt = zoal::ct::optional_type<T, U>;
        using calibration_pin_a = typename opt<typename Cfg::calibration_pin_a, typename Board::ard_d02>::type;
        using calibration_pin_b = typename opt<typename Cfg::calibration_pin_b, typename Board::ard_d03>::type;

        static constexpr uint8_t button_count = 5;
        using analog_pin = typename Board::ard_a00;
        using adc = Adc;
        using keypad = zoal::io::analog_keypad<Tools, button_count>;

        static void init() {
            lcd::init();
        }

        static void calibrate(bool force) {
            using namespace zoal::gpio;

            logger::trace() << "Buttons ADC Values: " << keypad::values[0] << " " << keypad::values[1] << " "
                            << keypad::values[2] << " " << keypad::values[3] << " " << keypad::values[4] << " ";

            uint16_t value = 0;
            for (int i = 0; i < 4; i++) {
                calibration_pin_a::low();
                zoal::utils::nop<3>::place();
                value = value << 1u | calibration_pin_b::read();

                calibration_pin_a::high();
                zoal::utils::nop<3>::place();
                value = value << 1u | calibration_pin_b::read();
            }

            logger::trace() << "value:" << value << " force: " << ((int)force);
            if (value != 85 && !force) {
                return;
            }

            mcu::mux::template adc<adc, analog_pin>::on();
            keypad::values[select_btn] = calibrate_button("SELECT");
            keypad::values[left_btn] = calibrate_button("LEFT");
            keypad::values[down_btn] = calibrate_button("DOWN");
            keypad::values[up_btn] = calibrate_button("UP");
            keypad::values[right_btn] = calibrate_button("RIGHT");

            logger::trace() << "Calibrated: " << keypad::values[0] << " " << keypad::values[1] << " "
                            << keypad::values[2] << " " << keypad::values[3] << " " << keypad::values[4] << " ";
            lcd::clear();
        }

        static uint16_t read_adc() {
            uint16_t result = 1 << 12;
            do {
                uint16_t value = adc::read();
                result = result > value ? value : result;

                int diff = static_cast<int>(result) - value;
                if (diff < -50) {
                    break;
                }
            } while (true);

            return result;
        }

        static uint16_t calibrate_button(const char *name) {
            lcd::clear();
            lcd::write("Calibration");
            Tools::delay::ms(100);
            lcd::move(1, 0);
            lcd::write("Press ");
            lcd::write(name);
            return read_adc();
        }

        template<class H>
        static void handle_keypad(H handler) {
            mcu::mux::template adc<adc, analog_pin>::on();
            handle_keypad(handler, (int16_t)adc::read());
        }

        template<class H>
        static void handle_keypad(H handler, int16_t channelValue) {
            keypad::handle(handler, channelValue);
        }
    };
}}

#endif
