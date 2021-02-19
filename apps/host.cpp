#include "../misc/timer_freq_calculator.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/data/vector.hpp>

using calc = zoal::misc::timer_freq_calculator<1000, 16000000, 256, 1, 8, 64, 256, 1024>;

class Functor {
public:
    template<class T>
    void operator()(int index) {
        std::cout << "--- index: " << index << std::endl;
        std::cout << "current_prescaler:\t" << T::current_prescaler << std::endl;
        std::cout << "current_value:\t\t" << T::current_value << std::endl;

        std::cout << "best_prescaler:\t\t" << T::prescaler << std::endl;
        std::cout << "best_prescaler:\t\t" << T::prescaler << std::endl;
        std::cout << "best_value:\t\t\t" << T::compare_value << std::endl;
        std::cout << "real_freq:\t\t\t" << T::real_freq << std::endl;
        std::cout << "delta_freq_abs:\t\t" << T::delta_freq_abs << std::endl;
    }
};

int main() {
    zoal::data::vector<int> v(1, 0, 1);
    auto nv = v.normalize();

    std::cout << "length: " << v.length() << std::endl;
    std::cout << "x: " << nv.x << " y: " << nv.y << " z: " << nv.z << std::endl;
    return 0;
}
