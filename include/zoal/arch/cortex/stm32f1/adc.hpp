#ifndef ZOAL_ARCH_STM32F1_A2D_CONVERTER_HPP
#define ZOAL_ARCH_STM32F1_A2D_CONVERTER_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
	template<uintptr_t BaseAddress, uint8_t N, class Clock>
	class adc : public Clock {
	public:
		volatile uint32_t SR;
		volatile uint32_t CR1;
		volatile uint32_t CR2;
		volatile uint32_t SMPR1;
		volatile uint32_t SMPR2;
		volatile uint32_t JOFR1;
		volatile uint32_t JOFR2;
		volatile uint32_t JOFR3;
		volatile uint32_t JOFR4;
		volatile uint32_t HTR;
		volatile uint32_t LTR;
		volatile uint32_t SQR1;
		volatile uint32_t SQR2;
		volatile uint32_t SQR3;
		volatile uint32_t JSQR;
		volatile uint32_t JDR1;
		volatile uint32_t JDR2;
		volatile uint32_t JDR3;
		volatile uint32_t JDR4;
		volatile uint32_t DR;

		using Class = adc<BaseAddress, N, Clock>;

		static constexpr uintptr_t Base = BaseAddress;
		static constexpr uint8_t no = N;
		static constexpr uint8_t resolution = 12;

	    static inline Class* instance() {
	        return (Class*)BaseAddress;
	    }

	    template <class Config>
	    static void connect() {
//			ADC_TypeDef *adc = (ADC_TypeDef *)Base;
//			ADC_RegularChannelConfig(adc, Config::Channel, 1, ADC_SampleTime_28Cycles5);
	    }

	    static uint16_t read() {
	    	auto adc = Class::instance();
	    	adc->CR2 |= 0x00500000 | 0x00000001;
			while ((adc->SR & 0x2) == 0);
			uint16_t value = adc->DR;
			adc->CR2 &= 0xFFAFFFFF;
			return value;
	    }

	    static void setup() {
	    	Clock::enable();
//
//			ADC_TypeDef *adc = (ADC_TypeDef *)Base;
//
//	        ADC_InitTypeDef ADC_InitStructure;
//	        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//	        ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//	        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // we work in continuous sampling mode
//	        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	        ADC_InitStructure.ADC_NbrOfChannel = 1;
//	        ADC_Init (adc, &ADC_InitStructure);   //set config of ADC1
	    }
	};
}}}

#endif
