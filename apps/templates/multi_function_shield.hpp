#ifndef APPS_MULTIFUNCTION_SHIELD_HPP
#define APPS_MULTIFUNCTION_SHIELD_HPP

#include <zoal/gpio/pin_mode.hpp>
#include <zoal/ic/ic74hc595.hpp>
#include <zoal/io/button.hpp>
#include <zoal/data/segment7.hpp>
#include <zoal/gpio/pin.hpp>

template<class Tools, class PCB>
class multi_function_shield {
public:
    using self_type = multi_function_shield<Tools, PCB>;
    using delay = typename Tools::delay;
    using counter = typename Tools::counter;
    using method_scheduler = typename Tools::template method_scheduler<self_type, 4>;
    using led1 = zoal::gpio::active_low<typename PCB::ard_d13>;
    using led2 = zoal::gpio::active_low<typename PCB::ard_d12>;
    using led3 = zoal::gpio::active_low<typename PCB::ard_d11>;
    using led4 = zoal::gpio::active_low<typename PCB::ard_d10>;
    using beeper = zoal::gpio::active_drain<typename PCB::ard_d03>;
    using display_type = zoal::ic::ic74hc595<
            typename PCB::ard_d08,
            typename PCB::ard_d04,
            typename PCB::ard_d07>;

    method_scheduler scheduler;
    uint16_t value;
    uint8_t segments[4]{0};
    zoal::io::button_ext<typename PCB::ard_a01, counter> button1;
    zoal::io::button_ext<typename PCB::ard_a02, counter> button2;
    zoal::io::button_ext<typename PCB::ard_a03, counter> button3;

    multi_function_shield() : scheduler(this), value(0) {
    }

    void hex_to_segments(uint16_t value) {
        uint16_t v = value;
        for (int i = 3; i >= 0; i--) {
            auto d = static_cast<uint8_t>(v & 0x0F);
            segments[i] = ~zoal::data::segment7::abcd_hex(d);
            v >>= 4;
        }
    }

    void init() {
        led1::off();
        led2::off();
        led3::off();
        led4::off();

        display_type::init();

        hex_to_segments(0xBEEF);

        beeper::on();
        delay::ms(10);
        beeper::off();

        scheduler.schedule(0, &self_type::dynamic_indication);
    }

    void dynamic_indication() {
        static uint8_t segmentIndex = 0;
        auto segmentMask = static_cast<uint8_t>(1 << segmentIndex);
        auto segmentValue = segments[segmentIndex];
        typename display_type::transaction() << segmentValue << segmentMask;
        segmentIndex = static_cast<uint8_t>((segmentIndex + 1) & 0x3);
        scheduler.schedule(0, &self_type::dynamic_indication);
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
        scheduler.handle();
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
