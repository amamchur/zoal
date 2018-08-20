#ifndef ZOAL_UTILS_TOOLS_HPP
#define ZOAL_UTILS_TOOLS_HPP

#include "scheduler.hpp"
#include "delay.hpp"
#include "nop.hpp"
#include "logger.hpp"
#include "../gpio/base_api.hpp"

namespace zoal { namespace utils {
	template<class Microcontroller, class Counter, class Logger = plain_logger<void>>
	class tool_set {
	public:
		using mcu = Microcontroller;
		using counter = Counter;
		using logger = Logger;
		using counter_value_type = typename Counter::value_type;
		using delay = utils::delay<mcu, counter>;

		template <size_t Capacity, class Token = void>
		using function_scheduler = typename ::zoal::utils::function_scheduler<counter, Capacity, Token>;

		template <class T, size_t Capacity, class Token = void>
		using method_scheduler = typename ::zoal::utils::method_scheduler<T, counter, Capacity, Token>;

		using api = ::zoal::gpio::base_api<typename mcu::port_chain>;

		template <class A, class B>
		static B remap(A x, A in_min, A in_max, B out_min, B out_max) {
  			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		template <class A, class B, A in_min, A in_max, B out_min, B out_max>
		static B remap(A x) {
  			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
	};
}}

#endif
