#ifndef ZOAL_IC_LSM303DLHC_HPP
#define ZOAL_IC_LSM303DLHC_HPP

#include "../data/vector.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"

namespace zoal { namespace ic {
    class lsm303dlhc_api : public zoal::periph::i2c_device {
    public:
        enum registers : uint8_t {
            ctrl_reg1_a = 0x20,
            ctrl_reg2_a = 0x21,
            ctrl_reg3_a = 0x22,
            ctrl_reg4_a = 0x23,
            ctrl_reg5_a = 0x24,
            ctrl_reg6_a = 0x25,
            reference_a = 0x26,
            status_reg_a = 0x27,
            out_x_l_a = 0x28,
            out_x_h_a = 0x29,
            out_y_l_a = 0x2a,
            out_y_h_a = 0x2b,
            out_z_l_a = 0x2c,
            out_z_h_a = 0x2d,
            fifo_ctrl_reg_a = 0x2e,
            fifo_src_reg_a = 0x2f,
            int1_cfg_a = 0x30,
            int1_source_a = 0x31,
            int1_ths_a = 0x32,
            int1_duration_a = 0x33,
            int2_cfg_a = 0x34,
            int2_source_a = 0x35,
            int2_ths_a = 0x36,
            int2_duration_a = 0x37,
            click_cfg_a = 0x38,
            click_src_a = 0x39,
            click_ths_a = 0x3a,
            time_limit_a = 0x3b,
            time_latency_a = 0x3c,
            time_window_a = 0x3d
        };

        enum values : uint8_t {
            ctrl_reg1_a_odr_down = 0b0000 << 4,
            ctrl_reg1_a_odr_1hz = 0b0001 << 4,
            ctrl_reg1_a_odr_10hz = 0b0010 << 4,
            ctrl_reg1_a_odr_25hz = 0b0011 << 4,
            ctrl_reg1_a_odr_50hz = 0b0100 << 4,
            ctrl_reg1_a_odr_100hz = 0b0101 << 4,
            ctrl_reg1_a_odr_200hz = 0b0110 << 4,
            ctrl_reg1_a_odr_400Hz = 0b0111 << 4,

            ctrl_reg1_a_lp_en = 0b0001 << 3,
            ctrl_reg1_a_z_en = 0b0001 << 2,
            ctrl_reg1_a_y_en = 0b0001 << 1,
            ctrl_reg1_a_x_en = 0b0001 << 0,
            ctrl_reg1_a_x_y_x_en = 0b0111 << 0,

            ctrl_reg4_a_bdu = 0b01 << 7,
            ctrl_reg4_a_ble = 0b01 << 6,
            ctrl_reg4_a_fs_2g = 0b00 << 4,
            ctrl_reg4_a_fs_4g = 0b01 << 4,
            ctrl_reg4_a_fs_8g = 0b10 << 4,
            ctrl_reg4_a_fs_16g = 0b11 << 4,
            ctrl_reg4_a_hr = 0b01 << 3,
            ctrl_reg4_a_sim = 0b01 << 0
        };

        using buffer_type = zoal::periph::i2c_device_buffer<2, 6>;

        explicit lsm303dlhc_api(uint8_t addr = 0x19)
            : address_(addr) {}

        template<class Dispatcher>
        typename Dispatcher::finisher_type config(Dispatcher &disp, uint8_t reg, uint8_t value) {
            using zoal::periph::i2c_request_status;

            buffer->command[0] = reg;
            buffer->command[1] = value;

            auto req = disp.acquire_request();
            req->write(address_, buffer->command, buffer->command + 2);
            next_sequence(disp, notify_client<Dispatcher>);

            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type fetch(Dispatcher &disp) {
            using zoal::periph::i2c_request_status;

            auto address_assigned = [this](Dispatcher &dispatcher, i2c_request_status s) {
                if (s == i2c_request_status::finished) {
                    auto req = dispatcher.acquire_request();
                    req->read(address_, this->buffer->data, this->buffer->data + 6);
                    next_sequence(dispatcher, notify_client<Dispatcher>);
                } else {
                    dispatcher.finish_sequence(-1);
                }
            };

            buffer->command[0] = out_x_l_a | 0x80u;

            auto req = disp.acquire_request();
            req->write(address_, buffer->command, buffer->command + 1);
            next_sequence(disp, address_assigned);

            return disp.make_finisher();
        }

        zoal::data::vector<uint16_t> vector() const {
            return {static_cast<uint16_t>(buffer->data[1] << 8 | buffer->data[0]),
                    static_cast<uint16_t>(buffer->data[3] << 8 | buffer->data[2]),
                    static_cast<uint16_t>(buffer->data[5] << 8 | buffer->data[4])};
        }

        zoal::data::vector<int16_t> vector_sing() const {
            auto ptr = reinterpret_cast<int16_t *>(buffer->data);
            return {ptr[0], ptr[1], ptr[2]};
        }

        uint8_t address_{0};
        buffer_type *buffer{nullptr};
    };

    template<zoal::periph::device_buffer_type BufferType = zoal::periph::device_buffer_type::static_mem>
    using lsm303dlhc = typename zoal::ct::conditional_type<BufferType == zoal::periph::device_buffer_type::static_mem,
                                                           zoal::periph::i2c_buffer_owner<lsm303dlhc_api>,
                                                           zoal::periph::i2c_buffer_guardian<lsm303dlhc_api>>::type;
}}

#endif
