#include <iostream>
#include <map>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/periph/i2c.hpp>
#include <zoal/utils/tool_set.hpp>

constexpr auto F_CPU = 16000000UL;

using pcb = zoal::board::arduino_uno;
using mcu = pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using adc = mcu::adc_00;
using i2c = mcu::i2c_00;

inline const std::map<uint16_t, const char *> regs{{0x0023, "PINB, Port B Input Pins"},
                                                   {0x0024, "DDRB, Port B Data Direction Register"},
                                                   {0x0025, "PORTB, Port B Data Register"},
                                                   {0x0026, "PINC, Port C Input Pins"},
                                                   {0x0027, "DDRC, Port C Data Direction Register"},
                                                   {0x0028, "PORTC, Port C Data Register"},
                                                   {0x0029, "PIND, Port D Input Pins"},
                                                   {0x002A, "DDRD, Port D Data Direction Register"},
                                                   {0x002B, "PORTD, Port D Data Register"},
                                                   {0x0035, "TIFR0, Timer/Counter 0 Interrupt Flag Register"},
                                                   {0x0036, "TIFR1, Timer/Counter 1 Interrupt Flag Register"},
                                                   {0x0037, "TIFR2, Timer/Counter 2 Interrupt Flag Register"},
                                                   {0x003B, "PCIFR, Pin Change Interrupt Flag Register"},
                                                   {0x003C, "EIFR, External Interrupt Flag Register"},
                                                   {0x003D, "EIMSK, External Interrupt Mask Register"},
                                                   {0x003E, "EECR, EEPROM Control Register"},
                                                   {0x003F, "EEDR, EEPROM Data Register"},
                                                   {0x0040, "EEARL, EEPROM Address Register Low"},
                                                   {0x0041, "EEARH, EEPROM Address Register High"},
                                                   {0x0043, "GTCCR, General Timer/Counter Control Register"},
                                                   {0x0044, "TCCR0A, Timer/Counter 0 Control Register A"},
                                                   {0x0045, "TCCR0B, Timer/Counter 0 Control Register B"},
                                                   {0x0046, "TCNT0, Timer/Counter 0"},
                                                   {0x0047, "OCR0A, Output Compare Register 0 A"},
                                                   {0x0048, "OCR0B, Output Compare Register 0 B"},
                                                   {0x004A, "GPIOR1, General Purpose I/O Register 1"},
                                                   {0x004B, "GPIOR2, General Purpose I/O Register 2"},
                                                   {0x004C, "SPCR, SPI Control Register"},
                                                   {0x004D, "SPSR, SPI Status Register"},
                                                   {0x004E, "SPDR, SPI Data Register"},
                                                   {0x0050, "ACSR, Analog Comparator Control and Status Register"},
                                                   {0x0053, "SMCR, Sleep Mode Control Register"},
                                                   {0x0054, "MCUSR, MCU Status Register"},
                                                   {0x0055, "MCUCR, MCU Control Register"},
                                                   {0x0057, "SPMCSR, Store Program Memory Control and Status Register"},
                                                   {0x005D, "SPL, Stack Pointer Low"},
                                                   {0x005E, "SPH, Stack Pointer High"},
                                                   {0x005F, "SREG, Status RegisterExtended I/O Registers ("},
                                                   {0x0060, "WDTCSR, Watchdog Timer Control Register"},
                                                   {0x0061, "CLKPR, Clock Prescale Register"},
                                                   {0x0064, "PRR, Power Reduction Register"},
                                                   {0x0066, "OSCCAL, Oscillator Calibration Register"},
                                                   {0x0068, "PCICR, Pin Change Interrupt Control Register"},
                                                   {0x0069, "EICRA, External Interrupt Control Register A"},
                                                   {0x006B, "PCMSK0, Pin Change Mask Register 0"},
                                                   {0x006C, "PCMSK1, Pin Change Mask Register 1"},
                                                   {0x006D, "PCMSK2, Pin Change Mask Register 2"},
                                                   {0x006E, "TIMSK0, Timer/Counter 0 Interrupt Mask Register"},
                                                   {0x006F, "TIMSK1, Timer/Counter 1 Interrupt Mask Register"},
                                                   {0x0070, "TIMSK2, Timer/Counter 2 Interrupt Mask Register"},
                                                   {0x0078, "ADCL, ADC Data Register Low"},
                                                   {0x0079, "ADCH, ADC Data Register High"},
                                                   {0x007A, "ADCSRA, ADC Control and Status Register A"},
                                                   {0x007B, "ADCSRB, ADC Control and Status Register B"},
                                                   {0x007C, "ADMUX, ADC Multiplexer Selection Register"},
                                                   {0x007E, "DIDR0, Digital Input Disable Register 0"},
                                                   {0x007F, "DIDR1, Digital Input Disable Register 1"},
                                                   {0x0080, "TCCR1A, Timer/Counter 1 Control Register A"},
                                                   {0x0081, "TCCR1B, Timer/Counter 1 Control Register B"},
                                                   {0x0082, "TCCR1C, Timer/Counter 1 Control Register C"},
                                                   {0x0084, "TCNT1L, Timer/Counter 1 Low Byte"},
                                                   {0x0085, "TCNT1H, Timer/Counter 1 High Byte"},
                                                   {0x0086, "ICR1L, Input Capture Register 1 Low Byte"},
                                                   {0x0087, "ICR1H, Input Capture Register 1 High Byte"},
                                                   {0x0088, "OCR1AL, Output Compare Register 1 A Low Byte"},
                                                   {0x0089, "OCR1AH, Output Compare Register 1 A High Byte"},
                                                   {0x008A, "OCR1BL, Output Compare Register 1 B Low Byte"},
                                                   {0x008B, "OCR1BH, Output Compare Register 1 B High Byte"},
                                                   {0x00B0, "TCCR2A, Timer/Counter 2 Control Register A"},
                                                   {0x00B1, "TCCR2B, Timer/Counter 2 Control Register B"},
                                                   {0x00B2, "TCNT2, Timer/Counter 2"},
                                                   {0x00B3, "OCR2A, Output Compare Register 2 A"},
                                                   {0x00B4, "OCR2B, Output Compare Register 2 B"},
                                                   {0x00B6, "ASSR, Asynchronous Status Register"},
                                                   {0x00B8, "TWBR, TWI Bit Rate Register"},
                                                   {0x00B9, "TWSR, TWI Status Register"},
                                                   {0x00BA, "TWAR, TWI (Slave) Address Register"},
                                                   {0x00BB, "TWDR, TWI Data Register"},
                                                   {0x00BC, "TWCR, TWI Control Register"},
                                                   {0x00BD, "TWAMR, TWI (Slave) Address Mask Register"},
                                                   {0x00C0, "UCSR0A, USART Control and Status Register 0 A"},
                                                   {0x00C1, "UCSR0B, USART Control and Status Register 0 B"},
                                                   {0x00C2, "UCSR0C, USART Control and Status Register 0 C"},
                                                   {0x00C4, "UBRR0L, USART Baud Rate Register 0 Low"},
                                                   {0x00C5, "UBRR0H, USART Baud Rate Register 0 High"},
                                                   {0x00C6, "UDR0, USART I/O Data Register 0"}

};

