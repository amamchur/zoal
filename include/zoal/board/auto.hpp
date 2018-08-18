#ifdef ARDUINO_AVR_UNO
	#include "arduino_uno.hpp"
#elif ARDUINO_AVR_NANO
	#include "ArduinoNano.hpp"
#elif ARDUINO_AVR_PRO
	#include "arduino_pro_mini.hpp"
#elif ARDUINO_AVR_PROMICRO
 	#include "SparkFunProMicro.hpp"
#elif ARDUINO_AVR_MEGA2560
	#include "ArduinoMega2560.hpp"
#elif ARDUINO_AVR_LEONARDO
	#include "ArduinoLeonardo.hpp" 
#elif ARDUINO_SAM_DUE
	#include "ArduinoDue.hpp"
#elif ARDUINO_ESP8266_WEMOS_D1MINI
	#include "WeMosD1.hpp"
#elif STM32F030
	#include "STM32F030.hpp"
#elif STM32F10X_MD
	#include "stm32f103.hpp"
#else
	#pragma message "Unknown board"
#endif
