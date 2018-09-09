#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP

#include "mcu_type.hpp"
#include "timer.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr {
    template<class Model, uintptr_t Address, uint8_t N, bool Async = false>
    class timer8 : public timer<Model, Address, N> {
    public:
        static constexpr bool async = Async;

        timer8() = delete;

        static typename Model::word top_value() {
            return 0xFF;
        }
    };
}}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class timer8_model {
    public:
        using word = uint8_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCCRxB = 1;
        static constexpr uintptr_t TCNTx = 2;
        static constexpr uintptr_t OCRxA = 3;
        static constexpr uintptr_t OCRxB = 4;
    };

    template<uintptr_t Address, uint8_t N, bool Async = false>
    using timer8 = ::zoal::arch::avr::timer8<timer8_model, Address, N, Async>;

}}}}

#endif
