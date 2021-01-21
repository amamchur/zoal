
#ifndef ZOAL_ARCH_STM32F1_PORT_HPP
#define ZOAL_ARCH_STM32F1_PORT_HPP

#include "../../../gpio/api.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../gpio/pin_outout_speed.hpp"
#include "../../../mem/reg.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    using zoal::ct::type_list;
    using zoal::gpio::pin_mode;
    using zoal::gpio::pin_output_speed;

    template<class P, pin_mode PinMode, uint32_t Mask>
    struct pin_mode_to_gpio {};

    template<class P, uint32_t Mask>
    struct pin_mode_to_gpio<P, pin_mode::input_floating, Mask> : type_list<typename P::GPIOx_BSRR::template cas<0, 0>> {
        static constexpr uint32_t crx = 0x4 | 0x0;
    };

    template<class P, uint32_t Mask>
    struct pin_mode_to_gpio<P, pin_mode::input_pull_down, Mask> : type_list<typename P::GPIOx_BSRR::template cas<0, (Mask << 16)>> {
        static constexpr uint32_t crx = 0x8 | 0x0;
    };

    template<class P, uint32_t Mask>
    struct pin_mode_to_gpio<P, pin_mode::input_pull_up, Mask> : type_list<typename P::GPIOx_BSRR::template cas<0, Mask>> {
        static constexpr uint32_t crx = 0x8 | 0x0;
    };

    template<class P, uint32_t Mask>
    struct pin_mode_to_gpio<P, pin_mode::output_open_drain, Mask> : type_list<typename P::GPIOx_BSRR::template cas<0, 0>> {
        static constexpr uint32_t crx = 0x4 | 0x2;
    };

    template<class P, uint32_t Mask>
    struct pin_mode_to_gpio<P, pin_mode::output_push_pull, Mask> : type_list<typename P::GPIOx_BSRR::template cas<0, 0>> {
        static constexpr uint32_t crx = 0x0 | 0x2;
    };

    template<class P, pin_output_speed Speed, uint32_t Mask>
    struct pin_output_speed_to_gpio {};

    template<class P, uint32_t Mask>
    struct pin_output_speed_to_gpio<P, pin_output_speed::low, Mask> {
        static constexpr uint32_t crx = 0x2; // b10 Output mode, max speed 2 MHz
    };

    template<class P, uint32_t Mask>
    struct pin_output_speed_to_gpio<P, pin_output_speed::medium, Mask> {
        static constexpr uint32_t crx = 0x1; // b01 Output mode, max speed 10 MHz
    };

    template<class P, uint32_t Mask>
    struct pin_output_speed_to_gpio<P, pin_output_speed::high, Mask> {
        static constexpr uint32_t crx = 0x3; // b11 Output mode, max speed 50 MHz
    };

    template<class P, uint32_t CnfMode, uint32_t Mask>
    struct cnf_mode_to_cas {
        static constexpr uint32_t zero = 0x00;
        static constexpr uint32_t crx_c = 0x0F;
        static constexpr uint32_t crx_s = CnfMode;

        template<uintptr_t V, uint8_t Shift = 0>
        using crl_cas = typename P::GPIOx_CRL::template cas<(V != zero ? crx_c : zero) << Shift, (V != zero ? crx_s : zero) << Shift>;

        template<uintptr_t V, uint8_t Shift = 0>
        using crh_cas = typename P::GPIOx_CRH::template cas<(V != zero ? crx_c : zero) << Shift, (V != zero ? crx_s : zero) << Shift>;

        using p00 = crl_cas<(Mask & (1u << 0x0)), 0>;
        using p01 = crl_cas<(Mask & (1u << 0x1)), 4>;
        using p02 = crl_cas<(Mask & (1u << 0x2)), 8>;
        using p03 = crl_cas<(Mask & (1u << 0x3)), 12>;
        using p04 = crl_cas<(Mask & (1u << 0x4)), 16>;
        using p05 = crl_cas<(Mask & (1u << 0x5)), 20>;
        using p06 = crl_cas<(Mask & (1u << 0x6)), 24>;
        using p07 = crl_cas<(Mask & (1u << 0x7)), 28>;

        using p08 = crh_cas<(Mask & (1u << 0x8)), 0>;
        using p09 = crh_cas<(Mask & (1u << 0x9)), 4>;
        using p10 = crh_cas<(Mask & (1u << 0xA)), 8>;
        using p11 = crh_cas<(Mask & (1u << 0xB)), 12>;
        using p12 = crh_cas<(Mask & (1u << 0xC)), 16>;
        using p13 = crh_cas<(Mask & (1u << 0xD)), 20>;
        using p14 = crh_cas<(Mask & (1u << 0xE)), 24>;
        using p15 = crh_cas<(Mask & (1u << 0xF)), 28>;

        using list = type_list<p00, p01, p02, p03, p04, p05, p06, p07, p08, p09, p10, p11, p12, p13, p14, p15>;
        using all = typename zoal::gpio::api::optimize<list>;
    };

    template<class P, pin_mode PinMode, uint32_t Mask>
    struct pin_mode_cfg {
        using mode = pin_mode_to_gpio<P, PinMode, Mask>;
        using pcm = cnf_mode_to_cas<P, mode::crx, Mask>;
        using all = typename zoal::gpio::api::optimize<typename pcm::all, mode>;
    };

    template<class P, pin_output_speed Speed, uint32_t Mask>
    struct pin_output_speed_cfg {
        using speed = pin_output_speed_to_gpio<P, Speed, Mask>;
        using pcm = cnf_mode_to_cas<P, speed::crx, Mask>;
        using all = typename zoal::gpio::api::optimize<typename pcm::all>;
    };

    template<uintptr_t Address, class Clock, uint32_t PinMask = 0xFFFF>
    class port : public Clock {
    public:
        using self_type = port<Address, Clock, PinMask>;
        using register_type = uint32_t;

        static constexpr uintptr_t address = Address;
        static constexpr uint32_t pin_mask = PinMask;
        static constexpr zoal::gpio::features feature = static_cast<zoal::gpio::features>(zoal::gpio::features::output_speed |
                                                                                          zoal::gpio::features::output_open_drain);
        using GPIOx_CRL = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_CRH = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, register_type, 0xFFFFFFFF>;
        using GPIOx_IDR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, register_type, pin_mask>;
        using GPIOx_ODR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, register_type, pin_mask>;
        using GPIOx_BSRR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::write, register_type, (pin_mask << 16 | pin_mask)>;
        using GPIOx_BRR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::write, register_type, pin_mask>;
        using GPIOx_LCKR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, register_type, 0x1FFFF>;

        template<uint32_t CnfMode, uint32_t Mask>
        using port_cnf_mode = cnf_mode_to_cas<self_type, CnfMode, Mask>;

        template<pin_mode PinMode, uint32_t Mask>
        using port_pin_mode_cfg = pin_mode_cfg<self_type, PinMode, Mask>;

        template<pin_mode PinMode, register_type Mask>
        using mode_cas = typename port_pin_mode_cfg<PinMode, Mask>::all;

        template<pin_output_speed Speed, register_type Mask>
        using output_speed_cas = typename pin_output_speed_cfg<self_type, Speed, Mask>::all;

        template<register_type Mask>
        using low_cas = zoal::mem::callable_cas_list_variadic<typename GPIOx_BSRR::template cas<0, (Mask << 16)>>;

        template<register_type Mask>
        using high_cas = zoal::mem::callable_cas_list_variadic<typename GPIOx_BSRR::template cas<0, Mask>>;

        ZOAL_INLINE_IO static register_type read() {
            return GPIOx_IDR::ref();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void toggle() {
            static_assert((Mask & PinMask) == Mask && Mask != 0, "Incorrect pin mask");
            auto data = GPIOx_ODR::ref();
            GPIOx_BRR::ref() = data & Mask;
            GPIOx_BSRR::ref() = ~data & Mask;
        }
    };
}}}

#endif
