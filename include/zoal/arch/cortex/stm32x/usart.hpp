#ifndef ZOAL_ARCH_STM32X_USART_HPP
#define ZOAL_ARCH_STM32X_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../utils/cooperation.hpp"
#include "../../../utils/interrupts.hpp"
#include "zoal/mem/segment.hpp"
#include "../../../utils/nop.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    class usart : public Mixin... {
    public:
        using buffer_type = Buffer;

        static constexpr uintptr_t address = Address;

        static constexpr uint32_t USARTx_ISR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
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
            mem[USARTx_CR1] |= USARTx_CR1_bit_UE;
        }

        static void disable() {
            mem[USARTx_CR1] &= ~USARTx_CR1_bit_UE;
        }

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            buffer.tx.enqueue(data, true);
            mem[USARTx_CR1] |= USARTx_CR1_bit_TXEIE;
        }

        template<class F>
        static void write(::zoal::io::output_stream_functor<F> &fn) {
            uint8_t data = 0;
            while (static_cast<F &>(fn)(data)) {
                write_byte(data);
            }
        }

        static inline void flush() {}

        static void handleIrq() {
            if ((mem[USARTx_ISR] & USARTx_ISR_bit_TXE) != 0) {
                if (buffer.tx.empty()) {
                    mem[USARTx_CR1] &= ~USARTx_CR1_bit_TXEIE;
                } else {
                    mem[USARTx_TDR] = buffer.tx.dequeue();
                }
            }
        }

    private:
        static zoal::mem::segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    zoal::mem::segment<uint32_t, Address> usart<Address, N, Buffer, Mixin...>::mem;

    template<uintptr_t Address, uint8_t N, class Buffer, class... Mixin>
    typename usart<Address, N, Buffer, Mixin...>::buffer_type usart<Address, N, Buffer, Mixin...>::buffer;
}}}

#endif
