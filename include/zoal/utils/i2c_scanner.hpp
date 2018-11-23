#ifndef ZOAL_UTILS_I2C_SCANNER_HPP
#define ZOAL_UTILS_I2C_SCANNER_HPP

#include "../periph/i2c_stream.hpp"

namespace zoal { namespace utils {
    template<class I2C>
    class i2c_scanner {
    public:
        using i2c = I2C;
        using self_type = i2c_scanner<I2C>;

        void scan(zoal::periph::i2c_stream<i2c> *stream, uint8_t address) {
            ready_ = false;

            address_ = static_cast<uint8_t>(address & 0x7Fu);
            stream->callback = &self_type::complete;
            stream->token = this;
            stream->write(address);
            i2c::transmit(stream);
        }

        bool ready() const {
            return ready_;
        }

        zoal::periph::i2c_result result() const {
            return result_;
        }

        void wait() const {
            while (!ready_);
        }

        uint8_t address() const {
            return address_;
        }

    private:
        static void complete(zoal::periph::i2c_stream<i2c> *stream, void *token) {
            auto obj = reinterpret_cast<self_type *>(token);
            obj->result_ = stream->result;
            obj->ready_ = true;
        }

        uint8_t address_{0xFF};
        zoal::periph::i2c_result result_;

        volatile bool ready_{false};
    };
}}

#endif
