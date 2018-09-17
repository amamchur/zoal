#ifndef ZOAL_IO_MATRIX_KEYPAD_HPP
#define ZOAL_IO_MATRIX_KEYPAD_HPP

#include "../ct/helpers.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/pin_mode.hpp"
#include "../utils/helpers.hpp"
#include "../utils/method_invoker.hpp"
#include "button_state_machine.hpp"

#include <string.h>

namespace zoal { namespace io {
    template<class r00,
             class r01 = ::zoal::gpio::null_pin,
             class r02 = ::zoal::gpio::null_pin,
             class r03 = ::zoal::gpio::null_pin,
             class r04 = ::zoal::gpio::null_pin,
             class r05 = ::zoal::gpio::null_pin,
             class r06 = ::zoal::gpio::null_pin,
             class r07 = ::zoal::gpio::null_pin,
             class r08 = ::zoal::gpio::null_pin,
             class r09 = ::zoal::gpio::null_pin,
             class r10 = ::zoal::gpio::null_pin,
             class r11 = ::zoal::gpio::null_pin,
             class r12 = ::zoal::gpio::null_pin,
             class r13 = ::zoal::gpio::null_pin,
             class r14 = ::zoal::gpio::null_pin,
             class r15 = ::zoal::gpio::null_pin>
    class keypad_row_selector {
    public:
        static constexpr auto rows = zoal::ct::
            pin_count<r00, r01, r02, r03, r04, r05, r06, r07, r08, r09, r10, r11, r12, r13, r14, r15>::value;

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
                               r00,
                               r01,
                               r02,
                               r03,
                               r04,
                               r05,
                               r06,
                               r07,
                               r08,
                               r09,
                               r10,
                               r11,
                               r12,
                               r13,
                               r14,
                               r15>::apply();

            switch (row) {
            case 0x0:
                to_ground<r00>();
                break;
            case 0x1:
                to_ground<r01>();
                break;
            case 0x2:
                to_ground<r02>();
                break;
            case 0x3:
                to_ground<r03>();
                break;
            case 0x4:
                to_ground<r04>();
                break;
            case 0x5:
                to_ground<r05>();
                break;
            case 0x6:
                to_ground<r06>();
                break;
            case 0x7:
                to_ground<r07>();
                break;
            case 0x8:
                to_ground<r08>();
                break;
            case 0x9:
                to_ground<r09>();
                break;
            case 0xA:
                to_ground<r10>();
                break;
            case 0xB:
                to_ground<r11>();
                break;
            case 0xC:
                to_ground<r12>();
                break;
            case 0xD:
                to_ground<r13>();
                break;
            case 0xE:
                to_ground<r14>();
                break;
            default:
                to_ground<r15>();
                break;
            }
        }
    };

    template<class c00,
             class c01 = ::zoal::gpio::null_pin,
             class c02 = ::zoal::gpio::null_pin,
             class c03 = ::zoal::gpio::null_pin,
             class c04 = ::zoal::gpio::null_pin,
             class c05 = ::zoal::gpio::null_pin,
             class c06 = ::zoal::gpio::null_pin,
             class c07 = ::zoal::gpio::null_pin,
             class c08 = ::zoal::gpio::null_pin,
             class c09 = ::zoal::gpio::null_pin,
             class c10 = ::zoal::gpio::null_pin,
             class c11 = ::zoal::gpio::null_pin,
             class c12 = ::zoal::gpio::null_pin,
             class c13 = ::zoal::gpio::null_pin,
             class c14 = ::zoal::gpio::null_pin,
             class c15 = ::zoal::gpio::null_pin>
    class keypad_column_reader {
    public:
        static constexpr auto columns = zoal::ct::
            pin_count<c00, c01, c02, c03, c04, c05, c06, c07, c08, c09, c10, c11, c12, c13, c14, c15>::value;

        template<class api>
        static void begin() {
            using namespace zoal::gpio;

            api::template mode<pin_mode::input_pull_up,
                               c00,
                               c01,
                               c02,
                               c03,
                               c04,
                               c05,
                               c06,
                               c07,
                               c08,
                               c09,
                               c10,
                               c11,
                               c12,
                               c13,
                               c14,
                               c15>::apply();
        }

        static uint8_t read_column(uint8_t column) {
            switch (column) {
            case 0x0:
                return c00::read();
            case 0x1:
                return c01::read();
            case 0x2:
                return c02::read();
            case 0x3:
                return c03::read();
            case 0x4:
                return c04::read();
            case 0x5:
                return c05::read();
            case 0x6:
                return c06::read();
            case 0x7:
                return c07::read();
            case 0x8:
                return c08::read();
            case 0x9:
                return c09::read();
            case 0xA:
                return c10::read();
            case 0xB:
                return c11::read();
            case 0xC:
                return c12::read();
            case 0xD:
                return c13::read();
            case 0xE:
                return c14::read();
            default:
                return c15::read();
            }
        }
    };

    class keypad_config {
    public:
        static constexpr uint16_t read_delay_us = 4;
        static constexpr uint16_t debounce_delay_ms = 50;
        static constexpr uint16_t press_delay_ms = 300;
    };

    template<class Tools,
             class RowSelector,
             class ColumnReader,
             class Config = keypad_config,
             class Machine = button_state_machine>
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

            for (uint8_t i = 0; i < rows; i++) {
                RowSelector::template select_row<api>(i);
                delay::template us<Config::read_delay_us>();

                for (uint8_t j = 0; j < columns; j++) {
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

    template<class Tools, class RowSelector, class ColumnReader, class Config, class Machine>
    typename matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::counter_type
        matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::prevTime = 0;

    template<class tools, class row_selector, class column_reader, class config, class machine>
    uint8_t matrix_keypad<tools, row_selector, column_reader, config, machine>::states[row_selector::rows]
                                                                                      [column_reader::columns];
}}

#endif
