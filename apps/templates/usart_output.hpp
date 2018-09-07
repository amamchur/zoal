#ifndef APPS_USARTAPP_HPP_
#define APPS_USARTAPP_HPP_

#include <zoal/io/output_stream.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/gpio/pin.hpp>

template<class USART, class Config, class Tools, class LED>
class usart_output {
public:
    void init() {
        LED::template mode<zoal::gpio::pin_mode::output>();
        USART::template setup<Config>();
    }

    zoal::io::output_stream<USART> cout;

    void run_once() {
        cout << "DataBits: " << (uint16_t) Config::data_bits << "\r\n";
        cout << "Parity  : " << (uint16_t) Config::parity << "\r\n";
        cout << "StopBits: " << (uint16_t) Config::stop_bits << "\r\n";
        cout << "\r\n";
    }

    void run() {
        zoal::io::output_stream<USART> cout;

        cout << "\r\nStart!!!\r\n";
        while (true) {
            run_once();
            Tools::delay::ms(1000);
        }
    }
};

#endif
