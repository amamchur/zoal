#ifndef ZOAL_PERIPH_I2C_STREAM_HPP
#define ZOAL_PERIPH_I2C_STREAM_HPP

#include <stdint.h>

namespace zoal { namespace periph {
    enum class i2c_result {
        ok,
        error,
        a, b, c, d, e, f, g
    };

    template<class iic>
    class i2c_stream {
    public:
        using i2c = iic;
        using self_type = i2c_stream<iic>;

        explicit i2c_stream(void *buffer)
                : index_(0), length_(0), address_rw_(0) {
            data = (uint8_t *) buffer;
        }

        i2c_stream &write(uint8_t address) {
            address_rw_ = address << 1;
            index_ = 0;
            length_ = 0;
            return *this;
        }

        i2c_stream &read(uint8_t address, uint16_t count) {
            address_rw_ = static_cast<uint8_t>(address << 1 | 1);
            index_ = 0;
            length_ = count;
            return *this;
        }

        uint8_t address() const {
            return address_rw_;
        }

        uint16_t size() const {
            return length_;
        }

        bool has_next() const {
            return index_ < length_;
        }

        bool request_next() const {
            return index_ + 1 < length_;
        }

        uint8_t dequeue() {
            return data[index_++];
        }

        i2c_stream &value(uint8_t value) {
            data[length_++] = value;
            return *this;
        }

        void enqueue(uint8_t value) {
            data[index_++] = value;
        }

        void (*callback)(self_type *, void *){nullptr};

        void *token{nullptr};

        uint16_t index_;
        uint16_t length_;
        uint8_t address_rw_;
        uint8_t *data;
        i2c_result result{i2c_result::ok};
    };
}}

#endif
