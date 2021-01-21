#ifndef ZOAL_TEST_UTILS_MS_COUNTER_HPP
#define ZOAL_TEST_UTILS_MS_COUNTER_HPP

#include <stdint.h>

namespace tests { namespace utils {
    class ms_counter {
    public:
        using value_type = uint32_t;

        static uint32_t now();
    };
}}


#endif
