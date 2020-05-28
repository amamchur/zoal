#ifndef ZOAL_WRITE_ONLY_HPP
#define ZOAL_WRITE_ONLY_HPP

namespace zoal { namespace mem {
    template<uintptr_t Value>
    struct bits_write_only {
        static constexpr uintptr_t value = Value;

        template<class T>
        ZOAL_INLINE_MF static void apply(T &target) {
            target = Value;
        }

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {
            using target = zoal::mem::accessor<T, Address, 0>;
            *target::ptr() = Value;
        }
    };

    template<>
    struct bits_write_only<0> {
        static constexpr uintptr_t value = 0;

        template<class T>
        ZOAL_INLINE_MF static void apply(T &target) {}

        template<uintptr_t Address, class T>
        ZOAL_INLINE_MF static void apply() {}
    };

    template<class A, class... Rest>
    struct merge_bits_write_only {
        using next = typename merge_bits_write_only<Rest...>::result;
        using result = bits_write_only<A::value | next::value>;
    };

    template<class A>
    struct merge_bits_write_only<A> {
        using result = A;
    };
}}

#endif
