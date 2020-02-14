#ifndef ZOAL_ARCH_STM32F1_USART_HPP
#define ZOAL_ARCH_STM32F1_USART_HPP

#include "../../../data/ring_buffer_ext.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class... Mixin>
    class usart : public Mixin... {
    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint32_t, Address, Offset>;

        static constexpr uintptr_t address = Address;

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

        static inline void enable() {
            accessor<USARTx_CR1>::ref() |= USARTx_CR1_bit_UE;
        }

        static inline void disable() {
            accessor<USARTx_CR1>::ref() &= ~USARTx_CR1_bit_UE;
        }

        static inline void enable_tx() {
            accessor<USARTx_CR1>::ref() |= USARTx_CR1_bit_TXEIE;
        }

        static inline void disable_tx() {
            accessor<USARTx_CR1>::ref() &= ~USARTx_CR1_bit_TXEIE;
        }

        static inline void enable_rx() {
            accessor<USARTx_CR1>::ref() |= USARTx_CR1_bit_RXNEIE;
        }

        static inline void flush() {}

        template<class H>
        static void rx_handler() {
            auto rx_enabled = accessor<USARTx_CR1>::ref() & USARTx_CR1_bit_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = accessor<USARTx_SR>::ref() & USARTx_SR_bit_RXNE;
            if (!rx_not_empty) {
                return;
            }

            H::put(accessor<USARTx_DR>::ref());
        }

        template<class H>
        static inline void tx_handler() {
            if (H::empty()) {
                disable_tx();
                return;
            }

            auto tx_enabled = accessor<USARTx_CR1>::ref() & USARTx_CR1_bit_TXEIE;
            if (!tx_enabled) {
                return;
            }

            auto tx_empty = accessor<USARTx_SR>::ref() & USARTx_SR_bit_TXE;
            if (!tx_empty) {
                return;
            }

            accessor<USARTx_DR>::ref() = H::get();
        }

        template<class Buffer>
        static inline void rx_handler_v2() {
            auto rx_enabled = accessor<USARTx_CR1>::ref() & USARTx_CR1_bit_RXNEIE;
            if (!rx_enabled) {
                return;
            }

            auto rx_not_empty = accessor<USARTx_SR>::ref() & USARTx_SR_bit_RXNE;
            if (!rx_not_empty) {
                return;
            }

            Buffer::push_back(accessor<USARTx_DR>::ref());
        }

        template<class Buffer>
        static void tx_handler_v2() {
            auto tx_enabled = accessor<USARTx_CR1>::ref() & USARTx_CR1_bit_TXEIE;
            if (!tx_enabled) {
                return;
            }

            auto tx_empty = accessor<USARTx_SR>::ref() & USARTx_SR_bit_TXE;
            if (!tx_empty) {
                return;
            }

            typename Buffer::value_type value;
            if (Buffer::pop_front(value)) {
                accessor<USARTx_DR>::ref() = value;
            } else {
                disable_tx();
            }
        }
    };
}}}

#endif
