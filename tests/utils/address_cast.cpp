#include "address_cast.hpp"

namespace zoal { namespace test {
    uint8_t avr_mcu_mem[0x200] = {0};
    uint32_t cortex_periph_mem[0x30000 / sizeof(uint32_t)];
}}