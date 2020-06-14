#ifndef ZOAL_MEM_CAS_HPP
#define ZOAL_MEM_CAS_HPP

#include "../ct/check.hpp"
#include "../ct/helpers.hpp"
#include "../ct/type_list.hpp"
#include "../utils/defs.hpp"

namespace zoal { namespace mem {
    enum class reg_io { read, write, read_write };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask, uint32_t C, uint32_t S>
    struct cas {
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = S;
        using type = Type;

        static_assert((clear & Mask) == clear, "Incorrect clear mask");
        static_assert((set & Mask) == set, "Incorrect set mask");

        inline cas() {
            auto p = reinterpret_cast<volatile Type *>(Address);
            *p = (*p & ~clear) | set;
        }
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask>
    struct cas<Address, RegIO, Type, Mask, 0, 0> {
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = 0;
        static constexpr Type set = 0;
        using type = Type;
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask, uint32_t CS>
    struct cas<Address, RegIO, Type, Mask, CS, CS> {
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = CS;
        static constexpr Type set = CS;
        using type = Type;

        static_assert((CS & Mask) == CS, "Incorrect mask");

        inline cas() {
            *reinterpret_cast<volatile Type *>(Address) |= CS;
        }
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask, uint32_t S>
    struct cas<Address, RegIO, Type, Mask, 0, S> {
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = 0;
        static constexpr Type set = S;
        using type = Type;

        static_assert((S & Mask) == S, "Incorrect set mask");

        ZOAL_INLINE_IO cas() {
            *reinterpret_cast<volatile Type *>(Address) |= set;
        }
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask, uint32_t C>
    struct cas<Address, RegIO, Type, Mask, C, 0> {
        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = 0;
        using type = Type;

        static_assert((C & Mask) == C, "Incorrect clear mask");

        ZOAL_INLINE_IO cas() {
            *reinterpret_cast<volatile Type *>(Address) &= ~clear;
        }
    };

    template<uintptr_t Address, class Type, Type Mask, uint32_t C, uint32_t S>
    struct cas<Address, reg_io::read, Type, Mask, C, S> {
        static constexpr auto address = Address;
        static constexpr auto io = reg_io::read;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = S;
        using type = Type;

        static_assert(clear == 0 && set == 0, "Memory is readonly");
    };

    template<uintptr_t Address, class Type, Type Mask>
    struct cas<Address, reg_io::write, Type, Mask, 0, 0> {
        static constexpr auto address = Address;
        static constexpr auto io = reg_io::write;
        static constexpr auto mask = Mask;
        static constexpr Type clear = 0;
        static constexpr Type set = 0;
        using type = Type;
    };

    template<uintptr_t Address, class Type, Type Mask, uint32_t S>
    struct cas<Address, reg_io::write, Type, Mask, 0, S> {
        static constexpr auto address = Address;
        static constexpr auto io = reg_io::write;
        static constexpr auto mask = Mask;
        static constexpr Type clear = 0;
        static constexpr Type set = S;
        using type = Type;

        inline cas() {
            *reinterpret_cast<volatile Type *>(Address) = set;
        }
    };

    template<uintptr_t Address, class Type, Type Mask, uint32_t C, uint32_t S>
    struct cas<Address, reg_io::write, Type, Mask, C, S> {
        static constexpr auto address = Address;
        static constexpr auto io = reg_io::write;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = S;
        using type = Type;

        static_assert(clear == 0, "Memory write only");

        inline cas() {
            *reinterpret_cast<volatile Type *>(Address) = set;
        }
    };

    using null_cas = cas<0, reg_io::read_write, uint8_t, 0xFF, 0, 0>;

    template<class A, class... Rest>
    struct merge_cas {
        using next = typename merge_cas<Rest...>::result;
        using result = cas<A::address, A::io, typename A::type, A::mask, A::clear | next::clear, A::set | next::set>;

