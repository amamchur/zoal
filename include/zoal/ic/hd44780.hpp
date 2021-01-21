#ifndef ZOAL_IC_HD44780_HPP
#define ZOAL_IC_HD44780_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin_mode.hpp"
#include "../io/stream.hpp"

#include <stdint.h>

namespace zoal { namespace ic {
    using zoal::gpio::api;
    using zoal::gpio::pin_mode;

    template<class Delay, class RegisterSelect, class Enable, class DataBus4, class DataBus5, class DataBus6, class DataBus7>
    class hd44780_interface_4bit {
    public:
        using delay = Delay;

        static constexpr uint8_t bit_mode = 4;

        using gpio_cfg = api::optimize<
            //
            api::clock_on<
                //
                typename RegisterSelect::port,
                typename Enable::port,
                typename DataBus4::port,
                typename DataBus5::port,
                typename DataBus6::port,
                typename DataBus7::port>,
            api::mode<pin_mode::output_push_pull, RegisterSelect, Enable, DataBus4, DataBus5, DataBus6, DataBus7>,
            api::low<RegisterSelect, Enable, DataBus6, DataBus7>,
            api::high<DataBus4, DataBus4>>;

        static void init() {
            using namespace zoal::gpio;

            pulse_enable();
            delay::ms(5);

            pulse_enable();
            delay::template us<100>();

            typename DataBus4::low();
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
            typename Enable::low();
            delay::template us<1>();
            typename Enable::high();
            delay::template us<1>();
            typename Enable::low();
        }
    };

    template<class Delay,
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
        using delay = Delay;
        using gpio_cfg = api::optimize<
            //
            api::clock_on<
                //
                typename RegisterSelect::port,
                typename Enable::port,
                typename DataBus0::port,
                typename DataBus1::port,
                typename DataBus2::port,
                typename DataBus3::port,
                typename DataBus4::port,
                typename DataBus5::port,
                typename DataBus6::port,
                typename DataBus7::port>,
            api::mode<pin_mode::output_push_pull, RegisterSelect, Enable, DataBus0, DataBus1, DataBus2, DataBus3, DataBus4, DataBus5, DataBus6, DataBus7>,
            api::low<RegisterSelect, Enable, DataBus6, DataBus7>,
            api::high<DataBus4, DataBus4>>;

        static constexpr uint8_t bit_mode = 8;

        static void init() {
            pulse_enable();
            delay::template ms<5>();
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

    struct hd44780_move_cursor_functor : zoal::io::transport_functor<hd44780_move_cursor_functor> {
        hd44780_move_cursor_functor(uint8_t r, uint8_t c)
            : row(r)
            , column(c) {}

        template<class L>
        inline void apply(L &lcd) {
            lcd.move(row, column);
        }

        uint8_t row;
        uint8_t column;
    };

    struct hd44780_clear_functor : zoal::io::transport_functor<hd44780_clear_functor> {
        template<class L>
        inline void apply(L &lcd) {
            lcd.clear();
        }
    };

    template<class Interface, class AddressSelector>
    class hd44780 {
    public:
        using iface = Interface;
        using address_selector = AddressSelector;
        using delay = typename iface::delay;
        using gpio_cfg = typename iface::gpio_cfg;
        using mc = hd44780_move_cursor_functor;
        using cls = hd44780_clear_functor;

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

        void init() {
            iface::init();

            auto cfg = static_cast<uint8_t>(iface::bit_mode == 8 ? 0x10 : 0);
            cfg |= address_selector::rows == 1 ? 0x00 : 0x08;

            iface::send(cmd_function_set | cfg, 0);
            delay::template us<37>();

            display();
            clear();
        }

        void clear() {
            iface::send(cmd_clear_display, 0);
            delay::ms(5);
        }

        void home() {
            iface::send(cmd_cursor_home, 0);
            delay::ms(2);
        }

        void display(bool on = true, bool cursor = false, bool blink = false) {
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

        void move(uint8_t row, uint8_t column) {
            uint8_t cmd = cmd_set_ddram_addr;
            cmd |= address_selector::get_address(row, column);
            iface::send(cmd, 0);
            delay::template us<37>();
        }

        void ddram_addr(uint8_t address) {
            uint8_t cmd = cmd_set_ddram_addr + address;
            iface::send(cmd, 0);
            delay::template us<37>();
        }

        void send_byte(uint8_t value) {
            iface::send(value, 1);
            delay::template us<37>();
        }

        void send_data(const void *data, size_t size) {
            auto ptr = reinterpret_cast<const uint8_t *>(data);
            while (size > 0) {
                send_byte(*ptr);
                ptr++;
                size--;
            }
        }

        template<class F>
        inline void apply_functor(::zoal::io::transport_functor<F> &fn) {
            static_cast<F &>(fn).apply(*this);
        }

        void write(const char *str) {
            while (*str) {
                iface::send(*str++, 1);
                delay::template us<37>();
            }
        }

        void create_char(uint8_t pos, const uint8_t *data) {
            uint8_t cmd = cmd_set_cgram_addr | pos << 3;
            iface::send(cmd, 0);
            delay::template us<37>();

            for (int i = 0; i < 8; i++) {
                send_byte(data[i]);
            }
        }

        void entry_mode(bool increment = true, bool shift = false) {
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
