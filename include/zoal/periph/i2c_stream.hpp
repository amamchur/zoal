#ifndef ZOAL_PERIPH_I2C_STREAM_HPP
#define ZOAL_PERIPH_I2C_STREAM_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace periph {
    enum class i2c_result {
        ok, end_of_stream, error
    };

    template<class iic>
    class i2c_stream {
    public:
        using i2c = iic;
        using self_type = i2c_stream<iic>;

        static self_type *from_memory(void *buffer, size_t buffer_size) {
            auto *obj = reinterpret_cast<self_type *>(buffer);
            obj->data_ = reinterpret_cast<uint8_t *>(buffer) + sizeof(self_type);
            obj->capacity_ = buffer_size - sizeof(self_type);
            return obj;
        }

        i2c_stream &write(uint8_t address, bool stop = true) {
            address_rw_ = address << 1;
            index_ = 0;
            size_ = 0;
            stop_ = stop;
            return *this;
        }

        i2c_stream &read(uint8_t address, uint16_t count) {
            address_rw_ = static_cast<uint8_t>(address << 1 | 1);
            index_ = 0;
            size_ = count;
            return *this;
        }

        size_t copy(const void *source, size_t count) {
            auto l = reinterpret_cast<const uint8_t *>(source);
            auto r = l + count;
            while (index_ < capacity_ && l < r) {
                data_[size_++] = *l++;
            }

            return static_cast<size_t>(l - reinterpret_cast<const uint8_t *>(source));
        }

        uint8_t address() const {
            return address_rw_;
        }

        size_t size() const {
            return size_;
        }

        size_t capacity() const {
            return capacity_;
        }

        size_t index() const {
            return index_;
        }

        bool has_next() const {
            return index_ < size_;
        }

        bool request_next() const {
            return index_ + 1 < size_;
        }

        uint8_t dequeue() {
            return data_[index_++];
        }

        i2c_stream &value(uint8_t value) {
            data_[size_++] = value;
            return *this;
        }

        void enqueue(uint8_t value) {
            data_[index_++] = value;
        }

        void (*callback)(self_type *, void *){nullptr};

        void *token{nullptr};

        size_t index_;
        size_t size_;
        size_t capacity_;
        uint8_t address_rw_;
        uint8_t *data_;
        i2c_result result{i2c_result::ok};
        bool stop_{true};
    };
}}

#endif
