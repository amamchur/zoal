#ifndef ZOAL_PERIPH_I2C_HPP
#define ZOAL_PERIPH_I2C_HPP

namespace zoal { namespace periph {
	template <uint32_t Freq, uint32_t I2CFreq = 400000>
	class i2c_params {
	public:
		static constexpr uint32_t clock_frequency = Freq;
		static constexpr uint32_t i2c_frequency = I2CFreq;
	};

    template <uint32_t Freq>
    using i2c_fast_mode = i2c_params<Freq, 400000>;
}}

#endif
