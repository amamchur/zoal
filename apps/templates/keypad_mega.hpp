#ifndef APPS_KEYPAD_APP_HPP
#define APPS_KEYPAD_APP_HPP

#include <zoal/io/matrix_keypad.hpp>
#include <zoal/board/arduino_mega.hpp>

class keypad_config {
public:
    static constexpr uint16_t read_delay_us = 4;
    static constexpr uint16_t debounce_delay_ms = 5;
    static constexpr uint16_t press_delay_ms = 300;
};

enum class keypad_type {
    keypad_5x4,
    keypad_4x4,
    keypad_4x3,
    keypad_1x4
};

template<keypad_type type>
struct keypad_layout {
};

template<>
struct keypad_layout<keypad_type::keypad_5x4> {
    static char map[5][4];
};

char keypad_layout<keypad_type::keypad_5x4>::map[5][4] = {
        {'f', 'F', '#', '*'},
        {'1', '2', '3', '^'},
        {'4', '5', '6', 'v'},
        {'7', '8', '9', 'e'},
        {'<', '0', '>', 'r'}
};

template<>
struct keypad_layout<keypad_type::keypad_4x4> {
    static char map[4][4];
};

char keypad_layout<keypad_type::keypad_4x4>::map[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

template<>
struct keypad_layout<keypad_type::keypad_4x3> {
    static char map[4][3];
};

char keypad_layout<keypad_type::keypad_4x3>::map[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
};

template<>
struct keypad_layout<keypad_type::keypad_1x4> {
    static char map[1][4];
};

char keypad_layout<keypad_type::keypad_1x4>::map[1][4] = {
        {'2', '1', '4', '3'},
};


template<class Tools, keypad_type type>
struct keypad_builder {
};

template<class Tools>
struct keypad_builder<Tools, keypad_type::keypad_5x4> {
    typedef zoal::io::keypad_row_selector<Tools,
            zoal::pcb::ard_d35,
            zoal::pcb::ard_d37,
            zoal::pcb::ard_d39,
            zoal::pcb::ard_d41,
            zoal::pcb::ard_d43> row_selector;
    typedef zoal::io::keypad_column_reader<Tools,
            zoal::pcb::ard_d51,
            zoal::pcb::ard_d49,
            zoal::pcb::ard_d47,
            zoal::pcb::ard_d45> column_reader;
};

template<class Tools>
struct keypad_builder<Tools, keypad_type::keypad_4x4> {
    typedef zoal::io::keypad_row_selector<Tools,
            zoal::pcb::ard_d51,
            zoal::pcb::ard_d49,
            zoal::pcb::ard_d47,
            zoal::pcb::ard_d45> row_selector;
    typedef zoal::io::keypad_column_reader<Tools,
            zoal::pcb::ard_d43,
            zoal::pcb::ard_d41,
            zoal::pcb::ard_d39,
            zoal::pcb::ard_d37> column_reader;
};

template<class Tools>
struct keypad_builder<Tools, keypad_type::keypad_4x3> {
    typedef zoal::io::keypad_row_selector<Tools,
            zoal::pcb::ard_d51,
            zoal::pcb::ard_d49,
            zoal::pcb::ard_d47,
            zoal::pcb::ard_d45> row_selector;
    typedef zoal::io::keypad_column_reader<Tools,
            zoal::pcb::ard_d43,
            zoal::pcb::ard_d41,
            zoal::pcb::ard_d39> column_reader;
};

template<class Tools>
struct keypad_builder<Tools, keypad_type::keypad_1x4> {
    typedef zoal::io::keypad_row_selector<Tools, zoal::pcb::ard_d33> row_selector;
    typedef zoal::io::keypad_column_reader<Tools,
            zoal::pcb::ard_d31,
            zoal::pcb::ard_d29,
            zoal::pcb::ard_d27,
            zoal::pcb::ard_d25> column_reader;
};

template<class Tools, keypad_type Type>
class keypad {
public:
    using self_type = keypad<Tools, Type>;
    using builder = keypad_builder<Tools, Type>;
    using layout = keypad_layout<Type>;
    using row_selector = typename builder::row_selector;
    using column_reader = typename builder::column_reader;
    using logger = typename Tools::logger;
    using counter = typename Tools::counter;
    using matrix_keypad = zoal::io::matrix_keypad<Tools, row_selector, column_reader, keypad_config>;

    void init() {
    }

    void keypadHandler(size_t row, size_t column, zoal::io::button_event e) {
        using namespace zoal::io;

        if (e != button_event::press) {
            return;
        }

        auto time = counter::now();
        logger::info()
                << time
                << " "
                << layout::map[row][column];
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    void run_once() {
//        matrix_keypad::handle(this, &self_type::keypadHandler);
    }

    void run() {
        while (true) {
            run_once();
        }
    }

#pragma clang diagnostic pop
};

#endif
