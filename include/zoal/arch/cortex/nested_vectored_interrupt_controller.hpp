#pragma once

#ifndef ZOAL_ARCH_STM32F1_NESTED_VECTORED_INTERRUPT_CONTROLLER_HPP
#define ZOAL_ARCH_STM32F1_NESTED_VECTORED_INTERRUPT_CONTROLLER_HPP

#include "../../mem/segment.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {

    template<uintptr_t Address = 0xE000E100>
    class nested_vectored_interrupt_controller {
    public:
        using Class = nested_vectored_interrupt_controller<Address>;
        static constexpr uintptr_t ISER_address = Address;
        static constexpr uintptr_t ICER_address = Address + 0x080;
        static constexpr uintptr_t ISPR_address = Address + 0x100;
        static constexpr uintptr_t ICPR_address = Address + 0x180;
        static constexpr uintptr_t IABR_address = Address + 0x200;
        static constexpr uintptr_t IPR_address = Address + 0x300;
        static constexpr uintptr_t STIR_address = Address + 0xE00;

        template<uintptr_t index>
        static inline void enable_interrupt() {
            zoal::mem::segment<uint32_t, ISER_address> mem;
            mem[index >> 5] = 1u << (index & 0x1Fu);
            mem.happyInspection();
        }

        template<uintptr_t index>
        static inline void disable_interrupt() {
            zoal::mem::segment<uint32_t, ICER_address> mem;
            mem[index >> 5] = 1u << (index & 0x1Fu);
            mem.happyInspection();
        }
    };
}}}

#endif
