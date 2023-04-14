#include "../../misc/terminal_input.hpp"
#include "command_machine.hpp"
#include "ramps.hpp"
#include "sdcard.h"
#include "serial.hpp"
#include "spi.h"

#include <avr/eeprom.h>
#include <zoal/fs/fat32.hpp>
#include <zoal/ic/hd44780.hpp>
#include <zoal/utils/scheduler.hpp>

FUSES = {.low = 0xFF, .high = 0xD7, .extended = 0xFC};

using ms_timer = typename mcu::timer_00;
using pwm_timer = typename mcu::timer_03;

lcd_type lcd;
encoder_type encoder;
enc_btn_type button;

char terminal_buffer[32];
auto terminal_greeting = "\033[0;32mroot@mcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));

using scheduler_type = zoal::utils::lambda_scheduler<uint32_t, 8>;
scheduler_type scheduler;

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;
    api::optimize<api::clock_on<usart, ms_timer>>();
    api::optimize<api::disable<usart, ms_timer>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pe_00, mcu::pe_01>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,

        mcu::cfg::timer<ms_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        mcu::irq::timer<ms_timer>::enable_overflow_interrupt,

        mcu::cfg::timer<pwm_timer, zoal::periph::timer_mode::up, 1, 1, 0xFF>::apply,

        api::mode<zoal::gpio::pin_mode::input_pull_up, pcb::ard_d31, pcb::ard_d33, pcb::ard_d35>,
        api::mode<zoal::gpio::pin_mode::output, pcb::ard_d37>,

        api::mode<zoal::gpio::pin_mode::input_pull_up, sd_detection>,

        beep::gpio_cfg,

        //        encoder_type::
        lcd_type::gpio_cfg
        //
        >();

    api::optimize<api::enable<usart, ms_timer>>();

    zoal::utils::interrupts::on();
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx.send_data(s, e - s);
}

static void command_callback(zoal::misc::command_machine *, zoal::misc::command cmd, int argc, zoal::misc::cmd_arg *argv) {
    using c = zoal::misc::command;
    switch (cmd) {
    case c::ls:
        break;
    case c::enc:
        stream << (int)(100 + encoder_type::pin_a::read() * 10 + encoder_type::pin_b::read()) << "\r\n";
        break;
    case c::clear:
        lcd.clear();
        break;
    case c::print:
        lcd.home();
        lcd.write(argv->start + 1, argv->end - 1);
        break;
    case c::print_at_row:
        lcd.move((int)argv[0], 0);
        lcd.write(argv[1].start + 1, argv[1].end - 1);
        break;
    case c::print_at_row_column:
        lcd.move((int)argv[0], (int)argv[1]);
        lcd.write(argv[2].start + 1, argv[2].end - 1);
        break;
    case c::pwm:
        lcd.home();
        lcd.write("PWM Pressed");
        break;
    default:
        break;
    }
}

static void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    stream << "\r\n";

    zoal::misc::command_machine cm;
    cm.callback(command_callback);
    cm.run_machine(s, e, e);
    terminal.sync();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

volatile uint8_t update_enc = 0;
int enc_counter = 0;
uint32_t last_enc_event = 0;

void encoder_callback(zoal::io::rotary_event d) {
    int m = 1;
    int dt = (int)(milliseconds - last_enc_event);

    if (dt < 10) {
        m = 10;
    } else if (dt < 100) {
        m = 5;
    }

    switch (d) {
    case zoal::io::rotary_event::direction_1:
        enc_counter += 1 * m;
        break;
    case zoal::io::rotary_event::direction_2:
        enc_counter -= 1 * m;
        break;
    }

    last_enc_event = milliseconds;

    if (!update_enc) {
        update_enc = 1;
        scheduler.schedule(0, 100, []() {
            stream << "counter: " << enc_counter << "\r\n";
            update_enc = 0;
        });
    }
}

void button_callback(zoal::io::button_event e) {
    if (e == zoal::io::button_event::press) {
        stream << "button_event::press: "
               << "\r\n";

        beep::on();
        scheduler.schedule(0, 50, []() { beep::off(); });
    }
}

void print_buffer(const uint8_t *buffer) {
    for (int i = 0; i < 512; i++) {
        if (i % 32 == 0) {
            stream << "\r\n";
        }

        auto v = buffer[i];
        auto h = v >> 4;
        stream << static_cast<char>(h < 10 ? ('0' + h) : ('A' + h - 10));

        h = v & 0xF;
        stream << static_cast<char>(h < 10 ? ('0' + h) : ('A' + h - 10));
        stream << " ";
    }

    stream << "\r\n";
}

uint8_t last_card_state = 1;
uint8_t test_started = 0;

zoal::fs::partition_table current_partition;

void print_mbr(uint8_t buf[512]);
void print_boot_sector(uint8_t buf[512]);
void print_dir(uint8_t buf[512]);

class SD_SPI_API {
public:
    static int read_block(uint32_t addr, void *buffer) {
        uint8_t res[5], token;
        res[0] = SD_readSingleBlock(addr, (uint8_t *)buffer, &token);
        if ((res[0] == 0x00) && (token == SD_START_TOKEN)) {
            return 0;
        } else {
            return 1;
        }
    }
};

zoal::fs::volume<SD_SPI_API> sd_volume;

