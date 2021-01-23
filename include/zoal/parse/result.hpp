#pragma once

#ifndef ZOAL_PARSER_RESULT_HPP
#define ZOAL_PARSER_RESULT_HPP

namespace zoal { namespace parser {
    enum class result {
        unknown = -1,
        done = 0,
        end_of_stream,
        error
    };
}}

#endif
