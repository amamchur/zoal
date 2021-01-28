#ifndef ZOAL_SHIELD_UNO_ACCESSORY_HPP
#define ZOAL_SHIELD_UNO_ACCESSORY_HPP

#include <zoal/gpio/pin.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/ssd1306.hpp>

namespace zoal { namespace shield {
    /**
     *
     * @tparam Delay - delay class
     * @tparam UnoCompatibleBoard - Arduino Uno Compatible Board
     */
    template<class Delay, class UnoCompatibleBoard>
    class uno_accessory {
    public:
        using pcb = UnoCompatibleBoard;
        using delay = Delay;
        using potentiometer_pin = typename pcb::ard_a00;

        using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<delay, typename pcb::ard_d07, typename pcb::ard_d08, 0x3C>;
        using display_type = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;
        using clock_type = zoal::ic::ds3231;
        using thermometer_type = zoal::ic::lm75;
        using accelerometer_type = zoal::ic::adxl345;
        using buzzer = zoal::gpio::active_high<typename pcb::ard_d11>;

        enum class joystick_button { up, right, button, left, enter };

        class joystick {
        public:
            using counter_value_type = typename delay::counter_value_type;

            zoal::io::button<uint32_t, typename pcb::ard_a01> u_button;
            zoal::io::button<uint32_t, typename pcb::ard_a02> r_button;
            zoal::io::button<uint32_t, typename pcb::ard_a03> l_button;
            zoal::io::button<uint32_t, typename pcb::ard_a04> e_button;
            zoal::io::button<uint32_t, typename pcb::ard_a05> d_button;

            /**
             * Handle Uno Accessory Joystick
             * @tparam H
             * @param time current time in abstract units
             * @param button_handler callback void fn(zoal::io::button_event e, joystick_button b)
             */
            template<class H>
            void handle(counter_value_type time, H button_handler) {
                u_button.handle(time, button_handler, joystick_button::up);
                r_button.handle(time, button_handler, joystick_button::right);
                l_button.handle(time, button_handler, joystick_button::left);
                e_button.handle(time, button_handler, joystick_button::enter);
                d_button.handle(time, button_handler, joystick_button::button);
            }
        };

        using gpio_cfg = zoal::gpio::api::optimize<
            // Enabled clock for periphs
            typename pcb::ard_a01::port::clock_on_cas,
            typename pcb::ard_a02::port::clock_on_cas,
            typename pcb::ard_a03::port::clock_on_cas,
            typename pcb::ard_a04::port::clock_on_cas,
            typename pcb::ard_a05::port::clock_on_cas,
            typename buzzer::pin::port::clock_on_cas,
            typename buzzer::gpio_cfg,
            // Joystick buttons
            zoal::gpio::api::mode<zoal::gpio::pin_mode::input_pull_up,
                                  typename pcb::ard_a01,
                                  typename pcb::ard_a02,
                                  typename pcb::ard_a03,
                                  typename pcb::ard_a04,
                                  typename pcb::ard_a05>
            //
            >;
    };
}}

#endif
