#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_USART_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_USART_HPP

#include "../../../io/stream_functor.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/yield.hpp"
#include "../../../utils/memory_segment.hpp"
#include "../../../data/ring_buffer.hpp"
#include "../../../periph/usart_config.hpp"
#include "../../bus.hpp"

namespace zoal { namespace arch { namespace avr {
    template<::zoal::periph::usart_data_bits Bits>
    struct atmega_usart_data_bits_flags {
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_5> {
        static constexpr uint8_t flags = 0 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_6> {
        static constexpr uint8_t flags = 1 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_7> {
        static constexpr uint8_t flags = 2 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_8> {
        static constexpr uint8_t flags = 3 << 1;
    };

    template<::zoal::periph::usart_parity Parity>
    struct atmega_usart_parity_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::even> {
        static constexpr uint8_t flags = 2 << 4;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::odd> {
        static constexpr uint8_t flags = 3 << 4;
    };

    template<::zoal::periph::usart_stop_bits StopBits>
    struct atmega_usart_stop_bit_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_stop_bit_flags<::zoal::periph::usart_stop_bits::stop_bits_2> {
        static constexpr uint8_t flags = 1 << 3;
    };

    template<class Config>
    struct atmega_usart_mode {
        static constexpr uint8_t UCSRxC = static_cast<uint8_t >(0)
                                          | atmega_usart_data_bits_flags<Config::data_bits>::flags
                                          | atmega_usart_parity_flags<Config::parity>::flags
                                          | atmega_usart_stop_bit_flags<Config::stop_bits>::flags;
    };

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize>
    class usart {
    private:
        enum UCSRxA_Flags : uint8_t {
            RXCx = 7,
            TXCx = 6,
            UDREx = 5,
            FEx = 4,
            DORx = 3,
            UPEx = 2,
            U2Xx = 1,
            MPCMx = 0
        };

        enum UCSRxB_Flags : uint8_t {
            RXCIEx = 7,
            TXCIEx = 6,
            UDRIEx = 5,
            RXENx = 4,
            TXENx = 3,
            UCSZx2 = 2,
            RXB8x = 1,
            TXB8x = 0
        };
    public:
        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        using yield = zoal::utils::yield<>;

        template<uintptr_t Size>
        using buffer = typename ::zoal::data::ring_buffer<uint8_t, Size, yield::place>;

        static constexpr uintptr_t UCSRxA = 0;
        static constexpr uintptr_t UCSRxB = 1;
        static constexpr uintptr_t UCSRxC = 2;
        static constexpr uintptr_t UCSRxD = 3; // ATmega32U4 only
        static constexpr uintptr_t UBRRxL = 4;
        static constexpr uintptr_t UBRRxH = 5;
        static constexpr uintptr_t UDRx = 6;

        using buffer_tx = buffer<TxSize>;
        using buffer_rx = buffer<RxSize>;

        static buffer_tx tx;
        static buffer_rx rx;

        static void power_on() {
        }

        static void power_off() {
        }

        static void enable() {
            mem[UCSRxB] |= static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);;
        }

        static void disable() {
            mem[UCSRxB] &= ~static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static inline void flush() {
        }

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            tx.enqueue(data, true);
            mem[UCSRxB] |= 1 << UDRIEx;
        }

        template<class F>
        static void write(::zoal::io::output_stream_functor<F> &fn) {
            uint8_t data = 0;
            while (static_cast<F &>(fn)(data)) {
                write_byte(data);
            }
        }

        template<class F>
        static void read(::zoal::io::input_stream_functor<F> &fn) {
            while (static_cast<F &>(fn)(rx.dequeue(true)));
        }

        static void handle_tx_irq() {
            mem[UDRx] = tx.dequeue();
            mem[UCSRxA] |= (1 << TXCx);

            if (tx.empty()) {
                mem[UCSRxB] &= ~(1 << UDRIEx);
            }
        }

        static void handle_rx_irq() {
            uint8_t data = mem[UDRx];
            if (mem[UCSRxA] & (1 << UPEx)) {
                return;
            }

            rx.enqueue(data);
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize>
    typename usart<Address, N, TxSize, RxSize>::buffer_tx usart<Address, N, TxSize, RxSize>::tx;

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize>
    typename usart<Address, N, TxSize, RxSize>::buffer_rx usart<Address, N, TxSize, RxSize>::rx;

    template<uintptr_t Address, uint8_t N, uintptr_t TxSize, uintptr_t RxSize>
    zoal::utils::memory_segment<uint8_t, Address> usart<Address, N, TxSize, RxSize>::mem;

}}}

#endif
