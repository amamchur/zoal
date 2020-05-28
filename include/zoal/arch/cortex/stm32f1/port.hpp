
#ifndef ZOAL_ARCH_STM32F1_PORT_HPP
#define ZOAL_ARCH_STM32F1_PORT_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../mem/bits_write_only.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/modifier.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_to_gpio {};

    template<uint32_t Mask>
    struct pin_mode_to_gpio<::zoal::gpio::pin_mode::input_floating, Mask> {
        static constexpr uint32_t crx = 0x4 | 0x0;
        static constexpr uint32_t bsrr = 0;
        static constexpr uint32_t brr = 0;
    };

    template<uint32_t Mask>
    struct pin_mode_to_gpio<::zoal::gpio::pin_mode::input_pull_down, Mask> {
        static constexpr uint32_t crx = 0x8 | 0x0;
        static constexpr uint32_t bsrr = Mask;
        static constexpr uint32_t brr = 0;
    };

    template<uint32_t Mask>
    struct pin_mode_to_gpio<::zoal::gpio::pin_mode::input_pull_up, Mask> {
        static constexpr uint32_t crx = 0x8 | 0x0;
        static constexpr uint32_t bsrr = Mask;
        static constexpr uint32_t brr = 0;
    };

    template<uint32_t Mask>
    struct pin_mode_to_gpio<::zoal::gpio::pin_mode::output_open_drain, Mask> {
        static constexpr uint32_t crx = 0x4 | 0x3;
        static constexpr uint32_t bsrr = 0;
        static constexpr uint32_t brr = 0;
    };

    template<uint32_t Mask>
    struct pin_mode_to_gpio<::zoal::gpio::pin_mode::output_push_pull, Mask> {
        static constexpr uint32_t crx = 0x0 | 0x3;
        static constexpr uint32_t bsrr = 0;
        static constexpr uint32_t brr = 0;
    };

    template<::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_cfg {
        static constexpr auto crx_clear = 0x0F;

        using mode = pin_mode_to_gpio<PinMode, Mask>;

        template<uintptr_t V, uint8_t Shift = 0>
        using cr_cas = struct ::zoal::mem::clear_and_set<(V != 0 ? crx_clear : 0), (V != 0 ? mode::crx : 0), Shift>;

        using p00 = cr_cas<(Mask & (1 << 0x0)), 0>;
        using p01 = cr_cas<(Mask & (1 << 0x1)), 4>;
        using p02 = cr_cas<(Mask & (1 << 0x2)), 8>;
        using p03 = cr_cas<(Mask & (1 << 0x3)), 12>;
        using p04 = cr_cas<(Mask & (1 << 0x4)), 16>;
        using p05 = cr_cas<(Mask & (1 << 0x5)), 20>;
        using p06 = cr_cas<(Mask & (1 << 0x6)), 24>;
        using p07 = cr_cas<(Mask & (1 << 0x7)), 28>;

        using p08 = cr_cas<(Mask & (1 << 0x8)), 0>;
        using p09 = cr_cas<(Mask & (1 << 0x9)), 4>;
        using p10 = cr_cas<(Mask & (1 << 0xA)), 8>;
        using p11 = cr_cas<(Mask & (1 << 0xB)), 12>;
        using p12 = cr_cas<(Mask & (1 << 0xC)), 16>;
        using p13 = cr_cas<(Mask & (1 << 0xD)), 20>;
        using p14 = cr_cas<(Mask & (1 << 0xE)), 24>;
        using p15 = cr_cas<(Mask & (1 << 0xF)), 28>;

        using GPIOx_CRL = typename ::zoal::mem::merge_clear_and_set<p00, p01, p02, p03, p04, p05, p06, p07>::result;
        using GPIOx_CRH = typename ::zoal::mem::merge_clear_and_set<p08, p09, p10, p11, p12, p13, p14, p15>::result;
        using GPIOx_BSRR = typename ::zoal::mem::bits_write_only<mode::bsrr>;
        using GPIOx_BRR = typename ::zoal::mem::bits_write_only<mode::brr>;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask = 0xFFFF>
    class port : public Clock {
    public:
        using register_type = uint32_t;
        static constexpr uintptr_t address = Address;
        static constexpr uint32_t pin_mask = PinMask;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<register_type, Address, Offset>;

        static constexpr uintptr_t GPIOx_CRL = 0x00;
        static constexpr uintptr_t GPIOx_CRH = 0x04;
        static constexpr uintptr_t GPIOx_IDR = 0x08;
        static constexpr uintptr_t GPIOx_ODR = 0x0C;
        static constexpr uintptr_t GPIOx_BSRR = 0x10;
        static constexpr uintptr_t GPIOx_BRR = 0x14;
        static constexpr uintptr_t GPIOx_LCKR = 0x18;

        template<intptr_t Offset, register_type Clear, register_type Set, bool WriteOnly = false>
        using modifier = zoal::mem::modifier<Offset, register_type, Clear, Set, WriteOnly>;

        ZOAL_INLINE_IO static register_type read() {
            return accessor<GPIOx_IDR>::ref();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void low() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            accessor<GPIOx_BRR>::ref() = Mask;
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void high() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            accessor<GPIOx_BSRR>::ref() = Mask;
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void toggle() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            auto data = accessor<GPIOx_ODR>::ref();
            accessor<GPIOx_BRR>::ref() = data & Mask;
            accessor<GPIOx_BSRR>::ref() = ~data & Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        ZOAL_INLINE_IO static void mode() {
            using md = pin_mode_cfg<PinMode, Mask>;
            md::GPIOx_BSRR::template apply<address + GPIOx_BSRR, register_type>();
            md::GPIOx_BRR::template apply<address + GPIOx_BRR, register_type>();
            md::GPIOx_CRL::template apply<address + GPIOx_CRL, register_type>();
            md::GPIOx_CRH::template apply<address + GPIOx_CRH, register_type>();
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        struct mode_modifiers {
            using cfg = pin_mode_cfg<PinMode, Mask>;
            using modifiers = zoal::ct::type_list<modifier<GPIOx_CRL, cfg::GPIOx_CRL::clear_mask, cfg::GPIOx_CRL::set_mask>,
                                                  modifier<GPIOx_CRH, cfg::GPIOx_CRH::clear_mask, cfg::GPIOx_CRH::set_mask>,
                                                  modifier<GPIOx_BRR, 0, cfg::brr, true>,
                                                  modifier<GPIOx_BSRR, 0, cfg::bsrr, true>>;

            mode_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };

        template<register_type Mask>
        struct low_modifiers {
            static_assert((Mask & pin_mask) == Mask, "Incorrect pin mask");

            using modifiers = zoal::ct::
                type_list<modifier<GPIOx_CRL, 0, 0>, modifier<GPIOx_CRH, 0, 0>, modifier<GPIOx_BRR, 0, Mask, true>, modifier<GPIOx_BSRR, 0, 0, true>>;

            low_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };

        template<register_type Mask>
        struct high_modifiers {
            static_assert((Mask & pin_mask) == Mask, "Incorrect pin mask");
            using modifiers = zoal::ct::
                type_list<modifier<GPIOx_CRL, 0, 0>, modifier<GPIOx_CRH, 0, 0>, modifier<GPIOx_BRR, 0, 0, true>, modifier<GPIOx_BSRR, 0, Mask, true>>;

            high_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };
    };
}}}

#endif
