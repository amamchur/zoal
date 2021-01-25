#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/ic/lm75.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/new.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

FUSES = {
    .low = 0xFF,
    .high = 0xD7,
    .extended = 0xFC
};

volatile uint32_t milliseconds = 0;
volatile uint16_t adc_value = 0;
volatile bool process_adc = false;

using pcb = zoal::pcb;
using mcu = zoal::pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using spi = mcu::spi_00;
using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using counter_irq_handler = counter::handler<F_CPU, 64, timer>;

zoal::data::ring_buffer<uint8_t, 16> rx_buffer;

using usart_tx_transport = zoal::utils::usart_transmitter<usart, 16, zoal::utils::interrupts_off>;
usart_tx_transport transport;
using tx_stream_type = zoal::io::output_stream<usart_tx_transport>;
tx_stream_type stream(transport);

using tools = zoal::utils::tool_set<mcu, F_CPU, counter, void>;
using delay = tools::delay;
using api = zoal::gpio::api;
using blink_pin = pcb::ard_d13;
using lcd_back_light = pcb::ard_d10;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

scheduler_type scheduler;
constexpr size_t terminal_str_size = 128;
using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[terminal_str_size];
zoal::misc::terminal_input terminal(terminal_buffer, sizeof(terminal_buffer));
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";

const char help_msg[] PROGMEM =
    "ZOAL Demo Application\r\n"
    "Commands: \r\n"
    "\t start-blink\tstart blinking\r\n"
    "\t stop-blink\tstop blinking\r\n";
const char help_cmd[] PROGMEM = "help";
const char adc_cmd[] PROGMEM = "adc";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";

void initialize_hardware() {
    // Power on modules
    api::optimize<api::clock_on<usart, timer>>();

    // Disable all modules before applying settings
    api::optimize<api::disable<usart, timer, adc, i2c>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::cfg::usart<usart, zoal::periph::usart_115200<F_CPU>>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, zoal::periph::adc_config<>>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::connect,
        mcu::cfg::i2c<i2c, F_CPU>::apply,
        //
        api::mode<zoal::gpio::pin_mode::output, blink_pin, lcd_back_light>,
        api::_1<lcd_back_light>,
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //
        >();

    // Enable system interrupts
    zoal::utils::interrupts::on();

    // Enable all modules
    api::optimize<api::enable<usart, timer, adc, i2c>>();

    adc::enable();
    adc::enable_interrupt();
}

void led_on(void *);

void led_off(void *);

void led_on(void *) {
    ::blink_pin::high();
    scheduler.schedule(500, led_off);
}

void led_off(void *) {
    ::blink_pin::low();
    scheduler.schedule(500, led_on);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx_transport::send_data(s, e - s);
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    p->callback(&command_line_parser::empty_callback);

    stream << "\r\n";

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(help_cmd), ts, te)) {
        stream << zoal::io::progmem_str(help_msg);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(adc_cmd), ts, te)) {
        auto v = adc::read();
        stream << "ADC: " << v << "\r\n";
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(start_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        scheduler.schedule(0, led_on);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
    }

    terminal.sync();
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);
    terminal.sync();
}

const char zoal_ascii_logo[] PROGMEM = "  __________          _\r\n"
                                       " |___  / __ \\   /\\   | |\r\n"
                                       "    / / |  | | /  \\  | |\r\n"
                                       "   / /| |  | |/ /\\ \\ | |\r\n"
                                       "  / /_| |__| / ____ \\| |____\r\n"
                                       " /_____\\____/_/    \\_\\______|\r\n"
                                       "\r\n";

void analog_keypad_handler(zoal::io::button_event e, int button) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    stream << "Buttons press: " << button << "\r\n";
}

template<class Ret, class... Args>
class abstract_invocation {
public:
    virtual Ret operator()(Args ...args) = 0;
};

template<class T, class Ret, class... Args>
class method_invocation : public abstract_invocation<Ret, Args...> {
public:
    method_invocation(T *obj, Ret (T::*method)(Args ...)) : instance(obj), callback(method) {}

    Ret operator()(Args ...args) override {
        return (*instance.*callback)(args...);
    }

    T *instance;

    Ret (T::*callback)(Args ...);
};

