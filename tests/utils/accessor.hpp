#ifndef ZOAL_MEM_ACCESSOR_HPP
#define ZOAL_MEM_ACCESSOR_HPP

#include "mcu_memory.hpp"

#include <stdint.h>
#include <zoal/ct/helpers.hpp>
#include <zoal/ct/type_list.hpp>

namespace zoal { namespace mem {
    enum class mcu_memory_segment { avr_plain, stm32f0_gpio, stm32f1_gpio };

    template<mcu_memory_segment Type, uintptr_t From, uintptr_t To>
    struct segment {
        static_assert(From < To, "Incorrect from & to values");
        static constexpr mcu_memory_segment type = Type;
        static constexpr uintptr_t from = From;
        static constexpr uintptr_t to = To;
        static constexpr ptrdiff_t mock_offset = -From;
        static uint8_t *memory;
    };

    using avr_plain = segment<mcu_memory_segment::avr_plain, 0x00, 0xFF>;
    using stm32f0_gpio = segment<mcu_memory_segment::stm32f0_gpio, 0x48000000, 0x480017FF>;
    using stm32f1_gpio = segment<mcu_memory_segment::stm32f1_gpio, 0x40010800, 0x400123FF>;
    using mem_segments = zoal::ct::type_list<avr_plain, stm32f1_gpio, stm32f0_gpio>;

    template<uintptr_t Address, class Item>
    struct find_segment {
        using current = typename Item::type;
        using next = typename find_segment<Address, typename Item::next>::segment;

        static constexpr bool matched = current::from <= Address && Address <= current::to;
        using segment = typename zoal::ct::conditional_type<matched, current, next>::type;
    };

    template<uintptr_t Address>
    struct find_segment<Address, void> {
        using segment = void;
    };

    template<uintptr_t Address>
    struct address_to_segment {
        using type = avr_plain;
    };

    template<mcu_memory_segment Type, uintptr_t From, uintptr_t To>
    uint8_t *segment<Type, From, To>::memory = nullptr;

    template<class T, uintptr_t Address, uintptr_t Offset>
    struct accessor {
        using seg = typename address_to_segment<Address>::type;
        using reg = tests::utils::register_accessor<T, Address, Offset>;

        static reg ptr() {
            return reg(seg::memory, seg::mock_offset, Address, Offset);
        }

        static reg ref() {
            return reg(seg::memory, seg::mock_offset, Address, Offset);
        }
    };
}}

#endif
