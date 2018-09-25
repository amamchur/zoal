#ifndef ZOAL_ARCH_STM32F1_USART_HPP
#define ZOAL_ARCH_STM32F1_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    class usart : public Mixin... {
    public:
        using buffer_type = Buffer;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint32_t, Address, Offset>;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static buffer_type buffer;

        static constexpr uintptr_t USARTx_SR = 0x00;
        static constexpr uintptr_t USARTx_DR = 0x04;
        static constexpr uintptr_t USARTx_BRR = 0x08;
        static constexpr uintptr_t USARTx_CR1 = 0x0C;
        static constexpr uintptr_t USARTx_CR2 = 0x10;
        static constexpr uintptr_t USARTx_CR3 = 0x14;
        static constexpr uintptr_t USARTx_GTPR = 0x18;

        static constexpr uint32_t USARTx_SR_bit_RXNE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_SR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
        static constexpr uint32_t USARTx_CR1_bit_UE = 1 << 13; // Bit 13 UE: USART enable
        static constexpr uint32_t USARTx_CR1_bit_RXNEIE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_bit_TXEIE = 1 << 7; // Bit 7 TXEIE: interrupt enable

        static void enable() {
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_UE;
        }

        static void disable() {
            *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_UE;
        }

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            buffer.tx.enqueue(data, true);
            *accessor<USARTx_CR1>::p |= USARTx_CR1_bit_TXEIE;
        }

        static inline void flush() {}

        static void handleIrq() {
            if ((*accessor<USARTx_SR>::p & USARTx_SR_bit_TXE) != 0) {
                if (buffer.tx.empty()) {
                    *accessor<USARTx_CR1>::p &= ~USARTx_CR1_bit_TXEIE;
                } else {
                    *accessor<USARTx_DR>::p = buffer.tx.dequeue();
                }
            }
        }
    };

    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    typename usart<Address, N, Buffer, Mixin...>::buffer_type usart<Address, N, Buffer, Mixin...>::buffer;
}}}

#endif
