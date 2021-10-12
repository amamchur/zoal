#ifndef ZOAL_SHIELD_UNO_ACCESSORY_HPP
#define ZOAL_SHIELD_UNO_ACCESSORY_HPP

#include <zoal/gpio/pin.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/p9813.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/periph/adc.hpp>

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
        using mcu = typename pcb::mcu;
        using delay = Delay;
        using potentiometer_pin = typename pcb::ard_a00;
        using potentiometer_channel = typename mcu::mux::template adc_channel<typename mcu::adc_00, typename pcb::ard_a00>;

        using ssd1306_reset = typename pcb::ard_d07;
        using ssd1306_address_select = typename pcb::ard_d08;
        using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<0x3C>;
        using display_type = zoal::ic::ssd1306<128, 64, ssd1306_interface>;
        using clock_type = zoal::ic::ds3231<zoal::periph::device_buffer_type::static_mem>;
        using thermometer_type = zoal::ic::lm75;
        using accelerometer_type = zoal::ic::adxl345;
        using buzzer = zoal::gpio::active_high<typename pcb::ard_d11>;

        using rgb_spi = zoal::periph::tx_software_spi<typename pcb::ard_d05, typename pcb::ard_d06>;
        using rgb_led = zoal::ic::p9813<rgb_spi>;
        using rbg_power = typename pcb::ard_d12;

        enum class joystick_button { up, right, button, left, enter };

        class joystick {
        public:
            using counter_value_type = typename delay::counter_value_type;
            using gpio_cfg = zoal::gpio::api::optimize<
                // Enabled clock for periphs
                typename pcb::ard_a01::port::clock_on_cas,
                typename pcb::ard_a02::port::clock_on_cas,
                typename pcb::ard_a03::port::clock_on_cas,
                typename pcb::ard_a04::port::clock_on_cas,
                typename pcb::ard_a05::port::clock_on_cas,

                zoal::gpio::api::mode<zoal::gpio::pin_mode::input_pull_up,
                                      typename pcb::ard_a01,
                                      typename pcb::ard_a02,
                                      typename pcb::ard_a03,
                                      typename pcb::ard_a04,
                                      typename pcb::ard_a05>
                //
                >;

            zoal::io::button<counter_value_type, typename pcb::ard_a01> u_button;
            zoal::io::button<counter_value_type, typename pcb::ard_a02> r_button;
            zoal::io::button<counter_value_type, typename pcb::ard_a03> l_button;
            zoal::io::button<counter_value_type, typename pcb::ard_a04> e_button;
            zoal::io::button<counter_value_type, typename pcb::ard_a05> d_button;

            /**
             * Handle Uno Accessory Joystick
             * @tparam H handler
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
            typename joystick::gpio_cfg,
            typename buzzer::pin::port::clock_on_cas,
            typename buzzer::gpio_cfg,
            // RGB Led
            typename rbg_power::port::clock_on_cas,
            typename rgb_spi::gpio_cfg,
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output, rbg_power>,
            // SSD1306
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output, ssd1306_reset, ssd1306_address_select>,
            // Joystick buttons
            zoal::gpio::api::mode<zoal::gpio::pin_mode::input_pull_up,
                                  typename pcb::ard_a01,
                                  typename pcb::ard_a02,
                                  typename pcb::ard_a03,
                                  typename pcb::ard_a04,
                                  typename pcb::ard_a05>
            //
            >;

        static void ssd1306_slave_address_setup() {
            typename ssd1306_address_select::low();

            typename ssd1306_reset::high();
            delay::ms(1);
            typename ssd1306_reset::low();
            delay::ms(10);
            typename ssd1306_reset::high();
        }

        static void rgb(uint8_t red, uint8_t green, uint8_t blue) {
            typename rbg_power::_1();
            rgb_led::frame();
            rgb_led::send(red, green, blue);
            rgb_led::frame();
        }

        static inline void rgb_off() {
            typename rbg_power::_0();
        }
    };
}}

#endif
