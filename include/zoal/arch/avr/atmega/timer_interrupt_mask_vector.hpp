#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP

#include "../../../ct/type_list.hpp"
#include "../../../mem/cas.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal::arch::avr {
    using ct::conditional_type;
    using ct::type_list;
    using mem::callable_cas_list_variadic;

    template<uintptr_t Address>
    class timer_interrupt_mask_vector {
    private:
        enum { TOIEx = 0, OCIExA = 1, OCIExB = 2 };

    public:
        template<uint32_t No>
        using TIMRx = mem::reg<Address + No, mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        using enable_overflow_interrupt = callable_cas_list_variadic<typename TIMRx<Timer>::template cas<0, 1 << TOIEx>>;

        template<uint8_t Timer>
        using disable_overflow_interrupt = callable_cas_list_variadic<typename TIMRx<Timer>::template cas<1 << TOIEx, 0>>;

        template<uint8_t Timer, uint8_t Channel>
        using enable_compare_match_interrupt = callable_cas_list_variadic<typename type_at_index<
            //
            Channel,
            mem::null_cas, // default
            type_list<
                //
                typename TIMRx<Timer>::template cas<0, 1 << OCIExA>,
                typename TIMRx<Timer>::template cas<0, 1 << OCIExB>>>::result>;


        template<uint8_t Timer, uint8_t Channel>
        using disable_compare_match_interrupt = callable_cas_list_variadic<typename type_at_index<
            //
            Channel,
            mem::null_cas, // default
            type_list<
                //
                typename TIMRx<Timer>::template cas<1 << OCIExA, 0>,
                typename TIMRx<Timer>::template cas<1 << OCIExB, 0>>>::result>;
    };
}

#endif
