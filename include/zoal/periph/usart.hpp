#ifndef ZOAL_PERIPH_USART_CONFIG_HPP
#define ZOAL_PERIPH_USART_CONFIG_HPP

#include <stdint.h>

namespace zoal { namespace periph {
    enum class usart_parity { none, even, odd };

    enum class usart_stop_bits { stop_bits_1, stop_bits_2 };

    template<uint32_t UsartFreq,
             uint32_t BaudRate,
             uint8_t Bits = 8,
             zoal::periph::usart_parity Parity = zoal::periph::usart_parity::none,
             zoal::periph::usart_stop_bits StopBits = zoal::periph::usart_stop_bits::stop_bits_1>
    class usart_params {
    public:
        static constexpr uint32_t clock_frequency = UsartFreq;
        static constexpr uint32_t baud_rate = BaudRate;
        static constexpr uint32_t word_length_bits = Bits;
        static constexpr zoal::periph::usart_parity parity = Parity;
        static constexpr zoal::periph::usart_stop_bits stop_bits = StopBits;
    };

    template<uint32_t UsartFreq>
    using usart_115200 = usart_params<UsartFreq, 115200>;

    template<uint32_t UsartFreq>
    using usart_57600 = usart_params<UsartFreq, 57600>;

    template<uint32_t UsartFreq>
    using usart_38400 = usart_params<UsartFreq, 38400>;

    template<uint32_t UsartFreq>
    using usart_19200 = usart_params<UsartFreq, 19200>;

    template<uint32_t UsartFreq>
    using usart_9600 = usart_params<UsartFreq, 9600>;
}}

#endif
