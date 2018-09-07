#ifndef ZOAL_IO_MATRIX_KEYPAD_HPP
#define ZOAL_IO_MATRIX_KEYPAD_HPP

#include <string.h>
#include "button_state_machine.hpp"
#include "../gpio/pin_mode.hpp"
#include "../gpio/pin.hpp"
#include "../utils/method_invoker.hpp"
#include "../utils/helpers.hpp"

namespace zoal { namespace io {
    template<
            class R00,
            class R01 = ::zoal::gpio::null_pin,
            class R02 = ::zoal::gpio::null_pin,
            class R03 = ::zoal::gpio::null_pin,
            class R04 = ::zoal::gpio::null_pin,
            class R05 = ::zoal::gpio::null_pin,
            class R06 = ::zoal::gpio::null_pin,
            class R07 = ::zoal::gpio::null_pin,
            class R08 = ::zoal::gpio::null_pin,
            class R09 = ::zoal::gpio::null_pin,
            class R10 = ::zoal::gpio::null_pin,
            class R11 = ::zoal::gpio::null_pin,
            class R12 = ::zoal::gpio::null_pin,
            class R13 = ::zoal::gpio::null_pin,
            class R14 = ::zoal::gpio::null_pin,
            class R15 = ::zoal::gpio::null_pin
    >
    class keypad_row_selector {
    public:
        static constexpr auto rows = zoal::utils::pin_count<
                R00,
                R01,
                R02,
                R03,
                R04,
                R05,
                R06,
                R07,
                R08,
                R09,
                R10,
                R11,
                R12,
                R13,
                R14,
                R15>::value;

        template<class RowPin>
        static inline void to_ground() {
            using namespace zoal::gpio;
            RowPin::template mode<pin_mode::output_push_pull>();
            RowPin::low();
        }

        template<class API>
        static void select_row(uint8_t row) {
            using namespace zoal::gpio;

            // Configure all pins to high impedance state
            API::template mode<pin_mode::input_floating,
                    R00, R01, R02, R03,
                    R04, R05, R06, R07,
                    R08, R09, R10, R11,
                    R12, R13, R14, R15
            >::apply();

            switch (row) {
                case 0x0:
                    to_ground<R00>();
                    break;
                case 0x1:
                    to_ground<R01>();
                    break;
                case 0x2:
                    to_ground<R02>();
                    break;
                case 0x3:
                    to_ground<R03>();
                    break;
                case 0x4:
                    to_ground<R04>();
                    break;
                case 0x5:
                    to_ground<R05>();
                    break;
                case 0x6:
                    to_ground<R06>();
                    break;
                case 0x7:
                    to_ground<R07>();
                    break;
                case 0x8:
                    to_ground<R08>();
                    break;
                case 0x9:
                    to_ground<R09>();
                    break;
                case 0xA:
                    to_ground<R10>();
                    break;
                case 0xB:
                    to_ground<R11>();
                    break;
                case 0xC:
                    to_ground<R12>();
                    break;
                case 0xD:
                    to_ground<R13>();
                    break;
                case 0xE:
                    to_ground<R14>();
                    break;
                default:
                    to_ground<R15>();
                    break;
            }
        }
    };

    template<
            class C00,
            class C01 = ::zoal::gpio::null_pin,
            class C02 = ::zoal::gpio::null_pin,
            class C03 = ::zoal::gpio::null_pin,
            class C04 = ::zoal::gpio::null_pin,
            class C05 = ::zoal::gpio::null_pin,
            class C06 = ::zoal::gpio::null_pin,
            class C07 = ::zoal::gpio::null_pin,
            class C08 = ::zoal::gpio::null_pin,
            class C09 = ::zoal::gpio::null_pin,
            class C10 = ::zoal::gpio::null_pin,
            class C11 = ::zoal::gpio::null_pin,
            class C12 = ::zoal::gpio::null_pin,
            class C13 = ::zoal::gpio::null_pin,
            class C14 = ::zoal::gpio::null_pin,
            class C15 = ::zoal::gpio::null_pin
    >
    class keypad_column_reader {
    public:
        static constexpr auto columns = zoal::utils::pin_count<
                C00,
                C01,
                C02,
                C03,
                C04,
                C05,
                C06,
                C07,
                C08,
                C09,
                C10,
                C11,
                C12,
                C13,
                C14,
                C15>::value;

