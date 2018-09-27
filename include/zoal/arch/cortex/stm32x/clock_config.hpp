#ifndef ZOAL_ARCH_STM32X_CLOCK_CONFIG_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONFIG_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    enum class pll_clock_source { hsi, hse };

    enum class system_clock_source { hsi, hse, pll };

    enum class rtc_clock_source { hse, lse, lsi };

    template<class T, pll_clock_source Src>
    struct pll_clock_freq {
        static constexpr uint32_t value = 0;
    };

    template<class T>
    struct pll_clock_freq<T, pll_clock_source::hse> {
        static constexpr uint32_t value = T::hse;
    };

    template<class T>
    struct pll_clock_freq<T, pll_clock_source::hsi> {
        static constexpr uint32_t value = T::hsi;
    };

    template<class T, system_clock_source Src>
    struct sys_clock_clock_freq {
        static constexpr uint32_t value = 0;
    };

    template<class T>
    struct sys_clock_clock_freq<T, system_clock_source::hsi> {
        static constexpr uint32_t value = T::hsi;
    };

    template<class T>
    struct sys_clock_clock_freq<T, system_clock_source::hse> {
        static constexpr uint32_t value = T::hse;
    };

    template<class T>
    struct sys_clock_clock_freq<T, system_clock_source::pll> {
        static constexpr uint32_t value = T::pll_clock_freq * T::pll_mul;
    };

    template<uint32_t LSIv = 40000,
             uint32_t LSEv = 32768,
             uint32_t HSIv = 8000000,
             uint32_t HSEv = 8000000,
             pll_clock_source PLLs = pll_clock_source::hse,
             uint8_t PLLv = 9,
             uint8_t HSId = 2,
             uint8_t HSEd = 1,
             system_clock_source SCs = system_clock_source::pll>
    class clock_config {
    public:
        using self_type = clock_config;

        static constexpr uint32_t lsi = LSIv;
        static constexpr uint32_t lse = LSEv;
        static constexpr uint32_t hsi = HSIv;
        static constexpr uint32_t hse = HSEv;
        static constexpr uint32_t hsi_divider = HSId;
        static constexpr uint32_t hse_divider = HSEd;
        static constexpr uint32_t pll_mul = PLLv;

        static constexpr pll_clock_source pll_clock_src = PLLs;
        static constexpr system_clock_source sys_clock_src = SCs;
        static constexpr rtc_clock_source rtc_clock_src = rtc_clock_source::lse;

        static constexpr uint32_t ahb_prescaler = 1;
        static constexpr uint32_t apb1_prescaler = 1;
        static constexpr uint32_t apb2_prescaler = 2;
        static constexpr uint32_t pll_clock_freq = pll_clock_freq<self_type, pll_clock_src>::value;
        static constexpr uint32_t sys_clock_frequency = sys_clock_clock_freq<self_type, sys_clock_src>::value;
    };
}}}

#endif
