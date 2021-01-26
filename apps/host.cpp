#include <iostream>
#include <zoal/func/function.hpp>

zoal::func::function<64, void> fn1;
zoal::func::function<64, void> fn2;
int main() {
    fn1 = [](){
        std::cout << "Hello" << std::endl;
    };

    fn1();
    fn2 = fn1;
    fn2();

    return 0;
}
