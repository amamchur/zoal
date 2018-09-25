
#ifndef ZOAL_METADATA_HPP
#define ZOAL_METADATA_HPP

#include "../../../ct/constant.hpp"
#include "../../../periph/usart.hpp"
#include "../../bus.hpp"

#include <stdint.h>

namespace zoal { namespace metadata {
    enum class signal { rx, cts, tx, ck, de, rts, ch1, ch2, ch3, ch4, etr };

    template<uintptr_t A, uintptr_t Port, uint8_t Pin, signal S>
    struct stm32_af : zoal::ct::integral_constant<int, -1> {};

    template<uintptr_t A, uintptr_t Port, uint8_t Pin, signal S>
    struct stm32_remap : zoal::ct::integral_constant<int, -1> {};

    template<zoal::arch::bus Bus>
    struct stm32_bus_prescaler {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_ahb> : zoal::ct::integral_constant<uint8_t, 1> {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_apb1> : zoal::ct::integral_constant<uint8_t, 2> {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_apb2> : zoal::ct::integral_constant<uint8_t, 1> {};

    template<uint8_t Bits>
    struct stm32_data_bits_to_cr1 {};

    template<>
    struct stm32_data_bits_to_cr1<7> {
        static const uint32_t set_mask = 0x10000000;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct stm32_data_bits_to_cr1<8> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct stm32_data_bits_to_cr1<9> {
        static const uint32_t set_mask = 0x1000;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<zoal::periph::usart_parity Parity>
    struct stm32_parity_to_cr1 {};

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::none> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::even> {
        static const uint32_t set_mask = 0x400;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::odd> {
        static const uint32_t set_mask = 0x600;
        static const uint32_t clear_mask = 0x600;
    };

    template<zoal::periph::usart_stop_bits StopBit>
    struct stm32_stop_bits_to_cr2 {};

    template<>
    struct stm32_stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_1> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x3000;
    };

    template<>
    struct stm32_stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_2> {
        static const uint32_t set_mask = 0x2000;
        static const uint32_t clear_mask = 0x3000;
    };
}}

#endif
