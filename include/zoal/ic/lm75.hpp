#ifndef ZOAL_IC_LM75_HPP
#define ZOAL_IC_LM75_HPP

#include "../periph/i2c_request.hpp"

namespace zoal { namespace ic {
    class lm75 {
    public:
        enum lm75_request {
            //
            unknown,
            reg_temp_address_assigment,
            reg_temp_data_fetch
        };

        enum class register_address : uint8_t { temperature = 0x00, configuration = 0x01, t_hyst = 0x02, t_os = 0x03 };

        lm75() = default;
        explicit lm75(uint8_t addr)
            : address_(addr) {}

        void fetch(zoal::periph::i2c_request &request) {
            request.initiator = this;
            request.token = reg_temp_address_assigment;
            request.write(address_, &reg_addr, &reg_addr + sizeof(reg_addr));
        }

        zoal::periph::i2c_request_completion_result complete_request(zoal::periph::i2c_request &request) {
            if (request.initiator != this || request.result != zoal::periph::i2c_result::ok) {
                return zoal::periph::i2c_request_completion_result::ignored;
            }

            switch (request.token) {
            case reg_temp_address_assigment:
                request.token = reg_temp_data_fetch;
                request.read(address_, data_, data_ + sizeof(data_));
                return zoal::periph::i2c_request_completion_result::new_request;
            case reg_temp_data_fetch:
                return zoal::periph::i2c_request_completion_result::finished;
            }

            return zoal::periph::i2c_request_completion_result::ignored;
        }

        float temperature() {
            uint8_t msb = data_[0];
            uint8_t lsb = data_[1];
            bool negative = (msb & 0x80) != 0;
            uint16_t twos_complement = static_cast<uint16_t>(msb) << 8 | lsb;
            uint16_t absolute_x256 = negative ? (~twos_complement + 1u) : twos_complement;
            uint16_t absolute_x2 = absolute_x256 >> 7;
            auto result = static_cast<float>(absolute_x2) * (negative ? -0.5f : 0.5f);
            return result;
        }

        uint8_t address_{0x48};
        uint8_t reg_addr{static_cast<uint8_t>(register_address::temperature)};
        uint8_t data_[6]{0};
    };
}}

#endif
