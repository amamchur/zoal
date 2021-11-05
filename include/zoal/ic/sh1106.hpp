#ifndef ZOAL_IC_SH1106_HPP
#define ZOAL_IC_SH1106_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"
#include "./ssd1306.hpp"

#include <stdint.h>
#include <string.h>

namespace zoal { namespace ic {

    template<int Width, int Height, bool Safe = true>
    using sh1106_adapter_0 = ssd1306_adapter_0<Width, Height, Safe>;

    template<int Width, int Height, bool Safe = true>
    using sh1106_adapter_90 = ssd1306_adapter_90<Width, Height, Safe>;

    template<int Width, int Height, bool Safe = true>
    using sh1106_adapter_180 = ssd1306_adapter_180<Width, Height, Safe>;

    template<int Width, int Height, bool Safe = true>
    using sh1106_adapter_270 = ssd1306_adapter_270<Width, Height, Safe>;

    enum class sh1106_resolution { sh1106_128x64, sh1106_128x32, sh1106_128x16 };

    template<sh1106_resolution Resolution>
    class sh1106_resolution_info {};

    template<>
    class sh1106_resolution_info<sh1106_resolution::sh1106_128x64> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 64;
        static constexpr size_t buffer_size = width * height / 8;
    };

    template<>
    class sh1106_resolution_info<sh1106_resolution::sh1106_128x32> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 32;
        static constexpr size_t buffer_size = width * height / 8;
    };

    template<>
    class sh1106_resolution_info<sh1106_resolution::sh1106_128x16> {
    public:
        static constexpr uint8_t width = 128;
        static constexpr uint8_t height = 16;
        static constexpr size_t buffer_size = width * height / 8;
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
        set_display_start_line = 0x40,
        set_memory_address_mode = 0x20,
        set_page_address = 0xB0,
        set_com_output_scan_inc = 0xC0,
        set_com_output_scan_dec = 0xC8,
        set_segment_remap = 0xA0,
        set_charge_pump = 0x8D,
        set_column_low_address = 0x00,
        set_column_high_address = 0x10
    };

    template<uint8_t Width, uint8_t Height, class Spi, class Reset, class DataSelect, class ChipSelect, class Delay>
    class sh1106_spi {
    public:
        using spi = Spi;
        using res = Reset;
        using ds = DataSelect;
        using cs = ChipSelect;
        using delay = Delay;

        static_assert(Width > 0, "Width must be greater than 0");
        static_assert(Width <= 128, "Width must be less or equal to 128");
        static_assert(Height > 0, "Height must be greater than 0");
        static_assert(Height <= 64, "Height must be less or equal to 64");

        static constexpr uint8_t width = Width;
        static constexpr uint8_t height = Height;
        static constexpr size_t buffer_size = width * height / 8;

        struct buffer_type {
            uint8_t control_buffer[16];
            uint8_t canvas[buffer_size];
        };

        buffer_type buffer;

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd) {
            *ptr++ = cmd;
            return ptr;
        }

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd, uint8_t arg1) {
            *ptr++ = cmd;
            *ptr++ = arg1;
            return ptr;
        }

        static inline uint8_t *add_command(uint8_t *ptr, uint8_t cmd, uint8_t arg1, uint8_t arg2) {
            *ptr++ = cmd;
            *ptr++ = arg1;
            *ptr++ = arg2;
            return ptr;
        }

        void write_command(uint8_t *command) {
            typename ds::_0();
            typename cs::_0();
            spi::transfer(command, nullptr, 1);
            typename cs::_1();
        }

        void init() {
            auto start = buffer.control_buffer;
            auto end = start;

            typename res::_0();
            delay::ms(1);
            typename res::_1();
            delay::ms(1);

            end = add_command(end, display_off);
            end = add_command(end, set_display_clock_divide_ratio, 0x80);
            end = add_command(end, set_multiplex_ratio, 0x3F);
            end = add_command(end, set_display_offset, 0x00);
            end = add_command(end, set_display_start_line | 0x00);
            end = add_command(end, set_charge_pump, 0x14);
            end = add_command(end, set_memory_address_mode, 0x00);
            end = add_command(end, set_segment_remap | 0x1);
            end = add_command(end, set_com_output_scan_dec);
            end = add_command(end, set_com_pin_hardware_cfg, 0x12);
            end = add_command(end, set_contrast, 0xCF);
            end = add_command(end, set_precharge_period, 0xF1);
            end = add_command(end, set_vcom_detect_level, 0x40);
            end = add_command(end, entire_display_resume, 0x40);
            end = add_command(end, normal_display, 0x40);
            end = add_command(end, display_on);

            typename ds::_0();
            typename cs::_0();
            spi::transfer(start, nullptr, end - start);
            typename cs::_1();
        }

        void display_page(uint8_t page) {
            using zoal::periph::i2c_request_status;

            if (page >= (height >> 3)) {
                return;
            }

            auto start = buffer.control_buffer;
            auto end = start;
            end = add_command(end, set_page_address | page);
            end = add_command(end, set_column_low_address | 0x02);
            end = add_command(end, set_column_high_address | 0x00);

            typename ds::_0();
            typename cs::_0();
            spi::transfer(start, nullptr, end - start);
            typename cs::_1();

            typename ds::_1();
            typename cs::_0();
            auto page_data = buffer.canvas + width * page;
            spi::transfer(page_data, nullptr, width);
            typename cs::_1();
        }

        void display() {
            for (int i = 0; i < (height >> 3); i++) {
                display_page(i);
            }
        }
    };

    template<uint8_t Width, uint8_t Height, uint8_t Address = 0x3C>
    class sh1106_i2c : public zoal::periph::i2c_device {
    public:
        static constexpr uint8_t i2c_address = Address;

        static_assert(Width > 0, "Width must be greater than 0");
        static_assert(Width <= 128, "Width must be less or equal to 128");
        static_assert(Height > 0, "Height must be greater than 0");
        static_assert(Height <= 64, "Height must be less or equal to 64");

        static constexpr uint8_t width = Width;
        static constexpr uint8_t height = Height;
        static constexpr size_t buffer_size = width * height / 8;

        struct buffer_type {
            uint8_t control_buffer[16];
            uint8_t canvas[buffer_size];
        };

        buffer_type buffer;

        static const uint8_t co_bit = 1 << 7;
        static const uint8_t dc_bit = 1 << 6;

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
            auto start = buffer.control_buffer;
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

            prepare_request(disp, start, ptr);

            this->next_sequence(disp, [](Dispatcher &dispatcher, zoal::periph::i2c_request_status) { dispatcher.finish_sequence(0); });
            return disp.make_finisher();
        }

        template<class Dispatcher>
        void display_page(uint8_t page, Dispatcher &dispatcher) {
            using zoal::periph::i2c_request_status;

            if (page >= (height >> 3)) {
                dispatcher.finish_sequence(0);
                return;
            }

            auto start = buffer.control_buffer;
            auto ptr = start;
            ptr = add_command(ptr, set_page_address | page);
            ptr = add_command(ptr, set_column_low_address | 0x02);
            ptr = add_command(ptr, set_column_high_address | 0x00);
            *ptr++ = dc_bit;

            auto page_data = buffer.canvas + width * page;
            prepare_request(dispatcher, buffer.control_buffer, ptr, page_data, page_data + width);

            auto cb = [this, page](Dispatcher &d, i2c_request_status) { display_page(page + 1, d); };
            this->next_sequence(dispatcher, cb);
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type display(Dispatcher &disp) {
            display_page(0, disp);
            return disp.make_finisher();
        }

    private:
        template<class Dispatcher>
        void prepare_request(Dispatcher &disp, uint8_t *s, uint8_t *e, uint8_t *es = nullptr, uint8_t *ee = nullptr) {
            auto req = disp.acquire_request();
            req->write(i2c_address, s, e);
            req->extra_ptr = es;
            req->extra_end = ee;
        }
    };
}}

#endif
