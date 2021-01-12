#ifndef ZOAL_STM32F103C8_TAKS_OBJ_HPP
#define ZOAL_STM32F103C8_TAKS_OBJ_HPP

#include "./types.hpp"
#include "types.hpp"

#include <zoal/freertos/queue.hpp>
#include <zoal/mem/reserve_mem.hpp>

using queue_type = zoal::freertos::queue<app_cmd, zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<queue_type, 5, queue_type::item_type> command_queue;

#endif
