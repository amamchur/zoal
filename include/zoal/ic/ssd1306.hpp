#ifndef ZOAL_IC_SSD1306_HPP
#define ZOAL_IC_SSD1306_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../periph/i2c_request.hpp"

#include <stdint.h>
#include <string.h>

namespace zoal { namespace ic {
    enum class cntr_flags : uint8_t { co_bit = 1 << 7, dc_bit = 1 << 6 };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter {
    public:
        static void ssd1306_h_line(void *data, int x, int y, int w, uint8_t value) {
            if (safe && ((y < 0) || (y >= Height))) {
                return;
            }

            auto buffer = reinterpret_cast<uint8_t *>(data);
            auto mask = 1 << (y & 0x07);

            for (int i = 0; i < w; i++) {
                if ((x < 0) || (x >= Width)) {
                    continue;
                }

                auto ptr = buffer + x + i + (y / 8) * Width;
                if (value) {
                    *ptr |= mask;
                } else {
                    *ptr &= ~mask;
                }
            }
        }

        static void ssd1306_v_line(void *data, int x, int y, int h, uint8_t value) {
            if (safe && ((x < 0) || (x >= Width))) {
                return;
            }

            auto t = (y + 0x7) & 0xF8;
            auto b = (y + h) & 0xF8;
            auto buffer = reinterpret_cast<uint8_t *>(data);
            for (int i = t; i < b && i < Height; i++) {
                auto ptr = buffer + x + (i / 8) * Width;
                *ptr = static_cast<uint8_t>(value ? 0xFF : 0x00);
            }

            for (int i = y; i < t; i++) {
                auto ptr = buffer + x + (i / 8) * Width;
                if (value) {
                    *ptr |= 1 << (i & 0x07);
                } else {
                    *ptr &= ~(1 << (i & 0x07));
                }
            }

            for (int i = b; i <= y + h; i++) {
                auto ptr = buffer + x + (i / 8) * Width;
                if (value) {
                    *ptr |= 1 << (i & 0x07);
                } else {
                    *ptr &= ~(1 << (i & 0x07));
                }
            }
        }

        static void ssd1306_pixel(void *data, int x, int y, uint8_t value) {
            if (safe && ((x < 0) || (x >= Width) || (y < 0) || (y >= Height))) {
                return;
            }

            auto buffer = reinterpret_cast<uint8_t *>(data);
            auto bit = y & 0x07;
            auto ptr = buffer + x + (y / 8) * Width;
            if (value) {
                *ptr |= 1 << bit;
            } else {
                *ptr &= ~(1 << bit);
            }
        }

