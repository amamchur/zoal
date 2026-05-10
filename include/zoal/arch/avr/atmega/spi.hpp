#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../mem/reg.hpp"

namespace zoal::arch::avr::atmega {
    template<uintptr_t Address, uint8_t N>
    class spi {
    public:
        using self_type = spi<Address, N>;

        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        using SPCRx = mem::reg<Address + 0x00, mem::reg_io::read_write, uint8_t, 0xFF>;
        using SPSRx = mem::reg<Address + 0x01, mem::reg_io::read_write, uint8_t, 0xFF>;
        using SPDRx = mem::reg<Address + 0x02, mem::reg_io::read_write, uint8_t, 0xFF>;

        using enable_cas = mem::callable_cas_list_variadic<typename SPCRx::template cas<0, 0x40>>;
        using disable_cas = mem::callable_cas_list_variadic<typename SPCRx::template cas<0x40, 0>>;
        using clock_on_cas = ct::type_list<mem::null_cas>;
        using clock_off_cas = ct::type_list<mem::null_cas>;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            enable_cas();
        }

        static void disable() {
            disable_cas();
        }

        static uint8_t transfer_byte(uint8_t data) {
            SPDRx::ref() = data;
            while (!(SPSRx::ref() & 0x80)) continue; // SPIFx
            return SPDRx::ref();
        }
    };
}

#endif
