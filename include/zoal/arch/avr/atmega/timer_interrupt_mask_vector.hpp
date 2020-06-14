#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP

#include "../../../ct/helpers.hpp"
#include "../../../ct/type_list.hpp"
#include "../../../mem/cas.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    using zoal::ct::conditional_type;
    using zoal::ct::type_list;
    using zoal::mem::callable_cas_list;

    template<uintptr_t Address>
    class timer_interrupt_mask_vector {
    private:
        enum { TOIEx = 0, OCIExA = 1, OCIExB = 2 };

    public:
        template<uint32_t No>
        using TIMRx = zoal::mem::reg<Address + No, zoal::mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        using enable_overflow_interrupt = callable_cas_list<type_list<typename TIMRx<Timer>::template cas<0, 1 << TOIEx>>>;

        template<uint8_t Timer>
        using disable_overflow_interrupt = callable_cas_list<type_list<typename TIMRx<Timer>::template cas<1 << TOIEx, 0>>>;

        template<uint8_t Timer, uint8_t Channel>
        using enable_compare_match_interrupt = callable_cas_list<type_list<typename type_at_index<
            //
            Channel,
            zoal::mem::null_cas, // default
            type_list<
                //
                typename TIMRx<Timer>::template cas<0, 1 << OCIExA>,
                typename TIMRx<Timer>::template cas<0, 1 << OCIExB>>>::result>>;


        template<uint8_t Timer, uint8_t Channel>
        using disable_compare_match_interrupt = callable_cas_list<type_list<typename type_at_index<
            //
            Channel,
            zoal::mem::null_cas, // default
            type_list<
                //
                typename TIMRx<Timer>::template cas<1 << OCIExA, 0>,
                typename TIMRx<Timer>::template cas<1 << OCIExB, 0>>>::result>>;

//        template<uint8_t Timer>
//        static void enable_overflow_interrupt() {
//            TIMRx<Timer>::ref() |= (1 << TOIEx);
//        }
//
//        template<uint8_t Timer>
//        static void disable_overflow_interrupt() {
//            TIMRx<Timer>::ref() &= ~(1 << TOIEx);
//        }
//
//        template<uint8_t Timer, uint8_t Channel>
//        static void enable_compare_match_interrupt() {
//            switch (Channel) {
//            case 0:
//                TIMRx<Timer>::ref() |= (1 << OCIExA);
//                break;
//            case 1:
//                TIMRx<Timer>::ref() |= (1 << OCIExB);
//                break;
//            default:
//                break;
//            }
//        }
//
//        template<uint8_t Timer, uint8_t Channel>
//        static void disable_compare_match_interrupt() {
//            switch (Channel) {
//            case 0:
//                TIMRx<Timer>::ref() &= ~(1 << OCIExA);
//                break;
//            case 1:
//                TIMRx<Timer>::ref() &= ~(1 << OCIExB);
//                break;
//            default:
//                break;
//            }
//        }
    };
}}}

#endif
