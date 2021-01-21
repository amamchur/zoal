#include "ms_counter.hpp"

#include <chrono>

namespace zoal { namespace tests {
    uint32_t ms_counter::now() {
        using namespace std::chrono;
        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return static_cast<uint32_t>(now);
    }
}}
