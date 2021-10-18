
#ifndef ZOAL_METADATA_HPP
#define ZOAL_METADATA_HPP

#include "../../../ct/constant.hpp"
#include "../../../periph/usart.hpp"
#include "../../bus.hpp"

#include <cstdint>

namespace zoal { namespace metadata {
    enum class signal {
        //
        bkin,
        bkin2,
        ch1,
        ch1n,
        ch2,
        ch2n,
        ch3,
        ch3n,
        ch4,
        ck,
        cts,
        de,
        etr,
        in0,
        in1,
        in2,
        in3,
        in4,
        in5,
        in6,
        in7,
        in8,
        in9,
        in10,
        in11,
        in12,
        in13,
        in14,
        in15,
        in16,
        rts,
        rx,
        scl,
        sda,
        tx,
        exti2,
        exti11,
        exti15,
        smba,
        nss,
        sck,
        miso,
        mosi,
        mck,
        sd,
        ws,
        ckin
    };

    enum class stm32_remap_mode { no_remap, partial, full };

    template<uintptr_t PeriphAddr, stm32_remap_mode mode>
    struct stm32_afio_mapr {};

    template<uintptr_t PeriphAddr, stm32_remap_mode mode>
    struct stm32_afio_mapr2 {};

    template<class Sign, uintptr_t PeriphAddr, uintptr_t Port, uint8_t Pin, signal S>
    struct stm32_signal_map : zoal::ct::integral_constant<int, -1> {};

    template<int v1, int v2>
    struct stm32_af_to_remap_mode : zoal::ct::integral_constant<stm32_remap_mode, stm32_remap_mode::partial> {};

    template<>
    struct stm32_af_to_remap_mode<0, 0> : zoal::ct::integral_constant<stm32_remap_mode, stm32_remap_mode::no_remap> {};

    template<int v>
    struct stm32_af_to_remap_mode<v, v> : zoal::ct::integral_constant<stm32_remap_mode, stm32_remap_mode::full> {};

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
