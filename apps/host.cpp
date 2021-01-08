#include "../misc/terminal_input.hpp"

#include <iostream>
#include <zoal/utils/scheduler.hpp>
#include <zoal/algorithm/kmp.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>

using namespace std;

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;
using usart = usart_01;

using usart_tx_buffer = usart::default_tx_buffer<128>;
using usart_rx_buffer = usart::default_rx_buffer<128>;
using api = zoal::gpio::api;

struct cas_print_functor {
    template<class A>
    void operator()() {
        std::cout << "CAS: " << (void *)A::address << " | " << (void *)A::clear << ", " << (void *)A::set << std::endl;
        //        std::cout << counter++ << " name: " << type_name<A>::name << std::endl;
    }

    int counter{0};
};

int main() {
    using namespace std;

    using cfg_cas = api::optimize<api::power_on<usart, mcu::port_a, mcu::port_c, mcu::port_b>>;
    cas_print_functor func;
    zoal::ct::type_list_iterator<cfg_cas>::for_each(func);
    cfg_cas();

    return 0;
}
