#ifndef ZOAL_CT_VALUE_LIST_HPP
#define ZOAL_CT_VALUE_LIST_HPP

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
}}

#endif
