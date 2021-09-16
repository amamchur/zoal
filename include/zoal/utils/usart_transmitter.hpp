#ifndef ZOAL_UTILS_USART_TRANSMITTER_HPP
#define ZOAL_UTILS_USART_TRANSMITTER_HPP

#include "../data/ring_buffer.hpp"
#include "./interrupts.hpp"

namespace zoal { namespace utils {
    template<class U, class TxBuffer, class ScopeLock = zoal::utils::interrupts_off>
    class usart_transmitter {
    public:
        using tx_buffer_type = TxBuffer;

        explicit usart_transmitter(tx_buffer_type &b) noexcept
            : tx_buffer(b) {}

        void send_byte(uint8_t value) {
            bool done = false;
            while (!done) {
                ScopeLock off;
                done = tx_buffer.push_back(value);
            }

            U::enable_tx();
        }

        void send_data(const void *data, size_t size) {
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

        tx_buffer_type &tx_buffer;
    };
}}

#endif
