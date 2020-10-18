#ifndef ZOAL_MEM__REG_HPP
#define ZOAL_MEM__REG_HPP

#include "cas.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace mem {
    template<class Register, uint32_t C, uint32_t S>
    class reg_cas {
    public:
        using cas = zoal::mem::cas<Register::address, Register::io, typename Register::type, Register::mask, C, S>;
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask>
    class reg {
    public:
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        using type = Type;
        static constexpr auto mask = Mask;

        using self_type = reg<Address, RegIO, Type, Mask>;
        template<uint32_t C, uint32_t S>
        using cas = typename zoal::mem::reg_cas<self_type, C, S>::cas;

        ZOAL_INLINE_IO static volatile type &ref() {
            return *ZOAL_ADDRESS_CAST(Type, Address);
        }
    };
}}

#endif
