#ifndef ZOAL_IC_LM75_HPP
#define ZOAL_IC_LM75_HPP

#include "../utils/logger.hpp"

namespace zoal { namespace ic {
    template<uint8_t Address = 0x48>
    class lm75 {
    public:
        using self_type = lm75<Address>;

        enum class register_address : uint8_t { temperature = 0x00, configuration = 0x01, t_hyst = 0x02, t_os = 0x03 };

        template<class Stream>
        void fetch(Stream *stream) {
            ready_ = false;

            stream->callback = &i2c_callback<Stream>::address_assigned;
            stream->token = this;
            stream->write(Address).value(static_cast<uint8_t>(register_address::temperature));
            Stream::i2c::transmit(stream);
        }

        uint8_t operator[](register_address addr) const {
            return data_[static_cast<uintptr_t>(addr)];
        }

        volatile uint8_t &operator[](register_address addr) {
            return data_[static_cast<uintptr_t>(addr)];
        }

        float temperature() {
            uint8_t msb = data_[0];
            uint8_t lsb = data_[1];
            return ((int16_t)(msb << 8) | lsb) / 256.0f;
        }

        void wait() const {
            while (!ready_)
                ;
        }

        bool ready() const {
            return ready_;
        }

    private:
        template<class Stream>
        class i2c_callback {
        public:
            static void address_assigned(Stream *stream, void *token) {
                stream->read(Address, sizeof(data_));
                stream->callback = &i2c_callback<Stream>::data_fetched;
                Stream::i2c::transmit(stream);
            }

            static void data_fetched(Stream *stream, void *token) {
                auto *obj = reinterpret_cast<self_type *>(token);
                for (uint8_t i = 0; i < stream->size(); i++) {
                    obj->data_[i] = stream->data_[i];
                }

                obj->ready_ = true;
            }
        };

        volatile bool ready_{false};

        volatile uint8_t data_[6]{0};
    };
}}

#endif
