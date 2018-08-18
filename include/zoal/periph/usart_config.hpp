#ifndef ZOAL_PERIPH_USART_CONFIG_HPP
#define ZOAL_PERIPH_USART_CONFIG_HPP

#include <stdint.h>

namespace zoal { namespace periph {
	enum class usart_data_bits {
		data_bits_5,
		data_bits_6,
		data_bits_7,
		data_bits_8
	};

	enum class usart_parity {
		none,
		even,
		odd
	};

	enum class usart_stop_bits {
		stop_bits_1,
		stop_bits_2
	};

	template <
		uint32_t Freq,
		uint32_t BaudRate,
		usart_data_bits Bits = usart_data_bits::data_bits_8,
		usart_parity Parity = usart_parity::none,
		usart_stop_bits StopBits = usart_stop_bits::stop_bits_1
	>
	class usart_config {
	public:
		static constexpr uint32_t mcu_freq = Freq;
		static constexpr uint32_t baud_rate = BaudRate;
		static constexpr usart_data_bits data_bits = Bits;
		static constexpr usart_parity parity = Parity;
		static constexpr usart_stop_bits stop_bits = StopBits;
	};
}}

#endif
