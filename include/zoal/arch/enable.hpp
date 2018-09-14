#ifndef ZOAL_ARCH_ENABLE_HPP
#define ZOAL_ARCH_ENABLE_HPP

namespace zoal { namespace arch {
    template<class Module, class... Modules>
    class enable {
    private:
        using next = enable<Modules...>;

    public:
        enable() = delete;

        static void on() {
            Module::enable();
            next::on();
        }

        static void off() {
            Module::disable();
            next::off();
        }
    };

    template<class Module>
    class enable<Module> {
    public:
        enable() = delete;

        static void on() {
            Module::enable();
        }

        static void off() {
            Module::disable();
        }
    };
}}

#endif
