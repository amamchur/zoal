#pragma once

#ifndef ZOAL_ARCH_STM32F1_NESTED_VECTORED_INTERRUPT_CONTROLLER_HPP
#define ZOAL_ARCH_STM32F1_NESTED_VECTORED_INTERRUPT_CONTROLLER_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {

    template<uintptr_t Address>
    class nested_vectored_interrupt_controller {
    public:
        using Class = nested_vectored_interrupt_controller<Address>;

        volatile uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        volatile uint32_t RESERVED0[24U];
        volatile uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        volatile uint32_t RSERVED1[24U];
        volatile uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        volatile uint32_t RESERVED2[24U];
        volatile uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        volatile uint32_t RESERVED3[24U];
        volatile uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        volatile uint32_t RESERVED4[56U];
        volatile uint8_t IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        volatile uint32_t RESERVED5[644U];
        volatile uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */

        static inline Class *instance() {
            return (Class *) Address;
        }
    };
}}}

#endif
