#ifndef ZOAL_CT_VALUE_LIST_HPP
#define ZOAL_CT_VALUE_LIST_HPP

#include "./conditional_value.hpp"

namespace zoal { namespace ct {
    template<class T, T First, T... Rest>
    struct value_list {
        static constexpr size_t count = sizeof...(Rest) + 1;
        static constexpr T value = First;
        using next = value_list<T, Rest...>;
    };

    template<class T, T First>
    struct value_list<T, First> {
        static constexpr size_t count = 1;
        static constexpr T value = First;
        using next = void;
    };

    template<class List, size_t Index = 0>
    struct value_list_iterator {
        template<class F>
        static void for_each(F fn) {
            fn(Index, List::value);
            value_list_iterator<typename List::next, Index + 1>::for_each(fn);
        }
    };

    template<size_t Index>
    struct value_list_iterator<void, Index> {
        template<class F>
        static void for_each(F fn) {}
    };

    template<int Index, int Pos, class T, T Default, class List>
    struct value_at_index_iter {
        using next = value_at_index_iter<Index, Pos + 1, T, Default, typename List::next>;
        static constexpr T result = conditional_value<Index == Pos, T, List::value, next::result>::value;
    };

    template<int Index, int Pos, class T, T Default>
    struct value_at_index_iter<Index, Pos, T, Default, void> {
        static constexpr T result = Default;
    };

    template<int Index, class T, T Default, class List>
    struct value_at_index {
        static constexpr T result = value_at_index_iter<Index, 0, T, Default, List>::result;
    };

    template<int Index, class T, T Value, class List>
    struct index_of_value_iter {
        static constexpr auto index = Index;
        static constexpr bool m = Value == List::value;
        using next = index_of_value_iter<Index + 1, T, Value, typename List::next>;
        static constexpr int result = m ? index : next::result;
    };

    template<int Index, class T, T Value>
    struct index_of_value_iter<Index, T, Value, void> {
        static constexpr int result = -1;
    };

    template<class T, T Value, class List>
    struct index_of_value {
        static constexpr int result = index_of_value_iter<0, T, Value, List>::result;
    };
}}

#endif
