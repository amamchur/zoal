#ifndef ZOAL_ARCH_POWER_HPP
#define ZOAL_ARCH_POWER_HPP

namespace zoal { namespace arch {
    template<class Module, class... Modules>
    class power {
    private:
        using next = power<Modules...>;

    public:
        power() = delete;

        static void on() {
            Module::power_on();
            next::on();
        }

        static void off() {
            Module::power_off();
            next::off();
        }
    };

    template<class Module>
    class power<Module> {
    public:
        power() = delete;

        static void on() {
            Module::power_on();
        }

        static void off() {
            Module::power_off();
        }
    };
}}

#endif