        using same_type = zoal::ct::is_same<typename A::type, typename next::type>;
        static_assert(A::address == next::address, "Memory address does not match");
        static_assert(A::mask == next::mask, "Memory mask does not match");
        static_assert(A::io == next::io, "Memory IO does not match");
        static_assert(same_type::value, "Memory type does not match");
    };

    template<class A>
    struct merge_cas<A> {
        using result = A;
    };

    template<class A, class B>
    struct aggregate_cas {
        using cas = A;
        using current = typename B::type;
        using next = aggregate_cas<A, typename B::next>;

        static constexpr auto clear = A::clear | next::clear | (cas::address == current::address ? current::clear : 0);
        static constexpr auto set = A::set | next::set | (cas::address == current::address ? current::set : 0);
    };

    template<class A>
    struct aggregate_cas<A, void> {
        using cas = A;
        using next = void;
        static constexpr auto clear = A::clear;
        static constexpr auto set = A::set;
    };

    template<class M, class Tail>
    struct make_cas_list {
        using item = zoal::ct::type_list_item<M, Tail, Tail::count + 1>;
    };

    template<class M>
    struct make_cas_list<M, void> {
        using item = zoal::ct::type_list_item<M, void, 1>;
    };

    template<class C, class List>
    struct contains_ca {
        using current = typename List::type;
        static constexpr auto result = C::address == current::address || contains_ca<C, typename List::next>::result;
    };

    template<class C>
    struct contains_ca<C, void> {
        static constexpr auto result = false;
    };

    template<class List>
    struct contains_ca<void, List> {
        static constexpr auto result = false;
    };

    template<class List>
    struct filter_cas_address {
        using current_cas = typename List::type;
        static constexpr bool noop = current_cas::address != 0 && current_cas::clear == 0 && current_cas::set == 0;
        static constexpr bool skip = contains_ca<current_cas, typename List::next>::result;

        using next = typename filter_cas_address<typename List::next>::result;
        using current = typename make_cas_list<current_cas, next>::item;
        using result = typename zoal::ct::conditional_type<skip, next, current>::type;
    };

    template<>
    struct filter_cas_address<void> {
        using result = void;
    };

    template<class List>
    struct filter_cas_zeros {
        using current_cas = typename List::type;
        static constexpr bool skip = current_cas::address != 0 && current_cas::clear == 0 && current_cas::set == 0;

        using next = typename filter_cas_zeros<typename List::next>::result;
        using current = typename make_cas_list<current_cas, next>::item;
        using result = typename zoal::ct::conditional_type<skip, next, current>::type;
    };

    template<>
    struct filter_cas_zeros<void> {
        using result = void;
    };

    template<class Filtered, class All>
    struct perform_cas_aggregation {
        using current_item = Filtered;
        using cc = typename current_item::type;
        using agg = aggregate_cas<typename current_item::type, All>;
        using agg_cas = cas<cc::address, cc::io, typename cc::type, cc::mask, agg::clear, agg::set>;

        using next = typename perform_cas_aggregation<typename Filtered::next, All>::result;

        using result = typename make_cas_list<agg_cas, next>::item;
    };

    template<class All>
    struct perform_cas_aggregation<void, All> {
        using result = void;
    };

    template<class List>
    struct merge_cas_in_list {
        using address_list = typename zoal::mem::filter_cas_address<List>::result;
        using agg_list = typename perform_cas_aggregation<address_list, List>::result;
        using result = typename zoal::mem::filter_cas_zeros<agg_list>::result;
    };

    template<class List>
    ZOAL_INLINE_IO void apply_cas_list() {
        typename List::type();
        apply_cas_list<typename List::next>();
    }

    template<>
    ZOAL_INLINE_IO void apply_cas_list<void>() {}

    template<class List>
    struct callable_cas_list : List {
        ZOAL_INLINE_IO callable_cas_list() {
            apply_cas_list<List>();
        }

        static void apply() {
            zoal::mem::apply_cas_list<List>();
        }
    };
}}

#endif
