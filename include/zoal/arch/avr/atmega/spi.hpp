#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/segment.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address, uint8_t N>
    class spi {
    public:
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
            mem[SPCRx] |= 0x40; // SPE0
        }

        static void disable() {
            mem[SPCRx] &= ~0x40; // SPE0
        }

        static uint8_t transfer_byte(uint8_t data) {
            mem[SPDRx] = data;
            while (!(mem[SPSRx] & 0x80))
                ; // SPIFx
            return mem[SPDRx];
        }

    private:
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N>
    zoal::mem::segment<uint8_t, Address> spi<Address, N>::mem;

}}}}

#endif
