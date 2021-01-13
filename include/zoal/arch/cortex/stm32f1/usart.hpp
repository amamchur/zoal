#ifndef ZOAL_ARCH_STM32F1_USART_HPP
#define ZOAL_ARCH_STM32F1_USART_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../periph/usart.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class... Mixin>
    class usart : public Mixin... {
    public:
        using self_type = usart<Address, Mixin...>;
        static constexpr uintptr_t address = Address;

        using USARTx_SR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_DR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_BRR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR1 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR2 = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_CR3 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using USARTx_GTPR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uint32_t USARTx_CR1_TE = 1 << 3; // Bit 3 TE: Transmitter enable
        static constexpr uint32_t USARTx_CR1_RE = 1 << 2; // Bit 2 RE: Receiver enable
        static constexpr uint32_t USARTx_SR_bit_RXNE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_SR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
        static constexpr uint32_t USARTx_CR1_bit_UE = 1 << 13; // Bit 13 UE: USART enable
        static constexpr uint32_t USARTx_CR1_bit_RXNEIE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_bit_TXEIE = 1 << 7; // Bit 7 TXEIE: interrupt enable

        using enable_cas = zoal::ct::type_list<typename self_type::USARTx_CR1::template cas<0, USARTx_CR1_bit_UE>>;
        using disable_cas = zoal::ct::type_list<typename self_type::USARTx_CR1::template cas<USARTx_CR1_bit_UE, 0>>;

        static inline void enable() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_UE;
        }

        static inline void disable() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_UE;
        }

        static inline void enable_tx() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_TXEIE;
        }

        static inline void disable_tx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_TXEIE;
        }

        static inline void enable_rx() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_RXNEIE;
        }

        static inline void disable_rx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_RXNEIE;
        }

        template<class RxCallback>
        static inline void rx_handler(RxCallback rx_callback) {
            auto rx_enabled = USARTx_CR1::ref() & USARTx_CR1_bit_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = USARTx_SR::ref() & USARTx_SR_bit_RXNE;
            if (!rx_not_empty) {
                return;
            }

            rx_callback(USARTx_DR::ref());
        }

        template<class TxCallback>
        static bool tx_handler(TxCallback callback) {
            auto tx_enabled = USARTx_CR1::ref() & USARTx_CR1_bit_TXEIE;
            if (!tx_enabled) {
                return false;
            }

            auto tx_empty = USARTx_SR::ref() & USARTx_SR_bit_TXE;
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
