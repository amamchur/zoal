#ifndef ZOAL_IC_SSD1306_HPP
#define ZOAL_IC_SSD1306_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"

#include <stdint.h>
#include <string.h>

namespace zoal { namespace ic {
    enum class cntr_flags : uint8_t { co_bit = 1 << 7, dc_bit = 1 << 6 };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter {
    public:
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
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_0::ssd1306_pixel(data, x, y, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_90 : public ssd1306_adapter<Width, Height, safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_90::ssd1306_pixel(data, Width - y - 1, x, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_180 : public ssd1306_adapter<Width, Height, safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
            ssd1306_adapter_180::ssd1306_pixel(data, Width - x - 1, Height - y - 1, value);
        }
    };

    template<int Width, int Height, bool safe = true>
    class ssd1306_adapter_270 : public ssd1306_adapter<Width, Height, safe> {
    public:
        inline static void pixel(void *data, int x, int y, uint8_t value) {
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

    template<class Delay, class Reset, class SlaveAddressSelect, uint8_t Address = 0x3C>
    class ssd1306_interface_i2c : public zoal::periph::i2c_device {
    public:
        using device_type = zoal::periph::i2c_device;
        using self_type = ssd1306_interface_i2c<Delay, Reset, SlaveAddressSelect, Address>;
        using delay = Delay;
        using rst = Reset;
        using sas = SlaveAddressSelect;
        static constexpr uint8_t address = Address;

        using gpio_cfg = typename zoal::gpio::api::optimize<
            //
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output, rst, sas>>;

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

        template<class Dispatcher>
        void prepare_request(Dispatcher &disp, uint8_t *s, uint8_t *e) {
            auto req = disp.acquire_request();
            req->write(address, s, e);
        }
    };

    template<ssd1306_resolution Resolution, class Interface>
    class ssd1306 : public Interface::device_type {
    public:
        static constexpr ssd1306_resolution resolution = Resolution;
        using iface = Interface;
        using resolution_info = ssd1306_resolution_info<resolution>;

        struct buffer_type {
            uint8_t control_buffer[16];
            uint8_t canvas[resolution_info::buffer_size];
        };

        buffer_type buffer;

        static const uint8_t co_bit = 1 << 7;
        static const uint8_t dc_bit = 1 << 6;

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

        void assign_addr() {
            iface_.init();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type init(Dispatcher &disp) {
            auto ptr = reinterpret_cast<uint8_t *>(&buffer);
            *ptr++ = co_bit, *ptr++ = display_off;
            *ptr++ = co_bit, *ptr++ = set_display_clock_divide_ratio;
            *ptr++ = co_bit, *ptr++ = 0x80; // the suggested ratio 0x80
            *ptr++ = co_bit, *ptr++ = set_multiplex_ratio; // 0xA8
            *ptr++ = co_bit, *ptr++ = 0x3F;
            *ptr++ = co_bit, *ptr++ = set_display_offset; // 0xD3
            *ptr++ = co_bit, *ptr++ = 0x0; // no offset
            *ptr++ = co_bit, *ptr++ = set_display_start_line | 0x0; // line #0
            *ptr++ = co_bit, *ptr++ = set_charge_pump; // 0x8D
            *ptr++ = co_bit, *ptr++ = 0x14;
            *ptr++ = co_bit, *ptr++ = set_memory_address_mode; // 0x20
            *ptr++ = co_bit, *ptr++ = 0x00; // 0x0 act like ks0108
            *ptr++ = co_bit, *ptr++ = set_segment_remap | 0x1;
            *ptr++ = co_bit, *ptr++ = set_com_output_scan_dec;
            *ptr++ = co_bit, *ptr++ = set_com_pin_hardware_cfg; // 0xDA
            *ptr++ = co_bit, *ptr++ = 0x12;
            *ptr++ = co_bit, *ptr++ = set_contrast; // 0x81
            *ptr++ = co_bit, *ptr++ = 0xCF;
            *ptr++ = co_bit, *ptr++ = set_precharge_period; // 0xd9
            *ptr++ = co_bit, *ptr++ = 0xF1;
            *ptr++ = co_bit, *ptr++ = set_vcom_detect_level; // 0xDB
            *ptr++ = co_bit, *ptr++ = 0x40;
            *ptr++ = co_bit, *ptr++ = entire_display_resume; // 0xA4
            *ptr++ = co_bit, *ptr++ = normal_display; // 0xA6
            *ptr++ = 0, *ptr++ = display_on; //--turn on oled panel

            iface_.prepare_request(disp, reinterpret_cast<uint8_t *>(&buffer), ptr);

            this->next_sequence(disp, [](Dispatcher &dispatcher, zoal::periph::i2c_request_status) { dispatcher.finish_sequence(0); });
            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type display(Dispatcher &disp) {
            uint8_t *ptr = buffer.control_buffer + sizeof(buffer.control_buffer);
            // Backward filling
            *--ptr = dc_bit;
            *--ptr = (resolution_info::height / 8) - 1; *--ptr = co_bit;
            *--ptr = 0; *--ptr = co_bit;
            *--ptr = set_page_address; *--ptr = co_bit;
            *--ptr = resolution_info::width - 1; *--ptr = co_bit;
            *--ptr = 0; *--ptr = co_bit;
            *--ptr = set_column_address; *--ptr = co_bit;

            iface_.prepare_request(disp, ptr, buffer.canvas + sizeof(buffer.canvas));

            this->next_sequence(disp, [](Dispatcher &dispatcher, zoal::periph::i2c_request_status) { dispatcher.finish_sequence(0); });
            return disp.make_finisher();
        }

        iface iface_;
    };
}}

#endif
