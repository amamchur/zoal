#ifndef ZOAL_MEM_MODIFIER_HPP
#define ZOAL_MEM_MODIFIER_HPP

#include "../ct/type_list.hpp"
#include "../ct/helpers.hpp"
#include "../mem/clear_and_set.hpp"
#include "../mem/segment.hpp"

#include <stddef.h>
#include <stdint.h>
#include <zoal/ct/check.hpp>

namespace zoal { namespace mem {
    template<intptr_t Offset, class T, T Clear, T Set, bool WriteOnly = false>
    struct modifier : clear_and_set<Clear, Set> {
        using mem_type = T;
        static constexpr intptr_t offset = Offset;
        static constexpr uint32_t clear_mask = WriteOnly ? 0 : Clear;
        static constexpr uint32_t set_mask = Set;
        static constexpr auto write_only = WriteOnly;
    };

    template<intptr_t Offset, class T, T Clear, T Set>
    struct modifier<Offset, T, Clear, Set, true> {
        using mem_type = T;
        static constexpr intptr_t offset = Offset;
        static constexpr uint32_t clear_mask = Clear;
        static constexpr uint32_t set_mask = Set;
        static constexpr auto write_only = true;

        template<class A>
        ZOAL_INLINE_MF static void apply(A &a) {
            if (set_mask != static_cast<T>(0)) {
                a = set_mask;
            }
        }
    };

    template<class ListA, class ListB>
    struct merge_modifiers_data {
        using a = typename ListA::type;
        using b = typename ListB::type;

        static_assert(a::offset == b::offset, "does not match");
        static_assert(a::write_only == b::write_only, "does not match");

        using self_type = merge_modifiers_data<ListA, ListB>;
        using type = modifier<a::offset,
                typename a::mem_type,
                (a::clear_mask | b::clear_mask),
                (a::set_mask & ~b::clear_mask) | b::set_mask,
                a::write_only>;
        using next = typename merge_modifiers_data<typename ListA::next, typename ListB::next>::self_type;
    };

    template<>
    struct merge_modifiers_data<void, void> {
        using self_type = void;
    };

    template<class ListA, class ListB, class... Lists>
    struct merge_modifiers {
        using tmp = merge_modifiers_data<ListA, ListB>;
        using result = typename merge_modifiers<tmp, Lists...>::result;
    };

    template<class ListA, class ListB>
    struct merge_modifiers<ListA, ListB> {
        using result = merge_modifiers_data<ListA, ListB>;
    };

    template<class M, class Next>
    struct filter_modifiers_item {
        using item = zoal::ct::type_list_item<M, Next, Next::count + 1>;
    };

    template<class M>
    struct filter_modifiers_item<M, void> {
        using item = zoal::ct::type_list_item<M, void, 1>;
    };

    template<class ModifiersTypeList>
    struct filter_modifiers {
        using modifier = typename ModifiersTypeList::type;
        static constexpr bool empty = modifier::clear_mask == 0 && modifier::set_mask == 0;

        using next = typename filter_modifiers<typename ModifiersTypeList::next>::result;
        using current = typename filter_modifiers_item<modifier, next>::item;
        using result = typename zoal::ct::conditional_type<empty, next, current>::type;
    };

    template<>
    struct filter_modifiers<void> {
        static constexpr size_t count = 0;
        using result = void;
    };

    template<uintptr_t Address, class ModifiersTypeList>
    struct apply_modifiers {
        using next = apply_modifiers<Address, typename ModifiersTypeList::next>;
        using current = typename ModifiersTypeList::type;

        ZOAL_INLINE_MF apply_modifiers() {
            zoal::mem::segment<typename current::mem_type, Address> mem;
            current::template apply(mem[current::offset]);
            next();
        }
    };

    template<uintptr_t Address>
    struct apply_modifiers<Address, void> {
    };

    template<uintptr_t Address, class ModifiersTypeList>
    struct clear_modifiers {
        using modifiers = typename filter_modifiers<ModifiersTypeList>::result;
        using next = apply_modifiers<Address, typename ModifiersTypeList::next>;
        using modifier = typename modifiers::type;

        ZOAL_INLINE_MF clear_modifiers() {
            zoal::mem::segment<typename modifier::mem_type, Address> mem;
            modifier::template clear(mem[modifier::offset]);
        }
    };

    template<uintptr_t Address>
    struct clear_modifiers<Address, void> {
    };
}}

#endif
