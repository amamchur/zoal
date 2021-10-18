#ifndef ZOAL_ARCH_M3_DWT_HPP
#define ZOAL_ARCH_M3_DWT_HPP

#include "../../../mem/reg.hpp"

#include <cstdint>

namespace zoal { namespace arch {
    class dwt {
    public:
        static constexpr uintptr_t address = 0xE0001000;
        using DWTx_CTRL = zoal::mem::reg<address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using DWTx_CYCCNT = zoal::mem::reg<address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uint32_t DWTx_CTRL_CYCCNTENA = 1 << 0;

        static __attribute__((noinline)) void nop(uint32_t d) {
            DWTx_CYCCNT::ref() = 0;
            DWTx_CTRL::ref() |= DWTx_CTRL_CYCCNTENA;
            do {
            } while (d > DWTx_CYCCNT::ref());
            DWTx_CTRL::ref() &= ~DWTx_CTRL_CYCCNTENA;
        }
    };
}}

#endif
