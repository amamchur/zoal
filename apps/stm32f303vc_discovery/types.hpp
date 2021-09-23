#ifndef ZOAL_STM32F103C8_TYPES_HPP
#define ZOAL_STM32F103C8_TYPES_HPP

enum class app_cmd {
    none,
    led,
    ticks,
    button1_pressed,
    button2_pressed,
    print_heap_size,
    task_info,
    help,
    time_print,
    time_set,
    scan_i2c,
    read_eeprom
};

#endif
