#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP

#include "../../../ct/helpers.hpp"
#include "../../../ct/type_list.hpp"
#include "../../../mem/cas.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    using zoal::ct::conditional_type;
    using zoal::ct::type_at_index;
    using zoal::ct::type_list;
    using zoal::mem::callable_cas_list_variadic;

    template<uintptr_t Address>
    class timer_interrupt_flags_vector {
    private:
        enum { TOVx = 0, OCFAx = 1, OCFBx = 2, ICF = 5 };

    public:
        template<uint32_t No>
        using TIFRx = zoal::mem::reg<Address + No, zoal::mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        using clear_counter_flag = callable_cas_list_variadic<typename TIFRx<Timer>::template cas<1 << TOVx, 0>>;

        template<uint8_t Timer, uint8_t Channel>
        using clear_channel_flag = callable_cas_list_variadic<typename type_at_index<
            //
            Channel,
            zoal::mem::null_cas, // default
            type_list<
                //
                typename TIFRx<Timer>::template cas<1 << OCFAx, 0>,
                typename TIFRx<Timer>::template cas<1 << OCFBx, 0>>>::result>;


        template<uint8_t Timer>
        static void clear_counter_flag__() {
            TIFRx<Timer>::ref() = static_cast<uint8_t>(~(1 << TOVx));
        }

        template<uint8_t Timer, uint8_t Channel>
        static void clear_channel_flag__() {
            switch (Channel) {
            case 0:
                TIFRx<Timer>::ref() = static_cast<uint8_t>(~(1 << OCFAx));
                break;
            case 1:
                TIFRx<Timer>::ref() = static_cast<uint8_t>(~(1 << OCFBx));
                break;
            default:
                break;
            }
        }
    };
}}}

#endif
