#ifndef ZOAL_DATA_RX_TX_BUFFER_HPP
#define ZOAL_DATA_RX_TX_BUFFER_HPP

#include "ring_buffer.hpp"
#include "../utils/cooperation.hpp"

namespace zoal { namespace data {
    template<uintptr_t RxSize, uintptr_t TxSize>
    class rx_tx_buffer {
    public:
        static constexpr auto rx_size = RxSize;
        static constexpr auto tx_size = TxSize;

        using coop = zoal::utils::cooperation<>;

        template<uintptr_t Size>
        using buffer = typename ::zoal::data::ring_buffer<uint8_t, Size, coop::yield>;

        using buffer_rx = buffer<RxSize>;
        using buffer_tx = buffer<TxSize>;

        buffer_rx rx;
        buffer_tx tx;
    };
}}

#endif
