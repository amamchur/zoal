#include <cstring>
#include <iostream>
#include <string>

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