void card_test() {
    uint8_t res[5], buf[512], token;
    uint32_t addr = 0x00000000;
    test_started = 0;

    SPI_init(SPI_MASTER | SPI_2X_FOSC_2 | SPI_MODE_0);

    if (SD_init() != SD_SUCCESS) {
        stream << "Error initializaing SD CARD\r\n";
    } else {
        stream << "SD Card initialized\r\n";
    }

    uint32_t pfc = 0;
    int result = sd_volume.read_partitions([&pfc](zoal::fs::partition_table &p) -> int {
        stream << "read_partitions callback\r\n";

        pfc = p.first_sector;
        return pfc == 0 ? 0 : 1;
    });

    if (result != 0) {
        stream << "read_partitions error: " << result << "\r\n";
    }

    if (pfc != 0) {
        result = sd_volume.mount_partition(pfc);
    } else {
        stream << "no pfc\r\n";
    }

    if (result != 0) {
        stream << "mount_partition error: " << result << "\r\n";
    } else {
        stream << "mount_partition ok\r\n";
    }

    result = sd_volume.enum_root([](zoal::fs::entry *e, const char *name)->int{
        stream << name;
        stream << "\ta: " << e->attributes;
        stream << "\tsize: " << e->file_size;
        stream << "\r\n";
        return 0;
    });

    if (result != 0) {
        stream << "enum_root error: " << result << "\r\n";
    }
}

void print_entry_name(zoal::fs::entry *e) {
    stream << "name: ";

    for (size_t i = 0; i < sizeof(e->name); i++) {
        if (e->name[i] == ' ') {
            continue;
        }

        if (i == 8) {
            stream << '.';
        }

        stream << (char)e->name[i];
    }

    stream << "\r\n";
}

char long_name[256];

void print_dir(uint8_t *buf) {
    auto entry = reinterpret_cast<zoal::fs::entry *>(buf);
    for (int i = 0; i < 512 / (int)sizeof(zoal::fs::entry); i++, entry++) {
        if (entry->is_free()) {
            break;
        }

        if (entry->is_delete()) {
            continue;
        }

        if (entry->is_long_name()) {
            auto lne = reinterpret_cast<zoal::fs::long_name_entry *>(entry);
            if (lne->is_last()) {
                memset(long_name, 0, sizeof(long_name));
                long_name[0] = '!';
                long_name[1] = '?';
            }

            lne->extract_long_name(long_name);
            continue;
        }

        print_entry_name(entry);

        auto cluster = ((uint32_t)entry->first_cluster_high << 16) | entry->first_cluster_low;

        stream << "attributes: " << entry->attributes << "\r\n";
        stream << "file_size: " << entry->file_size << "\r\n";
        stream << "cluster: " << cluster << "\r\n";
        stream << "\r\n";
    }
}

void print_boot_sector(uint8_t *buf) {
    auto vbr = reinterpret_cast<zoal::fs::volume_boot_sector *>(buf);
    stream << "OEM NAME: ";
    for (char ch : vbr->oem_name) {
        stream << ch;
    }
    stream << "\r\n";

    stream << "bytes_per_sector: " << vbr->bpb.bytes_per_sector << "\r\n";
    stream << "sectors_per_cluster: " << vbr->bpb.sectors_per_cluster << "\r\n";
    stream << "reserved_sector_count: " << vbr->bpb.reserved_sector_count << "\r\n";
    stream << "fat32_root_cluster: " << vbr->bpb.fat32_root_cluster << "\r\n";

    auto blocks_per_fat = vbr->bpb.sectors_per_fat32;
    auto fat_start_block = current_partition.first_sector + vbr->bpb.reserved_sector_count;
    auto first_data_block = fat_start_block + vbr->bpb.fat_count * blocks_per_fat;
    auto first_block_of_cluster = [&](uint32_t c) { return first_data_block + (c - 2) * vbr->bpb.sectors_per_cluster; };

    auto root_dir_start_block = first_block_of_cluster(vbr->bpb.fat32_root_cluster);

    stream << "blocks_per_fat: " << blocks_per_fat << "\r\n";
    stream << "fat_start_block: " << fat_start_block << "\r\n";
    stream << "root_dir_start_block: " << root_dir_start_block << "\r\n";

    root_dir_block = root_dir_start_block;
}

void print_mbr(uint8_t *buf) {
    auto mbr = reinterpret_cast<zoal::fs::master_boot_record *>(buf);

    current_partition = *mbr->partitions;

    for (int i = 0; i < 4; i++) {
        auto p = mbr->partitions[i];
        stream << "Partition: " << i << "\r\n";
        stream << "first_sector_1: " << p.first_sector << "\r\n";
        stream << "\r\n";
    }

    stream << "boot_signature_1: " << reinterpret_cast<void *>(mbr->boot_signature[0]) << "\r\n";
    stream << "boot_signature_2: " << reinterpret_cast<void *>(mbr->boot_signature[1]) << "\r\n";
}

void card_check() {
    auto v = sd_detection::read();
    if (last_card_state != v) {
        last_card_state = v;

        if (v) {
            stream << "Disconnected"
                   << "\r\n";
        } else {
            stream << "Connected"
                   << "\r\n";
            if (!test_started) {
                test_started = 1;
                scheduler.schedule(0, 200, card_test);
            }
        }
    }
}

int main() {
    using zoal::gpio::pin_mode;

    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();
    terminal.sync();

    lcd.init();
    lcd.clear();
    lcd.home();
    lcd.write("Hello QQQ");

    //    spi_check();

    stream << "entry size: " << sizeof(zoal::fs::entry) << "\r\n";
    stream << "long entry size: " << sizeof(zoal::fs::long_name_entry) << "\r\n";

    while (true) {
        uint8_t rx_byte = 0;
        bool result;
        {
            zoal::utils::interrupts_off scope_off;
            result = rx_buffer.pop_front(rx_byte);
        }

        if (result) {
            terminal.push_and_scan(&rx_byte, 1);
        }

        encoder.handle(encoder_callback);
        button.handle(milliseconds, button_callback);
        scheduler.handle(milliseconds);

        card_check();
    }
    return 0;
}

#pragma clang diagnostic pop
