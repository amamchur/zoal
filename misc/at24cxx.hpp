#ifndef ZOAL_IC_AT24C32_HPP
#define ZOAL_IC_AT24C32_HPP

#include <stdint.h>
#include <zoal/ct/conditional_type.hpp>
#include <zoal/periph/i2c_device.hpp>
#include <zoal/periph/i2c_request.hpp>

namespace zoal { namespace ic {
    class at24cxx_api : public zoal::periph::i2c_device {
    public:
        using buffer_type = zoal::periph::i2c_device_buffer<2, 1>;

        explicit at24cxx_api(uint8_t addr = 0x57)
            : address_(addr) {}

        template<class Dispatcher>
        typename Dispatcher::finisher_type write(Dispatcher &disp, uint32_t pos, void *src, size_t size) {
            buffer->command[0] = static_cast<uint8_t>(pos >> 16);
            buffer->command[1] = static_cast<uint8_t>(pos & 0xFF);

            auto req = disp.acquire_request();
            req->write(address_, buffer->command, buffer->command + 6);
            req->extra_ptr = reinterpret_cast<uint8_t *>(src);
            req->extra_end = req->extra_ptr + size;
            next_sequence(disp, notify_client<Dispatcher>);
            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type read(Dispatcher &disp, uint32_t pos, void *dst, size_t size) {
            using zoal::periph::i2c_request_status;

            buffer->command[0] = static_cast<uint8_t>(pos >> 16);
            buffer->command[1] = static_cast<uint8_t>(pos & 0xFF);

            auto address_assigned = [this, dst, size](Dispatcher &dispatcher, i2c_request_status s) {
                if (s == i2c_request_status::finished) {
                    auto req = dispatcher.acquire_request();
                    req->read(address_, reinterpret_cast<uint8_t *>(dst), reinterpret_cast<uint8_t *>(dst) + size);
                    next_sequence(dispatcher, notify_client<Dispatcher>);
                } else {
                    dispatcher.finish_sequence(-1);
                }
            };

            auto req = disp.acquire_request();
            req->write(address_, buffer->command, buffer->command + 2);
            next_sequence(disp, address_assigned);

            return disp.make_finisher();
        }

        uint8_t address_{0};
        buffer_type *buffer{nullptr};
    };

    template<zoal::periph::device_buffer_type BufferType = zoal::periph::device_buffer_type::static_mem>
    using at24cxx = typename zoal::ct::conditional_type<BufferType == zoal::periph::device_buffer_type::static_mem,
                                                        zoal::periph::i2c_buffer_owner<at24cxx_api>,
                                                        zoal::periph::i2c_buffer_guardian<at24cxx_api>>::type;
}}

#endif
