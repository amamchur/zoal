#ifndef ZOAL_SHIELD_UNO_ACCESSORY_HPP
#define ZOAL_SHIELD_UNO_ACCESSORY_HPP

#include <zoal/gpio/pin.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/p9813.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/software_spi.hpp>

namespace zoal { namespace shield {
    template<class Tools, class Board>
    class uno_accessory {
    public:
        using pcb = Board;
        using mcu = typename pcb::mcu;
        using i2c = typename mcu::i2c_00;
        using tools = Tools;
        using potentiometer = typename pcb::ard_a00;

        using p9813_spi = zoal::periph::tx_software_spi<typename pcb::ard_d05, typename pcb::ard_d06>;
        using p9813 = zoal::ic::p9813<p9813_spi>;
        using p9813_switch = zoal::gpio::active_high<typename pcb::ard_d12>;

        using i2c_stream = zoal::periph::i2c_stream<i2c>;
        using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<tools, i2c, typename pcb::ard_d07, typename pcb::ard_d08, 0x3C>;
        using ssd1306 = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;
        using ds3231 = zoal::ic::ds3231<>;
        using lm75 = zoal::ic::lm75<>;
        using adxl345 = zoal::ic::adxl345<>;

        using u_button = zoal::io::button<uint32_t, typename pcb::ard_a01>;
        using r_button = zoal::io::button<uint32_t, typename pcb::ard_a02>;
        using l_button = zoal::io::button<uint32_t, typename pcb::ard_a03>;
        using e_button = zoal::io::button<uint32_t, typename pcb::ard_a04>;
        using d_button = zoal::io::button<uint32_t, typename pcb::ard_a05>;

        uno_accessory(i2c_stream *i2cs)
            : display(i2cs) {}

        void init() {
            display.init();
            typename tools::api::template mode<zoal::gpio::pin_mode::input_pull_up,
                                               typename u_button::pin,
                                               typename r_button::pin,
                                               typename l_button::pin,
                                               typename e_button::pin,
                                               typename d_button::pin>();
        }

        static uint16_t read_potentiometer() {
            using adc = typename mcu::adc_00;
            mcu::mux::template adc<adc, potentiometer>::on();
            return adc::read();
        }

        static void potentiometer_async() {
            using adc = typename mcu::adc_00;
            typename mcu::mux::template adc<adc, potentiometer>::connect();
            adc::start();
        }

        template<class Callback>
        void handle_buttons(Callback callback) {
            // Enter & down button uses SDA & SCL I2C pins
            // Do not read it values when I2C is working
            if (i2c::busy()) {
                return;
            }

            using zoal::io::button_event;
            u_btn.handle([callback](button_event e) { callback(0, e); });
            l_btn.handle([callback](button_event e) { callback(1, e); });
            r_btn.handle([callback](button_event e) { callback(2, e); });
            d_btn.handle([callback](button_event e) { callback(3, e); });
            e_btn.handle([callback](button_event e) { callback(4, e); });
        }

        u_button u_btn;
        r_button r_btn;
        l_button l_btn;
        e_button e_btn;
        d_button d_btn;

        ssd1306 display;
        ds3231 rtc;
        lm75 temp_sensor;
    };
}}

#endif
