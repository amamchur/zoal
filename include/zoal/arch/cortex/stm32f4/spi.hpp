#ifndef ZOAL_ARCH_STM32F4_SPI_SPI_HPP
#define ZOAL_ARCH_STM32F4_SPI_SPI_HPP

#include "../../../mem/reg.hpp"
#include "../../../utils/defs.hpp"

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address, class Clock>
    class spi : public Clock {
    public:
        using self_type = spi<Address, Clock>;
        using register_type = uint32_t;

        static constexpr uintptr_t address = Address;

        static constexpr uint32_t SPIx_CR1_SPE = 1 << 6;
        static constexpr uint32_t SPIx_SR_RXNE = 1 << 0;
        static constexpr uint32_t SPIx_SR_TXE = 1 << 1;
        static constexpr uint32_t SPIx_SR_BSY = 1 << 7;

        using SPIx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_SR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_DR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_CRCPR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_RXCRCR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using SPIx_TXCRCR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;

        using enable_cas = zoal::ct::type_list<typename SPIx_CR1::template cas<0, SPIx_CR1_SPE>>;
        using disable_cas = zoal::ct::type_list<typename SPIx_CR1::template cas<SPIx_CR1_SPE, 0>>;

        static inline void enable() {
            SPIx_CR1::ref() |= SPIx_CR1_SPE;
        }

        static inline void disable() {
            SPIx_CR1::ref() &= ~SPIx_CR1_SPE;
        }

        static uint8_t transfer_byte(uint8_t data) {
            SPIx_DR::ref() = data;
            while (!(SPIx_SR::ref() & SPIx_SR_RXNE)) continue;
            return SPIx_DR::ref();
        }

        static uint8_t transfer(void *txb, void *rxb, size_t size) {
            auto tx = reinterpret_cast<uint8_t *>(txb);
            auto rx = reinterpret_cast<uint8_t *>(rxb);
            auto txe = tx + size;
            auto rxe = rx + size;
            bool tx_allowed = true;

            enable();
            while (!(SPIx_SR::ref() & SPIx_SR_TXE));
            while (tx < txe && rx < rxe) {
                if ((SPIx_SR::ref() & SPIx_SR_TXE) && (tx < txe) && tx_allowed) {
                    SPIx_DR::ref() = *tx++;
                    tx_allowed = false;
                }

                if ((SPIx_SR::ref() & SPIx_SR_RXNE) && (rx < rxe)) {
                    auto d = SPIx_DR::ref();
                    *rx++ = d;
                    tx_allowed = true;
                }
            }

            do {
            } while (SPIx_SR::ref() & SPIx_SR_BSY);
            disable();
            return 0;
        }
    };
}}}

#endif
