#pragma once

#ifndef ZOAL_IC_SSD1306_HPP
#define ZOAL_IC_SSD1306_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"
#include "./ssd1306.hpp"

#include <stdint.h>
#include <string.h>

namespace zoal::ic {
    /**
     * Adapter for SSD1306 Graphic Display Data RAM
     * @tparam Width screen width
     * @tparam Height screen height
     * @tparam Safe check pixel ranges
     */
    template<int Width, int Height, bool Safe = true>
    class ssd1306_gddram_adapter {
    public:
        static constexpr size_t buffer_size = Width * Height / 8;

        static void ssd1306_pixel(void *data, int x, int y, uint8_t value) {
            if (Safe && ((x < 0) || (x >= Width) || (y < 0) || (y >= Height))) {
                return;
            }

            auto page = y >> 3;
            auto bit = y & 0x07;
            auto ptr = reinterpret_cast<uint8_t *>(data) + page * Width + x;
            if (value) {
                *ptr |= 1 << bit;
            } else {
                *ptr &= ~(1 << bit);
            }
        }

        static void clear(void *data, uint8_t v) {
            memset(data, v ? 0xFF : 0x00, buffer_size);
        }
    };

    template<int Width, int Height, bool Safe = true>
    class ssd1306_adapter_0 : public ssd1306_gddram_adapter<Width, Height, Safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_0::ssd1306_pixel(data, x, y, value);
        }
    };

    template<int Width, int Height, bool Safe = true>
    class ssd1306_adapter_90 : public ssd1306_gddram_adapter<Width, Height, Safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_90::ssd1306_pixel(data, Width - y - 1, x, value);
        }
    };

    template<int Width, int Height, bool Safe = true>
    class ssd1306_adapter_180 : public ssd1306_gddram_adapter<Width, Height, Safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_180::ssd1306_pixel(data, Width - x - 1, Height - y - 1, value);
        }
    };

    template<int Width, int Height, bool Safe = true>
    class ssd1306_adapter_270 : public ssd1306_gddram_adapter<Width, Height, Safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_270::ssd1306_pixel(data, y, Height - x - 1, value);
        }
    };

    template<uint8_t Address = 0x3C>
    class ssd1306_interface_i2c : public zoal::periph::i2c_device {
    public:
        using device_type = zoal::periph::i2c_device;
        using self_type = ssd1306_interface_i2c<Address>;
        static constexpr uint8_t i2c_address = Address;

        template<class Dispatcher>
        void prepare_request(Dispatcher &disp, uint8_t *s, uint8_t *e, uint8_t *es = nullptr, uint8_t *ee = nullptr) {
            auto req = disp.acquire_request();
            req->write(i2c_address, s, e);
            req->extra_ptr = es;
            req->extra_end = ee;
        }
    };

    template<uint8_t Width, uint8_t Height, class Interface>
    class ssd1306 : public Interface::device_type {
    public:
        static_assert(Width > 0, "Width must be greater than 0");
        static_assert(Width <= 128, "Width must be less or equal to 128");
        static_assert(Height > 0, "Height must be greater than 0");
        static_assert(Height <= 64, "Height must be less or equal to 64");

        static constexpr uint8_t width = Width;
        static constexpr uint8_t height = Height;
        static constexpr size_t buffer_size = width * height / 8;

        using iface = Interface;

        struct buffer_type {
            uint8_t control_buffer[16];
            uint8_t canvas[buffer_size];
        };

        buffer_type buffer;

        static constexpr uint8_t co_bit = 1 << 7;
        static constexpr uint8_t dc_bit = 1 << 6;

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

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd) {
            *ptr++ = co_bit;
            *ptr++ = cmd;
            return ptr;
        }

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd, uint8_t arg1) {
            *ptr++ = co_bit;
            *ptr++ = cmd;
            *ptr++ = co_bit;
            *ptr++ = arg1;
            return ptr;
        }

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd, uint8_t arg1, uint8_t arg2) {
            *ptr++ = co_bit;
            *ptr++ = cmd;
            *ptr++ = co_bit;
            *ptr++ = arg1;
            *ptr++ = co_bit;
            *ptr++ = arg2;
            return ptr;
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type init(Dispatcher &disp) {
            auto start = reinterpret_cast<uint8_t *>(&buffer.control_buffer);
            auto ptr = start;
            ptr = add_command(ptr, display_off);
            ptr = add_command(ptr, set_display_clock_divide_ratio, 0x80);
            ptr = add_command(ptr, set_multiplex_ratio, 0x3F);
            ptr = add_command(ptr, set_display_offset, 0x00);
            ptr = add_command(ptr, set_display_start_line | 0x00);
            ptr = add_command(ptr, set_charge_pump, 0x14);
            ptr = add_command(ptr, set_memory_address_mode, 0x00);
            ptr = add_command(ptr, set_segment_remap | 0x1);
            ptr = add_command(ptr, set_com_output_scan_dec);
            ptr = add_command(ptr, set_com_pin_hardware_cfg, 0x12);
            ptr = add_command(ptr, set_contrast, 0xCF);
            ptr = add_command(ptr, set_precharge_period, 0xF1);
            ptr = add_command(ptr, set_vcom_detect_level, 0x40);
            ptr = add_command(ptr, entire_display_resume, 0x40);
            ptr = add_command(ptr, normal_display, 0x40);
            ptr = add_command(ptr, display_on);

            iface_.prepare_request(disp, start, ptr);

            this->next_sequence(disp, [](Dispatcher &dispatcher, zoal::periph::i2c_request_status) { dispatcher.finish_sequence(0); });
            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type display(Dispatcher &disp) {
            uint8_t *ptr = buffer.control_buffer;

            // Setup page start and end address (triple byte command)
            ptr = add_command(ptr, set_page_address, 0, (height >> 3) - 1);

            // Setup column start and end address (triple byte command)
            ptr = add_command(ptr, set_column_address, 0, width - 1);

            // Next will be data for GDDRAM
            *ptr++ = dc_bit;

            auto cmd_start = buffer.control_buffer;
            auto cmd_end = ptr;
            auto data_start = buffer.canvas;
            auto data_end = buffer.canvas + sizeof(buffer.canvas);
            iface_.prepare_request(disp, cmd_start, cmd_end, data_start, data_end);

            this->next_sequence(disp, [](Dispatcher &dispatcher, zoal::periph::i2c_request_status) { dispatcher.finish_sequence(0); });
            return disp.make_finisher();
        }

        iface iface_;
    };
}

#endif
