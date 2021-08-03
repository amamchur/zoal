#ifndef ZOAL_MEMORY_TARGET_HPP
#define ZOAL_MEMORY_TARGET_HPP

#include <stdint.h>
#include <stdlib.h>

namespace zoal { namespace io {
    class memory_target {
    public:
        memory_target(void *buffer, size_t capacity)
            : buffer_(reinterpret_cast<uint8_t *>(buffer))
            , capacity_(capacity) {
            reset();
        }

        void send_byte(uint8_t v) {
            buffer_[size_++] = v;
        }

        void send_data(const void *data, size_t size) {
            auto d = reinterpret_cast<const uint8_t *>(data);
            for (size_t i = 0; i < size; i++) {
                buffer_[size_++] = d[i];
            }
        }

        void reset() {
            size_ = 0;
        }

        uint8_t *buffer_{nullptr};
        size_t size_{0};
        size_t capacity_{0};
    };
}}

#endif
