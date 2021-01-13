#ifndef ZOAL_TYPE_DETECTOR_HPP
#define ZOAL_TYPE_DETECTOR_HPP

#include <stddef.h>
#include <stdint.h>
#include <zoal/parser/ragel_scanner.hpp>

namespace zoal { namespace misc {
    enum class value_type { unknown, time };

    class type_detector_machine : public zoal::parser::scanner_machine<type_detector_machine *, value_type> {
    public:
        void init_machine();
        int start_state() const;

        const char *run_machine(const char *p, const char *pe);
    };

    using type_detector = zoal::parser::ragel_scanner<type_detector_machine>;
}}

#endif
