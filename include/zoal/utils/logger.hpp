#ifndef ZOAL_UTILS_LOGGER_HPP
#define ZOAL_UTILS_LOGGER_HPP

#include "../io/output_stream.hpp"
#include "defs.hpp"

namespace zoal { namespace utils {
    enum class log_level { trace, debug, info, warn, error, highest };

    template<class Pattern>
    class placer : public Pattern {};

    template<>
    class placer<void> {
    public:
        template<class T>
        ZOAL_INLINE_IO static void write() {}
    };

    template<class Transport, class Prefixer = void, class Suffixer = void, bool enabled = true>
    class log_stream : public zoal::io::output_stream<Transport> {
    public:
        using pp_class = placer<Prefixer>;
        using sp_class = placer<Suffixer>;
        using os_class = zoal::io::output_stream<Transport>;

        log_stream(Transport &t)
            : zoal::io::output_stream<Transport>(t) {
            pp_class::template write<Transport>();
        }

        log_stream(const log_stream &log)
            : zoal::io::output_stream<Transport>(log.target) {
            log.final = false;
            this->precision = log.precision;
            this->radix = log.radix;
        }

        template<class T>
        log_stream &operator<<(T value) {
            static_cast<os_class &>(*this) << value;
            return *this;
        }

        ~log_stream() {
            if (final) {
                sp_class::template write<Transport>();
            }
        }

    private:
        mutable bool final{true};
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

        static Transport *transport;

        ZOAL_INLINE_IO static hs stream() {
            return hs(*transport);
        }

        ZOAL_INLINE_IO static void clear() {}

        ZOAL_INLINE_IO static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>(*transport);
        }

        ZOAL_INLINE_IO static ls<log_level::info >= MinLevel> info() {
            return ls<log_level::info >= MinLevel>(*transport);
        }

        ZOAL_INLINE_IO static ls<log_level::debug >= MinLevel> debug() {
            return ls<log_level::debug >= MinLevel>(*transport);
        }

        ZOAL_INLINE_IO static ls<log_level::warn >= MinLevel> warn() {
            return ls<log_level::warn >= MinLevel>(*transport);
        }

        ZOAL_INLINE_IO static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>(*transport);
        }
    };

    template<class Transport, log_level MinLevel, class Prefixer, class Suffixer>
    Transport *plain_logger<Transport, MinLevel, Prefixer, Suffixer>::transport = nullptr;

    template<class Transport, log_level MinLevel = log_level::info, class Prefixer = void, class Suffixer = zoal::io::modesoff_cr_lf>
    class terminal_logger {
    public:
        static constexpr log_level min_level = MinLevel;

        template<bool enabled>
        using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, enabled>;
        using hs = typename zoal::utils::log_stream<Transport, void, void, true>;

        static Transport *transport;

        ZOAL_INLINE_IO static hs stream() {
            return hs();
        }

        static void clear() {
            hs() << "\033c";
        }

        ZOAL_INLINE_IO static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>(*transport) << "\033[0;36mTRACE ";
        }

        ZOAL_INLINE_IO static ls<log_level::debug >= MinLevel> debug() {
            return ls<log_level::debug >= MinLevel>(*transport) << "\033[0;37mDEBUG ";
        }

        ZOAL_INLINE_IO static ls<log_level::info >= MinLevel> info() {
            return ls<log_level::info >= MinLevel>(*transport) << "\033[0;32mINFO  ";
        }

        ZOAL_INLINE_IO static ls<log_level::warn >= MinLevel> warn() {
            return ls<log_level::warn >= MinLevel>(*transport) << "\033[0;33mWARN  ";
        }

        ZOAL_INLINE_IO static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>(*transport) << "\033[0;31mERROR ";
        }
    };

    template<class Transport, log_level MinLevel, class Prefixer, class Suffixer>
    Transport *terminal_logger<Transport, MinLevel, Prefixer, Suffixer>::transport = nullptr;

    template<class Logger>
    struct cas_print_functor {
        template<class A>
        void operator()() {
            Logger::info() << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set;
        }
    };
}}

#endif
