#ifndef ZOAL_IO_MATRIX_KEYPAD_HPP
#define ZOAL_IO_MATRIX_KEYPAD_HPP

#include "../ct/helpers.hpp"
#include "../ct/type_list.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/pin_mode.hpp"
#include "../utils/helpers.hpp"
#include "../utils/method_invoker.hpp"
#include "button_state_machine.hpp"

#include <string.h>

namespace zoal { namespace io {
    template<class Tools, class... Rows>
    class keypad_row_selector {
    public:
        using tools = Tools;
        using api = typename tools::api;
        using high_impedance = typename api::template mode<zoal::gpio::pin_mode::input_floating, Rows...>;
        using pins = zoal::ct::type_list<Rows...>;

        static constexpr auto rows = pins::count;

        class to_ground {
        public:
            explicit to_ground(size_t index)
                : index(index) {}

            template<class T>
            void operator()(size_t idx) const {
                using namespace zoal::gpio;
                if (idx == index) {
                    T::template mode<pin_mode::output_push_pull>();
                    T::low();
                }
            }

        private:
            size_t index;
        };

        static void select_row(size_t row) {
            high_impedance();
            zoal::ct::type_list_index_iterator<pins>::for_each(to_ground(row));
        }
    };

    template<class Tools, class... Columns>
    class keypad_column_reader {
    public:
        using tools = Tools;
        using api = typename tools::api;

        using pins = zoal::ct::type_list<Columns...>;
        using gpio_cfg = typename api::template mode<zoal::gpio::pin_mode::input_pull_up, Columns...>;

        static constexpr auto columns = pins::count;

        class pin_value {
        public:
            explicit pin_value(size_t index)
                : index(index) {}

            template<class T>
            void operator()(size_t idx) const {
                using namespace zoal::gpio;
                if (idx == index) {
                    value = T::read();
                }
            }

            mutable uint8_t value{0};
            size_t index;
        };

        static uint8_t read_column(size_t column) {
            pin_value pv(column);
            zoal::ct::type_list_index_iterator<pins>::for_each(pv);
            return pv.value;
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
        using gpio_cfg = typename ColumnReader::gpio_cfg;

        static constexpr uint8_t rows = RowSelector::rows;
        static constexpr uint8_t columns = ColumnReader::columns;

        matrix_keypad() = delete;

        static void init() {
            memset(states, 0, sizeof(states));
        }

        template<class H>
        static void handle(H handler) {
            Machine machine(Config::debounce_delay_ms, Config::press_delay_ms);
            counter_type now = counter::now();
            counter_type dt = now - prevTime;
            uint8_t allEvents = 0;

            for (size_t r = 0; r < rows; r++) {
                RowSelector::select_row(r);
                delay::template us<Config::read_delay_us>();

                for (size_t c = 0; c < columns; c++) {
                    uint8_t v = 1 - ColumnReader::read_column(c);
                    uint8_t state = machine.handle_button(dt, states[r][c], v);

                    uint8_t events = state & button_state_trigger;
                    allEvents |= events;

                    if (events) {
                        if ((events & button_state_trigger_down) != 0) {
                            handler(r, c, button_event::down);
                        }

                        if ((events & button_state_trigger_press) != 0) {
                            handler(r, c, button_event::press);
                        }

                        if ((events & button_state_trigger_up) != 0) {
                            handler(r, c, button_event::up);
                        }
                    }

                    states[r][c] = state & ~button_state_trigger;
                }
            }

            if (allEvents != 0) {
                prevTime = now;
            }
        }

        template<class T, class M>
        static void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, size_t, size_t, button_event>(obj, m));
        }

    protected:
        static counter_type prevTime;
        static uint8_t states[RowSelector::rows][ColumnReader::columns];
    };

    template<class Tools, class RowSelector, class ColumnReader, class Config, class Machine>
    typename matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::counter_type
        matrix_keypad<Tools, RowSelector, ColumnReader, Config, Machine>::prevTime = 0;

    template<class tools, class RowSelector, class ColumnReader, class Config, class Machine>
    uint8_t matrix_keypad<tools, RowSelector, ColumnReader, Config, Machine>::states[RowSelector::rows]
                                                                                    [ColumnReader::columns];
}}

#endif
