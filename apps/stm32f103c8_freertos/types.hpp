#ifndef ZOAL_STM32F103C8_TYPES_HPP
#define ZOAL_STM32F103C8_TYPES_HPP

enum app_cmd {
    app_cmd_none,
    app_cmd_led_on,
    app_cmd_led_off,
    app_cmd_ticks,
    app_cmd_button1_pressed,
    app_cmd_button2_pressed,
    app_cmd_print_heap_size,
    app_cmd_task_info,
    app_cmd_help,
    app_cmd_not_found
};

#endif
