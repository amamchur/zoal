#ifndef ZOAL_ARCH_STM32X_USART_HPP
#define ZOAL_ARCH_STM32X_USART_HPP

#include "../../../mem/reg.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address, class... Mixin>
    class usart : public Mixin... {
    public:
        static constexpr uintptr_t address = Address;

        static constexpr uint32_t USARTx_ISR_TXE = 1 << 7; // TXE: Transmit data register empty
        static constexpr uint32_t USARTx_ISR_RXNE = 1 << 5; // Receive data register not empty (receivers)
        static constexpr uint32_t USARTx_CR1_RXNEIE = 1 << 5; // RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_TXEIE = 1 << 7; // TXEIE: interrupt enable
        static constexpr uint32_t USARTx_CR1_UE = 1 << 0; // USART enable

        using USARTx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR3 = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_BRR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_GTPR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_RTOR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_RQR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_ISR = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_ICR = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_RDR = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_TDR = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        using self_type = usart<Address, Mixin...>;

        static void enable() {
            USARTx_CR1::ref() |= USARTx_CR1_UE;
        }

        static void disable() {
            USARTx_CR1::ref() &= ~USARTx_CR1_UE;
        }

        static void enable_tx() {
            USARTx_CR1::ref() |= USARTx_CR1_TXEIE;
        }

        static void disable_tx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_TXEIE;
        }

        static void enable_rx() {
            USARTx_CR1::ref() |= USARTx_CR1_RXNEIE;
        }

        static void disable_rx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_RXNEIE;
        }

        template<class RxCallback>
        static void rx_handler(RxCallback rx_callback) {
            auto rx_enabled = USARTx_CR1::ref() & USARTx_CR1_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = USARTx_ISR::ref() & USARTx_ISR_RXNE;
            if (!rx_not_empty) {
                return;
            }

            rx_callback(USARTx_RDR::ref());
        }

        template<class TxCallback>
        static bool tx_handler(TxCallback callback) {
            auto tx_enabled = USARTx_CR1::ref() & USARTx_CR1_TXEIE;
            if (!tx_enabled) {
                return false;
            }

            auto tx_empty = USARTx_ISR::ref() & USARTx_ISR_TXE;
            if (!tx_empty) {
                return false;
            }

            uint8_t value;
            if (callback(value)) {
                USARTx_TDR::ref() = value;
            } else {
                disable_tx();
            }

            return true;
        }
    };
}}}

#endif
