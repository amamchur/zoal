#ifndef ZOAL_DMA_HPP
#define ZOAL_DMA_HPP

#include "../../../mem/reg.hpp"
#include "../../../utils/defs.hpp"

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address, class Clock>
    class dma {
    public:
        static constexpr uintptr_t address = Address;

        template<int N>
        class stream {
        public:
            using dma = zoal::arch::stm32f4::dma<Address, Clock>;

            static constexpr uintptr_t address = Address + N * 0x18;
            static constexpr uintptr_t no = N;

            using SxCR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
            using SxNDTR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
            using SxPAR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
            using SxM0AR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
            using SxM1AR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
            using SxFCR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        };

        using DMAx_LISR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using DMAx_HISR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using DMAx_LIFCR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using DMAx_HIFCR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        using stream_0 = stream<0>;
        using stream_1 = stream<1>;
        using stream_2 = stream<2>;
        using stream_3 = stream<3>;
        using stream_4 = stream<4>;
        using stream_5 = stream<5>;
        using stream_6 = stream<6>;
        using stream_7 = stream<7>;
    };

}}}

#endif
