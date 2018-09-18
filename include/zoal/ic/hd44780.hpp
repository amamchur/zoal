#ifndef ZOAL_IC_HD44780_HPP
#define ZOAL_IC_HD44780_HPP

#include "../gpio/pin_mode.hpp"
#include "../io/stream.hpp"

#include <stdint.h>

namespace zoal { namespace ic {
    template<class Tools,
             class RegisterSelect,
             class Enable,
             class DataBus4,
             class DataBus5,
             class DataBus6,
             class DataBus7>
    class hd44780_interface_4bit {
    public:
        using tools = Tools;
        using api = typename tools::api;
        using delay = typename tools::delay;
        using gpio_cfg = typename api::template merge<
            typename api::template mode<zoal::gpio::pin_mode::output_push_pull,
                                        RegisterSelect,
                                        Enable,
                                        DataBus4,
                                        DataBus5,
                                        DataBus6,
                                        DataBus7>,
            typename api::template low<RegisterSelect, Enable, DataBus6, DataBus7>,
            typename api::template high<DataBus4, DataBus5>>;

        static constexpr uint8_t bit_mode = 4;

        static void init() {
            using namespace zoal::gpio;

            api::template power_on<RegisterSelect, Enable, DataBus4, DataBus5, DataBus6, DataBus7>::apply();

            pulse_enable();
            delay::ms(5);

            pulse_enable();
            delay::template us<100>();

            DataBus4::low();
            pulse_enable();
            delay::template us<37>();
        }

        static void send(uint8_t value, uint8_t rs) {
            RegisterSelect::write(rs);

            DataBus4::write((value >> 4) & 1);
            DataBus5::write((value >> 5) & 1);
            DataBus6::write((value >> 6) & 1);
            DataBus7::write((value >> 7) & 1);
            pulse_enable();

            DataBus4::write((value >> 0) & 1);
            DataBus5::write((value >> 1) & 1);
            DataBus6::write((value >> 2) & 1);
            DataBus7::write((value >> 3) & 1);
            pulse_enable();
        }

    protected:
        static void pulse_enable() {
            Enable::low();
            delay::template us<1>();
            Enable::high();
            delay::template us<1>();
            Enable::low();
        }
    };

    template<class Tools,
             class RegisterSelect,
             class Enable,
             class DataBus0,
             class DataBus1,
             class DataBus2,
             class DataBus3,
             class DataBus4,
             class DataBus5,
             class DataBus6,
             class DataBus7>
    class hd44780_interface_8bit {
    public:
        using tools = Tools;
        using api = typename tools::api;
        using delay = typename tools::delay;
        using gpio_cfg = typename api::template merge_actions<
                typename api::template mode<zoal::gpio::pin_mode::output_push_pull,
                        RegisterSelect,
                        Enable,
                        DataBus0,
                        DataBus1,
                        DataBus2,
                        DataBus3,
                        DataBus4,
                        DataBus5,
                        DataBus6,
                        DataBus7>,
                typename api::template low<RegisterSelect, Enable, DataBus6, DataBus7>,
                typename api::template high<DataBus4, DataBus5>>;

        static constexpr uint8_t bit_mode = 8;

        static void init() {
            using namespace zoal::gpio;

            pulse_enable();
            delay::template us<5>();

            pulse_enable();
            delay::template us<100>();

            pulse_enable();
            delay::template us<37>();
        }

        static void send(uint8_t value, uint8_t rs) {
            RegisterSelect::write(rs);

            DataBus0::write((value >> 0) & 1);
            DataBus1::write((value >> 1) & 1);
            DataBus2::write((value >> 2) & 1);
            DataBus3::write((value >> 3) & 1);
            DataBus4::write((value >> 4) & 1);
            DataBus5::write((value >> 5) & 1);
            DataBus6::write((value >> 6) & 1);
            DataBus7::write((value >> 7) & 1);
            pulse_enable();
        }

    protected:
        static void pulse_enable() {
            Enable::high();
            delay::template us<4>();
            Enable::low();
        }
    };

    template<uint8_t Columns, uint8_t Rows>
    class hd44780_address_selector {
    public:
        static constexpr auto columns = Columns;
        static constexpr auto rows = Rows;

        static uint8_t get_address(uint8_t row, uint8_t column) {
            uint8_t address;
            switch (row) {
            case 0:
                address = 0x00;
                break;
            case 1:
                address = 0x40;
                break;
            case 2:
                address = columns;
                break;
            default:
                address = static_cast<uint8_t>(0x40 + columns);
                break;
            }

            return address + column;
        }
    };

    template<class Interface, class AddressSelector>
    class hd44780 {
    public:
        using iface = Interface;
        using address_selector = AddressSelector;
        using delay = typename iface::tools::delay;
        using gpio_cfg = typename iface::gpio_cfg;

        enum : uint8_t {
            cmd_clear_display = 0x01,
            cmd_cursor_home = 0x02,
            cmd_entry_mode_set = 0x04,
            cmd_display_ctrl = 0x08,
            cmd_function_set = 0x20,
            cmd_set_cgram_addr = 0x40,
            cmd_set_ddram_addr = 0x80
        };

        using self_type = hd44780<iface, address_selector>;

        hd44780() = delete;

        static void init() {
            iface::init();

            auto cfg = static_cast<uint8_t>(iface::bit_mode == 8 ? 0x10 : 0);
            cfg |= address_selector::rows == 1 ? 0x00 : 0x08;

            iface::send(cmd_function_set | cfg, 0);
            delay::template us<37>();

            display();
            clear();
        }

        static void clear() {
            iface::send(cmd_clear_display, 0);
            delay::ms(5);
        }

        static void home() {
            iface::send(cmd_cursor_home, 0);
            delay::ms(2);
        }

        static void display(bool on = true, bool cursor = false, bool blink = false) {
            uint8_t cmd = cmd_display_ctrl;
            if (on) {
                cmd |= 1 << 2;
            }

            if (cursor) {
                cmd |= 1 << 1;
            }

            if (blink) {
                cmd |= 1;
            }

            iface::send(cmd, 0);
            delay::template us<37>();
        }

        static void move(uint8_t row, uint8_t column) {
            uint8_t cmd = cmd_set_ddram_addr;
            cmd |= address_selector::get_address(row, column);
            iface::send(cmd, 0);
            delay::template us<37>();
        }

        static void ddram_addr(uint8_t address) {
            uint8_t cmd = cmd_set_ddram_addr + address;
            iface::send(cmd, 0);
            delay::template us<37>();
        }

        static void write_byte(uint8_t char_code) {
            iface::send(char_code, 1);
            delay::template us<37>();
        }

        static void write(const char *str) {
            while (*str) {
                iface::send(*str++, 1);
                delay::template us<37>();
            }
        }

        template<class F>
        static void write(::zoal::io::output_stream_functor<F> &fn) {
            uint8_t data = 0;
            while (static_cast<F &>(fn)(data)) {
                write_byte(data);
            }
        }

        static void create_char(uint8_t pos, const uint8_t *data) {
            uint8_t cmd = cmd_set_cgram_addr | pos << 3;
            iface::send(cmd, 0);
            delay::template us<37>();

            for (int i = 0; i < 8; i++) {
                write_byte(data[i]);
            }
        }

        static void entry_mode(bool increment = true, bool shift = false) {
            uint8_t cmd = cmd_entry_mode_set;
            if (increment) {
                cmd |= 2;
            }

            if (shift) {
                cmd |= 1;
            }
            iface::send(cmd, 0);
            delay::template us<37>();
        }
    };
}}

#endif
