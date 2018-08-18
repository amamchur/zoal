#ifdef __AVR_ARCH__
	#include "../arch/avr/utils/nop.hpp"
#elif defined(__CORTEX_M)
	#include "../arch/cortex/nop.hpp"
#else
    #include "../arch/simulator/nop.hpp"
#endif
