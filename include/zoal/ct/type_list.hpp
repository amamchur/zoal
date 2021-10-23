#ifndef ZOAL_CT_TYPE_LIST_HPP
#define ZOAL_CT_TYPE_LIST_HPP

#include "../utils/defs.hpp"
#include "conditional_type.hpp"

#include <stddef.h>

namespace zoal { namespace ct {
    template<class T, class Next, size_t Count>
    struct type_list_item {
        static constexpr size_t count = Count;
        using type = T;
        using next = Next;
    };

    template<class First, class... Rest>
    struct type_list : type_list_item<First, type_list<Rest...>, sizeof...(Rest) + 1> {};

    template<class First>
    struct type_list<First> : type_list_item<First, void, 1> {};

    template<class Current, class Next, class Tail>
    struct perform_type_list_join {
        using next_helper = perform_type_list_join<Next, typename Next::next, Tail>;
        using next_item = typename next_helper::result;
        using result = type_list_item<typename Current::type, next_item, next_item::count + 1>;
    };

    template<class Current, class B>
    struct perform_type_list_join<Current, void, B> {
        using result = type_list_item<typename Current::type, B, B::count + 1>;
    };

    template<class A, class... Rest>
    struct type_list_join {
        using list_tail = typename type_list_join<Rest...>::result;
        using helper = perform_type_list_join<A, typename A::next, list_tail>;
        using result = typename helper::result;
    };

    template<class A>
    struct type_list_join<A> {
        using result = A;
    };

    template<class List>
    struct type_list_iterator {
        template<class F>
        ZOAL_INLINE_MF static void for_each(F &fn) {
            fn.template operator()<typename List::type>();
            type_list_iterator<typename List::next>::for_each(fn);
        }
    };

    template<>
    struct type_list_iterator<void> {
        template<class F>
        static void for_each(F fn) {}
    };

    template<class List, size_t Index = 0>
    struct type_list_index_iterator {
        template<class F>
        static void for_each(const F &fn) {
            fn.template operator()<typename List::type>(Index);
            type_list_index_iterator<typename List::next, Index + 1>::for_each(fn);
        }
    };

    template<class List>
    struct type_chain_iterator {
        template<class F>
        ZOAL_INLINE_MF static void for_each(F &fn) {
            fn.template operator()<List>();
            type_chain_iterator<typename List::next>::for_each(fn);
        }
    };

    template<>
    struct type_chain_iterator<void> {
        template<class F>
        static void for_each(F fn) {}
    };

    template<class List, size_t Index = 0>
    struct type_chain_index_iterator {
        template<class F>
        static void for_each(F &fn) {
            fn.template operator()<List>(Index);
            type_chain_index_iterator<typename List::next, Index + 1>::for_each(fn);
        }
    };

    template<size_t Index>
    struct type_chain_index_iterator<void, Index> {
        template<class F>
        static void for_each(const F &fn) {}
    };

    template<size_t Index>
    struct type_list_index_iterator<void, Index> {
        template<class F>
        static void for_each(F fn) {}
    };

    template<int Index, int Pos, class Default, class List>
    struct type_at_index_iter {
        using current = typename List::type;
        using next = type_at_index_iter<Index, Pos + 1, Default, typename List::next>;
        using result = typename conditional_type<Index == Pos, current, typename next::result>::type;
    };

    template<int Index, int Pos, class Default>
    struct type_at_index_iter<Index, Pos, Default, void> {
        using result = Default;
    };

    template<int Index, class Default, class List>
    struct type_at_index {
        using result = typename type_at_index_iter<Index, 0, Default, List>::result;
    };

    template<int Index, class M, class List>
    struct index_of_type_iter {
        static constexpr auto index = Index;
        static constexpr bool m = M::template match<typename List::type>::value;
        using next = index_of_type_iter<Index + 1, M, typename List::next>;
        static constexpr int result = m ? index : next::result;
    };

    template<int Index, class M>
    struct index_of_type_iter<Index, M, void> {
        static constexpr int result = -1;
    };

    template<class M, class List>
    struct index_of_type {
        static constexpr int result = index_of_type_iter<0, M, List>::result;
    };
}}

#endif
