#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER16_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER16_HPP

#include "../timer16.hpp"

#include <stdint.h>

namespace zoal::arch::avr::atmega {
    template<uint32_t Address>
    class timer16_model {
    public:
        using word = uint16_t;

        using TCCRxA = mem::reg<Address + 0x00, mem::reg_io::read_write, uint8_t, 0xFF>;
        using TCCRxB = mem::reg<Address + 0x01, mem::reg_io::read_write, uint8_t, 0xFF>;
        using TCCRxC = mem::reg<Address + 0x02, mem::reg_io::read_write, uint8_t, 0xFF>;
        using TCNTx = mem::reg<Address + 0x04, mem::reg_io::read_write, uint16_t, 0xFFFF>;
        using ICRx = mem::reg<Address + 0x06, mem::reg_io::read_write, uint16_t, 0xFFFF>;
        using OCRxA = mem::reg<Address + 0x08, mem::reg_io::read_write, uint16_t, 0xFFFF>;
        using OCRxB = mem::reg<Address + 0x0A, mem::reg_io::read_write, uint16_t, 0xFFFF>;
        using OCRxC = mem::reg<Address + 0x0C, mem::reg_io::read_write, uint16_t, 0xFFFF>;

        static constexpr uint8_t TCCRxA_COMxA = 0xC0;
        static constexpr uint8_t TCCRxA_COMxB = 0x30;
        static constexpr uint8_t TCCRxA_WGMx = 0x03;
        static constexpr uint8_t TCCRxB_ICNCx = 0x80;
        static constexpr uint8_t TCCRxB_ICESx = 0x40;
        static constexpr uint8_t TCCRxB_WGMx = 0x18;
        static constexpr uint8_t TCCRxB_CSx = 0x07;
        static constexpr uint8_t TCCRxC_FOCxA = 0x80;
        static constexpr uint8_t TCCRxC_FOCxB = 0x40;
    };

    template<uintptr_t Address, uint8_t N>
    using timer16 = timer16<Address, N, timer16_model<Address>>;

}

#endif
