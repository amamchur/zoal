#pragma once

#ifndef ZOAL_UTILS_METHOD_INVOKER_HPP
#define ZOAL_UTILS_METHOD_INVOKER_HPP

namespace zoal { namespace utils {
	template <class T, typename... Args>
	class method_invoker {
	public:
		method_invoker(T *obj, void (T::*m)(Args...)) : instance(obj), method(m) {
		}

		void operator()(Args... args) {
			(instance->*method)(args...);
		}
	private:
		T *instance;
		void (T::*method)(Args...);
	};
}}

#endif
