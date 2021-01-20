#ifndef ZOAL_ARCH_STM32F1_METADATA_HPP
#define ZOAL_ARCH_STM32F1_METADATA_HPP

#include "../stm32x/metadata.hpp"

namespace zoal { namespace metadata {
    template<>
    struct stm32_afio_mapr<0x40013800, stm32_remap_mode::no_remap> {
        static constexpr uint32_t clear = 1 << 2;
        static constexpr uint32_t set = 0;
    };

    template<>
    struct stm32_afio_mapr<0x40013800, stm32_remap_mode::full> {
        static constexpr uint32_t clear = 1 << 2;
        static constexpr uint32_t set = 1 << 2;
    };

    template<stm32_remap_mode Mode>
    struct stm32_afio_mapr2<0x40013800, Mode> {
        static constexpr uint32_t clear = 0;
        static constexpr uint32_t set = 0;
    };
}}

#endif
