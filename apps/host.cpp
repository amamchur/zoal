#include "../misc/type_detector.hpp"

#include <iostream>
#include <string>
#include <zoal/arch/bus.hpp>

using namespace std;

void callback(zoal::misc::type_detector_machine *pm, zoal::misc::value_type t) {
    std::cout << std::string(pm->token_start(), pm->token_end()) << ": " << (int)t << std::endl;
}

enum class clock_source { hsi, hse, lsi, lse, pll_clk };

template<class C, clock_source src>
struct pll_freq_calc {};

template<class C>
struct pll_freq_calc<C, clock_source::hse> {
    static constexpr uint32_t value = C::hse_freq / C::hse_pll_prescaler * C::pll_mul;
};

template<class C>
struct pll_freq_calc<C, clock_source::hsi> {
    static constexpr uint32_t value = C::hsi_freq / C::hsi_pll_prescaler * C::pll_mul;
};

template<clock_source src, uint32_t hsi_freq, uint32_t hse_freq, uint32_t pll_freq>
struct sys_clock_mux {};

template<uint32_t hsi_freq, uint32_t hse_freq, uint32_t pll_freq>
struct sys_clock_mux<clock_source::hsi, hsi_freq, hse_freq, pll_freq> {
    static constexpr uint32_t value = hsi_freq;
};

template<uint32_t hsi_freq, uint32_t hse_freq, uint32_t pll_freq>
struct sys_clock_mux<clock_source::hse, hsi_freq, hse_freq, pll_freq> {
    static constexpr uint32_t value = hse_freq;
};

template<uint32_t hsi_freq, uint32_t hse_freq, uint32_t pll_freq>
struct sys_clock_mux<clock_source::pll_clk, hsi_freq, hse_freq, pll_freq> {
    static constexpr uint32_t value = pll_freq;
};

template<class C>
struct freq_calc {
    static constexpr uint32_t pll_freq = pll_freq_calc<C, C::pll_mux_src>::value;
    static constexpr uint32_t hsi_freq = C::hsi_freq;
    static constexpr uint32_t hse_freq = C::hsi_freq;
    static constexpr uint32_t lsi_freq = C::hsi_freq;
    static constexpr uint32_t lse_freq = C::lse_freq;
    static constexpr uint32_t sys_freq = sys_clock_mux<C::sys_clock_src, hsi_freq, hse_freq, pll_freq>::value;
    static constexpr uint32_t ahb_freq = sys_freq / C::ahb_prescaler;
    static constexpr uint32_t apb1_freq = sys_freq / C::apb1_prescaler;
    static constexpr uint32_t apb2_freq = sys_freq / C::apb2_prescaler;
};

template<class Calc, zoal::arch::bus Type>
struct bus_freq {};

template<class Calc>
struct bus_freq<Calc, zoal::arch::bus::cortex_ahb> {
    static constexpr uint32_t value = Calc::ahb_freq;
};

template<class Calc>
struct bus_freq<Calc, zoal::arch::bus::cortex_apb1> {
    static constexpr uint32_t value = Calc::apb1_freq;
};

template<class Calc>
struct bus_freq<Calc, zoal::arch::bus::cortex_apb2> {
    static constexpr uint32_t value = Calc::apb2_freq;
};

class clock_config {
public:
    // High speed internal clock signal
    static constexpr uint32_t hsi_freq = 8000000;

    // High speed external clock signal
    static constexpr uint32_t hse_freq = 8000000;

    // Low speed internal clock signal
    static constexpr uint32_t lsi_freq = 40000;

    // Low speed external clock signal
    static constexpr uint32_t lse_freq = 32768;

    static constexpr uint32_t hsi_pll_prescaler = 2;
    static constexpr uint32_t hse_pll_prescaler = 1;
    static constexpr uint32_t ahb_prescaler = 1;
    static constexpr uint32_t apb1_prescaler = 2;
    static constexpr uint32_t apb2_prescaler = 1;

    static constexpr clock_source pll_mux_src = clock_source::hse;
    static constexpr uint32_t pll_mul = 9;
    static constexpr clock_source sys_clock_src = clock_source::pll_clk;
};

using calc = freq_calc<clock_config>;

int main() {
    std::cout << "PLL Clock: " << calc::pll_freq << std::endl;
    std::cout << "Sys Clock: " << calc::sys_freq << std::endl;
    std::cout << "AHB Bus: " << bus_freq<calc, zoal::arch::bus::cortex_ahb>::value << std::endl;
    std::cout << "APB1 Bus: " << bus_freq<calc, zoal::arch::bus::cortex_apb1>::value << std::endl;
    std::cout << "APB2 Bus: " << bus_freq<calc, zoal::arch::bus::cortex_apb2>::value << std::endl;

    return 0;
}
