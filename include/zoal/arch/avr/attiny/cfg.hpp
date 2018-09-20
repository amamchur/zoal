#ifndef ZOAL_ARCH_AVR_ATTINY_CFG_HPP
#define ZOAL_ARCH_AVR_ATTINY_CFG_HPP

#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/segment.hpp"
#include "zoal/periph/adc.hpp"
#include "../../../periph/timer_mode.hpp"
#include "zoal/periph/usart.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace metadata {
}}

namespace zoal { namespace arch { namespace avr { namespace attiny {
    using zoal::mem::clear_and_set;
    using zoal::mem::merge_clear_and_set;
    using zoal::mem::segment;
    using zoal::periph::usart_parity;
    using zoal::periph::usart_stop_bits;

    template<class Api, uint32_t Frequency>
    class cfg {
    public:
        static constexpr auto frequency = Frequency;

        template<class U,
                 uint32_t BaudRate,
                 uint8_t Bits = 8,
                 usart_parity Parity = usart_parity::none,
                 usart_stop_bits StopBits = usart_stop_bits::stop_bits_1,
                 uint32_t Freq = frequency>
        class usart {
        public:
            static constexpr uint8_t UCSRxA_bit_U2X = 1 << 1;
            static constexpr auto value_x2 = (Freq / (8 * BaudRate)) - 1;
            static constexpr auto value_x1 = (Freq / (16 * BaudRate)) - 1;
            static constexpr auto value = value_x2 > 4095 ? value_x1 : value_x2;
            static constexpr uint8_t UCSRxA_value = static_cast<const uint8_t>(value_x2 > 4095 ? 0u : UCSRxA_bit_U2X);
            static constexpr uint8_t UBRRxL_value = static_cast<uint8_t>(value & 0xFFu);
            static constexpr uint8_t UBRRxH_value = static_cast<uint8_t>(value >> 0x8u);

            static void apply() {
                U::disable();
            }
        };

        template<class T, zoal::periph::timer_mode Mode, uintptr_t ClockDivider, uintptr_t Prescale, uintptr_t Period>
        class timer {
        public:
            static_assert(Prescale == 1, "Unsupported prescale");

            static void apply() {
                T::disable();
            }
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::external, uintptr_t ClockDivider = 128>
        class adc {
        public:
            static void apply() {
                A::disable();
            }
        };
    };
}}}}

#endif
