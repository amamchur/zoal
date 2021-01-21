//
// Created by amam on 20.08.18.
//

#include "ms_counter.hpp"
#include <chrono>

uint32_t tests::utils::ms_counter::now() {
    using namespace std::chrono;
    auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint32_t>(now);
}
