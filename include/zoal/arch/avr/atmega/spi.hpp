#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/accessor.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address, uint8_t N>
    class spi {
    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        static constexpr uintptr_t SPCRx = 0;
        static constexpr uintptr_t SPSRx = 1;
        static constexpr uintptr_t SPDRx = 2;

        spi() = delete;

        spi(const spi &) = delete;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            *accessor<SPCRx>::p |= 0x40; // SPE0
        }

        static void disable() {
            *accessor<SPCRx>::p &= ~0x40; // SPE0
        }

        static uint8_t transfer_byte(uint8_t data) {
            *accessor<SPDRx>::p = data;
            while (!(*accessor<SPSRx>::p & 0x80)) continue; // SPIFx
            return *accessor<SPDRx>::p;
        }
    };
}}}}

#endif
