#ifndef ZOAL_ARCH_STM32X_USART_HPP
#define ZOAL_ARCH_STM32X_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../utils/cooperation.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    class usart : public Mixin... {
    public:
        using buffer_type = Buffer;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint32_t, Address, Offset>;

        static constexpr uintptr_t address = Address;

        static constexpr uint32_t USARTx_ISR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
        static constexpr uint32_t USARTx_ISR_bit_RXNE = 1 << 5;
        static constexpr uint32_t USARTx_CR1_bit_RXNEIE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_bit_TXEIE = 1 << 7; // Bit 7 TXEIE: interrupt enable
        static constexpr uint32_t USARTx_CR1_bit_UE = 1 << 0; // Bit 0 UE: USART enable

        static constexpr uintptr_t USARTx_CR1 = 0x00;
        static constexpr uintptr_t USARTx_CR2 = 0x04;
        static constexpr uintptr_t USARTx_CR3 = 0x08;
        static constexpr uintptr_t USARTx_BRR = 0x0C;
        static constexpr uintptr_t USARTx_GTPR = 0x10;
        static constexpr uintptr_t USARTx_RTOR = 0x14;
        static constexpr uintptr_t USARTx_RQR = 0x18;
        static constexpr uintptr_t USARTx_ISR = 0x1C;
        static constexpr uintptr_t USARTx_ICR = 0x20;
        static constexpr uintptr_t USARTx_RDR = 0x24;
        static constexpr uintptr_t USARTx_TDR = 0x28;

        static constexpr uint8_t no = N;

        static buffer_type buffer;

        static void enable() {
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_UE;
        }

        static void disable() {
            *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_UE;
        }

        static void enable_tx() {
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_TXEIE;
        }

        static void disable_tx() {
            *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_TXEIE;
        }

        static void enable_rx() {
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_RXNEIE;
        }

        static void disable_rx() {
            *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_RXNEIE;
        }

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            buffer.tx.enqueue(data, true);
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_TXEIE;
        }

        static inline void flush() {}

        template<class H>
        static void rx_handler() {
            auto rx_enabled = *accessor<USARTx_CR1>::p & USARTx_CR1_bit_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = *accessor<USARTx_ISR>::p & USARTx_ISR_bit_RXNE;
            if (!rx_not_empty) {
                return;
            }

            H::put(*accessor<USARTx_RDR>::p);
        }

        template<class H>
        static void tx_handler() {
            if (H::empty()) {
                disable_tx();
                return;
            }

            auto tx_enabled = *accessor<USARTx_CR1>::p & USARTx_CR1_bit_TXEIE;
            if (!tx_enabled) {
                return;
            }

            auto tx_empty = *accessor<USARTx_ISR>::p & USARTx_ISR_bit_TXE;
            if (!tx_empty) {
                return;
            }

            *accessor<USARTx_TDR>::p = H::get();
        }

        static void handleIrq() {
            if ((*accessor<USARTx_ISR>::p & USARTx_ISR_bit_TXE) != 0) {
                if (buffer.tx.empty()) {
                    *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_TXEIE;
                } else {
                    *accessor<USARTx_TDR>::p = buffer.tx.dequeue();
                }
            }
        }
    };

    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    typename usart<Address, N, Buffer, Mixin...>::buffer_type usart<Address, N, Buffer, Mixin...>::buffer;
}}}

#endif
