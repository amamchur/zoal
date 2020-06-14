#ifndef ZOAL_ARCH_STM32F1_MUX_HPP
#define ZOAL_ARCH_STM32F1_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../utils/helpers.hpp"
#include "../stm32x/metadata.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
    using zoal::ct::type_list;
    using zoal::metadata::signal;

    template<class P, uint32_t mode, uint32_t Mask>
    struct crh_crl {
        static constexpr auto cnf_clear = 0x03;

        template<uintptr_t V, uint8_t Shift = 0>
        using crl_cas = typename P::GPIOx_CRL::template cas<(V != 0 ? cnf_clear : 0) << Shift, 0>;

        template<uintptr_t V, uint8_t Shift = 0>
        using crh_cas = typename P::GPIOx_CRH::template cas<(V != 0 ? cnf_clear : 0) << Shift, 0>;

        using p00 = crl_cas<(Mask & 1 << 0x0), 0>;
        using p01 = crl_cas<(Mask & 1 << 0x1), 4>;
        using p02 = crl_cas<(Mask & 1 << 0x2), 8>;
        using p03 = crl_cas<(Mask & 1 << 0x3), 12>;
        using p04 = crl_cas<(Mask & 1 << 0x4), 16>;
        using p05 = crl_cas<(Mask & 1 << 0x5), 20>;
        using p06 = crl_cas<(Mask & 1 << 0x6), 24>;
        using p07 = crl_cas<(Mask & 1 << 0x7), 28>;

        using p08 = crh_cas<(Mask & 1 << 0x8), 0>;
        using p09 = crh_cas<(Mask & 1 << 0x9), 4>;
        using p10 = crh_cas<(Mask & 1 << 0xA), 8>;
        using p11 = crh_cas<(Mask & 1 << 0xB), 12>;
        using p12 = crh_cas<(Mask & 1 << 0xC), 16>;
        using p13 = crh_cas<(Mask & 1 << 0xD), 20>;
        using p14 = crh_cas<(Mask & 1 << 0xE), 24>;
        using p15 = crh_cas<(Mask & 1 << 0xF), 28>;

        using all = type_list<p00, p01, p02, p03, p04, p05, p06, p07, p08, p09, p10, p11, p12, p13, p14, p15>;
    };

    template<class Microcontroller>
    class mux {
    private:
        using afio = typename Microcontroller::afio;

//        template<uintptr_t A, uint8_t Rm, class Dummy = void>
//        struct usart_remapper {
//            static constexpr int register_offset = -1;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40013800, 0, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<1, 0, 2>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40013800, 1, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<1, 1, 2>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40004400, 0, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<1, 0, 3>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40004400, 1, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<1, 1, 3>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40004800, 0, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<3, 0, 4>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40004800, 1, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<3, 1, 4>;
//        };
//
//        template<class Dummy>
//        struct usart_remapper<0x40004800, 2, Dummy> {
//            static constexpr int register_offset = afio::AFIOx_MAPR;
//            using mapr = ::zoal::mem::clear_and_set<3, 2, 4>;
//        };

    public:
        template<class U, class PinRX, class PinTX>
        class usart {
        public:
            using rx_rm = zoal::metadata::stm32_remap<U::address, PinRX::port::address, PinRX::offset, signal::rx>;
            using tx_rm = zoal::metadata::stm32_remap<U::address, PinTX::port::address, PinTX::offset, signal::tx>;

            static_assert(rx_rm::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_rm::value >= 0, "Unsupported TX pin mapping");
            static_assert(rx_rm::value == tx_rm::value, "Different remapping");

//            using remap = usart_remapper<U::address, rx_rm::value>;
//            static_assert(remap::register_offset >= 0, "Unsupported pin mapping");

            static inline void on() {
                PinRX::template mode<zoal::gpio::pin_mode::input_floating>();
                stm32_alternate_function<PinTX>();
            }

        private:
            template<class P>
            static inline void stm32_alternate_function() {
                using cfg = crh_crl<typename P::port, 10, 1 << P::offset>;
                zoal::mem::apply_cas_list<typename cfg::all>();
            }
        };
    };
}}}

#endif
