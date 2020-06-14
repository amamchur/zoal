#ifndef ZOAL_ARCH_STM32X_USART_HPP
#define ZOAL_ARCH_STM32X_USART_HPP

#include "../../../data/ring_buffer_ext.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../utils/cooperation.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address, class... Mixin>
    class usart : public Mixin... {
    public:
        static constexpr uintptr_t address = Address;

        static constexpr uint32_t USARTx_ISR_bit_TXE = 1 << 7; // Bit 7 TXE: Transmit data register empty
        static constexpr uint32_t USARTx_ISR_bit_RXNE = 1 << 5;
        static constexpr uint32_t USARTx_CR1_bit_RXNEIE = 1 << 5; // Bit 5 RXNE: Read data register not empty
        static constexpr uint32_t USARTx_CR1_bit_TXEIE = 1 << 7; // Bit 7 TXEIE: interrupt enable
        static constexpr uint32_t USARTx_CR1_bit_UE = 1 << 0; // Bit 0 UE: USART enable

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

        class tx_fifo_control {
        public:
            using scope_lock = zoal::utils::interrupts_off;

            static inline void item_added() {
                self_type::enable_tx();
            }

            static inline void item_removed() {}
        };

        class rx_fifo_control {
        public:
            using scope_lock = zoal::utils::interrupts_off;

            static inline void item_added() {
                self_type::enable_rx();
            }

            static inline void item_removed() {}
        };

        template<size_t Size>
        using default_tx_buffer = zoal::data::static_blocking_fifo_buffer<uint8_t, Size, tx_fifo_control>;

        template<size_t Size>
        using default_rx_buffer = zoal::data::static_blocking_fifo_buffer<uint8_t, Size, rx_fifo_control>;

        using null_tx_buffer = zoal::data::null_fifo_buffer<uint8_t>;
        using null_rx_buffer = zoal::data::null_fifo_buffer<uint8_t>;

        static void enable() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_UE;
        }

        static void disable() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_UE;
        }

        static void enable_tx() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_TXEIE;
        }

        static void disable_tx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_TXEIE;
        }

        static void enable_rx() {
            USARTx_CR1::ref() |= USARTx_CR1_bit_RXNEIE;
        }

        static void disable_rx() {
            USARTx_CR1::ref() &= ~USARTx_CR1_bit_RXNEIE;
        }

        static inline void flush() {}

        template<class Buffer>
        static inline void rx_handler() {
            auto rx_enabled = USARTx_CR1::ref() & USARTx_CR1_bit_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = USARTx_ISR::ref() & USARTx_ISR_bit_RXNE;
            if (!rx_not_empty) {
                return;
            }

            Buffer::push_back(USARTx_RDR::ref());
        }

        template<class Buffer>
        static void tx_handler() {
            auto tx_enabled = USARTx_CR1::ref() & USARTx_CR1_bit_TXEIE;
            if (!tx_enabled) {
                return;
            }

            auto tx_empty = USARTx_ISR::ref() & USARTx_ISR_bit_TXE;
            if (!tx_empty) {
                return;
            }

            typename Buffer::value_type value;
            if (Buffer::pop_front(value)) {
                USARTx_TDR::ref() = value;
            } else {
                disable_tx();
            }
        }
    };
}}}

#endif
