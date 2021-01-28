#ifndef ZOAL_GPIO_BIT_ORDER_HPP
#define ZOAL_GPIO_BIT_ORDER_HPP

#include <stdint.h>

namespace zoal { namespace gpio {
    enum class bit_order { lsbf, msbf };

    template<class T, bit_order Order = bit_order::lsbf>
    class push_bit_op {
    public:
        static T push(T value, uint8_t b) {
            constexpr auto shift = static_cast<uint8_t>(sizeof(T) << 3) - 1;
            return (value >> 1) | (static_cast<T>(b) << shift);
        }
    };

    template<class T>
    class push_bit_op<T, bit_order::msbf> {
    public:
        static T push(T value, uint8_t b) {
            return (value << 1) | static_cast<T>(b);
        }
    };
}}

#endif
