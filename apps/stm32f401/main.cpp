#include "command_processor.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "terminal.hpp"
#include "usart.h"
#include "w25qxx.h"
#include "tim.h"
#include "adc.h"

#include <cstring>
#include <zoal/data/ring_buffer.hpp>
#include <zoal/freertos/task.hpp>

using mcu = zoal::mcu::stm32f401ccux;
using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::cmsis_os2::delay<84000000>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
using api = zoal::gpio::api;
using spi = mcu::spi_01;

[[noreturn]] void zoal_main_task(void *);

__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

void SPI1_Send(uint8_t *dt, uint16_t cnt) {
    HAL_SPI_Transmit(&hspi1, dt, cnt, 5000);
}
//-------------------------------------------------------------
void SPI1_Recv(uint8_t *dt, uint16_t cnt) {
    HAL_SPI_Receive(&hspi1, dt, cnt, 5000);
}

#define cs_set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define cs_reset() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define W25_ENABLE_RESET 0x66
#define W25_RESET 0x99
#define W25_SECTOR_ERASE 0x20
#define W25_PROGRAM_PAGE 0x02
#define W25_READ 0x03
#define W25_GET_JEDEC_ID 0x9f

typedef struct {
    uint16_t PageSize;
    uint32_t PageCount;
    uint32_t SectorSize;
    uint32_t SectorCount;
    uint32_t BlockSize;
    uint32_t BlockCount;
    uint32_t NumKB;
    uint8_t SR1;
    uint8_t SR2;
    uint8_t SR3;
} w25_info_t;
w25_info_t w25_info;
char str1[30];
uint8_t rx_buf[1024];
uint8_t tx_buf[1024];

void W25_Reset() {
    cs_set();
    tx_buf[0] = W25_ENABLE_RESET;
    tx_buf[1] = W25_RESET;
    SPI1_Send(tx_buf, 2);
    cs_reset();
}

void W25_Read_Data(uint32_t addr, uint8_t *data, uint32_t sz) {
    cs_set();
    tx_buf[0] = W25_READ;
    tx_buf[1] = (addr >> 16) & 0xFF;
    tx_buf[2] = (addr >> 8) & 0xFF;
    tx_buf[3] = addr & 0xFF;
    SPI1_Send(tx_buf, 4);
    SPI1_Recv(data, sz);
    cs_reset();
}

uint32_t W25_Read_ID(void) {
    uint8_t dt[4];
    tx_buf[0] = W25_GET_JEDEC_ID;
    cs_set();
    SPI1_Send(tx_buf, 1);
    SPI1_Recv(dt, 3);
    cs_reset();
    return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}

