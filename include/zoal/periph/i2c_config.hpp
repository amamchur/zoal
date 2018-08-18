namespace zoal { namespace periph {
	template <uint32_t Freq, uint32_t I2CFreq = 400000>
	class i2c_config {
	public:
		static constexpr uint32_t freq = Freq;
		static constexpr uint32_t i2c_freq = I2CFreq;
	};
}}