#ifndef ZOAL_RX_BUFFER_HPP
#define ZOAL_RX_BUFFER_HPP

#include "../data/ring_buffer.hpp"
#include "../utils/cooperation.hpp"

#include <stddef.h>

namespace zoal { namespace periph {
    template<class U, size_t Size>
    class rx_buffer {
    public:
        using coop = zoal::utils::cooperation<>;
        using buffer_type = zoal::data::ring_buffer<uint8_t, Size, coop::yield>;
        static buffer_type rx;

        static bool empty() {
            return rx.empty();
        }

        static void put(uint8_t data) {
            return rx.enqueue(data);
        }

        static void flush() {}
    };

    template<class U, size_t Size>
    typename rx_buffer<U, Size>::buffer_type rx_buffer<U, Size>::rx;
}}

#endif //ZOAL_RX_BUFFER_HPP
