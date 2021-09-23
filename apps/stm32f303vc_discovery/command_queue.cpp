#include "command_queue.hpp"

zoal::mem::reserve_mem<queue_type, 5, queue_type::item_type> command_queue;

command_msg::command_msg(app_cmd cmd)
    : command(cmd) {}

command_msg::command_msg()
    : command(app_cmd::none) {}
