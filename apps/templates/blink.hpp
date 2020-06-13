#pragma once

#ifndef APPS_BLINK_APP_HPP
#define APPS_BLINK_APP_HPP

#include <zoal/gpio/pin_mode.hpp>

template<class Tools, class LedPin>
class blink {
private:
    using delay = typename Tools::delay;
public:
    void init() {
        LedPin::port::power_on();
        LedPin::template mode<zoal::gpio::pin_mode::output>();
    }

    void run_once() {
        LedPin::high();
        delay::ms(200);

        LedPin::low();
        delay::ms(200);
    }

    void run() {
        while (true) {
            run_once();
        }
    }
};

#endif
