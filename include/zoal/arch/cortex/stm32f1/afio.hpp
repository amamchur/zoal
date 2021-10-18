#ifndef ZOAL_ARCH_STM32F1_AFIO_HPP
#define ZOAL_ARCH_STM32F1_AFIO_HPP

#include "../../../mem/reg.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class Clock>
    class afio : public Clock {
    public:
        using self_type = afio<Address, Clock>;

        static constexpr uintptr_t address = Address;

        using AFIOx_EVCR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_MAPR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_EXTICR1 = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_EXTICR2 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_EXTICR3 = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_EXTICR4 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using AFIOx_MAPR2 = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
    };
}}}

#endif
