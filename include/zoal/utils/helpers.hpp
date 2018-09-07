#ifndef ZOAL_STATIC_ASSERTION_HPP
#define ZOAL_STATIC_ASSERTION_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal {
    namespace utils {
        template<class T, T v>
        struct integral_constant {
            using value_type = T;
            using type = integral_constant<T, v>;

            static constexpr T value = v;

            constexpr operator T() { return v; }
        };

        struct false_type : integral_constant<bool, false> {
        };

        struct true_type : integral_constant<bool, true> {
        };

        template<class T, class U>
        struct is_same : false_type {
        };

        template<class T>
        struct is_same<T, T> : true_type {
        };

        template<class T>
        struct is_pin : integral_constant<bool, T::port::address != 0 && T::offset != 0> {
        };

        template<class Pin, class Port>
        struct belongs_to_port : is_same<typename Pin::port, Port> {
        };

        template<typename T, typename... Rest>
        struct has_duplicates : false_type {
        };

        template<typename T, typename First>
        struct has_duplicates<T, First> : is_same<T, First> {
        };

        template<typename T, typename First, typename... Rest>
        struct has_duplicates<T, First, Rest...> : integral_constant<bool,
                is_same<T, First>::value || has_duplicates<T, Rest...>::value> {
        };

        template<class T, class U>
        struct optional_type {
            using type = T;
        };

        template<class U>
        struct optional_type<void, U> {
            using type = U;
        };

        template<class First, class ... Rest>
        struct pin_count {
            using other = pin_count<Rest...>;
            static constexpr auto value = First::not_pin ? 0 : 1 + other::value;
        };

        template<class First>
        struct pin_count<First> {
            static constexpr auto value = First::not_pin ? 0 : 1;
        };

        template<class T, class U>
        U to_exponents(T value, U pos, size_t radix) {
            for (; value > 0; value /= radix, pos++) {
                auto v = value % radix;
                *pos = v;
            }
            return pos;
        }

        template<class T, class U>
        void apply(T obj, U begin, U end) {
            for (; begin != end; begin++) {
                *begin = obj(*begin);
            }
        }

        template<uintptr_t Clear, uintptr_t Set, uint8_t Shift = 0>
        struct clear_and_set {
            static constexpr uint32_t clear_mask = static_cast<uint32_t>(static_cast<uint64_t >(Clear) << Shift);
            static constexpr uint32_t set_mask = static_cast<uint32_t>(static_cast<uint64_t >(Set) << Shift);

            template<class T>
            static inline void apply(T &value) {
                value = (value & ~clear_mask) | set_mask;
            }
        };

        template<uint8_t Shift>
        struct clear_and_set<0, 0, Shift> {
            template<class T>
            static inline void apply(T &) {
            }
        };

        template<uintptr_t Clear, uint8_t Shift>
        struct clear_and_set<Clear, 0, Shift> {
            static constexpr uint32_t clear_mask = static_cast<uint32_t>(static_cast<uint64_t >(Clear) << Shift);

            template<class T>
            static inline void apply(T &value) {
                value &= ~clear_mask;
            }
        };

        template<uintptr_t Set, uint8_t Shift>
        struct clear_and_set<0, Set, Shift> {
            static constexpr uint32_t set_mask = static_cast<uint32_t>(static_cast<uint64_t >(Set) << Shift);

            template<class T>
            static inline void apply(T &value) {
                value |= set_mask;
            }
        };
    };
}

#endif
