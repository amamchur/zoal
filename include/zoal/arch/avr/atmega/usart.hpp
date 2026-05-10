#ifndef ZOAL_ARCH_AVR_ATMEGA_USART_HPP
#define ZOAL_ARCH_AVR_ATMEGA_USART_HPP

#include "../../../periph/usart.hpp"
#include "../../bus.hpp"

namespace zoal::arch::avr::atmega {
    template<uintptr_t Address>
    class usart {
    private:
        enum UCSRxA_Flags : uint8_t { RXCx = 7, TXCx = 6, UDREx = 5, FEx = 4, DORx = 3, UPEx = 2, U2Xx = 1, MPCMx = 0 };

        enum UCSRxB_Flags : uint8_t { RXCIEx = 7, TXCIEx = 6, UDRIEx = 5, RXENx = 4, TXENx = 3, UCSZx2 = 2, RXB8x = 1, TXB8x = 0 };

    public:
        using self_type = usart;

        static constexpr auto bus = zoal::arch::bus::common;
        static constexpr auto address = Address;

        using UCSRxA = mem::reg<Address + 0x00, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxB = mem::reg<Address + 0x01, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxC = mem::reg<Address + 0x02, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UCSRxD = mem::reg<Address + 0x03, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UBRRxL = mem::reg<Address + 0x04, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UBRRxH = mem::reg<Address + 0x05, mem::reg_io::read_write, uint8_t, 0xFF>;
        using UDRx = mem::reg<Address + 0x06, mem::reg_io::read_write, uint8_t, 0xFF>;

        using enable_cas =  type_list<typename UCSRxB::template cas<0, 1u << TXENx | 1u << RXENx | 1u << RXCIEx>>;
        using disable_cas =  type_list<typename UCSRxB::template cas<1u << TXENx | 1u << RXENx | 1u << RXCIEx, 0>>;
        using clock_on_cas = mem::null_cas_list;
        using clock_off_cas = mem::null_cas_list;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            UCSRxB::ref() |= static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void disable() {
            UCSRxB::ref() &= ~static_cast<uint8_t>(1u << TXENx | 1u << RXENx | 1u << RXCIEx);
        }

        static void enable_tx() {
            UCSRxB::ref() |= 1 << UDRIEx;
        }

        static void disable_tx() {
            UCSRxB::ref() &= ~(1 << UDRIEx);
        }

        template<class TxCallback>
        static void rx_handler(TxCallback callback) {
            if (UCSRxA::ref() & (1 << UPEx)) {
                return;
            }

            callback(UDRx::ref());
        }

        template<class TxCallback>
        static void tx_handler(TxCallback callback) {
            uint8_t value;
            if (callback(value)) {
                UDRx::ref() = value;
                UCSRxA::ref() |= (1 << TXCx);
            } else {
                disable_tx();
            }
        }
    };
}

#endif
