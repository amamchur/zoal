#ifndef ZOAL_STM32F103C8_TASK_OBJ_HPP
#define ZOAL_STM32F103C8_TASK_OBJ_HPP

#include "./types.hpp"
#include "stm32f1xx_hal.h"
#include "types.hpp"

#include <zoal/freertos/queue.hpp>
#include <zoal/mem/reserve_mem.hpp>

struct update_date_time_args {
    RTC_TimeTypeDef time;
};

struct command_msg {
    command_msg();
    explicit command_msg(app_cmd cmd);

    app_cmd command;
    union {
        update_date_time_args update_time{};
    };
};

using queue_type = zoal::freertos::queue<command_msg, zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<queue_type, 5, queue_type::item_type> command_queue;

#endif
