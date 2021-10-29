#include "hardware.hpp"
#include "main.h"
#include "stm32f0xx.h"
#include "terminal.hpp"

class functor {
public:
    template<class T>
    void operator()() {
        using cas = typename T::type;
        print(cas::address, cas::clear, cas::set);
    }

    static void print(uint32_t address, uint32_t clear, uint32_t set) {
        auto reg = reinterpret_cast<uint32_t *>(address);
        stream << "ADDR: " << zoal::io::hexadecimal(address) << " " << zoal::io::hexadecimal(clear) << " " << zoal::io::hexadecimal(set);
        stream << " REG: " << zoal::io::hexadecimal(*reg) << "\r\n";
    }
};

extern "C" void SystemClock_Config(void);

static void zoal_init_hardware() {
    HAL_Init();
    SystemClock_Config();

    using namespace zoal::gpio;
    using api = zoal::gpio::api;
    using zoal::gpio::pin_mode;

    // Enable bus clock for peripherals
    api::optimize<
        //
        usart_mux::clock_on,
        usart_cfg::clock_on,
        led_pin::port::clock_on_cas
        //
        >();
    // Disable peripherals before configuration
    api::optimize<api::disable<usart>>();
    api::optimize<
        //
        usart_mux::connect,
        usart_cfg::apply,
        api::mode<pin_mode::output, led_pin>>();
    // Enable peripherals after configuration
    api::optimize<api::enable<usart>>();

    NVIC_EnableIRQ(USART1_IRQn);
    usart::enable_rx();
    zoal::utils::interrupts::on();
}

[[noreturn]] static void zoal_main() {
    using namespace zoal::gpio;

    init_terminal();

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

        //        ::led_pin::low();
        //        ::delay::ms(500);
        //
        //        ::led_pin::high();
        //        ::delay::ms(500);
        //        stream << counter++ << "\r\n";
    }
}

int main() {
    HAL_Init();
    SystemClock_Config();
    zoal_init_hardware();
    zoal_main();
    return 0;
}
