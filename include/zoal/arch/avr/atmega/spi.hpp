#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/reg.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address, uint8_t N>
    class spi {
    public:
        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        using SPCRx = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using SPSRx = zoal::mem::reg<Address + 0x01, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using SPDRx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            SPCRx::ref() |= 0x40; // SPE0
        }

        static void disable() {
            SPCRx::ref() &= ~0x40; // SPE0
        }

        static uint8_t transfer_byte(uint8_t data) {
            SPDRx::ref() = data;
            while (!(SPSRx::ref() & 0x80)) continue; // SPIFx
            return SPDRx::ref();
        }
    };
}}}}

#endif
