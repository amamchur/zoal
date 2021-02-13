#ifndef ZOAL_MEM__REG_HPP
#define ZOAL_MEM__REG_HPP

#include "cas.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace mem {
    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask>
    class reg final {
    public:
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        using type = Type;
        static constexpr auto mask = Mask;

        using self_type = reg<Address, RegIO, Type, Mask>;

        template<uint32_t C, uint32_t S>
        using cas = typename zoal::mem::cas<Address, RegIO, Type, Mask, C, S>;

        template<uint32_t V>
        using set = typename zoal::mem::cas<Address, RegIO, Type, Mask, mask, V>;

        ZOAL_INLINE_IO static volatile type &ref() {
            return *ZOAL_ADDRESS_CAST(Type, Address);
        }

        reg() = delete;
    };
}}

#endif
