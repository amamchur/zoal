#ifndef ZOAL_UTILS_LOGGER_HPP
#define ZOAL_UTILS_LOGGER_HPP

#include "../io/output_stream.hpp"

namespace zoal { namespace utils {
	enum class log_level {
		trace,
		debug,
		info,
		warn,
		error,
		highest
	};

	template <class Prefixer>
	class prefix_placer {
	public:
		template <class T>
		void place_preffix() {
			Prefixer p;
			T::write(p);
		}
	};

	template <>
	class prefix_placer<void> {
	public:
		template <class T>
		inline void place_preffix() {
		}
	};

	template <class Suffixer>
	class suffix_placer {
	public:
		suffix_placer() : enabled(false) {
		}

		template <class T>
		void place_suffix() {
			if (enabled) {
				Suffixer p;
				T::write(p);
			}
		}

		void enable_suffix() {
			enabled = true;
		}

		bool enabled;
	};

	template <>
	class suffix_placer<void> {
	public:
		template <class T>
		inline void place_suffix() {
		}

		inline void enable_suffix() {
		}
	};

	template <
		class Transport,
		class Prefixer = void,
		class Suffixer = void,
		log_level Current = log_level::highest,
		log_level MinLevel = log_level::highest
	>
	class log_stream : public prefix_placer<Prefixer>, suffix_placer<Suffixer> {
	public:
		log_stream() {
			if (Current >= MinLevel) {
                this->template place_preffix<Transport>();
			}
		}

		log_stream(const log_stream &log) noexcept {
            log.flush = false;
			stream.precision = log.stream.precision;
			stream.radix = log.stream.radix;
			if (Current >= MinLevel) {
				this->enable_suffix();
			}
		}

		~log_stream() {
			if (Current >= MinLevel) {
				this->template place_suffix<Transport>();
			}
			if (flush) {
                Transport::flush();
            }
		}

		template<class T>
		inline log_stream& operator<< (T value) {
			if (Current >= MinLevel) {
				stream << value;
			}
			return *this;
		}

		log_stream& place(const char *file, int line) {
			stream << file << ":" << line << " ";
			return *this;
		}
	private:
	    mutable bool flush{true};
		zoal::io::output_stream<Transport> stream;
	};

	template <class Prefixer, class Suffixer, log_level Current, log_level MinLevel>
	class log_stream<void, Prefixer, Suffixer, Current, MinLevel> {
	public:
		template<class T>
		inline log_stream& operator<< (T) {
			return *this;
		}
	};

	template <
		class Transport,
		log_level MinLevel = log_level::info,
		class Prefixer = zoal::io::new_line_cr_lf,
		class Suffixer = void
	>
	class plain_logger {
	public:
		static constexpr log_level min_level = MinLevel;

		template <log_level Current>
		using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, Current, MinLevel>;
		using hs = typename zoal::utils::log_stream<Transport, void, void, log_level::highest, log_level::highest>;

		static inline hs stream() {
			return hs();
		}

        static inline void clear() {
        }

		static inline ls<log_level::trace> trace() {
			return ls<log_level::trace>();
		}

		static inline ls<log_level::info> info() {
			return ls<log_level::info>();
		}

		static inline ls<log_level::debug> debug() {
			return ls<log_level::debug>();
		}

		static inline ls<log_level::warn> warn() {
			return ls<log_level::warn>();
		}

		static inline ls<log_level::error> error() {
			return ls<log_level::error>();
		}
	};

	template <
		class Transport,
		log_level MinLevel = log_level::info,
		class Prefixer = zoal::io::new_line_cr_lf,
		class Suffixer = zoal::io::stop_escape_sequence
	>
	class terminal_logger {
	public:
		static constexpr log_level min_level = MinLevel;

		template <log_level Current>
		using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, Current, MinLevel>;
		using hs = typename zoal::utils::log_stream<Transport, void, void, log_level::highest, log_level::highest>;

		static inline hs stream() {
			return hs();
		}
		static inline void clear() {
			hs() << "\033c";
		}

		static inline ls<log_level::trace> trace() {
			return ls<log_level::trace>() << "\033[36;0mTRACE ";
		}

		static inline ls<log_level::debug> debug() {
			return ls<log_level::debug>() << "\033[34;0mDEBUG ";
		}

		static inline ls<log_level::info> info() {
			return ls<log_level::info>() << "\033[32;0mINFO  ";
		}

		static inline ls<log_level::warn> warn() {
			return ls<log_level::warn>() << "\033[33;0mWARN  ";
		}

		static inline ls<log_level::error> error() {
			return ls<log_level::error>() << "\033[31;0mERROR ";
		}
	};
}}

#endif
