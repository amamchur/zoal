#ifndef ZOAL_ARCH_STM32X_PORT_F0_F3_F4_HPP
#define ZOAL_ARCH_STM32X_PORT_F0_F3_F4_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/modifier.hpp"
#include "../../../mem/segment.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<::zoal::gpio::pin_mode PinMode>
    struct pin_mode_to_reg_mask {};

    template<>
    struct pin_mode_to_reg_mask<::zoal::gpio::pin_mode::input_floating> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x1;
        static constexpr auto GPIOx_PUPDR_mask = 0x0;
    };

    template<>
    struct pin_mode_to_reg_mask<::zoal::gpio::pin_mode::input_pull_up> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x1;
    };

    template<>
    struct pin_mode_to_reg_mask<::zoal::gpio::pin_mode::input_pull_down> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x2;
    };

    template<>
    struct pin_mode_to_reg_mask<::zoal::gpio::pin_mode::output_open_drain> {
        static constexpr auto GPIOx_MODER_mask = 0x1;
        static constexpr auto GPIOx_OTYPER_mask = 0x1;
        static constexpr auto GPIOx_PUPDR_mask = 0x0;
    };

    template<>
    struct pin_mode_to_reg_mask<::zoal::gpio::pin_mode::output_push_pull> {
        static constexpr auto GPIOx_MODER_mask = 0x1;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x2;
    };

    template<::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_to_cnf_mode {
        using rm = pin_mode_to_reg_mask<PinMode>;

        static constexpr auto speed_mask = 0x3; // 11: High speed

        template<uintptr_t V, uint8_t Shift = 0>
        using spd_cas = struct ::zoal::mem::clear_and_set<V != 0 ? 0x03 : 0, V != 0 ? speed_mask : 0, Shift>;
        using GPIOx_OSPEEDR = ::zoal::mem::merge_clear_and_set<spd_cas<(Mask & 1 << 0x0), 0>,
                                                               spd_cas<(Mask & 1 << 0x1), 2>,
                                                               spd_cas<(Mask & 1 << 0x2), 4>,
                                                               spd_cas<(Mask & 1 << 0x3), 6>,
                                                               spd_cas<(Mask & 1 << 0x4), 8>,
                                                               spd_cas<(Mask & 1 << 0x5), 10>,
                                                               spd_cas<(Mask & 1 << 0x6), 12>,
                                                               spd_cas<(Mask & 1 << 0x7), 14>,
                                                               spd_cas<(Mask & 1 << 0x8), 16>,
                                                               spd_cas<(Mask & 1 << 0x9), 18>,
                                                               spd_cas<(Mask & 1 << 0xA), 20>,
                                                               spd_cas<(Mask & 1 << 0xB), 22>,
                                                               spd_cas<(Mask & 1 << 0xC), 24>,
                                                               spd_cas<(Mask & 1 << 0xD), 26>,
                                                               spd_cas<(Mask & 1 << 0xE), 28>,
                                                               spd_cas<(Mask & 1 << 0xF), 30>>;

        template<uintptr_t V, uint8_t Shift = 0>
        using md_cas = struct ::zoal::mem::
            clear_and_set<(V != 0 ? 0x03 : 0), (V != 0 ? rm::GPIOx_MODER_mask : 0), Shift>;
        using GPIOx_MODER = ::zoal::mem::merge_clear_and_set<md_cas<(Mask & 1 << 0x0), 0>,
                                                             md_cas<(Mask & 1 << 0x1), 2>,
                                                             md_cas<(Mask & 1 << 0x2), 4>,
                                                             md_cas<(Mask & 1 << 0x3), 6>,
                                                             md_cas<(Mask & 1 << 0x4), 8>,
                                                             md_cas<(Mask & 1 << 0x5), 10>,
                                                             md_cas<(Mask & 1 << 0x6), 12>,
                                                             md_cas<(Mask & 1 << 0x7), 14>,
                                                             md_cas<(Mask & 1 << 0x8), 16>,
                                                             md_cas<(Mask & 1 << 0x9), 18>,
                                                             md_cas<(Mask & 1 << 0xA), 20>,
                                                             md_cas<(Mask & 1 << 0xB), 22>,
                                                             md_cas<(Mask & 1 << 0xC), 24>,
                                                             md_cas<(Mask & 1 << 0xD), 26>,
                                                             md_cas<(Mask & 1 << 0xE), 28>,
                                                             md_cas<(Mask & 1 << 0xF), 30>>;

        template<uintptr_t V, uint8_t Shift = 0>
        using tp_cas = struct ::zoal::mem::
            clear_and_set<(V != 0 ? 0x03 : 0), (V != 0 ? rm::GPIOx_OTYPER_mask : 0), Shift>;
        using GPIOx_OTYPER = ::zoal::mem::merge_clear_and_set<tp_cas<(Mask & 1 << 0x0), 0>,
                                                              tp_cas<(Mask & 1 << 0x1), 2>,
                                                              tp_cas<(Mask & 1 << 0x2), 4>,
                                                              tp_cas<(Mask & 1 << 0x3), 6>,
                                                              tp_cas<(Mask & 1 << 0x4), 8>,
                                                              tp_cas<(Mask & 1 << 0x5), 10>,
                                                              tp_cas<(Mask & 1 << 0x6), 12>,
                                                              tp_cas<(Mask & 1 << 0x7), 14>,
                                                              tp_cas<(Mask & 1 << 0x8), 16>,
                                                              tp_cas<(Mask & 1 << 0x9), 18>,
                                                              tp_cas<(Mask & 1 << 0xA), 20>,
                                                              tp_cas<(Mask & 1 << 0xB), 22>,
                                                              tp_cas<(Mask & 1 << 0xC), 24>,
                                                              tp_cas<(Mask & 1 << 0xD), 26>,
                                                              tp_cas<(Mask & 1 << 0xE), 28>,
                                                              tp_cas<(Mask & 1 << 0xF), 30>>;

        template<uintptr_t V, uint8_t Shift = 0>
        using pud_cas = struct ::zoal::mem::
            clear_and_set<(V != 0 ? 0x03 : 0), (V != 0 ? rm::GPIOx_PUPDR_mask : 0), Shift>;
        using GPIOx_PUPDR = ::zoal::mem::merge_clear_and_set<pud_cas<(Mask & 1 << 0x0), 0>,
                                                             pud_cas<(Mask & 1 << 0x1), 2>,
                                                             pud_cas<(Mask & 1 << 0x2), 4>,
                                                             pud_cas<(Mask & 1 << 0x3), 6>,
                                                             pud_cas<(Mask & 1 << 0x4), 8>,
                                                             pud_cas<(Mask & 1 << 0x5), 10>,
                                                             pud_cas<(Mask & 1 << 0x6), 12>,
                                                             pud_cas<(Mask & 1 << 0x7), 14>,
                                                             pud_cas<(Mask & 1 << 0x8), 16>,
                                                             pud_cas<(Mask & 1 << 0x9), 18>,
                                                             pud_cas<(Mask & 1 << 0xA), 20>,
                                                             pud_cas<(Mask & 1 << 0xB), 22>,
                                                             pud_cas<(Mask & 1 << 0xC), 24>,
                                                             pud_cas<(Mask & 1 << 0xD), 26>,
                                                             pud_cas<(Mask & 1 << 0xE), 28>,
                                                             pud_cas<(Mask & 1 << 0xF), 30>>;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask = 0xFFFF>
    class port : public Clock {
    public:
        using register_type = uint32_t;

        template<intptr_t Offset, register_type Clear, register_type Set, bool WriteOnly = false>
        using modifier = zoal::mem::modifier<Offset, register_type, Clear, Set, WriteOnly>;

        static constexpr uintptr_t address = Address;
        static constexpr uint32_t pin_mask = PinMask;

        static constexpr uintptr_t GPIOx_MODER = 0x00;
        static constexpr uintptr_t GPIOx_OTYPER = 0x04;
        static constexpr uintptr_t GPIOx_OSPEEDR = 0x08;
        static constexpr uintptr_t GPIOx_PUPDR = 0x0C;
        static constexpr uintptr_t GPIOx_IDR = 0x10;
        static constexpr uintptr_t GPIOx_ODR = 0x14;
        static constexpr uintptr_t GPIOx_BSRR = 0x18;
        static constexpr uintptr_t GPIOx_LCKR = 0x1C;
        static constexpr uintptr_t GPIOx_AFRL = 0x20;
        static constexpr uintptr_t GPIOx_AFRH = 0x24;
        static constexpr uintptr_t GPIOx_BRR = 0x28;

        static inline register_type read() {
            return mem[GPIOx_IDR];
        }

        template<register_type Mask>
        static inline void low_() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[GPIOx_BRR] = Mask;
        }

        template<register_type Mask>
        static inline void high_() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[GPIOx_BSRR] = Mask;
        }

        template<register_type Mask>
        static inline void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            auto data = mem[GPIOx_ODR];
            mem[GPIOx_BRR] = data & Mask;
            mem[GPIOx_BSRR] = ~data & Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        struct mode {
            using cfg = pin_mode_to_cnf_mode<PinMode, Mask>;
            using modifiers = zoal::ct::type_list<
                modifier<GPIOx_OSPEEDR, cfg::GPIOx_OSPEEDR::clear_mask, cfg::GPIOx_OSPEEDR::set_mask>,
                modifier<GPIOx_OTYPER, cfg::GPIOx_OTYPER::clear_mask, cfg::GPIOx_OTYPER::set_mask>,
                modifier<GPIOx_MODER, cfg::GPIOx_MODER::clear_mask, cfg::GPIOx_MODER::set_mask>,
                modifier<GPIOx_PUPDR, cfg::GPIOx_PUPDR::clear_mask, cfg::GPIOx_PUPDR::set_mask>,
                modifier<GPIOx_BSRR, 0, 0, true>,
                modifier<GPIOx_BRR, 0, 0, true>>;

            mode() {
                zoal::mem::apply_modifiers<address, modifiers>();
            }
        };

        template<register_type Mask>
        struct low {
            using modifiers = zoal::ct::type_list<modifier<GPIOx_OSPEEDR, 0, 0>,
                                                  modifier<GPIOx_OTYPER, 0, 0>,
                                                  modifier<GPIOx_MODER, 0, 0>,
                                                  modifier<GPIOx_PUPDR, 0, 0>,
                                                  modifier<GPIOx_BSRR, 0, 0, true>,
                                                  modifier<GPIOx_BRR, 0, Mask, true>>;

            low() {
                zoal::mem::apply_modifiers<address, modifiers>();
            }
        };

        template<register_type Mask>
        struct high {
            using modifiers = zoal::ct::type_list<modifier<GPIOx_OSPEEDR, 0, 0>,
                                                  modifier<GPIOx_OTYPER, 0, 0>,
                                                  modifier<GPIOx_MODER, 0, 0>,
                                                  modifier<GPIOx_PUPDR, 0, 0>,
                                                  modifier<GPIOx_BSRR, 0, Mask, true>,
                                                  modifier<GPIOx_BRR, 0, 0, true>>;

            high() {
                zoal::mem::apply_modifiers<address, modifiers>();
            }
        };

    private:
        static zoal::mem::segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask>
    zoal::mem::segment<uint32_t, Address> port<Address, Clock, PinMask>::mem;
}}}

#endif
