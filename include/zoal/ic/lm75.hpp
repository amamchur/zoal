#ifndef ZOAL_IC_LM75_HPP
#define ZOAL_IC_LM75_HPP

#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"

namespace zoal { namespace ic {
    class lm75 : public zoal::periph::i2c_device {
    public:
        enum class register_address : uint8_t { temperature = 0x00, configuration = 0x01, t_hyst = 0x02, t_os = 0x03 };

        explicit lm75(uint8_t addr = 0x48)
            : address_(addr) {}

        template<class Dispatcher>
        typename Dispatcher::finisher_type fetch(Dispatcher &disp) {
            auto address_assigned = [this](Dispatcher &dispatcher, zoal::periph::i2c_request_status) {
                auto req = dispatcher.acquire_request();
                req->read(address_, this->data_, this->data_ + sizeof(this->data_));

                next_sequence(dispatcher, notify_client<Dispatcher>);
            };

            auto req = disp.acquire_request();
            req->write(address_, &reg_addr, &reg_addr + sizeof(reg_addr));
            next_sequence(disp, address_assigned);

            return disp.make_finisher();
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

        uint8_t address_{0};
        uint8_t reg_addr{static_cast<uint8_t>(register_address::temperature)};
        uint8_t data_[6]{0};
    };
}}

#endif
