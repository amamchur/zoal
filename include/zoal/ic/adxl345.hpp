#ifndef ZOAL_ADXL345_HPP
#define ZOAL_ADXL345_HPP

#include <stdint.h>

namespace zoal { namespace ic {
    template<class I2C, uint8_t Address, class Logger>
    class adxl345 {
    public:
        using self_type = adxl345<I2C, Address, Logger>;

        enum class reg_address : uint8_t {
            device_id = 0x00
        };

        volatile bool ready{false};
        int data_size{1};
        uint8_t data[8];

        void fetch() {
            Logger::info() << "Started: " << (int)Address;

            ready = false;

            I2C::wait();
            I2C::stream().write(Address).value(static_cast<uint8_t >(reg_address::device_id));
            I2C::transmit(&i2c_address_assigned, this);
        }

        static void i2c_address_assigned(void *token) {
            Logger::info() << "i2c_address_assigned: " << I2C::error;

            I2C::stream().read(Address, 1);
            I2C::transmit(&i2c_data_fetched, token);
        }

        static void i2c_data_fetched(void *token) {
            Logger::info() << "i2c_data_fetched: " << I2C::error;

            auto *cls = (self_type *)token;
            auto &stream = I2C::stream();

            for (uint8_t i = 0; i < cls->data_size; i++) {
                Logger::info() << "D: " << (int)stream.data[i];
                cls->data[i] = stream.data[i];
            }

            cls->ready = true;
        }
    };
}}

#endif
