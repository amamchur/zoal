#ifndef ZOAL_MEM_MODIFIER_HPP
#define ZOAL_MEM_MODIFIER_HPP

#include "../ct/type_list.hpp"
#include "../mem/clear_and_set.hpp"
#include "../mem/segment.hpp"

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace mem {
    template<intptr_t Offset, class T, T Clear, T Set, bool WriteOnly = false>
    struct modifier {
        using mem_type = T;
        static constexpr intptr_t offset = Offset;
        static constexpr uint32_t clear_mask = Clear;
        static constexpr uint32_t set_mask = Set;
        static constexpr auto write_only = WriteOnly;

        template<class A>
        static void apply(A &a) {
            clear_and_set<clear_mask, set_mask>::apply(a);
        }
    };

    template<intptr_t Offset, class T, T Clear, T Set>
    struct modifier<Offset, T, Clear, Set, true> {
        using mem_type = T;
        static constexpr intptr_t offset = Offset;
        static constexpr uint32_t clear_mask = Clear;
        static constexpr uint32_t set_mask = Set;
        static constexpr auto write_only = true;

        template<class A>
        static void apply(A &a) {
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

    template<uintptr_t Address, class Modifier>
    struct apply_modifiers {
        using self_type = apply_modifiers<Address, Modifier>;
        using mem_type = typename Modifier::type::mem_type;
        using segment = zoal::mem::segment<mem_type, Address>;

        template<class T>
        void operator()() const {
            segment mem;
            T::template apply(mem[T::offset]);
        }

        apply_modifiers() {
            zoal::ct::type_list_iterator<Modifier>::for_each(*this);
        }
    };
}}

#endif
