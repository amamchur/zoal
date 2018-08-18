
#ifndef MCURDK_COMPILE_CHECK_HPP
#define MCURDK_COMPILE_CHECK_HPP

#include <stdint.h>

template<class First, class ... Rest>
class compile_check {
public:
    using current = compile_check<First>;
    using next = compile_check<Rest...>;

    static void check() {
        current::check();
        next::check();
    }
};

template<class First>
class compile_check<First> {
public:
    static void check() {
        First f;
        f.init();
        f.run_once();
    }
};

#endif
