#ifndef APPS_MULTIFUNCTION_SHIELD_HPP
#define APPS_MULTIFUNCTION_SHIELD_HPP

#include <zoal/gpio/pin_mode.hpp>
#include <zoal/ic/ic74hc595.hpp>
#include <zoal/io/button.hpp>
#include <zoal/data/segment7.hpp>
#include <zoal/gpio/pin.hpp>

template<class Tools, class Board>
class multi_function_shield {
public:
    using self_type = multi_function_shield<Tools, Board>;
    using delay = typename Tools::delay;
    using counter = typename Tools::counter;
    using method_scheduler = typename Tools::template method_scheduler<self_type, 4>;
    using led1 = zoal::gpio::active_low<typename Board::ard_d13>;
    using led2 = zoal::gpio::active_low<typename Board::ard_d12>;
    using led3 = zoal::gpio::active_low<typename Board::ard_d11>;
    using led4 = zoal::gpio::active_low<typename Board::ard_d10>;
    using beeper = zoal::gpio::active_drain<typename Board::ard_d03>;
    using display_type = zoal::ic::ic74hc595<
            typename Board::ard_d08,
            typename Board::ard_d04,
            typename Board::ard_d07>;
    uint16_t value{0};
    uint8_t segments[4]{0};
    zoal::io::button_ext<typename Board::ard_a01, counter> button1;
    zoal::io::button_ext<typename Board::ard_a02, counter> button2;
    zoal::io::button_ext<typename Board::ard_a03, counter> button3;

    multi_function_shield() = default;

    multi_function_shield(const multi_function_shield &) = delete;

    void hex_to_segments(uint16_t value) {
        uint16_t v = value;
        for (int i = 3; i >= 0; i--) {
            auto d = static_cast<uint8_t>(v & 0x0F);
            segments[i] = ~zoal::data::segment7::abcd_hex(d);
            v >>= 4;
        }
    }

    void init() {
        Board::mcu::api::template power_on<
                typename Board::ard_d13,
                typename Board::ard_d12,
                typename Board::ard_d11,
                typename Board::ard_d10,
                typename Board::ard_d08,
                typename Board::ard_d07,
                typename Board::ard_d04,
                typename Board::ard_d03,
                typename Board::ard_a01,
                typename Board::ard_a02,
                typename Board::ard_a03
        >::apply();

        led1::off();
        led2::off();
        led3::off();
        led4::off();

        display_type::init();

        hex_to_segments(0xBEEF);

        beeper::on();
        delay::ms(10);
        beeper::off();
    }

    void dynamic_indication() {
        static uint8_t segmentIndex = 0;
        auto segmentMask = static_cast<uint8_t>(1 << segmentIndex);
        auto segmentValue = segments[segmentIndex];
        typename display_type::transaction() << segmentValue << segmentMask;
        segmentIndex = static_cast<uint8_t>((segmentIndex + 1) & 0x3);
    }

    void button1_handler(zoal::io::button_event event) {
        if (event == zoal::io::button_event::press) {
            value++;
            hex_to_segments(value);
        }
    }

    void button2_handler(zoal::io::button_event event) {
        if (event == zoal::io::button_event::press) {
            value--;
            hex_to_segments(value);
        }
    }

    void button3_handler(zoal::io::button_event event) {
        if (event == zoal::io::button_event::press) {
            value = 0;
            hex_to_segments(value);
        }
    }

    void run_once() {
        dynamic_indication();
        button1.handle(this, &self_type::button1_handler);
        button2.handle(this, &self_type::button2_handler);
        button3.handle(this, &self_type::button3_handler);
    }

    void run() {
        while (true) {
            run_once();
        }
    }
};

#endif
