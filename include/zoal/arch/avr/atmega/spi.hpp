#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../gpio/pin_mode.hpp"
#include "zoal/mem/segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class base_spi {
    public:
        enum : uint8_t {
            SPIFx = 7,
            SPEx = 6,
            MSTRx = 4,
            DORDx = 5,
            SPI2Xx = 0,
        };

        static constexpr uintptr_t SPCRx = 0;
        static constexpr uintptr_t SPSRx = 1;
        static constexpr uintptr_t SPDRx = 2;

        base_spi() = delete;

        base_spi(const base_spi &) = delete;

        static uint8_t transfer_byte(uint8_t data) {
            zoal::mem::segment<uint8_t, 0x4C> m8;
            m8[SPDRx] = data;

            asm volatile("nop");

            while (!(m8[SPSRx] & (1u << SPIFx)));
            return m8[SPDRx];
        }
    };
}}}

#endif
