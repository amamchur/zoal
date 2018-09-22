#ifndef ZOAL_TX_BUFFER_HPP
#define ZOAL_TX_BUFFER_HPP

#include "../data/ring_buffer.hpp"
#include "../utils/cooperation.hpp"

#include <stddef.h>

namespace zoal { namespace periph {

    template<class U, size_t Size>
    class tx_buffer {
    public:
        using coop = zoal::utils::cooperation<>;
        using buffer_type = zoal::data::ring_buffer<uint8_t, Size, coop::yield>;
        static buffer_type tx;

        static void write_byte(uint8_t data) {
            zoal::utils::interrupts ni(false);
            tx.enqueue(data, true);
            U::enable_tx();
        }

        static bool empty() {
            return tx.empty();
        }

        static uint8_t get() {
            return tx.dequeue();
        }

        static void flush() {}
    };

    template<class U, size_t Size>
    typename tx_buffer<U, Size>::buffer_type tx_buffer<U, Size>::tx;

}}

#endif
