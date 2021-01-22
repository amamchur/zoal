#include <iostream>
#include <string>

#include <zoal/gpio/api.hpp>
#include <zoal/mem/cas.hpp>
#include <zoal/mcu/atmega2560.hpp>
#include <zoal/ct/type_list.hpp>
#include <zoal/ct/signature.hpp>

using namespace std;

struct cas_print_functor {
    template<class A>
    void operator()() {
        std::cout << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set << "\r\n";
    }
};

int main() {
    return 0;
}
