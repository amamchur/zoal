#ifdef __AVR_ARCH__
	#include "../arch/avr/utils/interrupts.hpp"
#elif defined(__CORTEX_M)
	#include "../arch/cortex/interrupts.hpp"
#else
    #include "../arch/simulator/interrupts.hpp"
#endif
