#pragma once

#ifndef ZOAL_ARCH_STM32F1_PORT_HPP
#define ZOAL_ARCH_STM32F1_PORT_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/segment.hpp"
#include "../../../mem/write_only_mask.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_to_cnf_mode {};

    template<uint32_t Mask>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_floating, Mask> {
        static constexpr uint32_t value = 0x4 | 0x0;
        using GPIOx_BRR = ::zoal::mem::write_only_mask<0, 0>;
        using GPIOx_BSRR = ::zoal::mem::write_only_mask<0, 0>;
    };

    template<uint32_t Mask>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_pull_down, Mask> {
        static constexpr uint32_t value = 0x8 | 0x0;
        using GPIOx_BRR = ::zoal::mem::write_only_mask<Mask, 0>;
        using GPIOx_BSRR = ::zoal::mem::write_only_mask<0, 0>;
    };

    template<uint32_t Mask>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_pull_up, Mask> {
        static constexpr uint32_t value = 0x8 | 0x0;
        using GPIOx_BRR = ::zoal::mem::write_only_mask<0, 0>;
        using GPIOx_BSRR = ::zoal::mem::write_only_mask<Mask, 0>;
    };

    template<uint32_t Mask>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::output_open_drain, Mask> {
        static constexpr uint32_t value = 0x4 | 0x3;
        using GPIOx_BRR = ::zoal::mem::clear_and_set<0, 0>;
        using GPIOx_BSRR = ::zoal::mem::clear_and_set<0, 0>;
    };

    template<uint32_t Mask>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::output_push_pull, Mask> {
        static constexpr uint32_t value = 0x0 | 0x3;
        using GPIOx_BRR = ::zoal::mem::write_only_mask<0, 0>;
        using GPIOx_BSRR = ::zoal::mem::write_only_mask<0, 0>;
    };

    template<::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_cfg {
        static constexpr auto cnf_clear = 0x03;

        using mode = pin_mode_to_cnf_mode<PinMode, Mask>;
        using GPIOx_BRR = typename mode::GPIOx_BRR;
        using GPIOx_BSRR = typename mode::GPIOx_BSRR;

        template<uintptr_t V, uint8_t Shift = 0>
        using cr_cas = struct ::zoal::mem::clear_and_set<(V != 0 ? cnf_clear : 0), (V != 0 ? mode::value : 0), Shift>;

        using crl_p00 = cr_cas<(Mask & 1 << 0x0), 0>;
        using crl_p01 = cr_cas<(Mask & 1 << 0x1), 4>;
        using crl_p02 = cr_cas<(Mask & 1 << 0x2), 8>;
        using crl_p03 = cr_cas<(Mask & 1 << 0x3), 12>;
        using crl_p04 = cr_cas<(Mask & 1 << 0x4), 16>;
        using crl_p05 = cr_cas<(Mask & 1 << 0x5), 20>;
        using crl_p06 = cr_cas<(Mask & 1 << 0x6), 24>;
        using crl_p07 = cr_cas<(Mask & 1 << 0x7), 28>;

        using crh_p08 = cr_cas<(Mask & 1 << 0x8), 0>;
        using crh_p09 = cr_cas<(Mask & 1 << 0x9), 4>;
        using crh_p10 = cr_cas<(Mask & 1 << 0xA), 8>;
        using crh_p11 = cr_cas<(Mask & 1 << 0xB), 12>;
        using crh_p12 = cr_cas<(Mask & 1 << 0xC), 16>;
        using crh_p13 = cr_cas<(Mask & 1 << 0xD), 20>;
        using crh_p14 = cr_cas<(Mask & 1 << 0xE), 24>;
        using crh_p15 = cr_cas<(Mask & 1 << 0xF), 28>;

        using GPIOx_CRL =
            ::zoal::mem::merge_clear_and_set<crl_p00, crl_p01, crl_p02, crl_p03, crl_p04, crl_p05, crl_p06, crl_p07>;
        using GPIOx_CRH =
            ::zoal::mem::merge_clear_and_set<crh_p08, crh_p09, crh_p10, crh_p11, crh_p12, crh_p13, crh_p14, crh_p15>;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask = 0xFFFF>
    class port : public Clock {
    public:
        using register_type = uint32_t;
        static constexpr uintptr_t address = Address;
        static constexpr uint32_t pin_mask = PinMask;

        static constexpr uintptr_t GPIOx_CRL = 0x00;
        static constexpr uintptr_t GPIOx_CRH = 0x04;
        static constexpr uintptr_t GPIOx_IDR = 0x08;
        static constexpr uintptr_t GPIOx_ODR = 0x0C;
        static constexpr uintptr_t GPIOx_BSRR = 0x10;
        static constexpr uintptr_t GPIOx_BRR = 0x14;
        static constexpr uintptr_t GPIOx_LCKR = 0x18;

        static inline register_type read() {
            return mem[GPIOx_IDR];
        }

        template<register_type Mask>
        static void low() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[GPIOx_BRR] = Mask;
        }

        template<register_type Mask>
        static void high() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[GPIOx_BSRR] = Mask;
        }

        template<register_type Mask>
        static void toggle() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            auto data = mem[GPIOx_ODR];
            mem[GPIOx_BRR] = data & Mask;
            mem[GPIOx_BSRR] = ~data & Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask, bool TimeStable = false>
        static inline void mode() {
            using namespace zoal::gpio;
            using cfg = pin_mode_cfg<PinMode, Mask>;
            if (TimeStable) {
                // Do not use clear_and_set & write_only template optimization
                mem[GPIOx_CRL] = (mem[GPIOx_CRL] & ~cfg::GPIOx_CRL::clear_mask) | cfg::GPIOx_CRL::set_mask;
                mem[GPIOx_CRH] = (mem[GPIOx_CRH] & ~cfg::GPIOx_CRH::clear_mask) | cfg::GPIOx_CRH::set_mask;
                mem[GPIOx_BRR] = cfg::GPIOx_BRR::mask;
                mem[GPIOx_BSRR] = cfg::GPIOx_BSRR::mask;
            } else {
                cfg::GPIOx_CRL::apply(mem[GPIOx_CRL]);
                cfg::GPIOx_CRH::apply(mem[GPIOx_CRH]);
                cfg::GPIOx_BRR::apply(mem[GPIOx_BRR]);
                cfg::GPIOx_BSRR::apply(mem[GPIOx_BSRR]);
            }
        }

    private:
        static zoal::mem::segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask>
    zoal::mem::segment<uint32_t, Address> port<Address, Clock, PinMask>::mem;
}}}

#endif