struct cas_iter {
    template<class T>
    void operator()() const {
        std::string desc;
        auto pos = regs.find(T::address);
        if (pos != regs.end()) {
            desc = pos->second;
        }

        std::cout << std::format("addr: 0x{:04X} 0x{:02X} 0x{:02X} | {}", T::address, T::clear, T::set, desc) << std::endl;
    }
};

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;
    using i2c_cfg = zoal::periph::i2c_fast_mode<F_CPU>;
    using adc_cfg = zoal::periph::adc_params<>;
    using i2c_cfg = zoal::periph::i2c_fast_mode<F_CPU>;

    cas_iter ci;

    // Power on modules
    using l1 = api::optimize<api::clock_on<usart, i2c, timer>>;
    zoal::ct::type_list_iterator<l1>::for_each(ci);

    // l1();
    std::cout << "----------------" << std::endl;

    // Disable all modules before applying settings
    using l2 = api::optimize<api::disable<usart, timer, adc, i2c>>;
    zoal::ct::type_list_iterator<l2>::for_each(ci);
    //l2();
    std::cout << "----------------" << std::endl;

    using l3 = api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, adc_cfg>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::connect,
        mcu::cfg::i2c<i2c, i2c_cfg>::apply,
        //
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //
        >;
    zoal::ct::type_list_iterator<l3>::for_each(ci);
    // l3();
    std::cout << "----------------" << std::endl;

    //
    // // Enable system interrupts
    // zoal::utils::interrupts::on();
    //
    // // Enable all modules
    // api::optimize<api::enable<usart, timer, adc, i2c>>();
}

int main() {
    initialize_hardware();
    return 0;
}
