#ifndef ZOAL_UTILS_I2C_SCANNER_HPP
#define ZOAL_UTILS_I2C_SCANNER_HPP

#include "../func/function.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"

namespace zoal { namespace utils {
    class i2c_scanner : public zoal::periph::i2c_device {
    public:
        template<class Dispatcher>
        typename Dispatcher::finisher_type scan(Dispatcher &disp) {
            address = 0;
            scan_next(disp);
            return disp.make_finisher();
        }

        zoal::func::function<sizeof(void *) * 8, void, uint8_t> device_found;

    private:
        template<class Dispatcher>
        void scan_next(Dispatcher &m) {
            using zoal::periph::i2c_request_status;

            auto req = m.acquire_request();
            if (req->status == i2c_request_status::finished && device_found) {
                device_found(address);
            }

            if (++address >= 0x7F) {
                m.finish_sequence(0);
                return;
            }

            auto cb = [this](Dispatcher &dispatcher, i2c_request_status) { scan_next(dispatcher); };
            req->write(address, nullptr, nullptr);
            next_sequence(m, cb);
        }

        uint8_t address{0};
    };
}}

#endif
