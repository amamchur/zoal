#ifndef ZOAL_ARCH_AVR_ATMEGA_USART_HPP
#define ZOAL_ARCH_AVR_ATMEGA_USART_HPP

#include "../../../data/ring_buffer_ext.hpp"
#include "../../../io/stream_functor.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../bus.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address>
    class usart {
    private:
        enum UCSRxA_Flags : uint8_t { RXCx = 7, TXCx = 6, UDREx = 5, FEx = 4, DORx = 3, UPEx = 2, U2Xx = 1, MPCMx = 0 };

        enum UCSRxB_Flags : uint8_t { RXCIEx = 7, TXCIEx = 6, UDRIEx = 5, RXENx = 4, TXENx = 3, UCSZx2 = 2, RXB8x = 1, TXB8x = 0 };

    public:
        using self_type = usart<Address>;

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

            static inline void item_added() {}

            static inline void item_removed() {}
        };

        template<size_t Size>
        using default_tx_buffer = zoal::data::static_blocking_fifo_buffer<uint8_t, Size, tx_fifo_control>;

        template<size_t Size>
        using default_rx_buffer = zoal::data::static_blocking_fifo_buffer<uint8_t, Size, rx_fifo_control>;

        using null_tx_buffer = zoal::data::null_fifo_buffer<uint8_t>;
        using null_rx_buffer = zoal::data::null_fifo_buffer<uint8_t>;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;

        using SPCRx = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

        using UCSRxA = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxB = zoal::mem::reg<Address + 0x01, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxC = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxD = zoal::mem::reg<Address + 0x03, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UBRRxL = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UBRRxH = zoal::mem::reg<Address + 0x05, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using UDRx = zoal::mem::reg<Address + 0x06, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            UCSRxB::ref() |= static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void disable() {
            UCSRxB::ref() &= ~static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static inline void enable_tx() {
            UCSRxB::ref() |= 1 << UDRIEx;
        }

        static inline void disable_tx() {
            UCSRxB::ref() &= ~(1 << UDRIEx);
        }

        static void flush() {}

        template<class Buffer>
        static void rx_handler() {
            if (UCSRxA::ref() & (1 << UPEx)) {
                return;
            }

            Buffer::push_back(UDRx::ref());
        }

        template<class Buffer>
        static void tx_handler() {
            typename Buffer::value_type value;
            if (Buffer::pop_front(value)) {
                UDRx::ref() = value;
                UCSRxA::ref() |= (1 << TXCx);
            } else {
                disable_tx();
            }
        }
    };
}}}}

#endif