        template<class api>
        static void begin() {
            using namespace zoal::gpio;

            api::template mode<pin_mode::input_pull_up,
                    C00, C01, C02, C03,
                    C04, C05, C06, C07,
                    C08, C09, C10, C11,
                    C12, C13, C14, C15
            >::apply();
        }

        static uint8_t read_column(uint8_t column) {
            switch (column) {
                case 0x0:
                    return C00::read();
                case 0x1:
                    return C01::read();
                case 0x2:
                    return C02::read();
                case 0x3:
                    return C03::read();
                case 0x4:
                    return C04::read();
                case 0x5:
                    return C05::read();
                case 0x6:
                    return C06::read();
                case 0x7:
                    return C07::read();
                case 0x8:
                    return C08::read();
                case 0x9:
                    return C09::read();
                case 0xA:
                    return C10::read();
                case 0xB:
                    return C11::read();
                case 0xC:
                    return C12::read();
                case 0xD:
                    return C13::read();
                case 0xE:
                    return C14::read();
                default:
                    return C15::read();
            }
        }
    };

    class keypad_config {
    public:
        static constexpr uint16_t read_delay_us = 4;
        static constexpr uint16_t debounce_delay_ms = 50;
        static constexpr uint16_t press_delay_ms = 300;
    };

    template<
            class Tools,
            class RowSelector,
            class ColumnReader,
            class Config = keypad_config,
            class Machine = button_state_machine
    >
    class matrix_keypad {
    public:
        using api = typename Tools::api;
        using delay = typename Tools::delay;
        using counter = typename Tools::counter;
        using counter_type = typename counter::value_type;

        static constexpr uint8_t rows = RowSelector::rows;
        static constexpr uint8_t columns = ColumnReader::columns;

        matrix_keypad() = delete;

        static void begin() {
            ColumnReader::template begin<api>();
            memset(states, 0, sizeof(states));
        }

        template<class H>
        static void handle(H handler) {
            Machine machine(Config::debounce_delay_ms, Config::press_delay_ms);
            counter_type now = counter::now();
            counter_type dt = now - prevTime;
            uint8_t allEvents = 0;

            for (uint8_t i = 0; i < RowSelector::rows; i++) {
                RowSelector::template select_row<api>(i);
                delay::template us<Config::read_delay_us>();

                for (uint8_t j = 0; j < ColumnReader::columns; j++) {
                    uint8_t v = 1 - ColumnReader::read_column(j);
                    uint8_t state = machine.handle_button(dt, states[i][j], v);

                    uint8_t events = state & button_state_trigger;
                    allEvents |= events;

                    if (events) {
                        uint8_t button = i << 4 | j;
                        if ((events & button_state_trigger_down) != 0) {
                            handler(button, button_event::down);
                        }

                        if ((events & button_state_trigger_press) != 0) {
                            handler(button, button_event::press);
                        }

                        if ((events & button_state_trigger_up) != 0) {
                            handler(button, button_event::up);
                        }
                    }

                    states[i][j] = state & ~button_state_trigger;
                }
            }

            if (allEvents != 0) {
                prevTime = now;
            }
        }

        template<class T, class M>
        static void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, uint8_t, button_event>(obj, m));
        }

    protected:
        static counter_type prevTime;
        static uint8_t states[RowSelector::rows][ColumnReader::columns];
    };

    template<
            class Tools,
            class RowSelector,
            class ColumnReader,
            class Config,
            class Machine
    >
    typename matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::counter_type matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::prevTime = 0;

    template<
            class tools,
            class row_selector,
            class column_reader,
            class config,
            class machine
    >
    uint8_t matrix_keypad<tools, row_selector, column_reader, config, machine>::states[row_selector::rows][column_reader::columns];
}}

#endif
