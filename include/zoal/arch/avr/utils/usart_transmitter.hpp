#ifndef ZOAL_ARCH_AVR_UTILS_USART_TX_TRANSPORT_HPP
#define ZOAL_ARCH_AVR_UTILS_USART_TX_TRANSPORT_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../utils/interrupts.hpp"

namespace zoal { namespace utils {
    template<class U, size_t Size, class ScopeLock = zoal::utils::interrupts_off>
    class usart_transmitter {
    public:
        using tx_buffer_type = zoal::data::ring_buffer<uint8_t, 16>;

        static void send_byte(uint8_t value) {
            bool done = false;
            while (!done) {
                ScopeLock off;
                done = tx_buffer.push_back(value);
            }

            U::enable_tx();
        }

        static void send_data(const void *data, size_t size) {
            auto ptr = reinterpret_cast<const char *>(data);
            while (size > 0) {
                ScopeLock off;
                bool run = true;
                while (run && size > 0) {
                    run = tx_buffer.push_back(*ptr);
                    if (run) {
                        size--;
                        ptr++;
                    }
                }
                U::enable_tx();
            }
        }

        static tx_buffer_type tx_buffer;
    };

    template<class U, size_t Size, class ScopeLock>
    typename usart_transmitter<U, Size, ScopeLock>::tx_buffer_type usart_transmitter<U, Size, ScopeLock>::tx_buffer;
}}

#endif
