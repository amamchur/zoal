#ifdef __AVR_ARCH__
	#include "../arch/avr/utils/nop.hpp"
#elif __ARM_ARCH == 7
	#include "../arch/cortex/m3/nop.hpp"
#else
    #include "../arch/simulator/nop.hpp"
#endif
