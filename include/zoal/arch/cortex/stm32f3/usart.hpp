#ifndef ZOAL_ARCH_STM32F3_USART_HPP
#define ZOAL_ARCH_STM32F3_USART_HPP

#include <stdint.h>
#include "../../../data/ring_buffer.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../utils/memory_segment.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"
#include "../../../utils/yield.hpp"

namespace zoal { namespace arch { namespace stm32f3 {
    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize, class ... Mixin>
    class usart : public Mixin ... {
    public:
        static constexpr uint32_t USARTx_ISR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
        static constexpr uint32_t USARTx_CR1_bit_TXEIE = 1 << 7; // Bit 7 TXEIE: interrupt enable

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


        using yield = zoal::utils::yield<>;

        template<uintptr_t Size>
        using buffer = typename ::zoal::data::ring_buffer<uint8_t, Size, yield::place>;
        using buffer_tx = buffer<TxSize>;
        using buffer_rx = buffer<RxSize>;

        static buffer_tx tx;
        static buffer_rx rx;

        template<class Config>
        static void begin() {

        }

        static void write(uint8_t data) {
            zoal::utils::interrupts ni(false);
            tx.enqueue(data, true);
            mem[USARTx_CR1] |= USARTx_CR1_bit_TXEIE;
        }

        template<class F>
        static void write(::zoal::io::output_stream_functor<F> &fn) {
            uint8_t data = 0;
            while (static_cast<F &>(fn)(data)) {
                write(data);
            }
        }

        static inline void flush() {
        }

        static void handleIrq() {
            if ((mem[USARTx_ISR] & USARTx_ISR_bit_TXE) != 0) {
                if (tx.empty()) {
                    mem[USARTx_CR1] &= ~USARTx_CR1_bit_TXEIE;
                } else {
                    mem[USARTx_TDR] = tx.dequeue();
                }
            }
        }

    private:
        static zoal::utils::memory_segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize, class ... Mixin>
    zoal::utils::memory_segment<uint32_t, Address> usart<Address, N, TxSize, RxSize, Mixin...>::mem;

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize, class ... Mixin>
    typename usart<Address, N, TxSize, RxSize, Mixin...>::buffer_tx usart<Address, N, TxSize, RxSize, Mixin...>::tx;

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize, class ... Mixin>
    typename usart<Address, N, TxSize, RxSize, Mixin...>::buffer_rx usart<Address, N, TxSize, RxSize, Mixin...>::rx;
}}}

#endif
