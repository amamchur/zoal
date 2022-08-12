#ifndef ZOAL_MEM_CAS_HPP
#define ZOAL_MEM_CAS_HPP

#include "../ct/check.hpp"
#include "../ct/helpers.hpp"
#include "../ct/type_list.hpp"
#include "../utils/defs.hpp"

#include <stdint.h>

#ifdef ZOAL_COVERAGE
#include "../../../tests/test_utils/address_cast.hpp"
#else
#define ZOAL_ADDRESS_CAST(TYPE, ADDRESS) reinterpret_cast<volatile TYPE *>(ADDRESS)
#endif

namespace zoal { namespace mem {
    enum class reg_io { read, write, read_write };
    enum class cas_strategy_type { main, set, clear, assign, ignore };

    template<cas_strategy_type Type>
    class cas_strategy_implementation {};

    template<zoal::mem::reg_io RegIO, uint32_t Mask, uint32_t C, uint32_t S>
    struct cas_strategy {
        static_assert(RegIO != reg_io::write || C == 0, "Register is write only, clear not allowed");
        static constexpr auto ignore = C == 0 && S == 0;
        static constexpr auto assign = ((C | S) & Mask) == Mask || RegIO == zoal::mem::reg_io::write;
        static constexpr auto clear = S == 0;
        static constexpr auto set = C == 0 || C == S;
        static constexpr cas_strategy_type strategy = ignore ? cas_strategy_type::ignore
                                                             : (assign ? cas_strategy_type::assign
                                                                       : (clear ? cas_strategy_type::clear
                                                                                : (set ? cas_strategy_type::set : cas_strategy_type::main)));
        using impl = cas_strategy_implementation<strategy>;
    };

    template<uintptr_t Address, reg_io RegIO, class Type, Type Mask, uint32_t C, uint32_t S>
    struct cas : public cas_strategy<RegIO, Mask, C, S>::impl {
        using self_type = cas<Address, RegIO, Type, Mask, C, S>;
        using type = Type;

        static constexpr auto address = Address;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = S;

        ZOAL_INLINE_IO cas() noexcept {
            self_type::template apply<typename cas::type>(*ZOAL_ADDRESS_CAST(type, address), cas::clear, cas::set);
        };

        cas(const self_type &) = delete;
    };

    template<reg_io RegIO, class Type, Type Mask, uint32_t C, uint32_t S>
    struct cas<0, RegIO, Type, Mask, C, S> {
        using self_type = cas<0, RegIO, Type, Mask, C, S>;
        using type = Type;

        static constexpr auto address = 0;
        static constexpr auto io = RegIO;
        static constexpr auto mask = Mask;
        static constexpr Type clear = C;
        static constexpr Type set = S;

        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T, T) {}
    };

    using null_cas = cas<0, reg_io::read_write, uint8_t, 0xFF, 0, 0>;
    using null_cas_list = zoal::ct::type_list<null_cas>;

    template<>
    struct cas_strategy_implementation<cas_strategy_type::main> {
        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T c, T s) {
            ref = (ref & ~c) | s;
        }
    };

    template<>
    struct cas_strategy_implementation<cas_strategy_type::set> {
        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T c, T s) {
            ref |= s;
        }
    };

    template<>
    struct cas_strategy_implementation<cas_strategy_type::clear> {
        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T c, T s) {
            ref &= ~c;
        }
    };

    template<>
    struct cas_strategy_implementation<cas_strategy_type::assign> {
        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T c, T s) {
            ref = s;
        }
    };

    template<>
    struct cas_strategy_implementation<cas_strategy_type::ignore> {
        template<class R, class T>
        ZOAL_INLINE_IO static void apply(volatile R &ref, T, T) {}
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
    struct contains_cas {
        using current = typename List::type;
        static constexpr auto result = C::address == current::address || contains_cas<C, typename List::next>::result;
    };

    template<class C>
    struct contains_cas<C, void> {
        static constexpr auto result = false;
    };

    template<class List>
    struct contains_cas<void, List> {
        static constexpr auto result = false;
    };

    template<class List, class Filtered>
    struct filter_cas_list {
        using current_cas = typename List::type;
        static constexpr bool skip = zoal::mem::contains_cas<current_cas, Filtered>::result;
        using filtered = typename zoal::ct::conditional_type<
            //
            skip,
            Filtered,
            zoal::ct::type_list_item<current_cas, Filtered, Filtered::count + 1>>::type;

        using next = typename filter_cas_list<typename List::next, filtered>::result;
        using current = typename zoal::mem::make_cas_list<current_cas, next>::item;
        using result = typename zoal::ct::conditional_type<skip, next, current>::type;
    };

    template<class Filtered>
    struct filter_cas_list<void, Filtered> {
        using result = void;
    };

    template<class List>
    struct filter_cas_list<List, void> {
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
        using filtered = typename zoal::mem::filter_cas_list<List, zoal::mem::null_cas_list>::result;
        using list = typename perform_cas_aggregation<filtered, List>::result;
        using is_void = zoal::ct::is_same<void, list>;
        using result = typename zoal::ct::conditional_type<is_void::value, null_cas_list, list>::type;
    };

    template<class TypeList>
    class apply_cas_list {
    public:
        using cas = typename TypeList::type;

        ZOAL_INLINE_IO static void apply() {
            cas::template apply<typename cas::type>(*ZOAL_ADDRESS_CAST(typename cas::type, cas::address), cas::clear, cas::set);
            apply_cas_list<typename TypeList::next>::apply();
        }

        apply_cas_list() = delete;
    };

    template<>
    class apply_cas_list<void> {
    public:
        ZOAL_INLINE_IO static void apply() {}
    };

    template<class TypeList>
    struct callable_cas_list : TypeList {
        ZOAL_INLINE_IO callable_cas_list() {
            apply_cas_list<TypeList>::apply();
        }
    };

    template<>
    struct callable_cas_list<void> {
        ZOAL_INLINE_IO callable_cas_list() = default;
    };

    template<class... Rest>
    using callable_cas_list_variadic = callable_cas_list<zoal::ct::type_list<Rest...>>;
}}

#endif
