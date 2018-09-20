#ifndef ZOAL_ARCH_AVR_ATMEGA_USART_HPP
#define ZOAL_ARCH_AVR_ATMEGA_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../io/stream_functor.hpp"
#include "zoal/periph/usart.hpp"
#include "../../../utils/interrupts.hpp"
#include "zoal/mem/segment.hpp"
#include "../../bus.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address, uint8_t N, class Buffer>
    class usart {
    private:
        enum UCSRxA_Flags : uint8_t
        {
            RXCx = 7,
            TXCx = 6,
            UDREx = 5,
            FEx = 4,
            DORx = 3,
            UPEx = 2,
            U2Xx = 1,
            MPCMx = 0
        };

        enum UCSRxB_Flags : uint8_t
        {
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
        using buffer_type = Buffer;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        static constexpr intptr_t UCSRxA = 0;
        static constexpr intptr_t UCSRxB = 1;
        static constexpr intptr_t UCSRxC = 2;
        static constexpr intptr_t UCSRxD = 3; // ATmega32U4 only
        static constexpr intptr_t UBRRxL = 4;
        static constexpr intptr_t UBRRxH = 5;
        static constexpr intptr_t UDRx = 6;

        static buffer_type buffer;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            mem[UCSRxB] |= static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void disable() {
            mem[UCSRxB] &= ~static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void flush() {}

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            buffer.tx.enqueue(data, true);
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
            auto f = static_cast<F &>(fn);
            while (f(buffer.rx.dequeue(true))) {
            }
        }

        static void handle_tx_irq() {
            mem[UDRx] = buffer.tx.dequeue();
            mem[UCSRxA] |= (1 << TXCx);

            if (buffer.tx.empty()) {
                mem[UCSRxB] &= ~(1 << UDRIEx);
            }
        }

        static void handle_rx_irq() {
            uint8_t data = mem[UDRx];
            if (mem[UCSRxA] & (1 << UPEx)) {
                return;
            }

            buffer.rx.enqueue(data);
        }

    private:
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, class Buffer>
    typename usart<Address, N, Buffer>::buffer_type usart<Address, N, Buffer>::buffer;

    template<uintptr_t Address, uint8_t N, class Buffer>
    zoal::mem::segment<uint8_t, Address> usart<Address, N, Buffer>::mem;
}}}}

#endif
