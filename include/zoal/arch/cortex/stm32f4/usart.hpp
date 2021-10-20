#ifndef ZOAL_ARCH_STM32F4_USART_HPP
#define ZOAL_ARCH_STM32F4_USART_HPP

#include "../../../mem/reg.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address, class... Mixin>
    class usart : public Mixin... {
    public:
        static constexpr uintptr_t address = Address;

        static constexpr uint32_t USARTx_CR1_RXNEIE = 1 << 5; // RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_TXEIE = 1 << 7; // TXEIE: interrupt enable
        static constexpr uint32_t USARTx_CR1_UE = 1 << 13; // USART enable
        static constexpr uint32_t USARTx_SR_TXE = 1 << 7;
        static constexpr uint32_t USARTx_SR_RXNE = 1 << 5;

        using USARTx_SR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_DR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_BRR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR1 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR2 = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR3 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_GTPR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        using self_type = usart<Address, Mixin...>;

        using enable_cas = zoal::ct::type_list<typename USARTx_CR1::template cas<0, USARTx_CR1_UE>>;
        using disable_cas = zoal::ct::type_list<typename USARTx_CR1::template cas<USARTx_CR1_UE, 0>>;

        static inline void enable() {
            USARTx_CR1::ref() |= USARTx_CR1_UE;
        }

        static inline void disable() {
            USARTx_CR1::ref() &= ~USARTx_CR1_UE;
        }

        static inline void enable_tx() {
            USARTx_CR1::ref() |= USARTx_CR1_TXEIE;
        }

        static inline void disable_tx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_TXEIE;
        }

        static inline void enable_rx() {
            USARTx_CR1::ref() |= USARTx_CR1_RXNEIE;
        }

        static inline void disable_rx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_RXNEIE;
        }

        template<class RxCallback>
        static void rx_handler(RxCallback rx_callback) {
            auto rx_enabled = USARTx_CR1::ref() & USARTx_CR1_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = USARTx_SR::ref() & USARTx_SR_RXNE;
            if (!rx_not_empty) {
                return;
            }

            rx_callback(USARTx_DR::ref());
        }

        template<class TxCallback>
        static bool tx_handler(TxCallback callback) {
            auto tx_enabled = USARTx_CR1::ref() & USARTx_CR1_TXEIE;
            if (!tx_enabled) {
                return false;
            }

            auto tx_empty = USARTx_SR::ref() & USARTx_SR_TXE;
            if (!tx_empty) {
                return false;
            }

            uint8_t value;
            if (callback(value)) {
                USARTx_DR::ref() = value;
            } else {
                disable_tx();
            }

            return true;
        }
    };
}}}

#endif
