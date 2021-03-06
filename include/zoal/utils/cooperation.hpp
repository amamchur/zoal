#ifndef ZOAL_UTILS_YIELD_HPP
#define ZOAL_UTILS_YIELD_HPP

#include <stdint.h>
#include "interrupts.hpp"
#include "nop.hpp"

namespace zoal { namespace utils {
	template<uint64_t NoOps = 1>
	class cooperation {
	public:
		static void yield() {
			zoal::utils::interrupts ei(true);
			zoal::utils::nop<NoOps>::place();
		}
	};
}}

#endif
