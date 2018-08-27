#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_USART_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_USART_HPP

#include "../../../utils/interrupts.hpp"
#include "../../../utils/yield.hpp"
#include "../../../utils/memory_segment.hpp"
#include "../../../data/ring_buffer.hpp"
#include "../../../periph/usart_config.hpp"

namespace zoal { namespace arch { namespace avr {
    template<::zoal::periph::usart_data_bits Bits>
    struct usart_data_bits_flags {
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_5> {
        static constexpr uint8_t Flags = 0 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_6> {
        static constexpr uint8_t Flags = 1 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_7> {
        static constexpr uint8_t Flags = 2 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_8> {
        static constexpr uint8_t Flags = 3 << 1;
    };

    template<::zoal::periph::usart_parity Parity>
    struct usart_parity_flags {
        static constexpr uint8_t Flags = 0;
    };

    template<>
    struct usart_parity_flags<::zoal::periph::usart_parity::even> {
        static constexpr uint8_t Flags = 2 << 4;
    };

    template<>
    struct usart_parity_flags<::zoal::periph::usart_parity::odd> {
        static constexpr uint8_t Flags = 3 << 4;
    };

    template<::zoal::periph::usart_stop_bits StopBits>
    struct usart_stop_bit_flags {
        static constexpr uint8_t Flags = 0;
    };

    template<>
    struct usart_stop_bit_flags<::zoal::periph::usart_stop_bits::stop_bits_2> {
        static constexpr uint8_t Flags = 1 << 3;
    };

    template<class Config>
    struct usart_mode {
        static constexpr uint8_t UCSRxC = static_cast<uint8_t >(0)
                                          | usart_data_bits_flags<Config::data_bits>::Flags
                                          | usart_parity_flags<Config::parity>::Flags
                                          | usart_stop_bit_flags<Config::stop_bits>::Flags;
    };

    template<uintptr_t Address, uintptr_t TxSize, uintptr_t RxSize>
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
        using buffer_rx = buffer<TxSize>;

        static buffer_tx tx;
        static buffer_rx rx;

        template<class Config>
        static void setup() {
            uint32_t value = (Config::mcu_freq / (8 * Config::baud_rate)) - 1;
            uint8_t ucsra = 1u << U2Xx;
            if (value > 4095) {
                ucsra = 0;
                value = (Config::mcu_freq / (16 * Config::baud_rate)) - 1;
            }

            mem[UBRRxL] = static_cast<uint8_t>(value & 0xFFu);
            mem[UBRRxH] = static_cast<uint8_t>(value >> 0x8u);
            mem[UCSRxA] = ucsra;
            mem[UCSRxB] = static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
            mem[UCSRxC] = usart_mode<Config>::UCSRxC;
        }

        static inline void flush() {
        }

        static void write(uint8_t data) {
            zoal::utils::interrupts ni(false);
            tx.enqueue(data, true);
            mem[UCSRxB] |= 1 << UDRIEx;
        }

        template<class F>
        static void write(F &fn) {
            uint8_t data = 0;
            while (fn(data)) {
                write(data);
            }
        }

        template<class F>
        static void read(F &fn) {
            while (fn(rx.dequeue(true)));
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
            if (mem[UCSRxA] & 1 << UPEx) {
                return;
            }

            rx.enqueue(data);
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uintptr_t TxSize, uintptr_t RxSize>
    typename usart<Address, TxSize, RxSize>::buffer_tx usart<Address, TxSize, RxSize>::tx;

    template<uintptr_t Address, uintptr_t TxSize, uintptr_t RxSize>
    typename usart<Address, TxSize, RxSize>::buffer_rx usart<Address, TxSize, RxSize>::rx;

    template<uintptr_t Address, uintptr_t TxSize, uintptr_t RxSize>
    zoal::utils::memory_segment<uint8_t, Address> usart<Address, TxSize, RxSize>::mem;

}}}

#endif
