#ifndef ZOAL_UTILS_INTERRUPTS_HPP
#define ZOAL_UTILS_INTERRUPTS_HPP

#ifdef __AVR_ARCH__
	#include "../arch/avr/utils/interrupts.hpp"
#elif defined(__ARM_ARCH)
	#include "../arch/cortex/interrupts.hpp"
#else
    #include "../arch/simulator/interrupts.hpp"
#endif

namespace zoal { namespace utils {
    class interrupts_off : public interrupts {
    public:
        interrupts_off() : interrupts(false) {
        }
    };

    class interrupts_on : public interrupts {
    public:
        interrupts_on() : interrupts(true) {
        }
    };
}}

#endif
