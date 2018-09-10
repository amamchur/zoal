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

    template<class Prefixer>
    class prefix_placer {
    public:
        template<class T>
        void place_preffix() {
            Prefixer p;
            T::write(p);
        }
    };

    template<>
    class prefix_placer<void> {
    public:
        template<class T>
        void place_preffix() {
        }
    };

    template<class Suffixer>
    class suffix_placer {
    public:
        suffix_placer() : enabled(false) {
        }

        template<class T>
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

    template<>
    class suffix_placer<void> {
    public:
        template<class T>
        void place_suffix() {
        }

        void enable_suffix() {
        }
    };

    template<
            class Transport,
            class Prefixer = void,
            class Suffixer = void,
            bool enabled = true
    >
    class log_stream : public prefix_placer<Prefixer>, suffix_placer<Suffixer> {
    public:
        log_stream() {
            this->template place_preffix<Transport>();
        }

        log_stream(const log_stream &log) noexcept {
            log.flush = false;
            stream.precision = log.stream.precision;
            stream.radix = log.stream.radix;
            this->enable_suffix();
        }

        ~log_stream() {
            if (flush) {
                this->template place_suffix<Transport>();
                Transport::flush();
            }
        }

        template<class T>
        log_stream &operator<<(T value) {
            stream << value;
            return *this;
        }

        log_stream &place(const char *file, int line) {
            stream << file << ":" << line << " ";
            return *this;
        }

    private:
        mutable bool flush{true};
        zoal::io::output_stream<Transport> stream;
    };

    template<class Prefixer, class Suffixer, bool enabled>
    class log_stream<void, Prefixer, Suffixer, enabled> {
    public:
        template<class T>
        log_stream &operator<<(T) {
            return *this;
        }
    };

    template<class Transport, class Prefixer, class Suffixer>
    class log_stream<Transport, Prefixer, Suffixer, false> {
    public:
        template<class T>
        log_stream &operator<<(T) {
            return *this;
        }
    };

    template<
            class Transport,
            log_level MinLevel = log_level::info,
            class Prefixer = zoal::io::new_line_cr_lf,
            class Suffixer = void
    >
    class plain_logger {
    public:
        static constexpr log_level min_level = MinLevel;

        template<bool enabled>
        using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, enabled>;
        using hs = typename zoal::utils::log_stream<Transport, void, void, true>;

        static hs stream() {
            return hs();
        }

        static void clear() {
        }

        static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>();
        }

        static ls<log_level::info >= MinLevel> info() {
            return ls<log_level::info >= MinLevel>();
        }

        static ls<log_level::debug >= MinLevel> debug() {
            return ls<log_level::debug >= MinLevel>();
        }

        static ls<log_level::warn >= MinLevel> warn() {
            return ls<log_level::warn >= MinLevel>();
        }

        static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>();
        }
    };

    template<
            class Transport,
            log_level MinLevel = log_level::info,
            class Prefixer = zoal::io::new_line_cr_lf,
            class Suffixer = zoal::io::stop_escape_sequence
    >
    class terminal_logger {
    public:
        static constexpr log_level min_level = MinLevel;

        template<bool enabled>
        using ls = typename zoal::utils::log_stream<Transport, Prefixer, Suffixer, enabled>;
        using hs = typename zoal::utils::log_stream<Transport, void, void, true>;

        static hs stream() {
            return hs();
        }

        static void clear() {
            hs() << "\033c";
        }

        static ls<log_level::trace >= MinLevel> trace() {
            return ls<log_level::trace >= MinLevel>() << "\033[0;36mTRACE ";
        }

        static ls<log_level::debug >= MinLevel>debug() {
            return ls<log_level::debug >= MinLevel>() << "\033[0;37mDEBUG ";
        }

        static ls<log_level::info >= MinLevel>info() {
            return ls<log_level::info >= MinLevel>() << "\033[0;32mINFO  ";
        }

        static ls<log_level::warn >= MinLevel>warn() {
            return ls<log_level::warn >= MinLevel>() << "\033[0;33mWARN  ";
        }

        static ls<log_level::error >= MinLevel> error() {
            return ls<log_level::error >= MinLevel>() << "\033[0;31mERROR ";
        }
    };
}}

#endif
