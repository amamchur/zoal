#ifndef ZOAL_TEST_UTILS_DELAY_HPP
#define ZOAL_TEST_UTILS_DELAY_HPP

#include <chrono>
#include <thread>

namespace zoal { namespace tests {
    template<class Counter>
    class delay {
    public:
        using counter_value_type = typename Counter::value_type;

        static void ms(counter_value_type milliseconds) {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }

        template<uint64_t Milliseconds>
        static inline void ms() {
            std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
        }

        template<uint64_t Microseconds>
        static inline void us() {
            std::this_thread::sleep_for(std::chrono::microseconds(Microseconds));
        }

        template<uint64_t Nanoseconds>
        static inline void ns() {
            std::this_thread::sleep_for(std::chrono::nanoseconds(Nanoseconds));
        }
    };
}}

#endif
