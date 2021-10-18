#ifndef ZOAL_ARCH_STM32X_PORT_F0_F3_F4_HPP
#define ZOAL_ARCH_STM32X_PORT_F0_F3_F4_HPP

#include "../../../gpio/api.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../mem/reg.hpp"
#include "../../../utils/defs.hpp"

namespace zoal { namespace arch { namespace stm32x {
    using zoal::ct::type_list;
    using zoal::gpio::pin_mode;

    template<::zoal::gpio::pin_mode PinMode>
    struct pin_mode_to_reg_mask {};

    template<>
    struct pin_mode_to_reg_mask<pin_mode::input_floating> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x1;
        static constexpr auto GPIOx_PUPDR_mask = 0x0;
    };

    template<>
    struct pin_mode_to_reg_mask<pin_mode::input_pull_up> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x1;
    };

    template<>
    struct pin_mode_to_reg_mask<pin_mode::input_pull_down> {
        static constexpr auto GPIOx_MODER_mask = 0x0;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x2;
    };

    template<>
    struct pin_mode_to_reg_mask<pin_mode::output_open_drain> {
        static constexpr auto GPIOx_MODER_mask = 0x1;
        static constexpr auto GPIOx_OTYPER_mask = 0x1;
        static constexpr auto GPIOx_PUPDR_mask = 0x0;
    };

    template<>
    struct pin_mode_to_reg_mask<pin_mode::output_push_pull> {
        static constexpr auto GPIOx_MODER_mask = 0x1;
        static constexpr auto GPIOx_OTYPER_mask = 0x0;
        static constexpr auto GPIOx_PUPDR_mask = 0x2;
    };

    template<class P, ::zoal::gpio::pin_mode PinMode, uint32_t Mask>
    struct pin_mode_to_cnf_mode {
        using rm = pin_mode_to_reg_mask<PinMode>;

        static constexpr auto speed_mask = 0x3; // 11: High speed

        template<uint32_t V, uint8_t Shift = 0>
        using spd_cas = typename P::GPIOx_OSPEEDR::template cas<
            //
            static_cast<uint32_t>((V != 0u ? speed_mask : 0u) << Shift),
            static_cast<uint32_t>((V != 0u ? speed_mask : 0u) << Shift)>;
        using GPIOx_OSPEEDR = type_list<
            //
            spd_cas<(Mask & 1 << 0x0), 0>,
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

        template<uint32_t V, uint8_t Shift = 0>
        using md_cas = typename P::GPIOx_MODER::template cas<
            //
            static_cast<uint32_t>((V != 0u ? 0x03u : 0u) << Shift),
            static_cast<uint32_t>((V != 0u ? rm::GPIOx_MODER_mask : 0u) << Shift)>;
        using GPIOx_MODER = type_list<
            //
            md_cas<(Mask & 1 << 0x0), 0>,
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

        template<uint32_t V, uint8_t Shift = 0>
        using tp_cas = typename P::GPIOx_OTYPER::template cas<
            //
            static_cast<uint32_t>((V != 0 ? 0x03 : 0) << Shift),
            static_cast<uint32_t>((V != 0 ? rm::GPIOx_OTYPER_mask : 0) << Shift)>;
        using GPIOx_OTYPER = type_list<
            //
            tp_cas<(Mask & 1 << 0x0), 0>,
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

        template<uint32_t V, uint8_t Shift = 0>
        using pud_cas = typename P::GPIOx_PUPDR::template cas<
            //
            static_cast<uint32_t>((V != 0 ? 0x03 : 0) << Shift),
            static_cast<uint32_t>((V != 0 ? rm::GPIOx_PUPDR_mask : 0) << Shift)>;
        using GPIOx_PUPDR = type_list<
            //
            pud_cas<(Mask & 1 << 0x0), 0>,
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

        using all = typename zoal::gpio::api::optimize<GPIOx_OSPEEDR, GPIOx_MODER, GPIOx_OTYPER, GPIOx_PUPDR>;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask = 0xFFFF>
    class port : public Clock {
    public:
        using self_type = port<Address, Clock, PinMask>;
        using register_type = uint32_t;

        static constexpr uintptr_t address = Address;
        static constexpr uint32_t pin_mask = PinMask;

        using GPIOx_MODER = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_OTYPER = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_OSPEEDR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_PUPDR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_IDR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read, register_type, 0xFFFFFFFF>;
        using GPIOx_ODR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_BSRR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::write, register_type, pin_mask>;
        using GPIOx_LCKR = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_AFRL = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_AFRH = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        using mode_cas = typename pin_mode_to_cnf_mode<self_type, PinMode, Mask>::all;

        template<register_type Mask>
        using low_cas = zoal::mem::callable_cas_list_variadic<typename GPIOx_BSRR::template cas<0, Mask << 16>>;

        template<register_type Mask>
        using high_cas = zoal::mem::callable_cas_list_variadic<typename GPIOx_BSRR::template cas<0, Mask>>;

        ZOAL_INLINE_IO static register_type read() {
            return GPIOx_IDR::ref();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            register_type data = GPIOx_ODR::ref();
            if ((data & Mask) == 0) {
                GPIOx_BSRR::ref() = Mask;
            } else {
                GPIOx_BSRR::ref() = Mask << 16;
            }
        }
    };
}}}

#endif
