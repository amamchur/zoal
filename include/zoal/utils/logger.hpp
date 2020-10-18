#ifndef ZOAL_UTILS_LOGGER_HPP
#define ZOAL_UTILS_LOGGER_HPP

#include "../io/output_stream.hpp"
#include "defs.hpp"

namespace zoal { namespace utils {
    enum class log_level { trace, debug, info, warn, error, highest };

    template<class Prefixer>
    class prefix_placer {
    public:
        template<class T>
        void place_prefix(T &t) {
            Prefixer p;
            t << p;
        }
    };

    template<>
    class prefix_placer<void> {
    public:
        template<class T>
        ZOAL_INLINE_IO void place_prefix(T &t) {}
    };

    template<class Suffixer>
    class suffix_placer {
    public:
        template<class T>
        void place_suffix(T &t) {
            Suffixer p;
            t << p;
        }
    };

    template<>
    class suffix_placer<void> {
    public:
        template<class T>
        ZOAL_INLINE_IO void place_suffix(T &) {}
    };

    template<class Transport, class Prefixer = void, class Suffixer = void, bool enabled = true>
    class log_stream : public prefix_placer<Prefixer>, suffix_placer<Suffixer> {
    public:
        log_stream()
            : stream(zoal::io::transport_proxy<Transport>::instance()) {
            this->template place_prefix<>(*this);
        }

        log_stream(const log_stream &log)
            : stream(log.stream) {
            log.final = false;
            stream.precision = log.stream.precision;
            stream.radix = log.stream.radix;
        }

        ~log_stream() {
            if (final) {
                this->template place_suffix<>(*this);
            }
        }

        template<class T>
        log_stream &operator<<(T value) {
            stream << value;
            return *this;
        }

    private:
        mutable bool final{true};
        zoal::io::output_stream stream;
    };

    template<class Prefixer, class Suffixer, bool enabled>
    class log_stream<void, Prefixer, Suffixer, enabled> {
    public:
        template<class T>
        ZOAL_INLINE_IO log_stream &operator<<(T) {
            return *this;
        }
    };

    template<class Transport, class Prefixer, class Suffixer>
    class log_stream<Transport, Prefixer, Suffixer, false> {
    public:
        template<class T>
        ZOAL_INLINE_IO log_stream &operator<<(T) {
            return *this;
        }
    };

    template<class Prefixer, class Suffixer>
    class log_stream<void, Prefixer, Suffixer, false> {
    public:
        template<class T>
        ZOAL_INLINE_IO log_stream &operator<<(T) {
            return *this;
        }
    };

    template<class Transport, log_level MinLevel = log_level::info, class Prefixer = zoal::io::new_line_cr_lf, class Suffixer = void>
    class plain_logger {
    public:
        static constexpr log_level min_level = MinLevel;

        template<bool enabled>
        using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, enabled>;
        using hs = typename zoal::utils::log_stream<Transport, void, void, true>;

        ZOAL_INLINE_IO static hs stream() {
            return hs();
        }

        ZOAL_INLINE_IO static void clear() {}

        ZOAL_INLINE_IO static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>();
        }

        ZOAL_INLINE_IO static ls<log_level::info >= MinLevel> info() {
            return ls<log_level::info >= MinLevel>();
        }

        ZOAL_INLINE_IO static ls<log_level::debug >= MinLevel> debug() {
            return ls<log_level::debug >= MinLevel>();
        }

        ZOAL_INLINE_IO static ls<log_level::warn >= MinLevel> warn() {
            return ls<log_level::warn >= MinLevel>();
        }

        ZOAL_INLINE_IO static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>();
        }
    };

    template<class Transport, log_level MinLevel = log_level::info, class Prefixer = zoal::io::new_line_cr_lf, class Suffixer = zoal::io::stop_escape_sequence>
    class terminal_logger {
    public:
        static constexpr log_level min_level = MinLevel;

        template<bool enabled>
        using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, enabled>;
        using hs = typename zoal::utils::log_stream<Transport, void, void, true>;

        ZOAL_INLINE_IO static hs stream() {
            return hs();
        }

        static void clear() {
            hs() << "\033c";
        }

        ZOAL_INLINE_IO static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>() << "\033[0;36mTRACE ";
        }

        ZOAL_INLINE_IO static ls<log_level::debug >= MinLevel> debug() {
            return ls<log_level::debug >= MinLevel>() << "\033[0;37mDEBUG ";
        }

        ZOAL_INLINE_IO static ls<log_level::info >= MinLevel> info() {
            return ls<log_level::info >= MinLevel>() << "\033[0;32mINFO  ";
        }

        ZOAL_INLINE_IO static ls<log_level::warn >= MinLevel> warn() {
            return ls<log_level::warn >= MinLevel>() << "\033[0;33mWARN  ";
        }

        ZOAL_INLINE_IO static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>() << "\033[0;31mERROR ";
        }
    };

    template<class Logger>
    struct cas_print_functor {
        template<class A>
        void operator()() {
            Logger::info() << "CAS: " << (void *)A::address << " | " << (void *)A::clear << ", " << (void *)A::set;
        }
    };
}}

#endif
