#include "../misc/terminal_input.hpp"

#include <iostream>
#include <zoal/algorithm/kmp.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/utils/scheduler.hpp>

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

template<class N>
class print_name {
public:
    static void print() {
        std::cout << "noname" << std::endl;
    }
};

template<>
class print_name<int> {
public:
    static void print() {
        std::cout << "int" << std::endl;
    }
};

template<>
class print_name<void> {
public:
    static void print() {
        std::cout << "void" << std::endl;
    }
};

template<>
class print_name<float> {
public:
    static void print() {
        std::cout << "float" << std::endl;
    }
};

template<int No, class T, class... Rest>
class test {
public:
    using next = test<No + 1, Rest...>;

    static inline void print_at_index(int index) {
        if (index == No) {
            print_name<T>::print();
        } else {
            next::print_at_index(index);
        }
    }
};

template<int No, class T>
class test<No, T> {
public:
    static inline void print_at_index(int index) {
        if (index == No) {
            print_name<T>::print();
        }
    }
};

int main() {
    using namespace std;

    for (int i = 0; i < 100; ++i) {
        test<0, int, void, float>::print_at_index(i);
    }

    return 0;
}
