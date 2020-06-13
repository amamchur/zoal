#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER8_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER8_HPP

#include <stdint.h>
#include "../timer8.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uint32_t Address>
    class timer8_model {
    public:
        using word = uint8_t;

        using TCCRxA = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TCCRxB = zoal::mem::reg<Address + 0x01, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TCNTx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using OCRxA = zoal::mem::reg<Address + 0x03, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using OCRxB = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

        static constexpr uint8_t TCCRxA_COMxA = 0xC0;
        static constexpr uint8_t TCCRxA_COMxB = 0x30;
        static constexpr uint8_t TCCRxA_WGMx = 0x03;
        static constexpr uint8_t TCCRxB_FOCxA = 0x80;
        static constexpr uint8_t TCCRxB_FOCxB = 0x40;
        static constexpr uint8_t TCCRxB_WGMx2 = 0x08;
        static constexpr uint8_t TCCRxB_CSx = 0x07;
    };

    template<uintptr_t Address, uint8_t N, bool Async = false>
    using timer8 = ::zoal::arch::avr::timer8<Address, N, Async, timer8_model<Address>>;

}}}}

#endif
