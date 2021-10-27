#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"

uint8_t spi_buffer[512];

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        command_msg msg;
        auto result = command_queue.pop(msg);
        if (!result || msg.command == app_cmd::none) {
            continue;
        }

        scoped_lock lock(tx_stream_mutex);
        tx_stream << "\033[2K\r";

        switch (msg.command) {
        case app_cmd::ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd::w25q_write:
            for (int i = 0; i < 256; i++) {
                spi_buffer[i] = i;
            }
            w25q32::page_program(msg.int_values[0], spi_buffer, 256);
            break;
        case app_cmd::w25q_read:
            memset(spi_buffer, 0, sizeof(spi_buffer));
            w25q32::fast_read(msg.int_values[0], spi_buffer, sizeof(spi_buffer));
            for (int i = 0; i < sizeof(spi_buffer); ++i) {
                if ((i & 0xf) == 0) {
                    tx_stream << "\r\n";
                }
                tx_stream << zoal::io::hexadecimal(spi_buffer[i]) << " ";
            }
            tx_stream << "\r\n";
            break;
        case app_cmd::w25q_erase_chip:
            w25q32::chip_erase();
            break;
        case app_cmd::w25q_erase_sector:
            w25q32::sector_sector(msg.int_values[0]);
            break;
        case app_cmd::help:
            tx_stream << help_message;
            break;
        default:
            break;
        }
        terminal.sync();
    }
}
