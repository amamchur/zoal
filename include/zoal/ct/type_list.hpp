#ifndef ZOAL_CT_TYPE_LIST_HPP
#define ZOAL_CT_TYPE_LIST_HPP

namespace zoal { namespace ct {
    template<class First, class... Rest>
    struct type_list {
        static constexpr size_t count = sizeof...(Rest) + 1;
        using type = First;
        using next = type_list<Rest...>;
    };

    template<class First>
    struct type_list<First> {
        static constexpr size_t count = 1;
        using type = First;
        using next = void;
    };

    template<class List>
    struct type_list_iterator {
        template<class F>
        static void for_each(F fn) {
            fn.template operator()<typename List::type>();
            type_list_iterator<typename List::next>::for_each(fn);
        }
    };

    template<>
    struct type_list_iterator<void> {
        template<class F>
        static void for_each(F fn) {}
    };
}}

#endif
