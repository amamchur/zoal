#ifndef ZOAL_ARCH_STM32X_PORT_SPI_HPP
#define ZOAL_ARCH_STM32X_PORT_SPI_HPP

#include "../../../mem/reg.hpp"
#include "../../../utils/defs.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address, class Clock>
    class spi : public Clock {
    public:
        using self_type = spi<Address, Clock>;
        using register_type = uint32_t;

        static constexpr uintptr_t address = Address;

        using SPIx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_SR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_DR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_CRCPR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_RXCRCR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_TXCRCR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_I2SCFGR = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_I2SPR = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
    };
}}}

#endif
