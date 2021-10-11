#ifndef ZOAL_ARCH_AVR_ATTINY_METADATA_HPP
#define ZOAL_ARCH_AVR_ATTINY_METADATA_HPP

#include "../../../ct/type_list.hpp"
#include "../metadata.hpp"

namespace zoal { namespace metadata {
    using zoal::ct::type_list;

    enum : uint8_t {
        //
        WGMx0 = 0,
        WGMx1 = 1,
        WGMx2 = 3,
        WGMx3 = 4,
        mode_clear_TCCRxA = 1u << WGMx0 | 1u << WGMx1,
        mode_clear_TCCRxB = 1u << WGMx2 | 1u << WGMx3
    };

    template<class T>
    struct timer_mode<T, ::zoal::periph::timer_mode::up> : type_list<
                                                               //
                                                               typename T::TCCRxA::template cas<mode_clear_TCCRxA, 1u << WGMx0 | 1u << WGMx1>,
                                                               typename T::TCCRxB::template cas<mode_clear_TCCRxB, 0x0>> {};

    template<class T>
    struct timer_mode<T, ::zoal::periph::timer_mode::up_down> : type_list<
                                                                    //
                                                                    typename T::TCCRxA::template cas<mode_clear_TCCRxA, 1u << WGMx0>,
                                                                    typename T::TCCRxB::template cas<mode_clear_TCCRxB, 0x0>> {};

    template<class T, uintptr_t Set>
    using clock_select_cas = zoal::ct::type_list<typename T::TCCRxB::template cas<0x7, Set>>;

    template<class T>
    struct timer_clock_divider<T, false, 0> : clock_select_cas<T, 0x0> {};

    template<class T>
    struct timer_clock_divider<T, false, 1> : clock_select_cas<T, 0x1> {};

    template<class T>
    struct timer_clock_divider<T, false, 8> : clock_select_cas<T, 0x2> {};

    template<class T>
    struct timer_clock_divider<T, false, 64> : clock_select_cas<T, 0x3> {};

    template<class T>
    struct timer_clock_divider<T, false, 256> : clock_select_cas<T, 0x4> {};

    template<class T>
    struct timer_clock_divider<T, false, 1024> : clock_select_cas<T, 0x5> {};

    template<class A, uintptr_t ClockDivider>
    struct adc_clock_divider {};

    template<class A>
    struct adc_clock_divider<A, 2> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 4> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 8> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 16> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 32> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 64> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};

    template<class A>
    struct adc_clock_divider<A, 128> : type_list<typename A::ADCSRAx::template cas<0x07, 0x04>> {};
}}

#endif
