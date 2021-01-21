#ifndef ZOAL_CAS_COLLECTOR_HPP
#define ZOAL_CAS_COLLECTOR_HPP

#include "gtest/gtest.h"

class cas_value {
public:
    cas_value() = default;
    cas_value(uintptr_t a, uintptr_t c, uintptr_t s)
        : address(a)
        , clear(c)
        , set(s) {}

    bool operator==(const cas_value &v) const {
        return address == v.address && clear == v.clear && set == v.set;
    }

    uintptr_t address{0};
    uintptr_t clear{0};
    uintptr_t set{0};
};

namespace testing { namespace internal {
    template<>
    class FormatForComparison<cas_value, cas_value> {
    public:
        static ::std::string Format(const cas_value &value) {
            std::stringstream ss;
            ss << "A: 0x" << std::setfill('0') << std::setw(16) << std::hex << value.address << " ";
            ss << "C: 0x" << std::setfill('0') << std::setw(16) << std::hex << value.clear << " ";
            ss << "S: 0x" << std::setfill('0') << std::setw(16) << std::hex << value.set << std::endl;
            return ss.str();
        }
    };
}}

class cas_collector {
public:
    template<class A>
    void operator()() {
        values.push_back(cas_value(A::address, A::clear, A::set));
    }

    std::vector<cas_value> values;
};

#endif
