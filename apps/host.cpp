#include "../misc/terminal_input.hpp"
#include "../misc/terminal_machine.hpp"
#include "../misc/type_detector.hpp"

#include <iostream>
#include <string>

#include <zoal/gpio/api.hpp>
#include <zoal/mem/cas.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/ct/type_list.hpp>

using namespace std;

using mcu = zoal::mcu::stm32f103c8tx;
using usart_01 = mcu::usart_01;

struct cas_print_functor {
    template<class A>
    void operator()() {
        std::cout << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set << "\r\n";
    }
};

int main() {

    using api = zoal::gpio::api;
    using cas = mcu::mux::usart<usart_01, mcu::pb_07, mcu::pb_06>::connect;
    using opt = api::optimize<cas>;

    cas_print_functor fn;
    zoal::ct::type_list_iterator<opt>::for_each(fn);

    return 0;
}
