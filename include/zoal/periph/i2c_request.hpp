#ifndef ZOAL_PERIPH_I2C_STREAM_HPP
#define ZOAL_PERIPH_I2C_STREAM_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace periph {
    enum class i2c_request_status { idle, pending, finished, finished_eos, failed };

    class i2c_request {
    public:
        i2c_request() = default;
        i2c_request(const i2c_request &) = delete;

        void write(uint8_t address, uint8_t *s, uint8_t *e) {
            address_rw_ = address << 1;
            status = zoal::periph::i2c_request_status::pending;
            ptr = s;
            end = e;
        }

        void read(uint8_t address, uint8_t *s, uint8_t *e) {
            address_rw_ = static_cast<uint8_t>(address << 1 | 1);
            status = zoal::periph::i2c_request_status::pending;
            ptr = s;
            end = e;
        }

        inline uint8_t address_rw() const {
            return address_rw_;
        }

        inline bool has_next() const {
            return ptr < end;
        }

        inline bool request_next() const {
            return ptr < end;
        }

        inline uint8_t dequeue() {
            return *ptr++;
        }

        inline void enqueue(uint8_t value) {
            *ptr++ = value;
        }

        inline void complete(zoal::periph::i2c_request_status r) {
            status = r;
        }

        inline volatile bool processing() {
            return status != zoal::periph::i2c_request_status::idle;
        }

        inline volatile bool finished() {
            switch (status) {
            case i2c_request_status::idle:
            case i2c_request_status::pending:
                return false;
            default:
                return true;
            }
        }

        void reset() {
            address_rw_ = 0;
            status = zoal::periph::i2c_request_status::idle;
            ptr = nullptr;
            end = nullptr;
        }

        uint8_t *ptr{nullptr};
        uint8_t *end{nullptr};
        uint8_t address_rw_{0};
        volatile zoal::periph::i2c_request_status status{zoal::periph::i2c_request_status::idle};
    };
}}

#endif
