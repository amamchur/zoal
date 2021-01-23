#ifndef ZOAL_TYPE_DETECTOR_HPP
#define ZOAL_TYPE_DETECTOR_HPP

#include <stddef.h>
#include <stdint.h>
#include <zoal/parse/ragel_scanner.hpp>

namespace zoal { namespace misc {
    enum class value_type { unknown, date_time, date, time };

    class type_detector_machine : public zoal::parse::scanner_machine<type_detector_machine *, value_type> {
    public:
        void init_machine();
        int start_state() const;

        const char *run_machine(const char *p, const char *pe);
    };

    using type_detector = zoal::parse::ragel_scanner<type_detector_machine>;

    class type_detector_v2 : protected zoal::parse::ragel_scanner<type_detector_machine> {
    public:
        type_detector_v2()
            : zoal::parse::ragel_scanner<type_detector_machine>(nullptr, 0) {}

        const char* detect(const void *start, const void *end, const void *eof = nullptr) {
            this->type_ = value_type::unknown;
            this->callback(&type_callback);
            this->scan(start, end, eof);
            return this->pos_;
        }

        inline const char *pos() const{
            return pos_;
        }

        inline value_type type() const{
            return type_;
        }

    private:
        static void type_callback(zoal::misc::type_detector_machine *pm, zoal::misc::value_type t) {
            auto *obj = reinterpret_cast<type_detector_v2 *>(pm);
            obj->pos_ = pm->token_start();
            obj->type_ = t;
        }

        const char *pos_{nullptr};
        value_type type_{value_type::unknown};
    };
}}

#endif
