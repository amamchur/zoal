#include <iostream>
#include <zoal/mcu/stm32f103c8tx.hpp>

using mcu = zoal::mcu::stm32f103c8tx<>;

int main() {
    using namespace std;

    using pin = mcu::pb_12;
    using port = pin::port;
    using md = port::mode_cls<zoal::gpio::pin_mode::input_pull_up, pin::mask>::md;
    using p12_md = md::p12;

    cout << "Test" << endl;
    cout << "C P12: " << p12_md::clear_mask << endl;
    cout << "S P12: " << p12_md::set_mask << endl;
    cout << "C CRH: " << md::GPIOx_CRH::clear_mask << endl;
    cout << "S CRH: " << md::GPIOx_CRH::set_mask << endl;

    return 0;
}
