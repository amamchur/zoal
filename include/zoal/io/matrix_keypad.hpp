#ifndef ZOAL_IO_MATRIX_KEYPAD_HPP
#define ZOAL_IO_MATRIX_KEYPAD_HPP

#include "../ct/check.hpp"
#include "../ct/helpers.hpp"
#include "../ct/type_list.hpp"
#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/pin_mode.hpp"
#include "../utils/helpers.hpp"
#include "../utils/method_invoker.hpp"
#include "../utils/nop.hpp"
#include "button_state_machine.hpp"

namespace zoal { namespace io {
    template<class... Rows>
    class keypad_row_selector {
    private:
        template<int No, class T, class... Rest>
        struct helper {
            using next = helper<No + 1, Rest...>;

            ZOAL_INLINE_IO static void to_ground(int index) {
                if (index == No) {
                    T::template mode<zoal::gpio::pin_mode::output_push_pull>();
                    T::low();
                } else {
                    next::to_ground(index);
                }
            }
        };

        template<int No, class T>
        struct helper<No, T> {
            ZOAL_INLINE_IO static void to_ground(int index) {
                if (index == No) {
                    T::template mode<zoal::gpio::pin_mode::output_push_pull>();
                    T::low();
                }
            }
        };
    public:
        static_assert(!zoal::ct::has_same<Rows...>::value, "Duplicated row pins");

        using high_impedance = typename zoal::gpio::api::optimize<zoal::gpio::api::mode<zoal::gpio::pin_mode::input_floating, Rows...>>;
        using pins = zoal::ct::type_list<Rows...>;

        static constexpr auto rows = pins::count;

        static void select_row(size_t row) {
            high_impedance();
            helper<0, Rows...>::to_ground(row);
        }
    };

    template<class... Columns>
    class keypad_column_reader {
    public:
        static_assert(!zoal::ct::has_same<Columns...>::value, "Duplicated column pins");

        using pins = zoal::ct::type_list<Columns...>;
        using cfg_gpio = zoal::gpio::api::optimize<zoal::gpio::api::mode<zoal::gpio::pin_mode::input_pull_up, Columns...>>;

        static constexpr auto columns = pins::count;

        class pin_value {
        public:
            explicit pin_value(size_t index)
                : index(index) {}

            template<class T>
            bool operator()(size_t idx) const {
                using namespace zoal::gpio;
                if (idx == index) {
                    value = T::read();
                }

                return idx == index;
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
        static constexpr uint16_t debounce_delay = 5;
        static constexpr uint16_t press_delay = 100;
        using machine_type = button_state_machine;
    };

    template<class TimeType, class Selector, class Reader, class Config = keypad_config>
    class matrix_keypad {
    public:
        static constexpr uint8_t rows = Selector::rows;
        static constexpr uint8_t columns = Reader::columns;

        using time_type = TimeType;
        using machine_type = typename Config::machine_type;

        template<class H, class C>
        void handle(H handler, time_type now, C capacity_delay) {
            constexpr auto debounce_delay = static_cast<TimeType>(Config::debounce_delay);
            constexpr auto press_delay = static_cast<TimeType>(Config::press_delay);

            typename Reader::cfg_gpio();

            for (size_t r = 0; r < rows; r++) {
                Selector::select_row(r);
                capacity_delay();

                for (size_t c = 0; c < columns; c++) {
                    auto &m = machines[r][c];
                    uint8_t v = 1 - Reader::read_column(c);
                    auto dt = now - prev_time[r][c];
                    auto switched = m.handle(v, dt, debounce_delay, press_delay);
                    if (switched) {
                        prev_time[r][c] = now;
                    }

                    m.invoke_callback(handler, r, c);
                }
            }
        }

        template<class H>
        void handle(H handler, TimeType now) {
            handle(handler, now, [](){
                asm volatile("nop \n");
            });
        }

        template<class T, class M>
        void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, size_t, size_t, button_event>(obj, m));
        }

    protected:
        time_type prev_time[Selector::rows][Reader::columns];
        machine_type machines[Selector::rows][Reader::columns];
    };
}}

#endif
