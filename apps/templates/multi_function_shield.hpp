#ifndef APPS_MULTIFUNCTION_SHIELD_HPP
#define APPS_MULTIFUNCTION_SHIELD_HPP

#include <zoal/data/segment7.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/gpio/pin_mode.hpp>
#include <zoal/ic/ic74hc595.hpp>
#include <zoal/io/button.hpp>
#include <zoal/utils/helpers.hpp>

template<class Tools, class Board>
class multi_function_shield {
public:
    using tools = Tools;
    using self_type = multi_function_shield<Tools, Board>;
    using delay = typename tools::delay;
    using counter = typename tools::counter;

    using data_input = typename Board::ard_d08;
    using shift_clock = typename Board::ard_d07;
    using storage_clock = typename Board::ard_d04;
    using display_type = zoal::ic::ic74hc595<Tools, data_input, shift_clock, storage_clock>;

    using button1_type = zoal::io::button_ext<tools, typename Board::ard_a01>;
    using button2_type = zoal::io::button_ext<tools, typename Board::ard_a02>;
    using button3_type = zoal::io::button_ext<tools, typename Board::ard_a03>;

    using pin_led1 = typename Board::ard_d13;
    using pin_led2 = typename Board::ard_d12;
    using pin_led3 = typename Board::ard_d11;
    using pin_led4 = typename Board::ard_d10;
    using led1 = zoal::gpio::active_drain<pin_led1>;
    using led2 = zoal::gpio::active_drain<pin_led2>;
    using led3 = zoal::gpio::active_drain<pin_led3>;
    using led4 = zoal::gpio::active_drain<pin_led4>;

    using pin_beeper = typename Board::ard_d03;
    using beeper = zoal::gpio::active_drain<pin_beeper>;

    uint16_t value{0};
    uint8_t segments[4]{0};
    button1_type button1;
    button2_type button2;
    button3_type button3;

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

    void dec_to_segments(uint16_t value) {
        uint8_t buffer[5] = {0};
        auto r = zoal::utils::radix<10>::split(value, buffer);
        if (r == buffer) {
            r++;
        }

        auto l = buffer;
        for (int i = 3; i >= 0; i--) {
            if (l < r) {
                segments[i] = ~zoal::data::segment7::abcd_hex(*l++);
            } else {
                segments[i] = 0xff;
            }
        }
    }

    void init() {
        hex_to_segments(0xBEEF);

        beeper::on();
        delay::template ms<10>();
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
