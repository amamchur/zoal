#ifndef APPS_TM1637_TM1637APP_HPP_
#define APPS_TM1637_TM1637APP_HPP_

#include <zoal/ic/tm1637.hpp>
#include <zoal/data/segment7.hpp>

template<class Tools, class CLK, class DIO>
class tm1637 {
public:
    using self_type = tm1637<Tools, CLK, DIO>;
    using delay = typename Tools::delay;
    using scheduler = typename Tools::template method_scheduler<self_type, 8>;
    using display_type = zoal::ic::tm1637<Tools, CLK, DIO>;
    using transaction = typename display_type::transaction;

    tm1637() : timeout(this) {
    }

    void init() {
        display_type::begin();
        transaction() << display_type::pulse_width5;
    }

    void run_once() {
        timeout.handle();
    }

    void run() {
        timeout.schedule(0, &self_type::displayValue);
        timeout.schedule(0, &self_type::toggleDot);
        timeout.schedule(1000, &self_type::changePulseWidth);
        while (true) {
            run_once();
        }
    }

private:
    void toggleDot() {
        dot_enabled = !dot_enabled;
        timeout.schedule(500, &self_type::displayValue);
    }

    void displayValue() {
        using namespace zoal::data;

        value++;

        transaction() << display_type::write_data;
        transaction() << display_type::set_address0
                      << segment7::abcd_hex((value >> 0xA) & 0xF)
                      << segment7::abcd_hex((value >> 0x8) & 0xF).dot(dot_enabled)
                      << segment7::abcd_hex((value >> 0x4) & 0xF)
                      << segment7::abcd_hex((value >> 0x0) & 0xF);
        transaction() << pulse_width;

        timeout.schedule(10, &self_type::displayValue);
    }

    void changePulseWidth() {
        pulse_width++;
        if (pulse_width > display_type::CmdPulseWidth5) {
            pulse_width = display_type::CmdPulseWidth0;
        }
        timeout.schedule(3000, &self_type::changePulseWidth);
    }

    display_type display;
    scheduler timeout;
    uint16_t value{0};
    uint8_t pulse_width{display_type::pulse_width0};
    bool dot_enabled{false};
};

#endif
