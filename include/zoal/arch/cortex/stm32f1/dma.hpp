#ifndef ZOAL_ARC_STM32F1_DMA_HPP
#define ZOAL_ARC_STM32F1_DMA_HPP

#include "../../../mem/reg.hpp"
#include "../../../utils/defs.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class Clock>
    class dma {
    public:
        static constexpr uintptr_t address = Address;

        using DMAx_ISR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using DMAx_IFCR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
    };

}}}

#endif