        static void clear(void *data, uint8_t v) {
            memset(data, v ? 0xFF : 0x00, static_cast<size_t>(Width * Height / 8));
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_0 : public ssd1306_adapter<Width, Height, safe> {
    public:
        static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_0::ssd1306_pixel(data, x, y, value);
        }

        static void h_line(void *data, int x, int y, int w, uint8_t value) {
            ssd1306_adapter_0::ssd1306_h_line(data, x, y, w, value);
        }

        static void v_line(void *data, int x, int y, int h, uint8_t value) {
            ssd1306_adapter_0::ssd1306_v_line(data, x, y, h, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_90 : public ssd1306_adapter<Width, Height, safe> {
    public:
        static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_90::ssd1306_pixel(data, Width - y - 1, x, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_180 : public ssd1306_adapter<Width, Height, safe> {
    public:
        static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_180::ssd1306_pixel(data, Width - x - 1, Height - y - 1, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_270 : public ssd1306_adapter<Width, Height, safe> {
    public:
        static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_270::ssd1306_pixel(data, y, Height - x - 1, value);
        }
    };

    enum class ssd1306_resolution { ssd1306_128x64, ssd1306_128x32, ssd1306_128x16 };

    template<ssd1306_resolution Resolution>
    class ssd1306_resolution_info {};

    template<>
    class ssd1306_resolution_info<ssd1306_resolution::ssd1306_128x64> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 64;
        static constexpr size_t buffer_size = width * height / 8;
    };

    template<>
    class ssd1306_resolution_info<ssd1306_resolution::ssd1306_128x32> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 32;
        static constexpr size_t buffer_size = width * height / 8;
    };

    template<>
    class ssd1306_resolution_info<ssd1306_resolution::ssd1306_128x16> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 16;
        static constexpr size_t buffer_size = width * height / 8;
    };

    template<class Tools, class IICircuit, class Reset, class SlaveAddressSelect, uint8_t Address = 0x3C>
    class ssd1306_interface_i2c {
    public:
        using self_type = ssd1306_interface_i2c<Tools, IICircuit, Reset, SlaveAddressSelect, Address>;
        using tools = Tools;
        using delay = typename tools::delay;
        using i2c = IICircuit;
        using rst = Reset;
        using sas = SlaveAddressSelect;
        static constexpr uint8_t address = Address;

        using gpio_cfg = typename zoal::gpio::api::optimize<
            //
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output, rst, sas>>;

        explicit ssd1306_interface_i2c(zoal::periph::i2c_request &req)
            : request(req) {}

        void init() {
            if (address == 0x3C) {
                typename sas::low();
            } else {
                typename sas::high();
            }

            typename rst::high();
            delay::ms(1);
            typename rst::low();
            delay::ms(10);
            typename rst::high();
        }

        void command(uint8_t cmd) {
            request.initiator = this;
            request.token = 0;
            cmd_buffer[0] = 0x00;
            cmd_buffer[1] = cmd;
            request.write(address, cmd_buffer, cmd_buffer + 2);
            zoal::periph::process_i2c_request_sync<i2c>(request, *this);
        }

        zoal::periph::i2c_request_completion_result complete_request(zoal::periph::i2c_request &req) {
            if (request.initiator != this) {
                return zoal::periph::i2c_request_completion_result::ignored;
            }

            return zoal::periph::i2c_request_completion_result::finished;
        }

        template<class B>
        void data(B &buffer) {
            request.initiator = this;
            request.token = 0;
            auto p = buffer.control_byte;
            p[0] = 0x40;
            request.write(address, p, p + sizeof(B));
            zoal::periph::process_i2c_request_sync<i2c>(request, *this);
        }

        uint8_t cmd_buffer[8]{0};
        zoal::periph::i2c_request &request;
    };

    template<ssd1306_resolution Resolution, class Interface>
    class ssd1306 {
    public:
        static constexpr ssd1306_resolution resolution = Resolution;
        using iface = Interface;
        using resolution_info = ssd1306_resolution_info<resolution>;

        struct buffer_type {
            uint8_t control_byte[1];
            uint8_t canvas[resolution_info::buffer_size];
        };

        enum : uint8_t {
            set_contrast = 0x81,
            entire_display_resume = 0xA4,
            entire_display_on = 0xA5,
            normal_display = 0xA6,
            invert_display = 0xA7,
            display_off = 0xAE,
            display_on = 0xAF,
            set_display_offset = 0xD3,
            set_com_pin_hardware_cfg = 0xDA,
            set_vcom_detect_level = 0xDB,
            set_display_clock_divide_ratio = 0xD5,
            set_precharge_period = 0xD9,
            set_multiplex_ratio = 0xA8,
            set_lower_column_start_address = 0x00,
            set_higher_column_start_address = 0x10,
            set_display_start_line = 0x40,
            set_memory_address_mode = 0x20,
            set_column_address = 0x21,
            set_page_address = 0x22,
            set_com_output_scan_inc = 0xC0,
            set_com_output_scan_dec = 0xC8,
            set_segment_remap = 0xA0,
            set_charge_pump = 0x8D
        };

        explicit ssd1306(zoal::periph::i2c_request &request)
            : iface_(request) {}

        void init() {
            iface_.init();
            iface_.command(display_off);
            iface_.command(set_display_clock_divide_ratio);
            iface_.command(0x80); // the suggested ratio 0x80
            iface_.command(set_multiplex_ratio); // 0xA8
            iface_.command(0x3F);
            iface_.command(set_display_offset); // 0xD3
            iface_.command(0x0); // no offset
            iface_.command(set_display_start_line | 0x0); // line #0
            iface_.command(set_charge_pump); // 0x8D
            iface_.command(0x14);
            iface_.command(set_memory_address_mode); // 0x20
            iface_.command(0x00); // 0x0 act like ks0108
            iface_.command(set_segment_remap | 0x1);
            iface_.command(set_com_output_scan_dec);
            iface_.command(set_com_pin_hardware_cfg); // 0xDA
            iface_.command(0x12);
            iface_.command(set_contrast); // 0x81
            iface_.command(0xCF);
            iface_.command(set_precharge_period); // 0xd9
            iface_.command(0xF1);
            iface_.command(set_vcom_detect_level); // 0xDB
            iface_.command(0x40);
            iface_.command(entire_display_resume); // 0xA4
            iface_.command(normal_display); // 0xA6
            iface_.command(display_on); //--turn on oled panel
        }

        void display_sync(buffer_type &buffer_type) {
            iface_.command(set_column_address); // 0x21 COMMAND
            iface_.command(0); // Column start address
            iface_.command(resolution_info::width - 1); // Column end address

            iface_.command(set_page_address); // 0x22 COMMAND
            iface_.command(0); // Start Page address
            iface_.command((resolution_info::height / 8) - 1); // End Page address

            iface_.data(buffer_type);
        }

        zoal::periph::i2c_request_completion_result complete_request(zoal::periph::i2c_request &req) {
            return iface_.complete_request(req);
        }

        iface iface_;
    };
}}

#endif
