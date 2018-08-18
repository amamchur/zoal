#pragma once

#ifndef ZOAL_SHIELDS_UNO_LCD_SHIELD_HPP
#define ZOAL_SHIELDS_UNO_LCD_SHIELD_HPP

#include <stdint.h>
#include "../gpio/pin_mode.hpp"
#include "zoal/ic/hd44780.hpp"
#include "../io/analog_keypad.hpp"
#include "../utils/helpers.hpp"
#include "../utils/nop.hpp"

namespace zoal { namespace shields {
    template<class PinA = void, class PinB = void>
    class uno_lcd_shield_config {
    public:
        using calibration_pin_a = PinA;
        using calibration_pin_b = PinB;
    };

    template<class Tools, class Board, class Cfg = uno_lcd_shield_config<>>
    class uno_lcd_shield {
    public:
        using interface_type = zoal::ic::hd44780_interface_4bit<Tools,
                typename Board::ard_d08,
                typename Board::ard_d09,
                typename Board::ard_d04,
                typename Board::ard_d05,
                typename Board::ard_d06,
                typename Board::ard_d07
        >;

        template<class T, class U>
        using Optional = zoal::utils::optional_type<T, U>;

        using calibration_pin_a = typename Optional<typename Cfg::calibration_pin_a, typename Board::ard_d02>::type;
        using calibration_pin_b = typename Optional<typename Cfg::calibration_pin_b, typename Board::ard_d03>::type;
        using logger = typename Tools::logger;

        using analog_channel = typename Board::ard_a00_adc;

        using address_selector = zoal::ic::hd44780_address_selector<16, 2>;
        using lcd = zoal::ic::hd44780<interface_type, address_selector>;

        static constexpr uint8_t button_count = 5;

        using keypad = zoal::io::analog_keypad<Tools, analog_channel, button_count>;

        static void init() {
            keypad::init();
            lcd::init();
        }

        static void calibrate(bool force) {
            using namespace zoal::gpio;

            logger::trace()
                    << "Buttons ADC Values: "
                    << keypad::values[0] << " "
                    << keypad::values[1] << " "
                    << keypad::values[2] << " "
                    << keypad::values[3] << " "
                    << keypad::values[4] << " ";

            calibration_pin_a::template mode<pin_mode::output>();
            calibration_pin_b::template mode<pin_mode::input_pull_up>();

            uint16_t value = 0;
            for (int i = 0; i < 4; i++) {
                calibration_pin_a::low();
                zoal::utils::nop<3>::place();
                value = value << 1 | calibration_pin_b::read();

                calibration_pin_a::high();
                zoal::utils::nop<3>::place();
                value = value << 1 | calibration_pin_b::read();
            }

            logger::trace() << "value:" << value << " f: " << ((int)force);
            if (value != 85 && !force) {
                return;
            }

            analog_channel::on();
            keypad::values[0] = calibrate_button("SELECT");
            keypad::values[1] = calibrate_button("LEFT");
            keypad::values[2] = calibrate_button("DOWN");
            keypad::values[3] = calibrate_button("UP");
            keypad::values[4] = calibrate_button("RIGHT");

            logger::trace()
                    << "Calibrated: "
                    << keypad::values[0] << " "
                    << keypad::values[1] << " "
                    << keypad::values[2] << " "
                    << keypad::values[3] << " "
                    << keypad::values[4] << " ";
            lcd::clear();
        }

        static uint16_t read_adc() {
            uint16_t result = 1 << 12;
            do {
                uint16_t value = analog_channel::read();
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
            analog_channel::on();
            handle_keypad(handler, (int16_t) analog_channel::read());
        }

        template<class H>
        static void handle_keypad(H handler, int16_t channelValue) {
            keypad::handle(handler, channelValue);
        }
    };
}}

#endif
