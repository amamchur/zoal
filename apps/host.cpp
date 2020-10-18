#include <iostream>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/mcu/atmega328p.hpp>
#include <zoal/mem/reg.hpp>

using PINx_A = zoal::mem::reg<0x0023, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
using PINx_B = zoal::mem::reg<0x0023, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
using DDRx_A = zoal::mem::reg<0x024, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
//using mcu = zoal::arduino_uno<16000000UL>::mcu;
//using mcu = zoal::mcu::stm32f103c8tx<>;
using A = typename PINx_A::template cas<0x0F, 0>;
using B = typename PINx_B::template cas<0x0F, 0>;
using M = zoal::mem::merge_cas<A, B>::result;

template<class C>
struct type_name {
    static const char *name;
};

template<class C>
const char *type_name<C>::name = "Unknown type";

template<>
const char *type_name<int>::name = "int";

template<>
const char *type_name<bool>::name = "bool";

template<>
const char *type_name<float>::name = "float";

template<>
const char *type_name<void>::name = "void";

template<>
const char *type_name<char>::name = "char";

template<>
const char *type_name<long>::name = "long";

struct cas_print_functor {
    template<class A>
    void operator()() {
        std::cout << "CAS: " << (void *)A::address << " | " << (void *)A::clear << ", " << (void *)A::set << std::endl;
        //        std::cout << counter++ << " name: " << type_name<A>::name << std::endl;
    }

    int counter{0};
};

using tl1 = zoal::ct::type_list<int, float>;
using tl2 = zoal::ct::type_list<bool, char>;
using tl3 = zoal::ct::type_list<void, int>;
using tlf = zoal::ct::type_list_join<tl1, tl2, tl3>::result;

int main() {
    using namespace std;

    using mcu = zoal::mcu::atmega328p<16000000>;
    using timer = mcu::timer_01;
    using cfg = mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>;
    using cfg_cas = cfg::cfg;

    cas_print_functor func;
    std::cout << "timer_mode_cfg ------------------ " << std::endl;
    zoal::ct::type_list_iterator<cfg::timer_mode_cfg>::for_each(func);

    std::cout << "clock_divider_cfg ------------------ " << std::endl;
    zoal::ct::type_list_iterator<cfg::clock_divider_cfg>::for_each(func);

    std::cout << "cfg_cas ------------------ " << std::endl;
    zoal::ct::type_list_iterator<cfg_cas>::for_each(func);
    return 0;
}
