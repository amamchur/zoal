#ifndef ZOAL_PERIPH_I2C_STREAM_HPP
#define ZOAL_PERIPH_I2C_STREAM_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace periph {
    enum class i2c_error { none, bus_error, abort_lost };

    enum class i2c_result { idle, pending, ok, end_of_stream, error };

    enum class i2c_request_completion_result { ignored, finished, new_request };

    class i2c_request {
    public:
        i2c_request() = default;
        i2c_request(const i2c_request &) = delete;

        void write(uint8_t address, uint8_t *s, uint8_t *e) {
            address_rw_ = address << 1;
            result = zoal::periph::i2c_result::pending;
            ptr = s;
            end = e;
        }

        void read(uint8_t address, uint8_t *s, uint8_t *e) {
            address_rw_ = static_cast<uint8_t>(address << 1 | 1);
            result = zoal::periph::i2c_result::pending;
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

        inline void complete(zoal::periph::i2c_result r) {
            result = r;
        }

        inline volatile bool processing() {
            return result != zoal::periph::i2c_result::idle;
        }

        inline volatile bool finished() {
            switch (result) {
            case i2c_result::idle:
            case i2c_result::pending:
                return false;
            default:
                return true;
            }
        }

        void reset() {
            address_rw_ = 0;
            result = zoal::periph::i2c_result::idle;
            context = nullptr;
            initiator = nullptr;
            token = 0;
            ptr = nullptr;
            end = nullptr;
        }

        void *initiator{nullptr};
        uintptr_t token{0};
        void *context{nullptr};

        uint8_t *ptr{nullptr};
        uint8_t *end{nullptr};

        uint8_t address_rw_{0};
        volatile zoal::periph::i2c_result result{zoal::periph::i2c_result::idle};
    };

    template<class I2Circuit, class H>
    zoal::periph::i2c_request_completion_result process_i2c_request_sync(zoal::periph::i2c_request &req, H &h) {
        I2Circuit::start();
        do {
            if (!req.finished()) {
                continue;
            }

            auto result = h.complete_request(req);
            switch (result) {
            case zoal::periph::i2c_request_completion_result::new_request:
                I2Circuit::start();
                break;
            default:
                return result;
            }
        } while (true);
    }
}}

#endif
