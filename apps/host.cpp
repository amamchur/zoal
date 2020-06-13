#include <iostream>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/mem/reg.hpp>

using PINx_A = zoal::mem::reg<0x0023, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
using PINx_B = zoal::mem::reg<0x0023, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
using DDRx_A = zoal::mem::reg<0x024, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
using mcu = zoal::arduino_uno<16000000UL>::mcu;

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

struct functor {
    template<class A>
    void operator()() const {
        std::cout << "address: " << (int)A::address << std::endl;
        std::cout << "clear:   " << (int)A::clear << std::endl;
        std::cout << "set:     " << (int)A::set << std::endl;
    }
};

using tl1 = zoal::ct::type_list<int, float>;
using tl2 = zoal::ct::type_list<bool, char>;
using tl3 = zoal::ct::type_list<void, int>;
using tlf = zoal::ct::type_list_join<tl1, tl2, tl3>::result;

using cas1 = PINx_A::cas<0xF1, 0xF1>;
using cas_list = zoal::ct::type_list<PINx_A::cas<0x00, 0x01>,
                                     DDRx_A::cas<0x00, 0x00>,
                                     PINx_A::cas<0x00, 0x04>,
                                     DDRx_A::cas<0x00, 0x00>,
                                     PINx_A::cas<0x00, 0x00>,
                                     DDRx_A::cas<0x02, 0x00>,
                                     PINx_A::cas<0x01, 0x00>,
                                     DDRx_A::cas<0x00, 0xF0>>;

using cas_list_2 = zoal::ct::type_list<PINx_A::cas<0x00, 0x01>>;

using filtered = typename zoal::mem::filter_cas<cas_list>::result;
using r = zoal::mem::merge_cas_in_list<cas_list>::result;

struct low_cas {
    template<class Pin>
    using list = typename Pin::port::template low_cas<Pin::mask>;
};

struct high_cas {
    template<class Pin>
    using list = typename Pin::port::template high_cas<Pin::mask>;
};

template<zoal::gpio::pin_mode PinMode>
struct mode_cas {
    template<class Pin>
    using list = typename Pin::port::template mode_cas<PinMode, Pin::mask>;
};

template<class Collector, class Pin, class... Rest>
struct collect_cas {
    using current = typename Collector::template list<Pin>;
    using next = typename collect_cas<Collector, Rest...>::result;
    using result = typename zoal::ct::type_list_join<current, next>::result;
};

template<class Collector, class Pin>
struct collect_cas<Collector, Pin> {
    using result = typename Collector::template list<Pin>;
};

struct new_api {
    template<class... Pins>
    using low = typename zoal::mem::merge_cas_in_list<typename collect_cas<low_cas, Pins...>::result>::result;

    template<class... Pins>
    using high = typename zoal::mem::merge_cas_in_list<typename collect_cas<high_cas, Pins...>::result>::result;

    template<zoal::gpio::pin_mode PinMode, class... Pins>
    using mode = typename zoal::mem::merge_cas_in_list<typename collect_cas<mode_cas<PinMode>, Pins...>::result>::result;

    template<class L, class... Rest>
    struct apply {
        using all = typename zoal::ct::type_list_join<L, Rest...>::result;
        using result = typename zoal::mem::merge_cas_in_list<all>::result;
        apply() {
            zoal::mem::apply_cas_list<result>();
        }
    };

    template<class L>
    struct apply<L> {
        apply() {
            zoal::mem::apply_cas_list<L>();
        }
    };
};

using hello_list = new_api::low<mcu::pb_00, mcu::pb_03>;

int main() {
    using namespace std;

    using www = new_api::apply<
        //
        new_api::mode<zoal::gpio::pin_mode::output, mcu::pb_00, mcu::pb_02, mcu::pb_04, mcu::pb_01, mcu::pb_03, mcu::pb_05>,
        new_api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_06, mcu::pb_07>,
        new_api::low<mcu::pb_00, mcu::pb_02, mcu::pb_04>,
        new_api::high<mcu::pb_01, mcu::pb_03, mcu::pb_05>
        //
        >::result;

    functor func;
    zoal::ct::type_list_iterator<www>::for_each(func);

    return 0;
}