template<class T, class... Args>
class method_invocation<T, void, Args...> : public abstract_invocation<void, Args...> {
public:
    method_invocation(T *obj, void (T::*method)(Args ...)) : instance(obj), callback(method) {}

    void operator()(Args ...args) override {
        (*instance.*callback)(args...);
    }

    T *instance;

    void (T::*callback)(Args ...);
};

template<class T, class Ret, class... Args>
class const_method_invocation : public abstract_invocation<Ret, Args...> {
public:
    const_method_invocation(const T *obj, Ret (T::*method)(Args ...) const) : instance(obj), callback(method) {}

    Ret operator()(Args ...args) override {
        return (*instance.*callback)(args...);
    }

    const T *instance;

    Ret (T::*callback)(Args ...) const;
};

template<class L, class Ret, class... Args>
class lambda_invocation : public abstract_invocation<Ret, Args...> {
public:
    explicit lambda_invocation(L l) : lambda(l) {}

    Ret operator()(Args ...args) override {
        return lambda(args...);
    }

    L lambda;
};

template<class L, class... Args>
class lambda_invocation<L, void, Args...> : public abstract_invocation<void, Args...> {
public:
    explicit lambda_invocation(L l) : lambda(l) {}

    void operator()(Args ...args) override {
        lambda(args...);
    }

    L lambda;
};

template<class Ret, class... Args>
class base_invoker {
public:
protected:
    abstract_invocation<Ret, Args...> *invocation{nullptr};
};

template<class... Args>
class base_invoker<void, Args...> {
public:
protected:
    abstract_invocation<void, Args...> *invocation{nullptr};
};

template<size_t ClosureBankSize, class Ret, class... Args>
class manager : public base_invoker<Ret, Args...> {
public:
    static constexpr size_t bank_size = ClosureBankSize;
    static constexpr size_t bank_count = 2;

    void *get_mem_bank() {
        return mem_banks[active_bank];
    }

    template<class T>
    void create_handler(T *obj, Ret (T::*method)(Args ...)) {
        using invocation_type = method_invocation<T, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(obj, method);
        this->invocation = instance;
    }

    template<class T>
    void create_handler(T *obj, Ret (T::*method)(Args ...) const) {
        using invocation_type = const_method_invocation<T, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(obj, method);
        this->invocation = instance;
    }

    template<class L>
    void create_handler(L l) {
        using invocation_type = lambda_invocation<L, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(l);
        this->invocation = instance;
    }

    void invoke(Args ...args) {
        auto inv = this->invocation;
        active_bank = (active_bank + 1) % bank_count;
        this->invocation = nullptr;
        (*inv)(args...);
    }

    bool has_invocation() const {
        return this->invocation != nullptr;
    }

private:
    size_t active_bank{0};
    uint8_t mem_banks[bank_count][bank_size]{{0}};
};

template<class M>
class mng_client {
public:
    explicit mng_client(M &m) : manager(m) {}

    mng_client(const mng_client &) = delete;

    void start(int number, int &b) {
        auto l3 = [&b]() {
            b++;
            return 0;
        };

        auto l2 = [this, l3, &b]() {
            b++;
            this->manager.create_handler(l3);
            return 0;
        };

        auto l1 = [this, l2, &b]() {
            b++;
            this->manager.create_handler(l2);
            return 0;
        };
        manager.create_handler(l1);
    }

    M &manager;
};

void do_test() {
    using manager_type = manager<sizeof(void *) * 8, int>;
    manager_type mng;
    mng_client<manager_type> client(mng);

    int b = 1;
    client.start(5, b);
    while (mng.has_invocation()) {
        mng.invoke();
    }

    stream << b << "\r\n";
}

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    do_test();

    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();

    while (true) {
        uint8_t rx_byte = 0;
        bool result;
        {
            zoal::utils::interrupts_off scope_off;
            result = rx_buffer.pop_front(rx_byte);
        }

        if (result) {
            terminal.push(&rx_byte, 1);
        }

        scheduler.handle();
    }
}

#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    counter_irq_handler::increment();
}

ISR(USART_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_transport::tx_buffer.pop_front(value); });
}

ISR(ADC_vect) {
}

ISR(TWI_vect) {
}