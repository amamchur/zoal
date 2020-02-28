#ifndef ZOAL_MEM_CLEAR_AND_SET_HPP
#define ZOAL_MEM_CLEAR_AND_SET_HPP

#include "../utils/defs.hpp"
#include "accessor.hpp"

#include <stdint.h>

namespace zoal { namespace mem {
    template<uintptr_t Clear, uintptr_t Set, uint8_t Shift = 0>
    struct clear_and_set {
        static constexpr uint32_t clear_mask = static_cast<uint32_t>(static_cast<uint64_t>(Clear) << Shift);
        static constexpr uint32_t set_mask = static_cast<uint32_t>(static_cast<uint64_t>(Set) << Shift);

        template<class T>
        ZOAL_INLINE_MF static void apply(T &value) {
            value = (value & ~clear_mask) | set_mask;
        }

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {
            using ma = zoal::mem::accessor<T, Address, 0>;
            *ma::p = (*ma::p & ~clear_mask) | set_mask;
        }

        template<class T, uintptr_t Address, uintptr_t Offset>
        ZOAL_INLINE_MF static void apply(const zoal::mem::mock<T> &value) {
            apply(*value.get_ptr());
        }
    };

    template<uint8_t Shift>
    struct clear_and_set<0, 0, Shift> {
        static constexpr uint32_t clear_mask = 0;
        static constexpr uint32_t set_mask = 0;

        template<class T>
        ZOAL_INLINE_MF static void apply(T &) {}

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {}

        template<class T>
        ZOAL_INLINE_MF static void apply(const zoal::mem::mock<T> &value) {
            apply(*value.get_ptr());
        }
    };

    template<uintptr_t Clear, uint8_t Shift>
    struct clear_and_set<Clear, 0, Shift> {
        static constexpr uint32_t clear_mask = static_cast<uint32_t>(static_cast<uint64_t>(Clear) << Shift);
        static constexpr uint32_t set_mask = 0;

        template<class T>
        ZOAL_INLINE_MF static void apply(T &value) {
            value &= ~clear_mask;
        }

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {
            using ma = zoal::mem::accessor<T, Address, 0>;
            *ma::p &= ~clear_mask;
        }

        template<class T>
        ZOAL_INLINE_MF static void apply(const zoal::mem::mock<T> &value) {
            apply(*value.get_ptr());
        }
    };

    template<uintptr_t Set, uint8_t Shift>
    struct clear_and_set<Set, Set, Shift> {
        static constexpr uint32_t clear_mask = Set << Shift;
        static constexpr uint32_t set_mask = static_cast<uint32_t>(static_cast<uint64_t>(Set) << Shift);

        template<class T>
        ZOAL_INLINE_MF static void apply(T &value) {
            value |= set_mask;
        }

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {
            using ma = zoal::mem::accessor<T, Address, 0>;
            *ma::p |= set_mask;
        }

        template<class T>
        ZOAL_INLINE_MF static void apply(const zoal::mem::mock<T> &value) {
            apply(*value.get_ptr());
        }
    };

    template<uintptr_t Set, uint8_t Shift>
    struct clear_and_set<0, Set, Shift> {
        static constexpr uint32_t clear_mask = 0;
        static constexpr uint32_t set_mask = static_cast<uint32_t>(static_cast<uint64_t>(Set) << Shift);

        template<class T>
        ZOAL_INLINE_MF static void apply(T &value) {
            value |= set_mask;
        }

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {
            using ma = zoal::mem::accessor<T, Address, 0>;
            *ma::p |= set_mask;
        }

        template<class T>
        ZOAL_INLINE_MF static void apply(const zoal::mem::mock<T> &value) {
            apply(*value.get_ptr());
        }
    };

    template<class A, class B, class... Rest>
    struct merge_clear_and_set {
        using next = typename merge_clear_and_set<B, Rest...>::result;
        using result = clear_and_set<A::clear_mask | next::clear_mask, A::set_mask | next::set_mask>;
    };

    template<class A, class B>
    struct merge_clear_and_set<A, B> {
        using result = clear_and_set<A::clear_mask | B::clear_mask, A::set_mask | B::set_mask>;
    };
}}

#endif
