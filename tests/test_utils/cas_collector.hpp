#ifndef ZOAL_CAS_COLLECTOR_HPP
#define ZOAL_CAS_COLLECTOR_HPP

#include "gtest/gtest.h"

namespace zoal { namespace tests {
    class cas_value {
    public:
        cas_value() = default;
        cas_value(uintptr_t a, uintptr_t c, uintptr_t s, size_t rs)
            : address(a)
            , clear(c)
            , set(s)
            , reg_size(rs) {}

        bool operator==(const cas_value &v) const {
            return address == v.address && clear == v.clear && set == v.set && reg_size == v.reg_size;
        }

        std::string str() const {
            std::stringstream ss;
            auto sw = reg_size * 2;
            const char *type_cast;
            switch (reg_size) {
            case 1:
                type_cast = "(uint8_t *)";
                break;
            case 2:
                type_cast = "(uint16_t *)";
                break;
            case 4:
                type_cast = "(uint32_t *)";
                break;
            case 8:
                type_cast = "(uint64_t *)";
                break;
            default:
                type_cast = "(?)";
                break;
            }
            ss << "*" << type_cast << "0x" << std::setfill('0') << std::setw(sw) << std::hex << address << " =";
            ss << " (" << type_cast << "0x" << std::setfill('0') << std::setw(sw) << std::hex << address;
            ss << " & ~0x" << std::setfill('0') << std::setw(sw) << std::hex << clear << ") ";
            ss << "| 0x" << std::setfill('0') << std::setw(sw) << std::hex << set << std::endl;
            return ss.str();
        }

        uintptr_t address{0};
        uintptr_t clear{0};
        uintptr_t set{0};
        size_t reg_size{0};
    };

    class cas_collector {
    public:
        template<class A>
        void operator()() {
            values.push_back(cas_value(A::address, A::clear, A::set, sizeof(typename A::type)));
        }

        bool operator==(const cas_collector &c) const {
            bool result = values.size() == c.values.size();
            auto i1 = values.begin();
            auto i2 = c.values.begin();
            for (; result && i1 != values.end(); ++i1, ++i2) {
                result = *i1 == *i2;
            }
            return result;
        }

        std::string str() const {
            std::stringstream ss;
            ss << "size: " << values.size();
            for (auto iter = values.begin(); iter != values.end(); ++iter) {
                ss << ", " << iter->str();
            }

            return ss.str();
        }

        std::vector<cas_value> values;
    };
}}

namespace testing { namespace internal {
    template<>
    class FormatForComparison<zoal::tests::cas_value, zoal::tests::cas_value> {
    public:
        static ::std::string Format(const zoal::tests::cas_value &value) {
            return value.str();
        }
    };

    template<>
    class FormatForComparison<zoal::tests::cas_collector, zoal::tests::cas_collector> {
    public:
        static ::std::string Format(const zoal::tests::cas_collector &value) {
            return value.str();
        }
    };
}}

#endif
