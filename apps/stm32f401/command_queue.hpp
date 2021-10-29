#ifndef ZOAL_STM32F401_TASK_OBJ_HPP
#define ZOAL_STM32F401_TASK_OBJ_HPP

#include "stm32f4xx_hal.h"

#include <zoal/data/date_time.hpp>
#include <zoal/freertos/queue.hpp>
#include <zoal/mem/reserve_mem.hpp>

enum class app_cmd {
    none,
    adc,
    help,
    i2c,
    pwm,
    pwm_off,
    pwm_on,
    read_eeprom,
    ticks,
    time_print,
    time_set,
    timer,
    w25q_erase_chip,
    w25q_erase_sector,
    w25q_read,
    w25q_write
};

struct command_msg {
    command_msg();
    explicit command_msg(app_cmd cmd);

    app_cmd command;
    union {
        char str_value[configMAX_TASK_NAME_LEN]{0};
        int int_values[4];
        zoal::data::date_time date_time_value;
    };
};

using queue_type = zoal::freertos::queue<command_msg, zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<queue_type, 5, queue_type::item_type> command_queue;

#endif
