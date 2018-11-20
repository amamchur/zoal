#ifndef ZOAL_ARCH_AVR_ATMEGA_USART_HPP
#define ZOAL_ARCH_AVR_ATMEGA_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../bus.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address>
    class usart {
    private:
        enum UCSRxA_Flags : uint8_t {
            RXCx = 7, TXCx = 6, UDREx = 5, FEx = 4, DORx = 3, UPEx = 2, U2Xx = 1, MPCMx = 0
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
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;

        static constexpr intptr_t UCSRxA = 0;
        static constexpr intptr_t UCSRxB = 1;
        static constexpr intptr_t UCSRxC = 2;
        static constexpr intptr_t UCSRxD = 3; // ATmega32U4 only
        static constexpr intptr_t UBRRxL = 4;
        static constexpr intptr_t UBRRxH = 5;
        static constexpr intptr_t UDRx = 6;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            *accessor<UCSRxB>::p |= static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void disable() {
            *accessor<UCSRxB>::p &= ~static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static inline void enable_tx() {
            *accessor<UCSRxB>::p |= 1 << UDRIEx;
        }

        static inline void disable_tx() {
            *accessor<UCSRxB>::p &= ~(1 << UDRIEx);
        }

        static void flush() {}

        //        static void write_byte(uint8_t data) {
        //            zoal::utils::interrupts ni(false);
        //            buffer.tx.enqueue(data, true);
        //            *accessor<UCSRxB>::p |= 1 << UDRIEx;
        //        }

        //        template<class F>
        //        static void read(::zoal::io::input_stream_functor<F> &fn) {
        //            auto f = static_cast<F &>(fn);
        //            while (f(buffer.rx.dequeue(true))) continue;
        //        }

        //        static void handle_tx_irq() {
        //            *accessor<UDRx>::p = buffer.tx.dequeue();
        //            *accessor<UCSRxA>::p |= (1 << TXCx);
        //
        //            if (buffer.tx.empty()) {
        //                *accessor<UCSRxB>::p &= ~(1 << UDRIEx);
        //            }
        //        }
        //
        //        static void handle_rx_irq() {
        //            if (*accessor<UCSRxA>::p & (1 << UPEx)) {
        //                return;
        //            }
        //
        //            buffer.rx.enqueue(*accessor<UDRx>::p);
        //        }

        template<class H>
        static inline void rx_handler() {
            if (*accessor<UCSRxA>::p & (1 << UPEx)) {
                return;
            }

            H::put(*accessor<UDRx>::p);
        }

        template<class H>
        static void tx_handler() {
            if (H::empty()) {
                disable_tx();
                return;
            }

            *accessor<UDRx>::p = H::get();
            *accessor<UCSRxA>::p |= (1 << TXCx);

            if (H::empty()) {
                *accessor<UCSRxB>::p &= ~(1 << UDRIEx);
            }
        }
    };
}}}}

#endif