void W25_Ini() {
    HAL_Delay(100);
    W25_Reset();
    HAL_Delay(100);
    unsigned int id = W25_Read_ID();
    HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 2, 0x1000);

    tx_stream << "ID\t\t: " << id << "\r\n";

    HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), 0x1000);
    id &= 0x0000ffff;
    switch (id) {
    case 0x401A:
        w25_info.BlockCount = 1024;
        tx_stream << "w25qxx Chip\t: w25q512"
                  << "\r\n";
        break;
    case 0x4019:
        w25_info.BlockCount = 512;
        tx_stream << "w25qxx Chip\t: w25q256"
                  << "\r\n";
        break;
    case 0x4018:
        w25_info.BlockCount = 256;
        tx_stream << "w25qxx Chip\t: w25q128"
                  << "\r\n";
        break;
    case 0x4017:
        w25_info.BlockCount = 128;
        tx_stream << "w25qxx Chip\t: w25q64"
                  << "\r\n";
        break;
    case 0x4016:
        w25_info.BlockCount = 64;
        tx_stream << "w25qxx Chip\t: w25q32"
                  << "\r\n";
        break;
    case 0x4015:
        w25_info.BlockCount = 32;
        tx_stream << "w25qxx Chip\t: w25q16"
                  << "\r\n";
        break;
    case 0x4014:
        w25_info.BlockCount = 16;
        tx_stream << "w25qxx Chip\t: w25q80"
                  << "\r\n";
        break;
    case 0x4013:
        w25_info.BlockCount = 8;
        tx_stream << "w25qxx Chip: w25q40"
                  << "\r\n";
        break;
    case 0x4012:
        w25_info.BlockCount = 4;
        tx_stream << "w25qxx Chip\t: w25q20"
                  << "\r\n";
        break;
    case 0x4011:
        w25_info.BlockCount = 2;
        tx_stream << "w25qxx Chip\t: w25q10"
                  << "\r\n";
        break;
    default:
        tx_stream << "w25qxx Unknown ID"
                  << "\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), 0x1000);
        return;
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), 0x1000);
    w25_info.PageSize = 256;
    w25_info.SectorSize = 0x1000;
    w25_info.SectorCount = w25_info.BlockCount * 16;
    w25_info.PageCount = (w25_info.SectorCount * w25_info.SectorSize) / w25_info.PageSize;
    w25_info.BlockSize = w25_info.SectorSize * 16;
    w25_info.NumKB = (w25_info.SectorCount * w25_info.SectorSize) / 1024;
    tx_stream << "Page Size\t: " << w25_info.PageSize << "\r\n";
    tx_stream << "Page Count\t: " << w25_info.PageCount << "\r\n";
    tx_stream << "Sector Size\t: " << w25_info.SectorSize << " Bytes"
              << "\r\n";
    tx_stream << "Sector Count\t: " << w25_info.SectorCount << "\r\n";
    tx_stream << "Block Size\t: " << w25_info.BlockSize << " Bytes"
              << "\r\n";
    tx_stream << "Block Count\t: " << w25_info.BlockCount << "\r\n";
    tx_stream << "Capacity\t: " << w25_info.NumKB << " KB"
              << "\r\n";
}

uint8_t buffer[16];

void write_data_1(uint32_t addr) {
    cs_set();
    tx_buf[0] = 0x06;
    SPI1_Send(tx_buf, 1);

    HAL_Delay(100);

    tx_buf[0] = W25_SECTOR_ERASE;
    tx_buf[1] = (addr >> 16) & 0xFF;
    tx_buf[2] = (addr >> 8) & 0xFF;
    tx_buf[3] = addr & 0xFF;
    SPI1_Send(tx_buf, 4);
    HAL_Delay(100);

    tx_buf[0] = W25_PROGRAM_PAGE;
    tx_buf[1] = (addr >> 16) & 0xFF;
    tx_buf[2] = (addr >> 8) & 0xFF;
    tx_buf[3] = addr & 0xFF;
    for (int i = 4; i < 8; i++) {
        tx_buf[i] = i;
    }
    SPI1_Send(tx_buf, 8);
    HAL_Delay(100);

    tx_buf[0] = 0x04;
    SPI1_Send(tx_buf, 1);
    cs_reset();
    HAL_Delay(100);
}

void write_data(uint32_t addr) {}

using w25q32 = zoal::ic::w25qxx<spi, mcu::pa_04, delay>;

uint8_t test_buffer[16] = {0};

[[noreturn]] void zoal_main_task(void *) {
    init_terminal();

    W25qxx_Init();
    w25q32::read_data(0, test_buffer, sizeof(test_buffer));

    tx_stream << "\r\n";
    for (unsigned char i : test_buffer) {
        tx_stream << zoal::io::hexadecimal(i) << " ";
    }
    tx_stream << "\r\n";

    terminal.sync();

    for (;;) {
        auto bits = hardware_events.wait(all_hardware_events);
        if ((bits & usart_event) == usart_event) {
            uint8_t rx_buffer[8];
            size_t size;
            do {
                size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer), 0);
                if (size != 0) {
                    terminal.push_and_scan(rx_buffer, size);
                }
            } while (size == sizeof(rx_buffer));
        }
    }
}

extern "C" void SystemClock_Config(void);

using timer = mcu::timer_02;
using pin = mcu::pa_00;
using ch = mcu::mux::pwm_channel<timer, pin>;

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_TIM2_Init();
    MX_ADC1_Init();
//    HAL_ADC_GetValue(&myADC2Handle);

    ch::connect();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}
