#include <iostream>
#include <zoal/io/output_stream.hpp>
#include <zoal/mcu/atmega328p.hpp>
#include <zoal/mem/reg.hpp>
#include <zoal/utils/logger.hpp>

struct cas_print_functor {
    template<class A>
    void operator()() {
        std::cout << "CAS: " << (void *)A::address << " | " << (void *)A::clear << ", " << (void *)A::set << std::endl;
    }

    int counter{0};
};

using mcu = zoal::mcu::atmega328p<16000000>;
using api = zoal::gpio::api;

void print_timer_cfg() {
    using timer = mcu::timer_01;
    using cfg = mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>;

    cas_print_functor func;
    std::cout << "timer_mode_cfg ------------------ " << std::endl;
    zoal::ct::type_list_iterator<cfg::timer_mode_cfg>::for_each(func);

    std::cout << "power_off_cas ------------------ " << std::endl;
    using qqq = typename mcu::port_b::power_off_cas;
    zoal::ct::type_list_iterator<qqq>::for_each(func);
}

char buffer[1000] = {0};
int length = 0;

using mt = zoal::io::memory_transport<>;
using logger = zoal::utils::terminal_logger<mt, zoal::utils::log_level::trace>;
//using logger = zoal::utils::plain_logger<std_transport, zoal::utils::log_level::trace>;

int main() {
    using namespace std;
    mt::init(buffer);

    print_timer_cfg();

    logger::trace() << "1: " << (int)312;
    logger::debug() << "2";
    logger::info() << "3";
    logger::warn() << "4";
    logger::error() << "5";

    std::cout << buffer;

    mcu::port_b::PINx::cas<0x0f, 0xf0>();

    return 0;
}
