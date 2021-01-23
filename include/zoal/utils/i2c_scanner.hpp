#ifndef ZOAL_UTILS_I2C_SCANNER_HPP
#define ZOAL_UTILS_I2C_SCANNER_HPP

#include "../periph/i2c_request.hpp"

namespace zoal { namespace utils {
    class i2c_scanner {
    public:
        void scan(zoal::periph::i2c_request &request) {
            request.initiator = this;
            request.token = 1;
            request.write(request.token, nullptr, nullptr);
        }

        template<class C>
        zoal::periph::i2c_request_completion_result complete_request(zoal::periph::i2c_request &request, C callback) {
            if (request.initiator != this) {
                return zoal::periph::i2c_request_completion_result::ignored;
            }

            switch (request.result) {
            case periph::i2c_result::idle:
            case periph::i2c_result::pending:
                return zoal::periph::i2c_request_completion_result::ignored;
            case periph::i2c_result::ok:
                callback(static_cast<uint8_t>(request.token));
                break;
            default:
                break;
            }

            if (request.token < 0x7F) {
                request.initiator = this;
                request.token = request.token + 1;
                request.write(request.token, nullptr, nullptr);
                return zoal::periph::i2c_request_completion_result::new_request;
            }

            return zoal::periph::i2c_request_completion_result::finished;
        }
    };
}}

#endif
